
#ifdef SUPPORT_KPH_IR_FUNCTION

#ifdef IRKEY_RT_00FF_MUTE_807F_KEY

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
#define IR_U_SD                11
#define IR_1                   12
#define IR_2                   13
#define IR_3                   14
#define IR_4                   15
#define IR_5                   16
#define IR_6                   17
#define IR_7                   18
#define IR_8                   19
#define IR_9                   20
#define IR_REC                 21
#define IR_RECPLAY             22
#define IR_AUX_MODE            23
#define IR_FM_MODE             24
#define IR_PREV_ScanDn         25
#define IR_NEXT_ScanUP         26
#define IR_PP_FmPP             27
#define IR_STOP          	 28
#define IR_CH_UP             29
#define IR_CH_DN           30



#define SYS_CODE      0x7F80
#define SYS_CODE_1    0xFF00
#define IR_KEY_MAX             40

const u8 IRTab[] =
{
	0x12, 0x1a, 0x1e, 
	0x01, 0x02, 0x03, 
	0x04, 0x05, 0x06, 
	0x07, 0x08, 0x09, 
	0x0a, 0x1b, 0x1f, 
	0x0c, 0x0d, 0x0e, 
	0x00, 0x0f, 0x19, 
};

const u8 IRTab_change[] =
{ 
	IR_MUTE,  IR_MODE,  IR_RPT,
	IR_PP,  IR_PREV,   IR_NEXT,
	IR_EQ,   IR_VOL_DN,   IR_VOL_UP,
	IR_0,   IR_RECPLAY,      IR_REC,
	IR_1,   IR_2,      IR_3,
	IR_4,   IR_5,      IR_6,
	IR_7,   IR_8,      IR_9,
};

const u8 IRTab_1[] =
{
   0x45, 0x46, 0x47,  // power   mode   mute
   0x44, 0x40, 0x43,  //   pp       pre     nex
   0x07, 0x15, 0x09,  //  eq      v-     v+
   0x16, 0x19, 0x0d,  //  0     rep    u/sd
   0x0c, 0x18, 0x5e,  //   1     2       3
   0x08, 0x1c, 0x5a,  //   4    5       6
   0x42, 0x52, 0x4a,  //    7      8      9
};

const u8 IRTab_change_1[] =
{ 
	IR_POWER,  IR_MODE,	 IR_MUTE,
	IR_PP,		 IR_PREV,	 IR_NEXT,
	IR_EQ,		 IR_VOL_DN,  IR_VOL_UP,
	IR_0,		 IR_RPT,	 IR_U_SD,
	IR_1,		 IR_2,		 IR_3,
	IR_4,		 IR_5,		 IR_6,
	IR_7,		 IR_8,		 IR_9,
};

#endif
#endif


