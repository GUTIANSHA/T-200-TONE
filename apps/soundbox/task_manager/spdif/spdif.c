
/*************************************************************
   此文件函数主要是spdif模式按键处理和事件处理

	void app_spdif_task()
   spdif模式主函数

	static int spdif_sys_event_handler(struct sys_event *event)
    spdif模式系统事件所有处理入口

	static void spdif_task_close(void)
	spdif模式退出

**************************************************************/


#include "app_config.h"
#include "key_event_deal.h"
#include "system/includes.h"
#include "tone_player.h"
#include "app_task.h"
#include "tone_player.h"
#include "media/includes.h"
#include "asm/audio_spdif.h"
#include "clock_cfg.h"

#include "ui/ui_api.h"
#include "ui_manage.h"


#if TCFG_APP_SPDIF_EN
#include "user_cfg.h"
extern struct audio_spdif_hdl spdif_slave_hdl;
/* #define LOG_TAG_CONST    APP_SPDIF */
#define LOG_TAG             "[APP_SPDIF]"
#define LOG_ERROR_ENABLE
#define LOG_DEBUG_ENABLE
#define LOG_INFO_ENABLE
/* #define LOG_DUMP_ENABLE */
#define LOG_CLI_ENABLE
#include "debug.h"

static u8 source = 0;

extern void hdmi_cec_close(void);
extern int hdmi_dcc_dump_edid_info();

extern void spdif_dec_init(void);
extern void spdif_dec_close(void);

///*----------------------------------------------------------------------------*/
/**@brief    spdif 硬件设置
   @param    无
   @return   无
   @note
*/
/*----------------------------------------------------------------------------*/
static void spdif_open(void)
{
    app_audio_set_volume(APP_AUDIO_STATE_MUSIC, use_ctl.led_vol, 1);

#if	TCFG_SPDIF_OUTPUT_ENABLE
    spdif_slave_hdl.output_port = SPDIF_OUT_PORT_A;//PB11
#endif

    #ifdef SUPPORT_USER_SPDIF_MODE
    #ifdef 	SUPPORT_SPDIF_OPT_MODE
    if (use_ctl.bSpdifMode==SPDIF_OPT)
    {
        spdif_slave_hdl.input_port  = SUPPORT_SPDIF_OPT_IN_PORT;// opt	
        #ifdef SUPPORT_VM_SAVE_WORK_MODE
        vm_save_work_mode(WORK_OPT_MODE);
        printf(">>>>>>>>work  in  opt\n");
        #endif
    }
    #endif
    #ifdef 	SUPPORT_SPDIF_COAX_MODE
    if (use_ctl.bSpdifMode==SPDIF_COAX)
    {
        spdif_slave_hdl.input_port  = SUPPORT_SPDIF_COAX_IN_PORT;// COAX
        #ifdef SUPPORT_VM_SAVE_WORK_MODE
        vm_save_work_mode(WORK_COAX_MODE);
        printf(">>>>>>>>work  in  coax\n");
        #endif
    }
    #endif
    #ifdef 	SUPPORT_SPDIF_HDMI_MODE
    if (use_ctl.bSpdifMode==SPDIF_HDMI)
    {
        printf(">>>>>>>>work  in  hdmi\n");
        spdif_slave_hdl.input_port  = SUPPORT_SPDIF_HDMI_IN_PORT;// HDMI
        #ifdef SUPPORT_VM_SAVE_WORK_MODE
        vm_save_work_mode(WORK_HDMI_MODE);
        #endif
    }
    #endif
    #else
    spdif_slave_hdl.input_port  = SPDIF_IN_PORT_B;
    #endif
    delay_2ms(100);//延时等待偏置电压稳定

    #ifdef SUPPORT_SPDIF_USE_MUSIC_PP_KEY
    use_ctl.spdif_pp_flag = 0;
    #endif

#ifndef SUPPORT_HDMI_CH_DELAY_OPEN
    audio_spdif_slave_open(&spdif_slave_hdl);
    audio_spdif_slave_start(&spdif_slave_hdl);
#endif

    pa_unmute();
#if TCFG_HDMI_ARC_ENABLE
    #ifdef 	SUPPORT_SPDIF_HDMI_MODE
    if (use_ctl.bSpdifMode==SPDIF_HDMI)
    {
        if (!use_ctl.hdmi_cec_init_flag)
        {
            hdmi_dcc_dump_edid_info();
            hdmi_cec_init();
        }
    }
    else
    {
#ifdef SUPPORT_HDMI_CH_DELAY_OPEN
        audio_spdif_slave_open(&spdif_slave_hdl);
        audio_spdif_slave_start(&spdif_slave_hdl);
#endif
        if(use_ctl.hdmi_cec_init_flag)
        {
            hdmi_cec_close();
		}
    }
    #endif
#endif
}


///*----------------------------------------------------------------------------*/
/**@brief    spdif 模式初始化
   @param    无
   @return   无
   @note
*/
/*----------------------------------------------------------------------------*/
static void spdif_app_init(void)
{

    #ifdef SUPPORT_SPDIF_POWER_CONTROL
    SPDIF_POWER_ON();
    #endif

    ui_update_status(STATUS_SPDIF_MODE);
    clock_idle(SPDIF_IDLE_CLOCK);
    UI_SHOW_WINDOW(ID_WINDOW_SPDIF);
    UI_SHOW_MENU(MENU_SPDIF, 0, 0, NULL);	
    sys_key_event_enable();
    #ifndef SUPPORT_SPDIF_MODE_TONE
    #endif


}


//*----------------------------------------------------------------------------*/
/**@brief    spdif 模式退出
   @param    无
   @return   无
   @note
*/
/*----------------------------------------------------------------------------*/
static void spdif_task_close(void)
{
    UI_HIDE_CURR_WINDOW();


    audio_spdif_slave_close(&spdif_slave_hdl);
    log_info("APP_SPDIF_STOP1\n");
    spdif_dec_close();


#if TCFG_HDMI_ARC_ENABLE
#ifdef 	SUPPORT_SPDIF_HDMI_MODE
    if (use_ctl.hdmi_cec_init_flag)
    {
        extern void hdmi_cec_close(void);
        hdmi_cec_close();
    }
#endif
#endif


#ifdef SUPPORT_SPDIF_POWER_CONTROL
    SPDIF_POWER_OFF();
#endif

}


static void  spdif_tone_play_end_callback(void *priv, int flag)
{
	u32 index = (u32)priv;

	if (APP_SPDIF_TASK != app_get_curr_task()) 
    {
		log_error("tone callback task out \n");
		return;
	}
    printf(">>>>>>>>>>>>callback\n");

    spdif_dec_init();
    spdif_open();


	
}
void user_spidf_mode_tone_play(void)
{
    u8 ret = 1;
    #ifdef SUPPORT_CHANGE_MODE_LED7_SHOW_BUG
    use_ctl.vChangeModeCnt=0;
    #endif
    audio_dac_mute_ch(BIT(0)|BIT(1),0);
    pa_unmute();
#ifdef SUPPORT_SPDIF_MODE_TONE
    #ifdef SUPPORT_SPDIF_OPT_MODE
	if(use_ctl.bSpdifMode==SPDIF_OPT)   
	{
        printf(">>>>enter opt tone\n");
		ret=tone_play_with_callback_by_name(tone_table[IDEX_TONE_OPT], 1, spdif_tone_play_end_callback, (void *)IDEX_TONE_OPT);
	}
	#endif
	#ifdef SUPPORT_SPDIF_COAX_MODE
	if(use_ctl.bSpdifMode==SPDIF_COAX)   
	{
        printf(">>>>enter coax tone\n");
		ret=tone_play_with_callback_by_name(tone_table[IDEX_TONE_COAX], 1, spdif_tone_play_end_callback, (void *)IDEX_TONE_COAX);
	}
	#endif
	#ifdef SUPPORT_SPDIF_HDMI_MODE
	if(use_ctl.bSpdifMode==SPDIF_HDMI)
	{
		ret=tone_play_with_callback_by_name(tone_table[IDEX_TONE_NULL], 1, spdif_tone_play_end_callback, (void *)IDEX_TONE_NULL);
	}
	#endif
    if (ret)
#endif
    {
        printf("tone_play_error!!!!!!!");
	    spdif_dec_init();
        spdif_open();
    }
}


void user_switch_spdif_input_port(void)
{
#if defined SUPPORT_USER_SPDIF_MODE
    printf("\n--func=%s\n", __FUNCTION__);

#if TCFG_HDMI_ARC_ENABLE

#ifdef 	SUPPORT_SPDIF_HDMI_MODE
    if (use_ctl.hdmi_cec_init_flag)
    {
        hdmi_cec_close();
    }
#endif

#endif

    audio_spdif_slave_close(&spdif_slave_hdl);
	spdif_dec_close();
    delay_2ms(250);//延时等待偏置电压稳定

    UI_SHOW_MENU(MENU_SPDIF, 0, 0, NULL);
    app_audio_set_volume(APP_AUDIO_STATE_MUSIC, use_ctl.led_vol, 1);

    user_spidf_mode_tone_play();
    return;

#endif
}

//*----------------------------------------------------------------------------*/
/**@brief    spdif 输入io口设置
   @param    无
   @return   无
   @note
*/
/*----------------------------------------------------------------------------*/
void switch_spdif_input_port(void)
{

#ifdef SUPPORT_USER_SPDIF_MODE
    user_switch_spdif_input_port();		 	 
    return;
#else	

    source++;
    if (source > 2) {
        source = 0;
    }

    switch (source) {
    case 0:
        audio_spdif_slave_switch_port(&spdif_slave_hdl, SPDIF_IN_PORT_A);
        break;
    case 1:
        audio_spdif_slave_switch_port(&spdif_slave_hdl, SPDIF_IN_PORT_C);
        break;
    case 2:
        audio_spdif_slave_switch_port(&spdif_slave_hdl, SPDIF_IN_PORT_D);
        break;
    default:
        break;
    }
	
#endif	
	
}

//*----------------------------------------------------------------------------*/
/**@brief    spdif 按键消息入口
   @param    无
   @return   1、消息已经处理，不需要发送到common  0、消息发送到common处理
   @note
*/
/*----------------------------------------------------------------------------*/
static int spdif_key_event_opr(struct sys_event *event)
{
    int ret = true;
    int err = 0;

    int key_event = event->u.key.event;
    int key_value = event->u.key.value;

    log_info("key_event:%d \n", key_event);

    switch (key_event) 
    {
    case KEY_SPDIF_SW_SOURCE:
        log_info("KEY_SPDIF_SW_SOURCE \n");
        switch_spdif_input_port();
        break;
#ifdef SUPPORT_SPDIF_USE_MUSIC_PP_KEY
	case KEY_MUSIC_PP:
        delay_2ms(50);//延时等待偏置电压稳定
        if(use_ctl.spdif_pp_flag == 0)
        {
            audio_spdif_slave_close(&spdif_slave_hdl);
            use_ctl.spdif_pp_flag = 1;
        }
        else
        {
            use_ctl.spdif_pp_flag = 0;
            audio_spdif_slave_open(&spdif_slave_hdl);
            audio_spdif_slave_start(&spdif_slave_hdl);
        }
        break;
#endif
#if TCFG_SPEED_PITCH_ENABLE
#ifdef OFF_SPDIF_PITCH
    case KEY_PITCH_RESET:		
    case KEY_PITCH_UP:
    case KEY_PITCH_DN:		
        break;

#else

    case KEY_PITCH_RESET:		
	     use_ctl.bPitchVal=SPEED_PITCH_MAX/2;
 	     spdif_audio_file_set_pitch(use_ctl.bPitchVal);	
	 break;		 
    case KEY_PITCH_UP:
          if(use_ctl.bPitchVal<SPEED_PITCH_MAX)
	     use_ctl.bPitchVal++;	 
	  spdif_audio_file_set_pitch(use_ctl.bPitchVal);
    break;	
    case KEY_PITCH_DN:		
       if(use_ctl.bPitchVal>0)
	     use_ctl.bPitchVal--;
	  spdif_audio_file_set_pitch(use_ctl.bPitchVal);
    break;
#endif
#endif
    default :
        ret = false;
        break;
    }
    return ret;
}

//*----------------------------------------------------------------------------*/
/**@brief    spdif 模式活跃状态 所有消息入口
   @param    无
   @return   1、当前消息已经处理，不需要发送comomon 0、当前消息不是linein处理的，发送到common统一处理
   @note
*/
/*----------------------------------------------------------------------------*/
static int spdif_sys_event_handler(struct sys_event *event)
{
    switch (event->type)
    {
        case SYS_KEY_EVENT:
            return spdif_key_event_opr(event);
        case SYS_DEVICE_EVENT:
            return false;
        default:
            return false;
    }
    return false;
}



#ifdef SUPPORT_SPDIF_SCAN

void app_spdif_scan()
{
#ifdef SUPPORT_HDMI_CH_DELAY_OPEN
    if (use_ctl.bSpdifMode==SPDIF_HDMI)
    {
        static u8 prev_ok_flag = 0;
        if (prev_ok_flag != get_cec_ok_flag())
        {
#ifdef SUPPORT_SPDIF_HDMI_CHECK
            if (get_cec_ok_flag() && get_hdmi_online())
#else
            if (get_cec_ok_flag())
#endif
            {
                audio_spdif_slave_open(&spdif_slave_hdl);
                audio_spdif_slave_start(&spdif_slave_hdl);
            }
        else
        {
            audio_spdif_slave_close(&spdif_slave_hdl);
            delay_2ms(50);//延时等待偏置电压稳定
        }
            prev_ok_flag = get_cec_ok_flag(); 
            
        }
    }
#endif
}

void spdif_scan_enable(void) 
{
    if (!user_timer.spdif_scan_id)
    {
        user_timer.spdif_scan_id = sys_timer_add(NULL,app_spdif_scan,500);
        puts("spdif_scan_enable\n");
    }
}

void spdif_scan_disable(void) 
{
    if (user_timer.spdif_scan_id)
    {
        sys_timer_del(user_timer.spdif_scan_id);
        user_timer.spdif_scan_id = 0;
        puts("spdif_scan_disable\n");
    }

}
#endif



//*----------------------------------------------------------------------------*/
/**@brief    spdif 主任务
   @param    无
   @return   无
   @note
*/
/*----------------------------------------------------------------------------*/
void app_spdif_task()
{
    int res;
    int msg[32];


    if(use_ctl.power_work_init)
    {
        if(use_ctl.standby_out_to_spdif)
        {
                
        }
        else if(use_ctl.is_forward)
            use_ctl.bSpdifMode = 0;
        else
        {
            if(get_hdmi_online())
                use_ctl.bSpdifMode = MAX_SPDIF_MODE - 1;
            else
                use_ctl.bSpdifMode = MAX_SPDIF_MODE - 2;
        }
    }

    use_ctl.power_work_init = 1;

    spdif_app_init();
    user_spidf_mode_tone_play();

#ifdef SUPPORT_SPDIF_SCAN
    spdif_scan_enable();
#endif
    while (1) 
    {
        app_task_get_msg(msg, ARRAY_SIZE(msg), 1);

        switch (msg[0]) 
        {
        case APP_MSG_SYS_EVENT:
            if (spdif_sys_event_handler((struct sys_event *)(&msg[1])) == false) 
            {
                app_default_event_deal((struct sys_event *)(&msg[1]));    //由common统一处理
            }
            break;
        default:
            break;
        }

        if (app_task_exitting()) 
        {
            spdif_task_close();
            #ifdef SUPPORT_SPDIF_SCAN
            spdif_scan_disable();
            #endif
            return;
        }
    }
}





#else
void app_spdif_task()
{

}




#endif
