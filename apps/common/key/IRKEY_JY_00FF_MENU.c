
#ifdef SUPPORT_KPH_IR_FUNCTION
#define SYS_CODE      0xFF00
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
	IR_PREV,		IR_MENU,		IR_NEXT,
					IR_VOL_DN,
	IR_RPT,							IR_EQ,
	IR_1,			IR_2,			IR_3,
	IR_4,			IR_5,			IR_6,
	IR_7,			IR_8,			IR_9,
					IR_0,
};

#endif


