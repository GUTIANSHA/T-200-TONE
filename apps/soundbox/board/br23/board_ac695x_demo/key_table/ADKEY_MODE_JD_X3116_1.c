
/*
#define KEY1_RES         1   mic
#define KEY2_RES         12   rec
#define KEY3_RES         22   vocal
#define KEY4_RES         47   pp/tws
#define KEY5_RES         82  eq
#define KEY6_RES         120	next
#define KEY7_RES         180	mode
#define KEY8_RES         560  pre
*/ 

 /***********************************************************
  * 			 music ģʽ�� adkey table
  ***********************************************************/
#if TCFG_APP_MUSIC_EN
 const u16 music_key_ad_table[KEY_AD_NUM_MAX][KEY_EVENT_MAX] = {
	 [0] = {
		 KEY_NULL,	 KEY_NULL,	 KEY_NULL,	 KEY_NULL,	 KEY_NULL,	 KEY_NULL
	 },
	 [1] = {
		 KEY_MIC_ONOFF, 	KEY_NULL,	KEY_NULL,	KEY_NULL,	KEY_NULL,	 KEY_NULL
	 },
	 [2] = {
		 KEY_MUSIC_PLAYE_REC_FILE,   KEY_REC_MODE,	 KEY_NULL,	 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [3] = {
		 KEY_VOCAL_PP,   KEY_NULL,	 KEY_NULL,	 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [4] = {
		 KEY_MUSIC_PP,   KEY_NULL,	 KEY_NULL,	 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [5] = {
		 KEY_EQ_MODE,   KEY_NULL,	 KEY_NULL,	 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [6] = {
		 KEY_MUSIC_NEXT,	KEY_NULL,		 KEY_NULL,		 KEY_NULL,		 KEY_NULL,	KEY_NULL
	 },
	 [7] = {
		 KEY_CHANGE_MODE,   KEY_NULL,	 KEY_NULL,	 KEY_NULL,		 KEY_NULL,	 KEY_NULL
	 },
	 [8] = {
		 KEY_MUSIC_PREV,	 KEY_NULL, 		 KEY_NULL,			 KEY_NULL,		 KEY_NULL,	 KEY_NULL
	 },
	 [9] = {
		 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,		KEY_NULL
	 },
 };
#endif

 /***********************************************************
  * 			 fm ģʽ�� adkey table
  ***********************************************************/
#if TCFG_APP_FM_EN
 const u16 fm_key_ad_table[KEY_AD_NUM_MAX][KEY_EVENT_MAX] = {
		 [0] = {
			 KEY_NULL,	 KEY_NULL,	 KEY_NULL,	 KEY_NULL,	 KEY_NULL,	 KEY_NULL
		 },
		 [1] = {
			 KEY_MIC_ONOFF, 	KEY_NULL,	KEY_NULL,	KEY_NULL,	KEY_NULL,	 KEY_NULL
		 },
		 [2] = {
			 KEY_COME_TO_PLAY_REC_FILE,	 KEY_REC_MODE,	 KEY_NULL,	 KEY_NULL,		 KEY_NULL,			 KEY_NULL
		 },
		 [3] = {
			 KEY_VOCAL_PP,	 KEY_NULL,	 KEY_NULL,	 KEY_NULL,		 KEY_NULL,			 KEY_NULL
		 },
		 [4] = {
			 KEY_FM_SCAN_ALL,	 KEY_NULL,	 KEY_NULL,	 KEY_NULL,		 KEY_NULL,			 KEY_NULL
		 },
		 [5] = {
			 KEY_EQ_MODE,	KEY_NULL,	 KEY_NULL,	 KEY_NULL,		 KEY_NULL,			 KEY_NULL
		 },
		 [6] = {
			 KEY_FM_NEXT_STATION,	KEY_NULL,		 KEY_NULL,		 KEY_NULL,		 KEY_NULL,	KEY_NULL
		 },
		 [7] = {
			 KEY_CHANGE_MODE,	KEY_NULL,	 KEY_NULL,	 KEY_NULL,		 KEY_NULL,	 KEY_NULL
		 },
		 [8] = {
			 KEY_FM_PREV_STATION,	 KEY_NULL,		 KEY_NULL,			 KEY_NULL,		 KEY_NULL,	 KEY_NULL
		 },
		 [9] = {
			 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,		KEY_NULL
		 },
	 };
#endif
 
 /***********************************************************
  * 			 linein ģʽ�� adkey table
  ***********************************************************/
#if TCFG_APP_LINEIN_EN
 const u16 linein_key_ad_table[KEY_AD_NUM_MAX][KEY_EVENT_MAX] = {
		 [0] = {
			 KEY_NULL,	 KEY_NULL,	 KEY_NULL,	 KEY_NULL,	 KEY_NULL,	 KEY_NULL
		 },
		 [1] = {
			 KEY_MIC_ONOFF, 	KEY_NULL,	KEY_NULL,	KEY_NULL,	KEY_NULL,	 KEY_NULL
		 },
		 [2] = {
			 KEY_COME_TO_PLAY_REC_FILE,	 KEY_REC_MODE,	 KEY_NULL,	 KEY_NULL,		 KEY_NULL,			 KEY_NULL
		 },
		 [3] = {
			 KEY_VOCAL_PP,	 KEY_NULL,	 KEY_NULL,	 KEY_NULL,		 KEY_NULL,			 KEY_NULL
		 },
		 [4] = {
			 KEY_MUSIC_PP,	 KEY_NULL,	 KEY_NULL,	 KEY_NULL,		 KEY_NULL,			 KEY_NULL
		 },
		 [5] = {
			 KEY_EQ_MODE,	KEY_NULL,	 KEY_NULL,	 KEY_NULL,		 KEY_NULL,			 KEY_NULL
		 },
		 [6] = {
			 KEY_NULL,	KEY_NULL,		 KEY_NULL,		 KEY_NULL,		 KEY_NULL,	KEY_NULL
		 },
		 [7] = {
			 KEY_CHANGE_MODE,	KEY_NULL,	 KEY_NULL,	 KEY_NULL,		 KEY_NULL,	 KEY_NULL
		 },
		 [8] = {
			 KEY_NULL,	 KEY_NULL,		 KEY_NULL,			 KEY_NULL,		 KEY_NULL,	 KEY_NULL
		 },
		 [9] = {
			 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,		KEY_NULL
		 },
	 };
#endif

 /***********************************************************
  * 			 bt ģʽ�� adkey table
  ***********************************************************/
 
#if TCFG_APP_BT_EN
 
 const u16 bt_key_ad_table[KEY_AD_NUM_MAX][KEY_EVENT_MAX] = {
		 [0] = {
			 KEY_NULL,	 KEY_NULL,	 KEY_NULL,	 KEY_NULL,	 KEY_NULL,	 KEY_NULL
		 },
		 [1] = {
			 KEY_MIC_ONOFF, 	KEY_NULL,	KEY_NULL,	KEY_NULL,	KEY_NULL,	 KEY_NULL
		 },
		 [2] = {
			 KEY_COME_TO_PLAY_REC_FILE,	 KEY_REC_MODE,	 KEY_NULL,	 KEY_NULL,		 KEY_NULL,			 KEY_NULL
		 },
		 [3] = {
			 KEY_VOCAL_PP,	 KEY_NULL,	 KEY_NULL,	 KEY_NULL,		 KEY_NULL,			 KEY_NULL
		 },
		 [4] = {
			 KEY_MUSIC_PP,	 KEY_TWS_SEARCH_REMOVE_PAIR,	 KEY_NULL,	 KEY_NULL,		 KEY_NULL,			 KEY_NULL
		 },
		 [5] = {
			 KEY_EQ_MODE,	KEY_NULL,	 KEY_NULL,	 KEY_NULL,		 KEY_NULL,			 KEY_NULL
		 },
		 [6] = {
			 KEY_MUSIC_NEXT,	KEY_NULL,		 KEY_NULL,		 KEY_NULL,		 KEY_NULL,	KEY_NULL
		 },
		 [7] = {
			 KEY_CHANGE_MODE,	KEY_NULL,	 KEY_NULL,	 KEY_NULL,		 KEY_NULL,	 KEY_NULL
		 },
		 [8] = {
			 KEY_MUSIC_PREV,	 KEY_NULL,		 KEY_NULL,			 KEY_NULL,		 KEY_NULL,	 KEY_NULL
		 },
		 [9] = {
			 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,		KEY_NULL
		 },
	 };
#endif
 

 

 
 /***********************************************************
  * 			 pc ģʽ�� adkey table
  ***********************************************************/
#if TCFG_APP_PC_EN
 const u16 pc_key_ad_table[KEY_AD_NUM_MAX][KEY_EVENT_MAX] = {
	 [0] = {
		 KEY_MUSIC_PP,	 KEY_NULL,	 KEY_NULL,	 KEY_NULL,	 KEY_NULL,	 KEY_NULL
	 },
	 [1] = {
		 KEY_MOMODA,	 KEY_NULL,	KEY_NULL,	KEY_NULL,KEY_NULL,	KEY_NULL
	 },
	 [2] = {
		 KEY_MIC_ONOFF, 	KEY_NULL,	KEY_NULL,	KEY_NULL,	KEY_NULL,	 KEY_NULL
	 },
	 [3] = {
		 KEY_CHANGE_MODE,   KEY_NULL,	 KEY_NULL,	 KEY_NULL,		 KEY_NULL,	 KEY_NULL
	 },
	 [4] = {
		 KEY_KOUSHAO,	   KEY_NULL, 	 KEY_NULL,	 KEY_NULL,		 KEY_NULL,	 KEY_NULL
	 },
	 [5] = {
		 KEY_REC_MODE,   KEY_NULL,	 KEY_NULL,	 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [6] = {
		 KEY_KAICHANG,		KEY_NULL,		 KEY_NULL,		 KEY_NULL,		 KEY_NULL,	KEY_NULL
	 },
	 [7] = {
		 KEY_MUSIC_NEXT,	KEY_NULL,		 KEY_NULL,		 KEY_NULL,		 KEY_NULL,	KEY_NULL
	 },
	 [8] = {
		 KEY_MUSIC_PREV,	 KEY_NULL, 		 KEY_NULL,			 KEY_NULL,		 KEY_NULL,	 KEY_NULL
	 },
	 [9] = {
		 KEY_HECAI,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,		KEY_NULL
	 },
	 };
#endif
 
 /***********************************************************
  * 			 record ģʽ�� adkey table
  ***********************************************************/
  
#if TCFG_APP_RECORD_EN
 const u16 record_key_ad_table[KEY_AD_NUM_MAX][KEY_EVENT_MAX] = {
		 [0] = {
			 KEY_NULL,	 KEY_NULL,	 KEY_NULL,	 KEY_NULL,	 KEY_NULL,	 KEY_NULL
		 },
		 [1] = {
			 KEY_NULL, 	KEY_NULL,	KEY_NULL,	KEY_NULL,	KEY_NULL,	 KEY_NULL
		 },
		 [2] = {
			 KEY_REC_STOP,	 KEY_NULL,	 KEY_NULL,	 KEY_NULL,		 KEY_NULL,			 KEY_NULL
		 },
		 [3] = {
			 KEY_NULL,	 KEY_NULL,	 KEY_NULL,	 KEY_NULL,		 KEY_NULL,			 KEY_NULL
		 },
		 [4] = {
			 KEY_NULL,	 KEY_NULL,	 KEY_NULL,	 KEY_NULL,		 KEY_NULL,			 KEY_NULL
		 },
		 [5] = {
			 KEY_NULL,	KEY_NULL,	 KEY_NULL,	 KEY_NULL,		 KEY_NULL,			 KEY_NULL
		 },
		 [6] = {
			 KEY_NULL,	KEY_NULL,		 KEY_NULL,		 KEY_NULL,		 KEY_NULL,	KEY_NULL
		 },
		 [7] = {
			 KEY_CHANGE_MODE,	KEY_NULL,	 KEY_NULL,	 KEY_NULL,		 KEY_NULL,	 KEY_NULL
		 },
		 [8] = {
			 KEY_NULL,	 KEY_NULL,		 KEY_NULL,			 KEY_NULL,		 KEY_NULL,	 KEY_NULL
		 },
		 [9] = {
			 KEY_NULL,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,		KEY_NULL
		 },
	 };
#endif
 
 /***********************************************************
  * 			 rtc ģʽ�� adkey table
  ***********************************************************/
#if TCFG_APP_RTC_EN
 const u16 rtc_key_ad_table[KEY_AD_NUM_MAX][KEY_EVENT_MAX] = {
	 [0] = {
		 KEY_MUSIC_PP,	 KEY_NULL,	 KEY_NULL,	 KEY_NULL,	 KEY_NULL,	 KEY_NULL
	 },
	 [1] = {
		 KEY_MOMODA,	 KEY_NULL,	KEY_NULL,	KEY_NULL,KEY_NULL,	KEY_NULL
	 },
	 [2] = {
		 KEY_MIC_ONOFF, 	KEY_NULL,	KEY_NULL,	KEY_NULL,	KEY_NULL,	 KEY_NULL
	 },
	 [3] = {
		 KEY_CHANGE_MODE,   KEY_NULL,	 KEY_NULL,	 KEY_NULL,		 KEY_NULL,	 KEY_NULL
	 },
	 [4] = {
		 KEY_KOUSHAO,	   KEY_NULL, 	 KEY_NULL,	 KEY_NULL,		 KEY_NULL,	 KEY_NULL
	 },
	 [5] = {
		 KEY_REC_MODE,   KEY_NULL,	 KEY_NULL,	 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [6] = {
		 KEY_KAICHANG,		KEY_NULL,		 KEY_NULL,		 KEY_NULL,		 KEY_NULL,	KEY_NULL
	 },
	 [7] = {
		 KEY_MUSIC_NEXT,	KEY_NULL,		 KEY_NULL,		 KEY_NULL,		 KEY_NULL,	KEY_NULL
	 },
	 [8] = {
		 KEY_MUSIC_PREV,	 KEY_NULL, 		 KEY_NULL,			 KEY_NULL,		 KEY_NULL,	 KEY_NULL
	 },
	 [9] = {
		 KEY_HECAI,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,		KEY_NULL
	 },
 };
#endif
 
 /***********************************************************
  * 			 spdif ģʽ�� adkey table
  ***********************************************************/
#if TCFG_APP_SPDIF_EN
 const u16 spdif_key_ad_table[KEY_AD_NUM_MAX][KEY_EVENT_MAX] = {
	 [0] = {
		 KEY_MUSIC_PP,	 KEY_NULL,	 KEY_NULL,	 KEY_NULL,	 KEY_NULL,	 KEY_NULL
	 },
	 [1] = {
		 KEY_MOMODA,	 KEY_NULL,	KEY_NULL,	KEY_NULL,KEY_NULL,	KEY_NULL
	 },
	 [2] = {
		 KEY_MIC_ONOFF, 	KEY_NULL,	KEY_NULL,	KEY_NULL,	KEY_NULL,	 KEY_NULL
	 },
	 [3] = {
		 KEY_CHANGE_MODE,   KEY_NULL,	 KEY_NULL,	 KEY_NULL,		 KEY_NULL,	 KEY_NULL
	 },
	 [4] = {
		 KEY_KOUSHAO,	   KEY_NULL, 	 KEY_NULL,	 KEY_NULL,		 KEY_NULL,	 KEY_NULL
	 },
	 [5] = {
		 KEY_REC_MODE,   KEY_NULL,	 KEY_NULL,	 KEY_NULL,		 KEY_NULL,			 KEY_NULL
	 },
	 [6] = {
		 KEY_KAICHANG,		KEY_NULL,		 KEY_NULL,		 KEY_NULL,		 KEY_NULL,	KEY_NULL
	 },
	 [7] = {
		 KEY_MUSIC_NEXT,	KEY_NULL,		 KEY_NULL,		 KEY_NULL,		 KEY_NULL,	KEY_NULL
	 },
	 [8] = {
		 KEY_MUSIC_PREV,	 KEY_NULL, 		 KEY_NULL,			 KEY_NULL,		 KEY_NULL,	 KEY_NULL
	 },
	 [9] = {
		 KEY_HECAI,		 KEY_NULL,			 KEY_NULL,			 KEY_NULL,		 KEY_NULL,		KEY_NULL
	 },
 };
#endif
 
 /***********************************************************
  * 			 idle ģʽ�� adkey table
  ***********************************************************/
 const u16 idle_key_ad_table[KEY_AD_NUM_MAX][KEY_EVENT_MAX] = {
	 [0] = {
		 KEY_NULL, 	 KEY_NULL,	KEY_NULL,	KEY_NULL,	KEY_NULL,		KEY_NULL
	 },
	 [1] = {
		 KEY_NULL, 	 KEY_NULL,	KEY_NULL,	KEY_NULL,	KEY_NULL,		KEY_NULL
	 },
	 [2] = {
		 KEY_NULL, 	 KEY_NULL,	KEY_NULL,	KEY_NULL,	KEY_NULL,		KEY_NULL
	 },
	 [3] = {
		 KEY_NULL, 	 KEY_NULL,	KEY_NULL,	KEY_NULL,	KEY_NULL,		KEY_NULL
	 },
	 [4] = {
		 KEY_NULL, 	 KEY_NULL,	KEY_NULL,	KEY_NULL,	KEY_NULL,		KEY_NULL
	 },
	 [5] = {
		 KEY_NULL, 	 KEY_NULL,	KEY_NULL,	KEY_NULL,	KEY_NULL,		KEY_NULL
	 },
	 [6] = {
		 KEY_NULL, 	 KEY_NULL,	KEY_NULL,	KEY_NULL,	KEY_NULL,		KEY_NULL
	 },
	 [7] = {
		 KEY_NULL, 	 KEY_NULL,	KEY_NULL,	KEY_NULL,	KEY_NULL,		KEY_NULL
	 },
	 [8] = {
		 KEY_NULL, 	 KEY_NULL,	KEY_NULL,	KEY_NULL,	KEY_NULL,		KEY_NULL
	 },
	 [9] = {
		 KEY_NULL, 	 KEY_NULL,	KEY_NULL,	KEY_NULL,	KEY_NULL,		KEY_NULL
	 },
 };
 
 
 
 
 
