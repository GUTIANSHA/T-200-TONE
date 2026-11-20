#include "key_driver.h"
#include "irkey.h"
#include "gpio.h"
#include "asm/irflt.h"
#include "app_config.h"

#if TCFG_IRKEY_ENABLE

u8 ir_get_key_value(void);
//按键驱动扫描参数列表
struct key_driver_para irkey_scan_para = {
    .scan_time 	  	  = 10,				//按键扫描频率, 单位: ms
    .last_key 		  = NO_KEY,  		//上一次get_value按键值, 初始化为NO_KEY;
    .filter_time  	  = 1,				//按键消抖延时;
    .long_time 		  = 75,  			//按键判定长按数量
    .hold_time 		  = (75 + 15),  	//按键判定HOLD数量
    .click_delay_time = 1,				//按键被抬起后等待连击延时数量
    .key_type		  = KEY_DRIVER_TYPE_IR,
    .get_value 		  = ir_get_key_value,
};



const u8 IRTabFF00[] = {
#if 0	
    NKEY_00, NKEY_01, NKEY_02, NKEY_03, NKEY_04, NKEY_05, NKEY_06, IR_06, IR_15, IR_08, NKEY_0A, NKEY_0B, IR_12, IR_11, NKEY_0E, NKEY_0F,
    NKEY_10, NKEY_11, NKEY_12, NKEY_13, NKEY_14, IR_07, IR_09, NKEY_17, IR_13, IR_10, NKEY_1A, NKEY_1B, IR_16, NKEY_1D, NKEY_1E, NKEY_1F,
    NKEY_20, NKEY_21, NKEY_22, NKEY_23, NKEY_24, NKEY_25, NKEY_26, NKEY_27, NKEY_28, NKEY_29, NKEY_2A, NKEY_2B, NKEY_2C, NKEY_2D, NKEY_2E, NKEY_2F,
    NKEY_30, NKEY_31, NKEY_32, NKEY_33, NKEY_34, NKEY_35, NKEY_36, NKEY_37, NKEY_38, NKEY_39, NKEY_3A, NKEY_3B, NKEY_3C, NKEY_3D, NKEY_3E, NKEY_3F,
    IR_04, NKEY_41, IR_18, IR_05, IR_03, IR_00, IR_01, IR_02, NKEY_48, NKEY_49, IR_20, NKEY_4B, NKEY_4C, NKEY_4D, NKEY_4E, NKEY_4F,
    NKEY_50, NKEY_51, IR_19, NKEY_53, NKEY_54, NKEY_55, NKEY_56, NKEY_57, NKEY_58, NKEY_59, IR_17, NKEY_5B, NKEY_5C, NKEY_5D, IR_14, NKEY_5F,
#endif
};


#ifdef SUPPORT_KPH_IR_FUNCTION
extern IR_CODE  ir_code; 

#ifdef IRKEY_GW_00FF_807F_21KEY

#include "IRKEY_GW_00FF_807F_21KEY.C"

#elif defined IRKEY_YS_00FF_807F_21KEY
#include "IRKEY_YS_00FF_807F_21KEY.C"

#elif defined IRKEY_YH_7F00_7F80_KEY
#include "IRKEY_YH_7F00_7F80_KEY.C"
#elif defined IRKEY_RT_00FF_MUTE_807F_KEY
#include "IRKEY_RT_00FF_MUTE_807F_KEY.C"
#elif defined IRKEY_FY_FFFD_33KEY
#include "IRKEY_FY_FFFD_33KEY.C"
#elif defined IRKEY_JY_00BF
#include "IRKEY_JY_00BF.c"
#elif defined IRKEY_JY_00FF
#include "IRKEY_JY_00FF.c"
#elif defined IRKEY_JY_00FF_MENU
#include "IRKEY_JY_00FF_MENU.c"
#elif defined IRKEY_JY_00FF_807F_MENU
#include "IRKEY_JY_00FF_807F_MENU.c"

#elif defined IRKEY_KM_00FF
#include "IRKEY_KM_00FF_KEY_TABLE.c"


#elif defined IRKEY_HL_00FF_IR102_807F_KEY
#define SYS_CODE      0x7F80
#define SYS_CODE_1    0xFF00

const u8 IRTab[] =
{
0x1e,	0x12,
0x0a,0x1b,0x1f,
0x0c,0x0d,0x0e,
0x00,0x0f,0x19,
0x09,0x07,0x1a,
0x08,0x17,0x16,0x15,
0x02,0x01,0x06,
0x03,0x04,0x05,
};

const u8 IRTab_change[] =
{ 
IR_RPT,	IR_MUTE,
IR_1,IR_2,IR_3,
IR_4,IR_5,IR_6,
IR_7,IR_8,IR_9,
IR_REC,IR_0,IR_MODE,
IR_RECPLAY,IR_U_SD,IR_AUX_MODE,IR_FM_MODE,
IR_PREV,IR_PP,IR_VOL_UP,
IR_NEXT,IR_EQ,IR_VOL_DN,
};

const u8 IRTab_1[] =
{
	0x45, 0x46, 0x47,//  POWER   mode  MUTE
	0x44, 0x40, 0x43,//  pp      prev     next
	0x07, 0x15, 0x09,//  eq      v-          v+
	0x16, 0x19, 0x0d,//  0       RPT     USD
	0x0c, 0x18, 0x5e,//  1       2             3
	0x08, 0x1c, 0x5a,// 4        5            6
	0x42, 0x52, 0x4a,//  7        8            9      

};

const u8 IRTab_change_1[] =
{ 
	IR_POWER,     IR_MODE,	 IR_MUTE,
	IR_PP,		 IR_PREV,	 IR_NEXT,
	IR_EQ,		 IR_VOL_DN,  IR_VOL_UP,
	IR_0,		 IR_RPT,     IR_U_SD,
	IR_1,		 IR_2,		 IR_3,
	IR_4,		 IR_5,		 IR_6,
	IR_7,		 IR_8,		 IR_9,

};

#else
#define SYS_CODE      0x7F80
#define SYS_CODE_1    0xFF00

const u8 IRTab[] =
{
	0x12, 0x1a, 0x1e,  //  POWER   mode  MUTE
	0x01, 0x02, 0x03,  //  pp      prev     next
	0x04, 0x05, 0x06,  //  eq      v-          v+
	0x07, 0x08, 0x09,  //  0       RPT     USD
	0x0a, 0x1b, 0x1f,  //  1       2             3
	0x0c, 0x0d, 0x0e,  // 4        5            6
	0x00, 0x0f, 0x19,  //  7        8            9     
};

const u8 IRTab_change[] =
{ 
	IR_POWER,    IR_MODE,	 IR_MUTE,
	IR_PP,		 IR_PREV,	 IR_NEXT,
	IR_EQ,		 IR_VOL_DN,  IR_VOL_UP,
	IR_0,		 IR_RPT,     IR_U_SD,
	IR_1,		 IR_2,		 IR_3,
	IR_4,		 IR_5,		 IR_6,
	IR_7,		 IR_8,		 IR_9,

};

const u8 IRTab_1[] =
{
	0x45, 0x46, 0x47,//  POWER   mode  MUTE
	0x44, 0x40, 0x43,//  pp      prev     next
	0x07, 0x15, 0x09,//  eq      v-          v+
	0x16, 0x19, 0x0d,//  0       RPT     USD
	0x0c, 0x18, 0x5e,//  1       2             3
	0x08, 0x1c, 0x5a,// 4        5            6
	0x42, 0x52, 0x4a,//  7        8            9      

};

const u8 IRTab_change_1[] =
{ 
	IR_POWER,     IR_MODE,	 IR_MUTE,
	IR_PP,		 IR_PREV,	 IR_NEXT,
	IR_EQ,		 IR_VOL_DN,  IR_VOL_UP,
	IR_0,		 IR_RPT,     IR_U_SD,
	IR_1,		 IR_2,		 IR_3,
	IR_4,		 IR_5,		 IR_6,
	IR_7,		 IR_8,		 IR_9,

};

#endif
#endif


/*----------------------------------------------------------------------------*/
/**@brief   获取ir按键值
   @param   void
   @param   void
   @return  void
   @note    void get_irkey_value(void)
*/
/*----------------------------------------------------------------------------*/
u8 ir_get_key_value(void)
{
    u8 tkey = 0xff;
    tkey = get_irflt_value();
    if (tkey == 0xff) 
	{
        return tkey;
    }
	
#ifdef SUPPORT_KPH_IR_FUNCTION

    if(ir_code.wUserCode == SYS_CODE)
   {
	    for(u8 i=0;i<IR_KEY_MAX;i++)
	    {
	        if(tkey == IRTab[i])
			{
	            tkey = IRTab_change[i];
	            break;
	        }
	       
	    }
    }
#ifdef SYS_CODE_1	
   else if(ir_code.wUserCode == SYS_CODE_1)
   {
	   for(u8 i=0;i<IR_KEY_MAX;i++)
	    {
	        if(tkey == IRTab_1[i])
			{
	            tkey = IRTab_change_1[i];
	            break;
	        }
	       
	    }
    }
#endif   
    else
    {
      tkey = NO_KEY;
    }
#else

    tkey = IRTabFF00[tkey];
#endif
	
    return tkey;
}


/*----------------------------------------------------------------------------*/
/**@brief   ir按键初始化
   @param   void
   @param   void
   @return  void
   @note    void ir_key_init(void)
*/
/*----------------------------------------------------------------------------*/
int irkey_init(const struct irkey_platform_data *irkey_data)
{
    printf("irkey_init ");

    ir_input_io_sel(irkey_data->port);

    ir_output_timer_sel();

    irflt_config();

    ir_timeout_set();

    return 0;
}

#endif

