#ifdef SUPPORT_KPH_IR_FUNCTION


#define SYS_CODE      0xFF00
#define SYS_CODE_1      0x7F80


enum{
	IR_POWER,
	IR_MODE,
IR_PP,							IR_MUTE,
	IR_VOL_UP,
IR_PREV,		IR_MENU,		IR_NEXT,
	IR_VOL_DN,
IR_RPT,							IR_EQ,
IR_1,			IR_2,			IR_3,
IR_4,			IR_5,			IR_6,
IR_7,			IR_8,			IR_9,
	IR_0,
IR_MENU_VDN ,IR_MENU_MIC,	IR_MENU_VUP,
IR_MICVOL_DN,IR_MICVOL_UP,
IR_BASS_UP,	IR_BASS_DN ,
IR_TRE_UP,	IR_TRE_DN,
IR_LED_ONOFF,IR_PREV_MICVDN,IR_NEXT_MICVU,
IR_KEY_MAX,
};

const u8 IRTab[] =
{
0x45,
0x46,
0x0d,			0x47,
0x09,
0x40,	0x44,	0x43,
0x15,
0x19,			0x07,	
0x0c,	0x18,	0x5e,
0x08,	0x1c,	0x5a,
0x42,	0x52,	0x4a,
0x16,
};

const u8 IRTab_change[] =
{ 
	IR_POWER,
	IR_MODE,
IR_PP,							IR_MUTE,
	IR_VOL_UP,
IR_PREV_MICVDN,		IR_MENU,		IR_NEXT_MICVU,
	IR_VOL_DN,
IR_RPT,							IR_EQ,
IR_1,			IR_2,			IR_3,
IR_4,			IR_5,			IR_6,
IR_7,			IR_8,			IR_9,
	IR_0,
};












const u8 IRTab_1[] =
{
		   0x60,
   0x61,   0x62,   0x63,
   0x64,   0x65,   0x66,   
		   
		   0x67,
   0x68,   0x69,   0x70,
		   0x71,
		   
//   0x72,   0x73,   0x74,
//   0x75,   0x76,   0x77,
  0x75,    	  0x77,
   0x78,   0x79,   0x80,
		   0x81,
};

const u8 IRTab_change_1[] =
{ 
					IR_MODE,
	IR_LED_ONOFF,	IR_MUTE,		IR_RPT,		
	IR_PREV, 		IR_PP,			IR_NEXT,
	
					IR_MENU_VUP,
	IR_MICVOL_DN,	IR_MENU_MIC,		IR_MICVOL_UP,
					IR_MENU_VDN,
					
//	IR_NULL,		IR_NULL,		IR_NULL,
//	IR_BASS_UP,		IR_NULL,		IR_TRE_UP,
	IR_BASS_UP,				IR_TRE_UP,
	IR_BASS_DN,		IR_VOL_UP,		IR_TRE_DN,
					IR_VOL_DN,
};


#endif


