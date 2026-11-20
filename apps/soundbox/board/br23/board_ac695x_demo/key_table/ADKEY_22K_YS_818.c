//#define KEY0_RES         1  // PREV
//#define KEY1_RES         10   //PP
//#define KEY2_RES         36   //NEXT
//#define KEY3_RES         56 // EQ 
//#define KEY4_RES         120 //light
//#define KEY5_RES         180 // REPEAT
//#define KEY6_RES         270	//rec
//#define KEY7_RES         470 //MODE/TWS
//#define KEY8_RES         1000 //  MIC
//#define KEY9_RES         5000 //  

/***********************************************************
 *				bt ģʽ�� adkey table
 ***********************************************************/
/////   3  0  2  7        10  4  1  5      9  11  6  8
#if TCFG_APP_BT_EN

const u16 bt_key_ad_table[KEY_AD_NUM_MAX][KEY_EVENT_MAX] = {
    //����             //����          //hold         //̧��            //˫��                //����
    [0] = {
        KEY_MUSIC_PREV,	    KEY_NULL,		KEY_NULL,	KEY_NULL,   	KEY_NULL,			KEY_NULL
    },
    [1] = {
        KEY_MUSIC_PP,	KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [2] = {
        KEY_MUSIC_NEXT,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,	KEY_NULL
    },
    [3] = {
        KEY_MIC_ONOFF,   KEY_NULL,		KEY_NULL,	KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [4] = {
        KEY_TWS_SEARCH_REMOVE_PAIR,		   KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [5] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [6] = {
        KEY_COME_TO_PLAY_REC_FILE,	KEY_REC_MODE,		KEY_NULL,		KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [7] = {
        KEY_RGB_MODE,		   KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [8] = {
        KEY_CHANGE_MODE,		  KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [9] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },


};
#endif

/***********************************************************
 *				fm ģʽ�� adkey table
 ***********************************************************/
#if TCFG_APP_FM_EN
const u16 fm_key_ad_table[KEY_AD_NUM_MAX][KEY_EVENT_MAX] = {
    //����             //����          //hold         //̧��            //˫��                //����
			[0] = {
				KEY_FM_PREV_STATION, 	KEY_NULL,		KEY_NULL,	KEY_NULL,		KEY_NULL,			KEY_NULL
			},
			[1] = {
				KEY_FM_SCAN_STOP,	KEY_FM_SCAN_ALL, 		KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
			},
			[2] = {
				KEY_FM_NEXT_STATION, 	KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,	KEY_NULL
			},
			[3] = {
				KEY_MIC_ONOFF,	 KEY_NULL,		KEY_NULL,	KEY_NULL,		KEY_NULL,			KEY_NULL
			},
			[4] = {
				KEY_NULL,		   KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,		KEY_NULL
			},
			[5] = {
				KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
			},
			[6] = {
				KEY_COME_TO_PLAY_REC_FILE,	KEY_REC_MODE,		KEY_NULL,		KEY_NULL,		KEY_NULL,			KEY_NULL
			},
			[7] = {
				KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
			},
			[8] = {
				KEY_CHANGE_MODE,		  KEY_NULL, 		KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
			},
			[9] = {
				KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
			},

		
		};
#endif

/***********************************************************
 *				linein ģʽ�� adkey table
 ***********************************************************/
#if TCFG_APP_LINEIN_EN
const u16 linein_key_ad_table[KEY_AD_NUM_MAX][KEY_EVENT_MAX] = {
    //����             //����          //hold         //̧��            //˫��                //����
			[0] = {
				KEY_NULL, 	KEY_NULL,		KEY_NULL,	KEY_NULL,		KEY_NULL,			KEY_NULL
			},
			[1] = {
				KEY_MUSIC_PP,	KEY_NULL, 		KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
			},
			[2] = {
				KEY_NULL, 	KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,	KEY_NULL
			},
			[3] = {
				KEY_MIC_ONOFF,	 KEY_NULL,		KEY_NULL,	KEY_NULL,		KEY_NULL,			KEY_NULL
			},
			[4] = {
				KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
			},
			[5] = {
				KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
			},
			[6] = {
				KEY_COME_TO_PLAY_REC_FILE,	KEY_REC_MODE,		KEY_NULL,		KEY_NULL,		KEY_NULL,			KEY_NULL
			},
			[7] = {
				KEY_RGB_MODE,		   KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,		KEY_NULL
			},
			[8] = {
				KEY_CHANGE_MODE,		  KEY_NULL, 		KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
			},
			[9] = {
				KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
			},

		
		};
#endif

/***********************************************************
 *				music ģʽ�� adkey table
 ***********************************************************/
#if TCFG_APP_MUSIC_EN

const u16 music_key_ad_table[KEY_AD_NUM_MAX][KEY_EVENT_MAX] = {
    //����             //����          //hold         //̧��            //˫��                //����
			[0] = {
				KEY_MUSIC_PREV, 	KEY_NULL,		KEY_NULL,	KEY_NULL,		KEY_NULL,			KEY_NULL
			},
			[1] = {
				KEY_MUSIC_PP,	KEY_NULL, 		KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
			},
			[2] = {
				KEY_MUSIC_NEXT, 	KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,	KEY_NULL
			},
			[3] = {
				KEY_MIC_ONOFF,	 KEY_NULL,		KEY_NULL,	KEY_NULL,		KEY_NULL,			KEY_NULL
			},
			[4] = {
				KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
			},
			[5] = {
				KEY_MUSIC_CHANGE_REPEAT,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
			},
			[6] = {
				KEY_MUSIC_PLAYE_REC_FILE,	KEY_REC_MODE,		KEY_NULL,		KEY_NULL,		KEY_NULL,			KEY_NULL
			},
			[7] = {
				KEY_RGB_MODE,		   KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,		KEY_NULL
			},
			[8] = {
				KEY_CHANGE_MODE,		  KEY_NULL, 		KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
			},
			[9] = {
				KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
			},

		
		};
#endif

/***********************************************************
 *				pc ģʽ�� adkey table
 ***********************************************************/
#if TCFG_APP_PC_EN
const u16 pc_key_ad_table[KEY_AD_NUM_MAX][KEY_EVENT_MAX] = {
    //����             //����          //hold         //̧��            //˫��                //����
    [0] = {
        KEY_CHANGE_MODE,	KEY_NULL,	KEY_NULL,			KEY_NULL,		KEY_NULL,	KEY_NULL
    },
    [1] = {
        ,	KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [2] = {
        KEY_MUSIC_PREV,   KEY_NULL,		KEY_NULL,	KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [3] = {
        KEY_MUSIC_PP,	KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [4] = {
        KEY_MUSIC_NEXT,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,	KEY_NULL
    },
    [5] = {
        KEY_RGB_MODE,		   KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [6] = {
        KEY_VOCAL_PP,		   KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [7] = {
        KEY_NULL,		  KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [8] = {
        KEY_MIC_ONOFF,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [9] = {
        KEY_MUSIC_CHANGE_REPEAT,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },

};
#endif

/***********************************************************
 *				record ģʽ�� adkey table
 ***********************************************************/
#if TCFG_APP_RECORD_EN
const u16 record_key_ad_table[KEY_AD_NUM_MAX][KEY_EVENT_MAX] = {
    //����             //����          //hold         //̧��            //˫��                //����
			[0] = {
				KEY_NULL, 	KEY_NULL,		KEY_NULL,	KEY_NULL,		KEY_NULL,			KEY_NULL
			},
			[1] = {
				KEY_NULL,	KEY_NULL, 		KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
			},
			[2] = {
				KEY_NULL, 	KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,	KEY_NULL
			},
			[3] = {
				KEY_NULL,	KEY_NULL,	KEY_NULL,			KEY_NULL,		KEY_NULL,	KEY_NULL
			},
			[4] = {
				KEY_NULL,	 KEY_NULL,		KEY_NULL,	KEY_NULL,		KEY_NULL,			KEY_NULL
			},
			[5] = {
				KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
			},
			[6] = {
				KEY_COME_TO_PLAY_REC_FILE,	KEY_REC_STOP,		KEY_NULL,		KEY_NULL,		KEY_NULL,			KEY_NULL
			},
			[7] = {
				KEY_RGB_MODE,		   KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,		KEY_NULL
			},
			[8] = {
				KEY_CHANGE_MODE,		  KEY_NULL, 		KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
			},
			[9] = {
				KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
			},

		
		};
#endif

/***********************************************************
 *				rtc ģʽ�� adkey table
 ***********************************************************/
#if TCFG_APP_RTC_EN
const u16 rtc_key_ad_table[KEY_AD_NUM_MAX][KEY_EVENT_MAX] = {
    //����             //����          //hold         //̧��            //˫��                //����
    [0] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [1] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [2] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [3] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [4] = {
        KEY_NULL,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
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
 *				spdif ģʽ�� adkey table
 ***********************************************************/
#if TCFG_APP_SPDIF_EN
const u16 spdif_key_ad_table[KEY_AD_NUM_MAX][KEY_EVENT_MAX] = {
    //����             //����          //hold         //̧��            //˫��                //����
    [0] = {
        KEY_CHANGE_MODE,	KEY_NULL,	KEY_NULL,			KEY_NULL,		KEY_NULL,	KEY_NULL
    },
    [1] = {
        KEY_REC_MODE,	KEY_COME_TO_PLAY_REC_FILE,		KEY_NULL,		KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [2] = {
        KEY_MUSIC_PREV,   KEY_NULL,		KEY_NULL,	KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [3] = {
        KEY_MUSIC_PP,	KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [4] = {
        KEY_MUSIC_NEXT,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,	KEY_NULL
    },
    [5] = {
        KEY_RGB_MODE,		   KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [6] = {
        KEY_VOCAL_PP,		   KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [7] = {
        KEY_NULL,		  KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [8] = {
        KEY_MIC_ONOFF,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },
    [9] = {
        KEY_MUSIC_CHANGE_REPEAT,		KEY_NULL,			KEY_NULL,			KEY_NULL,		KEY_NULL,			KEY_NULL
    },

};
#endif

/***********************************************************
 *				idle ģʽ�� adkey table
 ***********************************************************/
const u16 idle_key_ad_table[KEY_AD_NUM_MAX][KEY_EVENT_MAX] = {
    //����             //����          //hold         //̧��            //˫��                //����
    [0] = {
        KEY_STANBY,	    KEY_NULL,		KEY_NULL,	KEY_NULL,   	KEY_NULL,			KEY_NULL
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

};




