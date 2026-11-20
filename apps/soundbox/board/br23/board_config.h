#ifndef BOARD_CONFIG_H
#define BOARD_CONFIG_H

/*
 *  板级配置选择
 */

#define CONFIG_BOARD_AC695X_DEMO
// #define CONFIG_BOARD_AC6951_KGB_V1
// #define CONFIG_BOARD_AC6955F_HEADSET_MONO
// #define CONFIG_BOARD_AC6952E_LIGHTER
// #define CONFIG_BOARD_AC695X_CHARGING_BIN
// #define CONFIG_BOARD_AC695X_BTEMITTER
// #define CONFIG_BOARD_AC695X_TWS_BOX
// #define CONFIG_BOARD_AC695X_TWS
// #define CONFIG_BOARD_AC695X_MULTIMEDIA_CHARGING_BIN
// #define CONFIG_BOARD_AC695X_SOUNDCARD
// #define CONFIG_BOARD_AC6954A_DEMO
// #define CONFIG_BOARD_AC695X_SMARTBOX
// #define CONFIG_BOARD_AC695X_LCD
// #define CONFIG_BOARD_AC695X_CVP_DEVELOP//第三方清晰语音处理模块开发
/*
 *CONFIG_BOARD_AC695X_AUDIO_EFFECTS板级用于开发音效方案
 *(1)使用说明：该板级根据现有芯片资源，优化配置了音效模块组合
 *			   如果修改了里面某些配置，导致数据流卡顿，则表示不支持（触及芯片性能极限）
 *(2)适用场景：K歌宝
 */
// #define CONFIG_BOARD_AC695X_AUDIO_EFFECTS

/*
 *CONFIG_BOARD_AC695X_MEGAPHONE板级用于开发大声公方案
 *(1)使用说明：该板级根据现有芯片资源，优化配置了音效模块组合
 *			   如果修改了里面某些配置，导致数据流卡顿，则表示不支持（触及芯片性能极限）
 *(2)适用场景：大声公/扩音器
 */
// #define CONFIG_BOARD_AC695X_MEGAPHONE

// #define CONFIG_BOARD_AC6951G
// #define CONFIG_BOARD_AC6083A
// #define CONFIG_BOARD_AC6083A_IAP


#define DAC_OUTPUT_MONO_L                  0    //左声道
#define DAC_OUTPUT_MONO_R                  1    //右声道
#define DAC_OUTPUT_LR                      2    //立体声
#define DAC_OUTPUT_MONO_LR_DIFF            3    //单声道差分输出

#define DAC_OUTPUT_DUAL_LR_DIFF            6    //双声道差分
#define DAC_OUTPUT_FRONT_LR_REAR_L         7    //三声道单端输出 前L+前R+后L (不可设置vcmo公共端)
#define DAC_OUTPUT_FRONT_LR_REAR_R         8    //三声道单端输出 前L+前R+后R (可设置vcmo公共端)
#define DAC_OUTPUT_FRONT_LR_REAR_LR        9    //四声道单端输出(不可设置vcmo公共端)


//-------------------------------------------------

#define MULT_IO_PA1PA2_OFF_PA2

//佳韵
//   #define board_ac695x_jy_TH_15_ac6951c	 
//  #define board_ac695x_JY_TP_15_ac6951c		//TH-15换显示屏
// #define board_ac695x_JY_TS_12_12RGB_ac6951c		//TP15 改功能 编码器 按键操作
//  #define board_ac695x_JY_TS_15_28RGB_ac6951c		//TS15 改电位器 按键
// #define board_ac695x_JY_TS_12_24RGB_ac6951c		//TP15 改功能 编码器 按键操作
//  #define board_ac695x_JY_TB_15_ac6951c		//TH-15换显示屏
//  #define board_ac695x_JY_TS_8_18RGB_ac6951c		//TP15 改功能 编码器 按键操作
// #define board_ac695x_JY_TB_15_RGB24_ac6951c		// 
//    #define board_ac695x_JY_GB_12_ac6951c		//Tp-15 add aux
//    #define board_ac695x_JY_GB_15_ac6951c		// gb12 modify rgb 
////  #define board_ac695x_JY_GW_12_ac6951c		// 
//  #define board_ac695x_JY_TS_8_7VR_18RGB_ac6951c		// NOUSE


//#define board_ac695x_JY_TX_2208_12V_48LED_AC6951C	// 20250412 modify  gb15
// #define board_ac695x_JY_tx_2212_12v_64led_ac6951c		// 20250412 modify  gb15
// #define board_ac695x_JY_tx_2310_124led_ac6951c		// 20250412 modify  gb15
// #define board_ac695x_JY_tx_2308_7_4_ac6951c		// 20250412 modify  gb15
// #define board_ac695x_JY_tx_2608_88led_ac6951c		// 20250412 modify  gb15
//#define board_ac695x_JY_tx_2610_7_4v_88led_ac6951c		// 20250412 modify  gb15
//#define board_ac695x_JY_TX_2610_12v_88led_AC6951C		// 20260126 add
// #define board_ac695x_JY_tx_2728_7_4_ac6951c		// 20260525 add
/// #define board_ac695x_JY_tx_2508_7_4_ac6951c		// 20260527 add
 

//#define board_ac695x_JY_tx_2210_12v_56led_ac6951c		// 20250412 modify  gb15
// #define board_ac695x_JY_TX_2208_7v4_48LED_AC6951C		// 20250412 modify  gb15
// #define board_ac695x_JY_TX_2810_7v4_118LED_AC6951C		// 20260526 add modify adkey rgb 



//#define board_dy_DS4751C_FL872_ac6951c_6ADKEY		// 20250412 modify  gb15
//#define board_dy_DS4751C_FL872_ac6951c		// 20250412 modify  gb15
// #define board_dy_DS4751C_FL872_V1_ac6951c		// dy_DS4751C_FL872



//#define board_DG_1257_73led_ac6951c		// 20250412 modify  gb15
//#define board_LH_808_20led_ac6951c		// 20250412 modify  gb15
//#define board_DG_1263_2led_ac6951c		// 20250412 modify  gb15
//#define board_DG_1257_79led_ac6951c		// 20250412 modify  gb15
//#define board_DG_1258_ac6951c		// 20250412 modify  gb15
//#define board_sub2100_ac6951c		// 20250412 modify  gb15


//#define JD_X3116_1_XY8080_M62429_ac6951c		// 20250412 modify  gb15



#define board_ac695x_T_200_TONE_ac6951c		
 

#if defined board_ac695x_kph_k16309_ac6951c
#include "board_ac695x_demo/board_ac695x_kph_k16309_ac6951c.h"

#elif defined JD_X3116_1_XY8080_M62429_ac6951c
#include "board_ac695x_demo/JD_X3116_1_XY8080_M62429_ac6951c.h"

#elif defined board_DG_1257_73led_ac6951c
#include "board_ac695x_demo/board_DG_1257_73led_ac6951c.h"
#elif defined board_DG_1257_79led_ac6951c
#include "board_ac695x_demo/board_DG_1257_79led_ac6951c.h"
#elif defined board_DG_1258_ac6951c
#include "board_ac695x_demo/board_DG_1258_ac6951c.h"
#elif defined board_LH_808_20led_ac6951c
#include "board_ac695x_demo/board_LH_808_20led_ac6951c.h"
#elif defined board_DG_1263_2led_ac6951c
#include "board_ac695x_demo/board_DG_1263_2led_ac6951c.h"
#elif defined board_sub2100_ac6951c
#include "board_ac695x_demo/board_sub2100_ac6951c.h"

#elif defined board_dy_DS4751C_FL872_ac6951c_6ADKEY
#include "board_ac695x_demo/board_dy_DS4751C_FL872_ac6951c_6ADKEY.h"
#elif defined board_dy_DS4751C_FL872_ac6951c
#include "board_ac695x_demo/board_dy_DS4751C_FL872_ac6951c.h"
#elif defined board_dy_DS4751C_FL872_V1_ac6951c
#include "board_ac695x_demo/board_dy_DS4751C_FL872_V1_ac6951c.h"


#elif defined board_ac695x_T_200_TONE_ac6951c
#include "board_ac695x_demo/board_ac695x_T_200_TONE_ac6951c.h"


#elif defined board_ac695x_jy_TH_15_ac6951c
#include "board_ac695x_demo/board_ac695x_jy_TH_15_ac6951c.h"
#elif defined board_ac695x_JY_TP_15_ac6951c
#include "board_ac695x_demo/board_ac695x_JY_TP_15_ac6951c.h"
#elif defined board_ac695x_JY_GB_12_ac6951c
#include "board_ac695x_demo/board_ac695x_JY_GB_12_ac6951c.h"
#elif defined board_ac695x_JY_GB_15_ac6951c
#include "board_ac695x_demo/board_ac695x_JY_GB_15_ac6951c.h"
#elif defined board_ac695x_JY_tx_2210_56led_ac6951c
#include "board_ac695x_demo/board_ac695x_JY_tx_2210_56led_ac6951c.h"
#elif defined board_ac695x_JY_tx_2210_12v_56led_ac6951c
#include "board_ac695x_demo/board_ac695x_JY_tx_2210_12v_56led_ac6951c.h"
#elif defined board_ac695x_JY_TX_2208_7v4_48LED_AC6951C
#include "board_ac695x_demo/board_ac695x_JY_TX_2208_7v4_48LED_AC6951C.h"
#elif defined board_ac695x_JY_TX_2810_7v4_118LED_AC6951C
#include "board_ac695x_demo/board_ac695x_JY_TX_2810_7v4_118LED_AC6951C.h"
#elif defined board_ac695x_JY_TX_2208_12V_48LED_AC6951C
#include "board_ac695x_demo/board_ac695x_JY_TX_2208_12V_48LED_AC6951C.h"
#elif defined board_ac695x_JY_tx_2212_12v_64led_ac6951c
#include "board_ac695x_demo/board_ac695x_JY_tx_2212_12v_64led_ac6951c.h"
#elif defined board_ac695x_JY_tx_2310_124led_ac6951c
#include "board_ac695x_demo/board_ac695x_JY_tx_2310_124led_ac6951c.h"
#elif defined board_ac695x_JY_tx_2308_7_4_ac6951c
#include "board_ac695x_demo/board_ac695x_JY_tx_2308_7_4_ac6951c.h"
#elif defined board_ac695x_JY_tx_2728_7_4_ac6951c
#include "board_ac695x_demo/board_ac695x_JY_tx_2728_7_4_ac6951c.h"
#elif defined board_ac695x_JY_tx_2508_7_4_ac6951c
#include "board_ac695x_demo/board_ac695x_JY_tx_2508_7_4_ac6951c.h"
#elif defined board_ac695x_JY_tx_2608_88led_ac6951c
#include "board_ac695x_demo/board_ac695x_JY_tx_2608_88led_ac6951c.h"
#elif defined board_ac695x_JY_TX_2610_12v_88led_AC6951C
#include "board_ac695x_demo/board_ac695x_JY_TX_2610_12v_88led_AC6951C.h"
#elif defined board_ac695x_JY_tx_2210_56led_ac6951c
#include "board_ac695x_demo/board_ac695x_JY_tx_2210_56led_ac6951c.h"
#elif defined board_ac695x_JD_X3116_1_ac6951c
#include "board_ac695x_demo/board_ac695x_JD_X3116_1_ac6951c.h"
#elif defined board_ac695x_JY_GB_8_ac6951c
#include "board_ac695x_demo/board_ac695x_JY_GB_8_ac6951c.h"
#elif defined board_ac695x_JY_GB_10_SW_ac6951c
#include "board_ac695x_demo/board_ac695x_JY_GB_10_SW_ac6951c.h"
#elif defined board_ac695x_JY_TB_15_8848_ac6951c
#include "board_ac695x_demo/board_ac695x_JY_TB_15_8848_ac6951c.h"
#elif defined board_ac695x_JY_TB_12_8848_ac6951c
#include "board_ac695x_demo/board_ac695x_JY_TB_12_8848_ac6951c.h"
#elif defined board_ac695x_JY_GK_15_ac6951c
#include "board_ac695x_demo/board_ac695x_JY_GK_15_ac6951c.h"
#elif defined board_ac695x_JY_GW_12_ac6951c
#include "board_ac695x_demo/board_ac695x_JY_GW_12_ac6951c.h"
#elif defined board_ac695x_JY_TB_15_ac6951c
#include "board_ac695x_demo/board_ac695x_JY_TB_15_ac6951c.h"
#elif defined board_ac695x_JY_TS_12_12RGB_ac6951c
#include "board_ac695x_demo/board_ac695x_JY_TS_12_12RGB_ac6951c.h"
#elif defined board_ac695x_JY_TS_15_28RGB_ac6951c
#include "board_ac695x_demo/board_ac695x_JY_TS_15_28RGB_ac6951c.h"
#elif defined board_ac695x_JY_TB_15_RGB24_ac6951c
#include "board_ac695x_demo/board_ac695x_JY_TB_15_RGB24_ac6951c.h"
#elif defined board_ac695x_JY_TS_12_24RGB_ac6951c
#include "board_ac695x_demo/board_ac695x_JY_TS_12_24RGB_ac6951c.h"
#elif defined board_ac695x_JY_TS_8_18RGB_ac6951c
#include "board_ac695x_demo/board_ac695x_JY_TS_8_18RGB_ac6951c.h"
#elif defined board_ac695x_JY_TS_8_16V_18RGB_ac6951c
#include "board_ac695x_demo/board_ac695x_JY_TS_8_16V_18RGB_ac6951c.h"
#elif defined board_ac695x_JY_TS_8_7VR_18RGB_ac6951c
#include "board_ac695x_demo/board_ac695x_JY_TS_8_7VR_18RGB_ac6951c.h"
#elif defined board_ac695x_JY_TS_8_7VR_18RGB_XL1198BR_ac6951c
#include "board_ac695x_demo/board_ac695x_JY_TS_8_7VR_18RGB_XL1198BR_ac6951c.h"


//-------------------------------------------------
#else
#include "board_ac6954a_demo/board_ac6954a_demo_cfg.h"  //SPDIF-HDMI-IIS测试板
#include "board_ac695x_demo/board_ac695x_demo_cfg.h"
#include "board_ac6951_kgb_v1/board_ac6951_kgb_cfg.h"
#include "board_ac6952e_lighter/board_ac6952e_lighter_cfg.h"       //蓝牙点烟器
#include "board_ac6955f_headset_mono/board_ac6955f_headset_mono_cfg.h" //蓝牙带卡耳机
#include "board_ac695x_charging_bin/board_ac695x_charging_bin.h"   //智能充电仓
#include "board_ac695x_btemitter/board_ac695x_btemitter.h"   //蓝牙发射器
#include "board_ac695x_tws_box/board_ac695x_tws_box.h"   //对箱
#include "board_ac695x_tws/board_ac695x_tws.h"   //纯对箱
#include "board_ac695x_multimedia_charging_bin/board_ac695x_multimedia_charging_bin.h"   //多媒体智能充电仓发射器
#include "board_ac695x_soundcard/board_ac695x_soundcard.h"   //声卡
#include "board_ac695x_smartbox/board_ac695x_smartbox.h"   //杰理之家智能音箱
#include "board_ac695x_lcd/board_ac695x_lcd_cfg.h"//
#include "board_ac695x_cvp_develop/board_ac695x_cvp_develop_cfg.h"   //第三方清晰语音处理模块开发
#include "board_ac695x_audio_effects/board_ac695x_audio_effects_cfg.h"   //K歌宝开发
#include "board_ac695x_megaphone/board_ac695x_megaphone_cfg.h"   //大声公/扩音器开发

#include "board_ac6951g/board_ac6951g_cfg.h"

#include "board_ac6083a/board_ac6083a_cfg.h"
#include "board_ac6083a_iap/board_ac6083a_iap_cfg.h"
#endif

#define  DUT_AUDIO_DAC_LDO_VOLT   							DACVDD_LDO_2_90V


#endif
