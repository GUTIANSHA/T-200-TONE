#include "app_config.h"
#include "audio_config.h"
#include "tone_player.h"

#ifdef SUPPORT_MULTI_VOICE_MODE
#include "user_cfg.h"
#endif

#ifndef SUPPORT_MULTI_VOICE_MODE
const char *tone_table[] = {
    [IDEX_TONE_NUM_0] 			= TONE_RES_ROOT_PATH"tone/0.*",
    [IDEX_TONE_NUM_1] 			= TONE_RES_ROOT_PATH"tone/1.*",
    [IDEX_TONE_NUM_2] 			= TONE_RES_ROOT_PATH"tone/2.*",
    [IDEX_TONE_NUM_3] 			= TONE_RES_ROOT_PATH"tone/3.*",
    [IDEX_TONE_NUM_4] 			= TONE_RES_ROOT_PATH"tone/4.*",
    [IDEX_TONE_NUM_5] 			= TONE_RES_ROOT_PATH"tone/5.*",
    [IDEX_TONE_NUM_6] 			= TONE_RES_ROOT_PATH"tone/6.*",
    [IDEX_TONE_NUM_7] 			= TONE_RES_ROOT_PATH"tone/7.*",
    [IDEX_TONE_NUM_8] 			= TONE_RES_ROOT_PATH"tone/8.*",
    [IDEX_TONE_NUM_9] 			= TONE_RES_ROOT_PATH"tone/9.*",
    [IDEX_TONE_BT_MODE] 		= TONE_RES_ROOT_PATH"tone/bt.*",
    [IDEX_TONE_BT_CONN] 		= TONE_RES_ROOT_PATH"tone/bt_conn.*",
    [IDEX_TONE_BT_DISCONN] 		= TONE_RES_ROOT_PATH"tone/bt_dconn.*",
    [IDEX_TONE_TWS_CONN] 		= TONE_RES_ROOT_PATH"tone/tws_conn.*",
    [IDEX_TONE_TWS_DISCONN]		= TONE_RES_ROOT_PATH"tone/tws_dconn.*",
    [IDEX_TONE_LOW_POWER] 		= TONE_RES_ROOT_PATH"tone/low_power.*",
    [IDEX_TONE_POWER_OFF] 		= TONE_RES_ROOT_PATH"tone/power_off.*",
    [IDEX_TONE_POWER_ON] 		= TONE_RES_ROOT_PATH"tone/power_on.*",
    [IDEX_TONE_RING] 			= TONE_RES_ROOT_PATH"tone/ring.*",
    [IDEX_TONE_MAX_VOL] 		= TONE_NORMAL,
    [IDEX_TONE_NORMAL] 			= TONE_NORMAL,
    [IDEX_TONE_MUSIC] 			= TONE_RES_ROOT_PATH"tone/music.*",
    [IDEX_TONE_LINEIN] 			= TONE_RES_ROOT_PATH"tone/linein.*",
    [IDEX_TONE_FM] 				= TONE_RES_ROOT_PATH"tone/fm.*",
    [IDEX_TONE_PC] 				= TONE_RES_ROOT_PATH"tone/pc.*",
    [IDEX_TONE_RTC] 			= TONE_RES_ROOT_PATH"tone/rtc.*",
    [IDEX_TONE_RECORD] 			= TONE_RES_ROOT_PATH"tone/record.*",
    [IDEX_TONE_UDISK] 			= TONE_RES_ROOT_PATH"tone/usb.*",
    [IDEX_TONE_SD] 				= TONE_RES_ROOT_PATH"tone/sd.*",
    [IDEX_TONE_SPEECH_START]	= TONE_NORMAL,//TONE_RES_ROOT_PATH"tone/speech.*",

    [IDEX_TONE_NULL] 			= TONE_RES_ROOT_PATH"tone/null.*",
    [IDEX_TONE_BT_MODE_N] 			= TONE_RES_ROOT_PATH"tone/bt_n.*",
    [IDEX_TONE_BT_CONN_N] 				= TONE_RES_ROOT_PATH"tone/connect_n.*",
    [IDEX_TONE_BT_DISCONN_N]	= TONE_RES_ROOT_PATH"tone/discon_n.*",
    
#ifdef SUPPORT_WH_VOICE
    [IDEX_TONE_GANXIE]	= TONE_RES_ROOT_PATH"tone/ganxie.*",
    [IDEX_TONE_HECAI]	= TONE_RES_ROOT_PATH"tone/hecai.*",
    [IDEX_TONE_KOUSHAO]	= TONE_RES_ROOT_PATH"tone/koushao.*",
    [IDEX_TONE_ZHANGSHENG]	= TONE_RES_ROOT_PATH"tone/zhangsheng.*",
    [IDEX_TONE_KAICHANG]	= TONE_RES_ROOT_PATH"tone/kaichang.*",
    [IDEX_TONE_JIQIANG]	= TONE_RES_ROOT_PATH"tone/jiqiang.*",
    [IDEX_TONE_BISHI]	= TONE_RES_ROOT_PATH"tone/bishi.*",
    [IDEX_TONE_MOMODA]	= TONE_RES_ROOT_PATH"tone/momoda.*",
    [IDEX_TONE_DAERGUANG]	= TONE_RES_ROOT_PATH"tone/daerguang.*",
#endif   	

#ifdef SUPPORT_TWS_KEY_VOICE
    [IDEX_TONE_TWS_PAIR]	= TONE_RES_ROOT_PATH"tone/tws.*",
#endif			

#ifdef SUPPORT_TONE_VOICE_ONOFF
    [IDEX_TONE_VOICE_ON]	= TONE_RES_ROOT_PATH"tone/v_on.*",
    [IDEX_TONE_VOICE_OFF]	= TONE_RES_ROOT_PATH"tone/v_off.*",
#endif   		
#ifdef SUPPORT_TONE_MIC_ONOFF
    [IDEX_TONE_MIC_ON]	= TONE_RES_ROOT_PATH"tone/mic_on.*",
    [IDEX_TONE_MIC_OFF]	= TONE_RES_ROOT_PATH"tone/mic_off.*",
#endif   		
#ifdef SUPPORT_TONE_MUTE_ONOFF
    [IDEX_TONE_MUTE_ON]	= TONE_RES_ROOT_PATH"tone/mute_on.*",
    [IDEX_TONE_MUTE_OFF]	= TONE_RES_ROOT_PATH"tone/mute_off.*",
#endif   		
#ifdef SUPPORT_TONE_FM_OP
    [IDEX_TONE_FM_SCAN]	= TONE_RES_ROOT_PATH"tone/fm_scan.*",
    [IDEX_TONE_PREV_CH]	= TONE_RES_ROOT_PATH"tone/prev_ch.*",
    [IDEX_TONE_NEXT_CH]	= TONE_RES_ROOT_PATH"tone/next_ch.*",
#endif   		
#ifdef SUPPORT_TONE_MUSIC_OP
    [IDEX_TONE_MUSIC_PLAY]	= TONE_RES_ROOT_PATH"tone/play.*",
    [IDEX_TONE_MUSIC_PAUSE]	= TONE_RES_ROOT_PATH"tone/pause.*",
    [IDEX_TONE_PREV_FILE]	= TONE_RES_ROOT_PATH"tone/prev.*",
    [IDEX_TONE_NEXT_FILE]	= TONE_RES_ROOT_PATH"tone/next.*",
#endif   		
#ifdef SUPPORT_TONE_EQ
    [IDEX_TONE_EQ_NORMAL]	= TONE_RES_ROOT_PATH"tone/flat.*",
    [IDEX_TONE_EQ_ROCK]	= TONE_RES_ROOT_PATH"tone/rock.*",
    [IDEX_TONE_EQ_JAZZ]	= TONE_RES_ROOT_PATH"tone/jazz.*",
    [IDEX_TONE_EQ_POP]	= TONE_RES_ROOT_PATH"tone/pop.*",
    [IDEX_TONE_EQ_CLASSIC]	= TONE_RES_ROOT_PATH"tone/classic.*",
#endif   		
#ifdef SUPPORT_TONE_RPT
    [IDEX_TONE_RPT_ONE]	= TONE_RES_ROOT_PATH"tone/rpt_one.*",
    [IDEX_TONE_RPT_ALL]	= TONE_RES_ROOT_PATH"tone/rpt_all.*",
#endif   		

#ifdef SUPPORT_TONE_LOWPOWER_N
    [IDEX_TONE_LOW_POWER_N]	= TONE_RES_ROOT_PATH"tone/lowpower_n.*",	
#endif   		

#ifdef SUPPORT_TONE_RECPLAY_DEL
    [IDEX_TONE_REC_PLAY]	= TONE_RES_ROOT_PATH"tone/recplay.*",	
    [IDEX_TONE_REC_DEL]	= TONE_RES_ROOT_PATH"tone/recdel.*",	
#endif   	
#ifdef SUPPORT_TONE_REC_SAVE
    [IDEX_TONE_REC_SAVE]	= TONE_RES_ROOT_PATH"tone/recsave.*",	
#endif  

#ifdef SUPPORT_TONE_BT_TWS_PAIR
    [IDEX_TONE_TWS_PAIR]	= TONE_RES_ROOT_PATH"tone/tws_pair.*",		
     [IDEX_TONE_TWS_PAIR_N]	= TONE_RES_ROOT_PATH"tone/twspair_n.*",		
   [IDEX_TONE_TWS_CON_N]	= TONE_RES_ROOT_PATH"tone/twscon_n.*",	
    [IDEX_TONE_TWS_DISCON_N]	= TONE_RES_ROOT_PATH"tone/twsdis_n.*",	
    [IDEX_TONE_TWS_CON_SLAVE_N]	= TONE_RES_ROOT_PATH"tone/twscons_n.*",    
    [IDEX_TONE_TWS_CON_SLAVE]	= TONE_RES_ROOT_PATH"tone/twscons.*",    
#endif   	

};


#else

const char *tone_table_ch[] = {

    [IDEX_TONE_NUM_0] 			= TONE_RES_ROOT_PATH"tone/0.*",
    [IDEX_TONE_NUM_1] 			= TONE_RES_ROOT_PATH"tone/1.*",
    [IDEX_TONE_NUM_2] 			= TONE_RES_ROOT_PATH"tone/2.*",
    [IDEX_TONE_NUM_3] 			= TONE_RES_ROOT_PATH"tone/3.*",
    [IDEX_TONE_NUM_4] 			= TONE_RES_ROOT_PATH"tone/4.*",
    [IDEX_TONE_NUM_5] 			= TONE_RES_ROOT_PATH"tone/5.*",
    [IDEX_TONE_NUM_6] 			= TONE_RES_ROOT_PATH"tone/6.*",
    [IDEX_TONE_NUM_7] 			= TONE_RES_ROOT_PATH"tone/7.*",
    [IDEX_TONE_NUM_8] 			= TONE_RES_ROOT_PATH"tone/8.*",
    [IDEX_TONE_NUM_9] 			= TONE_RES_ROOT_PATH"tone/9.*",
    [IDEX_TONE_BT_MODE] 		= TONE_RES_ROOT_PATH"tone/bt_ch.*",
    [IDEX_TONE_BT_CONN] 		= TONE_RES_ROOT_PATH"tone/con_ch.*",
    [IDEX_TONE_BT_DISCONN] 		= TONE_RES_ROOT_PATH"tone/disc_ch.*",
    [IDEX_TONE_TWS_CONN] 		= TONE_RES_ROOT_PATH"tone/tws_conn.*",
    [IDEX_TONE_TWS_DISCONN]		= TONE_RES_ROOT_PATH"tone/tws_dconn.*",
    [IDEX_TONE_LOW_POWER] 		= TONE_RES_ROOT_PATH"tone/low_power.*",
    [IDEX_TONE_POWER_OFF] 		= TONE_RES_ROOT_PATH"tone/power_off.*",
    [IDEX_TONE_POWER_ON] 		= TONE_RES_ROOT_PATH"tone/power_on.*",
    [IDEX_TONE_RING] 			= TONE_RES_ROOT_PATH"tone/ring.*",
    [IDEX_TONE_MAX_VOL] 		= TONE_RES_ROOT_PATH"tone/volmax.*",
    [IDEX_TONE_NORMAL] 			= TONE_NORMAL,
    [IDEX_TONE_MUSIC] 			= TONE_RES_ROOT_PATH"tone/music_ch.*",
    [IDEX_TONE_LINEIN] 			= TONE_RES_ROOT_PATH"tone/linein_ch.*",
    [IDEX_TONE_FM] 				= TONE_RES_ROOT_PATH"tone/fm.*",
    [IDEX_TONE_PC] 				= TONE_RES_ROOT_PATH"tone/pc.*",
    [IDEX_TONE_RTC] 			= TONE_RES_ROOT_PATH"tone/rtc.*",
    [IDEX_TONE_RECORD] 			= TONE_RES_ROOT_PATH"tone/record.*",
    [IDEX_TONE_UDISK] 			= TONE_RES_ROOT_PATH"tone/usb.*",
    [IDEX_TONE_SD] 				= TONE_RES_ROOT_PATH"tone/sd.*",
    [IDEX_TONE_SPEECH_START]	= TONE_NORMAL,//TONE_RES_ROOT_PATH"tone/speech.*",
    [IDEX_TONE_COAX]            = TONE_RES_ROOT_PATH"tone/coax_ch.*",
    [IDEX_TONE_OPT]            = TONE_RES_ROOT_PATH"tone/opt_ch.*",
    [IDEX_TONE_NULL] 			= TONE_RES_ROOT_PATH"tone/null.*",
};

const char *tone_table_en[] = {

    [IDEX_TONE_NUM_0] 			= TONE_RES_ROOT_PATH"tone/0.*",
    [IDEX_TONE_NUM_1] 			= TONE_RES_ROOT_PATH"tone/1.*",
    [IDEX_TONE_NUM_2] 			= TONE_RES_ROOT_PATH"tone/2.*",
    [IDEX_TONE_NUM_3] 			= TONE_RES_ROOT_PATH"tone/3.*",
    [IDEX_TONE_NUM_4] 			= TONE_RES_ROOT_PATH"tone/4.*",
    [IDEX_TONE_NUM_5] 			= TONE_RES_ROOT_PATH"tone/5.*",
    [IDEX_TONE_NUM_6] 			= TONE_RES_ROOT_PATH"tone/6.*",
    [IDEX_TONE_NUM_7] 			= TONE_RES_ROOT_PATH"tone/7.*",
    [IDEX_TONE_NUM_8] 			= TONE_RES_ROOT_PATH"tone/8.*",
    [IDEX_TONE_NUM_9] 			= TONE_RES_ROOT_PATH"tone/9.*",
    [IDEX_TONE_BT_MODE] 		= TONE_RES_ROOT_PATH"tone/bt_en.*",
    [IDEX_TONE_BT_CONN] 		= TONE_RES_ROOT_PATH"tone/bt_conn_en.*",
    [IDEX_TONE_BT_DISCONN] 		= TONE_RES_ROOT_PATH"tone/bt_dconn_en.*",
    [IDEX_TONE_TWS_CONN] 		= TONE_RES_ROOT_PATH"tone/tws_conn.*",
    [IDEX_TONE_TWS_DISCONN]		= TONE_RES_ROOT_PATH"tone/tws_dconn.*",
    [IDEX_TONE_LOW_POWER] 		= TONE_RES_ROOT_PATH"tone/low_power_en.*",
    [IDEX_TONE_POWER_OFF] 		= TONE_RES_ROOT_PATH"tone/power_off.*",
    [IDEX_TONE_POWER_ON] 		= TONE_RES_ROOT_PATH"tone/power_on.*",
    [IDEX_TONE_RING] 			= TONE_RES_ROOT_PATH"tone/ring.*",
    [IDEX_TONE_MAX_VOL] 		= TONE_RES_ROOT_PATH"tone/volmax.*",
    [IDEX_TONE_NORMAL] 			= TONE_NORMAL,
    [IDEX_TONE_MUSIC] 			= TONE_RES_ROOT_PATH"tone/music_en.*",
    [IDEX_TONE_LINEIN] 			= TONE_RES_ROOT_PATH"tone/linein_en.*",
    [IDEX_TONE_FM] 				= TONE_RES_ROOT_PATH"tone/fm.*",
    [IDEX_TONE_PC] 				= TONE_RES_ROOT_PATH"tone/pc.*",
    [IDEX_TONE_RTC] 			= TONE_RES_ROOT_PATH"tone/rtc.*",
    [IDEX_TONE_RECORD] 			= TONE_RES_ROOT_PATH"tone/record.*",
    [IDEX_TONE_UDISK] 			= TONE_RES_ROOT_PATH"tone/usb.*",
    [IDEX_TONE_SD] 				= TONE_RES_ROOT_PATH"tone/sd.*",
    [IDEX_TONE_COAX]            = TONE_RES_ROOT_PATH"tone/coax_en.*",
    [IDEX_TONE_OPT]            = TONE_RES_ROOT_PATH"tone/opt_en.*",
    [IDEX_TONE_SPEECH_START]	= TONE_NORMAL,//TONE_RES_ROOT_PATH"tone/speech.*",
    [IDEX_TONE_NULL] 			= TONE_RES_ROOT_PATH"tone/null.*",
};

const char *tone_table_null[IDEX_TONE_NUMBER_MAX] = {
    [IDEX_TONE_NULL] 			= TONE_RES_ROOT_PATH"tone/null.*"
    
    
    };

 char *(*tone_table) = tone_table_null;//防止内存溢出

void tone_table_switch(unsigned char language)
{
    switch (language)
    {
#ifdef SUPPORT_NULL_VOICE
    case VOICE_MODE_NULL:
        tone_table = tone_table_null;
        break;
#endif
#ifdef SUPPORT_EN_VOICE
    case VOICE_MODE_EN:
        tone_table = tone_table_en;
        break;
#endif
#ifdef SUPPORT_CH_VOICE
    case VOICE_MODE_CH:
        tone_table = tone_table_ch;
        break;
#endif
    default:
        break;
    }
}


#endif
