
/***********************************************************
 *				bt 模式的 adkey table
 ***********************************************************/

#if TCFG_APP_BT_EN
// 0    3        6     9     15   22   47  100    200
//0     1        2     3     4    5     6    7        8     9
// pp  prev   next  m    rec  bas  0   m pr
// t+   b+    t-     m+  e+  b-    e-  m-



//0pp   1prev  2next  3mode   4rec   5o bass   6null 7mpr
//0t+   1b-     2t-     3mv+   4e+   5b+  6e- 7 mv-     +10

const u16 bt_key_ad_table[KEY_AD_NUM_MAX][KEY_EVENT_MAX] = {
    //单击             //长按          //hold         //抬起            //双击                //三击
    [0] = {
        KEY_MUSIC_PP,	KEY_TWS_SEARCH_REMOVE_PAIR,	KEY_NULL,			KEY_NULL,		KEY_NULL,	KEY_NULL
    },
    [1] = {
        KEY_MUSIC_PREV,	KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [2] = {
        KEY_MUSIC_NEXT,   KEY_NULL,		KEY_NULL,	KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [3] = {
        KEY_CHANGE_MODE,	KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [4] = {
        KEY_REC_MODE,		KEY_COME_TO_PLAY_REC_FILE,			KEY_NULL,			KEY_NULL,		KEY_NULL,	KEY_NULL
    },
    [5] = {
        KEY_NULL,		   KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [6] = {
        KEY_NULL,		   KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [7] = {
        KEY_MIC_ONOFF,		  KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [8] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [9] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [10] = {
        KEY_TRE_UP,		KEY_TRE_UP,			KEY_TRE_UP,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [11] = {
        KEY_BASS_UP,	KEY_BASS_UP,		KEY_BASS_UP,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [12] = {
        KEY_TRE_DN,		KEY_TRE_DN,			KEY_TRE_DN,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [13] = {
        KEY_MIC_VOL_UP,		KEY_MIC_VOL_UP,			KEY_MIC_VOL_UP,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [14] = {
        KEY_MIC_ECHO_UP,	KEY_MIC_ECHO_UP,			KEY_MIC_ECHO_UP,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [15] = {
        KEY_BASS_DN,		KEY_BASS_DN,			KEY_BASS_DN,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [16] = {
        KEY_MIC_ECHO_DN,		KEY_MIC_ECHO_DN,			KEY_MIC_ECHO_DN,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [17] = {
        KEY_MIC_VOL_DN,		KEY_MIC_VOL_DN,			KEY_MIC_VOL_DN,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [18] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [19] = {
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
        KEY_FM_SCAN_ALL,	KEY_TWS_SEARCH_REMOVE_PAIR,	KEY_NULL,			KEY_NULL,		KEY_NULL,	KEY_NULL
    },
    [1] = {
        KEY_FM_PREV_STATION,	KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [2] = {
        KEY_FM_NEXT_STATION,   KEY_NULL,		KEY_NULL,	KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [3] = {
        KEY_CHANGE_MODE,	KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [4] = {
        KEY_REC_MODE,		KEY_COME_TO_PLAY_REC_FILE,			KEY_NULL,			KEY_NULL,		KEY_NULL,	KEY_NULL
    },
    [5] = {
        KEY_NULL,		   KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [6] = {
        KEY_NULL,		   KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [7] = {
        KEY_MIC_ONOFF,		  KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [8] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [9] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [10] = {
        KEY_TRE_UP,		KEY_TRE_UP,			KEY_TRE_UP,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [11] = {
        KEY_BASS_UP,	KEY_BASS_UP,		KEY_BASS_UP,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [12] = {
        KEY_TRE_DN,		KEY_TRE_DN,			KEY_TRE_DN,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [13] = {
        KEY_MIC_VOL_UP,		KEY_MIC_VOL_UP,			KEY_MIC_VOL_UP,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [14] = {
        KEY_MIC_ECHO_UP,	KEY_MIC_ECHO_UP,			KEY_MIC_ECHO_UP,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [15] = {
        KEY_BASS_DN,		KEY_BASS_DN,			KEY_BASS_DN,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [16] = {
        KEY_MIC_ECHO_DN,		KEY_MIC_ECHO_DN,			KEY_MIC_ECHO_DN,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [17] = {
        KEY_MIC_VOL_DN,		KEY_MIC_VOL_DN,			KEY_MIC_VOL_DN,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [18] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [19] = {
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
        KEY_NULL,	KEY_NULL,	KEY_NULL,			KEY_NULL,		KEY_NULL,	KEY_NULL
    },
    [1] = {
        KEY_NULL,	KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [2] = {
        KEY_NULL,   KEY_NULL,		KEY_NULL,	KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [3] = {
        KEY_CHANGE_MODE,	KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [4] = {
        KEY_REC_MODE,		KEY_COME_TO_PLAY_REC_FILE,			KEY_NULL,			KEY_NULL,		KEY_NULL,	KEY_NULL
    },
    [5] = {
        KEY_NULL,		   KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [6] = {
        KEY_NULL,		   KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [7] = {
        KEY_MIC_ONOFF,		  KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [8] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [9] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [10] = {
        KEY_TRE_UP,		KEY_TRE_UP,			KEY_TRE_UP,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [11] = {
        KEY_BASS_UP,	KEY_BASS_UP,		KEY_BASS_UP,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [12] = {
        KEY_TRE_DN,		KEY_TRE_DN,			KEY_TRE_DN,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [13] = {
        KEY_MIC_VOL_UP,		KEY_MIC_VOL_UP,			KEY_MIC_VOL_UP,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [14] = {
        KEY_MIC_ECHO_UP,	KEY_MIC_ECHO_UP,			KEY_MIC_ECHO_UP,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [15] = {
        KEY_BASS_DN,		KEY_BASS_DN,			KEY_BASS_DN,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [16] = {
        KEY_MIC_ECHO_DN,		KEY_MIC_ECHO_DN,			KEY_MIC_ECHO_DN,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [17] = {
        KEY_MIC_VOL_DN,		KEY_MIC_VOL_DN,			KEY_MIC_VOL_DN,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [18] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [19] = {
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
        KEY_MUSIC_PP,	KEY_NULL,	KEY_NULL,			KEY_NULL,		KEY_NULL,	KEY_NULL
    },
    [1] = {
        KEY_MUSIC_PREV,	KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [2] = {
        KEY_MUSIC_NEXT,   KEY_NULL,		KEY_NULL,	KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [3] = {
        KEY_CHANGE_MODE,	KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [4] = {
        KEY_REC_MODE,		KEY_MUSIC_PLAYE_REC_FILE,			KEY_NULL,			KEY_NULL,		KEY_NULL,	KEY_NULL
    },
    [5] = {
        KEY_MUSIC_CHANGE_REPEAT,		   KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [6] = {
        KEY_NULL,		   KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [7] = {
        KEY_MIC_ONOFF,		  KEY_MUSIC_DELETE_FILE,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [8] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [9] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [10] = {
        KEY_TRE_UP,		KEY_TRE_UP,			KEY_TRE_UP,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [11] = {
        KEY_BASS_UP,	KEY_BASS_UP,		KEY_BASS_UP,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [12] = {
        KEY_TRE_DN,		KEY_TRE_DN,			KEY_TRE_DN,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [13] = {
        KEY_MIC_VOL_UP,		KEY_MIC_VOL_UP,			KEY_MIC_VOL_UP,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [14] = {
        KEY_MIC_ECHO_UP,	KEY_MIC_ECHO_UP,			KEY_MIC_ECHO_UP,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [15] = {
        KEY_BASS_DN,		KEY_BASS_DN,			KEY_BASS_DN,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [16] = {
        KEY_MIC_ECHO_DN,		KEY_MIC_ECHO_DN,			KEY_MIC_ECHO_DN,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [17] = {
        KEY_MIC_VOL_DN,		KEY_MIC_VOL_DN,			KEY_MIC_VOL_DN,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [18] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [19] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
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
        KEY_CHANGE_MODE, KEY_POWEROFF,		KEY_POWEROFF_HOLD,	KEY_NULL,		KEY_NULL,			KEY_NULL
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
    [10] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [11] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [12] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [13] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [14] = {
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
        KEY_ENC_START, KEY_NULL,		KEY_NULL,	KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [1] = {
        KEY_NULL,			KEY_NULL,	KEY_NULL,	KEY_NULL,		KEY_NULL,				KEY_NULL
    },
    [2] = {
        KEY_NULL,			KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,				KEY_NULL
    },
    [3] = {
        KEY_NULL,			KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,				KEY_NULL
    },
    [4] = {
        KEY_REC_STOP,		KEY_REC_STOP,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [5] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_ENC_START,			KEY_NULL
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
    [10] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [11] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [12] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [13] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [14] = {
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
        KEY_CHANGE_MODE, KEY_POWEROFF,		KEY_POWEROFF_HOLD,	KEY_NULL,		KEY_NULL,			KEY_NULL
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
    [10] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [11] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [12] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [13] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [14] = {
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
        KEY_CHANGE_MODE, KEY_POWEROFF,		KEY_POWEROFF_HOLD,	KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [1] = {
        KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL,		KEY_NULL,				KEY_NULL
    },
    [2] = {
        KEY_MUSIC_PP,		KEY_POWEROFF,	KEY_POWEROFF_HOLD,	KEY_NULL,		KEY_NULL,				KEY_NULL
    },
    [3] = {
        KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL,		KEY_NULL,				KEY_NULL
    },
    [4] = {
        KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL,		KEY_SPDIF_SW_SOURCE,	KEY_NULL
    },
    [5] = {
        KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL,		KEY_NULL,				KEY_NULL
    },
    [6] = {
        KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL,		KEY_SPDIF_SW_SOURCE,	KEY_NULL
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
    [10] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [11] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [12] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [13] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [14] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
};
#endif

/***********************************************************
 *				idle 模式的 adkey table
 ***********************************************************/
const u16 idle_key_ad_table[KEY_AD_NUM_MAX][KEY_EVENT_MAX] = {
    //单击             //长按          //hold         //抬起            //双击                //三击
    [0] = {
        KEY_NULL,	    KEY_NULL,		KEY_NULL,	KEY_NULL,   	KEY_NULL,			KEY_NULL
    },
    [1] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,   	KEY_NULL,           KEY_NULL
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
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [8] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [9] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [10] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [11] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [12] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [13] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [14] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
};



