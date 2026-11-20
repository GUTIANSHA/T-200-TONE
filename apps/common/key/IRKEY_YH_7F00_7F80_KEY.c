
#ifdef SUPPORT_KPH_IR_FUNCTION

#ifdef IRKEY_YH_7F00_7F80_KEY

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
#define IR_LED_ONOFF           28
#define IR_REC_DEL             29
#define IR_BT_MODE           30
#define IR_MIC_PRIO             31
#define IR_VOCAL_PP             32
#define IR_BT_TWS             33
#define IR_KAICHANG             34
#define IR_ZHANGSHENG             35
#define IR_BISHI             36
#define IR_KOUSHAO             37
#define IR_HECAI             38
#define IR_QIQNGSHENG             39
#define IR_MOMODA             40
#define IR_DAERGUANG             41
#define IR_GANXIE             42

#define IR_KEY_MAX             43



#define SYS_CODE      0x7F00
#define SYS_CODE_1    0x7f80

const u8 IRTab[] =
{
	0x12, 	    0x10,  //  POWER   	  MUTE
	0x01, 0x02, 0x03,  //  1      2   	  3
	0x04, 0x05, 0x06,  //  4      5          6
	0x07, 0x08, 0x09,  //  7        8            9
	0x4a, 0x00, 0x0b,  //  usd     0          mode
	0x5a, 	     0x55,  // aux			bt
	         0x1b, 		  //  	v+
	0x1e, 0x17, 0x1a,  // prev	pp		next
	         0x1f, 		  //  	v-
	0x14, 	     0x18,  // prio			eq
	0x4c, 0x49, 0x56,  // rpt	rec		del
	         
};

const u8 IRTab_change[] =
{ 

	IR_POWER, 	    IR_MUTE,  //  POWER   	  MUTE
	IR_1,		 IR_2,		 IR_3,  //  1      2   	  3
	IR_4,		 IR_5,		 IR_6,  //  4      5          6
	IR_7,		 IR_8,		 IR_9,  //  7        8            9
	IR_U_SD, 	IR_0, 		IR_MODE,  //  usd     0          mode
	IR_AUX_MODE, 	     IR_BT_MODE,  // aux			bt	
	         IR_VOL_UP, 		  //  	v+
	IR_PREV, IR_PP, IR_NEXT,  // prev	pp		next
	         IR_VOL_DN, 		  //  	v-	         
	IR_MIC_PRIO, 	     IR_EQ,  // prio			eq
	IR_RPT, IR_REC, IR_RECPLAY,  // rpt	rec		del
};

const u8 IRTab_1[] =
{
	0x1e, 	    0x12,  //  rpt	   	  mute
	0x0a, 0x1b, 0x1f,  //  1      2   	  3
	0x0c, 0x0d, 0x0e,  //  4      5          6
	0x00, 0x0f, 0x19,  //  7        8            9
	0x07, 0x16, 0x1a,  //  prio     0          mode
	         0x06, 		  //  	v+
	0x02, 0x01, 0x03,  // prev	pp		next
	         0x05, 		  //  	v-
	0x17, 0x04, 0x08,  // vocal	bt		tws
	0x09, 	     0x10,  // rec			recplay
	0x15, 0x13, 0x40,  // kaichang  zhangsheng bishi
	0x41, 0x42, 0x43,  //koushao  hecai  qiangsheng 
	0x44, 0x45, 0x46,  // momoda  daerguang  ganxie

};

const u8 IRTab_change_1[] =
{ 
	IR_RPT, 	   				 IR_MUTE,  //  rpt	   	  mute
	IR_1,		 IR_2,		 IR_3, //  1      2   	  3
	IR_4,		 IR_5,		 IR_6, //  4      5          6
	IR_7,		 IR_8,		 IR_9, //  7        8            9
	IR_MIC_PRIO, IR_0, 		  IR_MODE,  //  prio     0          mode
	         		IR_VOL_UP, 			  //  	v+
	IR_PREV, 	IR_PP, 		IR_NEXT,  // prev	pp		next
	        		 IR_VOL_DN, 			  //  	v-
	IR_VOCAL_PP, IR_BT_MODE, IR_BT_TWS,  // vocal	bt		tws
	IR_REC, 	   				  IR_RECPLAY,  // rec			recplay
	IR_KAICHANG, IR_ZHANGSHENG, IR_BISHI,  // kaichang  zhangsheng bishi
	IR_KOUSHAO, IR_HECAI, IR_QIQNGSHENG,  //koushao  hecai  qiangsheng 
	IR_MOMODA, IR_DAERGUANG, IR_GANXIE,  // momoda  daerguang  ganxie
};


#endif
#endif


