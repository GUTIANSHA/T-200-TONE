/*
#define KEY0_RES         0    //PREV  1
#define KEY1_RES         10  // NEXT  2
#define KEY2_RES         20   // MODE  3
#define KEY3_RES         33   // PP      4
#define KEY4_RES         47 //MIC/VOCAL     5
#define KEY5_RES         68 //REC	6
#define KEY6_RES         100 //GANXIE	7
#define KEY7_RES         150 // ZHANGSHENG	8
#define KEY8_RES         200 //HECAI			9
#define KEY9_RES         330  // KOUSHAO		0
#define KEY10_RES         560 // KAICHANG	REPT
#define KEY11_RES         1000 //JIQIANG		MUTE
*/ 
 /***********************************************************
  * 			 bt 模式的 adkey table
  ***********************************************************/
 
#if TCFG_APP_BT_EN
 
 const u16 bt_key_ad_table[KEY_AD_NUM_MAX][KEY_EVENT_MAX] = {
	 //单击 			//长按			//hold		   //抬起			 //双击 			   //三击
	 [0] = {
		 KEY_MUSIC_PREV, KEY_NULL,		 KEY_NULL,		 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [1] = {
		 KEY_MUSIC_NEXT,   KEY_NULL,	 KEY_NULL,	 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [2] = {
		 KEY_CHANGE_MODE,	 KEY_VOICE_ONOFF,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [3] = {
		 KEY_MUSIC_PP,	 KEY_TWS_SEARCH_REMOVE_PAIR, KEY_NULL,			 KEY_NULL,		 KEY_NULL,	 KEY_NULL
	 },
	 [4] = {
		 KEY_MIC_ONOFF,	 KEY_VOCAL_PP, KEY_NULL,			 KEY_NULL,		 KEY_NULL,	 KEY_NULL
	 },
	 [5] = {
		 KEY_MUSIC_PLAYE_REC_FILE,			KEY_REC_MODE,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,		 KEY_NULL
	 },
	 [6] = {
		 KEY_GANXIE,		 KEY_TONE_VOL_DOWN,			 KEY_TONE_VOL_DOWN,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [7] = {
		 KEY_ZHANGSHENG,		 KEY_AMP_MUTE,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [8] = {
		 KEY_HECAI,		 KEY_OK_MIC_MAN,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [9] = {
		 KEY_KOUSHAO,		 KEY_OK_MIC_WOMAN,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
 	 [10] = {
		 KEY_KAICHANG,		 KEY_TONE_VOL_UP,			 KEY_TONE_VOL_UP,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },	 
	 [11] = {
		 KEY_JIQIANG,		 KEY_OK_MIC_RESET,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },

//AD1
	 [12] = {
		 KEY_NULL,   KEY_OK_MIC_LOOP,	 KEY_NULL,	 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [13] = {
		 KEY_NULL,	 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [14] = {
		 KEY_NULL,			KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,		 KEY_NULL
	 },
	 [15] = {
		 KEY_NULL,			KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,		 KEY_NULL
	 },
	 [16] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [17] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [18] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [19] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
 	 [20] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },	 
	 [21] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },	 
	 [22] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },	 
	 [23] = {
		 KEY_MUTE,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },	 
		 
 };
#endif
 
 /***********************************************************
  * 			 fm 模式的 adkey table
  ***********************************************************/
#if TCFG_APP_FM_EN
 const u16 fm_key_ad_table[KEY_AD_NUM_MAX][KEY_EVENT_MAX] = {
	 //单击 			//长按			//hold		   //抬起			 //双击 			   //三击
	 [0] = {
		 KEY_FM_PREV_STATION, 		 KEY_FM_SCAN_UP,		 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [1] = {
		 KEY_FM_NEXT_STATION,   KEY_FM_SCAN_DOWN ,	 KEY_NULL,	 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [2] = {
		 KEY_CHANGE_MODE,	 KEY_VOICE_ONOFF,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [3] = {
		 KEY_FM_SCAN_ALL,	 KEY_NULL, KEY_NULL,			 KEY_NULL,		 KEY_NULL,	 KEY_NULL
	 },
	 [4] = {
		 KEY_MIC_ONOFF,	 KEY_NULL, KEY_NULL,			 KEY_NULL,		 KEY_NULL,	 KEY_NULL
	 },
	 [5] = {
		 KEY_MUSIC_PLAYE_REC_FILE,			KEY_REC_MODE,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,		 KEY_NULL
	 },
	 [6] = {
		 KEY_GANXIE,		 KEY_TONE_VOL_DOWN,			 KEY_TONE_VOL_DOWN,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [7] = {
		 KEY_ZHANGSHENG,		 KEY_AMP_MUTE,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [8] = {
		 KEY_HECAI,		 KEY_OK_MIC_MAN,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [9] = {
		 KEY_KOUSHAO,		 KEY_OK_MIC_WOMAN,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
 	 [10] = {
		 KEY_KAICHANG,		 KEY_TONE_VOL_UP,			 KEY_TONE_VOL_UP,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },	 
	 [11] = {
		 KEY_JIQIANG,		 KEY_OK_MIC_RESET,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 

//AD1
	 [12] = {
		 KEY_IR_NUM_1,   KEY_OK_MIC_LOOP,	 KEY_NULL,	 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [13] = {
		 KEY_IR_NUM_2,	 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [14] = {
		 KEY_IR_NUM_3,			KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,		 KEY_NULL
	 },
	 [15] = {
		 KEY_IR_NUM_4,			KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,		 KEY_NULL
	 },
	 [16] = {
		 KEY_IR_NUM_5,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [17] = {
		 KEY_IR_NUM_6,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [18] = {
		 KEY_IR_NUM_7,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [19] = {
		 KEY_IR_NUM_8,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
 	 [20] = {
		 KEY_IR_NUM_9,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },	 
	 [21] = {
		 KEY_IR_NUM_0,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },	 
	 [22] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },	 
	 [23] = {
		 KEY_MUTE,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },	 	 
 };
#endif
 
 /***********************************************************
  * 			 linein 模式的 adkey table
  ***********************************************************/
#if TCFG_APP_LINEIN_EN
 const u16 linein_key_ad_table[KEY_AD_NUM_MAX][KEY_EVENT_MAX] = {
	 //单击 			//长按			//hold		   //抬起			 //双击 			   //三击
	 [0] = {
		 KEY_NULL, KEY_NULL,		 KEY_NULL,		 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [1] = {
		 KEY_NULL,   KEY_NULL,	 KEY_NULL,	 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [2] = {
		 KEY_CHANGE_MODE,	 KEY_VOICE_ONOFF,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [3] = {
		 KEY_NULL,	 KEY_NULL, KEY_NULL,			 KEY_NULL,		 KEY_NULL,	 KEY_NULL
	 },
	 [4] = {
		 KEY_MIC_ONOFF,	 KEY_NULL, KEY_NULL,			 KEY_NULL,		 KEY_NULL,	 KEY_NULL
	 },
	 [5] = {
		 KEY_MUSIC_PLAYE_REC_FILE,			KEY_REC_MODE,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,		 KEY_NULL
	 },
	 [6] = {
		 KEY_GANXIE,		 KEY_TONE_VOL_DOWN,			 KEY_TONE_VOL_DOWN,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [7] = {
		 KEY_ZHANGSHENG,		 KEY_AMP_MUTE,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [8] = {
		 KEY_HECAI,		 KEY_OK_MIC_MAN,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [9] = {
		 KEY_KOUSHAO,		 KEY_OK_MIC_WOMAN,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
 	 [10] = {
		 KEY_KAICHANG,		 KEY_TONE_VOL_UP,			 KEY_TONE_VOL_UP,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },	 
	 [11] = {
		 KEY_JIQIANG,		 KEY_OK_MIC_RESET,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	

//AD1
	 [12] = {
		 KEY_NULL,   KEY_OK_MIC_LOOP,	 KEY_NULL,	 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [13] = {
		 KEY_NULL,	 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [14] = {
		 KEY_NULL,			KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,		 KEY_NULL
	 },
	 [15] = {
		 KEY_NULL,			KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,		 KEY_NULL
	 },
	 [16] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [17] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [18] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [19] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
 	 [20] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },	 
	 [21] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },	 
	 [22] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },	 
	 [23] = {
		 KEY_MUTE,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },	

};
#endif
 
 /***********************************************************
  * 			 music 模式的 adkey table
  ***********************************************************/
#if TCFG_APP_MUSIC_EN
 
 const u16 music_key_ad_table[KEY_AD_NUM_MAX][KEY_EVENT_MAX] = {
	 //单击 			//长按			//hold		   //抬起			 //双击 			   //三击
	 [0] = {
		 KEY_MUSIC_PREV, KEY_MUSIC_FR,		 KEY_MUSIC_FR,		 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [1] = {
		 KEY_MUSIC_NEXT,   KEY_MUSIC_FF,	 KEY_MUSIC_FF,	 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [2] = {
		 KEY_CHANGE_MODE,	 KEY_VOICE_ONOFF,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [3] = {
		 KEY_MUSIC_PP,	 KEY_NULL, KEY_NULL,			 KEY_NULL,		 KEY_NULL,	 KEY_NULL
	 },
	 [4] = {
		 KEY_MIC_ONOFF,	 KEY_VOCAL_PP, KEY_NULL,			 KEY_NULL,		 KEY_NULL,	 KEY_NULL
	 },
	 [5] = {
		 KEY_MUSIC_PLAYE_REC_FILE,			KEY_REC_MODE,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,		 KEY_NULL
	 },
	 [6] = {
		 KEY_GANXIE,		 KEY_TONE_VOL_DOWN,			 KEY_TONE_VOL_DOWN,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [7] = {
		 KEY_ZHANGSHENG,		 KEY_AMP_MUTE,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [8] = {
		 KEY_HECAI,		 KEY_OK_MIC_MAN,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [9] = {
		 KEY_KOUSHAO,		 KEY_OK_MIC_WOMAN,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
 	 [10] = {
		 KEY_KAICHANG,		 KEY_TONE_VOL_UP,			 KEY_TONE_VOL_UP,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },	 
	 [11] = {
		 KEY_JIQIANG,		 KEY_OK_MIC_RESET,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 

//AD1
	 [12] = {
		 KEY_IR_NUM_1,   KEY_OK_MIC_LOOP,	 KEY_NULL,	 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [13] = {
		 KEY_IR_NUM_2,	 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [14] = {
		 KEY_IR_NUM_3,			KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,		 KEY_NULL
	 },
	 [15] = {
		 KEY_IR_NUM_4,			KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,		 KEY_NULL
	 },
	 [16] = {
		 KEY_IR_NUM_5,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [17] = {
		 KEY_IR_NUM_6,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [18] = {
		 KEY_IR_NUM_7,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [19] = {
		 KEY_IR_NUM_8,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
 	 [20] = {
		 KEY_IR_NUM_9,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },	 
	 [21] = {
		 KEY_IR_NUM_0,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },	 
	 [22] = {
		 KEY_MUSIC_CHANGE_REPEAT,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },	 
	 [23] = {
		 KEY_MUTE,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },	
	 	 
 };
#endif
 
 /***********************************************************
  * 			 pc 模式的 adkey table
  ***********************************************************/
#if TCFG_APP_PC_EN
 const u16 pc_key_ad_table[KEY_AD_NUM_MAX][KEY_EVENT_MAX] = {
	 //单击 			//长按			//hold		   //抬起			 //双击 			   //三击
	 [0] = {
		 KEY_NULL, KEY_NULL,		 KEY_NULL,		 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [1] = {
		 KEY_NULL,   KEY_NULL,	 KEY_NULL,	 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [2] = {
		 KEY_CHANGE_MODE,	 KEY_VOICE_ONOFF,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [3] = {
		 KEY_NULL,	 KEY_NULL, KEY_NULL,			 KEY_NULL,		 KEY_NULL,	 KEY_NULL
	 },
	 [4] = {
		 KEY_NULL,	 KEY_NULL, KEY_NULL,			 KEY_NULL,		 KEY_NULL,	 KEY_NULL
	 },
	 [5] = {
		 KEY_NULL,			KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,		 KEY_NULL
	 },
	 [6] = {
		 KEY_GANXIE,		 KEY_TONE_VOL_DOWN,			 KEY_TONE_VOL_DOWN,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [7] = {
		 KEY_ZHANGSHENG,		 KEY_AMP_MUTE,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [8] = {
		 KEY_HECAI,		 KEY_OK_MIC_MAN,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [9] = {
		 KEY_KOUSHAO,		 KEY_OK_MIC_WOMAN,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
 	 [10] = {
		 KEY_KAICHANG,		 KEY_TONE_VOL_UP,			 KEY_TONE_VOL_UP,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },	 
	 [11] = {
		 KEY_JIQIANG,		 KEY_OK_MIC_RESET,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 
	

//AD1
	 [12] = {
		 KEY_NULL,   KEY_NULL,	 KEY_NULL,	 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [13] = {
		 KEY_NULL,	 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [14] = {
		 KEY_NULL,			KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,		 KEY_NULL
	 },
	 [15] = {
		 KEY_NULL,			KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,		 KEY_NULL
	 },
	 [16] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [17] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [18] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [19] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
 	 [20] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },	 
	 [21] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },	 
	 [22] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },	 
	 [23] = {
		 KEY_MUTE,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },	
 };
#endif
 
 /***********************************************************
  * 			 record 模式的 adkey table
  ***********************************************************/
#if TCFG_APP_RECORD_EN
 const u16 record_key_ad_table[KEY_AD_NUM_MAX][KEY_EVENT_MAX] = {
	 //单击 			//长按			//hold		   //抬起			 //双击 			   //三击
	 [0] = {
		 KEY_NULL,	 KEY_NULL,	 KEY_NULL,			 KEY_NULL,		 KEY_NULL,	 KEY_NULL
	 },
	 [1] = {
		 KEY_NULL,	 KEY_NULL,		 KEY_NULL,		 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [2] = {
		 KEY_NULL,	 KEY_NULL,		 KEY_NULL,	 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [3] = {
		 KEY_NULL,	 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [4] = {
		 KEY_NULL,	  KEY_NULL, 		 KEY_NULL,			 KEY_NULL,		 KEY_NULL,	 KEY_NULL
	 },
	 [5] = {
		 KEY_REC_STOP,			KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,		 KEY_NULL
	 },
	 [6] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [7] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [8] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [9] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
  	 [10] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },	 
	 [11] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },

//AD1
	 [12] = {
		 KEY_NULL,   KEY_NULL,	 KEY_NULL,	 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [13] = {
		 KEY_NULL,	 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [14] = {
		 KEY_NULL,			KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,		 KEY_NULL
	 },
	 [15] = {
		 KEY_NULL,			KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,		 KEY_NULL
	 },
	 [16] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [17] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [18] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [19] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
 	 [20] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },	 
	 [21] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },	 
	 [22] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },	 
	 [23] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },	
 };
#endif
 
 /***********************************************************
  * 			 rtc 模式的 adkey table
  ***********************************************************/
#if TCFG_APP_RTC_EN
 const u16 rtc_key_ad_table[KEY_AD_NUM_MAX][KEY_EVENT_MAX] = {
	 //单击 			//长按			//hold		   //抬起			 //双击 			   //三击
	 [0] = {
		 KEY_CHANGE_MODE, KEY_POWEROFF, 	 KEY_POWEROFF_HOLD,  KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [1] = {
		 KEY_RTC_DOWN,		 KEY_RTC_DOWN,	 KEY_RTC_DOWN,	 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [2] = {
		 KEY_RTC_SW_POS,	 KEY_RTC_SW,	 KEY_NULL,		 KEY_NULL,		 KEY_NULL,				 KEY_NULL
	 },
	 [3] = {
		 KEY_RTC_UP,		 KEY_RTC_UP,	 KEY_RTC_UP,	 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [4] = {
		 KEY_NULL,			 KEY_NULL,		 KEY_NULL,		 KEY_NULL,		 KEY_REVERB_OPEN,				 KEY_NULL
	 },
	 [5] = {
		 KEY_NULL,			 KEY_NULL,		 KEY_NULL,		 KEY_NULL,		 KEY_NULL,				 KEY_NULL
	 },
	 [6] = {
		 KEY_NULL,			 KEY_NULL,		 KEY_NULL,		 KEY_NULL,		 KEY_NULL,				 KEY_NULL
	 },
	 [7] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [8] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [9] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
  	 [10] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },	 
	 [11] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },

//AD1
	 [12] = {
		 KEY_NULL,   KEY_NULL,	 KEY_NULL,	 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [13] = {
		 KEY_NULL,	 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [14] = {
		 KEY_NULL,			KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,		 KEY_NULL
	 },
	 [15] = {
		 KEY_NULL,			KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,		 KEY_NULL
	 },
	 [16] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [17] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [18] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [19] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
 	 [20] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },	 
	 [21] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },	 
	 [22] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },	 
	 [23] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },	
 };
#endif
 
 /***********************************************************
  * 			 spdif 模式的 adkey table
  ***********************************************************/
#if TCFG_APP_SPDIF_EN
 const u16 spdif_key_ad_table[KEY_AD_NUM_MAX][KEY_EVENT_MAX] = {
	 //单击 			//长按			//hold		   //抬起			 //双击 			   //三击
	 [0] = {
		 KEY_CHANGE_MODE, KEY_POWEROFF, 	 KEY_POWEROFF_HOLD,  KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [1] = {
		 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL,		 KEY_NULL,				 KEY_NULL
	 },
	 [2] = {
		 KEY_MUSIC_PP,		 KEY_POWEROFF,	 KEY_POWEROFF_HOLD,  KEY_NULL,		 KEY_NULL,				 KEY_NULL
	 },
	 [3] = {
		 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL,		 KEY_NULL,				 KEY_NULL
	 },
	 [4] = {
		 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL,		 KEY_SPDIF_SW_SOURCE,	 KEY_NULL
	 },
	 [5] = {
		 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL,		 KEY_NULL,				 KEY_NULL
	 },
	 [6] = {
		 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL,		 KEY_SPDIF_SW_SOURCE,	 KEY_NULL
	 },
	 [7] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [8] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [9] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
  	 [10] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },	 
	 [11] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },

//AD1
	 [12] = {
		 KEY_NULL,   KEY_NULL,	 KEY_NULL,	 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [13] = {
		 KEY_NULL,	 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [14] = {
		 KEY_NULL,			KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,		 KEY_NULL
	 },
	 [15] = {
		 KEY_NULL,			KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,		 KEY_NULL
	 },
	 [16] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [17] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [18] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [19] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
 	 [20] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },	 
	 [21] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },	 
	 [22] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },	 
	 [23] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },	
 };
#endif
 
 /***********************************************************
  * 			 idle 模式的 adkey table
  ***********************************************************/
 const u16 idle_key_ad_table[KEY_AD_NUM_MAX][KEY_EVENT_MAX] = {
	 //单击 			//长按			//hold		   //抬起			 //双击 			   //三击
	 [0] = {
		 KEY_NULL,		 KEY_NULL,		 KEY_NULL,	 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [1] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [2] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [3] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [4] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [5] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [6] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [7] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [8] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [9] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
  	 [10] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },	 
	 [11] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },

//AD1
	 [12] = {
		 KEY_NULL,   KEY_NULL,	 KEY_NULL,	 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [13] = {
		 KEY_NULL,	 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [14] = {
		 KEY_NULL,			KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,		 KEY_NULL
	 },
	 [15] = {
		 KEY_NULL,			KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,		 KEY_NULL
	 },
	 [16] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [17] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [18] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [19] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
 	 [20] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },	 
	 [21] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },	 
	 [22] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },	 
	 [23] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },	
 };
 
 
 
 
 
