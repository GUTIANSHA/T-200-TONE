#ifdef SUPPORT_KPH_IR_FUNCTION

#if 1

#define IR_POWER                0
#define IR_MODE                 1
#define IR_MUTE                 2
#define IR_PP                   3
#define IR_PREV                 4
#define IR_NEXT                 5
#define IR_VOL_DN               6
#define IR_VOL_UP               7
#define IR_EQ                   8
#define IR_LED					9

#define IR_0                    10
#define IR_1                   	11
#define IR_2                   	12
#define IR_3                   	13
#define IR_4                   	14
#define IR_5                   	15
#define IR_6                   	16
#define IR_7                   	17
#define IR_8                   	18
#define IR_9                   	19

#define IR_SCAN                 20//单独FM搜台
#define IR_ScanDn         		21
#define IR_ScanUP         		22
#define IR_PP_FmPP             	23//暂停，包括FM也是暂停
#define IR_CH_UP             	24
#define IR_CH_DN           		25
#define IR_FREQ_UP				26
#define IR_FREQ_DN				27

#define IR_REC                 	28//短按录音，长按录音播放
#define IR_RECPLAY             	29//录音播放
#define IR_REC_DEL				30//录音删除

#define IR_RPT                 	31
#define IR_STOP          	 	32
#define IR_U_SD                	33//音乐模式切换设备,FM模式搜台

#define IR_BT_MODE             	40
#define IR_AUX_MODE            	41
#define IR_FM_MODE             	42
#define IR_MUSIC_MODE			43	//音乐模式切换设备，其他模式跳到音乐
#define IR_USB_MODE				44
#define IR_SD_MODE				45
#define IR_SPDIF_MODE			46
#define IR_PC_MODE				47

#define SYS_CODE      0x7F80
#define SYS_CODE_1    0xFF00
#define IR_KEY_MAX             60

const u8 IRTab[] =
{
	0x41,		  	0x50,
			0x42,
	0x1a,   0x1b,   0x4f,
			0x43,

			0x5a,
	0x56,			0x51


};

const u8 IRTab_change[] =
{ 
	IR_POWER,				IR_MUTE,
	
				IR_PREV,
	IR_VOL_UP,	IR_PP,   	IR_VOL_DN, 
				IR_NEXT,

				IR_MODE,
	IR_EQ,					IR_BT_MODE


	// IR_POWER,  IR_MODE,  IR_RPT,
	// IR_PP,  IR_PREV,   IR_NEXT,
	// IR_EQ,   IR_VOL_DN,   IR_VOL_UP,
	// IR_0,   IR_RECPLAY,      IR_REC,
	// IR_1,   IR_2,      IR_3,
	// IR_4,   IR_5,      IR_6,
	// IR_7,   IR_8,      IR_9,
};

const u8 IRTab_1[] =
{
	0x45, 0x46, 0x47,//  POWER   mode  MUTE
	0x44, 0x40, 0x43,//  pp      prev     next
	0x07, 0x15, 0x09,//  eq      v-          v+
	0x16, 0x19, 0x0d,//  0       RPT     USD
	0x0c, 0x18, 0x5e,//  1       2             3
	0x08, 0x1c, 0x5a,//  4        5            6
	0x42, 0x52, 0x4a,//  7        8            9      
};

const u8 IRTab_change_1[] =
{ 
	IR_POWER,   IR_MODE,  		IR_MUTE,
	IR_PP,  	IR_PREV,   		IR_NEXT,
	IR_EQ,   	IR_VOL_DN,   	IR_VOL_UP,
	IR_0,  	    IR_RPT,      	IR_U_SD,
	IR_1,   	IR_2,      		IR_3,
	IR_4,   	IR_5,      		IR_6,
	IR_7,   	IR_8,      		IR_9,
};

#endif
#endif


