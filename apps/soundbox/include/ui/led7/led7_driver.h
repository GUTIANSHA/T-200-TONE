#ifndef	_LED7_DRV_H_
#define _LED7_DRV_H_


typedef struct _led7_VAR {
    u8  bCoordinateX;
    u8  bFlashChar;
    u8  bShowBuff[4];  //[0] ~ [3]显示的数字/字母显示的所有段
    u32 bShowIcon;     //每1bit对应LED4的1段, maybe icon > 7段
    u32 bFlashIcon;    //闪烁标志
    u8  bShowBuff1[9]; //[i]->第i个管脚, value的每一bit对应一个管脚, scan时使用
#ifdef SUPPORT_UI_XY8080
		u8	bShowBuff2[9]; 
#endif

}LED7_VAR;

#ifdef SUPPORT_UI_XY8080
void update_send_leddisp_byte(void);
void xy8080_update_send_leddisp_byte(void);
void send_xy8080_code(bool bBit);
void send_xy8080_led_disp(void);
#endif


#ifdef LED_ID_SdUsbFmAux_BathBatl_88n48

/*
TF USB		BATH
FM AUX 88:8.8 BATL
*/

#define LED7_SD        BIT(0)
#define LED7_USB       BIT(1)
#define LED7_FM        BIT(2)
#define LED7_AUX       BIT(3)
#define LED7_BAT_H     BIT(4)
#define LED7_BAT_L     BIT(5)
#define LED7_2POINT    BIT(6)
#define LED7_DOT       BIT(7)
#elif defined LED_ID_6PIN_JW3096
#define LED7_2POINT    BIT(0)
#define LED7_DOT       BIT(1)


#elif defined LED_ID_PlayUsbPauseSd_FmMp3
#define LED7_PLAY      BIT(0)
#define LED7_USB       BIT(1)
#define LED7_FM        BIT(2)
#define LED7_PAUSE     BIT(3)
#define LED7_SD        BIT(4)
#define LED7_MP3       BIT(5)
#define LED7_2POINT    BIT(6)
#define LED7_DOT       BIT(7)

#elif defined LED_ID_BtMp3AuxRecRptBat5
#ifndef SPI1_RGB_DS4751C_FL872
#define LED7_BT       BIT(0)
#define LED7_MP3       BIT(1)
#define LED7_AUX      BIT(2)
#endif
#define LED7_REC        BIT(3)
#define LED7_2POINT        BIT(4)
#define LED7_DOT     BIT(5)
#define LED7_REPEAT BIT(6)
#ifdef DY_16309_LED_PAUSE_PLAY_CHANGE
#define LED7_PAUSE   BIT(7)
#else
#define LED7_PLAY   BIT(7)
#endif
#define LED7_BAT01     BIT(8)
#define LED7_BAT02   BIT(9)
#define LED7_BAT03   BIT(10)
#define LED7_BAT04        BIT(11)
#define LED7_BAT05     BIT(12)
#define LED7_BAT06     BIT(13)


#elif defined LED_ID_SdUsbFmAux_BathBatl
#define LED7_SD        BIT(0)
#define LED7_USB       BIT(1)
#define LED7_BAT_H     BIT(2)
#define LED7_FM        BIT(3)
#define LED7_AUX       BIT(4)
#define LED7_BAT_L     BIT(5)
#define LED7_2POINT    BIT(6)
#define LED7_DOT       BIT(7)

#elif defined LED_ID_PlayUsbPauseSdFmBat3
#define LED7_PLAY      BIT(0)
#define LED7_USB       BIT(1)
#define LED7_PAUSE     BIT(2)
#define LED7_SD        BIT(3)
#define LED7_FM        BIT(4)
#define LED7_BAT01     BIT(5)
#define LED7_BAT02     BIT(6)
#define LED7_BAT03     BIT(7)
#define LED7_2POINT    BIT(8)
#define LED7_DOT       BIT(9)

#elif defined LED_XY8080_BTSDUSBRECRECPLAYBAT3
#define LED7_BT      	BIT(0)
#define LED7_SD       	BIT(1)
#define LED7_USB     	BIT(2)
#define LED7_REC        BIT(3)
#define LED7_RECPLAY   	BIT(4)
#define LED7_BAT01     	BIT(5)
#define LED7_BAT02     	BIT(6)
#define LED7_BAT03     	BIT(7)
#define LED7_2POINT    	BIT(8)
#define LED7_DOT       	BIT(9)

#elif defined LED_ID_BtSdUsbRecBat3Rpt_88N45

/*BT   USB  8 8:8.8 BAT 1 2 3 */
/*SD   REC		   RPT		 */
// LED7_BT,LED7_SD,LED7_usb,LED7_REC,LED7_REPEAT,LED7_BAT1,LED7_BAT2,LED7_BAT2,LED7_2POINT,LED7_DOT

#define LED7_BT      BIT(0)
#define LED7_SD       BIT(1)
#define LED7_USB     BIT(2)
#define LED7_REPEAT	BIT(3)
#define LED7_FM		BIT(4)
#define LED7_BAT01     BIT(5)
#define LED7_BAT02     BIT(6)
#define LED7_BAT03     BIT(7)
#define LED7_2POINT    BIT(8)
#define LED7_DOT       BIT(9)

#elif defined LED_ID_FMUSBTFBATHBATLMP3_XL1198BR 
/*
FM			BATH
USB 88:88	BATL
TF			MP3
// FM USB TF BATH BATL MP3 :  .


*/

#define LED7_FM     	 BIT(0)
#define LED7_USB       BIT(1)
#define LED7_SD    	 BIT(2)
#define LED7_BAT_H	BIT(3)
#define LED7_BAT_L   BIT(4)
#define LED7_MP3     	BIT(5)
#define LED7_2POINT BIT(6)
#define LED7_DOT       BIT(7)

#elif defined LED_ID_BtSdUsbRecRecplayBat3
// LED7_BT,LED7_SD,LED7_usb,LED7_REC,LED7_RECPLAY,LED7_BAT1,LED7_BAT2,LED7_BAT2,LED7_2POINT,LED7_DOT
/*
88:8.8
BT SD  USB REC RECPLAY BAT123 

*/
// #define LED7_BT             BIT(0)
// #define LED7_SD             BIT(1)
// #define LED7_USB            BIT(2)
// #define LED7_REC            BIT(3)
// #define LED7_RECPLAY        BIT(4)
// #define LED7_BAT01          BIT(5)
// #define LED7_BAT02          BIT(6)
// #define LED7_BAT03          BIT(7)
// #define LED7_2POINT         BIT(8)
// #define LED7_DOT            BIT(9)

#define LED7_PLAY  		BIT(0) 		//0: 播放图标
#define LED7_PAUSE 		BIT(1) 		//1: 暂停图标
#define LED7_USB   		BIT(2) 		//2: USB图标
#define LED7_SD   		BIT(3) 		//3: SD图标
#define LED7_2POINT 	BIT(4) 		//4: 冒号图标
#define LED7_FM 		BIT(5) 		//5: FM图标
#define LED7_MP3 		BIT(6) 		//7: MP3图标



#elif defined LED_ID_JW_5227_1B // JW52277_1B
/*
BAT123 FM SD USB REC RECPLAY BT
88:8.8
*/
// LED7_BAT1,LED7_BAT2,LED7_BAT2,LED_FM,LED_SD,LED_USB,LED_REC,LED_RECPLAY,LED_BT,LED7_2POINT,LED7_DOT

#define LED7_BAT01     BIT(0)
#define LED7_BAT02     BIT(1)
#define LED7_BAT03     BIT(2)
#define LED7_FM      BIT(3)
#define LED7_SD       BIT(4)
#define LED7_USB     BIT(5)
#define LED7_REC        BIT(6)
#define LED7_RECPLAY      BIT(7)
#define LED7_BT      BIT(8)
#define LED7_2POINT    BIT(9)
#define LED7_DOT       BIT(10)


#elif defined LED_ID_JW_18407_1B  

/*
88:8.8 bat1234
bt fm aux rec usb tf
*/
// LED7_BT,LED7_FM,LED7_AUX,LED_REC,LED_USB,LED_TF,LED7_2POINT,LED7_DOT,LED7_BAT_LL,LED7_BAT_RR,LED7_BAT01,LED7_BAT02,LED7_BAT03,LED7_BAT04


#define LED7_BT      BIT(0)
#define LED7_FM      BIT(1)
#define LED7_AUX        BIT(2)
#define LED7_REC        BIT(3)
#define LED7_USB     BIT(4)
#define LED7_SD       BIT(5)
#define LED7_2POINT    BIT(6)
#define LED7_DOT       BIT(7)
#define LED7_BAT_LL      BIT(8)
#define LED7_BAT_RR     BIT(9)
#define LED7_BAT01     BIT(10)
#define LED7_BAT02     BIT(11)
#define LED7_BAT03     BIT(12)
#define LED7_BAT04     BIT(13)


#elif defined LED_ID_BtSdUsbRecRecplayBat3_4018 // JW4018

// LED7_BT,LED7_SD,LED7_usb,LED7_REC,LED7_RECPLAY,LED7_BAT1,LED7_BAT2,LED7_BAT2,LED7_2POINT,LED7_DOT

#define LED7_BT      BIT(0)
#define LED7_SD       BIT(1)
#define LED7_USB     BIT(2)
#define LED7_REC        BIT(3)
#define LED7_RECPLAY  BIT(4)
#define LED7_BAT01     BIT(5)
#define LED7_BAT02     BIT(6)
#define LED7_BAT03     BIT(7)
#define LED7_2POINT    BIT(8)
#define LED7_DOT       BIT(9)

#elif defined LED_ID_SD_2776YRG_15 // JW4018

/*
	=		=
	= 18.8	=
	=		=
	=		=
	usb tf fm aux
*/
// LED7_USB,LED7_SD,LED7_FM,LED7_AUX,LED7_DOT

#define LED7_USB      BIT(0)
#define LED7_SD       BIT(1)
#define LED7_FM     BIT(2)
#define LED7_AUX        BIT(3)
#define LED7_DOT      BIT(4)

#else

// #define LED7_PLAY  		BIT(0) 		//0: 播放图标
// #define LED7_PAUSE 		BIT(1) 		//1: 暂停图标
// #define LED7_USB   		BIT(2) 		//2: USB图标
// #define LED7_SD   		BIT(3) 		//3: SD图标
// #define LED7_2POINT 	BIT(4) 		//4: 冒号图标
// #define LED7_FM 		BIT(5) 		//5: FM图标
// #define LED7_DOT 		BIT(6) 		//6: 小数点图标
// #define LED7_MP3 		BIT(7) 		//7: MP3图标
// #define LED7_REPEAT 	BIT(8) 		//8: REPEAT图标
// #define LED7_CHARGE 	BIT(9) 		//9: 充电图标
// #define LED7_BT 		BIT(10) 	//10: 充电图标
// #define LED7_AUX 		BIT(11)  	//11: AUX图标
// #define LED7_WMA 		BIT(12) 	//12: WMA图标

#define LED7_USB   		BIT(0) 		//2: USB图标
#define LED7_SD   		BIT(1) 		//3: SD图标
#define LED7_FM 		BIT(2) 		//5: FM图标
#define LED7_AUX 		BIT(3)  	//11: AUX图标
#define LED7_BAT_H      BIT(4)      //电量
#define LED7_2POINT 	BIT(5) 		//4: 冒号图标
#define LED7_DOT 		BIT(6) 		//6: 小数点图标


#endif

#ifndef LED7_2POINT
#define LED7_2POINT   0
#endif

#ifndef LED7_PLAY
#define LED7_PLAY   0
#endif
#ifndef LED7_PAUSE
#define LED7_PAUSE   0
#endif
#ifndef LED7_MP3
#define LED7_MP3   0
#endif
#ifndef LED7_REPEAT
#define LED7_REPEAT   0
#endif
#ifndef LED7_CHARGE
#define LED7_CHARGE   0
#endif
#ifndef LED7_BT
#define LED7_BT   0
#endif
#ifndef LED7_AUX
#define LED7_AUX   0
#endif
#ifndef LED7_WMA
#define LED7_WMA   0
#endif
#ifndef LED7_DOT
#define LED7_DOT   0
#endif
#ifndef LED7_FM
#define LED7_FM   0
#endif
#ifndef LED7_USB
#define LED7_USB   0
#endif
#ifndef LED7_SD
#define LED7_SD   0
#endif
#ifndef LED7_BAT_L
#define LED7_BAT_L   0
#endif
#ifndef LED7_BAT_H
#define LED7_BAT_H   0
#endif
#ifndef LED7_BAT_3
#define LED7_BAT_3   0
#endif
#ifndef LED7_BAT_2
#define LED7_BAT_2   0
#endif
#ifndef LED7_BAT_1
#define LED7_BAT_1   0
#endif
#ifndef LED7_BAT01
#define LED7_BAT01   0
#endif
#ifndef LED7_BAT02
#define LED7_BAT02   0
#endif
#ifndef LED7_BAT03
#define LED7_BAT03   0
#endif
#ifndef LED7_BAT04
#define LED7_BAT04   0
#endif


typedef u32 UI_LED7_ICON;

typedef enum  _led7_pin_type {
    LED7_PIN7, //7个引脚
    LED7_PIN12, //12个引脚
    LED7_PIN13, //13个引脚
} LED7_PIN_TYPE;

struct seg2pin {
    u8 pinH;
    u8 pinL;
};

struct icon_seg2pin {
    UI_LED7_ICON icon;
    struct seg2pin seg2pin;
};

struct led7_pin7 {
    u8 pin[7];
};

struct led7_pin12 {
    u8 pin_comh[5];
    u8 pin_segl[7];
};

struct led7_pin13 {
    u8 pin_com[6];
    u8 pin_seg[7];
};


union led7_pin_cfg {
    struct led7_pin7 pin7;
    struct led7_pin12 pin12;
    struct led7_pin13 pin13;
};

#define LED_A   BIT(0)
#define LED_B	BIT(1)
#define LED_C	BIT(2)
#define LED_D	BIT(3)
#define LED_E	BIT(4)
#define LED_F	BIT(5)
#define LED_G	BIT(6)
#define LED_H	BIT(7)


struct led7_platform_data {
    LED7_PIN_TYPE pin_type;
    union led7_pin_cfg pin_cfg;
};

#define LED7_PLATFORM_DATA_BEGIN(data) \
		const struct led7_platform_data data = {

#define LED7_PLATFORM_DATA_END() \
};

//UI LED7 API:
//=================================================================================//
//                        		模块初始化显示接口                    			   //
//=================================================================================//
void *led7_init(const struct led7_platform_data *_data);
const struct ui_display_api *ui_led7_init(void *para);

//=================================================================================//
//                        		设置显示坐标接口                    			   //
/*
  	   ___    ___    ___    ___
	  |___|  |___|  |___|  |___|
	  |___|  |___|  |___|  |___|
	 ---0------1------2------3------> X
*/
//=================================================================================//
void led7_setX(u8 X);

//=================================================================================//
//                        		字符类显示接口                    				   //
//=================================================================================//
void led7_show_char(u8 chardata); 			//显示字符(追加方式)
void led7_flash_char_start(u8 index);       //闪烁单个字符
void led7_flash_char_stop(u8 index);        //取消闪烁单个字符
void led7_show_string(u8 *str);   			//显示字符串(追加方式)
void led7_show_string_reset_x(u8 *str);  	//显示字符串, x从0开始
void led7_show_string_align_right(u8 *str); //led7显示字符串(清屏&右方式)
void led7_show_string_align_left(u8 *str);  //led7显示字符串(清屏&左方式)

//=================================================================================//
//                        		数字类显示接口                    				   //
//=================================================================================//
void led7_show_number(u16 val); //显示数字(清屏&高位显示0)
void led7_show_number2(u16 val); //显示数字(清屏&高位不显示)
void led7_show_number_add(u16 val); //显示数字(追加方式)

//=================================================================================//
//                        		图标类显示接口                    				   //
//=================================================================================//
void led7_show_icon(UI_LED7_ICON icon); //显示单个图标(追加)
void led7_flash_icon(UI_LED7_ICON icon); //闪烁单个图标(追加)

//=================================================================================//
//                        		模式类类显示接口                    			   //
//=================================================================================//

//=================================================================================//
//                        		清屏类显示接口                    				   //
//=================================================================================//
void led7_show_null(void); //清除所有显示(数字,字符串和图标)
void led7_clear_string(void); //清除显示数字和字母
void led7_clear_icon(UI_LED7_ICON icon); //清除显示单个图标
void led7_clear_all_icon(void); //清除显示所有图标


#endif	/*	_LED_H_	*/
