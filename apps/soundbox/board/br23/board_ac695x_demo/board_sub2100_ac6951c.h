#ifndef CONFIG_BOARD_AC695X_DEMO_CFG_H
#define CONFIG_BOARD_AC695X_DEMO_CFG_H

#ifdef CONFIG_BOARD_AC695X_DEMO

/************************************************
20230925  modify rgb static mode  show 
20250411 modify adkey
录音 阻值3K
静音键 阻值6K2
话筒优先 阻值9K1
灯开关 阻值15K 取消长按功能
上一曲 阻值24K
下一曲 阻值33K
模式 阻值51K 

************************************************/
//-----------eq online adjust------------
//#define SUPPORT_EQ_ONLINE_SET_DM_DP
//#define SUPPORT_EQ_ONLINE_USB

//#define SUPPORT_USB_DEBUG
#ifdef SUPPORT_USB_DEBUG
#define CONFIG_DEBUG_ENABLE
#endif

#define SUPPORT_POWER_ON_AD_SHOW_DLY

// #define SUPPORT_MULT_EQ_FILE

/*---------------------------------------------
		customer  page
---------------------------------------------*/
// ------GENRAL  PAGE------------
#define SUPPORT_KPH_DEF
#define SUPPORT_KPH_MSG
#define SUPPORT_MUTE_KEY
#define SUPPORT_RECPLAY_KEY
#define SUPPORT_MODE_KEY
#define SUPPORT_STANBY_KEY
#define SUPPORT_STANBY_BACK_MODE
#define SUPPORT_MIC_ONOFF_KEY
#define SUPPORT_VOL_KEY_CANCEL_MUTE_STATE
#define SUPPORT_USER_REC_MODE  // 打开录音功能
#define SUPPORT_STOP_KEY
//#define SUPPORT_CHANGE_PANEL_FLASH_ERRO
#define SUPPORT_RPT_ONY_ONE_ALL
#define SUPPORT_TWS_DEFAULT_CHANGE_MODE
//#define SUPPORT_REC_MODE_VOICE
#define SUPPORT_TWS_KEY
//#define SUPPORT_VOL_ADJUST_EQ_GAIN
//#define SUPPORT_POWER_ON_FIRST_VOICE_ERRO
#define SUPPORT_MUSIC_BUG_MODIFY
#define REC_DISK_DEL
#define SUPPORT_MIC_OFFLINE_DISABLE_REC_KEY

//#define SUPPORT_LED_TWS_SHOW_T_ON_T_OFF

#define SUPPORT_TWS_KEY_VOICE

#define MUTE_POWER_ON

#define TWS_SYNC_EQ

#define MULT_IO_PA1PA2_OFF_PA2
#define MULT_IO_PA4PA5_OFF_PA4
#define MULT_IO_PA0PC7_OFF_PA0
//*******************************
//           显示屏
//*******************************

#define SUPPORT_ENCODER
#ifdef SUPPORT_ENCODER
//#define ENCODE_PIN_USE_PA6PA3

//#define SUPORT_RT_DENCORD_MENU

//#define SUPPORT_ENCODER_USE_TIMER0_SET


#define SUPPORT_ENCODER_2_IO
//#define ENCODE_PIN_USE_PA1PA6 // PA1PA6

#define SUPPORT_DOUBLE_ENCODER
//#define ENCODE2_PIN_USE_PA3PC2  // MICVOL  

#define A_BIT     BIT(8)
#define B_BIT     BIT(6)
#define A_PORT     JL_PORTA
#define B_PORT     JL_PORTA

#define ENCODE_PORTB_IN()		A_PORT->DIE |= A_BIT;A_PORT->DIR |= A_BIT;A_PORT->PU|= A_BIT;A_PORT->PD&=~ A_BIT;  \
								B_PORT->DIE |= B_BIT;B_PORT->DIR |= B_BIT;B_PORT->PU|= B_BIT; B_PORT->PD&=~ B_BIT;  
#define  BM_DN_IS_H()			(A_PORT->IN & A_BIT)
#define  BM_UP_IS_H()			(B_PORT->IN & B_BIT)   

#define A2_BIT     BIT(9)
#define B2_BIT     BIT(10)
#define A2_PORT     JL_PORTA
#define B2_PORT     JL_PORTA

#define ENCODE2_PORTB_IN()		A2_PORT->DIE |= A2_BIT;A2_PORT->DIR |= A2_BIT;A2_PORT->PU|= A2_BIT;A2_PORT->PD&=~ A2_BIT;  \
								B2_PORT->DIE |= B2_BIT;B2_PORT->DIR |= B2_BIT;B2_PORT->PU|= B2_BIT; B2_PORT->PD&=~ B2_BIT;  
#define  BM2_DN_IS_H()			(A2_PORT->IN & A2_BIT)
#define  BM2_UP_IS_H()			(B2_PORT->IN & B2_BIT)   


#define SUPPORT_sub2100_Encoder

#define KEY_Encoder_MENU_UP 		KEY_MENU__UP
#define KEY_Encoder_MENU_MIC_UP 	KEY_MENU__UP

#define KEY_Encoder_MENU_DN 		KEY_MENU__DN
#define KEY_Encoder_MENU_MIC_DN 	KEY_MENU__DN

//#define SUPPORT_ENCORD_LED
//#ifdef SUPPORT_ENCORD_LED // PC5
//#define SUPPORT_ENCORD_LED_FLASH
//#define EN_LED_BIT 	BIT(5)
//#define ENCORD_LED_ON()  JL_PORTC->DIR &= ~EN_LED_BIT;JL_PORTC->OUT &= ~EN_LED_BIT;JL_PORTC->PD &= ~EN_LED_BIT;JL_PORTC->PU &= ~EN_LED_BIT
//#define ENCORD_LED_OFF()    JL_PORTC->DIR &= ~EN_LED_BIT;JL_PORTC->OUT |= EN_LED_BIT;JL_PORTC->PD &= ~EN_LED_BIT;JL_PORTC->PU &= ~EN_LED_BIT
//#endif
//

#endif



#define SUPPORT_UI_UART_XY8080
#ifdef SUPPORT_UI_UART_XY8080
#define UI_UART_XY8080_PORT IO_PORTB_04
#endif

#if 1
#if 0//def support_led7_pin_cfg
#if 1
#define led7_pin6 IO_PORTC_02
#define led7_pin5 IO_PORTC_00
#define led7_pin4 IO_PORTA_09
#define led7_pin3 IO_PORTA_07
#define led7_pin2 IO_PORTA_08
#define led7_pin1 IO_PORTA_10
#define led7_pin0 IO_PORTC_01


#endif

#else
#define LED7_PIN_NULL
#endif

#define SUPPORT_BT_MODE_SHOW_BLUE  // NO DEF  SHOW " BT "
//#define SUPPORT_BT_MODE_SHOW__BT_  // SHOW "-BT-"

#define LED_ID_PlayUsbPauseSd_FmMp3
#ifdef LED_ID_PlayUsbPauseSd_FmMp3
#define LED_ICON_NUMS    8
#define LED_8NUMS        4
#endif

//#define LED_ID_SdUsbFmAux_BathBatl_88n48
#ifdef LED_ID_SdUsbFmAux_BathBatl_88n48
#define LED_ICON_NUMS    8
#define LED_8NUMS        4
#endif

//#define LED_ID_PlayUsbPauseSdFmBat3
#ifdef LED_ID_PlayUsbPauseSdFmBat3
#define LED_ICON_NUMS    10
#define LED_8NUMS        4
#endif

//#define LED_ID_BtSdUsbRecRecplayBat3_4018
#ifdef LED_ID_BtSdUsbRecRecplayBat3_4018
#define LED_ICON_NUMS    10
#define LED_8NUMS        4
#endif


//#define LED_ID_BtSdUsbRecBat3Rpt_88N45
#ifdef LED_ID_BtSdUsbRecBat3Rpt_88N45
#define LED_ICON_NUMS    10
#define LED_8NUMS        4
#endif

#endif


//*********5050灯条**************



//#define SPI_RGB_JY_48LED_TX_2208
#if	0//def SPI_RGB_JY48LED_TX_2208
#define SPI1_RGB_DEV_EN
#define SYS_CLK_USE_240M
#define SUPPORT_RGB_MODE_KEY
#define RGB_MAX			2
#endif

#define SPI1_RGB_sub2100
#ifdef SPI1_RGB_sub2100
#define SPI1_RGB_DEV_EN
#define SYS_CLK_USE_240M
#define SUPPORT_RGB_MODE_KEY
#define RGB_MAX			5 // 2

 #define SUPPORT_LED_SIGLE
#ifdef SUPPORT_LED_SIGLE //PA6
#endif


#endif

//*******************************
//           功放控制
//*******************************
#define SUPPORT_AMP_PA_MUTE

#ifdef SUPPORT_AMP_PA_MUTE
#define AMP_CS8683_32PIN
#ifdef AMP_CS8683_32PIN
// PA7
#define PA_BIT 	(BIT(12))
#define PA_UNMUTE()  	
												
#define PA_MUTE()    	
												
#define FM_UNMUTE()		

											
//#define SUPPORT_PA_FM_MODE_1V5


//#define SUPPORT_AMP_AB_D_MODE  // Pa3 h-on l-off
#if	0//def SUPPORT_AMP_AB_D_MODE
#define EN_BIT 	BIT(3)
#define AMP_AB_MODE()     //  JL_PORTA->DIE |= EN_BIT;JL_PORTA->DIR &= ~EN_BIT;JL_PORTA->OUT &= ~EN_BIT;JL_PORTA->PD &= ~EN_BIT;JL_PORTA->PU &= ~EN_BIT
#define AMP_D_MODE()    //JL_PORTA->DIE |= EN_BIT;JL_PORTA->DIR &= ~EN_BIT;JL_PORTA->OUT |= EN_BIT;JL_PORTA->PD &= ~EN_BIT;JL_PORTA->PU &= ~EN_BIT
#endif

#endif
#endif

#define SUPPORT_KPH_IR_FUNCTION

//#define IRKEY_JY_00BF
//#define IRKEY_JY_00FF
//#define IRKEY_JY_00FF_MENU
#define IRKEY_YS_00FF_807F_21KEY


#define SUPPORT_KPH_ADKEY_FUNCTION
//#define ADKEY_22K_10KEY_MODE_KPH
//#define ADKEY_47K_10KEY_MODE_JY
//#define ADKEY_47K_10KEY_MODE_JY_GB_15
#define ADKEY_22K_sub2100

//*******************************
//         音量设置
//*******************************

#define LED_SHOW_MAX_VOL      32//26//30
#define LED_SYS_VOL           30 //30
#define LED_VOL_DETAULT_VAL   20
//#define LED_FM_VOL            16
#define LED_LINEIN_VOL            26

#define SUPPORT_D_VOL_TABLE_SET			0,546,1092,1638,2184,2730,3276,3822,4369,4915,5461,6007,6553,7099,7645,8192,8738,9284,9830,10376,10922,11468,12014,12561,13107,13653,14199,14745,15291,15837,16384


//#define VOL_IO_AD_EN
#if 0//def VOL_IO_AD_EN
#define  AD_CH_VOL            	AD_CH_PA6
#define  ADVOL_BIT           	BIT(6)
#define  VOL_AD_IO_INIT()     	JL_PORTA->DIE &= ~ADVOL_BIT;JL_PORTA->DIR |= ADVOL_BIT; JL_PORTA->PU &= ~ADVOL_BIT;JL_PORTA->PD &= ~ADVOL_BIT;
#define  VOL_IO_AD_VAL_MAX    	910
#endif

//#define JT_VOL_IO_AD_EN
#if	0//def JT_VOL_IO_AD_EN
#define  AD_CH_JTVOL            	AD_CH_PB1
#define  JTVOL_BIT           		BIT(1)
#define  JTVOL_AD_IO_INIT()     	JL_PORTB->DIE &= ~JTVOL_BIT;JL_PORTB->DIR |= JTVOL_BIT; JL_PORTB->PU &= ~JTVOL_BIT;JL_PORTB->PD &= ~JTVOL_BIT;
#define  JTVOL_IO_AD_VAL_MIN    	7
#define  JTVOL_IO_AD_VAL_MAX    	1000
#define  JTVOL_MAX             		22 //  1.8v-3.16v  0.1v   (270度/14)=19度一级
#endif


//*******************************
//         音乐高低音
//*******************************

#define SUPPORT_USER_EQ_MODE  1// 高低音调节
#if SUPPORT_USER_EQ_MODE
#define  BASS_HZ    150  //低音频点
#define  TRE_HZ     4000 //高音频点
#define  MID_HZ     1500 //高音频点

#define EQ_POS_BASS 	0
#define EQ_POS_TRE 		1
#define EQ_POS_MID 		2

#define TRE_MAX      	24
#define BASS_MAX     	24
#define  MID_MAX		24
//#define  TRE_TABLE    -12,-11,-10,-9,-8,-7,-6,-5,-4,-3,-2,-1,0
//#define  BASS_TABLE   -12,-11,-10,-9,-8,-7,-6,-5,-4,-3,-2,-1,0
//#define  TRE_TABLE    -6,-5,-4,-3,-2,-1,0,1,2,3,4,5,6
//#define  BASS_TABLE  -6,-5,-4,-3,-2,-1,0,1,2,3,4,5,6
#define  TRE_TABLE		-18,-17,-16,-15,-14,-13,-12,-11,-10,-9,-8,-7,-7,-6,-6,-5,-5,-4,-4,-3,-3,-2,-1,-1,0
#define  BASS_TABLE		-18,-17,-16,-15,-14,-13,-12,-11,-10,-9,-8,-7,-7,-6,-6,-5,-5,-4,-4,-3,-3,-2,-1,-1,0
#define  MID_TABLE		-18,-17,-16,-15,-14,-13,-12,-11,-10,-9,-8,-7,-7,-6,-6,-5,-5,-4,-4,-3,-3,-2,-1,-1,0

#define TRE_DEFAULT_VAL    24
#define BASS_DEFAULT_VAL  24
#define MID_DEFAULT_VAL    24

#define SUPPORT_MID_SET

#define SUPPORT_MULT_EQ_ADJUST


//#define TRE_IO_AD_EN
#if	0//def TRE_IO_AD_EN
#define  AD_CH_TRE             AD_CH_PA12
#define  TRE_IO_AD_VAL_MAX     980
#define TRE_BIT BIT(12)
#define  BASS_AD_IO_INIT()     JL_PORTA->DIR |=TRE_BIT; JL_PORTA->PU &= ~TRE_BIT;JL_PORTA->PD &= ~TRE_BIT;JL_PORTA->DIE &= ~TRE_BIT;
#endif

//#define MID_IO_AD_EN //no use
#if	0//def TRE_IO_AD_EN
#define  AD_CH_MID             AD_CH_PA5
#define  MID_IO_AD_VAL_MAX     980
#define MID_BIT BIT(5)
#define  MID_AD_IO_INIT()     JL_PORTA->DIR |=MID_BIT; JL_PORTA->PU &= ~MID_BIT;JL_PORTA->PD &= ~MID_BIT;JL_PORTA->DIE &= ~MID_BIT;
#endif

//#define BASS_IO_AD_EN
#if	0//def TRE_IO_AD_EN
#define  AD_CH_BASS            AD_CH_PC5
#define  BASS_IO_AD_VAL_MAX    980
#define BASS_BIT BIT(5)
#define  TRE_AD_IO_INIT()      JL_PORTC->DIE &= ~BASS_BIT;JL_PORTC->DIR |=BASS_BIT; JL_PORTC->PU &= ~BASS_BIT;JL_PORTC->PD &= ~BASS_BIT;
//#define SUPPORT_BASS_SHOW_L_XX
#endif

#endif

//*******************************
//           话筒检测
//*******************************

#define SUPPORT_MIC_CHECK
#ifdef SUPPORT_MIC_CHECK // PR1
//#define SUPPORT_MIC_DET_USE_ENERGY
#ifndef SUPPORT_MIC_DET_USE_ENERGY
//#define SUPPORT_MIC_DET_REUSE_ADKEY
#define MIC_IO_INIT()     JL_PORTC->DIE |= BIT(7);JL_PORTC->DIR |= BIT(7);JL_PORTC->PD &= ~BIT(7);JL_PORTC->PU &= ~BIT(7);
#define MIC_IS_IN()     !(JL_PORTC->IN & BIT(7))
#endif
#endif


//#define SUPPORT_JT_CHECK
#if	0//def SUPPORT_JT_CHECK // PR0
#define JT_IO_INIT()     rtc_port_pr_die(0, 1);   rtc_port_pr_in(0);rtc_port_pr_pu(0, 1);rtc_port_pr_pd(0, 0);
#define JT_IS_IN()      !(rtc_port_pr_read(0))

 #define SUPPORT_JTLED_EN
#ifdef SUPPORT_JTLED_EN
#define JTLED_BIT 	BIT(11)
#define JTLED_OFF() JL_PORTB->DIR &= ~JTLED_BIT;JL_PORTB->OUT &= ~JTLED_BIT;JL_PORTB->PD &= ~JTLED_BIT;JL_PORTB->PU &= ~JTLED_BIT
#define JTLED_ON()   JL_PORTB->DIR &= ~JTLED_BIT;JL_PORTB->OUT |= JTLED_BIT;JL_PORTB->PD &= ~JTLED_BIT;JL_PORTB->PU &= ~JTLED_BIT
#endif

#endif


//*******************************
//       内置混响
//*******************************
 #define SUPPORT_MIC_ECHO
#ifdef SUPPORT_MIC_ECHO
//#define  SUPPORT_SOUNDSCAPES  // 话筒环境音效调试

#define  SUPPORT_MIC_ECHO_VOL_SET

#define  MIC_BASS_HZ    150  //低音频点
#define  MIC_TRE_HZ     4000 //高音频点

#define MIC_BASS 0
#define MIC_TRE 1


#define USER_MIC_GAIN                3     // 话筒增益0-14
#define MIC_ECHO_DELAY            210  //回声的延时时间 0-300ms       
#define MIC_ECHO_DECAY            65   // 0-70%
#define  MICVOL_MAX            		 32
#define  ECHO_MAX                 		16

#define MIC_VOL_DEFAULT_VAL          (MICVOL_MAX-2) //30  //开机默认话筒音量
#define MIC_ECHO_DEFAULT_VAL        0  //开机默认话筒混响

//#define  MIC_VOL_TABLE  0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30
//#define  MIC_ECHO_TABLE	 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18
//#define  MIC_ECHO_TABLE	 0,3,6,9,12,15,18,21,24,27,30,33,36,39,42,45,50,55,60
#define MIC_VOL_TABLE  			0,1,2,3,4,5,6,7,8,9,10,11,\
						11,12,13,14,15,16,17,18,19,20,21,22,23,\
						23,24,25,26,27,28,29,30
#define MIC_ECHO_TABLE	 		 0,3,9,12,\
						18,21,24,27,\
						30,33,36,39,\
						42,45,50,55,MIC_ECHO_DECAY

#define USER_MIC_VOL                1 // 话筒音量独立调
#if USER_MIC_VOL
#define SUPPORT_MIC_ALONE_VOL_MODE
#endif

//#define MICVOL_IO_AD_EN
#if 0//def MICVOL_IO_AD_EN
#define  AD_CH_MICVOL           AD_CH_PA5
#define  MICVOL_BIT           BIT(5)
#define  MICVOL_AD_IO_INIT()    JL_PORTA->DIE &= ~MICVOL_BIT;JL_PORTA->DIR |=MICVOL_BIT; JL_PORTA->PU &= ~MICVOL_BIT;JL_PORTA->PD &= ~MICVOL_BIT;
#define  MICVOL_IO_AD_VAL_MAX   980
#endif


//#define ECHO_IO_AD_EN
#if	0//def ECHO_IO_AD_EN
#define  AD_CH_ECHO          AD_CH_PA10
#define  MICECHO_BIT           BIT(10)
#define  ECHO_AD_IO_INIT()    JL_PORTA->DIE &= ~MICECHO_BIT;JL_PORTA->DIR |=MICECHO_BIT;  JL_PORTA->PU &= ~MICECHO_BIT;JL_PORTA->PD &= ~MICECHO_BIT;
#define  ECHO_IO_AD_VAL_MAX  980
#endif

//#define MIC_DELAY_IO_AD_EN
#if 0//def MIC_DELAY_IO_AD_EN
#define  AD_CH_MICDELAY          AD_CH_PA6
#define  MIC_DELAY_BIT           BIT(6)
#define  MIC_DELAY_AD_IO_INIT()    JL_PORTA->DIE &= ~MIC_DELAY_BIT;JL_PORTA->DIR |=MIC_DELAY_BIT;  JL_PORTA->PU &= ~MIC_DELAY_BIT;JL_PORTA->PD &= ~MIC_DELAY_BIT;
#define  MIC_DELAY_IO_AD_VAL_MAX  980
#define  MIC_DELAY_MAX                 16
#define  SUPPORT_MIC_DELAY_SET
#define MIC_DELAY_TABLE	 0,20,40,60,80,100,120,140,160,180,200,220,240,260,280,300,320,340,360
#endif

#define  SUPPORT_MIC_BASS_TRE_SET 
#ifdef  SUPPORT_MIC_BASS_TRE_SET 
#define TCFG_MIC_TUNNING_EQ_ENABLE    1    // mic  bass treb  

#define  MIC_EQ_MODE_LOW_SOUND 	0
#define  MIC_EQ_MODE_HIGH_SOUND	1


//#define MIC_BASS_IO_AD_EN
#ifdef MIC_BASS_IO_AD_EN
#define  AD_CH_MIC_BASS         AD_CH_PA5
#define  MIC_BASS_BIT           BIT(5)
#define  MIC_BASS_AD_IO_INIT()    JL_PORTA->DIE &= ~MIC_BASS_BIT;JL_PORTA->DIR |=MIC_BASS_BIT;  JL_PORTA->PU &= ~MIC_BASS_BIT;JL_PORTA->PD &= ~MIC_BASS_BIT;
#define  MIC_BASS_IO_AD_VAL_MAX  980
#endif

//#define MIC_TRE_IO_AD_EN
#ifdef MIC_TRE_IO_AD_EN
#define  AD_CH_MIC_TRE          AD_CH_PA1
#define  MIC_TRE_BIT           BIT(1)
#define  MIC_TRE_AD_IO_INIT()    JL_PORTA->DIE &= ~MIC_TRE_BIT;JL_PORTA->DIR |=MIC_TRE_BIT;  JL_PORTA->PU &= ~MIC_TRE_BIT;JL_PORTA->PD &= ~MIC_TRE_BIT;
#define  MIC_TRE_IO_AD_VAL_MAX  980
#endif
#endif
#define  MIC_TRE_TABLE  	-12,-11,-10,-9,-8,-7,-6,-5,-4,-3,-2,-1,0
#define MIC_TRE_DEFAULT_VAL    7
#define  MIC_TRE_MAX      12

#define  MIC_BASS_TABLE		-12,-11,-10,-9,-8,-7,-6,-5,-4,-3,-2,-1,0
#define MIC_BASS_DEFAULT_VAL    7
#define  MIC_BASS_MAX     12

#define SUPPORT_MIC_PR_CHECK //话筒优先
#ifdef SUPPORT_MIC_PR_CHECK

#define  SUPPORT_DODGE_MIC_PR_JY_TB_12
#define  SUPPORT_USER_DODGE_MIC_PR_SET_MUSIC_VOL
#ifdef SUPPORT_USER_DODGE_MIC_PR_SET_MUSIC_VOL
#define  TCFG_MIC_DODGE_EN	1
#define  SUPPORT_MIC_VOL_ZERO_OFF_MIC_PR
#else
#define  TCFG_MIC_DODGE_EN	0
#endif

//#define SUPPORT_MIC_PR_USE_IO_CHECK// 话筒优先用IO口 高低电平检测
#ifdef SUPPORT_MIC_PR_USE_IO_CHECK
#define MIC_PR_IO_CHECK_INIT()      JL_PORTC->DIE |= BIT(7);JL_PORTC->DIR |= BIT(7);JL_PORTC->PD &= ~BIT(7);JL_PORTC->PU |= BIT(7);
#define MIC_HAVE_SIGNAL()          !(JL_PORTC->IN & BIT(7))
#endif

//#define SUPPORT_MIC_PR_USE_AD_CHECK// 话筒优先用AD检测
#ifdef SUPPORT_MIC_PR_USE_AD_CHECK
#define AD_CH_MIC    AD_CH_PA5
#define MIC_PR_AD_IO_INIT()   JL_PORTA->DIR |= BIT(5); JL_PORTA->PU &= ~BIT(5);JL_PORTA->PD &= ~BIT(5);JL_PORTA->DIE &= ~BIT(5);
#endif
#endif

#endif


#define SUPORT_MENU_MODE
#ifdef SUPORT_MENU_MODE
#define MENU_VOL_MODE            0
#define MENU_TRE_MODE            1
#define MENU_BASS_MODE          2

#define MENU_MIC_VOL_MODE     3
#define MENU_MIC_TRE_MODE    4
#define MENU_MIC_BASS_MODE            5
#define MENU_MAX_MODE   6

#define MENU_MIC_ECHO_MODE      6
#define MENU_MIC_DELAY_MODE  7


//#define SUPPORT_MENU_VOL_TRE_BASS 

//#define SUPPORT_MENU_MIC_BASS_MIC_TRE_MIC_DELAY

//#define SUPPORT_MUSIC_PREV_NEXT_MENU
//#define MENU_NO_VOL

//#define SUPPORT_MENU_MIC_BASS_MIC_TRE_MIC_DELAY
//#define SUPPORT_MENU_MICVOL_ECHO_DELAY_KEY
//#define SUPPORT_MENU_MIC_BASS_TRE_ECHO_DELAY_KEY

////#define SUPPORT_MENU_ONLY_BASS_TRE

#define  MIC_DELAY_MAX                 18
#define  MIC_DELAY_DEFAULT_VAL                 MIC_DELAY_MAX
#define  SUPPORT_MIC_DELAY_SET
#define  MIC_DELAY_TABLE	 0,20,40,60,80,100,120,140,160,180,200,220,240,260,280,300,320,340,360

//#define SUPPORT_VM_MAIN  // 不操作后200MS 存储
// #define SUPPORT_VM_VOL
//#define SUPPORT_VM_TREB
//#define SUPPORT_VM_BASS
//#define SUPPORT_VM_MICVOL
//#define SUPPORT_VM_ECHO
//#define SUPPORT_VM_MICBASS
//#define SUPPORT_VM_MICTRE
//#define SUPPORT_VM_MICDELAY
//#define SUPPORT_GW_MENU_MODE  //  prev next  adjust vol+ -

// #define SUPPORT_VM_ALL_VOL_MODE  

#endif



//#define EQ_MENU
#if	0//def EQ_MENU
#define EQ_MENU_NO			0
#define EQ_MENU_1			1
#define EQ_MENU_2			2
#define EQ_MENU_MAX			3

#define SUPPORT_VM_MAIN
#ifdef SUPPORT_VM_MAIN
#define SUPPORT_VM_EQ50HZ
#define SUPPORT_VM_EQ6KHZ
#endif

#define MENU_EQ_MAX_VAL		24
//#define MENU_EQ_TABLE		-5,-5,-4,-4,-3,-3,-2,-2,-2,-1,-1,-1,0,1,1,1,2,2,2,3,3,4,4,5,5
#define MENU_EQ_TABLE		-12,-10,-8,-6,-5,-4,-3,-2,-1,0,1,1,2,2,3,3,3,4,4,4,5,5,5,6,6
#define MENU_EQ_DEFAULT_VOL	12
#endif


//*******************************
//           电池电量检测
//*******************************
//#define BAT_EXTERN_AD_EN
#if 0//def BAT_EXTERN_AD_EN

#define AD_CH_BAT           AD_CH_PC5
#define BAT_BIT           	BIT(5)
#define BAT_DET_IO_INIT()   JL_PORTC->DIE &= ~BAT_BIT;JL_PORTC->DIR |= BAT_BIT; JL_PORTC->PU &= ~BAT_BIT;JL_PORTC->PD &= ~BAT_BIT;

// #define SUPPORT_BAT_SHOW_TEST

#define BATTERY_SHOW
#define SUPPORT_LED7_BAT_SHOW
//#define SUPPORT_LOWPOWER_VOICE

//#define BAT_11v1_100K_UP_27_GND
#define BAT_12v_100K_UP_27_GND



#define BAT_FULL_VAL    	390	//
#define BAT_3_VAL        	380	//
#define BAT_2_VAL         	360	//
#define BAT_1_VAL       	333	//
#define BAT_LOW_VAL      	306	//


#endif



//#define SUPPORT_MOS_POWER_ON  
#if 0//def SUPPORT_MOS_POWER_ON
#define MOS_IO_SET_ON()   JL_PORTB->DIE |= BIT(1);JL_PORTB->DIR &= ~BIT(1);JL_PORTB->OUT |= BIT(1);JL_PORTB->PD &= ~BIT(1);JL_PORTB->PU &= ~BIT(1)
#define MOS_IO_SET_OFF() JL_PORTB->DIE |= BIT(1);JL_PORTB->DIR &= ~BIT(1);JL_PORTB->OUT &= ~BIT(1);JL_PORTB->PD |=BIT(1);JL_PORTB->PU &= ~BIT(1)
#define POWER_DET_IO_INIT()     JL_PORTB->DIR |= BIT(0);JL_PORTB->DIE |= BIT(0);JL_PORTB->PD &=~BIT(0);JL_PORTB->PU &= ~BIT(0);
#define POWER_IS_IN()         (JL_PORTB->IN  &  BIT(0))

#define SUPPORT_MOS_KEY_SHORT_AS_MODE_KEY  
#endif



//*************************************
//      电池充电检测及开关检测
//************************************

//#define DCIN_DET
#if	0//def DCIN_DET // PB7
#define DC_BIT BIT(0)
#define DC_DET_IO_INIT()      JL_PORTB->DIE |=DC_BIT;JL_PORTB->DIR |= DC_BIT;JL_PORTB->PU &= ~DC_BIT;JL_PORTB->PD &= ~DC_BIT;
#define DC_IS_IN()            !(JL_PORTB->IN & DC_BIT)

//#define SW_ONOFF_DET
#ifdef SW_ONOFF_DET
#define SW_BIT BIT(2)
#define SW_DET_IO_INIT()      JL_PORTC->DIE |=SW_BIT;JL_PORTC->DIR |= SW_BIT; JL_PORTC->PD &= ~SW_BIT;JL_PORTC->PU &= ~SW_BIT;
#define SW_IS_OFF()           !(JL_PORTC->IN & SW_BIT)
#endif

//#define SUPORT_EXTERN_LED_BAT_SHOW
#if	0//def SUPPORT_LINEIN_DET
#define LED_BAT_FULL_ON()     JL_PORTA->DIR &= ~BIT(7);JL_PORTA->OUT |= BIT(7); 
#define LED_BAT_FULL_OFF()    JL_PORTA->DIR &= ~BIT(7);JL_PORTA->OUT &= ~BIT(7); 
#define LED_CHARGE_ON()       JL_PORTA->DIR &= ~BIT(8);JL_PORTA->OUT |= BIT(8);
#define LED_CHARGE_OFF()      JL_PORTA->DIR &= ~BIT(8);JL_PORTA->OUT &= ~BIT(8);
#endif

#endif

//#define SUPPORT_LINEIN_DET
#if	0//def SUPPORT_LINEIN_DET
#define LINEIN_DET_PORT		JL_PORTC
#define LINEIN_DET_BIT		BIT(7)
#define LINEIN_DET_INIT()	LINEIN_DET_PORT->DIE |=LINEIN_DET_BIT;LINEIN_DET_PORT->DIR |= LINEIN_DET_BIT;LINEIN_DET_PORT->PU &= ~LINEIN_DET_BIT;LINEIN_DET_PORT->PD &= ~LINEIN_DET_BIT;
#define LINEIN_IN()			!(LINEIN_DET_PORT->IN & LINEIN_DET_BIT)
#endif

//--------------------FM------------
//#define  SUPPORT_FM_DATA_AMP
// #define  SUPPORT_FM_DATA_AMP_MODE_1
// #define  SUPPORT_FM_DATA_AMP_MODE_MAX
 #define  SUPPORT_FM_DATA_AMP_MODE_JL_20220519


//-------------提示音最大限制
//#define SUPPORT_TONE_MAX_VOL
#ifdef SUPPORT_TONE_MAX_VOL
#define TONE_VOL_MAX     15
#endif
//-------------linein 检测配置
//#define TCFG_LINEIN_PORT_UP_ENABLE        	0					// 检测IO上拉使能
#define SUPPORT_MODE_KEY_CHANGE_IN_LINE // TCFG_LINEIN_PORT_UP_ENABLE

//------开机优先模式设置  拔出u盘卡进入模式设置
//#define SUPPORT_POWER_ON_TO_AUX
#define SUPPORT_POWER_ON_TO_BT
//#define SUPPORT_POWER_ON_TO_FM

#define SUPPOR_MUSIC_OUT_TO_BT
//#define SUPPOR_MUSIC_OUT_TO_LINEIN


//--------------------语音设置------
// #define SUPPORT_USB_SD_VOICE

//---------------line 配置----------
//#define AUX_USER_DAC_R
//#define AUX_USER_AUX2_L



#define SUPPORT_BT_AUTO_PLAY
#define SUPPORT_NO_PLAY_MUTE
#define SUPPORT_VOL_0_MUTE_PA


#define MUSIC_MSG_HALF_SECOND       1 // MUSIC 半秒中断
#define BT_MSG_HALF_SECOND          1 //BT 半秒中断



 #define SUPPORT_HW_EQ_MODE  //使用外部EQ文件

#define SUPPORT_FM_EQ_GAIN

//---------------------------------------------


#define CONFIG_SDFILE_ENABLE
#define CONFIG_FLASH_SIZE       (1024 * 1024)

//*********************************************************************************//
//                                 配置开始                                        //
//*********************************************************************************//
#define ENABLE_THIS_MOUDLE					1
#define DISABLE_THIS_MOUDLE					0

#define ENABLE								1
#define DISABLE								0

#define LINEIN_INPUT_WAY_ANALOG      0
#define LINEIN_INPUT_WAY_ADC         1
#define LINEIN_INPUT_WAY_DAC         2

#define NO_CONFIG_PORT						(-1)


//*********************************************************************************//
//                                  app 配置                                       //
//*********************************************************************************//
#define TCFG_APP_BT_EN			            1
#define TCFG_APP_MUSIC_EN			        1
#define TCFG_APP_LINEIN_EN					1
#define TCFG_APP_FM_EN					    1
#define TCFG_APP_PC_EN					    0
#define TCFG_APP_RTC_EN				    	0
#define TCFG_APP_RECORD_EN			   		1
#define TCFG_APP_SPDIF_EN                   0
//*********************************************************************************//
//                               PCM_DEBUG调试配置                                 //
//*********************************************************************************//

//#define AUDIO_PCM_DEBUG					  	//PCM串口调试，写卡通话数据

//*********************************************************************************//
//                                 UART配置                                        //
//*********************************************************************************//
#ifdef SUPPORT_USB_DEBUG
#define TCFG_UART0_ENABLE					1 //ENABLE_THIS_MOUDLE                     //串口打印模块使能
#define TCFG_UART0_RX_PORT					NO_CONFIG_PORT                         //串口接收脚配置（用于打印可以选择NO_CONFIG_PORT）
#define TCFG_UART0_TX_PORT                  		IO_PORT_DP
#define TCFG_UART0_BAUDRATE  				115200 //115200//1000000                                   //串口波特率配置
#else
#define TCFG_UART0_ENABLE					0 //ENABLE_THIS_MOUDLE                     //串口打印模块使能
#define TCFG_UART0_RX_PORT					NO_CONFIG_PORT                         //串口接收脚配置（用于打印可以选择NO_CONFIG_PORT）
#define TCFG_UART0_TX_PORT  				NO_CONFIG_PORT                             //串口发送脚配置
#define TCFG_UART0_BAUDRATE  				1000000                                //串口波特率配置
#endif


//*********************************************************************************//
//                                 IIC配置                                        //
//*********************************************************************************//
/*软件IIC设置*/
#define TCHFG_SOFT_I2C_ENABLE               DISABLE_THIS_MOUDLE                     //软件IIC使能
#define TCFG_SW_I2C0_CLK_PORT               IO_PORTB_00                             //软件IIC  CLK脚选择
#define TCFG_SW_I2C0_DAT_PORT               IO_PORTB_03                             //软件IIC  DAT脚选择
#define TCFG_SW_I2C0_DELAY_CNT              50                                      //IIC延时参数，影响通讯时钟频率

//A组IO: SDA:  DM    SCL:  DP           B组IO: SDA: PC4    SCL: PC5
//C组IO: SDA: PB4    SCL: PB6           D组IO: SDA: PA5    SCL: PA6
#define TCHFG_HW_I2C_ENABLE                 DISABLE_THIS_MOUDLE                     //硬件IIC使能
#define TCFG_HW_I2C0_PORTS                  'B'                                     //选择第几组硬件引脚
#define TCFG_HW_I2C0_CLK                    100000                                  //硬件IIC波特率

//*********************************************************************************//
//                                 硬件SPI 配置                                        //
//*********************************************************************************//
#ifdef SPI1_RGB_DEV_EN
#define	TCFG_HW_SPI1_ENABLE		1  //ENABLE_THIS_MOUDLE
#else
#define	TCFG_HW_SPI1_ENABLE		0  //ENABLE_THIS_MOUDLE
#endif
//A组IO:    DI: PB2     DO: PB1     CLK: PB0
//B组IO:    DI: PC3     DO: PC5     CLK: PC4
#define TCFG_HW_SPI1_PORT		'A'
#define TCFG_HW_SPI1_BAUD		2500000L // 4000000L
#define TCFG_HW_SPI1_MODE		SPI_MODE_BIDIR_1BIT
#define TCFG_HW_SPI1_ROLE		SPI_ROLE_MASTER

#define	TCFG_HW_SPI2_ENABLE		0// ENABLE_THIS_MOUDLE
//A组IO:    DI: PB8     DO: PB10    CLK: PB9
//B组IO:    DI: PA13    DO: DM      CLK: DP
#define TCFG_HW_SPI2_PORT		'A'
#define TCFG_HW_SPI2_BAUD		2000000L
#define TCFG_HW_SPI2_MODE		SPI_MODE_BIDIR_1BIT
#define TCFG_HW_SPI2_ROLE		SPI_ROLE_MASTER

//*********************************************************************************//
//                                 FLASH 配置                                      //
//*********************************************************************************//
#define TCFG_NORFLASH_DEV_ENABLE				DISABLE_THIS_MOUDLE
#define TCFG_NANDFLASH_DEV_ENABLE		    DISABLE_THIS_MOUDLE
#define TCFG_FLASH_DEV_SPI_HW_NUM			1// 1: SPI1    2: SPI2
#define TCFG_FLASH_DEV_SPI_CS_PORT	    	IO_PORTA_03


//*********************************************************************************//
//                                  充电参数配置                                   //
//*********************************************************************************//
//是否支持芯片内置充电
#define TCFG_CHARGE_ENABLE					DISABLE_THIS_MOUDLE
//是否支持开机充电
#define TCFG_CHARGE_POWERON_ENABLE			DISABLE
//是否支持拔出充电自动开机功能
#define TCFG_CHARGE_OFF_POWERON_NE			DISABLE

#define TCFG_CHARGE_FULL_V					CHARGE_FULL_V_4202

#define TCFG_CHARGE_FULL_MA					CHARGE_FULL_mA_10

#define TCFG_CHARGE_MA						CHARGE_mA_60

//*********************************************************************************//
//                                 设备异步读 配置                                 //
//*********************************************************************************//

#define TCFG_DEVICE_BULK_READ_ASYNC_ENABLE

//*********************************************************************************//
//                                  SD 配置                                        //
//*********************************************************************************//
#define     SD_CMD_DECT 	0
#define     SD_CLK_DECT  	1
#define     SD_IO_DECT 		2
#define TCFG_SD0_ENABLE					0//	ENABLE_THIS_MOUDLE
//A组IO: CMD:PA9    CLK:PA10   DAT0:PA5    DAT1:PA6    DAT2:PA7    DAT3:PA8
//B组IO: CMD:PB10   CLK:PB9    DAT0:PB8    DAT1:PB6    DAT2:PB5    DAT3:PB4
#define TCFG_SD0_PORTS						'A'
#define TCFG_SD0_DAT_MODE					1
#define TCFG_SD0_DET_MODE					
#define TCFG_SD0_DET_IO 					IO_PORT_DM//当SD_DET_MODE为2时有效
#define TCFG_SD0_DET_IO_LEVEL				0//IO检查，0：低电平检测到卡。 1：高电平(外部电源)检测到卡。 2：高电平(SD卡电源)检测到卡。
#define TCFG_SD0_CLK						(3000000*4L)

#define TCFG_SD1_ENABLE					0 //	DISABLE_THIS_MOUDLE
//A组IO: CMD:PC4    CLK:PC5    DAT0:PC3    DAT1:PC2    DAT2:PC1    DAT3:PC0
//B组IO: CMD:PB5    CLK:PB6    DAT0:PB4    DAT1:PB8    DAT2:PB9    DAT3:PB10
#define TCFG_SD1_PORTS						'B'
#define TCFG_SD1_DAT_MODE					1
#define TCFG_SD1_DET_MODE					SD_CLK_DECT
#define TCFG_SD1_DET_IO 					IO_PORT_DM//当SD_DET_MODE为2时有效
#define TCFG_SD1_DET_IO_LEVEL				0//IO检查，0：低电平检测到卡。 1：高电平(外部电源)检测到卡。 2：高电平(SD卡电源)检测到卡。
#define TCFG_SD1_CLK						(3000000*4L)

//*********************************************************************************//
//                                 USB 配置                                        //
//*********************************************************************************//

#if defined SUPPORT_USB_DEBUG|| defined SUPPORT_EQ_ONLINE_SET_DM_DP
#define TCFG_PC_ENABLE                      0//PC模块使能
#define TCFG_UDISK_ENABLE					0//U盘模块使能
#else
#define TCFG_PC_ENABLE                      TCFG_APP_PC_EN//PC模块使能
#define TCFG_UDISK_ENABLE					ENABLE_THIS_MOUDLE//U盘模块使能
#endif

#define TCFG_OTG_USB_DEV_EN                 BIT(0)//USB0 = BIT(0)  USB1 = BIT(1)

#define TCFG_VIR_UDISK_ENABLE               0//ENABLE_THIS_MOUDLE

#define TCFG_USB_PORT_CHARGE            DISABLE

#define TCFG_USB_DM_MULTIPLEX_WITH_SD_DAT0       DISABLE

#if TCFG_USB_DM_MULTIPLEX_WITH_SD_DAT0
//复用情况下，如果使用此USB口作为充电（即LDO5V_IN连接到此USB口），
//TCFG_OTG_MODE需要或上TCFG_OTG_MODE_CHARGE，用来把charge从host区
//分开；否则不需要，如果LDO5V_IN与其他IO绑定，则不能或上
//TCFG_OTG_MODE_CHARGE
#define TCFG_DM_MULTIPLEX_WITH_SD_PORT      0//0:sd0  1:sd1 //dm 参与复用的sd配置
#undef TCFG_OTG_MODE
#define TCFG_OTG_MODE                       (TCFG_OTG_MODE_HOST|TCFG_OTG_MODE_SLAVE|TCFG_OTG_MODE_CHARGE|OTG_DET_DP_ONLY)

#undef USB_DEVICE_CLASS_CONFIG
#if TCFG_SD0_SD1_USE_THE_SAME_HW //开启了双卡的可以使能读卡器存续设备
#define     USB_DEVICE_CLASS_CONFIG (MASSSTORAGE_CLASS|SPEAKER_CLASS|MIC_CLASS|HID_CLASS)
#else
#define     USB_DEVICE_CLASS_CONFIG (SPEAKER_CLASS|MIC_CLASS|HID_CLASS)
#endif

#undef TCFG_SD0_DET_MODE
#define TCFG_SD0_DET_MODE					SD_CLK_DECT
#define TCFG_USB_SD_MULTIPLEX_IO            IO_PORTB_08

#endif

//*********************************************************************************//
//                                 fat_FLASH 配置                                      //
//*********************************************************************************//
#define TCFG_CODE_FLASH_ENABLE				DISABLE_THIS_MOUDLE

#define FLASH_INSIDE_REC_ENABLE             0

#if  TCFG_NORFLASH_DEV_ENABLE
#define TCFG_NOR_FAT                    1//ENABLE
#define TCFG_NOR_FS                     0//ENABLE
#define TCFG_NOR_REC                    1//ENABLE
#else
#define TCFG_NOR_FAT                    0//ENABLE
#define TCFG_NOR_FS                     0//ENABLE
#define TCFG_NOR_REC                    0//ENABLE
#endif



//*********************************************************************************//
//                                 key 配置                                        //
//*********************************************************************************//
//#define KEY_NUM_MAX                        	10
//#define KEY_NUM                            	3
#define KEY_IO_NUM_MAX						6
#define KEY_AD_NUM_MAX					10
#define KEY_IR_NUM_MAX						31
#define KEY_TOUCH_NUM_MAX					6
#define KEY_RDEC_NUM_MAX                    3
#define KEY_CTMU_TOUCH_NUM_MAX				6

#define MULT_KEY_ENABLE						DISABLE 		//是否使能组合按键消息, 使能后需要配置组合按键映射表

#define TCFG_KEY_TONE_EN					0//ENABLE		// 按键提示音。建议音频输出使用固定采样率

//*********************************************************************************//
//                                 iokey 配置                                      //
//*********************************************************************************//
#define TCFG_IOKEY_ENABLE					DISABLE_THIS_MOUDLE //是否使能IO按键

#define TCFG_IOKEY_POWER_CONNECT_WAY		ONE_PORT_TO_LOW    //按键一端接低电平一端接IO

#define TCFG_IOKEY_POWER_ONE_PORT			IO_PORTB_01        //IO按键端口

#define TCFG_IOKEY_PREV_CONNECT_WAY			ONE_PORT_TO_LOW  //按键一端接低电平一端接IO
#define TCFG_IOKEY_PREV_ONE_PORT			IO_PORTB_00

#define TCFG_IOKEY_NEXT_CONNECT_WAY 		ONE_PORT_TO_LOW  //按键一端接低电平一端接IO
#define TCFG_IOKEY_NEXT_ONE_PORT			IO_PORTB_02

//*********************************************************************************//
//                                 adkey 配置                                      //
//*********************************************************************************//
#define TCFG_ADKEY_ENABLE                   ENABLE_THIS_MOUDLE//是否使能AD按键
#define TCFG_ADKEY_LED_IO_REUSE				DISABLE_THIS_MOUDLE	//ADKEY 和 LED IO复用，led只能设置蓝灯显示
#define TCFG_ADKEY_PORT                     IO_PORTA_12  //AD按键端口(需要注意选择的IO口是否支持AD功能)
#define TCFG_ADKEY_AD_CHANNEL               AD_CH_PA12
#define TCFG_ADKEY_EXTERN_UP_ENABLE         ENABLE_THIS_MOUDLE //是否使用外部上拉

#if TCFG_ADKEY_EXTERN_UP_ENABLE
#define R_UP    220                 //22K，外部上拉阻值在此自行设置
#else
#define R_UP    100                 //10K，内部上拉默认10K
#endif
/*
录音 阻值3K
静音键 阻值6K2
话筒优先 阻值9K1
灯开关 阻值15K 取消长按功能
上一曲 阻值24K
下一曲 阻值33K
模式 阻值51K 

*/
#ifdef SUPPORT_KPH_ADKEY_FUNCTION

//#define ADKEY_22K_10KEY_MODE_JY_TX_2210

//按键电阻设置(如30为 3k)
#define KEY0_RES         1  // mode
#define KEY1_RES         10   //pre 
#define KEY2_RES         36   //light 
#define KEY3_RES         56 // repeat  
#define KEY4_RES         120 //pp
#define KEY5_RES         180 //  
#define KEY6_RES         270	// NEXT
#define KEY7_RES         470 // micmenu
#define KEY8_RES         1000 //  musicmenu 
#define KEY9_RES         5000 //  





//必须从小到大填电阻，没有则同VDDIO,填0x3ffL
#define TCFG_ADKEY_AD0      (KEY0_RES)
#define TCFG_ADKEY_AD1      (0x3ffL * KEY1_RES  / (KEY1_RES + R_UP))     //3
#define TCFG_ADKEY_AD2      (0x3ffL * KEY2_RES  / (KEY2_RES + R_UP))
#define TCFG_ADKEY_AD3      (0x3ffL * KEY3_RES  / (KEY3_RES + R_UP))
#define TCFG_ADKEY_AD4      (0x3ffL * KEY4_RES  / (KEY4_RES + R_UP))
#define TCFG_ADKEY_AD5      (0x3ffL * KEY5_RES  / (KEY5_RES + R_UP))
#define TCFG_ADKEY_AD6      (0x3ffL * KEY6_RES  / (KEY6_RES + R_UP))
#define TCFG_ADKEY_AD7      (0x3ffL * KEY7_RES  / (KEY7_RES + R_UP))
#define TCFG_ADKEY_AD8      (0x3ffL * KEY8_RES  / (KEY8_RES + R_UP))
#define TCFG_ADKEY_AD9      (0x3ffL * KEY9_RES  / (KEY9_RES + R_UP))
#define TCFG_ADKEY_VDDIO    (0x3ffL)
#else
#define TCFG_ADKEY_AD0      (0)                                 //0R
#define TCFG_ADKEY_AD1      (0x3ffL * 30   / (30   + R_UP))     //3k
#define TCFG_ADKEY_AD2      (0x3ffL * 62   / (62   + R_UP))     //6.2k
#define TCFG_ADKEY_AD3      (0x3ffL * 91   / (91   + R_UP))     //9.1k
#define TCFG_ADKEY_AD4      (0x3ffL * 150  / (150  + R_UP))     //15k
#define TCFG_ADKEY_AD5      (0x3ffL * 240  / (240  + R_UP))     //24k
#define TCFG_ADKEY_AD6      (0x3ffL * 330  / (330  + R_UP))     //33k
#define TCFG_ADKEY_AD7      (0x3ffL * 510  / (510  + R_UP))     //51k
#define TCFG_ADKEY_AD8      (0x3ffL * 1000 / (1000 + R_UP))     //100k
#define TCFG_ADKEY_AD9      (0x3ffL * 2200 / (2200 + R_UP))     //220k
#define TCFG_ADKEY_VDDIO    (0x3ffL)
#endif

#define TCFG_ADKEY_VOLTAGE0 ((TCFG_ADKEY_AD0 + TCFG_ADKEY_AD1) / 2)
#define TCFG_ADKEY_VOLTAGE1 ((TCFG_ADKEY_AD1 + TCFG_ADKEY_AD2) / 2)
#define TCFG_ADKEY_VOLTAGE2 ((TCFG_ADKEY_AD2 + TCFG_ADKEY_AD3) / 2)
#define TCFG_ADKEY_VOLTAGE3 ((TCFG_ADKEY_AD3 + TCFG_ADKEY_AD4) / 2)
#define TCFG_ADKEY_VOLTAGE4 ((TCFG_ADKEY_AD4 + TCFG_ADKEY_AD5) / 2)
#define TCFG_ADKEY_VOLTAGE5 ((TCFG_ADKEY_AD5 + TCFG_ADKEY_AD6) / 2)
#define TCFG_ADKEY_VOLTAGE6 ((TCFG_ADKEY_AD6 + TCFG_ADKEY_AD7) / 2)
#define TCFG_ADKEY_VOLTAGE7 ((TCFG_ADKEY_AD7 + TCFG_ADKEY_AD8) / 2)
#define TCFG_ADKEY_VOLTAGE8 ((TCFG_ADKEY_AD8 + TCFG_ADKEY_AD9) / 2)
#define TCFG_ADKEY_VOLTAGE9 ((TCFG_ADKEY_AD9 + TCFG_ADKEY_VDDIO) / 2)

#define TCFG_ADKEY_VALUE0                   0
#define TCFG_ADKEY_VALUE1                   1
#define TCFG_ADKEY_VALUE2                   2
#define TCFG_ADKEY_VALUE3                   3
#define TCFG_ADKEY_VALUE4                   4
#define TCFG_ADKEY_VALUE5                   5
#define TCFG_ADKEY_VALUE6                   6
#define TCFG_ADKEY_VALUE7                   7
#define TCFG_ADKEY_VALUE8                   8
#define TCFG_ADKEY_VALUE9                   9


#ifdef SUPPORT_KPH_ADKEY_FUNCTION
#define USE_DEFINE_KEY_TABL      TCFG_ADKEY_VOLTAGE0,\
								 TCFG_ADKEY_VOLTAGE1,\
								 TCFG_ADKEY_VOLTAGE2,\
								 TCFG_ADKEY_VOLTAGE3,\
								 TCFG_ADKEY_VOLTAGE4,\
								 TCFG_ADKEY_VOLTAGE5,\
								 TCFG_ADKEY_VOLTAGE6,\
								 TCFG_ADKEY_VOLTAGE7,\
								 TCFG_ADKEY_VOLTAGE8,\
								 TCFG_ADKEY_VOLTAGE9,
#endif


//*********************************************************************************//
//                                 irkey 配置                                      //
//*********************************************************************************//
#define TCFG_IRKEY_ENABLE                   1 //DISABLE_THIS_MOUDLE//是否使能ir按键
#define TCFG_IRKEY_PORT                     IO_PORTA_05//IR按键端口 PA8

//*********************************************************************************//
//                             tocuh key 配置                                      //
//*********************************************************************************//
#define TCFG_TOUCH_KEY_ENABLE 				DISABLE_THIS_MOUDLE 		//是否使能触摸按键

/* 触摸按键计数参考时钟选择, 频率越高, 精度越高
** 可选参数:
	1.TOUCH_KEY_OSC_CLK,
    2.TOUCH_KEY_MUX_IN_CLK,  //外部输入, ,一般不用, 保留
    3.TOUCH_KEY_PLL_192M_CLK,
    4.TOUCH_KEY_PLL_240M_CLK,
*/
#define TCFG_TOUCH_KEY_CLK 					TOUCH_KEY_PLL_192M_CLK 	//触摸按键时钟配置
#define TCFG_TOUCH_KEY_CHANGE_GAIN 			4 	//变化放大倍数, 一般固定
#define TCFG_TOUCH_KEY_PRESS_CFG 			-100//触摸按下灵敏度, 类型:s16, 数值越大, 灵敏度越高
#define TCFG_TOUCH_KEY_RELEASE_CFG0 		-50 //触摸释放灵敏度0, 类型:s16, 数值越大, 灵敏度越高
#define TCFG_TOUCH_KEY_RELEASE_CFG1 		-80 //触摸释放灵敏度1, 类型:s16, 数值越大, 灵敏度越高

//key0配置
#define TCFG_TOUCH_KEY0_PORT 				IO_PORTB_06  //触摸按键IO配置
#define TCFG_TOUCH_KEY0_VALUE 				1 		 	 //触摸按键key0 按键值

//key1配置
#define TCFG_TOUCH_KEY1_PORT 				IO_PORTB_07  //触摸按键key1 IO配置
#define TCFG_TOUCH_KEY1_VALUE 				2 		 	 //触摸按键key1按键值

//*********************************************************************************//
//                            ctmu tocuh key 配置                                      //
//*********************************************************************************//
#define TCFG_CTMU_TOUCH_KEY_ENABLE              DISABLE_THIS_MOUDLE             //是否使能CTMU触摸按键
#define TCFG_CTMU_TOUCH_KEY_PRESS_CFG 		   	40//按下灵敏度（s16）,数值越小, 灵敏度越高，一般设置30-100
#define TCFG_CTMU_TOUCH_KEY_RELEASE_CFG0 		10 //释放灵敏度0（s16），数值越小，灵敏度越高，必须比按下灵敏度小
#define TCFG_CTMU_TOUCH_KEY_RELEASE_CFG1 		160 //释放灵敏度1（s16）, 数值越小, 灵敏度越高，一般只需要调节上面两个

//key0配置
#define TCFG_CTMU_TOUCH_KEY0_PORT 				IO_PORTA_00  //触摸按键key0 IO配置
#define TCFG_CTMU_TOUCH_KEY0_VALUE 				0 		 	 //触摸按键key0 按键值

//key1配置
#define TCFG_CTMU_TOUCH_KEY1_PORT 				IO_PORTA_01  //触摸按键key1 IO配置
#define TCFG_CTMU_TOUCH_KEY1_VALUE 				1 		 	 //触摸按键key1 按键值

//*********************************************************************************//
//                                 rdec_key 配置                                      //
//*********************************************************************************//
#define TCFG_RDEC_KEY_ENABLE				    DISABLE_THIS_MOUDLE //是否使能RDEC按键
//默认硬件引脚配置
//RDEC0配置
#define TCFG_RDEC0_ECODE1_PORT					IO_PORTA_02
#define TCFG_RDEC0_ECODE2_PORT					IO_PORTA_03
#define TCFG_RDEC0_KEY0_VALUE 				 	0
#define TCFG_RDEC0_KEY1_VALUE 				 	1

//RDEC1配置
#define TCFG_RDEC1_ECODE1_PORT					IO_PORTB_02
#define TCFG_RDEC1_ECODE2_PORT					IO_PORTB_03
#define TCFG_RDEC1_KEY0_VALUE 				 	2
#define TCFG_RDEC1_KEY1_VALUE 				 	3

//RDEC2配置
#define TCFG_RDEC2_ECODE1_PORT					IO_PORTB_04
#define TCFG_RDEC2_ECODE2_PORT					IO_PORTB_06
#define TCFG_RDEC2_KEY0_VALUE 				 	4
#define TCFG_RDEC2_KEY1_VALUE 				 	5


//*********************************************************************************//
//                                 Audio配置                                       //
//*********************************************************************************//
#define TCFG_AUDIO_ADC_ENABLE				ENABLE_THIS_MOUDLE
//MIC只有一个声道，固定选择右声道
#define TCFG_AUDIO_ADC_MIC_CHA				LADC_CH_MIC_R
//省电容MIC使能
#define TCFG_MIC_CAPLESS_ENABLE				DISABLE_THIS_MOUDLE
/*MIC LDO电流档位设置：
    0:0.625ua    1:1.25ua    2:1.875ua    3:2.5ua*/
#define TCFG_AUDIO_ADC_LDO_SEL				2

// LADC通道
#define TCFG_AUDIO_ADC_LINE_CHA0			LADC_LINE1_MASK
#define TCFG_AUDIO_ADC_LINE_CHA1			LADC_CH_LINE0_L

#define TCFG_AUDIO_DAC_ENABLE				ENABLE_THIS_MOUDLE
#define TCFG_AUDIO_DAC_LDO_SEL				1

#define TCFG_AUDIO_DAC_LDO_VOLT				DACVDD_LDO_2_90V
/*预留接口，未使用*/
#define TCFG_AUDIO_DAC_PA_PORT				NO_CONFIG_PORT
/*
DAC硬件上的连接方式,可选的配置：
    DAC_OUTPUT_MONO_L               左声道
    DAC_OUTPUT_MONO_R               右声道
    DAC_OUTPUT_LR                   立体声
    DAC_OUTPUT_MONO_LR_DIFF         单声道差分输出
    DAC_OUTPUT_FRONT_LR_REAR_LR    四声道输出
*/
//#define TCFG_AUDIO_DAC_CONNECT_MODE    DAC_OUTPUT_FRONT_LR_REAR_LR
#define TCFG_AUDIO_DAC_CONNECT_MODE   DAC_OUTPUT_LR //DAC_OUTPUT_LR
// #define TCFG_AUDIO_DAC_CONNECT_MODE    DAC_OUTPUT_MONO_LR_DIFF

/*通话降噪模式配置*/
#define CVP_ANS_MODE	0	/*传统降噪*/
#define CVP_DNS_MODE	1	/*神经网络降噪*/
#define TCFG_AUDIO_CVP_NS_MODE				CVP_ANS_MODE

#define AUDIO_OUTPUT_WAY_DAC        0
#define AUDIO_OUTPUT_WAY_IIS        1
#define AUDIO_OUTPUT_WAY_FM         2
#define AUDIO_OUTPUT_WAY_HDMI       3
#define AUDIO_OUTPUT_WAY_SPDIF      4
#define AUDIO_OUTPUT_WAY_BT      	5	// bt emitter
#define AUDIO_OUTPUT_WAY_DAC_IIS    6
#define AUDIO_OUTPUT_WAY_DONGLE		7
#define AUDIO_OUTPUT_WAY            AUDIO_OUTPUT_WAY_DAC
#define LINEIN_INPUT_WAY            LINEIN_INPUT_WAY_ADC //LINEIN_INPUT_WAY_ANALOG

#define AUDIO_OUTPUT_AUTOMUTE       ENABLE
#define DAC_AUTO_HIGH_Z_EN          DISABLE         //处理直推串音问题, 隔直不要开

/*
 *系统音量类型选择
 *软件数字音量是指纯软件对声音进行运算后得到的
 *硬件数字音量是指dac内部数字模块对声音进行运算后输出
 */
#define VOL_TYPE_DIGITAL		0	//软件数字音量
#define VOL_TYPE_ANALOG			1	//硬件模拟音量
#define VOL_TYPE_AD				2	//联合音量(模拟数字混合调节)
#define VOL_TYPE_DIGITAL_HW		3  	//硬件数字音量
#define VOL_TYPE_DIGGROUP       4   //独立通道数字音量

//每个解码通道都开启数字音量管理,音量类型为VOL_TYPE_DIGGROUP时要使能
#define SYS_DIGVOL_GROUP_EN     1 // DISABLE

#define SYS_VOL_TYPE            VOL_TYPE_DIGGROUP//VOL_TYPE_ANALOG//
/*
 *通话的时候使用数字音量
 *0：通话使用和SYS_VOL_TYPE一样的音量调节类型
 *1：通话使用数字音量调节，更加平滑
 */
 
#define TCFG_CALL_USE_DIGITAL_VOLUME		0

// 使能改宏，提示音音量使用music音量
#define APP_AUDIO_STATE_WTONE_BY_MUSIC      (0)
// 0:提示音不使用默认音量； 1:默认提示音音量值
#define TONE_MODE_DEFAULE_VOLUME            (6)

//AudioEffects代码链接管理
#define AUDIO_EFFECTS_DRC_AT_RAM			1
#define AUDIO_EFFECTS_REVERB_AT_RAM			0
#define AUDIO_EFFECTS_ECHO_AT_RAM			0
#define AUDIO_EFFECTS_AFC_AT_RAM			0
#define AUDIO_EFFECTS_EQ_AT_RAM				1
#define AUDIO_EFFECTS_NOISEGATE_AT_RAM		0
#define AUDIO_EFFECTS_MIC_EFFECT_AT_RAM		0
#define AUDIO_EFFECTS_MIC_STREAM_AT_RAM		0
#define AUDIO_EFFECTS_DEBUG_AT_RAM			0
#define AUDIO_EFFECTS_PITCHSHIFTER_AT_RAM   0
#define AUDIO_EFFECTS_CH_SWICTH_AT_RAM  	1
#define AUDIO_EFFECTS_VOCAL_REMOVER_AT_RAM  1

//*********************************************************************************//
//                                  充电仓配置                                     //
//*********************************************************************************//
#define TCFG_CHARGESTORE_ENABLE				DISABLE_THIS_MOUDLE       //是否支持智能充点仓
#define TCFG_TEST_BOX_ENABLE			    0
#define TCFG_CHARGESTORE_PORT				IO_PORTA_02               //耳机和充点仓通讯的IO口
#define TCFG_CHARGESTORE_UART_ID			IRQ_UART1_IDX             //通讯使用的串口号

#ifdef AUDIO_PCM_DEBUG
#ifdef	TCFG_TEST_BOX_ENABLE
#undef 	TCFG_TEST_BOX_ENABLE
#define TCFG_TEST_BOX_ENABLE				0		//因为使用PCM使用到了串口1
#endif
#endif/*AUDIO_PCM_DEBUG*/

//*********************************************************************************//
//                                  LED 配置                                       //
//******************************************************************************
#if TCFG_ADKEY_LED_IO_REUSE
//打开ADKEY和LED IO复用功能，LED使用ADKEY_IO
#define TCFG_PWMLED_ENABLE					ENABLE_THIS_MOUDLE			//是否支持PMW LED推灯模块
#define TCFG_PWMLED_IOMODE					LED_ONE_IO_MODE				//LED模式，单IO还是两个IO推灯
#define TCFG_PWMLED_PIN						TCFG_ADKEY_PORT						//LED使用的IO口

#else

#define TCFG_PWMLED_ENABLE					DISABLE_THIS_MOUDLE			//是否支持PMW LED推灯模块
#define TCFG_PWMLED_IOMODE					LED_ONE_IO_MODE				//LED模式，单IO还是两个IO推灯
#define TCFG_PWMLED_PIN						IO_PORTB_06					//LED使用的IO口 注意和led7是否有io冲突

#endif
//*********************************************************************************//
//                                  UI 配置                                        //
//*********************************************************************************//
#define TCFG_UI_ENABLE 						ENABLE_THIS_MOUDLE 	//UI总开关
#define CONFIG_UI_STYLE                     STYLE_JL_LED7
#define TCFG_UI_LED7_ENABLE 			 	ENABLE_THIS_MOUDLE 	//UI使用LED7显示
// #define TCFG_UI_LCD_SEG3X9_ENABLE 		ENABLE_THIS_MOUDLE 	//UI使用LCD段码屏显示
// #define TCFG_LCD_ST7735S_ENABLE	        ENABLE_THIS_MOUDLE
// #define TCFG_LCD_ST7789VW_ENABLE	        ENABLE_THIS_MOUDLE
#define TCFG_SPI_LCD_ENABLE                 DISABLE_THIS_MOUDLE //spi lcd开关
#define TCFG_TFT_LCD_DEV_SPI_HW_NUM			 1// 1: SPI1    2: SPI2 配置lcd选择的spi口

#define TCFG_LED7_RUN_RAM 					1 //DISABLE_THIS_MOUDLE 	//led7跑ram 不屏蔽中断(需要占据2k附近ram)

//*********************************************************************************//
//                                  时钟配置                                       //
//*********************************************************************************//
#define TCFG_CLOCK_SYS_SRC					SYS_CLOCK_INPUT_PLL_BT_OSC   //系统时钟源选择
#define TCFG_CLOCK_SYS_HZ					24000000                     //系统时钟设置
#define TCFG_CLOCK_OSC_HZ					24000000                     //外界晶振频率设置
#define TCFG_CLOCK_MODE                     CLOCK_MODE_ADAPTIVE

//*********************************************************************************//
//                                  低功耗配置                                     //
//*********************************************************************************//
#define TCFG_LOWPOWER_POWER_SEL				PWR_LDO15                    //电源模式设置，可选DCDC和LDO
#define TCFG_LOWPOWER_BTOSC_DISABLE			0                            //低功耗模式下BTOSC是否保持
#define TCFG_LOWPOWER_LOWPOWER_SEL			0//SLEEP_EN                     //SNIFF状态下芯片是否进入powerdown


#define TCFG_LOWPOWER_VDDIOM_LEVEL			VDDIOM_VOL_34V

#define TCFG_LOWPOWER_VDDIOW_LEVEL			VDDIOW_VOL_28V               //弱VDDIO等级配置


//*********************************************************************************//
//                                  EQ配置                                         //
//*********************************************************************************//
#define TCFG_EQ_ENABLE                      	1     //支持EQ功能,EQ总使能
#if TCFG_EQ_ENABLE
#define TCFG_BT_MUSIC_EQ_ENABLE             	1     //支持蓝牙音乐EQ
#define TCFG_PHONE_EQ_ENABLE                		1     //支持通话近端EQ
#define TCFG_MUSIC_MODE_EQ_ENABLE           	1     //支持音乐模式EQ
#define TCFG_LINEIN_MODE_EQ_ENABLE          	1     //支持linein近端EQ
#define TCFG_FM_MODE_EQ_ENABLE              	1     //支持fm模式EQ
#define TCFG_SPDIF_MODE_EQ_ENABLE           	1     //支持SPDIF模式EQ
#define TCFG_PC_MODE_EQ_ENABLE              	1     //支持pc模式EQ
#define TCFG_AUDIO_OUT_EQ_ENABLE			1 	  //高低音EQ
#define TCFG_AUDIO_MIC_EFFECT_POST_EQ_ENABLE	0	  //MIC音效后置EQ使能(includes DRC)

#define EQ_SECTION_MAX                      10 //eq 段数
#define TCFG_DYNAMIC_EQ_ENABLE              0     //动态eq使能，接在eq后，需输入32bit位宽数据
#endif//TCFG_EQ_ENABLE

#define TCFG_DRC_ENABLE						1 	  //DRC 总使能
#define TCFG_AUDIO_MDRC_ENABLE              0     //多带drc使能  0:关闭多带drc，  1：使能多带drc  2：使能多带drc 并且 多带drc后再做一次全带的drc

#if TCFG_DRC_ENABLE
#define TCFG_BT_MUSIC_DRC_ENABLE            1     //支持蓝牙音乐DRC
#define TCFG_MUSIC_MODE_DRC_ENABLE          1     //支持音乐模式DRC
#define TCFG_LINEIN_MODE_DRC_ENABLE         1     //支持LINEIN模式DRC
#define TCFG_FM_MODE_DRC_ENABLE             0     //支持FM模式DRC
#define TCFG_SPDIF_MODE_DRC_ENABLE          1     //支持SPDIF模式DRC
#define TCFG_PC_MODE_DRC_ENABLE             1     //支持PC模式DRC
#define TCFG_AUDIO_OUT_DRC_ENABLE			0 	  //高低音EQ后的drc
#endif//TCFG_DRC_ENABLE

#define LINEIN_MODE_SOLE_EQ_EN              0  //linein模式是否需要独立的eq与drc

//*********************************************************************************//
//                          新音箱配置工具 && 调音工具                             //
//*********************************************************************************//
#if defined SUPPORT_EQ_ONLINE_SET_DM_DP||defined SUPPORT_MIC_EQ_ONLINE_SET_DM_DP
#define TCFG_SOUNDBOX_TOOL_ENABLE			1 //DISABLE		  	//是否支持音箱在线配置工具
#define TCFG_EFFECT_TOOL_ENABLE			1 //	DISABLE		  	//是否支持在线音效调试,使能该项还需使能EQ总使能TCFG_EQ_ENABL,
#else
#define TCFG_SOUNDBOX_TOOL_ENABLE			0 //DISABLE		  	//是否支持音箱在线配置工具
#define TCFG_EFFECT_TOOL_ENABLE			0//	DISABLE		  	//是否支持在线音效调试,使能该项还需使能EQ总使能TCFG_EQ_ENABL,
#endif

#define TCFG_NULL_COMM						0				//不支持通信
#define TCFG_UART_COMM						1				//串口通信
#define TCFG_USB_COMM						2				//USB通信
#if (TCFG_SOUNDBOX_TOOL_ENABLE || TCFG_EFFECT_TOOL_ENABLE)
#ifdef SUPPORT_EQ_ONLINE_USB
#define TCFG_COMM_TYPE						TCFG_USB_COMM	
#else
#define TCFG_COMM_TYPE						TCFG_UART_COMM
#endif
#else
#define TCFG_COMM_TYPE						TCFG_NULL_COMM
#endif
#define TCFG_TOOL_TX_PORT					IO_PORT_DP      //UART模式调试TX口选择
#define TCFG_TOOL_RX_PORT					IO_PORT_DM      //UART模式调试RX口选择
#define TCFG_ONLINE_ENABLE                  (TCFG_EFFECT_TOOL_ENABLE)    //是否支持音效在线调试功能

/***********************************非用户配置区***********************************/
#if (TCFG_SOUNDBOX_TOOL_ENABLE || TCFG_ONLINE_ENABLE)
#if TCFG_COMM_TYPE == TCFG_UART_COMM
#undef TCFG_UDISK_ENABLE
#define TCFG_UDISK_ENABLE 					0
#undef TCFG_SD0_PORTS
#define TCFG_SD0_PORTS 					   'B'
#endif
#endif

#include "usb_std_class_def.h"
#if (TCFG_SOUNDBOX_TOOL_ENABLE || TCFG_EFFECT_TOOL_ENABLE)
#if (TCFG_COMM_TYPE == TCFG_USB_COMM)
#define TCFG_USB_CDC_BACKGROUND_RUN         ENABLE
#if (TCFG_USB_CDC_BACKGROUND_RUN && (!TCFG_PC_ENABLE))
#define TCFG_OTG_USB_DEV_EN                 0
#endif
#endif
#if (TCFG_COMM_TYPE == TCFG_UART_COMM)
#define TCFG_USB_CDC_BACKGROUND_RUN         DISABLE
#endif
#endif

/**********************************************************************************/


//*********************************************************************************//
//                                  混响配置                                   //
//*********************************************************************************//
#ifdef SUPPORT_MIC_ECHO
#define TCFG_MIC_EFFECT_ENABLE       1 //DISABLE
#else
#define TCFG_MIC_EFFECT_ENABLE       DISABLE
#endif
#define TCFG_MIC_EFFECT_START_DIR    0 //开机直接启动混响

#define MIC_EFFECT_REVERB             1//BIT(0)
#define MIC_EFFECT_ECHO               2//BIT(1)
#define MIC_EFFECT_REVERB_ECHO        3//(BIT(1)|BIT(0))
#define MIC_EFFECT_MEGAPHONE          4//BIT(2)
// #define TCFG_MIC_EFFECT_SEL           MIC_EFFECT_REVERB
#define TCFG_MIC_EFFECT_SEL           MIC_EFFECT_ECHO
//#define TCFG_MIC_TUNNING_EQ_ENABLE    1    // mic  bass treb  


#define TCFG_MIC_VOICE_CHANGER_ENABLE       0  //  DISABLE  //变声模块使能

/***********************************非用户配置区***********************************/
#if TCFG_MIC_EFFECT_ENABLE
#undef EQ_SECTION_MAX
#define EQ_SECTION_MAX 25
#ifdef TCFG_AEC_ENABLE
#undef TCFG_AEC_ENABLE
#define TCFG_AEC_ENABLE 0
#endif//TCFG_AEC_ENABLE

#if TCFG_DYNAMIC_EQ_ENABLE
#undef TCFG_DYNAMIC_EQ_ENABLE
#define TCFG_DYNAMIC_EQ_ENABLE 0
#endif

#if TCFG_AUDIO_MDRC_ENABLE
#undef TCFG_AUDIO_MDRC_ENABLE
#define TCFG_AUDIO_MDRC_ENABLE 0
#endif
#if !TCFG_EQ_ENABLE
#undef TCFG_EQ_ENABLE
#define TCFG_EQ_ENABLE 1//混响必开eq
#endif
#if !TCFG_DRC_ENABLE
#undef TCFG_DRC_ENABLE
#define TCFG_DRC_ENABLE 1//混响必开drc
#endif

#endif
/**********************************************************************************/

#define TCFG_MIC_DODGE_EN  1 //闪避是否使能


#define TCFG_REVERB_SAMPLERATE_DEFUAL  (44100)
#define MIC_EFFECT_SAMPLERATE			(44100L)

#if TCFG_MIC_EFFECT_ENABLE
#undef MIC_SamplingFrequency
#define     MIC_SamplingFrequency         1
#undef MIC_AUDIO_RATE
#define     MIC_AUDIO_RATE              MIC_EFFECT_SAMPLERATE
#undef  SPK_AUDIO_RATE
#define SPK_AUDIO_RATE                  TCFG_REVERB_SAMPLERATE_DEFUAL
#endif


#define TCFG_LOUDSPEAKER_ENABLE            DISABLE //不能与TCFG_MIC_EFFECT_ENABLE同时打开
#define TCFG_USB_MIC_ECHO_ENABLE           DISABLE //
#define TCFG_USB_MIC_DATA_FROM_MICEFFECT   DISABLE //
#define TCFG_KARAOKE_EARPHONE              DISABLE

//*********************************************************************************//
//                                  g-sensor配置                                   //
//*********************************************************************************//
#define TCFG_GSENSOR_ENABLE                       0     //gSensor使能
#define TCFG_DA230_EN                             0
#define TCFG_SC7A20_EN                            0
#define TCFG_STK8321_EN                           0
#define TCFG_GSENOR_USER_IIC_TYPE                 0     //0:软件IIC  1:硬件IIC

//*********************************************************************************//
//                                  系统配置                                         //
//*********************************************************************************//
#define TCFG_AUTO_SHUT_DOWN_TIME		    0   //没有蓝牙连接自动关机时间
#define TCFG_SYS_LVD_EN						1   //电量检测使能
#define TCFG_POWER_ON_NEED_KEY				0	  //是否需要按按键开机配置
#define TWFG_APP_POWERON_IGNORE_DEV         4000//上电忽略挂载设备，0时不忽略，非0则n毫秒忽略

#define TCFG_AUDIO_DEC_OUT_TASK				0	// 解码使用单独任务做输出

//*********************************************************************************//
//                                  蓝牙配置                                       //
//*********************************************************************************//
#define TCFG_USER_TWS_ENABLE                1   //tws功能使能
#define TCFG_USER_BLE_ENABLE                0   //BLE功能使能
#define TCFG_USER_BT_CLASSIC_ENABLE         1   //经典蓝牙功能使能
#define TCFG_BT_SUPPORT_AAC                 0   //AAC格式支持
#define TCFG_USER_EMITTER_ENABLE            0   //emitter功能使能
#define TCFG_BT_SNIFF_ENABLE                0   //bt sniff 功能使能

#define USER_SUPPORT_PROFILE_SPP    1
#define USER_SUPPORT_PROFILE_HFP    0
#define USER_SUPPORT_PROFILE_A2DP   1
#define USER_SUPPORT_PROFILE_AVCTP  1
#define USER_SUPPORT_PROFILE_HID    1
#define USER_SUPPORT_PROFILE_PNP    1
#define USER_SUPPORT_PROFILE_PBAP   0

#define USER_SUPPORT_DUAL_A2DP_SOURCE              0

#if TCFG_USER_TWS_ENABLE
#define TCFG_BD_NUM						    1   //连接设备个数配置
#define TCFG_AUTO_STOP_PAGE_SCAN_TIME       0   //配置一拖二第一台连接后自动关闭PAGE SCAN的时间(单位分钟)
#define TCFG_USER_ESCO_SLAVE_MUTE           0   //对箱通话slave出声音
#else
#define TCFG_BD_NUM						    1   //连接设备个数配置
#define TCFG_AUTO_STOP_PAGE_SCAN_TIME       0 //配置一拖二第一台连接后自动关闭PAGE SCAN的时间(单位分钟)
#define TCFG_USER_ESCO_SLAVE_MUTE           0   //对箱通话slave出声音
#endif

#define BT_INBAND_RINGTONE                  0   //是否播放手机自带来电铃声
#define BT_PHONE_NUMBER                     0   //是否播放来电报号
#define BT_SYNC_PHONE_RING                  0   //是否TWS同步播放来电铃声
#define BT_SUPPORT_DISPLAY_BAT              0   //是否使能电量检测
#define BT_SUPPORT_MUSIC_VOL_SYNC           0   //是否使能音量同步

#define TCFG_BLUETOOTH_BACK_MODE			0	//后台模式

#if (TCFG_USER_TWS_ENABLE && TCFG_BLUETOOTH_BACK_MODE) && (TCFG_BT_SNIFF_ENABLE==0) && defined(CONFIG_LOCAL_TWS_ENABLE)
#define TCFG_DEC2TWS_ENABLE					1	// 本地解码转发
#define TCFG_PCM_ENC2TWS_ENABLE				1	// pcm编码转发
#define TCFG_PCM2TWS_SBC_ENABLE				1	// pcm转发采样sbc编码
#define TCFG_TONE2TWS_ENABLE				1	// 提示音转发
#else
#define TCFG_DEC2TWS_ENABLE					0
#define TCFG_PCM_ENC2TWS_ENABLE				0
#define TCFG_PCM2TWS_SBC_ENABLE				0
#define TCFG_TONE2TWS_ENABLE				0
#endif


#if (APP_ONLINE_DEBUG && !USER_SUPPORT_PROFILE_SPP)
#error "NEED ENABLE USER_SUPPORT_PROFILE_SPP!!!"
#endif


//*********************************************************************************//
//                                  REC 配置                                       //
//*********************************************************************************//
#define RECORDER_MIX_EN						DISABLE//混合录音使能
#define TCFG_RECORD_FOLDER_DEV_ENABLE       DISABLE//音乐播放录音区分使能

#define TCFG_USB_MIC_ECHO_TO_REC


#if RECORDER_MIX_EN//限制不能同时打开
#ifdef TCFG_AEC_ENABLE
#undef TCFG_AEC_ENABLE
#define TCFG_AEC_ENABLE 0
#endif//TCFG_AEC_ENABLE
#endif

//                                  linein配置                                     //
//*********************************************************************************//
#define TCFG_LINEIN_ENABLE					TCFG_APP_LINEIN_EN	// linein使能
// #define TCFG_LINEIN_LADC_IDX				0					// linein使用的ladc通道，对应ladc_list
#ifdef AUX_USER_AUX2_L
#define TCFG_LINEIN_LR_CH					AUDIO_LIN2L_CH
#elif defined AUX_USER_DAC_R
#define TCFG_LINEIN_LR_CH					AUDIO_LIN_DACR_CH
#else //AUX_USER_AUX2_LR
#define TCFG_LINEIN_LR_CH					AUDIO_LIN2_LR
#endif
#define TCFG_LINEIN_CHECK_PORT				NO_CONFIG_PORT			// linein检测IO
#define TCFG_LINEIN_PORT_UP_ENABLE        	0					// 检测IO上拉使能
#define TCFG_LINEIN_PORT_DOWN_ENABLE       	0					// 检测IO下拉使能
#define TCFG_LINEIN_AD_CHANNEL             	NO_CONFIG_PORT		// 检测IO是否使用AD检测
#define TCFG_LINEIN_VOLTAGE                	0					// AD检测时的阀值
#define TCFG_LINEIN_INPUT_WAY               LINEIN_INPUT_WAY
#define TCFG_LINEIN_MULTIPLEX_WITH_SD		DISABLE 				// linein 检测与 SD cmd 复用
#define TCFG_LINEIN_SD_PORT                 0// 0：sd0 1：sd1       //选择复用的sd口
#define TCFG_LINEIN_DETECT_ENABLE    		0
//*********************************************************************************//
//                                  music 配置                                     //
//*********************************************************************************//
#define TCFG_DEC_G729_ENABLE               	ENABLE
#define TCFG_DEC_MP3_ENABLE					ENABLE
#define TCFG_DEC_WMA_ENABLE					ENABLE
#define TCFG_DEC_WAV_ENABLE					ENABLE
#define TCFG_DEC_FLAC_ENABLE				ENABLE
#define TCFG_DEC_APE_ENABLE					ENABLE
#define TCFG_DEC_M4A_ENABLE					0//ENABLE
#define TCFG_DEC_ALAC_ENABLE					0//ENABLE
#define TCFG_DEC_AMR_ENABLE					0//ENABLE
#define TCFG_DEC_DTS_ENABLE					0//ENABLE
#define TCFG_DEC_G726_ENABLE			    		DISABLE
#define TCFG_DEC_MIDI_ENABLE			    		DISABLE
#define TCFG_DEC_MTY_ENABLE					DISABLE
#define TCFG_DEC_SBC_ENABLE					ENABLE
#define TCFG_DEC_PCM_ENABLE					ENABLE
#define TCFG_DEC_CVSD_ENABLE					ENABLE
#define TCFG_DEC_WTGV2_ENABLE				DISABLE

#define TCFG_DEC_ID3_V1_ENABLE				DISABLE
#define TCFG_DEC_ID3_V2_ENABLE				DISABLE
#define TCFG_DEC_DECRYPT_ENABLE				DISABLE
#define TCFG_DEC_DECRYPT_KEY				(0x12345678)

////<变速变调
#define TCFG_SPEED_PITCH_ENABLE             DISABLE//
//*********************************************************************************//
//                                  fm 配置                                     //
//*********************************************************************************//
#define TCFG_FM_ENABLE							TCFG_APP_FM_EN // fm 使能
#define TCFG_FM_INSIDE_ENABLE					ENABLE
#define TCFG_FM_RDA5807_ENABLE					DISABLE //需要使能软件IIC模块
#define TCFG_FM_BK1080_ENABLE					DISABLE //需要使能软件IIC模块
#define TCFG_FM_QN8035_ENABLE					DISABLE //需要使能软件IIC模块

#define TCFG_FMIN_LADC_IDX				1				// linein使用的ladc通道，对应ladc_list
#define TCFG_FMIN_LR_CH					AUDIO_LIN1_LR
#define TCFG_FM_INPUT_WAY               LINEIN_INPUT_WAY_ANALOG

#if (TCFG_FM_INSIDE_ENABLE && TCFG_FM_ENABLE)
#if (((TCFG_USER_TWS_ENABLE) || (RECORDER_MIX_EN) || (TCFG_MIC_EFFECT_ENABLE)))
#define TCFG_CODE_RUN_RAM_FM_MODE 					ENABLE_THIS_MOUDLE  	//FM模式 代码跑ram
#else
#define TCFG_CODE_RUN_RAM_FM_MODE 					ENABLE_THIS_MOUDLE  	//FM模式 代码跑ram
#endif
#else
#define TCFG_CODE_RUN_RAM_FM_MODE 					DISABLE_THIS_MOUDLE 	//FM模式 代码跑ram
#endif /*(TCFG_FM_INSIDE_ENABLE && TCFG_FM_ENABLE)*/

#if (TCFG_CODE_RUN_RAM_FM_MODE && TCFG_UI_ENABLE)
#undef TCFG_LED7_RUN_RAM
#define TCFG_LED7_RUN_RAM 					ENABLE_THIS_MOUDLE 	//led7跑ram 不屏蔽中断(需要占据2k附近ram)
#endif /*(TCFG_CODE_RUN_RAM_FM_MODE && TCFG_UI_ENABLE)*/

//*********************************************************************************//
//                                  fm emitter 配置                                     //
//*********************************************************************************//
#define TCFG_APP_FM_EMITTER_EN                  DISABLE_THIS_MOUDLE
#define TCFG_FM_EMITTER_INSIDE_ENABLE			DISABLE
#define TCFG_FM_EMITTER_AC3433_ENABLE			DISABLE
#define TCFG_FM_EMITTER_QN8007_ENABLE			DISABLE
#define TCFG_FM_EMITTER_QN8027_ENABLE			DISABLE

//*********************************************************************************//
//                                  rtc 配置                                     //
//*********************************************************************************//
#define TCFG_RTC_ENABLE						TCFG_APP_RTC_EN

#define TCFG_USE_VIRTUAL_RTC                   0//ENABLE    //假时钟

//*********************************************************************************//
//                                  SPDIF & ARC 配置                                     //
//*********************************************************************************//
#define TCFG_SPDIF_ENABLE                       TCFG_APP_SPDIF_EN
#define TCFG_SPDIF_OUTPUT_ENABLE                ENABLE
#define TCFG_HDMI_ARC_ENABLE                    ENABLE
#define TCFG_HDMI_CEC_PORT                      IO_PORTA_02

//*********************************************************************************//
//                                  IIS 配置                                     //
//*********************************************************************************//
#if (AUDIO_OUTPUT_WAY == AUDIO_OUTPUT_WAY_IIS) || (AUDIO_OUTPUT_WAY == AUDIO_OUTPUT_WAY_DAC_IIS)
#define TCFG_IIS_ENABLE                       ENABLE_THIS_MOUDLE
#else
#define TCFG_IIS_ENABLE                       DISABLE_THIS_MOUDLE
#endif

#define TCFG_IIS_MODE                         (0)   //  0:master  1:slave

#define TCFG_IIS_OUTPUT_EN                    (ENABLE && TCFG_IIS_ENABLE)
#define TCFG_IIS_OUTPUT_PORT                  ALINK0_PORTA
#define TCFG_IIS_OUTPUT_CH_NUM                1 //0:mono,1:stereo
#define TCFG_IIS_OUTPUT_SR                    44100
#define TCFG_IIS_OUTPUT_DATAPORT_SEL          0

#define TCFG_IIS_INPUT_EN                    (DISABLE && TCFG_IIS_ENABLE)
#define TCFG_IIS_INPUT_PORT                  ALINK0_PORTA
#define TCFG_IIS_INPUT_CH_NUM                1 //0:mono,1:stereo
#define TCFG_IIS_INPUT_SR                    44100
#define TCFG_IIS_INPUT_DATAPORT_SEL          1

//*********************************************************************************//
//                                  fat 文件系统配置                                       //
//*********************************************************************************//
#define CONFIG_FATFS_ENABLE					ENABLE




//*********************************************************************************//
//                                  encoder 配置                                   //
//*********************************************************************************//
#define TCFG_ENC_CVSD_ENABLE               ENABLE
#define TCFG_ENC_MSBC_ENABLE               ENABLE
#define TCFG_ENC_G726_ENABLE               ENABLE
#define TCFG_ENC_MP3_ENABLE                ENABLE
#define TCFG_ENC_ADPCM_ENABLE              ENABLE
#define TCFG_ENC_PCM_ENABLE                ENABLE
#define TCFG_ENC_SBC_ENABLE                ENABLE
#define TCFG_ENC_OPUS_ENABLE               DISABLE
#define TCFG_ENC_SPEEX_ENABLE              DISABLE

//*********************************************************************************//
//ali ai profile
#define DUEROS_DMA_EN              0  //not surport
#define TRANS_DATA_EN              0  //not surport
#define	ANCS_CLIENT_EN			   0

#if (DUEROS_DMA_EN || TRANS_DATA_EN || ANCS_CLIENT_EN)
#define BT_FOR_APP_EN			   1
#else
#define BT_FOR_APP_EN			   0
#endif

//*********************************************************************************//

//*********************************************************************************//
//                                 电源切换配置                                    //
//*********************************************************************************//

#define CONFIG_PHONE_CALL_USE_LDO15	    1








//*********************************************************************************//
//            解码叠加配置
//*********************************************************************************//
#define DEC_MIX_ENABLE						  0

//*********************************************************************************//
//                                人声消除使能
//*********************************************************************************//
#define AUDIO_VOCAL_REMOVE_EN       0


///*********************************************************************************//
//          等响度 开启后，需要固定模拟音量,调节软件数字音量
//          等响度使用eq实现，同个数据流中，若打开等响度，请开eq总使能，关闭其他eq,例如蓝牙模式eq
//*********************************************************************************//
#define AUDIO_EQUALLOUDNESS_CONFIG  0  //等响度, 不支持四声道

///*********************************************************************************//
//          环绕音效使能
//*********************************************************************************//
#define AUDIO_SURROUND_CONFIG     0//3d环绕

#define AUDIO_VBASS_CONFIG        0//虚拟低音,虚拟低音不支持四声道
#define AUDIO_SPECTRUM_CONFIG     0  //频响能量值获取接口
#define AUDIO_MIDI_CTRL_CONFIG    0  //midi电子琴接口使能

#define TCFG_EQ_DIVIDE_ENABLE     0  // 四声道eq是否独立  0 使用同个eq效果

//*********************************************************************************//
//                                 2.X声道配置(需使能板极四声道宏DAC_OUTPUT_FRONT_LR_REAR_LR)
//                                 2.1  两个全频较弱的全频音箱 + 一个低音炮
//                                 2.2  两个全频较弱的全频音箱 + 2个低音炮
//*********************************************************************************//
#define SOUND_TRACK_2_P_X_CH_CONFIG        0 //  2.X声道总使能

#if SOUND_TRACK_2_P_X_CH_CONFIG
#define TWO_POINT_ONE_CH             BIT(0)// 2.1声道
#define TWO_POINT_TWO_CH             BIT(1)  // 2.2声道
#define TWO_POINT_X_CH               TWO_POINT_ONE_CH//选配使用2.1  还是2.2 声道

#if (TCFG_AUDIO_DAC_CONNECT_MODE != DAC_OUTPUT_FRONT_LR_REAR_LR)
#undef TCFG_AUDIO_DAC_CONNECT_MODE
#define TCFG_AUDIO_DAC_CONNECT_MODE    DAC_OUTPUT_FRONT_LR_REAR_LR
#endif
#endif/*SOUND_TRACK_2_P_X_CH_CONFIG*/






//*********************************************************************************//
//                                 编译警告                                         //
//*********************************************************************************//
#if ((ANCS_CLIENT_EN || TRANS_DATA_EN || ((TCFG_ONLINE_TX_PORT == IO_PORT_DP) && TCFG_ONLINE_ENABLE) ||((TCFG_COMM_TYPE == TCFG_UART_COMM) && TCFG_SOUNDBOX_TOOL_ENABLE)) && (TCFG_PC_ENABLE || TCFG_UDISK_ENABLE || TCFG_SD0_PORTS == 'E'))
// #error "eq online adjust enable, plaease close usb marco  and sdcard port not e!!!"
#endif// ((TRANS_DATA_EN || TCFG_ONLINE_ENABLE) && (TCFG_PC_ENABLE || TCFG_UDISK_ENABLE))

#if ((TCFG_COMM_TYPE == TCFG_UART_COMM) && TCFG_ONLINE_ENABLE) && (TCFG_PC_ENABLE || TCFG_UDISK_ENABLE)
#error "eq online adjust enable, plaease close usb marco"
#endif// ((TCFG_ONLINE_ENABLE) && (TCFG_PC_ENABLE || TCFG_UDISK_ENABLE))

#if ((TCFG_COMM_TYPE == TCFG_UART_COMM) && TCFG_SOUNDBOX_TOOL_ENABLE ) && TCFG_PC_ENABLE
#error "soundbox tool enable, plaease close pc marco"
#endif// (TCFG_SOUNDBOX_TOOL_ENABLE) && (TCFG_PC_ENABLE)

#if TCFG_UI_ENABLE
#if ((TCFG_SPI_LCD_ENABLE &&  TCFG_CODE_FLASH_ENABLE) && (TCFG_FLASH_DEV_SPI_HW_NUM == TCFG_TFT_LCD_DEV_SPI_HW_NUM))
#error "flash spi port == lcd spi port, please close one !!!"
#endif//((TCFG_SPI_LCD_ENABLE &&  TCFG_CODE_FLASH_ENABLE) && (TCFG_FLASH_DEV_SPI_HW_NUM == TCFG_TFT_LCD_DEV_SPI_HW_NUM))
#endif//TCFG_UI_ENABLE

#if((TRANS_DATA_EN + DUEROS_DMA_EN + ANCS_CLIENT_EN) > 1)
#error "they can not enable at the same time,just select one!!!"
#endif//(TRANS_DATA_EN && DUEROS_DMA_EN)

#if (TCFG_DEC2TWS_ENABLE && (TCFG_MIC_EFFECT_ENABLE ||TCFG_APP_RECORD_EN || TCFG_APP_RTC_EN ||TCFG_DRC_ENABLE))
#error "对箱支持音源转发，请关闭混响录音等功能 !!!"
#endif// (TCFG_DEC2TWS_ENABLE && (TCFG_MIC_EFFECT_ENABLE ||TCFG_APP_RECORD_EN || TCFG_APP_RTC_EN ||TCFG_DRC_ENABLE))

#if (TCFG_MIC_EFFECT_ENABLE && (TCFG_DEC_APE_ENABLE || TCFG_DEC_FLAC_ENABLE || TCFG_DEC_DTS_ENABLE))
// #error "无损格式+混响暂时不支持同时打开 !!!"
#endif//(TCFG_MIC_EFFECT_ENABLE && (TCFG_DEC_APE_ENABLE || TCFG_DEC_FLAC_ENABLE || TCFG_DEC_DTS_ENABLE))

#if (TCFG_REVERB_DODGE_EN && (USER_DIGITAL_VOLUME_ADJUST_ENABLE == 0))
#error "使用闪避功能，打开自定义数字音量调节 !!!"
#endif

#if ((TCFG_NORFLASH_DEV_ENABLE || TCFG_NOR_FS_ENABLE) &&  TCFG_UI_ENABLE)
#error "引脚复用问题，使用norflash需要关闭UI ！！！"
#endif

#if 0// ((TCFG_APP_RECORD_EN) && (TCFG_USER_TWS_ENABLE))
#error "TWS 暂不支持录音功能"
#endif
#if ((TCFG_MIC_EFFECT_ENABLE) && (TCFG_USER_TWS_ENABLE)&&(TCFG_DEC_M4A_ENABLE))
#error "TWS+混响 暂不支持M4A解码"
#endif

#if ((AUDIO_EQUALLOUDNESS_CONFIG) && (SYS_VOL_TYPE == VOL_TYPE_ANALOG))
#error "开启等响度 需要使用数字音量"
#endif

#if 0//((TCFG_APP_FM_EN) && (TCFG_MIC_EFFECT_ENABLE) && (RECORDER_MIX_EN))
#error "FM模式下开混响暂不支持混合录音"
#endif

#if AUDIO_EQUALLOUDNESS_CONFIG
#if (TCFG_EQ_ENABLE == 0)
#error "开启等响度 需要打开EQ总使能"
#endif
#if (TCFG_EQ_ENABLE && TCFG_BT_MUSIC_EQ_ENABLE)
#error "开启等响度 需要打开EQ总使能，关闭其他模式的eq,例如关闭蓝牙播歌eq"
#endif
#endif
#if TCFG_MIC_DODGE_EN
#if !SYS_DIGVOL_GROUP_EN
#error "请使能软件数字音量SYS_DIGVOL_GROUP_EN"
#endif
#endif

#if TCFG_DYNAMIC_EQ_ENABLE && !TCFG_DRC_ENABLE //动态eq使能后，需接入drc进行幅度控制
#error "动态eq使能后，需使能TCFG_DRC_ENABLE接入drc进行幅度控制"
#endif
#if TCFG_DYNAMIC_EQ_ENABLE && !TCFG_AUDIO_MDRC_ENABLE
#error "动态eq使能后，需使能TCFG_AUDIO_MDRC_ENABLE 为1接入多带drc进行幅度控制"
#endif
#if SOUND_TRACK_2_P_X_CH_CONFIG && LINEIN_MODE_SOLE_EQ_EN
#error "2.1/2.2声道不支持 linein模式独立的eq\drc配置"
#endif
#if TCFG_EQ_DIVIDE_ENABLE && (TCFG_AUDIO_DAC_CONNECT_MODE != DAC_OUTPUT_FRONT_LR_REAR_LR)
#error "eq效果独立，需使能四声道宏DAC_OUTPUT_FRONT_LR_REAR_LR"
#endif
#if SOUND_TRACK_2_P_X_CH_CONFIG && !TCFG_EQ_DIVIDE_ENABLE
#error  "2.1/2.2声道，需使能宏TCFG_EQ_DIVIDE_ENABLE"
#endif
#if TCFG_MIC_VOICE_CHANGER_ENABLE && (TCFG_MIC_EFFECT_SEL != MIC_EFFECT_ECHO)
#error  "变声目前支持加载ECHO音效下"
#endif
#if TCFG_USER_TWS_ENABLE && TCFG_MIC_EFFECT_ENABLE &&((TCFG_MIC_EFFECT_SEL != MIC_EFFECT_ECHO) && (TCFG_MIC_EFFECT_SEL != MIC_EFFECT_MEGAPHONE))
#error "tws时，只支持ehco混响或者megaphone"
#endif

///<<<<所有宏定义不要在编译警告后面定义！！！！！！！！！！！！！！！！！！！！！！！！！！
///<<<<所有宏定义不要在编译警告后面定义！！！！！！！！！！！！！！！！！！！！！！！！！！


//*********************************************************************************//
//                                 配置结束                                         //
//*********************************************************************************//


#endif //CONFIG_BOARD_AC695X_DEMO
#endif //CONFIG_BOARD_AC695X_DEMO_CFG_H
