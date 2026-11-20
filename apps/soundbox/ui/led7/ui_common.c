#include "includes.h"
#include "ui/ui_api.h"
#include "fm_emitter/fm_emitter_manage.h"
#ifdef SUPPORT_KPH_DEF
#include "user_cfg.h"
#endif

#if (TCFG_UI_ENABLE&&(CONFIG_UI_STYLE == STYLE_JL_LED7))
static void led7_show_hi(void *hd)
{
    LCD_API *dis = (LCD_API *)hd;
    dis->lock(1);
    dis->clear();
    dis->setXY(0, 0);
    dis->show_string((u8 *)" HI");
    dis->lock(0);
}
static void led7_show_volume(void *hd, u8 vol)
{
    LCD_API *dis = (LCD_API *)hd;
    dis->lock(1);
    dis->clear();
    dis->setXY(0, 0);
    dis->show_char('V');
    dis->show_char(' ');	
    #ifdef LED_SHOW_MAX_VOL
    vol = use_ctl.led_vol;
    #endif	
    dis->show_number(vol / 10);
    dis->show_number(vol % 10);
    dis->lock(0);
}

static void led7_show_char_music_dev(void *hd, u8 vol)
{
    LCD_API *dis = (LCD_API *)hd;
    dis->lock(1);
    dis->clear();
    dis->setXY(0, 0);
	if (vol)
	{
		dis->show_string((u8 *)" usb");
	}
	else
	{
		dis->show_string((u8 *)" tf ");
	}
    dis->lock(0);
}

static void led7_show_wait(void *hd)
{
    LCD_API *dis = (LCD_API *)hd;
    dis->lock(1);
    dis->clear();
    dis->setXY(0, 0);
    dis->show_string((u8 *)" Lod");
    dis->lock(0);
}

static void led7_show_bt(void *hd)
{

    LCD_API *dis = (LCD_API *)hd;
    dis->lock(1);
    dis->clear();
    dis->setXY(0, 0);
    dis->show_string((u8 *)" bt");
    dis->lock(0);
}

#if TCFG_APP_FM_EMITTER_EN
static void led7_fm_ir_set_freq(void *hd, u16 freq)
{

    LCD_API *dis = (LCD_API *)hd;
    u8 bcd_number[5] = {0};	  ///<换算结果显示缓存
    dis->lock(1);
    dis->clear();
    dis->setXY(0, 0);
    sprintf((char *)bcd_number, "%4d", freq);
    /* itoa4(freq,bcd_number); */
    if (freq > 1080) {
        dis->show_string((u8 *)" Err");
    } else if (freq >= 875) {
        dis->show_string(bcd_number);
        /* os_time_dly(100); */
        fm_emitter_manage_set_fre(freq);
        UI_REFLASH_WINDOW(TRUE);//设置回主页
    } else {
        dis->FlashChar(BIT(0) | BIT(1) | BIT(2) | BIT(3)); //设置闪烁
        dis->show_string(bcd_number);
    }
    dis->lock(0);

}
#endif


static void led7_fm_set_freq(void *hd, u32 arg)
{
    u8 bcd_number[5] = {0};
    LCD_API *dis = (LCD_API *)hd;
    u16 freq = 0;
    freq = arg;
    dis->lock(1);
    dis->clear();
    dis->setXY(0, 0);
    dis->FlashChar(BIT(0) | BIT(1) | BIT(2) | BIT(3)); //设置闪烁
    itoa4(freq, (u8 *)bcd_number);
    if (freq > 999 && freq <= 1999) {
        bcd_number[0] = '1';
    } else {
        bcd_number[0] = ' ';
    }
    dis->show_string(bcd_number);
    dis->show_icon(LED7_DOT);
    dis->lock(0);
}

#ifdef SUPPORT_KPH_DEF


 #ifdef SUPPORT_CHANGE_PANEL_FLASH_ERRO	
void led7_show_nop(void *hd)
{

    LCD_API *dis = (LCD_API *)hd;
    dis->lock(1);
    dis->clear();
    dis->setXY(0, 0);
    dis->show_string((u8 *)"    ");
    dis->lock(0);
}
#endif

#ifdef SUPPORT_TONE_VOL_CONTROL
static void led7_show_tone_volume(void *hd, u8 vol)
{
    LCD_API *dis = (LCD_API *)hd;
    dis->lock(1);
    dis->clear();
    dis->setXY(0, 0);
    dis->show_char(' ');
    dis->show_char('u');
    vol = use_ctl.bToneVol;
    dis->show_number(vol / 10);
    dis->show_number(vol % 10);
    dis->lock(0);
}
#endif

// static void led7_show_rgb_mode(void *hd, u8 vol)
// {
//     LCD_API *dis = (LCD_API *)hd;
//     dis->lock(1);
//     dis->clear();
//     dis->setXY(0, 0);
// 	if (vol==1)
// 	{
// 		dis->show_string((u8 *)"AUTO");
// 	}
// 	else if (vol)       //vol>1
// 	{
// 		dis->show_char('L');
// 		dis->show_char('d');
// 		dis->show_number((vol-1) / 10);
// 		dis->show_number((vol-1) % 10);
// 	}
// 	else                //vol == 0
// 	{
// 		dis->show_string((u8 *)" off");
// 	}
	
//     dis->lock(0);
// }

static void led7_show_rgb_mode(void *hd, u8 vol)
{
    LCD_API *dis = (LCD_API *)hd;
    dis->lock(1);
    dis->clear();
    dis->setXY(0, 0);

	if (vol>1)      
	{
		dis->show_char('L');
		dis->show_char('d');
		dis->show_number((vol-1) / 10);
		dis->show_number((vol-1) % 10);
	}
	else                //vol == 0
	{
		dis->show_string((u8 *)" off");
	}
	
    dis->lock(0);
}


#ifdef SUPPORT_RGB_MODE_KEY
extern u8 light_mode_mark;
static void led7_show_rtgb_mode(void *hd)
{
    LCD_API *dis = (LCD_API *)hd;
    dis->lock(1);
    dis->clear();
    dis->setXY(0, 0);
    if(light_mode_mark==0){
       dis->show_string((u8 *)" OFF");
    }
  else {
       dis->show_string((u8 *)"LED");
#ifdef SUPPORT_init_RGB_MODE_15s_change
				dis->show_number((light_mode_mark & 0xf));	   
#else
				dis->show_number(light_mode_mark);	   
#endif
    }
    dis->lock(0);
}
#endif

#ifdef SUPPORT_VOICE_EN_OFF
static void led7_show_voice_onoff(void *hd)
{
    LCD_API *dis = (LCD_API *)hd;
    dis->lock(1);
    dis->clear();
    dis->setXY(0, 0);
    if(use_ctl.bVoiceFlag){
       dis->show_string((u8 *)" ON ");
    }else{
       dis->show_string((u8 *)" OFF");
    }
    dis->lock(0);
}
#endif


static void led7_show_micvol(void *hd)
{
    LCD_API *dis = (LCD_API *)hd;
    dis->lock(1);
    dis->clear();
    dis->setXY(0, 0);
    dis->show_char('N');
    dis->show_char(' ');
    dis->show_number(use_ctl.mic_vol/ 10);
    dis->show_number(use_ctl.mic_vol % 10);
    dis->lock(0);
}

#ifdef JT_VOL_IO_AD_EN
static void led7_show_jtvol(void *hd)
{
    LCD_API *dis = (LCD_API *)hd;
    dis->lock(1);
    dis->clear();
    dis->setXY(0, 0);
    dis->show_char('J');
    dis->show_char('t');
    dis->show_number(use_ctl.jt_vol / 10);
    dis->show_number(use_ctl.jt_vol % 10);
    dis->lock(0);
}
#endif
static void led7_show_echo(void *hd)
{
    LCD_API *dis = (LCD_API *)hd;
    dis->lock(1);
    dis->clear();
    dis->setXY(0, 0);
#ifdef SUPPORT_ECHO_SHOW_EC	
    dis->show_char('E');
    dis->show_char('C');
#else
    dis->show_char('E');
    dis->show_char(' ');
#endif
    dis->show_number(use_ctl.echo/ 10);
    dis->show_number(use_ctl.echo % 10);

    dis->lock(0);
}
#ifdef SUPPORT_MIC_DELAY_SET	
static void led7_show_micdelay(void *hd)
{
    LCD_API *dis = (LCD_API *)hd;
    dis->lock(1);
    dis->clear();
    dis->setXY(0, 0);

    dis->show_char('d');
    dis->show_char('L');
    dis->show_number(use_ctl.micdelay / 10);
    dis->show_number(use_ctl.micdelay % 10);

    dis->lock(0);
}
#endif
#ifdef SUPPORT_MIC_BASS_TRE_SET	
static void led7_show_mic_tre(void *hd)
{
    LCD_API *dis = (LCD_API *)hd;
    dis->lock(1);
    dis->clear();
    dis->setXY(0, 0);
	dis->show_char('H');
static u8 i;

if(use_ctl.mic_tre<MIC_TRE_MAX/2)
	i=MIC_TRE_MAX/2-use_ctl.mic_tre;
else
	i=use_ctl.mic_tre-MIC_TRE_MAX/2;
	
	if(use_ctl.mic_tre < MIC_TRE_MAX/2){
		dis->show_char('-');
	}else{
    	dis->show_char(' ');
	}
	dis->show_number(i / 10);
    dis->show_number(i % 10);	

    dis->lock(0);
}

static void led7_show_mic_bass(void *hd)
{
    LCD_API *dis = (LCD_API *)hd;
    dis->lock(1);
    dis->clear();
    dis->setXY(0, 0);
	dis->show_char('L');
	
static u8 i;

if(use_ctl.mic_bass<MIC_BASS_MAX/2)
	i=MIC_BASS_MAX/2-use_ctl.mic_bass;
else
	i=use_ctl.mic_bass-MIC_BASS_MAX/2;
	
	if(use_ctl.mic_bass < MIC_BASS_MAX/2){
		dis->show_char('-');
	}else{
    	dis->show_char(' ');
	}
	dis->show_number(i / 10);
	dis->show_number(i % 10);	
	
    dis->lock(0);
}		
#endif		   		   

#if SUPPORT_USER_EQ_MODE

static void led7_show_tre(void *hd)
{
    LCD_API *dis = (LCD_API *)hd;
    dis->lock(1);
    dis->clear();
    dis->setXY(0, 0);
	dis->show_char('t');

	static u8 i;
	if(use_ctl.tre<TRE_MAX/2)
		i=TRE_MAX/2-use_ctl.tre;
	else
		i=use_ctl.tre-TRE_MAX/2;
		
		if(use_ctl.tre < TRE_MAX/2){
			dis->show_char('-');
		}else{
	    	dis->show_char(' ');
		}			
		dis->show_number(i / 10);
	    dis->show_number(i % 10);	

    dis->lock(0);
}

static void led7_show_bass(void *hd)
{
    LCD_API *dis = (LCD_API *)hd;
    dis->lock(1);
    dis->clear();
    dis->setXY(0, 0);
#ifdef SUPPORT_BASS_SHOW_L_XX	
    dis->show_char('L');
#else
    dis->show_char('b');
#endif
   	static u8 i;
	if(use_ctl.bass<BASS_MAX/2)
		i=BASS_MAX/2-use_ctl.bass;
	else
		i=use_ctl.bass-BASS_MAX/2;
		
		if(use_ctl.bass < BASS_MAX/2){
			dis->show_char('-');
		}else{
	    	dis->show_char(' ');
		}		
		dis->show_number(i / 10);
	    dis->show_number(i % 10);
    dis->lock(0);
}
#ifdef SUPPORT_MID_SET
static void led7_show_mid(void *hd)
{
    LCD_API *dis = (LCD_API *)hd;
    dis->lock(1);
    dis->clear();
    dis->setXY(0, 0);
	
    dis->show_char('C');

	static u8 i;

	if(use_ctl.mid<MID_MAX/2)
		i=MID_MAX/2-use_ctl.mid;
	else
		i=use_ctl.mid-MID_MAX/2;
		
		if(use_ctl.mid < MID_MAX/2){
			dis->show_char('-');
		}else{
	    	dis->show_char(' ');
		}
		dis->show_number(i / 10);
	    dis->show_number(i % 10);			

    dis->lock(0);
}
#endif


static void led7_show_voice(void *hd)
{
    LCD_API *dis = (LCD_API *)hd;
    dis->lock(1);
    dis->clear();
    dis->setXY(0, 0);
	
    if(use_ctl.bVoiceVal == VOICE_MODE_NULL)
        dis->show_string("NULL");
    else if(use_ctl.bVoiceVal == VOICE_MODE_CH)
        dis->show_string("CHIN");
	else if(use_ctl.bVoiceVal == VOICE_MODE_EN)
        dis->show_string("EN");

    dis->lock(0);
}

#ifdef EQ_MENU
static void led7_show_eq50hz(void *hd)
{
    LCD_API *dis = (LCD_API *)hd;
    dis->lock(1);
    dis->clear();
    dis->setXY(0, 0);
	dis->show_char('b');
	
	u8 i;
	
	if(use_ctl.eq50hz_val<MENU_EQ_MAX_VAL/2){i=MENU_EQ_MAX_VAL/2-use_ctl.eq50hz_val;}
	else {i=use_ctl.eq50hz_val-MENU_EQ_MAX_VAL/2;}
	
	if(use_ctl.eq50hz_val < MENU_EQ_MAX_VAL/2){
		dis->show_char('-');
	}else{
    	dis->show_char(' ');
	}			
	dis->show_number(i / 10);
    dis->show_number(i % 10);	

    dis->lock(0);
}

static void led7_show_eq6khz(void *hd)
{
    LCD_API *dis = (LCD_API *)hd;
    dis->lock(1);
    dis->clear();
    dis->setXY(0, 0);
	dis->show_char('t');
	
	u8 i;
	
	if(use_ctl.eq6khz_val<MENU_EQ_MAX_VAL/2){i=MENU_EQ_MAX_VAL/2-use_ctl.eq6khz_val;}
	else{i=use_ctl.eq6khz_val-MENU_EQ_MAX_VAL/2;}
	
	if(use_ctl.eq6khz_val < MENU_EQ_MAX_VAL/2){
		dis->show_char('-');
	}else{
    	dis->show_char(' ');
	}
	dis->show_number(i / 10);
    dis->show_number(i % 10);	

    dis->lock(0);
}

#endif
#endif

static void led7_show_vol_pr(void *hd)
{
    LCD_API *dis = (LCD_API *)hd;
    dis->lock(1);
    dis->clear();
    dis->setXY(0, 0);
    dis->show_char(' ');
    dis->show_char('V');
    dis->show_number(use_ctl.led_vol_pr/ 10);
    dis->show_number(use_ctl.led_vol_pr % 10);
    dis->lock(0);
}

static void led7_show_bass_mode(void *hd)
{
    LCD_API *dis = (LCD_API *)hd;
    dis->lock(1);
    dis->clear();
    dis->setXY(0, 0);
    if(use_ctl.one_key_bass){
       dis->show_string((u8 *)" ON ");
    }else{
       dis->show_string((u8 *)" OFF");
    }
    dis->lock(0);
}

static void led7_show_usb(void *hd)
{
    LCD_API *dis = (LCD_API *)hd;
    dis->lock(1);
    dis->clear();
    dis->setXY(0, 0);
    dis->show_string((u8 *)" USb");
    dis->lock(0);
}

static void led7_show_sd(void *hd)
{
    LCD_API *dis = (LCD_API *)hd;
    dis->lock(1);
    dis->clear();
    dis->setXY(0, 0);
    #ifdef Support_led7_show_sd_set
    dis->show_string((u8 *)Support_led7_show_sd_set);
    #else
    dis->show_string((u8 *)" Sd ");
    #endif
    dis->lock(0);
}

static void led7_show_del(void *hd)
{
    LCD_API *dis = (LCD_API *)hd;
    dis->lock(1);
    dis->clear();
    dis->setXY(0, 0);
    dis->show_string((u8 *)" dEL");
    dis->lock(0);
}

static void led7_show_mic(void *hd)
{
    LCD_API *dis = (LCD_API *)hd;
    dis->lock(1);
    dis->clear();
    dis->setXY(0, 0);
#ifdef SUPPORT_MIC_PR_SHOW_PRXX	
    if(use_ctl.mic_sw){
       dis->show_string((u8 *)"PRON");
    }else{
       dis->show_string((u8 *)"PROF");
    }
#else
    if(use_ctl.mic_sw){
       dis->show_string((u8 *)"P ON");
    }else{
       dis->show_string((u8 *)"P OF");
    }
#endif	
    dis->lock(0);
}

static void led7_show_tws(void *hd)
{
    LCD_API *dis = (LCD_API *)hd;
    dis->lock(1);
    dis->clear();
    dis->setXY(0, 0);
    #if defined SUPPORT_LED_TWS_SHOW_T_ON_T_OFF
     if(use_ctl.tws_start_connect_flag==0)	
	    dis->show_string((u8 *)"t-OF");
	else
	    dis->show_string((u8 *)"t-ON");		
	
    #elif defined SUPPORT_LED_TWS_SHOW_B_ON_B_OF
		if(use_ctl.bt_tws_key)
		{
		dis->show_string((u8 *)"b-ON");
		}
		else
		{
			dis->show_string((u8 *)"b-OF");
		}
		
    #else
	    dis->show_string((u8 *)" t-S");
    #endif
    dis->lock(0);
}

#if defined SUPPORT_CD4051_PONTEN ||defined SUPPORT_PONTEN_EQ1_EQ5
static void led7_show_eq1(void *hd)
{
    LCD_API *dis = (LCD_API *)hd;
    dis->lock(1);
    dis->clear();
    dis->setXY(0, 0);
   static u8 i;	
      	dis->show_char('E');      
  if(use_ctl.bEq1Val<AD_EQ_MAX/2)
  	i=AD_EQ_MAX/2-use_ctl.bEq1Val;
  else
  	i=use_ctl.bEq1Val-AD_EQ_MAX/2;
  	
  	if(use_ctl.bEq1Val < AD_EQ_MAX/2){
  		dis->show_char('-');
  	}else{
      	dis->show_char(' ');
  	}  	
	dis->show_number( i / 10);	
	dis->show_number(i % 10);	
  
    dis->lock(0);
}
static void led7_show_eq2(void *hd)
{
    LCD_API *dis = (LCD_API *)hd;
    dis->lock(1);
    dis->clear();
    dis->setXY(0, 0);
   static u8 i;	
      	dis->show_char('E');      
   
  if(use_ctl.bEq2Val<AD_EQ_MAX/2)
  	i=AD_EQ_MAX/2-use_ctl.bEq2Val;
  else
  	i=use_ctl.bEq2Val-AD_EQ_MAX/2;
  	
  	if(use_ctl.bEq2Val < AD_EQ_MAX/2){
  		dis->show_char('-');
  	}else{
      	dis->show_char(' ');
  	}  	
	dis->show_number( i / 10);	
	dis->show_number(i % 10);	
    dis->lock(0);
}
static void led7_show_eq3(void *hd)
{
    LCD_API *dis = (LCD_API *)hd;
    dis->lock(1);
    dis->clear();
    dis->setXY(0, 0);
   static u8 i;	
      	dis->show_char('E');      
   
  if(use_ctl.bEq3Val<AD_EQ_MAX/2)
  	i=AD_EQ_MAX/2-use_ctl.bEq3Val;
  else
  	i=use_ctl.bEq3Val-AD_EQ_MAX/2;
  	
  	if(use_ctl.bEq3Val < AD_EQ_MAX/2){
  		dis->show_char('-');
  	}else{
      	dis->show_char(' ');
  	}  	
	dis->show_number( i / 10);	
	dis->show_number(i % 10);	
    dis->lock(0);
}
static void led7_show_eq4(void *hd)
{
    LCD_API *dis = (LCD_API *)hd;
    dis->lock(1);
    dis->clear();
    dis->setXY(0, 0);
   static u8 i;	
      	dis->show_char('E');      
   
  if(use_ctl.bEq4Val<AD_EQ_MAX/2)
  	i=AD_EQ_MAX/2-use_ctl.bEq4Val;
  else
  	i=use_ctl.bEq4Val-AD_EQ_MAX/2;
  	
  	if(use_ctl.bEq4Val < AD_EQ_MAX/2){
  		dis->show_char('-');
  	}else{
      	dis->show_char(' ');
  	}  	
	dis->show_number( i / 10);	
	dis->show_number(i % 10);	
    dis->lock(0);
}
static void led7_show_eq5(void *hd)
{

    LCD_API *dis = (LCD_API *)hd;
	
    dis->lock(1);
    dis->clear();
    dis->setXY(0, 0);
	
   static u8 i;	
      	dis->show_char('E');      
  if(use_ctl.bEq5Val<AD_EQ_MAX/2)
  	i=AD_EQ_MAX/2-use_ctl.bEq5Val;
  else
  	i=use_ctl.bEq5Val-AD_EQ_MAX/2;
  	
  	if(use_ctl.bEq5Val < AD_EQ_MAX/2){
  		dis->show_char('-');
  	}else{
      	dis->show_char(' ');
  	}  	
	dis->show_number( i / 10);	
	dis->show_number(i % 10);	
		
    dis->lock(0);
}
#endif


static void led7_show_vocal(void *hd)
{
    LCD_API *dis = (LCD_API *)hd;
    dis->lock(1);
    dis->clear();
    dis->setXY(0, 0);
    if(use_ctl.vocal_pp){
       dis->show_string((u8 *)" ON ");
    }else{
       dis->show_string((u8 *)" OFF");
    }
    dis->lock(0);
}
		
static void led7_show_pitch_man(void *hd)
{
    LCD_API *dis = (LCD_API *)hd;
    dis->lock(1);
    dis->clear();
    dis->setXY(0, 0);
    dis->show_string((u8 *)"bd08");
    dis->lock(0);
}
static void led7_show_pitch_woman(void *hd)
{
    LCD_API *dis = (LCD_API *)hd;
    dis->lock(1);
    dis->clear();
    dis->setXY(0, 0);
    dis->show_string((u8 *)"bd01");
    dis->lock(0);
}
static void led7_show_pitch_off(void *hd)
{
    LCD_API *dis = (LCD_API *)hd;
    dis->lock(1);
    dis->clear();
    dis->setXY(0, 0);
    dis->show_string((u8 *)"bd0F");
    dis->lock(0);
}

static void led7_show_power_off(void *hd)
{
    LCD_API *dis = (LCD_API *)hd;
    dis->lock(1);
    dis->clear();
    dis->setXY(0, 0);
    dis->show_string((u8 *)" OFF");
    dis->lock(0);
}

static void led7_show_on_off(void *hd,bool sw)
{
    LCD_API *dis = (LCD_API *)hd;
    dis->lock(1);
    dis->clear();
    dis->setXY(0, 0);
	if(sw){
		dis->show_string((u8 *)" ON ");
	}else{
		dis->show_string((u8 *)" OFF");
	}

    dis->lock(0);
}

static void led7_show_eq(void *hd, u8 EqMode)
{
    LCD_API *dis = (LCD_API *)hd;
    dis->lock(1);
    dis->clear();
    dis->setXY(0, 0);
    dis->show_char(' ');
    dis->show_char('E');
    dis->show_char('q');
    #if defined SUPPORT_HW_EQ_MODE
    dis->show_number(use_ctl.eq_mode);
    #else
    dis->show_number(EqMode);
    #endif
    /*if(EqMode == 0) dis->show_string((u8 *)" NOr");
    else if(EqMode == 1) dis->show_string((u8 *)" rOC");
    else if(EqMode == 2) dis->show_string((u8 *)" POP");
    else if(EqMode == 3) dis->show_string((u8 *)" CLA");
    else if(EqMode== 4) dis->show_string((u8 *)" jAz");
    else if(EqMode == 5) dis->show_string((u8 *)" COU");
    else if(EqMode == 6) dis->show_string((u8 *)" SOF");*/
    dis->lock(0);
}

#ifdef SUPPORT_SOUNDSCAPES
static void led7_show_soundscapes(void *hd)
{
    LCD_API *dis = (LCD_API *)hd;
    dis->lock(1);
    dis->clear();
    dis->setXY(0, 0);
    dis->show_string((u8 *)"SC ");
    dis->show_number(use_ctl.bSoundScapesVal%10);	
    dis->lock(0);
}
#endif		

#endif

void ui_common(void *hd, void *private, u8 menu, u32 arg)//公共显示
{

#ifdef SUPPORT_POWER_ON_AD_SHOW_DLY
#ifndef SUPPORT_POWER_ON_AD_SHOW_DLY_FIX
	if(use_ctl.bPowerOnShowDlyCnt){
		return;
	}
#endif
#endif

    u16 fre = 0;

    if (!hd) {
        return;
    }

    switch (menu) {
    case MENU_POWER_UP:
        led7_show_hi(hd);
        break;
	
    case MENU_MUSIC_DEV_CHAR:
        led7_show_char_music_dev(hd, arg & 0xff);
        break;
	
    case MENU_MAIN_VOL:
        led7_show_volume(hd, arg & 0xff);
        break;
    case MENU_WAIT:
        led7_show_wait(hd);
        break;
    case MENU_BT:
        led7_show_bt(hd);
        break;
#ifdef EQ_MENU
		
	case MENU_EQ50HZ:
		led7_show_eq50hz(hd);
		break;
	case MENU_EQ6KHZ:
		led7_show_eq6khz(hd);
		break;
#endif		
	case MENU_ONOFF:
		led7_show_on_off(hd, arg);
		break;
    case MENU_IR_FM_SET_FRE:
#if TCFG_APP_FM_EMITTER_EN
        led7_fm_ir_set_freq(hd, arg);
#endif
        break;
    case MENU_FM_SET_FRE:

#if TCFG_APP_FM_EMITTER_EN
        fre = fm_emitter_manage_get_fre();
        led7_fm_set_freq(hd, arg);
#endif
        break;
#ifdef SUPPORT_KPH_DEF

#ifdef SUPPORT_RGB_MODE_KEY
    case MENU_RGB_SHOW:
        led7_show_rtgb_mode(hd);
        break;
#endif

	case MENU_RGBmode_SHOW:
		led7_show_rgb_mode(hd, arg);
		break;

#ifdef SUPPORT_VOICE_EN_OFF
    case MENU_VOICE_ONOFF:
        led7_show_voice_onoff(hd);
        break;
#endif

#ifdef SUPPORT_TONE_VOL_CONTROL
    case MENU_TONE_VOL:
        led7_show_tone_volume(hd, arg & 0xff);
        break;
#endif
		
    case MENU_MIC_VOL:
        led7_show_micvol(hd);
        break;
#ifdef JT_VOL_IO_AD_EN
   case MENU_JT_VOL:
        led7_show_jtvol(hd);
        break;
#endif
    case MENU_ECHO:
        led7_show_echo(hd);
        break;
#ifdef SUPPORT_MIC_DELAY_SET
    case MENU_MICDELAY:
        led7_show_micdelay(hd);
        break;
#endif		
#if SUPPORT_USER_EQ_MODE

    case MENU_TRE:
        led7_show_tre(hd);
        break;
    case MENU_BASS:
        led7_show_bass(hd);
        break;		
#ifdef SUPPORT_MID_SET		
    case MENU_MID:
        led7_show_mid(hd);
        break;
#endif
#endif

    case MENU_VOICE:
        led7_show_voice(hd);
    break;
#ifdef SUPPORT_MIC_BASS_TRE_SET	
    case MENU_MICTRE:
        led7_show_mic_tre(hd);
        break;

    case MENU_MICBASS:
        led7_show_mic_bass(hd);
        break;
#endif		   		   
		
    case MENU_VOL_PR:
        led7_show_vol_pr(hd);
        break;
    case MENU_ONE_KEY_BASS:
        led7_show_bass_mode(hd);
        break;
    case MENU_USB:
        led7_show_usb(hd);
        break;
    case MENU_SD:
        led7_show_sd(hd);
        break;
    case MENU_DEL:
        led7_show_del(hd);
        break;
    case MENU_MIC_MODE:
        led7_show_mic(hd);
        break;
    case MENU_VOCAL:
        led7_show_vocal(hd);
        break;
    case MENU_POWER_OFF:
        led7_show_power_off(hd);
        break;
    case MENU_SET_EQ:
        led7_show_eq(hd, arg & 0xff);
        break;
    case MENU_TWS:
        led7_show_tws(hd);
        break;

    case MENU_TWS_OFF:

        break;

    case MENU_PITCH_MAN:
        led7_show_pitch_man(hd);
        break;
#ifdef SUPPORT_SOUNDSCAPES
    case MENU_SOUNDSCAPES:
        led7_show_soundscapes(hd);
        break;
#endif		
		
    case MENU_PITCH_WOMAN:
        led7_show_pitch_woman(hd);
        break;
    case MENU_PITCH_OFF:
        led7_show_pitch_off(hd);
        break;
		
#if defined SUPPORT_CD4051_PONTEN ||defined SUPPORT_PONTEN_EQ1_EQ5
    case MENU_EQ1:
        led7_show_eq1(hd);
        break;
    case MENU_EQ2:
        led7_show_eq2(hd);
        break;
    case MENU_EQ3:
        led7_show_eq3(hd);
        break;
    case MENU_EQ4:
        led7_show_eq4(hd);
        break;
    case MENU_EQ5:
        led7_show_eq5(hd);
        break;
#endif


#endif

    default:
        break;
    }
}

#endif
