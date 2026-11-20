#include "system/includes.h"
#include "media/includes.h"
#include "app_config.h"
#include "tone_player.h"
#include "asm/charge.h"
#include "app_charge.h"
#include "app_main.h"
#include "ui_manage.h"
#include "vm.h"
#include "app_chargestore.h"
#include "user_cfg.h"
#include "ui/ui_api.h"
#include "app_task.h"
#include "key_event_deal.h"
#include "clock_cfg.h"


#define LOG_TAG_CONST       APP_IDLE
#define LOG_TAG             "[APP_IDLE]"
#define LOG_ERROR_ENABLE
#define LOG_DEBUG_ENABLE
#define LOG_INFO_ENABLE
/* #define LOG_DUMP_ENABLE */
#define LOG_CLI_ENABLE
#include "debug.h"


static void  lcd_ui_power_on_timeout(void *p)
{
#if (TCFG_SPI_LCD_ENABLE)
    /* sys_key_event_enable(); */
    /* logo_time = timer_get_ms(); */
    /* while (timer_get_ms() - logo_time <= 2 * 1000) { //显示开机logo */
    /* os_time_dly(10); */
    /* } */

    UI_HIDE_WINDOW(ID_WINDOW_POWER_ON);
    UI_SHOW_WINDOW(ID_WINDOW_MAIN);
#if TCFG_APP_BT_EN
    app_task_switch_to(APP_BT_TASK);
#else
    app_task_switch_to(APP_MUSIC_TASK);
#endif
#endif

}

static void  lcd_ui_power_on()
{
#if (TCFG_SPI_LCD_ENABLE)
    int logo_time = 0;
    UI_SHOW_WINDOW(ID_WINDOW_POWER_ON);
    sys_timeout_add(NULL, lcd_ui_power_on_timeout, 1000);
#endif
}


extern void user_scan(void);
extern void user_init(void);

void task_init(void)
{
	use_ctl.bVoiceVal=VOICE_DEFAULT;
	use_ctl.standby_out_to_spdif = 0;
	use_ctl.flag_10s = 0;
	tone_table_switch(use_ctl.bVoiceVal);
    u8 ret;

#ifdef SUPPORT_VM_SAVE_WORK_MODE
#ifndef SUPPORT_VM_SAVE_WORK_MODE_READ_MUSIC_DEV
    if (poweron_goto_vm_work_mode() == true)
    {
        return;
    }
#endif
#endif


    #ifdef SW_ONOFF_DET
	    SW_DET_IO_INIT();
	    if(SW_IS_OFF()){
              use_ctl.sw_off = true;			
	       app_task_switch_to(APP_IDLE_TASK);
	       return;
	    }
    #endif
	
	ret = music_app_check();
	printf("\n music device  == %d\n",ret);
	
	if(ret)
	{
		app_task_switch_to(APP_MUSIC_TASK);
	}
	else
	{
#ifdef SUPPORT_POWER_ON_TO_AUX
		app_task_switch_to(APP_LINEIN_TASK);
#elif defined SUPPORT_POWER_ON_TO_FM
		app_task_switch_to(APP_FM_TASK);
#else
	 #ifdef SUPPORT_POWER_ON_FIRST_DISABLE_BT_VOICE
	 	  use_ctl.bPowerOnDisableBtVoiceCnt = 7;		
	 #endif
		app_task_switch_to(APP_SPDIF_TASK);
#endif
	}

}

#ifdef SUPPORT_VM_MAIN
#if SUPPORT_USER_EQ_MODE
	const char t_table[TRE_MAX+1]={TRE_TABLE};
	const char b_table[BASS_MAX+1]={BASS_TABLE};
	const char m_table[MID_MAX+1]={MID_TABLE};
	const char eq50hz_table[MENU_EQ_MAX_VAL+1]={MENU_EQ_TABLE};
#endif

void power_on_vm_deal(){
	u8 ret;
	s8 music_volume;
#ifdef SUPPORT_VM_BASS
	ret = syscfg_read(VM_BASS, &music_volume, 1);
	if(ret==1){
		use_ctl.bass = music_volume;
	}else{
		use_ctl.bass = BASS_DEFAULT_VAL;
	}
	user_define_eq_update(EQ_POS_BASS,b_table[use_ctl.bass]);
#endif
#ifdef SUPPORT_VM_TREB
	ret = syscfg_read(VM_TREB, &music_volume, 1);
	if(ret==1){
		use_ctl.tre = music_volume;
	}else{
		use_ctl.tre = TRE_DEFAULT_VAL;
	}
	user_define_eq_update(EQ_POS_TRE,t_table[use_ctl.tre]);
#endif
#ifdef SUPPORT_VM_VOL
	ret = syscfg_read(VM_MUSICVOL, &music_volume, 1);
	if(ret==1)
	{
		use_ctl.led_vol = music_volume;
		printf(">>>>>>>>>>>>>>>music:%d\n",music_volume);
	}
	else
	{
		use_ctl.led_vol = LED_VOL_DETAULT_VAL;
	}
	app_audio_set_volume(APP_AUDIO_CURRENT_STATE, use_ctl.led_vol, 1);
#endif

#ifdef SUPPORT_VM_EQ50HZ
	ret = syscfg_read(VM_EQ50HZ, &music_volume, 1);
	if(ret==1){
		use_ctl.eq50hz_val = music_volume;
	}else{
		use_ctl.eq50hz_val = MENU_EQ_DEFAULT_VOL;
	}
	user_define_eq_update(EQ_POS_50HZ,eq50hz_table[use_ctl.eq50hz_val]);
#endif
#ifdef SUPPORT_VM_EQ6KHZ
	ret = syscfg_read(VM_EQ6KHZ, &music_volume, 1);
	if(ret==1){
		use_ctl.eq6khz_val = music_volume;
	}else{
		use_ctl.eq6khz_val = MENU_EQ_DEFAULT_VOL;
	}
	user_define_eq_update(EQ_POS_6KHZ,eq50hz_table[use_ctl.eq6khz_val]);
#endif

#ifdef SUPPORT_VM_MICVOL
	ret = syscfg_read(VM_MICVOL, &music_volume, 1);
	if(ret==1){
		use_ctl.mic_vol = music_volume;
	}else{
		use_ctl.mic_vol = MIC_VOL_DEFAULT_VAL;
	}
#endif
#ifdef SUPPORT_VM_ECHO
	ret = syscfg_read(VM_ECHO, &music_volume, 1);
	if(ret==1){
		use_ctl.echo = music_volume;
	}else{
		use_ctl.echo = MIC_ECHO_DEFAULT_VAL;
	}
#endif
#ifdef SUPPORT_VM_MICBASS
	ret = syscfg_read(VM_MIC_BASS, &music_volume, 1);
	if(ret==1){
		use_ctl.mic_bass = music_volume;
	}else{
		use_ctl.mic_bass = MIC_BASS_DEFAULT_VAL;
	}
#endif
#ifdef SUPPORT_VM_MICTRE
	ret = syscfg_read(VM_MIC_TRE, &music_volume, 1);
	if(ret==1){
		use_ctl.mic_tre = music_volume;
	}else{
		use_ctl.mic_tre = MIC_TRE_DEFAULT_VAL;
	}
#endif

}

#endif

static int power_on_init(void)
{
    ///有些需要在开机提示完成之后再初始化的东西， 可以在这里初始化
#if (TCFG_SPI_LCD_ENABLE)
    lcd_ui_power_on();//由ui决定切换的模式
    return 0;
#endif

	audio_dac_mute_ch(BIT(0)|BIT(1),1);
	sys_timeout_add(NULL, task_init, 1500);

#ifdef SUPPORT_VM_MAIN
	power_on_vm_deal();
#endif

#if 0
#if TCFG_APP_BT_EN
    app_task_switch_to(APP_BT_TASK);
#else

#if TCFG_USB_APPLE_DOCK_EN //苹果iap协议使用pc模式
    app_task_switch_to(APP_PC_TASK);
#else
    app_task_switch_to(APP_SLEEP_TASK);
    /* app_task_switch_to(APP_PC_TASK); */
    /* app_task_switch_to(APP_MUSIC_TASK); */
    /* app_task_switch_to(APP_IDLE_TASK); */
    /* app_task_switch_to(APP_LINEIN_TASK);//如果带检测，设备不在线，则不跳转 */
#endif

#endif
#endif

    return 0;
}

static int power_on_unint(void)
{

    tone_play_stop();
    UI_HIDE_CURR_WINDOW();
    return 0;
}






static int poweron_sys_event_handler(struct sys_event *event)
{
    switch (event->type) 
	{
		case SYS_KEY_EVENT:
			break;
		case SYS_BT_EVENT:
			break;
		case SYS_DEVICE_EVENT:
			break;
		default:
			return false;
    }
    return false;
}


static void  tone_play_end_callback(void *priv, int flag)
{
    int index = (int)priv;

    if (APP_POWERON_TASK != app_get_curr_task()) 
	{
        log_error("tone callback task out \n");
        return;
    }
	
    
    switch (index)
	{
    case IDEX_TONE_POWER_ON:
       
        power_on_init();
        break;
    }
}


void app_poweron_task()
{
	
    int msg[32];

	//此板功放没接静音脚，PA_MUTE()/pa_mute()都是空操作，
	//真正消冲击声的是这条DAC模拟输出静音(软件静音)。
	//放在最前面(初始化任何东西之前)先把DAC输出静音掉。
	pa_mute();
	audio_dac_mute_ch(BIT(0)|BIT(1),1);

    user_init();
	delay_2ms(50);

    UI_SHOW_MENU(MENU_POWER_UP, 0, 0, NULL);
	
	delay_2ms(50);
    int err =  tone_play_with_callback_by_name(tone_table[IDEX_TONE_POWER_ON], 1, tone_play_end_callback, (void *)IDEX_TONE_POWER_ON);
	//提示音开始解码时音频通路会重新打开，DAC模拟配置可能被重写，
	//所以这里再补一次静音，保证空白提示音这一整段都是静音的(冲击声落在这里)。
	audio_dac_mute_ch(BIT(0)|BIT(1),1);
	
#if defined board_ac695x_JY_tx_2212_64led_ac6951c || defined board_ac695x_JY_tx_2310_64led_ac6951c || defined JY_tx_2208_7v4_48led_ac6951c || defined board_ac695x_JY_tx_2210_56led_ac6951c || defined board_ac695x_JY_TX_2208_48LED_AC6951C
		JL_PORTC->DIE |= BIT(4);JL_PORTC->DIR &= ~BIT(4);  JL_PORTC->PU &= ~BIT(4);JL_PORTC->PD &= ~BIT(4);JL_PORTC->OUT &= ~BIT(4);
#endif

#ifdef board_dy_DS4751C_FL872_ac6951c_6ADKEY
		JL_PORTB->DIE |= BIT(0);JL_PORTB->DIR &= ~BIT(0);  JL_PORTB->PU |= BIT(0);JL_PORTB->PD &= ~BIT(0);JL_PORTB->OUT |= BIT(0);
#endif

    sys_timer_add(NULL, user_scan, 10); //10ms

    while (1) {
        app_task_get_msg(msg, ARRAY_SIZE(msg), 1);
        switch (msg[0]) {
        case APP_MSG_SYS_EVENT:
            if (poweron_sys_event_handler((struct sys_event *)(msg + 1)) == false) {
                app_default_event_deal((struct sys_event *)(&msg[1]));    //由common统一处理
            }
            break;
        default:
            break;
        }

        if (app_task_exitting()) 
		{
            power_on_unint();
            return;
        }
    }

}

