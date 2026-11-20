
#ifdef SUPPORT_KPH_IR_FUNCTION
#define SYS_CODE      0xBF00
enum{
	IR_MUTE,
	IR_MODE,				IR_RPT,
				IR_MENU_VUP,
	IR_VOL_DN,	IR_MENU,	IR_VOL_UP,
				IR_MENU_VDN,
	IR_MIC_NANS,IR_MIC_RST,	IR_MIC_NVS,
	IR_PREV,				IR_NEXT,
				IR_PP,
				IR_KEY_MAX,
};

const u8 IRTab[] =
{
	0x14,
	0x4a,			0x46,
			0x0a,
	0x19,	0x1b,	0x11,
			0x53,
	0x1a,	0x15,	0x1c,
	0x18,			0x1d,
			0x44,
};

const u8 IRTab_change[] =
{ 
	IR_MUTE,
	IR_MODE,				IR_RPT,
				IR_MENU_VUP,
	IR_VOL_DN,	IR_MENU,	IR_VOL_UP,
				IR_MENU_VDN,
	IR_MIC_NANS,IR_MIC_RST,	IR_MIC_NVS,
	IR_PREV,				IR_NEXT,
				IR_PP,

};

#endif


