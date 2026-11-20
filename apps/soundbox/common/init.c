
#include "app_config.h"
#include "system/includes.h"
#include "asm/charge.h"
#include "app_power_manage.h"
#include "update.h"
#include "app_main.h"
#include "app_charge.h"
#include "chgbox_ctrl.h"
#include "update_loader_download.h"

#ifdef SUPPORT_KPH_DEF
#include "asm/power/p33.h"
#include "user_cfg.h"
#endif

extern void setup_arch();
extern int audio_dec_init();
extern int audio_enc_init();
extern void audio_dac_mute_ch(u8 ch, u8 mute);

static void do_initcall()
{
    __do_initcall(initcall);
}

static void do_early_initcall()
{
    __do_initcall(early_initcall);
}

static void do_late_initcall()
{
    __do_initcall(late_initcall);
}

static void do_platform_initcall()
{
    __do_initcall(platform_initcall);
}

static void do_module_initcall()
{
    __do_initcall(module_initcall);
}

void __attribute__((weak)) board_init()
{

}
void __attribute__((weak)) board_early_init()
{

}

int eSystemConfirmStopStatus(void)
{
    /* 系统进入在未来时间里，无任务超时唤醒，可根据用户选择系统停止，或者系统定时唤醒(100ms) */
    //1:Endless Sleep
    //0:100 ms wakeup
    if (get_charge_full_flag()) {
        log_i("Endless Sleep");
        power_set_soft_poweroff();
        return 1;
    } else {
        log_i("100 ms wakeup");
        return 0;
    }

}

static void check_power_on_key(void)
{
    u32 delay_10ms_cnt = 0;
    u32 delay_10msp_cnt = 0;

    while (1) {
        clr_wdt();
        os_time_dly(2);

        extern u8 get_power_on_status(void);
        if (get_power_on_status()) {
            putchar('+');
            delay_10msp_cnt = 0;
            delay_10ms_cnt++;
            if (delay_10ms_cnt > 70) {
                return;
            }
        } else {
            putchar('-');
            delay_10ms_cnt = 0;

            delay_10msp_cnt++;
            if (delay_10msp_cnt > 20) {
                puts("enter softpoweroff\n");
                power_set_soft_poweroff();
            }
        }
    }
}

static void app_init()
{
    int update;

   
 #ifdef SUPPORT_AMP_PA_MUTE
   PA_MUTE();
 #ifdef SUPPORT_AMP_PA_MUTE_POWER_TIMER
   use_ctl.pa_delay_unmute=SUPPORT_AMP_PA_MUTE_POWER_TIMER; 
#else
   use_ctl.pa_delay_unmute=5; 
#endif
#endif

#ifdef SUPPORT_POWER_ONOFF_BAT
   BAT_IO_SET_OFF();
   use_ctl.bBatCnt=7; 
#endif


    do_early_initcall();
    do_platform_initcall();
#ifdef SUPPORT_KPH_DEF
/*
PR 口当普通IO 的使用方法
第一：RTCVDD 要外部供电：可以使用VDDIO 供电
第二：RTC 晶振使能要关掉：p33_tx_1byte(R3_OSL_CON,0);
 */
    extern void p33_tx_1byte(u16 addr, u8 data0);
   p33_tx_1byte(R3_OSL_CON, 0);
#endif


#ifdef SPI1_RGB_DEV_EN
	user_fun_init();
#endif

    board_init();

#ifdef POWER_ON_MUTE_DLY
	pa_mute();
	use_ctl.pa_delay_unmute=7;
    delay_2ms(50);   	
#endif

#ifdef SUPPORT_POWER_ON_IO_SET
	POWER_ON_IO_OFF(); // LOW
#endif

#ifdef MUTE_POWER_ON
			PA_MUTE();
#endif


    do_initcall();

    do_module_initcall();
    do_late_initcall();


    audio_enc_init();
    audio_dec_init();

    //此板功放没有接静音脚(PA_MUTE/PA_UNMUTE是空宏)，只能用DAC模拟输出静音：
    //DAC一旦初始化完就先把模拟输出静音，保证从DAC上电到播空白提示音之间不会有冲击声，
    //等空白提示音播完再在power_on里解除静音(audio_dac_mute_ch(...,0))。
    audio_dac_mute_ch(BIT(0)|BIT(1), 1);

    if (!UPDATE_SUPPORT_DEV_IS_NULL()) 
    {
        update = update_result_deal();
    }

    app_var.play_poweron_tone = 1;
	
    if (!get_charge_online_flag()) 
    {
        check_power_on_voltage();

#if TCFG_POWER_ON_NEED_KEY
        /*充电拔出,CPU软件复位, 不检测按键，直接开机*/
#if TCFG_CHARGE_OFF_POWERON_NE
        if ((!update && cpu_reset_by_soft()) || is_ldo5v_wakeup()) {
#else
        if (!update && cpu_reset_by_soft()) {
#endif
            app_var.play_poweron_tone = 0;
        } else {
            check_power_on_key();
        }
#endif
    }

#if (TCFG_MC_BIAS_AUTO_ADJUST == MC_BIAS_ADJUST_POWER_ON)
    extern u8 power_reset_src;
    u8 por_flag = 0;
    u8 cur_por_flag = 0;
    /*
     *1.update
     *2.power_on_reset(BIT0:上电复位)
     *3.pin reset(BIT4:长按复位)
     */
    if (update || (power_reset_src & BIT(0)) || (power_reset_src & BIT(4))) {
        //log_info("reset_flag:0x%x",power_reset_src);
        cur_por_flag = 0xA5;
    }
    int ret = syscfg_read(CFG_POR_FLAG, &por_flag, 1);
    if ((cur_por_flag == 0xA5) && (por_flag != cur_por_flag)) {
        //log_info("update POR flag");
        ret = syscfg_write(CFG_POR_FLAG, &cur_por_flag, 1);
    }
#endif

#if (TCFG_CHARGE_ENABLE && TCFG_CHARGE_POWERON_ENABLE)
    if (is_ldo5v_wakeup()) { //LDO5V唤醒
        extern u8 get_charge_online_flag(void);
        if (get_charge_online_flag()) { //关机时，充电插入

        } else { //关机时，充电拔出
            power_set_soft_poweroff();
        }
    }
#endif

#if(TCFG_CHARGE_BOX_ENABLE)
    /* clock_add_set(CHARGE_BOX_CLK); */
    chgbox_init_app();
#endif
}

static void app_task_handler(void *p)
{
    app_init();
	
#ifdef SUPPORT_KPH_DEF
/*
PR 口当普通IO 的使用方法
第一：RTCVDD 要外部供电：可以使用VDDIO 供电
第二：RTC 晶振使能要关掉：p33_tx_1byte(R3_OSL_CON,0);
 */
    extern void p33_tx_1byte(u16 addr, u8 data0);
   p33_tx_1byte(R3_OSL_CON, 0);
#endif
    
    app_main();
}

__attribute__((used)) int *__errno()
{
    static int err;
    return &err;
}

int main()
{


#if(CONFIG_CPU_BR25)

#if (TCFG_DEC2TWS_ENABLE ||RECORDER_MIX_EN || TCFG_DRC_ENABLE || TCFG_USER_BLE_ENABLE || TCFG_DEC_APE_ENABLE || TCFG_DEC_FLAC_ENABLE || TCFG_DEC_DTS_ENABLE || TCFG_USER_EMITTER_ENABLE)
    clock_set_sfc_max_freq(100 * 1000000, 100 * 1000000);
#else

#if ((TCFG_AEC_ENABLE) && (TCFG_USER_TWS_ENABLE))
    clock_set_sfc_max_freq(80 * 1000000, 80 * 1000000);
#else
    clock_set_sfc_max_freq(64 * 1000000, 64 * 1000000);
#endif

#endif

#endif

#if defined  board_LH_808_20led_ac6951c  ||  defined  board_DG_1263_2led_ac6951c
		JL_PORTA->DIE |= BIT(3);JL_PORTA->DIR &= ~BIT(3);JL_PORTA->OUT |= BIT(3);JL_PORTA->PD &= ~BIT(3);JL_PORTA->PU &= ~BIT(3);
#endif

    clock_set_sfc_max_freq(100 * 1000000, 100 * 1000000);
    wdt_close();

    os_init();
#if defined  board_LH_808_20led_ac6951c  ||  defined  board_DG_1263_2led_ac6951c
				JL_PORTA->DIE |= BIT(3);JL_PORTA->DIR &= ~BIT(3);JL_PORTA->OUT |= BIT(3);JL_PORTA->PD &= ~BIT(3);JL_PORTA->PU &= ~BIT(3);
#endif
	
#ifdef MUTE_POWER_ON
	PA_MUTE();
#endif

    setup_arch();

    board_early_init();
#if defined  board_LH_808_20led_ac6951c  ||  defined  board_DG_1263_2led_ac6951c
					JL_PORTA->DIE |= BIT(3);JL_PORTA->DIR &= ~BIT(3);JL_PORTA->OUT |= BIT(3);JL_PORTA->PD &= ~BIT(3);JL_PORTA->PU &= ~BIT(3);
#endif
	
#ifdef MUTE_POWER_ON
		PA_MUTE();
#endif
PA_MUTE();
    task_create(app_task_handler, NULL, "app_core");

    os_start();

    local_irq_enable();

    while (1) 
    {
        asm("idle");
    }

    return 0;
}

