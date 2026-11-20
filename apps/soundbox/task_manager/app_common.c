#include "common/app_common.h"
#include "app_task.h"
#include "app_main.h"
#include "key_event_deal.h"
#include "music/music.h"
#include "pc/pc.h"
#include "record/record.h"
#include "linein/linein.h"
#include "fm/fm.h"
#include "btstack/avctp_user.h"
#include "app_power_manage.h"
#include "app_chargestore.h"
#include "usb/otg.h"
#include "usb/host/usb_host.h"
#include <stdlib.h>
#include "bt/bt_tws.h"
#include "audio_config.h"
#include "common/power_off.h"
#include "common/user_msg.h"
#include "audio_config.h"
#include "audio_enc.h"
#include "ui/ui_api.h"
#include "fm_emitter/fm_emitter_manage.h"
#include "common/fm_emitter_led7_ui.h"
#if TCFG_CHARGE_ENABLE
#include "app_charge.h"
#endif
#include "dev_multiplex_api.h"
#include "chgbox_ctrl.h"
#include "device/chargebox.h"
#include "app_online_cfg.h"
#include "soundcard/soundcard.h"
#include "smartbox_bt_manage.h"
#include "bt.h"
#include "common/dev_status.h"
#include "tone_player.h"
#include "ui_manage.h"
#include "soundbox.h"
#include "bt_emitter.h"

#define LOG_TAG_CONST       APP_ACTION
#define LOG_TAG             "[APP_ACTION]"
#define LOG_ERROR_ENABLE
#define LOG_DEBUG_ENABLE
#define LOG_INFO_ENABLE
/* #define LOG_DUMP_ENABLE */
#define LOG_CLI_ENABLE
#include "debug.h"


#ifdef SUPPORT_KPH_DEF
#include "user_cfg.h"
#include "mic_effect.h"
#if SUPPORT_USER_EQ_MODE
const char  tre_table[TRE_MAX+1]={TRE_TABLE};
const char  bass_table[BASS_MAX+1]={BASS_TABLE};
#ifdef EQ_MENU
const char  eq_table[MENU_EQ_MAX_VAL+1]={MENU_EQ_TABLE};
#endif
#ifdef SUPPORT_MID_SET
const char  mid_table[MID_MAX+1]={MID_TABLE};
#endif
#ifdef SUPPORT_MIC_BASS_TRE_SET
const char  mic_tre_table[MIC_TRE_MAX+1]={MIC_TRE_TABLE};
const char  mic_bass_table[MIC_BASS_MAX+1]={MIC_BASS_TABLE};
#endif
#if defined SUPPORT_CD4051_PONTEN ||defined SUPPORT_PONTEN_EQ1_EQ5
const char  mult_eq_table[AD_EQ_MAX+1]={MULT_EQ_TABLE};
#endif
#endif
#ifdef SUPPORT_MIC_ECHO_VOL_SET
const char  mic_echo_table[ECHO_MAX+1]={MIC_ECHO_TABLE};
const char  mic_vol_table[MICVOL_MAX+1]={MIC_VOL_TABLE};
#endif
#ifdef SUPPORT_MIC_DELAY_SET
const int  mic_delay_table[MIC_DELAY_MAX+1]={MIC_DELAY_TABLE};
#endif

#ifdef SUPPORT_MIC_PR_CHECK
extern u8 vol_backup_temp,vol_obj_val;
#endif		
#endif

#ifdef SUPORT_MENU_MODE

#if defined SUPPORT_MENU_VOL_TRE_BASS
const u16 menu_sel_table[2][MENU_MAX_MODE] = {
 [0] = {KEY_VOL_UP,KEY_TRE_UP,KEY_BASS_UP},
 [1] = {KEY_VOL_DOWN,KEY_TRE_DN,KEY_BASS_DN},
};
#else

const u16 menu_sel_table[2][8] = {
 [0] = {
#if defined MENU_VOL_MODE
		[MENU_VOL_MODE		]			=	KEY_VOL_UP,
#endif
#if defined MENU_TRE_MODE
		[MENU_TRE_MODE		]			=	KEY_TRE_UP,
#endif
#if defined MENU_BASS_MODE
		[MENU_BASS_MODE 	]			=	KEY_BASS_UP,
#endif
#if defined MENU_MIC_VOL_MODE
		[MENU_MIC_VOL_MODE	]			=	KEY_MIC_VOL_UP,
#endif
#if defined MENU_MIC_ECHO_MODE
		[MENU_MIC_ECHO_MODE ]			=	KEY_ECHO_UP,
#endif
#if defined MENU_MIC_DELAY_MODE
		[MENU_MIC_DELAY_MODE]			=	KEY_MIC_DELAY_UP,
#endif
#if defined MENU_MIC_TRE_MODE
		[MENU_MIC_TRE_MODE	]			=	KEY_MIC_TRE_UP,
#endif
#if defined MENU_MIC_BASS_MODE
		[MENU_MIC_BASS_MODE ]			=	KEY_MIC_BASS_UP,
#endif
		},
 [1] = {
#if defined MENU_VOL_MODE
		[MENU_VOL_MODE		]			=	KEY_VOL_DOWN,
#endif
#if defined MENU_TRE_MODE
		[MENU_TRE_MODE		]			=	KEY_TRE_DN,
#endif
#if defined MENU_BASS_MODE
		[MENU_BASS_MODE 	]			=	KEY_BASS_DN ,
#endif
#if defined MENU_MIC_VOL_MODE
		[MENU_MIC_VOL_MODE	]			=	KEY_MIC_VOL_DN,
#endif
#if defined MENU_MIC_ECHO_MODE
		[MENU_MIC_ECHO_MODE ]			=	KEY_ECHO_DN,
#endif
#if defined MENU_MIC_DELAY_MODE
		[MENU_MIC_DELAY_MODE]			=	KEY_MIC_DELAY_DN,
#endif
#if defined MENU_MIC_TRE_MODE
		[MENU_MIC_TRE_MODE	]			=	KEY_MIC_TRE_DN,
#endif
#if defined MENU_MIC_BASS_MODE
		[MENU_MIC_BASS_MODE ]			=	KEY_MIC_BASS_DN,
#endif
 		},
};
		
#endif
#endif


int JL_rcsp_event_handler(struct rcsp_event *rcsp);
int bt_background_event_handler(struct sys_event *event);
extern u32 timer_get_ms(void);
extern int alarm_sys_event_handler(struct sys_event *event);
extern void bt_tws_sync_volume();

static u32 input_number = 0;
static u16 input_number_timer = 0;
static void input_number_timeout(void *p)
{
    input_number_timer = 0;
    printf("input_number = %d\n", input_number);
    if (app_get_curr_task() == APP_MUSIC_TASK) 
	{
        app_task_put_key_msg(KEY_MUSIC_PLAYE_BY_DEV_FILENUM, (int)input_number);
    }
    input_number = 0;
}


static void input_number_deal(u32 num)
{
    input_number = input_number * 10 + num;
    if (input_number > 9999) 
	{
        input_number = num;
    }
    printf("num = %d, input_number = %d, input_number_timer = %d\n", num, input_number, input_number_timer);
    if (input_number_timer == 0) 
	{
        input_number_timer = sys_timeout_add(NULL, input_number_timeout, 1000);
    } 
	else 
	{
        sys_timer_modify(input_number_timer, 1000);
    }
    UI_SHOW_MENU(MENU_FILENUM, 4 * 1000, input_number, NULL);
}


int app_common_key_msg_deal(struct sys_event *event)
{
    int ret = false;
    struct key_event *key = &event->u.key;
    int key_event = event->u.key.event;
    int key_value = event->u.key.value;

    if (key_event == KEY_NULL) 
	{
        return false;
    }

    if (key_is_ui_takeover()) 
	{
        ui_key_msg_post(key_event);
        return false;
    }

#if (TCFG_UI_ENABLE && TCFG_APP_FM_EMITTER_EN)
    if (!ui_fm_emitter_common_key_msg(key_event)) 
	{
        return false;
    }
#endif

    log_info("common_key_event:%d\n", key_event);

    if ((key_event != KEY_POWEROFF) && (key_event != KEY_POWEROFF_HOLD)) 
	{
        extern u8 goto_poweroff_first_flag;
        goto_poweroff_first_flag = 0;
    }

#if (SMART_BOX_EN)
    extern bool smartbox_key_event_filter_before(int key_event);
    if (smartbox_key_event_filter_before(key_event)) {
        return true;
    }
#endif

#ifdef EQ_MENU

	if(key_event==KEY_MENU_VUP){
		switch (use_ctl.eq_menu)
		{	
			case EQ_MENU_NO:key_event = KEY_VOL_UP,use_ctl.eq_menu_cnt=6;break;
			case EQ_MENU_1:key_event = KEY_MENU_EQ1_UP,use_ctl.eq_menu_cnt=6;break;
			case EQ_MENU_2:key_event = KEY_MENU_EQ2_UP,use_ctl.eq_menu_cnt=6;break;
			default:break;
		}
	}else if(key_event==KEY_MENU_VDN){
		switch (use_ctl.eq_menu)
		{
			case EQ_MENU_NO:key_event = KEY_VOL_DOWN,use_ctl.eq_menu_cnt=6;break;
			case EQ_MENU_1:key_event = KEY_MENU_EQ1_DN,use_ctl.eq_menu_cnt=6;break;
			case EQ_MENU_2:key_event = KEY_MENU_EQ2_DN,use_ctl.eq_menu_cnt=6;break;
			default:break;
		}
	}
#endif

#ifdef SUPORT_MENU1_MODE

	if(key_event==KEY_MIC_VOL_UP){
		switch (use_ctl.menu1_mode)
		{	
			case 0: key_event = KEY_MIC_VOL_UP,use_ctl.menu1_mode_cnt=6;break;
			case 1:key_event = KEY_MIC_BASS_UP,use_ctl.menu1_mode_cnt=6;break;
			case 2:key_event = KEY_MIC_TRE_UP,use_ctl.menu1_mode_cnt=6;break;
			case 3:key_event = KEY_ECHO_UP,use_ctl.menu1_mode_cnt=6;break;
			case 4:key_event = KEY_MIC_DELAY_UP,use_ctl.menu1_mode_cnt=6;break;
			default:break;
		}
	}else if(key_event==KEY_MIC_VOL_DN){
		switch (use_ctl.menu1_mode)
		{
			case 0:key_event = KEY_MIC_VOL_DN,use_ctl.menu1_mode_cnt=6;break;
			case 1:key_event = KEY_MIC_BASS_DN,use_ctl.menu1_mode_cnt=6;break;
			case 2:key_event = KEY_MIC_TRE_DN,use_ctl.menu1_mode_cnt=6;break;
			case 3:key_event = KEY_ECHO_DN,use_ctl.menu1_mode_cnt=6;break;
			case 4:key_event = KEY_MIC_DELAY_DN,use_ctl.menu1_mode_cnt=6;break;
			default:break;
		}
	}
#endif

#ifdef TWS_SYNC_EQ
		if(((u32)event->arg==KEY_EVENT_FROM_TWS)||((u32)event->arg==SYS_BT_EVENT_FROM_TWS))
		{
#ifdef dy_DS4751C
			if((key_event==KEY_MAIN_VOL_SET)||(key_event==KEY_VOL_UP)||(key_event==KEY_VOL_DOWN))
				UI_SHOW_MENU(MENU_MAIN_VOL, 1000, app_audio_get_volume(APP_AUDIO_CURRENT_STATE), NULL);
#endif
				return false;
		}
#endif

    switch (key_event) 
	{


#ifdef SUPPORT_KPH_DEF
/*----------------------------------------------------------
		customer add  msg
----------------------------------------------------------*/


#ifdef SUPPORT_USER_REC_MODE

    case KEY_REC_MODE:
        printf("\n KEY_REC_MODE====\n");
#ifdef board_dy_DS4751C_FL872_ac6951c_6ADKEY
		   if(use_ctl.mic_is_online==false) 
		   {
				if(music_app_check())
				{
					if (app_get_curr_task() == APP_MUSIC_TASK) 
					{
						use_ctl.play_rec_file_flag = 0;
						app_common_key_var_event(KEY_MUSIC_PLAYE_REC_FILE);	
					}
					else
					{
						use_ctl.rec_auto_play = true;		
						app_task_switch_to(APP_MUSIC_TASK);
					}
				}
		   		break;
		   }
#endif
#ifdef SUPPORT_MIC_OFFLINE_DISABLE_REC_KEY
		   if(use_ctl.mic_is_online==false) break;
#endif 
        if(use_ctl.tws_key_disable==0)break;
        if (dev_manager_get_total(0)) 
		{
          use_ctl.one_key_to_rec = 1;
          app_task_switch_to(APP_RECORD_TASK);
        }
		else
		{
            UI_SHOW_MENU(MENU_NO_DEVICE, 3000, 0, NULL);
        }
        break;
#endif		
#ifdef SUPPORT_MUTE_KEY
     case KEY_MUTE:
	 
        if(use_ctl.tws_key_disable==0)break;
        if(use_ctl.mute_flag)
        {
            use_ctl.mute_flag = 0;
            #ifdef SUPPORT_MIC_ALONE_VOL_MODE
		    if (app_get_curr_task() == APP_LINEIN_TASK)
			{
		          audio_linein_mute(0);
		    }
	    	#endif
		#ifndef SUPPORT_MUTE_KEY_DISABLE_MUTE_AMP		
		    pa_unmute();
		#endif
			app_audio_mute(1);
			app_audio_set_volume(APP_AUDIO_STATE_MUSIC, use_ctl.led_vol, 1);
			
        }
        else
        {
        	use_ctl.mute_flag = 1;
			app_audio_mute(0);
		 #ifndef SUPPORT_MUTE_KEY_DISABLE_MUTE_AMP		
		 	pa_mute();
		 #endif

        }
        break;
#endif
#ifdef SUPPORT_RECPLAY_KEY
    case KEY_COME_TO_PLAY_REC_FILE:
	 if(music_app_check())
	 {
        use_ctl.rec_auto_play = true;		
        app_task_switch_to(APP_MUSIC_TASK);
	 }
        break;
#endif	

#ifdef SUPPORT_MODE_KEY
    case KEY_MUSIC_MODE:
        app_task_switch_to(APP_MUSIC_TASK);
        break;
    case KEY_AUX_MODE:
        app_task_switch_to(APP_LINEIN_TASK);
        break;
    case KEY_FM_MODE:
        app_task_switch_to(APP_FM_TASK);
        break;
    case KEY_BT_MODE:
	 	
		bt_key_disconnect();	
        //app_task_switch_to(APP_BT_TASK);
        break;
#ifdef SUPPORT_USB_MODE_KEY
    case KEY_USB_MODE:
	if(check_usb_online())
	{
	    use_ctl.bMediaUsbFlag=1;
	    app_task_switch_to(APP_MUSIC_TASK);
	}
        break;
#endif	
#ifdef SUPPORT_SD_MODE_KEY
    case KEY_SD_MODE:
	if(check_sd0_online())		
	{
	    use_ctl.bMediaSdFlag=1;
	    app_task_switch_to(APP_MUSIC_TASK);
	}	
        break;
#endif		
 #endif
    
#ifdef SUPPORT_OK_MENU_KEY
    case KEY_OK_MENU:
	 if(use_ctl.bMicMenu!=MENU_OK_VOL)
	{
		use_ctl.bMicMenu=MENU_OK_VOL;
		UI_SHOW_MENU(MENU_MIC_VOL, 3000, 0, NULL);        
	}
	else if(use_ctl.bMicMenu==MENU_OK_VOL)
	{
		use_ctl.bMicMenu=MENU_OK_ECHO;
		UI_SHOW_MENU(MENU_ECHO, 3000, 0, NULL);        
	}
         use_ctl.bMicMenuCnt = 6;	
 	 break;

    case KEY_OK_MENU_UP:
	 if(use_ctl.bMicMenu==MENU_OK_VOL)
	{
          app_common_key_var_event(KEY_MIC_VOL_UP);	
	}
	else if(use_ctl.bMicMenu==MENU_OK_ECHO)
	{
          app_common_key_var_event(KEY_ECHO_UP);	
	}
	else if(use_ctl.bMicMenu==MENU_SYS_VOL)
	{
          app_common_key_var_event(KEY_VOL_UP);	
	}	
 	 break;	 

    case KEY_OK_MENU_DN:
	 if(use_ctl.bMicMenu==MENU_OK_VOL)
	{
          app_common_key_var_event(KEY_MIC_VOL_DN);	
	}
	else if(use_ctl.bMicMenu==MENU_OK_ECHO)
	{
          app_common_key_var_event(KEY_ECHO_DN);	
	}
	else if(use_ctl.bMicMenu==MENU_SYS_VOL)
	{
          app_common_key_var_event(KEY_VOL_DOWN);	
	}	
 	 break;	 
#endif

#ifdef SUPPORT_RESET_KEY
    case KEY_RESET:

 	 break;	 
#endif
#ifdef SUPPORT_OK_TONE_KEY
    case KEY_TONE_UP:

 	 break;
    case KEY_TONE_DN:

 	 break;	 
 #endif
		
#ifdef SUPPORT_KEY_LED_EN
    case KEY_LED_ONOFF:
		if(use_ctl.bKeyLedFlag)
 	 	{
	 	 	use_ctl.bKeyLedFlag=0;
			LED_MODE_OFF();
		}
		else
 	 	{
	 	 	use_ctl.bKeyLedFlag=1;
			LED_MODE_ON();
		}
 	 break;
 #endif
 
#ifdef SUPPORT_init_RGB_MODE_15s_change
		case KEY_RGB_ONOFF:
			
			extern u8 light_mode_mark;
			if(light_mode_mark)
			{
				light_mode_mark = RGB_OFF;
				UI_SHOW_MENU(MENU_RGBmode_SHOW, 3000, 0, NULL);
			}
			else
			{
				light_mode_mark =	0x10 | 1;
				UI_SHOW_MENU(MENU_RGBmode_SHOW, 3000, 1, NULL);
			}
	
			break;
#else

 #ifdef SPI1_RGB_DEV_EN
	case KEY_RGB_ONOFF:
		extern u8 light_mode_mark;
		if(light_mode_mark){
			light_mode_mark = RGB_OFF;
		}else{
			light_mode_mark = RGB_MODE_1;
		}
		printf("light_mode_mark=%d\n",light_mode_mark);
		UI_SHOW_MENU(MENU_ONOFF, 3000, light_mode_mark, NULL);
		break;
#endif
#endif

#ifdef SUPPORT_STANBY_KEY
     case KEY_STANBY:
        printf("\n KEY_STANBY====");
        if(use_ctl.tws_key_disable==0)break;
        if(app_get_curr_task() == APP_IDLE_TASK)
        {
			use_ctl.standby_out_to_spdif = 1;
            UI_SHOW_MENU(MENU_POWER_UP, 3000, 0, NULL);
            delay_2ms(200);
			PA_UNMUTE();

        #ifdef SUPPORT_STANBY_BACK_MODE
			app_task_switch_back();
		#else
	            if(music_app_check())
	            {
	               app_task_switch_to(APP_MUSIC_TASK);
	            }
	            else
	            {
	                #ifdef SUPPORT_POWER_ON_TO_AUX
	                app_task_switch_to(APP_LINEIN_TASK);
	                #else
	                app_task_switch_to(APP_BT_TASK);
	                #endif
	            }
		#endif
        }
        else
        {
			use_ctl.standby_out_to_spdif = 0;
            PA_MUTE();
            UI_SHOW_MENU(MENU_POWER_OFF, 3000, 0, NULL);
            delay_2ms(200);
            app_task_switch_to(APP_IDLE_TASK);
        }
        break;

#endif

#ifdef SUPPORT_MIC_ONOFF_KEY

    case KEY_MIC_ONOFF:
		
		if(use_ctl.tws_key_disable==0)break;
        if(use_ctl.mic_sw)
		{
            use_ctl.mic_sw=0;
        	app_audio_set_volume(APP_AUDIO_STATE_MUSIC, use_ctl.led_vol, 1);
        }
		else
		{
        	use_ctl.mic_sw=1;
        }
        UI_SHOW_MENU(MENU_MIC_MODE, 3000, 0, NULL);
	 #ifdef SUPPORT_VOICE_EN_OFF		 
	 #ifdef SUPPORT_TONE_MIC_ONOFF	
		if(use_ctl.bVoiceFlag)
			{
				if(use_ctl.mic_sw==0)
				{
			           tone_play_by_path(tone_table[IDEX_TONE_MIC_OFF], 1);
				}
				else
				{
			           tone_play_by_path(tone_table[IDEX_TONE_MIC_ON], 1);
				}
			}
	 #endif
	 #endif
		 
        break;
#endif
#if AUDIO_VOCAL_REMOVE_EN
    case KEY_VOCAL_PP:
         if(use_ctl.vocal_pp)
        {
           use_ctl.vocal_pp = 0;
           vocal_pp(1);
        }
        else
        {
           use_ctl.vocal_pp = 1;
           vocal_pp(0);
        }
         UI_SHOW_MENU(MENU_VOCAL, 3000, 0, NULL);
         break;
#endif

#ifdef SUPPORT_sub2100_Encoder
    case KEY__MENU2:
		 if (use_ctl.menu_mode>=(MENU_MAX_MODE-1) || use_ctl.menu_mode<2)
			 {
			 use_ctl.menu_mode=2;
			 }
		 app_common_key_var_event(KEY_MENU_MODE);			
         break;
    case KEY__MENU:
		 if (use_ctl.menu_mode>=2)
			 {
			 use_ctl.menu_mode=MENU_MAX_MODE;
			 }
		 app_common_key_var_event(KEY_MENU_MODE);			
         break;
    case KEY_MENU__UP:
		use_ctl.menu_mode_cnt = 6;
		if (key_value)
		{
			 if (use_ctl.menu_mode>(MENU_MAX_MODE-1) || use_ctl.menu_mode<=2)
				 {
				 use_ctl.menu_mode=3;
				 }
		}
		else
		{
			if (use_ctl.menu_mode>2)
				{
				use_ctl.menu_mode=0;
				}
		}
		 app_common_key_var_event(KEY_MENU_SEL_UP); 
		
         break;
    case KEY_MENU__DN:
		 if (key_value)
		 {
			 if (use_ctl.menu_mode>(MENU_MAX_MODE-1) || use_ctl.menu_mode<=2)
				 {
				 use_ctl.menu_mode=3;
				 }
		 }
		 else
		 {
			 if (use_ctl.menu_mode>2)
				 {
				 use_ctl.menu_mode=0;
				 }
		 }
		 app_common_key_var_event(KEY_MENU_SEL_DN); 		
		 use_ctl.menu_mode_cnt = 6;
         break;
		 
#endif

#ifdef SUPORT_MENU1_MODE
    case KEY_MENU1_MODE:
		
	          use_ctl.menu1_mode++;
	          if(use_ctl.menu1_mode>=MENU1_MAX_MODE)
			{			  	
				 use_ctl.menu1_mode=1;					 
	          	}

	          if(use_ctl.menu1_mode == MENU1_MIC_VOL_MODE)UI_SHOW_MENU(MENU_MIC_VOL, 3000, 0, NULL);
			  
	          if(use_ctl.menu1_mode == MENU1_MIC_ECHO_MODE)UI_SHOW_MENU(MENU_ECHO, 3000, 0, NULL);
		 #ifdef MENU1_MIC_BASS_MODE
		 	if(use_ctl.menu1_mode == MENU1_MIC_BASS_MODE)UI_SHOW_MENU(MENU_MICBASS, 3000, 0, NULL);
		 #endif
		 #ifdef MENU1_MIC_TRE_MODE
		 	if(use_ctl.menu1_mode == MENU1_MIC_TRE_MODE)UI_SHOW_MENU(MENU_MICTRE, 3000, 0, NULL);
		 #endif
		 #ifdef MENU1_MIC_DELAY_MODE
		 	if(use_ctl.menu1_mode == MENU1_MIC_DELAY_MODE)UI_SHOW_MENU(MENU_MICDELAY, 3000, 0, NULL);	
		 #endif

	          use_ctl.menu1_mode_cnt = 6;
	 
         break;
#endif

    #ifdef SUPORT_MENU_MODE
    case KEY_MENU_MODE:
		
	  #ifdef SUPPORT_MENU_ONLY_BASS_TRE
	  
	 	 if(use_ctl.menu_mode==MENU_BASS_MODE)
	 	 {
	 	             use_ctl.menu_mode=MENU_TRE_MODE;
	 		UI_SHOW_MENU(MENU_TRE, 3000, 0, NULL);
	 	 }
	 	 else
	 	 {
	              use_ctl.menu_mode=MENU_BASS_MODE;
	 		UI_SHOW_MENU(MENU_BASS, 3000, 0, NULL);
	 	 }		 
		 break;
	 #endif
	 
	          use_ctl.menu_mode++;
	          if(use_ctl.menu_mode>=MENU_MAX_MODE){
			  	
			 #if 1//def MENU_NO_VOL
			 	use_ctl.menu_mode=1;
			 #else
			 	use_ctl.menu_mode=0;
			 	UI_SHOW_MENU(MENU_MAIN_VOL, 3000, app_audio_get_volume(APP_AUDIO_CURRENT_STATE), NULL);
			 #endif
				 
	          }
	 		 
	          if(use_ctl.menu_mode == MENU_BASS_MODE)UI_SHOW_MENU(MENU_BASS, 3000, 0, NULL);
	          if(use_ctl.menu_mode == MENU_TRE_MODE)UI_SHOW_MENU(MENU_TRE, 3000, 0, NULL);
	          if(use_ctl.menu_mode == MENU_MIC_VOL_MODE)UI_SHOW_MENU(MENU_MIC_VOL, 3000, 0, NULL);
	          if(use_ctl.menu_mode == MENU_MIC_ECHO_MODE)UI_SHOW_MENU(MENU_ECHO, 3000, 0, NULL);
	 #ifdef MENU_MIC_BASS_MODE
	 	 if(use_ctl.menu_mode == MENU_MIC_BASS_MODE)UI_SHOW_MENU(MENU_MICBASS, 3000, 0, NULL);
#endif
#ifdef MENU_MIC_TRE_MODE
	          if(use_ctl.menu_mode == MENU_MIC_TRE_MODE)UI_SHOW_MENU(MENU_MICTRE, 3000, 0, NULL);
#endif
#ifdef MENU_MIC_DELAY_MODE
	          if(use_ctl.menu_mode == MENU_MIC_DELAY_MODE)UI_SHOW_MENU(MENU_MICDELAY, 3000, 0, NULL);	
	 #endif
	 	 #ifdef SUPPORT_ENCORD_LED 
	 	 	use_ctl.bEncordSw=5;
	 	 #endif					 
	          use_ctl.menu_mode_cnt = 6;
	 
         break;

    case KEY_MENU_SEL_UP:	
		 #ifdef MENU_NO_VOL
		 	if(use_ctl.menu_mode == MENU_VOL_MODE)
		 		break;
		 #endif
	         use_ctl.menu_mode_cnt = 6;		 		
	 	 #ifdef SUPPORT_ENCORD_LED 
	 	 	use_ctl.bEncordSw=5;
	 	 #endif							 
     		 app_common_key_var_event(menu_sel_table[0][use_ctl.menu_mode]);			
		break;		
    case KEY_MENU_SEL_DN:		
#ifdef MENU_NO_VOL
						  if(use_ctl.menu_mode == MENU_VOL_MODE)
							 break;
#endif
	         use_ctl.menu_mode_cnt = 6;	
	 	 #ifdef SUPPORT_ENCORD_LED 
	 	 	use_ctl.bEncordSw=5;
	 	 #endif							 
     		 app_common_key_var_event(menu_sel_table[1][use_ctl.menu_mode]);			
		break;

    case KEY_MENU_MICVOL:

		 #ifdef SUPPORT_ENCORD_LED 
		 use_ctl.bEncordSw=5;
		 #endif					 
	          use_ctl.menu_mode_cnt = 6;	 

		
		#ifdef SUPPORT_MENU_MICVOL_ECHO_DELAY_KEY
		 if(use_ctl.menu_mode==MENU_MIC_VOL_MODE)
		 	{
			     use_ctl.menu_mode=MENU_MIC_ECHO_MODE;
			    UI_SHOW_MENU(MENU_ECHO, 3000, 0, NULL);		 				 
		 	}
		 else  if(use_ctl.menu_mode==MENU_MIC_ECHO_MODE)
		 	{
			 	use_ctl.menu_mode=MENU_MIC_DELAY_MODE;
			 	UI_SHOW_MENU(MENU_MICDELAY, 3000, 0, NULL);		 		 
		 	}
		 else
		 	{
			 	use_ctl.menu_mode=MENU_MIC_VOL_MODE;
			 	UI_SHOW_MENU(MENU_MIC_VOL, 3000, 0, NULL);		 		 
		 	}
		#elif defined SUPPORT_MENU_MIC_BASS_TRE_ECHO_DELAY_KEY
		
		 if(use_ctl.menu_mode==MENU_MIC_BASS_MODE)
		 	{
			     use_ctl.menu_mode=MENU_MIC_TRE_MODE;
			    UI_SHOW_MENU(MENU_MICTRE, 3000, 0, NULL);		 				 
		 	}
		 else  if(use_ctl.menu_mode==MENU_MIC_TRE_MODE)
		 	{
			     use_ctl.menu_mode=MENU_MIC_ECHO_MODE;
			    UI_SHOW_MENU(MENU_ECHO, 3000, 0, NULL);		 				 
		 	}
		 else  if(use_ctl.menu_mode==MENU_MIC_ECHO_MODE)
		 	{
			 	use_ctl.menu_mode=MENU_MIC_DELAY_MODE;
			 	UI_SHOW_MENU(MENU_MICDELAY, 3000, 0, NULL);		 		 
		 	}
		 else
		 	{
			 	use_ctl.menu_mode=MENU_MIC_BASS_MODE;
			 	UI_SHOW_MENU(MENU_MICBASS, 3000, 0, NULL);		 		 
		 	}
		
		#else
		 	use_ctl.menu_mode=MENU_MIC_VOL_MODE;
		 	UI_SHOW_MENU(MENU_MIC_VOL, 3000, 0, NULL);		 		 
		 #endif
		 
         break;
		 
#if defined SUPPORT_MENU_MIC_BASS_TRE_ECHO_DELAY_KEY

    case KEY_OK_MENU_UP:	
		
		 if(use_ctl.menu_mode<MENU_MIC_BASS_MODE)  break;
		 
	         use_ctl.menu_mode_cnt = 6;		 		
	 	 #ifdef SUPPORT_ENCORD_LED 
	 	 	use_ctl.bEncordSw=5;
	 	 #endif						
		 
     		 app_common_key_var_event(menu_sel_table[0][use_ctl.menu_mode]);			
		break;		
    case KEY_OK_MENU_DN:		
		 if(use_ctl.menu_mode<MENU_MIC_BASS_MODE)  break;		
	         use_ctl.menu_mode_cnt = 6;	
	 	 #ifdef SUPPORT_ENCORD_LED 
	 	 	use_ctl.bEncordSw=5;
	 	 #endif				
//		 if(use_ctl.menu_mode==0)  use_ctl.menu_mode=MENU_BASS_MODE;		 
     		 app_common_key_var_event(menu_sel_table[1][use_ctl.menu_mode]);			
		break;
#endif

    case KEY_MENU_BASS:
		 use_ctl.menu_mode=MENU_BASS_MODE;
		 UI_SHOW_MENU(MENU_BASS, 3000, 0, NULL);		 
		 #ifdef SUPPORT_ENCORD_LED 
		 use_ctl.bEncordSw=5;
		 #endif					 
	          use_ctl.menu_mode_cnt = 6;	 		 	 
         break;
		 
    case KEY_MENU_TRE:
		 use_ctl.menu_mode=MENU_TRE_MODE;
		 UI_SHOW_MENU(MENU_TRE, 3000, 0, NULL);		 
		 #ifdef SUPPORT_ENCORD_LED 
		 use_ctl.bEncordSw=5;
		 #endif					 
	          use_ctl.menu_mode_cnt = 6;
         break;		

    case KEY_MENU_ECHO:
		 use_ctl.menu_mode=MENU_MIC_ECHO_MODE;
		 UI_SHOW_MENU(MENU_ECHO, 3000, 0, NULL);		 
		 #ifdef SUPPORT_ENCORD_LED 
		 use_ctl.bEncordSw=5;
		 #endif					 
	          use_ctl.menu_mode_cnt = 6;	 		 	 
         break;				 
	 
    case KEY_MENU_MICBASS:
		 use_ctl.menu_mode=MENU_MIC_BASS_MODE;
		 UI_SHOW_MENU(MENU_MICBASS, 3000, 0, NULL);		 
		 #ifdef SUPPORT_ENCORD_LED 
		 use_ctl.bEncordSw=5;
		 #endif					 
	          use_ctl.menu_mode_cnt = 6;	 		 	 
         break;				 
	 		
    case KEY_MENU_MICTRE:
		 use_ctl.menu_mode=MENU_MIC_TRE_MODE;
		 UI_SHOW_MENU(MENU_MICTRE, 3000, 0, NULL);		 
		 #ifdef SUPPORT_ENCORD_LED 
		 use_ctl.bEncordSw=5;
		 #endif					 
	          use_ctl.menu_mode_cnt = 6;	 		 	 
         break;	
#endif

#ifdef EQ_MENU

	case KEY_EQ_MENU:
		use_ctl.eq_menu_cnt=6;
		use_ctl.eq_menu++;
		if(use_ctl.eq_menu>EQ_MENU_2){
            use_ctl.eq_menu=0;
			UI_SHOW_MENU(MENU_MAIN_VOL, 3000, app_audio_get_volume(APP_AUDIO_CURRENT_STATE), NULL);
		}
		if(use_ctl.eq_menu==EQ_MENU_1)UI_SHOW_MENU(MENU_EQ50HZ, 3000, 0, NULL);
		if(use_ctl.eq_menu==EQ_MENU_2)UI_SHOW_MENU(MENU_EQ6KHZ, 3000, 0, NULL);
		printf("KEY_EQ_MENU=%d\n",use_ctl.eq_menu);
		break;

	case KEY_MENU_EQ1_UP:
		use_ctl.eq50hz_val++;
		if(use_ctl.eq50hz_val>=MENU_EQ_MAX_VAL)use_ctl.eq50hz_val=MENU_EQ_MAX_VAL;
		user_define_eq_update(EQ_POS_50HZ,eq_table[use_ctl.eq50hz_val]);
		UI_SHOW_MENU(MENU_EQ50HZ, 3000, 0, NULL);
		break;
	case KEY_MENU_EQ1_DN:
		if(use_ctl.eq50hz_val)use_ctl.eq50hz_val--;
		user_define_eq_update(EQ_POS_50HZ,eq_table[use_ctl.eq50hz_val]);
		UI_SHOW_MENU(MENU_EQ50HZ, 3000, 0, NULL);
		break;
	case KEY_MENU_EQ2_UP:
		use_ctl.eq6khz_val++;
		if(use_ctl.eq6khz_val>=MENU_EQ_MAX_VAL)use_ctl.eq6khz_val=MENU_EQ_MAX_VAL;
		user_define_eq_update(EQ_POS_6KHZ,eq_table[use_ctl.eq6khz_val]);
		UI_SHOW_MENU(MENU_EQ6KHZ, 3000, 0, NULL);
		break;
	case KEY_MENU_EQ2_DN:
		if(use_ctl.eq6khz_val)use_ctl.eq6khz_val--;
		user_define_eq_update(EQ_POS_6KHZ,eq_table[use_ctl.eq6khz_val]);
		UI_SHOW_MENU(MENU_EQ6KHZ, 3000, 0, NULL);
		break;
#endif
		
#if (TCFG_MIC_EFFECT_ENABLE)
	  
   #ifdef JT_VOL_IO_AD_EN
      case KEY_JT_VOL_SET: 
	  	
					
#ifdef JT_VOL_M62429_EN
					
					m62429_send_ctl(0,use_ctl.jt_vol);
			
#endif
		 #ifdef SUPPORT_POWER_ON_AD_SHOW_DLY
		 	if(use_ctl.bPowerOnShowDlyCnt==0)
		 #endif		 	  	
	         UI_SHOW_MENU(MENU_JT_VOL, 3000, 0, NULL);
         break;
   #endif



      case KEY_MIC_VOL_SET:
		 #ifdef SUPPORT_MIC_ECHO_VOL_SET
	 	   	  use_ctl.vMicVolTemp = mic_vol_table[use_ctl.mic_vol];
		         mic_effect_set_dvol(use_ctl.vMicVolTemp);
		 #else
		         mic_effect_set_dvol(use_ctl.mic_vol);
		 #endif

	 	 	#ifdef SUPPORT_VM_MICVOL
	 	 		 use_ctl.vMicvolCnt=3;
	 	 	#endif
		 
 	         printf("\n  KEY_MIC_VOL_SET==%d",use_ctl.vMicVolTemp);

		 #ifdef SUPPORT_POWER_ON_AD_SHOW_DLY
		 	if(use_ctl.bPowerOnShowDlyCnt==0)
		 #endif	 	  		 
		 	{
				 UI_SHOW_MENU(MENU_MIC_VOL, 3000, 0, NULL);		//电位器修改4
			}
         break;



       case KEY_ECHO_SET:
		 #ifdef SUPPORT_MIC_ECHO_VOL_SET
	 	   	  use_ctl.vMicEchoTemp = mic_echo_table[use_ctl.echo];
		         mic_effect_set_echo_decay(use_ctl.vMicEchoTemp);
		 #else
		         mic_effect_set_echo_decay(use_ctl.echo);
		 #endif	  	
	#ifndef MIC_DELAY_IO_AD_EN
	        if(use_ctl.echo==0)
			{
	            mic_effect_set_echo_delay(0);
	        }
			else
			{
	           mic_effect_set_echo_delay(MIC_ECHO_DELAY);
	        }
	#endif

	 	 	#ifdef SUPPORT_VM_ECHO
	 	 		 use_ctl.vEchoCnt=3;
	 	 	#endif		 
	
 	        printf("\n  KEY_ECHO_SET==%d",use_ctl.vMicEchoTemp);
		#ifdef SUPPORT_POWER_ON_AD_SHOW_DLY
			if(use_ctl.bPowerOnShowDlyCnt==0)
		#endif		 
			{	
        		UI_SHOW_MENU(MENU_ECHO, 3000, 0, NULL);			//电位器修改5
			}
			break;
 
	
	
	
	
	 #ifdef SUPPORT_MIC_DELAY_SET
       case KEY_MIC_DELAY_SET:
		 #ifdef SUPPORT_MIC_DELAY_SET
	 	   	  use_ctl.vMicDelayTemp = mic_delay_table[use_ctl.micdelay];
		         mic_effect_set_echo_delay(use_ctl.vMicDelayTemp);
		 #else
		         mic_effect_set_echo_delay(use_ctl.micdelay);
		 #endif	  

	 	 	#ifdef SUPPORT_VM_MICDELAY
	 	 		 use_ctl.vMicDelayCnt=3;
	 	 	#endif		 
		 
 	        // printf("\n  KEY_MIC_DELAY_SET==%d",use_ctl.vMicDelayTemp);

		 #ifdef SUPPORT_POWER_ON_AD_SHOW_DLY
		 	if(use_ctl.bPowerOnShowDlyCnt==0)
		 #endif		 
	         UI_SHOW_MENU(MENU_MICDELAY, 3000, 0, NULL);
         break;
      case KEY_MIC_DELAY_UP:
         if(use_ctl.micdelay<MIC_DELAY_MAX)use_ctl.micdelay++;		 
	 #ifdef SUPPORT_MIC_DELAY_SET
 	   	  use_ctl.vMicDelayTemp = mic_delay_table[use_ctl.micdelay];
	         mic_effect_set_echo_delay(use_ctl.vMicDelayTemp);			 
	 #else
	         mic_effect_set_echo_delay(use_ctl.micdelay);
	 #endif	  	

	 	#ifdef SUPPORT_VM_MICDELAY
	 		 use_ctl.vMicDelayCnt=3;
	 	#endif		 
	 
        UI_SHOW_MENU(MENU_MICDELAY, 3000, 0, NULL);
        break;
      case KEY_MIC_DELAY_DN:
         if(use_ctl.micdelay)use_ctl.micdelay--;	  	
	 #ifdef SUPPORT_MIC_ECHO_VOL_SET
 	   	  use_ctl.vMicDelayTemp = mic_delay_table[use_ctl.micdelay];
	         mic_effect_set_echo_delay(use_ctl.vMicDelayTemp);			 
	 #else
	         mic_effect_set_echo_delay(use_ctl.micdelay);
	 #endif	  
	 	#ifdef SUPPORT_VM_ECHO
	 		 use_ctl.vMicDelayCnt=3;
	 	#endif		 	 
        UI_SHOW_MENU(MENU_MICDELAY, 3000, 0, NULL);
        break;		
#endif		 
      case KEY_MIC_VOL_UP:
	 #ifdef SUPPORT_MIC_ECHO_VOL_SET
	          if(use_ctl.mic_vol<MICVOL_MAX)use_ctl.mic_vol++;
	   	  use_ctl.vMicVolTemp = mic_vol_table[use_ctl.mic_vol];
        	 mic_effect_set_dvol(use_ctl.vMicVolTemp);
	 #else
	         if(use_ctl.mic_vol<MICVOL_MAX)use_ctl.mic_vol++;
	         mic_effect_set_dvol(use_ctl.mic_vol);
	 #endif	  
 	 	#ifdef SUPPORT_VM_MICVOL
 	 		 use_ctl.vMicvolCnt=3;
 	 	#endif	 
	 
         UI_SHOW_MENU(MENU_MIC_VOL, 3000, 0, NULL);			//问题！！！
         break;
      case KEY_MIC_VOL_DN:
	 #ifdef SUPPORT_MIC_ECHO_VOL_SET
	         if(use_ctl.mic_vol)use_ctl.mic_vol--;
	   	  use_ctl.vMicVolTemp = mic_vol_table[use_ctl.mic_vol];
        	 mic_effect_set_dvol(use_ctl.vMicVolTemp);
	 #else
	         if(use_ctl.mic_vol)use_ctl.mic_vol--;
	         mic_effect_set_dvol(use_ctl.mic_vol);
	 #endif	  	
 	 	#ifdef SUPPORT_VM_MICVOL
 	 		 use_ctl.vMicvolCnt=3;
 	 	#endif	 
	 
         UI_SHOW_MENU(MENU_MIC_VOL, 3000, 0, NULL);
         break;
      case KEY_ECHO_UP:
	 #ifdef SUPPORT_MIC_ECHO_VOL_SET
	         if(use_ctl.echo<ECHO_MAX)use_ctl.echo++;
 	   	  use_ctl.vMicEchoTemp = mic_echo_table[use_ctl.echo];
	         mic_effect_set_echo_decay(use_ctl.vMicEchoTemp);			 
	 #else
	         if(use_ctl.echo<ECHO_MAX)use_ctl.echo++;
	         mic_effect_set_echo_decay(use_ctl.echo);
	 #endif	  		  	
	 #ifndef MIC_DELAY_IO_AD_EN
	         if(use_ctl.echo==0){
	            mic_effect_set_echo_delay(0);
	        }else{
	           mic_effect_set_echo_delay(MIC_ECHO_DELAY);
	        }
	#endif
 	#ifdef SUPPORT_VM_ECHO
 		 use_ctl.vEchoCnt=3;
 	#endif		 
	
        UI_SHOW_MENU(MENU_ECHO, 3000, 0, NULL);
        break;
      case KEY_ECHO_DN:
	 #ifdef SUPPORT_MIC_ECHO_VOL_SET
	         if(use_ctl.echo)use_ctl.echo--;
 	   	  use_ctl.vMicEchoTemp = mic_echo_table[use_ctl.echo];
	         mic_effect_set_echo_decay(use_ctl.vMicEchoTemp);			 
	 #else
	         if(use_ctl.echo)use_ctl.echo--;
	         mic_effect_set_echo_decay(use_ctl.echo);
	 #endif	  		  	
	 #ifndef MIC_DELAY_IO_AD_EN
	         if(use_ctl.echo==0){
	            mic_effect_set_echo_delay(0);
	        }else{
	           mic_effect_set_echo_delay(MIC_ECHO_DELAY);
	        }
	#endif

	 	 	#ifdef SUPPORT_VM_ECHO
	 	 		 use_ctl.vEchoCnt=3;
	 	 	#endif		 
	
        UI_SHOW_MENU(MENU_ECHO, 3000, 0, NULL);
        break;

   #ifdef SUPPORT_MIC_BASS_TRE_SET	
   
       case KEY_MIC_TRE_SET:
			
			use_ctl.vmictretemp= mic_tre_table[use_ctl.mic_tre];
			mic_tunning_eq_update(MIC_EQ_MODE_HIGH_SOUND,use_ctl.vmictretemp);
	  	 	#ifdef SUPPORT_VM_MICTRE
	  	 		 use_ctl.vMicTreCnt=3;
	  	 	#endif			 

 	        printf("\n MIC TRE==%d %d",use_ctl.mic_tre,mic_tre_table[use_ctl.mic_tre]);
		 #ifdef SUPPORT_POWER_ON_AD_SHOW_DLY
		 	if(use_ctl.bPowerOnShowDlyCnt==0)
		 #endif
 	         UI_SHOW_MENU(MENU_MICTRE, 3000, 0, NULL);		 
          break;


       case KEY_MIC_BASS_SET:
 	 	   	 use_ctl.vmicbasstemp= mic_bass_table[use_ctl.mic_bass];
 		     mic_tunning_eq_update(MIC_EQ_MODE_LOW_SOUND,use_ctl.vmicbasstemp);
	  	 	#ifdef SUPPORT_VM_MICBASS
	  	 		 use_ctl.vMicBassCnt=3;
	  	 	#endif			 
			
			printf("\nMIC BASS==%d %d",use_ctl.mic_bass,mic_bass_table[use_ctl.mic_bass]);		 
 		 #ifdef SUPPORT_POWER_ON_AD_SHOW_DLY
		 	if(use_ctl.bPowerOnShowDlyCnt==0)
		 #endif
	        UI_SHOW_MENU(MENU_MICBASS, 3000, 0, NULL);	
          break;

		  
		 
		 
		  case KEY_MIC_TRE_UP:
 	       	  if(use_ctl.mic_tre<MIC_TRE_MAX)use_ctl.mic_tre++;
 	 	   	  use_ctl.vmictretemp= mic_tre_table[use_ctl.mic_tre];
 		         mic_tunning_eq_update(MIC_EQ_MODE_HIGH_SOUND,use_ctl.vmictretemp);
	  	 	#ifdef SUPPORT_VM_MICTRE
	  	 		 use_ctl.vMicTreCnt=3;
	  	 	#endif			 
				 
 	         UI_SHOW_MENU(MENU_MICTRE, 3000, 0, NULL);
          break;
       case KEY_MIC_TRE_DN:
 		         if(use_ctl.mic_tre>0)use_ctl.mic_tre--;
 	 	   	  use_ctl.vmictretemp= mic_tre_table[use_ctl.mic_tre];
 		         mic_tunning_eq_update(MIC_EQ_MODE_HIGH_SOUND,use_ctl.vmictretemp);
	  	 	#ifdef SUPPORT_VM_MICTRE
	  	 		 use_ctl.vMicTreCnt=3;
	  	 	#endif			 
				 
 	         UI_SHOW_MENU(MENU_MICTRE, 3000, 0, NULL);		 
          break;
 		 
       case KEY_MIC_BASS_UP:
 	       	  if(use_ctl.mic_bass<MIC_TRE_MAX)use_ctl.mic_bass++;
 	 	   	  use_ctl.vmicbasstemp= mic_bass_table[use_ctl.mic_bass];
 		         mic_tunning_eq_update(MIC_EQ_MODE_LOW_SOUND,use_ctl.vmicbasstemp);
	  	 	#ifdef SUPPORT_VM_MICBASS
	  	 		 use_ctl.vMicBassCnt=3;
	  	 	#endif			 				 
 	           UI_SHOW_MENU(MENU_MICBASS, 3000, 0, NULL);		  	
          break;
       case KEY_MIC_BASS_DN:
 		         if(use_ctl.mic_bass>0)use_ctl.mic_bass--;
  	   	 	 use_ctl.vmicbasstemp= mic_bass_table[use_ctl.mic_bass];
 	        	 mic_tunning_eq_update(MIC_EQ_MODE_LOW_SOUND,use_ctl.vmicbasstemp);
	  	 	#ifdef SUPPORT_VM_MICBASS
	  	 		 use_ctl.vMicBassCnt=3;
	  	 	#endif			 				 
 	           UI_SHOW_MENU(MENU_MICBASS, 3000, 0, NULL);		  	
 	break;
  #endif // SUPPORT_MIC_BASS_TRE_SET		
#endif  // TCFG_MIC_EFFECT_ENABLE
	  
#ifdef VOL_IO_AD_EN



      case KEY_MAIN_VOL_SET:
       // if(use_ctl.tws_key_disable==0)break;
        // 最高位判断是否为 TWS消息
        use_ctl.mute_flag = 0;
#ifdef dy_DS4751C
		UI_SHOW_MENU(MENU_MAIN_VOL, 1000, app_audio_get_volume(APP_AUDIO_CURRENT_STATE), NULL);
#endif 
	   
#ifdef TWS_SYNC_EQ
		if(((u32)event->arg == KEY_EVENT_FROM_TWS) ||((u32)event->arg == SYS_BT_EVENT_FROM_TWS)) 
		{
#ifdef dy_DS4751C
								   UI_SHOW_MENU(MENU_MAIN_VOL, 1000, app_audio_get_volume(APP_AUDIO_CURRENT_STATE), NULL);
#endif
			break;
		}
		if(((u32)event->arg == KEY_EVENT_FROM_TWS) ||((u32)event->arg == SYS_BT_EVENT_FROM_TWS))
		{
#ifdef dy_DS4751C
									  UI_SHOW_MENU(MENU_MAIN_VOL, 1000, app_audio_get_volume(APP_AUDIO_CURRENT_STATE), NULL);
#endif
			   break;
		}
#endif

#ifdef SPI1_RGB_DS4751C_FL872
				use_ctl.l_vol_ed=1;
#endif

        if(key->value&BIT(7))
        {
           use_ctl.led_vol = key->value-0X80;
        }
#ifdef TWS_SYNC_EQ

		bt_tws_sync_volume();
#endif

	#ifdef SUPPORT_MIC_PR_CHECK
            use_ctl.led_vol_pr = use_ctl.led_vol;
            vol_backup_temp = use_ctl.led_vol;
            vol_obj_val = use_ctl.led_vol;
	#endif		
	
//       printf("\n ----KEY_MAIN_VOL_SET--ledvol==%d",use_ctl.led_vol);
		
        app_audio_set_volume(APP_AUDIO_STATE_MUSIC, use_ctl.led_vol, 1);
	 #ifdef SUPPORT_POWER_ON_AD_SHOW_DLY
	 	if(use_ctl.bPowerOnShowDlyCnt==0)
	 #endif
	 
		if (app_get_curr_task() != APP_IDLE_TASK) 
		{
        	UI_SHOW_MENU(MENU_MAIN_VOL, 3000, app_audio_get_volume(APP_AUDIO_CURRENT_STATE), NULL);		//电位器修改1
        }
		break;
#endif

#if defined SUPPORT_MIC_PR_CHECK
     




case KEY_MIC_PRE_VOL_DEAL:
         if(use_ctl.tws_key_disable==0)break;
         //use_ctl.led_vol_pr = 0;
         mic_pr_set_volume(use_ctl.led_vol_pr);
         //app_audio_set_volume_2(APP_AUDIO_STATE_MUSIC, use_ctl.led_vol_pr,  1);
        // UI_SHOW_MENU(MENU_VOL_PR, 3000, 0, NULL);
         break;
 #endif
 
#if SUPPORT_USER_EQ_MODE

	case KEY_BASS_SET:	
#ifdef TWS_SYNC_EQ
		if(((u32)event->arg==KEY_EVENT_FROM_TWS)||((u32)event->arg==SYS_BT_EVENT_FROM_TWS)){
			   break;
		}
		bt_tws_sync_volume();
#endif
		printf("\n bass=%d",use_ctl.bass);
		use_ctl.vbasstemp = bass_table[use_ctl.bass];
		user_define_eq_update(EQ_POS_BASS,use_ctl.vbasstemp);		
	 #ifdef SUPPORT_VM_BASS
	 	use_ctl.vbassCnt=3;
	 #endif		
		
	 #ifdef SUPPORT_POWER_ON_AD_SHOW_DLY
	 	if(use_ctl.bPowerOnShowDlyCnt==0)
	 #endif		 
	  {
			UI_SHOW_MENU(MENU_BASS, 3000, 0, NULL);		//电位器修改3
      }
		printf("\n  KEY_BASS_SET==%d-%d",use_ctl.bass,use_ctl.vbasstemp);
         break;
    
	 case KEY_TRE_SET:
#ifdef TWS_SYNC_EQ
		if(((u32)event->arg==KEY_EVENT_FROM_TWS)||((u32)event->arg==SYS_BT_EVENT_FROM_TWS)){
			   break;
		}
		bt_tws_sync_volume();
#endif
         printf("\n tre=%d",use_ctl.tre);
   	  	use_ctl.vtretemp = tre_table[use_ctl.tre];
	   	user_define_eq_update(EQ_POS_TRE,use_ctl.vtretemp);	
		 #ifdef SUPPORT_VM_TREB	 
		 	  use_ctl.vTreCnt=3;
		 #endif		
	 #ifdef SUPPORT_POWER_ON_AD_SHOW_DLY
	 	if(use_ctl.bPowerOnShowDlyCnt==0)
	 #endif
	    {
        	UI_SHOW_MENU(MENU_TRE, 3000, 0, NULL);				//电位器修改2
        }
	     printf("\n  KEY_TRE_SET==%d %d",use_ctl.tre,use_ctl.vtretemp);
         break;
#ifdef SUPPORT_MID_SET
    case KEY_MID_SET:
         printf("\n tre=%d",use_ctl.mid);
   	  use_ctl.vmidtemp = mid_table[use_ctl.mid];
	 #ifdef SUPPORT_POWER_ON_AD_SHOW_DLY
	 	if(use_ctl.bPowerOnShowDlyCnt==0)
	 #endif		 
	   user_define_eq_update(EQ_POS_MID,use_ctl.vmidtemp);			  
          UI_SHOW_MENU(MENU_MID, 3000, 0, NULL);
		  
         break;
#endif	 
    case KEY_BASS_UP:
#ifdef TWS_SYNC_EQ
			  if(((u32)event->arg==KEY_EVENT_FROM_TWS)||((u32)event->arg==SYS_BT_EVENT_FROM_TWS)){
					 break;
			  }
			  if(use_ctl.bass<BASS_MAX)use_ctl.bass++;
			  bt_tws_sync_volume();
#else
			if(use_ctl.bass<BASS_MAX)use_ctl.bass++;
#endif
   	  use_ctl.vbasstemp = bass_table[use_ctl.bass];
	   user_define_eq_update(EQ_POS_BASS,use_ctl.vbasstemp);		
	 #ifdef SUPPORT_VM_BASS
	 	use_ctl.vbassCnt=3;
	 #endif			   
         UI_SHOW_MENU(MENU_BASS, 3000, 0, NULL);
         break;
    case KEY_BASS_DN:
#ifdef TWS_SYNC_EQ
			  if(((u32)event->arg==KEY_EVENT_FROM_TWS)||((u32)event->arg==SYS_BT_EVENT_FROM_TWS)){
					 break;
			  }
			  if(use_ctl.bass)use_ctl.bass--;
			  bt_tws_sync_volume();
#else
	  if(use_ctl.bass)use_ctl.bass--;
#endif
   	  use_ctl.vbasstemp = bass_table[use_ctl.bass];
	   user_define_eq_update(EQ_POS_BASS,use_ctl.vbasstemp);		
	 #ifdef SUPPORT_VM_BASS
	 	use_ctl.vbassCnt=3;
	 #endif			   
         UI_SHOW_MENU(MENU_BASS, 3000, 0, NULL);
         break;
    case KEY_TRE_UP:
#ifdef TWS_SYNC_EQ
			if(((u32)event->arg==KEY_EVENT_FROM_TWS)||((u32)event->arg==SYS_BT_EVENT_FROM_TWS))
			{
				   break;
			}
			if(use_ctl.tre<TRE_MAX)use_ctl.tre++;
			bt_tws_sync_volume();
#else
		if(use_ctl.tre<TRE_MAX)
			use_ctl.tre++;
#endif
	use_ctl.vtretemp= tre_table[use_ctl.tre];
	user_define_eq_update(EQ_POS_TRE,use_ctl.vtretemp);	
	 #ifdef SUPPORT_VM_TREB	 
	 	  use_ctl.vTreCnt=3;
	 #endif	
	UI_SHOW_MENU(MENU_TRE, 3000, 0, NULL);
         break;
    case KEY_TRE_DN:
#ifdef TWS_SYNC_EQ
			if(((u32)event->arg==KEY_EVENT_FROM_TWS)||((u32)event->arg==SYS_BT_EVENT_FROM_TWS)){
				   break;
			}
			if(use_ctl.tre)use_ctl.tre--;
			bt_tws_sync_volume();
#else
	if(use_ctl.tre)use_ctl.tre--;
#endif
	use_ctl.vtretemp= tre_table[use_ctl.tre];
	user_define_eq_update(EQ_POS_TRE,use_ctl.vtretemp);		
	 #ifdef SUPPORT_VM_TREB	 
	 	  use_ctl.vTreCnt=3;
	 #endif	
	UI_SHOW_MENU(MENU_TRE, 3000, 0, NULL);
         break;
		 
#ifdef SUPPORT_MID_SET
    case KEY_MID_UP:
	if(use_ctl.mid<MID_MAX)use_ctl.mid++;
	use_ctl.vmidtemp = bass_table[use_ctl.mid];
	user_define_eq_update(EQ_POS_MID,use_ctl.vmidtemp);			  
	UI_SHOW_MENU(MENU_MID, 3000, 0, NULL);
	break;
    case KEY_MID_DN:
	if(use_ctl.mid)use_ctl.mid--;
	use_ctl.vmidtemp = bass_table[use_ctl.mid];
	user_define_eq_update(EQ_POS_MID,use_ctl.vmidtemp);			  
	UI_SHOW_MENU(MENU_MID, 3000, 0, NULL);
	break;
#endif
		 

#ifdef SUPPORT_CHANGE_VOICE_KEY
case KEY_VOICE_CHANGE:
		
	use_ctl.flag_10s++;
	printf(">>>>>>>>>>>>>>>>>>>>>>>%d\n",use_ctl.flag_10s);	
	if(use_ctl.flag_10s == 22)
	{
		printf("enter\n");
		use_ctl.bVoiceVal++;
		if (use_ctl.bVoiceVal >= VOICE_MODE_MAX)
		{
			use_ctl.bVoiceVal=0;
		}
		tone_table_switch(use_ctl.bVoiceVal);

#ifdef SUPPORT_VM_VOICE_MODE
	syscfg_write(VM_VOICE_MODE, &use_ctl.bVoiceVal, 1);
#endif

#ifdef SUPPORT_CHANGE_VOICE_TONE
#ifdef SUPPORT_VOICE_EN_OFF
   if (use_ctl.bVoiceFlag)
#endif
   {
	   #ifdef SUPPORT_CH_VOICE
	   if (use_ctl.bVoiceVal == VOICE_MODE_CH){
		   tone_play_by_path(tone_table[IDEX_TONE_VOICE_CHINESE], 1);
	   }
	   #endif
	   #ifdef SUPPORT_EN_VOICE
	   if (use_ctl.bVoiceVal == VOICE_MODE_EN){
		   tone_play_by_path(tone_table[IDEX_TONE_VOICE_ENGLISH], 1);
	   }
	   #endif
   }
#endif
		UI_SHOW_MENU(MENU_VOICE, 3000, 0, NULL);

	}
    break;


	case KEY_VOICE_CHANGE_END:

	use_ctl.flag_10s = 0;

   break;




#endif//SUPPORT_CHANGE_VOICE_KEY


    case KEY_ONE_KEY_BASS:
         use_ctl.one_key_bass = !use_ctl.one_key_bass;
         if(use_ctl.one_key_bass)eq_mode_set(EQ_MODE_NORMAL);
         else eq_mode_set(EQ_MODE_CUSTOM);
         UI_SHOW_MENU(MENU_ONE_KEY_BASS, 3000, 0, NULL);
         break;

#if defined SUPPORT_CD4051_PONTEN ||defined SUPPORT_PONTEN_EQ1_EQ5

    case KEY_EQ1_SET:
   	  use_ctl.veq1temp = mult_eq_table[use_ctl.bEq1Val];
	   user_define_eq_update(CD4051_EQ0_63HZ,use_ctl.veq1temp);		
	   #ifdef SUPPORT_OFF_CD4051_EQ_SHOW
	   break;
   	   #endif
          UI_SHOW_MENU(MENU_EQ1, 3000, 0, NULL);	
        break;
    case KEY_EQ2_SET:
   	  use_ctl.veq2temp = mult_eq_table[use_ctl.bEq2Val];
	   user_define_eq_update(CD4051_EQ1_400HZ,use_ctl.veq2temp);			  
	   #ifdef SUPPORT_OFF_CD4051_EQ_SHOW
	   break;
   	   #endif
          UI_SHOW_MENU(MENU_EQ2, 3000, 0, NULL);	
        break;
    case KEY_EQ3_SET:
   	  use_ctl.veq3temp = mult_eq_table[use_ctl.bEq3Val];
	   user_define_eq_update(CD4051_EQ2_1KHZ,use_ctl.veq3temp);			  
	   #ifdef SUPPORT_OFF_CD4051_EQ_SHOW
	   break;
   	   #endif
          UI_SHOW_MENU(MENU_EQ3, 3000, 0, NULL);	
        break;
    case KEY_EQ4_SET:
   	  use_ctl.veq4temp = mult_eq_table[use_ctl.bEq4Val];
	   user_define_eq_update(CD4051_EQ3_6KHZ,use_ctl.veq4temp);			  
	   #ifdef SUPPORT_OFF_CD4051_EQ_SHOW
	   break;
   	   #endif
          UI_SHOW_MENU(MENU_EQ4, 3000, 0, NULL);	
        break;
    case KEY_EQ5_SET:
   	  use_ctl.veq5temp = mult_eq_table[use_ctl.bEq5Val];
	   user_define_eq_update(CD4051_EQ4_12KHZ,use_ctl.veq5temp);			  
 	   #ifdef SUPPORT_OFF_CD4051_EQ_SHOW
	   break;
   	   #endif
         UI_SHOW_MENU(MENU_EQ5, 3000, 0, NULL);	
        break;


#endif

		 
#endif



#if TCFG_MIC_VOICE_CHANGER_ENABLE
    case KEY_VOICE_CHANGE_RESET:
	 mic_voicechange_reset();
        break;
    case KEY_VOICE_CHANGE_KEY:
	 mic_voicechange_loop();
        break;
#endif	

#ifdef SUPPORT_SOUNDSCAPES

    case KEY_VOICE_SOUNDSCAPES_KEY:
		
	use_ctl.bSoundScapesVal++;

	if(use_ctl.bSoundScapesVal>4)
   	 use_ctl.bSoundScapesVal=0;
	 mic_soundscapes();
	 
        UI_SHOW_MENU(MENU_SOUNDSCAPES, 3000, 0, NULL);		

        break;
#endif

    case KEY_OK_MIC_MAN:
		mic_voicechange_set(1);
            UI_SHOW_MENU(MENU_PITCH_MAN, 3000, 0, NULL);		
		break;

    case KEY_OK_MIC_WOMAN:
		mic_voicechange_set(2);		
            UI_SHOW_MENU(MENU_PITCH_WOMAN, 3000, 0, NULL);				
        break;
    case KEY_OK_MIC_RESET:
	     mic_voicechange_reset();
            UI_SHOW_MENU(MENU_PITCH_OFF, 3000, 0, NULL);				
        break;
    case KEY_OK_MIC_LOOP:
	     mic_voicechange_loop();
            UI_SHOW_MENU(MENU_PITCH_OFF, 3000, 0, NULL);				
        break;

#ifdef SUPPORT_TONE_VOL_CONTROL
    case KEY_TONE_VOL_UP:
		
#ifdef SUPPORT_MIC_VOICE_CHANGER_KEY
	mic_pitchv_up();
	break;
#endif	
        if(use_ctl.tws_key_disable==0)break;

		if(use_ctl.mute_flag)
			{
			        use_ctl.mute_flag = 0;
			         pa_unmute();
			}

        if(use_ctl.bToneVol<LED_SHOW_MAX_TONE_VOL)use_ctl.bToneVol++;
		
	app_audio_set_volume(APP_AUDIO_STATE_WTONE, use_ctl.bToneVol, 1);
	UI_SHOW_MENU(MENU_TONE_VOL, 1000, app_audio_get_volume(APP_AUDIO_STATE_WTONE), NULL);

        break;

    case KEY_TONE_VOL_DOWN:
        if(use_ctl.tws_key_disable==0)break;
		

	 #ifdef SUPPORT_MIC_VOICE_CHANGER_KEY
	 	mic_pitchv_down();
	 	break;
	 #endif	
		
        if(use_ctl.bToneVol)use_ctl.bToneVol--;
		
		if(use_ctl.mute_flag && (use_ctl.bToneVol>1))
			{
			        use_ctl.mute_flag = 0;
			         pa_unmute();
			}	
	app_audio_set_volume(APP_AUDIO_STATE_WTONE, use_ctl.bToneVol, 1);
	UI_SHOW_MENU(MENU_TONE_VOL, 1000, app_audio_get_volume(APP_AUDIO_STATE_WTONE), NULL);
        break;
		
#endif

#ifdef SUPPORT_WH_VOICE

    case KEY_GANXIE:
        printf("\n--111--KEY_GANXIE----\n");		
          tone_play_by_path(tone_table[IDEX_TONE_GANXIE], 1);
 	 break;
    case KEY_HECAI:
          tone_play_by_path(tone_table[IDEX_TONE_HECAI], 1);
 	 break;
    case KEY_KOUSHAO:
           tone_play_by_path(tone_table[IDEX_TONE_KOUSHAO], 1);
	 break;
    case KEY_ZHANGSHENG:
          tone_play_by_path(tone_table[IDEX_TONE_ZHANGSHENG], 1);
 	 break;
    case KEY_KAICHANG:
          tone_play_by_path(tone_table[IDEX_TONE_KAICHANG], 1);
 	 break;
    case KEY_JIQIANG:
          tone_play_by_path(tone_table[IDEX_TONE_JIQIANG], 1);
 	 break;
    case KEY_BISHI:
          tone_play_by_path(tone_table[IDEX_TONE_BISHI], 1);
 	 break;
    case KEY_MOMODA:
          tone_play_by_path(tone_table[IDEX_TONE_MOMODA], 1);
 	 break;
    case KEY_DAERGUANG:
          tone_play_by_path(tone_table[IDEX_TONE_DAERGUANG], 1);
 	 break;	 
 #endif


#ifdef SUPPORT_AMP_MUTE_KEY
    case KEY_AMP_MUTE:
        if(use_ctl.mute_flag)
        {
            use_ctl.mute_flag = 0;
	     pa_unmute();
        }
        else
        {
            use_ctl.mute_flag = 1;
            pa_mute();
        }		
        break;
		
#endif


#ifdef SUPPORT_init_RGB_MODE_15s_change

	case KEY_RGB_MODE:
		
		extern u8 light_mode_mark;
		u8 light_mode;
		if(light_mode_mark)
		{
			if (light_mode_mark & 0x10)
			{
				light_mode_mark=1;
			}
			else
			{
				light_mode_mark&=0x0f;
				light_mode_mark++;
			}
			light_mode=light_mode_mark+1;
			if(light_mode_mark>=RGB_MAX)
			{
				light_mode_mark =	0x10 | 1;
				light_mode=1;
			}
		}
		else
		{
			light_mode_mark =	0x10 | 1;
			light_mode=1;
		}
		UI_SHOW_MENU(MENU_RGBmode_SHOW, 3000, light_mode, NULL);
		break;
#else

#ifdef SUPPORT_RGB_MODE_KEY
    case KEY_RGB_MODE:
	extern u8 light_mode_mark;
	
	light_mode_mark++;
	if(light_mode_mark>=RGB_MAX)
	{
		light_mode_mark=0;
	}
	UI_SHOW_MENU(MENU_RGB_SHOW, 3000, 0, NULL);
	break;
#endif

#endif

#ifdef SUPPORT_VOICE_EN_OFF
    case KEY_VOICE_ONOFF:

	if(use_ctl.bVoiceFlag==0)
	{
		use_ctl.bVoiceFlag=1;
	}
	else
		use_ctl.bVoiceFlag=0;
	
         UI_SHOW_MENU(MENU_VOICE_ONOFF, 3000, 0, NULL);

 	#ifdef SUPPORT_VM_VOICE_ONOFF
 		 use_ctl.vVmVoiceOnoffCnt=3;
 	#endif			 
		 
	 #ifdef SUPPORT_TONE_VOICE_ONOFF		 
		if(use_ctl.bVoiceFlag==0)
		{
	           tone_play_by_path(tone_table[IDEX_TONE_VOICE_OFF], 1);
		}
		else
		{
	           tone_play_by_path(tone_table[IDEX_TONE_VOICE_ON], 1);
		}
	 #endif


	break;
#endif


      case  KEY_LOW_POWER:
     #ifdef SUPPORT_LOWPOWER_VOICE
	   #ifdef SUPPORT_VOICE_EN_OFF		 
	   #ifdef SUPPORT_TONE_LOWPOWER_N
		if(use_ctl.bVoiceFlag==0)  
		{			
     		tone_play_by_path(tone_table[IDEX_TONE_LOW_POWER_N], 1);	
		}else
	    #endif
	    #endif					 
       	tone_play_by_path(tone_table[IDEX_TONE_LOW_POWER], 1);	
     #endif
         break;
/*----------------------------------------------------------
		customer add  msg
----------------------------------------------------------*/
	 
#endif // SUPPORT_KPH_DEF



#if TCFG_APP_BT_EN

#if TCFG_USER_TWS_ENABLE
    case KEY_TWS_CONN:
        log_info("    KEY_TWS_CONN \n");
        bt_open_tws_conn(0);
        break;
    case KEY_TWS_DISCONN:
        log_info("    KEY_TWS_DISCONN \n");
        bt_disconnect_tws_conn();
        break;
    case KEY_TWS_REMOVE_PAIR:
        log_info("    KEY_TWS_REMOVE_PAIR \n");
        bt_tws_remove_tws_pair();
        break;
    case KEY_TWS_SEARCH_PAIR:
        log_info("    KEY_TWS_SEARCH_PAIR \n");
        bt_tws_start_search_and_pair();
        break;


    case KEY_TWS_SEARCH_REMOVE_PAIR:
	log_info("    KEY_TWS_SEARCH_REMOVE_PAIR \n");

	#if 1
	//kph_add_start
	if (get_bt_tws_connect_status()) 
	{
		use_ctl.tws_start_connect_flag=0;
		bt_tws_remove_tws_pair();
		bt_disconnect_tws_conn();
		
		tone_play_by_path(tone_table[IDEX_TONE_TWS_DISCONN], 0);	//断开连接
	}
	else
	{
		bt_tws_remove_tws_pair();
		bt_disconnect_tws_conn();

	
		
		#ifdef SUPPORT_TWS_NO_CONNECT_SW
		if(use_ctl.tws_start_connect_flag)
		{
			use_ctl.tws_start_connect_flag=0;
			bt_tws_close_search_and_pair();
		}
		else
		#endif
		{
			use_ctl.tws_start_connect_flag=1;
			bt_tws_start_search_and_pair();
		}
		tone_play_by_path(tone_table[IDEX_TONE_TWS_CONN], 0);		//连接
	}
	#else
	bt_tws_search_or_remove_pair();
	#endif
		UI_SHOW_MENU(MENU_TWS, 3000, 0, NULL);
        break;
#endif




    case KEY_BT_DIRECT_INIT:
        bt_direct_init();
        break;
    case KEY_BT_DIRECT_CLOSE:
        bt_direct_close();
        break;
#endif


	#ifdef SUPPORT_LED7_UART_TEST_IO
	case KEY_LED7_IO_TEST:
		use_ctl.temp_8bit_led_data++;
		printf("\nled7_dat=%x\n",use_ctl.temp_8bit_led_data);
		break;
	case KEY_LED7_TEST_UP:
		use_ctl.temp_8bit_led_data++;
		printf("\nled7_dat=%x\n",use_ctl.temp_8bit_led_data);
		break;
	case KEY_LED7_TEST_DN:
		use_ctl.temp_8bit_led_data--;
		printf("\nled7_dat=%x\n",use_ctl.temp_8bit_led_data);
		break;
	#endif




    case  KEY_POWEROFF:
    case  KEY_POWEROFF_HOLD:
        power_off_deal(event, key_event - KEY_POWEROFF);
        break;

    case KEY_IR_NUM_0:
    case KEY_IR_NUM_1:
    case KEY_IR_NUM_2:
    case KEY_IR_NUM_3:
    case KEY_IR_NUM_4:
    case KEY_IR_NUM_5:
    case KEY_IR_NUM_6:
    case KEY_IR_NUM_7:
    case KEY_IR_NUM_8:
    case KEY_IR_NUM_9:
#ifdef SUPPORT_KPH_DEF
        if ((app_get_curr_task() == APP_MUSIC_TASK)||(app_get_curr_task() == APP_FM_TASK))
        {
           key_event = key_event-KEY_IR_NUM_0;
           use_ctl.ir_num = use_ctl.ir_num*10+(key_event&0x0f);
	       use_ctl.input_number_cnt++;
	        if( (use_ctl.ir_num>9999)||(use_ctl.input_number_cnt>4) )
	        {
	           use_ctl.ir_num = (key_event&0x0f);
	           use_ctl.input_number_cnt = 0;
	        }
	        use_ctl.input_time_out = 4;
		 #ifdef SUPPORT_VOICE_EN_OFF		 
		 #ifdef SUPPORT_NUM_VOICE
		 	if(use_ctl.bVoiceFlag)
		 		tone_play_by_path(tone_table[IDEX_TONE_NUM_0+key_event], 1);
		 #endif			
		 #endif								
	        
        }
        printf("\n ir==%d", use_ctl.ir_num);
#else
        input_number_deal(key_event - KEY_IR_NUM_0);
#endif
        break;




	#ifdef SUPPORT_SPDIF_HDMI_CHECK
		case KEY_HDMI_IN:
			if((app_curr_task == APP_SPDIF_TASK))
			{
				use_ctl.bSpdifMode=SPDIF_HDMI;
				app_common_key_var_event(KEY_SPDIF_SW_SOURCE);
			}
			else
			{
				use_ctl.bSpdifMode=SPDIF_HDMI;
				app_task_switch_to(APP_SPDIF_TASK);
			}
			break;
		case KEY_HDMI_OUT:
			if ((app_curr_task == APP_SPDIF_TASK) && (use_ctl.bSpdifMode==SPDIF_HDMI))
			{
				#ifdef SUPPORT_SPDIF_HDMI_MODE_OUT_TO_MODE
				app_task_switch_to(SUPPORT_SPDIF_HDMI_MODE_OUT_TO_MODE);
				#else
				app_common_key_var_event(KEY_CHANGE_MODE);
				#endif
			}
			break;
	#endif


	case KEY_CHANGE_MODE:
	#if (TCFG_DEC2TWS_ENABLE)
			if (!key->init) 
			{
				break;
			}
	#endif

	use_ctl.is_forward=1;

	#ifdef 	SUPPORT_USER_SPDIF_MODE
	if (app_get_curr_task() == APP_SPDIF_TASK)
	{
		use_ctl.bSpdifMode++;
		if (use_ctl.bSpdifMode >= MAX_SPDIF_MODE)
		{
			use_ctl.bSpdifMode = 0;
		}
		else
		{
			#if defined SUPPORT_SPDIF_HDMI_CHECK && !defined SUPPORT_ENTER_HDMI_NOT_CHECK
			if (use_ctl.bSpdifMode == SPDIF_HDMI)
			{
				if (get_hdmi_online())
				{
					app_common_key_var_event(KEY_SPDIF_SW_SOURCE);
					break;
				}
				else
				{
					use_ctl.bSpdifMode++;
					if (use_ctl.bSpdifMode >= MAX_SPDIF_MODE)
					{
						use_ctl.bSpdifMode = 0;
					}
					else
					{
						app_common_key_var_event(KEY_SPDIF_SW_SOURCE);
						break;
					}
				}
			}
			else
			#endif
			{
				app_common_key_var_event(KEY_SPDIF_SW_SOURCE);
				break;
			}
		}
	}
	use_ctl.bSpdifMode = 0;
	#endif

			
	#ifdef SUPPORT_TWS_DEFAULT_CHANGE_MODE
		if (((u32)event->arg == SYS_BT_EVENT_FROM_TWS))break;
	#endif
	
	
	#ifdef SUPPORT_MULT_INPUT_IN_LINEIN        
			if(app_get_curr_task() == APP_LINEIN_TASK)
			{
			   use_ctl.mult_mode++;
			   //printf("\n auxmode = %d",use_ctl.mult_mode);
			   if(use_ctl.mult_mode < MODE_MAX)
			   {
				   app_common_key_var_event(KEY_LINEIN_START);
				   break;
			   }
			}
			use_ctl.mult_mode = 0;
	#endif
	
	
		if (app_get_curr_task() == APP_MUSIC_TASK)
		{
			use_ctl.music_dev++;
			if(dev_manager_get_total(1)>1)
			{
				if(use_ctl.music_dev==1)
				{
					app_common_key_var_event(KEY_MUSIC_CHANGE_DEV);
					break;
				}
			}
		}
		use_ctl.music_dev  = 0;
	
	
	#if TWFG_APP_POWERON_IGNORE_DEV
			if ((timer_get_ms() - app_var.start_time) > TWFG_APP_POWERON_IGNORE_DEV)
	#endif//TWFG_APP_POWERON_IGNORE_DEV
	
			{
				printf("KEY_CHANGE_MODE\n");
				app_task_switch_next();
			}
			break;



	case KEY_MODE_PREV:
	#if (TCFG_DEC2TWS_ENABLE)
        if (!key->init) 
		{
            break;
        }
	#endif

	use_ctl.is_forward=0;

	#ifdef 	SUPPORT_USER_SPDIF_MODE
	if (app_get_curr_task() == APP_SPDIF_TASK)
	{
		use_ctl.bSpdifMode--;
		if (use_ctl.bSpdifMode < 0)
		{
			use_ctl.bSpdifMode = MAX_SPDIF_MODE - 1;
		}
		else
		{
			#if defined SUPPORT_SPDIF_HDMI_CHECK && !defined SUPPORT_ENTER_HDMI_NOT_CHECK
			if (use_ctl.bSpdifMode == SPDIF_HDMI)
			{
				if (get_hdmi_online())
				{
					app_common_key_var_event(KEY_SPDIF_SW_SOURCE);
					break;
				}
				else
				{
					use_ctl.bSpdifMode--;
					if (use_ctl.bSpdifMode <= 0)
					{
						use_ctl.bSpdifMode = MAX_SPDIF_MODE - 1;
					}
					else
					{
						app_common_key_var_event(KEY_SPDIF_SW_SOURCE);
						break;
					}
				}
			}
			else
			#endif
			{
				app_common_key_var_event(KEY_SPDIF_SW_SOURCE);
				break;
			}
		}
	}
	use_ctl.bSpdifMode = 0;
	#endif


#ifdef SUPPORT_TWS_DEFAULT_CHANGE_MODE
	if (((u32)event->arg == SYS_BT_EVENT_FROM_TWS))break;
#endif


    if (app_get_curr_task() == APP_MUSIC_TASK)
    {
        use_ctl.music_dev++;
        if(dev_manager_get_total(1)>1)
        {
            if(use_ctl.music_dev==1)
			{
                app_common_key_var_event(KEY_MUSIC_CHANGE_DEV);
                break;
            }
        }
    }
    use_ctl.music_dev  = 0;


#if TWFG_APP_POWERON_IGNORE_DEV
        if ((timer_get_ms() - app_var.start_time) > TWFG_APP_POWERON_IGNORE_DEV)
#endif

        {
            app_task_switch_prev();
        }
        break;	






    case KEY_VOL_UP:
        log_info("COMMON KEY_VOL_UP\n");

        #ifdef SUPPORT_VOL_KEY_CANCEL_MUTE_STATE
		if(use_ctl.mute_flag)
			{
			        use_ctl.mute_flag = 0;
			         pa_unmute();
			}
	#endif
		 #ifdef TWS_SYNC_EQ
		 	if(((u32)event->arg==KEY_EVENT_FROM_TWS)||((u32)event->arg==SYS_BT_EVENT_FROM_TWS)){
		 #ifdef dy_DS4751C
		 	UI_SHOW_MENU(MENU_MAIN_VOL, 1000, app_audio_get_volume(APP_AUDIO_CURRENT_STATE), NULL);
		 #endif
		 
		 	break;
		 	}
		 						 
		 #endif
	 #ifdef LED_SHOW_MAX_VOL
	 	if(use_ctl.led_vol<LED_SHOW_MAX_VOL)
			use_ctl.led_vol++;
	 #endif
	 #ifdef TWS_SYNC_EQ
	    bt_tws_sync_volume();
	 #endif

	 #ifdef SUPORT_MENU_MODE
	       use_ctl.menu_mode=MENU_VOL_MODE;
	 	use_ctl.menu_mode_cnt = 6;	 
	 #endif		
		
        if (!tone_get_status()) 
		{
            app_audio_volume_up(1);
            printf("common vol+: %d", app_audio_get_volume(APP_AUDIO_CURRENT_STATE));
        }
        if (app_audio_get_volume(APP_AUDIO_CURRENT_STATE) == app_audio_get_max_volume()) 
		{
            if (tone_get_status() == 0) 
			{
		 #if TCFG_MAX_VOL_PROMPT
		             tone_play_by_path(tone_table[IDEX_TONE_MAX_VOL], 0);
		 #endif
            }
        }

	#ifdef SUPPORT_MIC_PR_CHECK
            use_ctl.led_vol_pr = use_ctl.led_vol;
            vol_backup_temp = use_ctl.led_vol;
            vol_obj_val = use_ctl.led_vol;
	#endif		

	#ifdef SUPPORT_VM_VOL
	use_ctl.vVolCnt = 3;
	#endif

#if (TCFG_DEC2TWS_ENABLE)
        bt_tws_sync_volume();
#endif

		if(use_ctl.led_vol>= LED_SHOW_MAX_VOL-1)
			tone_play_by_path(tone_table[IDEX_TONE_MAX_VOL], 0);


        UI_SHOW_MENU(MENU_MAIN_VOL, 1000, app_audio_get_volume(APP_AUDIO_CURRENT_STATE), NULL);
        break;

    case KEY_VOL_DOWN:
        log_info("COMMON KEY_VOL_DOWN\n");

		#ifdef SUPPORT_VM_VOL
		use_ctl.vVolCnt = 3;
		#endif


        #ifdef SUPPORT_VOL_KEY_CANCEL_MUTE_STATE
		if(use_ctl.mute_flag && (use_ctl.led_vol>1))
			{
			        use_ctl.mute_flag = 0;
			         pa_unmute();
			}
	#endif
#ifdef TWS_SYNC_EQ
		if(((u32)event->arg==KEY_EVENT_FROM_TWS)||((u32)event->arg==SYS_BT_EVENT_FROM_TWS))
		{
#ifdef dy_DS4751C
								   UI_SHOW_MENU(MENU_MAIN_VOL, 1000, app_audio_get_volume(APP_AUDIO_CURRENT_STATE), NULL);
#endif
			break;
		}
								  
#endif
#ifdef LED_SHOW_MAX_VOL
		   if(use_ctl.led_vol)use_ctl.led_vol--;
#endif
#ifdef TWS_SYNC_EQ
					  bt_tws_sync_volume();
#endif

	 #ifdef SUPORT_MENU_MODE
	       use_ctl.menu_mode=MENU_VOL_MODE;
	 	use_ctl.menu_mode_cnt = 6;	 
	 #endif		

		
        app_audio_volume_down(1);
        printf("common vol-: %d", app_audio_get_volume(APP_AUDIO_CURRENT_STATE));
#if (TCFG_DEC2TWS_ENABLE)
        bt_tws_sync_volume();
#endif
	#ifdef SUPPORT_MIC_PR_CHECK
            use_ctl.led_vol_pr = use_ctl.led_vol;
            vol_backup_temp = use_ctl.led_vol;
            vol_obj_val = use_ctl.led_vol;
	#endif		

	if(use_ctl.led_vol<= 0)
		tone_play_by_path(tone_table[IDEX_TONE_MAX_VOL], 0);

        UI_SHOW_MENU(MENU_MAIN_VOL, 1000, app_audio_get_volume(APP_AUDIO_CURRENT_STATE), NULL);
        break;

    case  KEY_EQ_MODE:
		
	//         printf("eq mode==%d",use_ctl.eq_mode);

	

		 #if(TCFG_EQ_ENABLE == 1)
	 	 #ifdef SUPPORT_MULT_EQ_FILE
	 		extern void user_eq_change(u8 flag);
	 
	 	     if(use_ctl.eq_mode==5)		
	 	     	{
	 		        use_ctl.eq_mode=0;
	 	 		 user_eq_change(use_ctl.eq_mode);
	 	     	}
	 		 else
	 	 #endif		 
		         eq_mode_sw();
		 #endif
		 #ifdef SUPPORT_HW_EQ_MODE
		 	UI_SHOW_MENU(MENU_SET_EQ, 3000, eq_mode_get_cur(), NULL);	
		 #endif		   


	   #ifdef SUPPORT_VOICE_EN_OFF		 
	   #ifdef SUPPORT_TONE_EQ
		if(use_ctl.bVoiceFlag)  
			{			
		           tone_play_by_path(tone_table[IDEX_TONE_EQ_NORMAL+use_ctl.eq_mode], 1);   
			}
	    #endif
	    #endif				

		
        break;
		
#if (AUDIO_OUTPUT_WAY == AUDIO_OUTPUT_WAY_BT)


    case KEY_BT_EMITTER_RECEIVER_SW:
        printf(" KEY_BT_EMITTER_RECEIVER_SW\n");
        bt_emitter_receiver_sw();
        break;

    case KEY_BT_EMITTER_PAUSE:
        r_printf(" KEY_BT_EMITTER_PAUSE\n");
        bt_emitter_pp(0);
        break;

    case KEY_BT_EMITTER_PLAY:
        r_printf(" KEY_BT_EMITTER_PLAY\n");
        bt_emitter_pp(1);
        break;

    case KEY_BT_EMITTER_SW:
        r_printf("KEY_BT_EMITTER_SW\n");
        {
            extern u8 bt_emitter_stu_sw(void);

            if (bt_emitter_stu_sw()) {
                printf("bt emitter start \n");
            } else {
                printf("bt emitter stop \n");
            }
        }
        break;
#endif


#if (TCFG_CHARGE_BOX_ENABLE)
    case  KEY_BOX_POWER_CLICK:
    case  KEY_BOX_POWER_LONG:
    case  KEY_BOX_POWER_HOLD:
    case  KEY_BOX_POWER_UP:
    case  KEY_BOX_POWER_DOUBLE:
    case  KEY_BOX_POWER_THREE:
    case  KEY_BOX_POWER_FOUR:
    case  KEY_BOX_POWER_FIVE:
        charge_box_key_event_handler(key_event);
        break;
#endif
#if (TCFG_MIC_EFFECT_ENABLE)
    case KEY_REVERB_SWITCH:
        switch_mic_effect_mode();
        break;
    case KEY_REVERB_OPEN:
		
	 #if  0//TCFG_USER_TWS_ENABLE
	         if (!key->init) {
	             break;
	         }
	 #endif


#ifdef SUPPORT_KPH_DEF

        if(use_ctl.mic_is_online == true)
        {

		mic_effect_start();
		//use_ctl.mic_vol = 30;

	//防止插话筒冲击		
		mic_effect_set_dvol(0);
		mic_effect_set_echo_decay(0);
	    delay2ms(200);   
	//防止插话筒冲击		

		printf("\n=111=  KEY_REVERB_OPEN===%d\n",use_ctl.mic_is_online);

 	   	  use_ctl.vMicVolTemp = mic_vol_table[use_ctl.mic_vol];
	         mic_effect_set_dvol(use_ctl.vMicVolTemp);
 	   	  use_ctl.vMicEchoTemp = mic_echo_table[use_ctl.echo];
	         mic_effect_set_echo_decay(use_ctl.vMicEchoTemp);
		#if TCFG_MIC_TUNNING_EQ_ENABLE
			mic_tunning_eq_update(MIC_EQ_MODE_HIGH_SOUND,mic_tre_table[use_ctl.mic_tre]);
			mic_tunning_eq_update(MIC_EQ_MODE_LOW_SOUND,mic_bass_table[use_ctl.mic_bass]);
		#endif
			 
		printf("\n=2222=  mic_vol-vMicVolTemp=%d-%d\n",use_ctl.mic_vol,use_ctl.vMicVolTemp);

	 	#ifndef MIC_DELAY_IO_AD_EN
	 	        if(use_ctl.echo==0)
				{
	 	            mic_effect_set_echo_delay(0);
	 	        }
				else
				{
	 	           mic_effect_set_echo_delay(MIC_ECHO_DELAY);
	 	        }
		#else
		     mic_effect_set_echo_delay(mic_delay_table[use_ctl.micdelay]);
	 	#endif
		
		#if TCFG_MIC_VOICE_CHANGER_ENABLE
			mic_voicechange_reset();
		#endif
        }
        else
        {
	      mic_effect_set_dvol(0);
	      mic_effect_set_echo_decay(0);
	      delay2ms(200);
          mic_effect_stop(); 
        }
		
#else
        if (mic_effect_get_status()) {
            mic_effect_stop();
        } else {
            mic_effect_start();
        }
#endif		
        ret = true;
        break;
#endif
    case KEY_ENC_START:
#if (RECORDER_MIX_EN)
        if (recorder_mix_get_status()) 
		{
            printf("recorder_encode_stop\n");
            recorder_mix_stop();
            use_ctl.rec_auto_play = true;		
            app_task_switch_to(APP_MUSIC_TASK);
		
        } 
		else 
		{
            if (dev_manager_get_phy_logo(dev_manager_find_active(0))) 
			{
                printf("recorder_encode_start\n");
                recorder_mix_start();
            }
        }
#endif/*RECORDER_MIX_EN*/
        break;

    default:
        ui_key_msg_post(key_event);
        ui_simple_key_msg_post(key_event, key_value);
#if (SOUNDCARD_ENABLE)
        soundcard_event_deal(event);
#endif
        break;

    }
#if (SMART_BOX_EN)
    extern int smartbox_common_key_event_deal(int key_event, int ret);
    ret = smartbox_common_key_event_deal(key_event, ret);
#endif
    return ret;
}

int app_power_user_event_handler(struct device_event *dev)
{
#if(TCFG_SYS_LVD_EN == 1)
    switch (dev->event) 
	{
		case POWER_EVENT_POWER_WARNING:
			ui_update_status(STATUS_LOWPOWER);
			tone_play_by_path(tone_table[IDEX_TONE_LOW_POWER], 1);
			/* return 0; */
    }
	#if (SOUNDCARD_ENABLE)
		soundcard_power_event(dev);
	#endif
#endif
    return app_power_event_handler(dev);
}

static void app_common_device_event_handler(struct sys_event *event)
{
    int ret = 0;
    const char *logo = NULL;
    const char *usb_msg = NULL;
    u8 app  = 0xff ;
    u8 alarm_flag = 0;
    switch ((u32)event->arg) {
#if (TCFG_SOUNDBOX_TOOL_ENABLE || TCFG_ONLINE_ENABLE)
    case DEVICE_EVENT_FROM_SOUNDBOX_TOOL:
        extern int app_soundbox_tool_event_handler(struct soundbox_tool_event * soundbox_tool_dev);
        app_soundbox_tool_event_handler(&event->u.soundbox_tool);
        break;
#endif
#if TCFG_CHARGE_ENABLE
    case DEVICE_EVENT_FROM_CHARGE:
        app_charge_event_handler(&event->u.dev);
        break;
#endif//TCFG_CHARGE_ENABLE

#if (TCFG_ONLINE_ENABLE || TCFG_SOUNDBOX_TOOL_ENABLE)
    case DEVICE_EVENT_FROM_CI_UART:
        ci_data_rx_handler(CI_UART);
        break;

#if TCFG_USER_TWS_ENABLE
    case DEVICE_EVENT_FROM_CI_TWS:
        ci_data_rx_handler(CI_TWS);
        break;
#endif//TCFG_USER_TWS_ENABLE
#endif//TCFG_ONLINE_ENABLE

    case DEVICE_EVENT_FROM_POWER:
        app_power_user_event_handler(&event->u.dev);
        break;

#if TCFG_CHARGESTORE_ENABLE || TCFG_TEST_BOX_ENABLE
    case DEVICE_EVENT_CHARGE_STORE:
        app_chargestore_event_handler(&event->u.chargestore);
        break;
#endif//TCFG_CHARGESTORE_ENABLE || TCFG_TEST_BOX_ENABLE

#if(TCFG_CHARGE_BOX_ENABLE)
    case DEVICE_EVENT_FROM_CHARGEBOX:
        charge_box_ctrl_event_handler(&event->u.chargebox);
        break;
#endif


    case DEVICE_EVENT_FROM_OTG:

	 #ifdef SUPPORT_CD4051_PONTEN
	 	use_ctl.bCd4051ModeDly = 40;
	 #endif
		
        ///先分析OTG设备类型
        usb_msg = (const char *)event->u.dev.value;
        if (usb_msg[0] == 's') {
            ///是从机
#if TCFG_PC_ENABLE
            ret = pc_device_event_handler(event);
            if (ret == true) {
                app = APP_PC_TASK;
            }
#endif
            break;
        } else if (usb_msg[0] == 'h') {
            ///是主机, 统一于SD卡等响应主机处理，这里不break
        } else {
            log_e("unknow otg devcie !!!\n");
            break;
        }
    case DRIVER_EVENT_FROM_SD0:
    case DRIVER_EVENT_FROM_SD1:
    case DRIVER_EVENT_FROM_SD2:
		
#ifdef SUPPORT_CD4051_PONTEN
	use_ctl.bCd4051ModeDly = 40;
#endif

		
#if TCFG_APP_MUSIC_EN
        ret = dev_status_event_filter(event);///解码设备上下线， 设备挂载等处理
        if (ret == true) 
		{
            if (event->u.dev.event == DEVICE_EVENT_IN) 
			{
                //设备上线， 非解码模式切换到解码模式播放
                if (app_get_curr_task() != APP_MUSIC_TASK) 
				{
                    app = APP_MUSIC_TASK;
						#if 1
						    if((u32)event->arg == DEVICE_EVENT_FROM_OTG)
							{
								use_ctl.music_device = 1;
							}
							else
							{
								use_ctl.music_device = 2;
							}
						#endif
                }
            }
        }
#endif
        break;

#if TCFG_APP_LINEIN_EN
    case DEVICE_EVENT_FROM_LINEIN:
        ret = linein_device_event_handler(event);
        if (ret == true) 
		{
            app = APP_LINEIN_TASK;
        }
        break;
#endif//TCFG_APP_LINEIN_EN

#if TCFG_APP_RTC_EN
    case DEVICE_EVENT_FROM_ALM:
        ret = alarm_sys_event_handler(event);
        if (ret == true) {
            alarm_flag = 1;
            app = APP_RTC_TASK;
        }
        break;
#endif//TCFG_APP_RTC_EN

    default:
        /* printf("unknow SYS_DEVICE_EVENT!!, %x\n", (u32)event->arg); */
        break;
    }

#if (SMART_BOX_EN)
    extern void smartbox_update_dev_state(u32 event);
    smartbox_update_dev_state((u32)event->arg);
#endif

    if (app != 0xff) {
        if ((true != app_check_curr_task(APP_PC_TASK)) || alarm_flag) {

            //PC 不响应因为设备上线引发的模式切换
#if TWFG_APP_POWERON_IGNORE_DEV
            if ((timer_get_ms() - app_var.start_time) > TWFG_APP_POWERON_IGNORE_DEV)
#endif//TWFG_APP_POWERON_IGNORE_DEV
            {
#if (TCFG_CHARGE_ENABLE && (!TCFG_CHARGE_POWERON_ENABLE))
                extern u8 get_charge_online_flag(void);
                if (get_charge_online_flag()) {

                } else
#endif
                {

                    printf(">>>>>>>>>>>>>%s %d \n", __FUNCTION__, __LINE__);
                    app_task_switch_to(app);
                }
            }
        }
    }
}


///公共事件处理， 各自模式没有处理的事件， 会统一在这里处理
void app_default_event_deal(struct sys_event *event)
{
    int ret;
    SYS_EVENT_HANDLER_SPECIFIC(event);
    switch (event->type) 
	{
		case SYS_DEVICE_EVENT:
			/*默认公共设备事件处理*/
			/* printf(">>>>>>>>>>>>>%s %d \n", __FUNCTION__, __LINE__); */
			app_common_device_event_handler(event);
			break;
	#if TCFG_APP_BT_EN
		case SYS_BT_EVENT:
			if (true != app_check_curr_task(APP_BT_TASK))
			{
				/*默认公共BT事件处理*/
				bt_background_event_handler(event);
			}
			break;
	#endif
		case SYS_KEY_EVENT:
			app_common_key_msg_deal(event);
			break;
		default:
			printf("unknow event\n");
			break;
    }
}


#if 0
extern int key_event_remap(struct sys_event *e);
extern const u16 bt_key_ad_table[KEY_AD_NUM_MAX][KEY_EVENT_MAX];
u8 app_common_key_var_2_event(u32 key_var)
{
    u8 key_event = 0;
    u8 key_value = 0;
    struct sys_event e = {0};
#if TCFG_ADKEY_ENABLE
    for (; key_value < KEY_AD_NUM_MAX; key_value++) {
        for (key_event = 0; key_event < KEY_EVENT_MAX; key_event++) {
            if (bt_key_ad_table[key_value][key_event] == key_var) {
                e.type = SYS_KEY_EVENT;
                e.u.key.type = KEY_DRIVER_TYPE_AD;
                e.u.key.event = key_event;
                e.u.key.value = key_value;
                /* e.u.key.tmr = timer_get_ms(); */
                e.arg  = (void *)DEVICE_EVENT_FROM_KEY;
                /* printf("key2event:%d %d %d\n", key_var, key_value, key_event); */
                if (key_event_remap(&e)) {
                    sys_event_notify(&e);
                    return true;
                }
            }
        }
    }
#endif
    return false;
}

#else

u8 app_common_key_var_2_event(u32 key_var)
{
    struct sys_event e = {0};
    e.type = SYS_KEY_EVENT;
    e.u.key.type = KEY_DRIVER_TYPE_SOFTKEY;
    e.u.key.event = key_var;
    e.arg  = (void *)DEVICE_EVENT_FROM_KEY;
    sys_event_notify(&e);
    return true;
}
#endif

