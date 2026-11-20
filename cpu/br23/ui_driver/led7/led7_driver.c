#include "includes.h"
#include "app_config.h"
#include "system/device/vm.h"

#include "ui/ui_api.h"

#pragma const_seg(".LED_const")
#pragma code_seg(".LED_code")

#include "led7_driver_cfg.h"

#if TCFG_UI_LED7_ENABLE

#ifdef SUPPORT_KPH_DEF
u16 led7_flahs_cnt=0;
bool led7_half_second=0;
static u16 charge_display_cnt=0;
#include "btstack/avctp_user.h"
#include "user_cfg.h"
#include "app_task.h"
#endif

#define LED7_DEBUG_ENABLE
#ifdef LED7_DEBUG_ENABLE
#define led7_debug(fmt, ...) 	printf("[LED7] "fmt, ##__VA_ARGS__)
#else
#define led7_debug(...)
#endif

#define led7_error(fmt, ...) 	printf("[LED7 ERR] "fmt, ##__VA_ARGS__)

//#define  UI_LED7_TRUE_TABLE1
//#define  UI_LED7_TRUE_TABLE2
#define  UI_LED7_TRUE_TABLE3

struct ui_led7_env {
    u8 init;
    LED7_VAR led7_var;
    const struct led7_platform_data *user_data;
    u32 flash_time;
    u8 lock: 1;
};

static struct ui_led7_env _led7_env = {0};

#define __this 		(&_led7_env)

//数字'0' ~ '9'显示段码表
static const  u8 LED7_NUMBER_2_SEG[10] = {
    (u8)(LED_A | LED_B | LED_C | LED_D | LED_E | LED_F), 		 //'0'
    (u8)(LED_B | LED_C), 										 //'1'
    (u8)(LED_A | LED_B | LED_D | LED_E | LED_G), 				 //'2'
    (u8)(LED_A | LED_B | LED_C | LED_D | LED_G),  				 //'3'
    (u8)(LED_B | LED_C | LED_F | LED_G),						 //'4'
    (u8)(LED_A | LED_C | LED_D | LED_F | LED_G), 				 //'5'
    (u8)(LED_A | LED_C | LED_D | LED_E | LED_F | LED_G), 		 //'6'
    (u8)(LED_A | LED_B | LED_C), 								 //'7'
    (u8)(LED_A | LED_B | LED_C | LED_D | LED_E | LED_F | LED_G), //'8'
    (u8)(LED_A | LED_B | LED_C | LED_D | LED_F | LED_G), 		 //'9'
};

//字母'A' ~ 'Z'显示段码表
static const  u8 LED7_LARGE_LETTER_2_SEG[26] = {
    0x77, 0x7c, 0x39, 0x3f, 0x79, ///<ABCDE
    0x71, 0x40, 0x76, 0x06, 0x0e, ///<FGHIJ
    0x40, 0x38, 0x40, 0x37, 0x3f, ///<KLMNO
    0x73, 0x67, 0x31, 0x6d, 0x78, ///<PQRST
    0x3e, 0x3e, 0x40, 0x76, 0x40, ///<UVWXY
    0x5b ///<Z
};

//字母'a' ~ 'z'显示段码表
static const  u8 LED7_SMALL_LETTER_2_SEG[26] = {
    0x77, 0x7c, 0x58, 0x5e, 0x79, ///<abcde
    0x71, 0x40, 0x76, 0x06, 0x0e, ///<fghij
    0x40, 0x38, 0x40, 0x54, 0x5c, ///<klmno
    0x73, 0x67, ///<pq
    #ifdef support_led7_show_r0
    0x50, ///<r
    #else
    0x70, ///<r
    #endif
    0x6d, 0x78, ///<st
    0x1c, 0x3e, 0x40, 0x40, 0x40, ///<uvwxy
    0x5b ///<z
};


#if defined LED_ID_SdUsbFmAux_BathBatl_88n48

/*
TF USB		BATH
FM AUX 88:8.8 BATL
*/


const u8 led_7[LED_8NUMS][7] =
{//  A     B     C     D      E     F     G
  {0x65,0x64,0x36,0x26,0x63,0x56,0x46},
  {0x54,0x53,0x25,0x51,0x52,0x45,0x35},
  {0x23,0x42,0x32,0x16,0x14,0x34,0x24},
  {0x01,0x10,0x21,0x13,0x31,0x12,0x20},
  
 // {0x00,0x00,0x00,0x00,0x00,0x00,0x00},
};
const u8 led_7icon[LED_ICON_NUMS] =
{
   //0x61,0x15,0x04,0x41,0x62,0x40,0x43,0x02,

   0x61,0x15,0x41,0x62,0x04,0x40,0x43, 0x02,
};

#elif defined LED_XY8080_BTSDUSBRECRECPLAYBAT3

const u8 led_7[LED_8NUMS][7] =
{//  A     B     C     D      E     F     G
	{0x20,0x30,0x13,0x14,0x40,0x11,0x12},
	{0x31,0x41,0x24,0x61,0x51,0x22,0x23},
	{0x44,0x52,0x53,0x15,0x35,0x33,0x34},
	{0x66,0x75,0x64,0x45,0x63,0x55,0x74},
};
const u8 led_7icon[LED_ICON_NUMS] =
{
	0x60,0x25,0x62,0x50,0x36,0x70,0x71,0x73,0x42,0x56,
};


#elif defined LED_ID_BtMp3AuxRecRptBat5
const u8 led_7[LED_8NUMS][7] =
{//  A     B     C     D      E     F     G
  {0x01,0x02,0x30,0x40,0x03,0x10,0x20},
  {0x12,0x13,0x41,0x15,0x14,0x21,0x31},
  {0x43,0x24,0x34,0x50,0x52,0x32,0x42},
  {0x65,0x56,0x45,0x53,0x35,0x54,0x46},
  
 // {0x00,0x00,0x00,0x00,0x00,0x00,0x00},
};
const u8 led_7icon[LED_ICON_NUMS] =
{
//bt,usb,aux,rec,2point,dot,earphone,pause,bat1,2,3,4,5,6
   0x05,0x25,0x51,	0x04,0x23,0x64,	0x26,0x36,0x60,	0x61,0x62,0x63,	0x06,0x16,
};

#elif defined LED_ID_PlayUsbPauseSd_FmMp3

const u8 led_7[LED_8NUMS][7] =
{//  A     B     C     D      E     F     G
  {0x65,0x64,0x36,0x26,0x63,0x56,0x46},
  {0x54,0x53,0x25,0x51,0x52,0x45,0x35},
  {0x23,0x42,0x32,0x16,0x14,0x34,0x24},
  {0x01,0x10,0x21,0x13,0x31,0x12,0x20},
  
 // {0x00,0x00,0x00,0x00,0x00,0x00,0x00},
};
const u8 led_7icon[LED_ICON_NUMS] =
{
   0x61,0x15,0x04,0x41,0x62,0x40,0x43,0x02,
};


#elif defined LED_ID_SdUsbFmAux_BathBatl

const u8 led_7[LED_8NUMS][7] =
{//  A     B     C     D      E     F     G
  {0x65,0x64,0x36,0x26,0x63,0x56,0x46},
  {0x54,0x53,0x25,0x51,0x52,0x45,0x35},
  {0x23,0x42,0x32,0x16,0x14,0x34,0x24},
  {0x01,0x10,0x21,0x13,0x31,0x12,0x20},
  
 // {0x00,0x00,0x00,0x00,0x00,0x00,0x00},
};
const u8 led_7icon[LED_ICON_NUMS] =
{
   0x61,0x15,0x04,0x41,0x62,0x40,0x43,0x02,
};


#elif defined LED_ID_PlayUsbPauseSdFmBat3
const u8 led_7[LED_8NUMS][7] =
{//  A     B     C     D      E     F     G
  {0x01,0x02,0x30,0x40,0x03,0x10,0x20},
  {0x12,0x13,0x41,0x15,0x14,0x21,0x31},
  {0x43,0x24,0x34,0x50,0x52,0x32,0x42},
  {0x65,0x56,0x45,0x53,0x35,0x54,0x46},
  
 // {0x00,0x00,0x00,0x00,0x00,0x00,0x00},
};
const u8 led_7icon[LED_ICON_NUMS] =
{
   0x05,0x51,0x25,0x04,0x62,0x06,0x16,0x36,0x23,0x64,
};


#elif defined LED_ID_BtSdUsbRecBat3Rpt_88N45

/*BT   USB  8 8:8.8 BAT 1 2 3 */
/*SD   REC		   RPT	 */

const u8 led_7[LED_8NUMS][7] =
{//  A     B     C     D      E     F     G
  {0x01,0x02,0x30,0x40,0x03,0x10,0x20},
  {0x12,0x13,0x41,0x15,0x14,0x21,0x31},
  {0x43,0x24,0x34,0x50,0x52,0x32,0x42},
  {0x65,0x56,0x45,0x53,0x35,0x54,0x46},
};
const u8 led_7icon[LED_ICON_NUMS] =
{
//LED_BT,LED_SD,LED_USB,  LED_REC,LED7_REPEAT,LED_BAT1,LED_BAT2,LED_BAT3, LED_2POINT,  LED_DOT
   0x60,0x04,0x51,0x61,0x63,0x06,0x16,0x36,0x23,0x64,
};

#elif defined LED_ID_BtSdUsbRecRecplayBat3_4018 // JW4018

const u8 led_7[LED_8NUMS][7] =
{//  A     B     C     D      E     F     G
  {0x01,0x02,0x30,0x40,0x03,0x10,0x20},
  {0x12,0x13,0x41,0x15,0x14,0x21,0x31},
  {0x43,0x24,0x34,0x50,0x52,0x32,0x42},
  {0x65,0x56,0x45,0x53,0x35,0x54,0x46},
};
const u8 led_7icon[LED_ICON_NUMS] =
{
// LED7_BT,LED7_SD,LED7_usb,LED7_REC,LED7_RECPLAY,LED7_BAT1,LED7_BAT2,LED7_BAT2,LED7_2POINT,LED7_DOT
   0x05,0x51,0x25,0x04,0x62,0x06,0x16,0x36,0x23,0x64,
};
#elif defined LED_ID_BtSdUsbRecRecplayBat3 // JW4018
/*
88:8.8
BT SD  USB REC RECPLAY BAT123 

*/

const u8 led_7[LED_8NUMS][7] =
{//  A     B     C     D      E     F     G
  {0x01,0x02,0x30,0x40,0x03,0x10,0x20},
  {0x12,0x13,0x41,0x15,0x14,0x21,0x31},
  {0x43,0x24,0x34,0x50,0x52,0x32,0x42},
  {0x65,0x56,0x45,0x53,0x35,0x54,0x46},
};
const u8 led_7icon[LED_ICON_NUMS] =
{
    0x05,0x25,0x51,0x04,0x23,0x62,0x26
};
#elif defined LED_ID_UsbSdFmAuxBat3 // JW4018
/*
88:8.8
BT SD  USB REC RECPLAY BAT123 

*/

const u8 led_7[LED_8NUMS][7] =
{//  A     B     C     D      E     F     G
  {0x65,0x64,0x36,0x26,0x63,0x56,0x46},
  {0x54,0x53,0x25,0x51,0x52,0x45,0x35},
  {0x23,0x42,0x32,0x16,0x14,0x34,0x24},
  {0x01,0x10,0x21,0x13,0x31,0x12,0x20},
};
const u8 led_7icon[LED_ICON_NUMS] =
{
    0x61,0x41,0x15,0x62,0x40,0x43,0x02
};
#elif defined LED_ID_FMUSBTFBATHBATLMP3_XL1198BR 
/*
FM			BATH
USB 88:88	BATL
TF			MP3
// FM USB TF BATH BATL MP3 :  .

*/

const u8 led_7[LED_8NUMS][7] =
{//  A     B     C     D      E     F     G
  {0x01,0x02,0x30,0x40,0x03,0x10,0x20},
  {0x12,0x13,0x41,0x15,0x14,0x21,0x31},
  {0x43,0x24,0x34,0x50,0x52,0x32,0x42},
  {0x65,0x56,0x45,0x53,0x35,0x54,0x46},
};
const u8 led_7icon[LED_ICON_NUMS] =
{
// FM USB TF BATH BATL MP3 :  .
   0x05,0x51,0x04,0x25,0x62,0x26,0x23,0x64,
};

#elif defined LED_ID_JW_5227_1B // JW52277_1B
/*
BAT123 FM SD USB REC RECPLAY BT
88:8.8
*/
const u8 led_7[LED_8NUMS][7] =
{//  A     B     C     D      E     F     G
  {0x01,0x02,0x30,0x40,0x03,0x10,0x20},
  {0x12,0x13,0x41,0x15,0x14,0x21,0x31},
  {0x43,0x24,0x34,0x50,0x52,0x32,0x42},
  {0x65,0x56,0x45,0x53,0x35,0x54,0x46},
};
const u8 led_7icon[LED_ICON_NUMS] =
{
// LED7_BAT1,LED7_BAT2,LED7_BAT2,LED_FM,LED_SD,LED_USB,LED_REC,LED_RECPLAY,LED_BT,LED7_2POINT,LED7_DOT
   0x06,0x16,0x36,0x26,0x51,0x25,0x04,0x62,0x05,0X23,0x64,
};

#elif defined LED_ID_JW_18407_1B // 

/*
88:8.8 bat1234
bt fm aux rec usb tf
*/

const u8 led_7[LED_8NUMS][7] =
{//  A     B     C     D      E     F     G
  {0x01,0x02,0x30,0x40,0x03,0x10,0x20},
  {0x12,0x13,0x41,0x15,0x14,0x21,0x31},
  {0x43,0x24,0x34,0x50,0x52,0x32,0x42},
  {0x65,0x56,0x45,0x53,0x35,0x54,0x46},
};
const u8 led_7icon[LED_ICON_NUMS] =
{
// LED7_BT,LED7_FM,LED7_AUX,LED_REC,LED_USB,LED_TF,LED7_2POINT,LED7_DOT,LED7_BAT_LL,LED7_BAT_RR,LED7_BAT01,LED7_BAT02,LED7_BAT03,LED7_BAT04
   0x05,0x51,0x25,0x04,0x62,0x26,0x23,0x64,0x60,0X62,0x06,0x16,0x36,0x63,
};



#elif defined LED_ID_SD_2776YRG_15 

/*
	=		=
	= 18.8	=
	=		=
	=		=
	usb tf fm aux
*/

const u8 led_7[LED_8NUMS][7] =
{//  A     B     C     D      E     F     G
  {0x26,0x02,0x30,0x26,0x26,0x26,0x26}, // 0x26 AS NULL 
  {0x12,0x13,0x41,0x15,0x14,0x21,0x31},
  {0x43,0x24,0x34,0x50,0x52,0x32,0x42},
  {0x63,0x36,0x06,0x53,0x35,0x60,0x16},
};
const u8 led_7icon[LED_ICON_NUMS] =
{
// LED7_USB,LED7_SD,LED7_FM,LED7_AUX,LED7_DOT
//   0x40,0x25,0x04,0x54,0x64,
   0x40,0x25,0x04,0x54,0x46,
};

const u8 led_7_EQ[LED_EQ_NUMS] =
{
// 0_1->4  
// 1_1->4 
0x03,0x10,0x20,0x01, 
0x45,0x05,0x51,0x23,
};

#elif defined LED_ID_6PIN_JW3096

const u8 led_7[LED_8NUMS][7] =
{//  A     B     C     D      E     F     G
  {0x54,0x53,0x25,0x15,0x52,0x45,0x35},
  {0x43,0x42,0x14,0x40,0x41,0x34,0x24},
  {0x12,0x31,0x21,0x05,0x03,0x23,0x13},
  {0x04,0x50,0x10,0x02,0x20,0x01,0x51},
  
 // {0x00,0x00,0x00,0x00,0x00,0x00,0x00},
};
const u8 led_7icon[LED_ICON_NUMS] =
{
   //0x61,0x15,0x04,0x41,0x62,0x40,0x43,0x02,

   0x32,0x30,
};


#endif




/*----------------------------------------------------------------------------*/
/**@brief  显示图标类
   @param   void
   @return  void
   @author  Change.tsai
   @note    void ui_led7_show_icon(UI_LED7_ICON icon)
*/
/*----------------------------------------------------------------------------*/
void led7_show_icon(UI_LED7_ICON icon)
{
    __this->led7_var.bShowIcon |= icon;
    __this->led7_var.bFlashIcon &= (~icon); //stop display
}

/*----------------------------------------------------------------------------*/
/**@brief  显示图标类
   @param   void
   @return  void
   @author  Change.tsai
   @note    void ui_led7_flash_icon(UI_LED7_ICON icon)
*/
/*----------------------------------------------------------------------------*/

#ifdef SUPPORT_KPH_DEF
void led7_flash_icon(UI_LED7_ICON icon)
{
    __this->led7_var.bFlashIcon |= icon;
//    __this->led7_var.bShowIcon &= (~icon); //stop display
}
void led7_flash_icon_off(UI_LED7_ICON icon)
{
    __this->led7_var.bFlashIcon &= ~icon;
}
void led7_show_icon_off(UI_LED7_ICON icon)
{
    __this->led7_var.bShowIcon &= ~icon;
}
void led7_flash_char(UI_LED7_ICON led7_char)
{
    __this->led7_var.bFlashChar = led7_char;
}

#else
void led7_flash_icon(UI_LED7_ICON icon)
{
    __this->led7_var.bFlashIcon |= icon;
   __this->led7_var.bShowIcon &= (~icon); //stop display
}
#endif
/*----------------------------------------------------------------------------*/
/**@brief   led7_drv 状态位缓存清除函数
   @param   void
   @return  void
   @author  Change.tsai
   @note    void led7_clear_icon(void)
*/
/*----------------------------------------------------------------------------*/
void led7_clear_icon(UI_LED7_ICON icon)
{
    __this->led7_var.bShowIcon &= (~icon);
    __this->led7_var.bFlashIcon &= (~icon);
}


/*----------------------------------------------------------------------------*/
/**@brief   led7_drv 清楚所有图标显示
   @param   void
   @return  void
   @author  Change.tsai
   @note    void led7_clear_icon(void)
*/
/*----------------------------------------------------------------------------*/
void led7_clear_all_icon(void)
{
    __this->led7_var.bFlashIcon = 0;
    __this->led7_var.bShowIcon = 0;
    __this->flash_time = jiffies_to_msecs(jiffies);
}


/*----------------------------------------------------------------------------*/
/**@brief   led7_drv 显示坐标设置
   @param   x：显示横坐标
   @return  void
   @author  Change.tsai
   @note    void led7_setX(u8 X)
*/
/*----------------------------------------------------------------------------*/
void led7_setX(u8 X)
{
    __this->led7_var.bCoordinateX = X;
}

/*----------------------------------------------------------------------------*/
/**@brief   led7_drv 清除显示数字和字母
   @param 	void
   @return  void
   @author  Change.tsai
   @note    void led7_clear_string(void)
*/
/*----------------------------------------------------------------------------*/
void led7_clear_string(void)
{
    memset(__this->led7_var.bShowBuff, 0x00, 4);
    __this->led7_var.bFlashChar = 0;
    led7_setX(0);
}


/*----------------------------------------------------------------------------*/
/**@brief   LED 清屏函数
   @param   void
   @return  void
   @author  Change.tsai
   @note    void led7_show_null(void)
*/
/*----------------------------------------------------------------------------*/
void led7_show_null(void)
{
    led7_clear_all_icon();
    led7_clear_string();
}


/*----------------------------------------------------------------------------*/
/**@brief   led7_drv 单个字符显示函数
   @param   chardata：显示字符
   @return  void
   @author  Change.tsai
   @note    void led7_show_char(u8 chardata)
   @ display:
	   ___    ___    ___    ___
	  |___|  |___|  |___|  |___|
	  |___|  |___|  |___|  |___|
	 ---0------1------2------3------> X
*/
/*----------------------------------------------------------------------------*/
void led7_show_char(u8 chardata)
{
    if (__this->led7_var.bCoordinateX >= 4) 
    {
        __this->led7_var.bCoordinateX = 0; 
        //return ;
    }
    if ((chardata >= '0') && (chardata <= '9')) 
    {
        __this->led7_var.bShowBuff[__this->led7_var.bCoordinateX++] = LED7_NUMBER_2_SEG[chardata - '0'];
    } 
    else if ((chardata >= 'a') && (chardata <= 'z')) 
    {
        __this->led7_var.bShowBuff[__this->led7_var.bCoordinateX++] = LED7_SMALL_LETTER_2_SEG[chardata - 'a'];
    } 
    else if ((chardata >= 'A') && (chardata <= 'Z')) 
    {
        __this->led7_var.bShowBuff[__this->led7_var.bCoordinateX++] = LED7_LARGE_LETTER_2_SEG[chardata - 'A'];
    } 
    else if (chardata == ':') 
    {
        __this->led7_var.bShowIcon |= LED7_2POINT;
        __this->led7_var.bFlashIcon &= (~LED7_2POINT);
    } 
    else if (chardata == '.') 
    {
        __this->led7_var.bShowIcon |= LED7_DOT;
        __this->led7_var.bFlashIcon &= (~LED7_DOT);
    } 
    else if (chardata == ' ') 
    {
        __this->led7_var.bShowBuff[__this->led7_var.bCoordinateX++] = 0;
    } 
    else 
    {
        __this->led7_var.bShowBuff[__this->led7_var.bCoordinateX++] = LED_G; //显示'-'
    }
}


/*----------------------------------------------------------------------------*/
/**@brief   led7_drv 某一位字符闪烁
   @param   index
   @return  void
   @author  Change.tsai
   @note    void led7_flash_char_start(u8 index)
*/
/*----------------------------------------------------------------------------*/
void led7_flash_char_start(u8 index)
{
    if (index < 4) 
    {
        __this->led7_var.bFlashChar |= BIT(index);
    }
}

/*----------------------------------------------------------------------------*/
/**@brief   led7_drv 某一位取消字符闪烁
   @param   index
   @return  void
   @author  Change.tsai
   @note    void led7_flash_char_stop(u8 index)
*/
/*----------------------------------------------------------------------------*/
void led7_flash_char_stop(u8 index)
{
    if (index < 4) 
    {
        __this->led7_var.bFlashChar &= ~BIT(index);
    }
}


/*----------------------------------------------------------------------------*/
/**@brief   led7_drv 字符串显示函数, 默认左对齐, 从x = 0开始显示
   @param   *str：字符串的指针   offset：显示偏移量
   @return  void
   @author  Change.tsai
   @note    void led7_show_string_left(u8 *str)
*/
/*----------------------------------------------------------------------------*/
void led7_show_string_reset_x(u8 *str)
{
    led7_clear_string();
    while (*str != '\0') {
        led7_show_char(*str++);
    }
}

/*----------------------------------------------------------------------------*/
/**@brief   led7_drv 字符串显示函数, 默认左对齐, 追加形式
   @param   *str：字符串的指针   offset：显示偏移量
   @return  void
   @author  Change.tsai
   @note    void led7_show_string_left(u8 *str)
*/
/*----------------------------------------------------------------------------*/
void led7_show_string(u8 *str)
{
    while (*str != '\0') 
    {
        led7_show_char(*str++);
    }
}

/*----------------------------------------------------------------------------*/
/**@brief   led7_drv 字符串显示函数, 左对齐, 清屏
   @param   *str：字符串的指针
   @return  void
   @author  Change.tsai
   @note    void led7_show_string_left(u8 *str)
*/
/*----------------------------------------------------------------------------*/
void led7_show_string_align_left(u8 *str)
{
    led7_show_string_reset_x(str);
}


/*----------------------------------------------------------------------------*/
/**@brief   led7_drv 字符串显示函数, 右对齐, 清屏
   @param   *str：字符串的指针
   @return  void
   @author  Change.tsai
   @note    void led7_show_string_left(u8 *str)
*/
/*----------------------------------------------------------------------------*/
void led7_show_string_align_right(u8 *str)
{
    u8 cnt = 0;
    u8 *_str = str;
    while (*_str++ != '\0') 
    {
        cnt++;
    }
    if (cnt > 4) 
    {
        led7_error("show string oversize", __func__);
        return;
    }
    led7_clear_string();
    led7_setX(4 - cnt);
    while (*str != '\0') 
    {
        led7_show_char(*str++);
    }
}


/*----------------------------------------------------------------------------*/
/**@brief   数字显示函数, 默认高位显示0
   @param   void
   @return  void
   @author  Change.tsai
   @note    void led7_show_number(u8 val)
*/
/*----------------------------------------------------------------------------*/
void led7_show_number(u16 val)
{
    u8 tmp_buf[5] = {0};

    itoa4(val, tmp_buf);
    led7_show_string_reset_x(tmp_buf);
}



/*----------------------------------------------------------------------------*/
/**@brief   数字显示函数, 高位不显示0
   @param   val, 显示数字
   @return  void
   @author  Change.tsai
   @note    void led7_show_number2(u8 val)
*/
/*----------------------------------------------------------------------------*/
void led7_show_number2(u16 val)
{
    u8 i;
    u8 tmp_buf[5] = {0};

    itoa4(val, tmp_buf);
    for (i = 0; i < 3; i++) 
    {
        if (tmp_buf[i] != '0') 
        {
            break;
        }
    }
    led7_show_string_align_right((u8 *) & (tmp_buf[i]));
}

/*----------------------------------------------------------------------------*/
/**@brief   数字显示函数(追加方式)
   @param   val, 显示数字
   @return  void
   @author  Change.tsai
   @note    void led7_show_number_add(u8 val)
*/
/*----------------------------------------------------------------------------*/
void led7_show_number_add(u16 val)
{
    u8 i;
    u8 tmp_buf[5] = {0};

    itoa4(val, tmp_buf);
    for (i = 0; i < 3; i++) {
        if (tmp_buf[i] != '0') {
            break;
        }
    }
    led7_show_string((u8 *)&tmp_buf[i]);
}


static u8 __ui_led7_get_sys_halfsec(void)
{
    if (((jiffies_to_msecs(jiffies) - __this->flash_time) & 999) > 500) {
        return true;
    } else {
        return false;
    }
}
#ifdef SUPPORT_KPH_DEF



#if defined SUPPORT_LED7_BAT_SHOW
void ledbat_notcharge_show(void)
{
   //led7_show_icon(LED7_BAT_H|LED7_BAT_L);
	//return;
   // if (app_get_curr_task() == APP_IDLE_TASK) 
   {
      // led7_show_icon_off(LED7_BAT_L);
      // led7_show_icon_off(LED7_BAT_H);
   }
  // else

	 #ifdef BAT_LOW_POWER_STANBY_DISABLE
	 	if(led_bat.bBatLowPowerFlag)
	 	{
	               led7_show_icon_off(LED7_BAT01|LED7_BAT02|LED7_BAT03);			 	
             		 led7_show_icon(LED7_BAT_H|LED7_BAT_L);				   
			return;
		}
	 #endif	 
  
   {
	//printf("led_bat.power=%d\n",led_bat.power);
   
      led7_flash_icon_off(LED7_BAT_3);
       led7_flash_icon_off(LED7_BAT03);
       switch(led_bat.power)
       {
            case POWER_FULL:
		   	
#ifdef LED_ID_BtMp3AuxRecRptBat5	
							  led7_show_icon(LED7_BAT_H|LED7_BAT01|LED7_BAT02|LED7_BAT03|LED7_BAT04|LED7_BAT05|LED7_BAT06);
								 led7_show_icon_off(LED7_BAT_L);
#else		   	
               led7_show_icon(LED7_BAT_H|LED7_BAT01|LED7_BAT02|LED7_BAT03);
               led7_show_icon_off(LED7_BAT_L);
#endif			   
               break;
            case POWER_H:
               led7_show_icon(LED7_BAT_H);
               led7_show_icon_off(LED7_BAT_L);
               break;
            case POWER_L:
               led7_show_icon(LED7_BAT_L);
               led7_show_icon_off(LED7_BAT_H);
               break;
  #ifdef LED_ID_BtMp3AuxRecRptBat5	  
					 case POWER_4:
				  led7_show_icon(LED7_BAT_H|LED7_BAT01|LED7_BAT02|LED7_BAT03|LED7_BAT04|LED7_BAT05|LED7_BAT06);
					 led7_show_icon_off(LED7_BAT_L);
			   break;		 
				  case POWER_3:
			  led7_show_icon(LED7_BAT_H|LED7_BAT02|LED7_BAT03|LED7_BAT04|LED7_BAT05|LED7_BAT06);
				led7_show_icon_off(LED7_BAT01);
			   break;
				  case POWER_2:
			  led7_show_icon(LED7_BAT_H|LED7_BAT03|LED7_BAT04|LED7_BAT05|LED7_BAT06);
			  led7_show_icon_off(LED7_BAT01|LED7_BAT02);
					 break;
				  case POWER_1:
			  led7_flash_icon_off(LED7_BAT04|LED7_BAT05|LED7_BAT06);
			  led7_show_icon(LED7_BAT04|LED7_BAT05|LED7_BAT06);
			  led7_show_icon_off(LED7_BAT01|LED7_BAT02|LED7_BAT03);
						 break;   
				  case POWER_LOW:
			led7_show_icon(LED7_BAT04|LED7_BAT05|LED7_BAT06);
			  led7_show_icon_off(LED7_BAT01|LED7_BAT02|LED7_BAT03);
			   led7_flash_icon(LED7_BAT04|LED7_BAT05|LED7_BAT06);
					 break;
				  case NO_POWER:		 
			  led7_show_icon(LED7_BAT04|LED7_BAT05|LED7_BAT06);
			  led7_show_icon_off(LED7_BAT01|LED7_BAT02|LED7_BAT03);
			   led7_flash_icon(LED7_BAT04|LED7_BAT05|LED7_BAT06);
					 break;
  #else
            case POWER_3:
               led7_show_icon(LED7_BAT03|LED7_BAT02|LED7_BAT01|LED7_BAT_H);
               break;
            case POWER_2:
               led7_show_icon(LED7_BAT02|LED7_BAT03|LED7_BAT_H);
               led7_show_icon_off(LED7_BAT01);
               break;
            case POWER_1:
               led7_show_icon(LED7_BAT03|LED7_BAT_L);
               led7_show_icon_off(LED7_BAT01|LED7_BAT02|LED7_BAT_H);
               break;
            
            case POWER_LOW:
               led7_show_icon(LED7_BAT03|LED7_BAT_L);
               led7_show_icon_off(LED7_BAT01|LED7_BAT02|LED7_BAT_H);
               led7_flash_icon(LED7_BAT03|LED7_BAT_L);
               break;
            case NO_POWER:
               led7_show_icon_off(LED7_BAT01|LED7_BAT02|LED7_BAT03|LED7_BAT_L|LED7_BAT_H);
               break;
#endif			   
	   }
   }
}

void ledbat_incharge_show(void)
{

    #ifdef SUPPORT_BAT_IN_CHARGE_DELAY_SHOW_FULL
    u8 static ledbatcnt=0;
    #endif
    //__this->led7_var.bShowIcon |= LED7_BATTERY_L|LED7_BATTERY_R|LED7_BATTERY_T|LED7_BATTERY_B;
   
    if(led_bat.power == POWER_FULL)
    {    

        #if LED7_BAT_H != 0
        led7_show_icon(LED7_BAT_H);
        #else
        led7_show_icon(LED7_BAT01|LED7_BAT02|LED7_BAT03|LED7_BAT04);
#ifdef LED_ID_BtMp3AuxRecRptBat5
				led7_show_icon(LED7_BAT05);
				led7_show_icon(LED7_BAT06);
#endif
        #endif
        
        #ifdef SUPPORT_InCharge_BAT_FULL_SLOW_FLASH
	        charge_display_cnt++;
	        if(charge_display_cnt<700)__this->led7_var.bShowIcon &= ~LED_BAT_H;
	        if(charge_display_cnt>2100)charge_display_cnt=0;
        #endif
        //puts("f");
    }
    else
    {
    //puts("n");
        /*charge_dis_cnt++;
        if(charge_dis_cnt>250)  __this->led7_var.bShowIcon |= (LED_BAT03);
        if(charge_dis_cnt>500)  __this->led7_var.bShowIcon |= (LED_BAT02);
        if(charge_dis_cnt>750)  __this->led7_var.bShowIcon |= (LED_BAT01);
        if(charge_dis_cnt>1000)  charge_dis_cnt = 0;*/
        #if LED7_BAT_H != 0
        //charge_display_cnt++;
	        led7_show_icon_off(LED7_BAT_H|LED7_BAT_L);
	        if(charge_display_cnt<250) { led7_show_icon(LED7_BAT_H);led7_show_icon_off(LED7_BAT_L);}
	        else if(charge_display_cnt<500) {  led7_show_icon(LED7_BAT_L);led7_show_icon_off(LED7_BAT_H);}
	        if(charge_display_cnt>500)  charge_display_cnt = 0;
        #else
		
#ifdef LED_ID_BtMp3AuxRecRptBat5
		 led7_show_icon(LED7_BAT05);
		 led7_show_icon(LED7_BAT06);
         led7_show_icon_off(LED7_BAT01|LED7_BAT02|LED7_BAT03|LED7_BAT04);
         //charge_display_cnt++;
         if(charge_display_cnt>250)  led7_show_icon(LED7_BAT04);
         if(charge_display_cnt>500)  led7_show_icon(LED7_BAT03);
         if(charge_display_cnt>750)  led7_show_icon(LED7_BAT02);
         if(charge_display_cnt>1000)  led7_show_icon(LED7_BAT01);
         if(charge_display_cnt>1250)  charge_display_cnt = 0;
         
#else
         #ifdef LED_BAT04

         led7_show_icon_off(LED7_BAT01|LED7_BAT02|LED7_BAT03|LED7_BAT04);
         //charge_display_cnt++;
         if(charge_display_cnt>250)  led7_show_icon(LED7_BAT04);
         if(charge_display_cnt>500)  led7_show_icon(LED7_BAT03);
         if(charge_display_cnt>750)  led7_show_icon(LED7_BAT02);
         if(charge_display_cnt>1000)  led7_show_icon(LED7_BAT01);
         if(charge_display_cnt>1250)  charge_display_cnt = 0;
         
         #else
    
         led7_show_icon_off(LED7_BAT01|LED7_BAT02|LED7_BAT03);
         //charge_dis_cnt++;
         if(charge_display_cnt>250)  led7_show_icon(LED7_BAT03);
         if(charge_display_cnt>500)  led7_show_icon(LED7_BAT02);
         if(charge_display_cnt>750)  led7_show_icon(LED7_BAT01);
         if(charge_display_cnt>1000)  charge_display_cnt = 0;
         
         #endif
#endif
        #endif
        
    }
}


void led7_bat_show(void)
{
    switch(led_bat.status)
    {
        case IN_CHARGE:
            ledbat_incharge_show(); 
            break;
        case NOT_CHARGE:
            ledbat_notcharge_show();
            break;
   }

}

#endif
#endif


static void __ui_led7_update_bShowbuf1(void)
{
    u8 k, i, j, temp;
    u16 temp1;
    u16 temp2;
    static bool mute_flag_last=0;
    k = 0;

    

    __this->led7_var.bShowBuff1[0] = 0;    //if pin0 output 1, other output 0/1
    __this->led7_var.bShowBuff1[1] = 0;
    __this->led7_var.bShowBuff1[2] = 0;
    __this->led7_var.bShowBuff1[3] = 0;
    __this->led7_var.bShowBuff1[4] = 0;
    __this->led7_var.bShowBuff1[5] = 0;
    __this->led7_var.bShowBuff1[6] = 0;

    #if defined SUPPORT_LED7_BAT_SHOW
	if(app_get_curr_task()!=APP_IDLE_TASK)
    {
        led7_bat_show();
	}

    #else
    led7_show_icon(LED7_BAT_H);
    #endif

    #ifdef SUPPORT_LED7_UART_TEST_IO
    temp=0xFF;
    for(j=0;j<7;j++){
        if(temp & BIT(j)){
            __this->led7_var.bShowBuff1[use_ctl.temp_8bit_led_data>>4] |= BIT(use_ctl.temp_8bit_led_data&0x0f);
        }
    }
    return;
    #endif


    if(use_ctl.mute_flag)
    {
		
	    if(app_get_curr_task()!=APP_IDLE_TASK)
        {
            led7_flash_char(0xff);
            led7_flash_icon(0xffff);
            mute_flag_last = 1;
	    }
        else
        {
			led7_show_icon_off(0xffff);
			led7_flash_icon_off(0xffff);	
		}
	   
    }
    else
    {
        if(mute_flag_last)
        {
            mute_flag_last = 0;
            led7_flash_char(0);
            led7_flash_icon_off(0xffff);
        }
    }

    if(use_ctl.input_time_out)
    {
        __this->led7_var.bShowIcon &= ~ LED7_DOT;
        __this->led7_var.bShowIcon &= ~ LED7_2POINT;
	    #ifdef LED7_LED188_SET	
	    if (use_ctl.ir_num < 999 ||use_ctl.ir_num > 1999) 
        {
	        __this->led7_var.bShowBuff[0] = ' ';
	    }
	 	else
	#endif	   
        __this->led7_var.bShowBuff[0] = LED7_NUMBER_2_SEG[use_ctl.ir_num/1000];
        __this->led7_var.bShowBuff[1] = LED7_NUMBER_2_SEG[use_ctl.ir_num/100%10];
        __this->led7_var.bShowBuff[2] = LED7_NUMBER_2_SEG[use_ctl.ir_num/10%10];
        __this->led7_var.bShowBuff[3] = LED7_NUMBER_2_SEG[use_ctl.ir_num%10];
    }

    for(i=0;i<4;i++)
    {
        temp = __this->led7_var.bShowBuff[i];
		
		if((led7_half_second) &&(__this->led7_var.bFlashChar & BIT(i)))
		   temp=0;
        for(j=0;j<7;j++)
        {
            if(temp & BIT(j))
            {
                __this->led7_var.bShowBuff1[led_7[i][j]>>4] |= BIT( led_7[i][j]&0x0f );
            }
        }
    }
   
    for(i=0;i<LED_ICON_NUMS;i++)
    {
        
       temp1 = __this->led7_var.bShowIcon;

  
        if((led7_half_second)  &&(__this->led7_var.bFlashIcon & BIT(i)))
        {
            temp1 &= ~BIT(i); 
        }
	 
        if(temp1 & BIT(i))
        {
            __this->led7_var.bShowBuff1[led_7icon[i]>>4] |= BIT( led_7icon[i]&0x0f );
        }
    }
	
}


/*----------------------------------------------------------------------------*/
/**@brief   把所有IO设置为高阻
   @param   x：显示横坐标
   @return  void
   @author  Change.tsai
   @note    void led7_clear(void)
*/
/*----------------------------------------------------------------------------*/
static void __ui_led7_port_set_hz(u8 port)
{
    gpio_set_pull_down(port, 0);
    gpio_set_pull_up(port, 0);
    gpio_set_direction(port, 1);
}

/*----------------------------------------------------------------------------*/
/**@brief   LED清屏函数, 把所有IO设置为高阻
   @param   x：显示横坐标
   @return  void
   @author  Change.tsai
   @note    void led7_clear(void)
*/
/*----------------------------------------------------------------------------*/
static void __ui_led7_clear_all(void)
{
    u8 port = 0;
    u8 i;

    if (__this->user_data->pin_type == LED7_PIN7) {
        for (i = 0; i < 7; i++) {
            port = __this->user_data->pin_cfg.pin7.pin[i];
            __ui_led7_port_set_hz(port);
        }
    }
    if (__this->user_data->pin_type == LED7_PIN12) {
        for (i = 0; i < 7; i++) {
            port = __this->user_data->pin_cfg.pin12.pin_segl[i];
            __ui_led7_port_set_hz(port);
        }
        for (i = 0; i < 5; i++) {
            port = __this->user_data->pin_cfg.pin12.pin_comh[i];
            __ui_led7_port_set_hz(port);
        }
    }

    if (__this->user_data->pin_type == LED7_PIN13) {
        for (i = 0; i < 7; i++) {
            port = __this->user_data->pin_cfg.pin13.pin_seg[i];
            __ui_led7_port_set_hz(port);
        }
        for (i = 0; i < 6; i++) {
            port = __this->user_data->pin_cfg.pin13.pin_com[i];
            __ui_led7_port_set_hz(port);
        }
    }

}

/*----------------------------------------------------------------------------*/
/**@brief   LED扫描函数
   @param   void
   @return  void
   @author  Change.tsai
   @note    void led7_scan(void *param)
*/
/*----------------------------------------------------------------------------*/
void led7_scan(void *param)
{
    static u8 cnt = 0;
    u8 seg, i;

    if (__this->init == false) 
    {
        return;
    }


    

#ifndef SUPPORT_UI_UART_XY8080
    #if 1//defined SUPPORT_LED7_BAT_SHOW
    charge_display_cnt++;
    #endif
    led7_flahs_cnt++;
    if(led7_flahs_cnt>250)
    {
      led7_flahs_cnt=0;
      led7_half_second = !led7_half_second;
    }
	__ui_led7_clear_all();
#else
 #if 1//defined SUPPORT_LED7_BAT_SHOW
    charge_display_cnt++;
#endif
    led7_flahs_cnt++;
    if(led7_flahs_cnt>250/25)
    {
      led7_flahs_cnt=0;
      led7_half_second = !led7_half_second;
    }
#endif


    if (!cnt && !__this->lock) 
    {
        __ui_led7_update_bShowbuf1();
    }

#ifdef SUPPORT_UI_UART_XY8080
	return ;
#endif

    seg = __this->led7_var.bShowBuff1[cnt];

   
    
    
    if (__this->user_data->pin_type == LED7_PIN7) 
    {
        //pin cnt output H
#ifdef led7_half_light        
//        gpio_set_hd0(__this->user_data->pin_cfg.pin7.pin[cnt], 1);
        gpio_set_pull_up(__this->user_data->pin_cfg.pin7.pin[cnt], 1);
#else
        gpio_set_hd0(__this->user_data->pin_cfg.pin7.pin[cnt], 1);
        gpio_set_hd(__this->user_data->pin_cfg.pin7.pin[cnt], 1);
#endif
        gpio_direction_output(__this->user_data->pin_cfg.pin7.pin[cnt], 1);
        for (i = 0; i < 7; i++) 
        {
            if (seg & BIT(i)) 
            {
                //pin i output L
	 #ifdef led7_half_light        
	 	gpio_set_pull_up(__this->user_data->pin_cfg.pin7.pin[i], 0);
	 #else
	 	gpio_set_hd0(__this->user_data->pin_cfg.pin7.pin[i], 0);
	 	gpio_set_hd(__this->user_data->pin_cfg.pin7.pin[i], 0);
	 #endif  
                gpio_direction_output(__this->user_data->pin_cfg.pin7.pin[i], 0);
            }
        }
        cnt = (cnt >= 6) ? 0 : cnt + 1;
    }
    
    else if (__this->user_data->pin_type == LED7_PIN13) 
    {
        //pin_comh  cnt output L
        gpio_set_hd0(__this->user_data->pin_cfg.pin13.pin_com[cnt], 1);
        gpio_set_hd(__this->user_data->pin_cfg.pin13.pin_com[cnt], 1);
        gpio_direction_output(__this->user_data->pin_cfg.pin13.pin_com[cnt], 0);
        for (i = 0; i < 7; i++) {
            if (seg & BIT(i)) {
                //pin_segl i output H
                gpio_set_hd0(__this->user_data->pin_cfg.pin13.pin_seg[i], 0);
                gpio_set_hd(__this->user_data->pin_cfg.pin13.pin_seg[i], 0);
                gpio_direction_output(__this->user_data->pin_cfg.pin13.pin_seg[i], 1);
            }
        }
        cnt = (cnt >= 5) ? 0 : cnt + 1;
    } 
    else 
    {
        //pin_comh  cnt output H
        gpio_set_hd0(__this->user_data->pin_cfg.pin12.pin_comh[cnt], 1);
        gpio_set_hd(__this->user_data->pin_cfg.pin12.pin_comh[cnt], 1);
        gpio_direction_output(__this->user_data->pin_cfg.pin12.pin_comh[cnt], 1);
        for (i = 0; i < 7; i++) {
            if (seg & BIT(i)) {
                //pin_segl i output L
                gpio_set_hd0(__this->user_data->pin_cfg.pin12.pin_segl[i], 0);
                gpio_set_hd(__this->user_data->pin_cfg.pin12.pin_segl[i], 0);
                gpio_direction_output(__this->user_data->pin_cfg.pin12.pin_segl[i], 0);
            }
        }
        cnt = (cnt >= 4) ? 0 : cnt + 1;
    }
}


static void led7_setXY(u32 x, u32 y)
{
    led7_setX(x);
}

static void  led7_Flashchar(u32 arg)
{
    __this->led7_var.bFlashChar |= arg;
}

static void  led7_Clear_Flashchar(u32 arg)
{
    __this->led7_var.bFlashChar &= (~arg);
}


static void led7_show_pic(u32 arg)
{

}

static void led7_hide_pic(u32 arg)
{

}

static void led7_show_one_number(u8 number)
{
    led7_show_char(number + '0');
}

static void led7_show_lock(u32 en)
{
    __this->lock = !!en;
}

static LCD_API LED7_HW = {
    .clear             = led7_show_null,
    .setXY             = led7_setXY,
    .FlashChar         = led7_Flashchar,
    .Clear_FlashChar   = led7_Clear_Flashchar,
    .show_icon         = led7_show_icon,
    .flash_icon        = led7_flash_icon,
    .clear_icon        = led7_clear_icon,
    .show_string       = led7_show_string,
    .show_char         = led7_show_char,
    .show_number       = led7_show_one_number,
    .show_pic          = led7_show_pic,
    .hide_pic          = led7_hide_pic,
    .lock              = led7_show_lock,
};




/*----------------------------------------------------------------------------*/
/**@brief   led7段数码管初始化
   @param   void
   @return  void
   @author  Change.tsai
   @note    void led7_init(void)
*/
/*----------------------------------------------------------------------------*/
void *led7_init(const struct led7_platform_data *_data)
{
    y_printf("==========================   \n");
    led7_debug("%s", __func__);
    memset(__this, 0x00, sizeof(struct ui_led7_env));
    if (_data == NULL) 
    {
        return NULL;
    }
    struct led7_platform_data *data = malloc(sizeof(struct led7_platform_data));
    if (!data) 
    {
        return NULL;
    }

    memcpy(data, _data, sizeof(struct led7_platform_data));
    __this->user_data = data;

    __ui_led7_clear_all();
    /* sys_hi_timer_add(NULL, led7_scan, 2); //2ms */
#ifdef SUPPORT_UI_XY8080 
	void UI_XY8080_update_send_scan(void);
	sys_timer_add(NULL, UI_XY8080_update_send_scan, 250);
#elif defined	SUPPORT_UI_UART_XY8080
    sys_timer_add(NULL, led7_scan, 50); //2ms
#elif TCFG_LED7_RUN_RAM
    void app_timer_led_scan(void (*led_scan)(void *));
    app_timer_led_scan(led7_scan);
#else
    usr_timer_add(NULL, led7_scan, 2 , 0); //2ms
#endif
    __this->init = true;
    return (void *)(&LED7_HW);
}


#ifdef SUPPORT_UI_XY8080
void UI_XY8080_update_send_scan(void)
{

	static u8 kkkkkkkkkkkkkkk=0;
	if (++kkkkkkkkkkkkkkk>2)
	{
		kkkkkkkkkkkkkkk=0;
		led7_half_second = !led7_half_second;
	}
	
	charge_display_cnt=charge_display_cnt+251;
	if (charge_display_cnt>1600)
	{
		charge_display_cnt=0;
	}
}
#endif


#ifdef SUPPORT_UI_XY8080
void update_send_leddisp_byte(void)
{
	__this->led7_var.bShowBuff1[0] = 0;
	__this->led7_var.bShowBuff1[1] = 0;
	__this->led7_var.bShowBuff1[2] = 0;
	__this->led7_var.bShowBuff1[3] = 0;
	__this->led7_var.bShowBuff1[4] = 0;
	__this->led7_var.bShowBuff1[5] = 0;
	__this->led7_var.bShowBuff1[6] = 0;
	__this->led7_var.bShowBuff1[7] = 0;
	

	xy8080_update_send_leddisp_byte();
}
#endif

#ifdef SUPPORT_UI_XY8080
void xy8080_update_send_leddisp_byte(void)
{
	u8 k, i, j, temp;
	u16 temp1;

	#ifdef XY8080_LED_DEBUG
		__this->led7_var.bShowBuff1[use_ctl.xy8080_com] = BIT(use_ctl.xy8080_seg);
		printf("XY8080_LED_DEBUG=%d=%d=\n",use_ctl.xy8080_com,use_ctl.xy8080_seg);
	#endif

	#ifdef XY8080_WORK_MODE_SET
	__this->led7_var.bShowBuff1[0] |= (XY8080_WORK_MODE_SET | BIT(7));
	#endif

	#ifdef XY8080_LED_DEBUG
		return;
	#endif
	
		__ui_led7_update_bShowbuf1();
}

void send_xy8080_code(bool bBit)
{
	static u16 cnt=0;
	cnt++;
	if(cnt<=XY8080_SEND_CODE_TIME)
	{
		TX_IO_L;
	}
	else if(cnt<=2*XY8080_SEND_CODE_TIME)
	{

		if(bBit)	//??1码就改变电平，是0码的话就继续保持低电??
		{
			TX_IO_H;
		}

	}
	else if(cnt<=3*XY8080_SEND_CODE_TIME)
	{

	}
	else if(cnt<=4*XY8080_SEND_CODE_TIME)
	{
		if(!bBit)	//0码改变电??
		{
			TX_IO_H;
		}
		if(cnt==4*XY8080_SEND_CODE_TIME)
		{
			if(use_ctl.buff_bit)
			{
				use_ctl.buff_bit--;
			}
			else
			{
				use_ctl.buff_bit=7;
				use_ctl.buff_cnt++;
			}
			cnt=0;
		}
	}
	
}

void send_xy8080_led_disp(void)
{
	if(use_ctl.send_data==0)
	{
		TX_IO_H;
		use_ctl.start_send_date_falg=0;
		return;
	}
	if(use_ctl.start_send_date_falg == 0&&use_ctl.bSendBusy==0)
	{
		for(u8 i=0;i<XY8080_SEND_BYTE;i++)
		{
		#ifdef XY8080_LED_DEBUG
			//__this->led7_var.bShowBuff2[i] =  0Xff; //全亮????
			__this->led7_var.bShowBuff2[i] = __this->led7_var.bShowBuff1[i];
		#else
			__this->led7_var.bShowBuff2[i] = __this->led7_var.bShowBuff1[i];
		#endif
		}
		__this->led7_var.bShowBuff2[0]= XY8080_WORK_MODE_SET	| BIT(7);
		use_ctl.buff_bit=7;	//字节中的第几??
		use_ctl.buff_cnt=0;	//第几个字节，也就是bShowBuff2数组??
		use_ctl.start_send_date_falg=1;
	}
		
	if(use_ctl.start_send_date_falg == 1)
	{
		if(use_ctl.buff_cnt<XY8080_SEND_BYTE)
		{
			use_ctl.bSendBusy=1;
			send_xy8080_code(__this->led7_var.bShowBuff2[use_ctl.buff_cnt]&BIT(use_ctl.buff_bit));
		}
		else
		{
			use_ctl.send_over_delay_reset++;
			TX_IO_H;
			if(use_ctl.send_over_delay_reset>XY8080_RESET_SEND_TIME)
			{
				use_ctl.send_over_delay_reset=0;
				use_ctl.bSendBusy=0;
				use_ctl.start_send_date_falg=0;
				use_ctl.send_data=0;
				use_ctl.buff_bit=7;	//字节中的第几??
				use_ctl.buff_cnt=0;	//第几个字节，也就是bShowBuff2数组??
			}
		}
	}
	
}

#endif

//=================================================================================//
//                        		For Test Module                    				   //
//=================================================================================//
#if 0 //for test code
/*
LED7_PLATFORM_DATA_BEGIN(led7_test_data)
	.pin_type = LED7_PIN7,
	.pin_cfg.pin7.pin[0] = IO_PORTB_00,
	.pin_cfg.pin7.pin[1] = IO_PORTB_01,
	.pin_cfg.pin7.pin[2] = IO_PORTB_02,
	.pin_cfg.pin7.pin[3] = IO_PORTB_03,
	.pin_cfg.pin7.pin[4] = IO_PORTB_04,
	.pin_cfg.pin7.pin[5] = IO_PORTB_05,
	.pin_cfg.pin7.pin[6] = IO_PORTB_06,
LED7_PLATFORM_DATA_END()
*/

LED7_PLATFORM_DATA_BEGIN(led7_test_data)
.pin_type = LED7_PIN12,
 .pin_cfg.pin12.pin_segl[0] = IO_PORTB_00,
                              .pin_cfg.pin12.pin_segl[1] = IO_PORTB_01,
                                      .pin_cfg.pin12.pin_segl[2] = IO_PORTB_02,
                                              .pin_cfg.pin12.pin_segl[3] = IO_PORTB_03,
                                                      .pin_cfg.pin12.pin_segl[4] = IO_PORTB_04,
                                                              .pin_cfg.pin12.pin_segl[5] = IO_PORTB_05,
                                                                      .pin_cfg.pin12.pin_segl[6] = IO_PORTB_06,

                                                                              .pin_cfg.pin12.pin_comh[0] = IO_PORTA_00,
                                                                                      .pin_cfg.pin12.pin_comh[1] = IO_PORTB_08,
                                                                                              .pin_cfg.pin12.pin_comh[2] = IO_PORTB_09,
                                                                                                      .pin_cfg.pin12.pin_comh[3] = IO_PORTB_10,
                                                                                                              .pin_cfg.pin12.pin_comh[4] = IO_PORTA_01,
                                                                                                                      LED7_PLATFORM_DATA_END()

                                                                                                                      void led7_scan_test(void)
{
    __ui_led7_scan(NULL);
}

#define LED7_RTC_DISPLAY_TEST 		1
#define LED7_ICON_DISPLAY_TEST 		1
#define LED7_ICON_FLASH_TEST 		0
#define LED7_SHOW_NUMBER_TEST 		0
#define LED7_SHOW_STRING_TEST 		0
#define LED7_SHOW_MODE_MENU_TEST 	0

void led7_test()
{
    if (__this->init == false) {
        ui_led7_init(&led7_test_data); //init
    }
#if LED7_RTC_DISPLAY_TEST
    //test1: rtc display
    static u8 Hour = 18;
    static u8 Min = 52;
    static u8 Sec = 0;

    Sec++;
    if (Sec >= 60) {
        Min++;
        Sec = 0;
    }
    if (Min >= 60) {
        Hour++;
        Min = 0;
    }
    if (Hour >= 24) {
        Hour = 0;
    }
    led7_debug("%d : %d", Hour, Min);
    ui_led7_show_RTC_time(Hour, Min);
#endif /* #if LED7_RTC_DISPLAY_TEST */

#if LED7_ICON_DISPLAY_TEST
    ui_led7_show_icon(LED7_PLAY);
    ui_led7_show_icon(LED7_PAUSE);
    ui_led7_show_icon(LED7_USB); //USB
    ui_led7_show_icon(LED7_SD); //SD
    ui_led7_show_icon(LED7_2POINT); //
    ui_led7_show_icon(LED7_FM);
    ui_led7_show_icon(LED7_DOT); //
    ui_led7_show_icon(LED7_MP3);
    ui_led7_show_icon(LED7_REPEAT); //
    ui_led7_show_icon(LED7_CHARGE); //
    ui_led7_show_icon(LED7_BT); //
    ui_led7_show_icon(LED7_AUX); //
    ui_led7_show_icon(LED7_WMA); //
#endif /* #if LED7_ICON_DISPLAY_TEST */

#if LED7_ICON_FLASH_TEST
    ui_led7_flash_icon(LED7_PLAY);
    ui_led7_flash_icon(LED7_PAUSE);
    ui_led7_flash_icon(LED7_USB);
    ui_led7_flash_icon(LED7_SD);
    ui_led7_flash_icon(LED7_2POINT);
    ui_led7_flash_icon(LED7_FM);
    ui_led7_flash_icon(LED7_DOT);
    ui_led7_flash_icon(LED7_MP3);
    ui_led7_flash_icon(LED7_REPEAT);
    ui_led7_flash_icon(LED7_CHARGE);
    ui_led7_flash_icon(LED7_BT);
    ui_led7_flash_icon(LED7_AUX);
    ui_led7_flash_icon(LED7_WMA);
#endif /* #if LED7_ICON_FLASH_TEST */

#if LED7_SHOW_NUMBER_TEST
    static u8 cnt = 0;
    if (!(cnt % 5)) {
        ui_led7_show_number(123); //高位显示0
    } else {
        ui_led7_show_number2(123); //高位不显示0
    }
    if (cnt == 0) {
        ui_led7_setX(0);
    }
    ui_led7_show_number_add(cnt++);
#endif

#if LED7_SHOW_STRING_TEST
    /* static u8 cnt = '0'; */
    /* if (cnt < ('z' + 1)) { */
    /* ui_led7_show_char(cnt++); //显示数字 */
    /* } else { */
    /* cnt = '0'; */
    /* } */

    /* u8 buf[3] = {0}; */
    /* if (cnt < 'z') { */
    /* buf[0] = cnt; */
    /* buf[1] = cnt + 1; */
    /* buf[2] = '\0'; */
    /* ui_led7_show_string(buf); //显示字符串 */
    /* cnt++; */
    /* } else { */
    /* cnt = '0'; */
    /* } */
#endif

#if LED7_SHOW_MODE_MENU_TEST
    static u8 index = 0;
    /* ui_led7_show_string_menu(index++); */
    /* if (index > ARRAY_SIZE(menu_string_table)) { */
    /* index = 0; */
    /* } */

    //ui_led7_show_music_play_time(index++);
    /* ui_led7_show_volume(index++); */
    /* if (index > 99) { */
    /* index = 0; */
    /* } */
    ui_led7_show_fm_station(index++);
#endif
}

#endif /* for test code mcro */


#endif /* #if TCFG_UI_LED7_ENABLE */
