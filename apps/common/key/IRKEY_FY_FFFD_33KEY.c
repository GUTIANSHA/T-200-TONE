
#ifdef SUPPORT_KPH_IR_FUNCTION


#define IR_POWER                0
#define IR_MODE                 1
#define IR_MUTE                 2
#define IR_PP                   3
#define IR_PREV                 4
#define IR_NEXT                 5
#define IR_EQ                   6
#define IR_VOL_DN               7
#define IR_VOL_UP               8
#define IR_0                    9
#define IR_RPT                 10
#define IR_USB_SD                11
#define IR_1                   12
#define IR_2                   13
#define IR_3                   14
#define IR_4                   15
#define IR_5                   16
#define IR_6                   17
#define IR_7                   18
#define IR_8                   19
#define IR_9                   20
#define IR_USB_SD		21
#define IR_MIC_PRIO                   22
#define IR_FM                              23
#define IR_BT                               24
#define IR_AUX                            25
#define IR_VOCAL_PP                   26
#define IR_BASS_DN                    27
#define IR_BASS_UP                    28
#define IR_TWS                         29
#define IR_TRE_DN                      30
#define IR_TRE_UP                      31
#define IR_STOP                          32
#define IR_MICVOL_DN                      33
#define IR_MICVOL_UP                      34
#define IR_REC                         35

#define IR_KEY_MAX             36


#define SYS_CODE      0xFDFF

const u8 IRTab[] =
{
0x1C,			0x08,  //  POWER    	   MUTE
0x01, 0x5F, 0x5B,  0x41,  //  USB  SD  FM   BT
0x18,	0x1A,	0x17,  //  AUXIN   V+	   OKVOL/ECHO
0x47,	0x06,	0x07,  //  BASS-  RESET BASS+
0x03, 	0x48, 	0x0A,  //  TRE-	V-		TRE+
0x4F, 	0x02, 	0x5D,  //  NEXT	PP		TONE+
0x4B, 	0x04, 	0x5C,  //  PREV	STOP		TON-
0x54, 	0x16, 	0x15,  //  1       2             3
0x50, 	0x12, 	0x11,  // 4        5            6
0x4C, 	0x0E, 	0x0D,  //  7        8            9     
0x42, 	0x0C,	 0x10,  //  RPT	0		REC
};

const u8 IRTab_change[] =
{ 
IR_POWER,    			 	IR_MUTE,
IR_AUX, 	IR_USB_SD,	IR_FM,	IR_BT,
IR_MICVOL_DN,		 IR_VOL_UP,	 IR_MICVOL_UP,
IR_BASS_DN,	 IR_TWS,  	IR_BASS_UP,
IR_TRE_DN,    IR_VOL_DN,	 IR_TRE_UP,
IR_NEXT,		 IR_PP,		 IR_MIC_PRIO,
IR_PREV,		 IR_STOP,  	IR_VOCAL_PP,
IR_1,		 IR_2,		 IR_3,
IR_4,		 IR_5,		 IR_6,
IR_7,		 IR_8,		 IR_9,
IR_RPT,		 IR_0,     		IR_REC,
};



#endif


