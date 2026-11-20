#include "key_event_deal.h"
#include "key_driver.h"
#include "app_config.h"
#include "board_config.h"
#include "app_task.h"

#ifdef CONFIG_BOARD_AC695X_DEMO

// SUPPORT_KPH_ADKEY_FUNCTION
#if defined ADKEY_22K_10KEY_MODE_KPH
#include "ADKEY_22K_10KEY_MODE_KPH.c"
#elif defined ADKEY_10K_10KEY_MODE_FY_A12
#include "ADKEY_10K_10KEY_MODE_FY_A12.c"
#elif defined ADKEY_22K_24KEY_MODE_YH_FH5123
#include "ADKEY_22K_24KEY_MODE_YH_FH5123.c"
#elif defined ADKEY_10K_10KEY_MODE_RT_A116
#include "ADKEY_10K_10KEY_MODE_RT_A116.c"
#elif defined ADKEY_22K_10KEY_MODE_HL
#include "ADKEY_22K_10KEY_MODE_HL.c"
#elif defined ADKEY_47K_10KEY_MODE_JY
#include "ADKEY_47K_10KEY_MODE_JY.c"
#elif defined ADKEY_47K_10KEY_MODE_JY_GB_15
#include "ADKEY_47K_10KEY_MODE_JY_GB_15.c"
#elif defined ADKEY_22K_10KEY_MODE_JY_TX_2210
#include "ADKEY_22K_10KEY_MODE_JY_TX_2210.c"

#elif defined ADKEY_22K_10KEY_MODE_JY_TX_2310
#include "ADKEY_22K_10KEY_MODE_JY_TX_2310.c"

#elif defined ADKEY_22K_10KEY_MODE_JY_TX_2728
#include "ADKEY_22K_10KEY_MODE_JY_TX_2728.c"
#elif defined ADKEY_MODE_JD_X3116_1
#include "ADKEY_MODE_JD_X3116_1.c"
#elif defined ADKEY_22K_YS_818
#include "ADKEY_22K_YS_818.c"
#elif defined ADKEY_22K_1258
#include "ADKEY_22K_1258.c"
#elif defined ADKEY_22K_DS4751C_FL872_6ADKEY
#include "ADKEY_22K_DS4751C_FL872_6ADKEY.c"
#elif defined ADKEY_22K_DS4751C_FL872
#include "ADKEY_22K_DS4751C_FL872.c"
#elif defined ADKEY_22K_DS4751C_FL872_V1
#include "ADKEY_22K_DS4751C_FL872_V1.c"
#elif defined ADKEY_22K_11KEY_MODE_JY_TX_2810
#include "ADKEY_22K_11KEY_MODE_JY_TX_2810.c"

#elif defined ADKEY_22K_LH_808
#include "ADKEY_22K_LH_808.c"
#elif defined ADKEY_22K_DG_1263
#include "ADKEY_22K_DG_1263.c"
#elif defined ADKEY_22K_sub2100
#include "ADKEY_22K_sub2100.c"
#elif defined ADKEY_47K_10KEY_MODE_JY_TB_12
#include "ADKEY_47K_10KEY_MODE_JY_TB_12.c"
#elif defined ADKEY_22K_10KEY_MODE_JY_TS12
#include "ADKEY_22K_10KEY_MODE_JY_TS12.c"
#elif defined ADKEY_22K_10KEY_MODE_JY_TS15
#include "ADKEY_22K_10KEY_MODE_JY_TS15.c"
#elif defined ADKEY_22K_10KEY_MODE_JY_GW_12
#include "ADKEY_22K_10KEY_MODE_JY_GW_12.c"
#elif defined ADKEY_22K_10KEY_MODE_JY_TS_8_7VR
#include "ADKEY_22K_10KEY_MODE_JY_TS_8_7VR.c"
#elif defined ADKEY_47K_10KEY_MODE_JY_GB_10
#include "ADKEY_47K_10KEY_MODE_JY_GB_10.c"


#else
/***********************************************************
 *				bt 模式的 adkey table
 ***********************************************************/
#if TCFG_APP_BT_EN
const u16 bt_key_ad_table[KEY_AD_NUM_MAX][KEY_EVENT_MAX] = {
    //单击             //长按          //hold         //抬起            //双击                //三击
    [0] = {
        KEY_BT_MODE,	KEY_NULL,	KEY_VOICE_CHANGE,			KEY_VOICE_CHANGE_END,		KEY_NULL,	KEY_NULL
    },
    [1] = {
        KEY_CHANGE_MODE, 	KEY_STANBY,		KEY_NULL,		KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [2] = {
        KEY_NULL,     KEY_NULL,		KEY_NULL,	KEY_NULL,		KEY_CALL_LAST_NO,			KEY_NULL
    },
    [3] = {
        KEY_NULL,	     KEY_TRE_DN,			KEY_TRE_DN,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [4] = {
        KEY_NULL,		    KEY_TRE_UP,			KEY_TRE_UP,			KEY_NULL,		KEY_REVERB_OPEN,	KEY_NULL
    },
    [5] = {
        KEY_NULL,		KEY_BASS_DN,			KEY_BASS_DN,			KEY_NULL,		KEY_ENC_START,		KEY_NULL
    },
    [6] = {
        KEY_NULL,		KEY_BASS_UP,			KEY_BASS_UP,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [7] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [8] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [9] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
};
#endif

/***********************************************************
 *				fm 模式的 adkey table
 ***********************************************************/
#if TCFG_APP_FM_EN
const u16 fm_key_ad_table[KEY_AD_NUM_MAX][KEY_EVENT_MAX] = {
    //单击             //长按          //hold         //抬起            //双击                //三击
    [0] = {
        KEY_MUSIC_PP,	KEY_VOICE_CHANGE,	KEY_NULL,			KEY_NULL,		KEY_NULL,	KEY_NULL
    },
    [1] = {
        KEY_MUSIC_PREV, 	KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [2] = {
        KEY_MUSIC_NEXT,     KEY_NULL,		KEY_NULL,	KEY_NULL,		KEY_CALL_LAST_NO,			KEY_NULL
    },
    [3] = {
        KEY_TRE_DN,	     KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [4] = {
        KEY_TRE_UP,		    KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_REVERB_OPEN,	KEY_NULL
    },
    [5] = {
        KEY_BASS_DN,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_ENC_START,		KEY_NULL
    },
    [6] = {
        KEY_BASS_UP,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [7] = {
        KEY_MODE_PREV,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [8] = {
        KEY_CHANGE_MODE,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [9] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
};
#endif

/***********************************************************
 *				linein 模式的 adkey table
 ***********************************************************/
#if TCFG_APP_LINEIN_EN
const u16 linein_key_ad_table[KEY_AD_NUM_MAX][KEY_EVENT_MAX] = {
    //单击             //长按          //hold         //抬起            //双击                //三击
    [0] = {
        KEY_NULL,    KEY_NULL,		KEY_VOICE_CHANGE,	KEY_VOICE_CHANGE_END,		KEY_NULL,			KEY_NULL
    },
    [1] = {
        KEY_CHANGE_MODE,	KEY_STANBY,	KEY_NULL,	KEY_NULL,		KEY_NULL,				KEY_NULL
    },
    [2] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,				KEY_NULL
    },
    [3] = {
        KEY_NULL,	     KEY_TRE_DN,			KEY_TRE_DN,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [4] = {
        KEY_NULL,		    KEY_TRE_UP,			KEY_TRE_UP,			KEY_NULL,		KEY_REVERB_OPEN,	KEY_NULL
    },
    [5] = {
        KEY_NULL,		KEY_BASS_DN,			KEY_BASS_DN,			KEY_NULL,		KEY_ENC_START,		KEY_NULL
    },
    [6] = {
        KEY_NULL,		KEY_BASS_UP,			KEY_BASS_UP,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [7] = {
        KEY_NULL,	KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [8] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [9] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
};
#endif

/***********************************************************
 *				music 模式的 adkey table
 ***********************************************************/
#if TCFG_APP_MUSIC_EN
const u16 music_key_ad_table[KEY_AD_NUM_MAX][KEY_EVENT_MAX] = {
    //单击             //长按          //hold         //抬起            //双击                //三击
    [0] = {
        KEY_NULL,	KEY_NULL,	KEY_VOICE_CHANGE,			KEY_VOICE_CHANGE_END,		KEY_NULL,	KEY_NULL
    },
    [1] = {
        KEY_CHANGE_MODE, 	KEY_STANBY,		KEY_NULL,		KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [2] = {
        KEY_NULL,     KEY_NULL,		KEY_NULL,	KEY_NULL,		KEY_CALL_LAST_NO,			KEY_NULL
    },
    [3] = {
        KEY_NULL,	     KEY_TRE_DN,			KEY_TRE_DN,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [4] = {
        KEY_NULL,		    KEY_TRE_UP,			KEY_TRE_UP,			KEY_NULL,		KEY_REVERB_OPEN,	KEY_NULL
    },
    [5] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_ENC_START,		KEY_NULL
    },
    [6] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [7] = {
        KEY_NULL,	KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [8] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [9] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
};
#endif

/***********************************************************
 *				pc 模式的 adkey table
 ***********************************************************/
#if TCFG_APP_PC_EN
const u16 pc_key_ad_table[KEY_AD_NUM_MAX][KEY_EVENT_MAX] = {
    //单击             //长按          //hold         //抬起            //双击                //三击
    [0] = {
        KEY_CHANGE_MODE, KEY_NULL,		KEY_NULL,	KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [1] = {
        KEY_MUSIC_PREV,		KEY_VOL_DOWN,	KEY_VOL_DOWN,	KEY_NULL,		KEY_NULL,				KEY_NULL
    },
    [2] = {
        KEY_MUSIC_PP,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,				KEY_NULL
    },
    [3] = {
        KEY_MUSIC_NEXT,		KEY_VOL_UP,		KEY_VOL_UP,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [4] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_REVERB_OPEN,	KEY_NULL
    },
    [5] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [6] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [7] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [8] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [9] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
};
#endif

/***********************************************************
 *				record 模式的 adkey table
 ***********************************************************/
#if TCFG_APP_RECORD_EN
const u16 record_key_ad_table[KEY_AD_NUM_MAX][KEY_EVENT_MAX] = {
    //单击             //长按          //hold         //抬起            //双击                //三击
    [0] = {
        KEY_CHANGE_MODE, KEY_NULL,		KEY_NULL,	KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [1] = {
        KEY_REC_STOP,			KEY_NULL,	KEY_NULL,	KEY_NULL,		KEY_NULL,				KEY_NULL
    },
    [2] = {
        KEY_REC_STOP,			KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,				KEY_NULL
    },
    [3] = {
        KEY_REC_STOP,			KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,				KEY_NULL
    },
    [4] = {
        KEY_REC_STOP,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [5] = {
        KEY_REC_STOP,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_ENC_START,			KEY_NULL
    },
    [6] = {
        KEY_RGB_MODE,		KEY_RGB_ONOFF,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [7] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [8] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [9] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
};
#endif

/***********************************************************
 *				rtc 模式的 adkey table
 ***********************************************************/
#if TCFG_APP_RTC_EN
const u16 rtc_key_ad_table[KEY_AD_NUM_MAX][KEY_EVENT_MAX] = {
    //单击             //长按          //hold         //抬起            //双击                //三击
    [0] = {
        KEY_CHANGE_MODE, KEY_NULL,		KEY_NULL,	KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [1] = {
        KEY_RTC_DOWN,		KEY_RTC_DOWN,	KEY_RTC_DOWN,	KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [2] = {
        KEY_RTC_SW_POS,		KEY_RTC_SW,		KEY_NULL,		KEY_NULL,		KEY_NULL,				KEY_NULL
    },
    [3] = {
        KEY_RTC_UP,			KEY_RTC_UP,		KEY_RTC_UP,		KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [4] = {
        KEY_NULL,			KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_REVERB_OPEN,				KEY_NULL
    },
    [5] = {
        KEY_NULL,			KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,				KEY_NULL
    },
    [6] = {
        KEY_NULL,			KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,				KEY_NULL
    },
    [7] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [8] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [9] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
};
#endif

/***********************************************************
 *				spdif 模式的 adkey table
 ***********************************************************/
#if TCFG_APP_SPDIF_EN
const u16 spdif_key_ad_table[KEY_AD_NUM_MAX][KEY_EVENT_MAX] = {
    //单击             //长按          //hold         //抬起            //双击                //三击
    [0] = {
        KEY_NULL,	KEY_NULL,	KEY_VOICE_CHANGE,			KEY_VOICE_CHANGE_END,		KEY_NULL,	KEY_NULL
    },
    [1] = {
        KEY_CHANGE_MODE, 	KEY_STANBY,		KEY_NULL,		KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [2] = {
        KEY_NULL,     KEY_NULL,		KEY_NULL,	KEY_NULL,		KEY_CALL_LAST_NO,			KEY_NULL
    },
    [3] = {
        KEY_TRE_DN,	     KEY_TRE_DN,			KEY_TRE_DN,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [4] = {
        KEY_TRE_UP,		    KEY_TRE_UP,			KEY_TRE_UP,			KEY_NULL,		KEY_REVERB_OPEN,	KEY_NULL
    },
    [5] = {
        KEY_BASS_DN,		KEY_BASS_DN,			KEY_BASS_DN,			KEY_NULL,		KEY_ENC_START,		KEY_NULL
    },
    [6] = {
        KEY_BASS_UP,		KEY_BASS_UP,			KEY_BASS_UP,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [7] = {
        KEY_MODE_PREV,	KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [8] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [9] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
};
#endif

/***********************************************************
 *				idle 模式的 adkey table
 ***********************************************************/
const u16 idle_key_ad_table[KEY_AD_NUM_MAX][KEY_EVENT_MAX] = {
    //单击             //长按          //hold         //抬起            //双击                //三击
    [0] = {
        KEY_NULL,	    KEY_NULL,		KEY_VOICE_CHANGE,	KEY_VOICE_CHANGE_END,   	KEY_NULL,			KEY_NULL
    },
    [1] = {
        KEY_CHANGE_MODE,	KEY_STANBY,			KEY_NULL,			KEY_NULL,   	KEY_NULL,           KEY_NULL
    },
    [2] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,   	KEY_NULL,           KEY_NULL
    },
    [3] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,   	KEY_NULL,			KEY_NULL
    },
    [4] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,   	KEY_NULL,			KEY_NULL
    },
    [5] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,	    KEY_NULL,			KEY_NULL
    },
    [6] = {
        KEY_NULL,		KEY_NULL,		    KEY_NULL,			KEY_NULL,		KEY_NULL,	        KEY_NULL
    },
    [7] = {
        KEY_MODE_PREV,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [8] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [9] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
};
#endif

#endif

