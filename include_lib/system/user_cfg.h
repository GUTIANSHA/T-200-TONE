#ifndef __USER_CFG_H__
#define __USER_CFG_H__

#include "typedef.h"
#include "app_config.h"

#define LOCAL_NAME_LEN	32	/*BD_NAME_LEN_MAX*/

#ifdef SUPPORT_VM_SAVE_WORK_MODE
enum{
	WORK_BT_MODE = 1,
	WORK_FM_MODE,
#ifdef SUPPORT_VM_SAVE_USB_SD_MODE
	WORK_USB_MODE,
	WORK_SD_MODE,
#else
	WORK_MUSIC_MODE,
#endif
	WORK_AUX_MODE,
#ifdef SUPPORT_AUX1_MODE
	// WORK_AUX1_MODE,
#endif
#ifdef SUPPORT_AUX2_MODE
	// WORK_AUX2_MODE,
#endif
#ifdef 	SUPPORT_SPDIF_OPT_MODE
    WORK_OPT_MODE,
#endif
#ifdef SUPPORT_SPDIF_COAX_MODE
    WORK_COAX_MODE,
#endif
#ifdef SUPPORT_SPDIF_HDMI_MODE
    WORK_HDMI_MODE,
#endif
	WORK_IDLE_MODE,
	WORK_MODE_MAX,
};
void vm_save_work_mode(u8 workmode);
u8 vm_read_work_mode();
int poweron_goto_vm_work_mode();
#endif


typedef struct {
    u16 m62429_in_check_id;
    u16 mic_check_id;
    u16 user_rgb_scan_add_id;
    #ifdef SUPPORT_SPDIF_SCAN
    u16 spdif_scan_id;
    #endif
    u16 user_scan_id;
    u16 poweron_task_init_id;
    } type_timer_id;    //用于记录定时器的id

    extern type_timer_id user_timer;


//bt bin??
typedef struct __BT_CONFIG {
    u8 edr_name[LOCAL_NAME_LEN];       //??????????
    u8 mac_addr[6];                    //????MAC???
    u8 rf_power;                       //???��??
    u8 dac_analog_gain;                //???DAC???????
    u8 mic_analog_gain;                //???MIC????
    u16 tws_device_indicate;           /*????????????????inquiry?????,???????????????????????*/
    u8 tws_local_addr[6];
    u8 ble_name[LOCAL_NAME_LEN];       //ble??????
    u8 ble_mac_addr[6];                //ble????MAC???
    u8 ble_rf_power;                   //ble???��??
} _GNU_PACKED_  BT_CONFIG;

//audio bin??
typedef struct __AUDIO_CONFIG {
    u8 sw;
    u8 max_sys_vol;         //???????????
    u8 default_vol;         //???????????
    u8 tone_vol;            //?????????
} _GNU_PACKED_ AUDIO_CONFIG;

//status bin????
typedef struct __STATUS {
    u8 charge_start;    //????????
    u8 charge_full;     //??????
    u8 power_on;        //????
    u8 power_off;       //???
    u8 lowpower;        //???
    u8 max_vol;         //?????????
    u8 phone_in;        //????
    u8 phone_out;       //???
    u8 phone_activ;     //?????
    u8 bt_init_ok;      //??????????????
    u8 bt_connect_ok;   //??????????
    u8 bt_disconnect;   //???????
    u8 tws_connect_ok;   	//TWS??????
    u8 tws_disconnect;   	//TWS???????
} _GNU_PACKED_ STATUS;

typedef struct __STATUS_CONFIG {
    u8 sw;
    STATUS led;    //led status
    STATUS tone;   //tone status
} _GNU_PACKED_ STATUS_CONFIG;

//charge bin??
typedef struct __CHARGE_CONFIG {
    u8 sw;                  //????
    u8 poweron_en;          //???????????
    u8 full_v;              //???????
    u8 full_c;              //????????
    u8 charge_c;            //??????
} _GNU_PACKED_  CHARGE_CONFIG;

//key
typedef struct __KEY_OP {
    u8 short_msg;     //??????
    u8 long_msg;      //???????
    u8 hold_msg;      //hold ???
    u8 up_msg;        //??????
    u8 double_msg;    //??????
    u8 triple_msg;    //???????
} _GNU_PACKED_ KEY_OP;

//mic type
typedef struct __MIC_TYPE_CONFIG {
    u8 type;     //0:?????????        1:???????
    //1:16K 2:7.5K 3:5.1K 4:6.8K 5:4.7K 6:3.5K 7:2.9K  8:3K  9:2.5K 10:2.1K 11:1.9K  12:2K  13:1.8K 14:1.6K  15:1.5K 16:1K 31:0.6K
    u8 pull_up;
    //00:2.3v 01:2.5v 10:2.7v 11:3.0v
    u8 ldo_lev;
} _GNU_PACKED_ MIC_TYPE_CONFIG;


//?????????????
typedef struct __AUTO_OFF_TIME_CONFIG {
    u8 auto_off_time;
} _GNU_PACKED_ AUTO_OFF_TIME_CONFIG;

//???????????
typedef struct __AUTO_LOWPOWER_V_CONFIG {
    u16 warning_tone_v;
    u16 poweroff_tone_v;
} _GNU_PACKED_ AUTO_LOWPOWER_V_CONFIG;

//LRC????
typedef struct __LRC_CONFIG {
    u16 lrc_ws_inc;
    u16 lrc_ws_init;
    u16 btosc_ws_inc;
    u16 btosc_ws_init;
    u8 lrc_change_mode;
} _GNU_PACKED_ LRC_CONFIG;


/*-----------------------------------------------------
		customer add function
-----------------------------------------------------*/

#ifdef SUPPORT_KPH_DEF

typedef struct {
u16 power_on_mic_init;
u8 music_dev;
u8 seg;
u8 com;
u16 bLedEq;
u8 bt_sta;

int test;
#ifdef SUPPORT_LED_FLASH
u8 led_flash;
#endif

#ifdef SPI1_RGB_DS4751C_FL872
u8 l_vol_ed;
#endif
#ifdef SUPPORT_UI_UART_XY8080
u8 xy8080_cnt;
#endif

u16 ad_vol;
u8 led_vol;
u8 led_vol_mem;
u16 ad_vol_pr;
u8 sys_vol;

u8 led_vol_pr;
u8 standby_out_to_spdif;
u8 tws_key_disable;
u8 dac_sys_max_vol;
u8 mic_vol;
u8 jt_vol;
u8 echo;
u8 micdelay;
s8 tre;
s8 bass;
s8 mid;
bool one_key_bass;
u8 mic_tre;
u8 mic_bass;
s8 vmictretemp;
s8 vmicbasstemp;

u16 rgb_timer;
u8 tmp_rgb_mode;

#ifdef SUPPORT_KEY_RGB_ADKEY
	u8 key_rgb_adkey;
	u8 key_rgb_cnt_adkey;
#endif
#ifdef SUPPORT_KEY_RGB_ADC
		u8 key_rgb_cnt_adc;
		u16 key_rgb_adc;
#endif

#ifdef SUPPORT_UI_XY8080
	bool send_data; //?????????
	bool start_send_date_falg; //????????????? ?????
	bool bSendBusy; //???????????????
	u8 buff_bit;	//???(bShowBuff2[buff_cnt])?��?????
	u8 buff_cnt;	//??????????????bShowBuff2??????
	u16 send_over_delay_reset; //??????????????????????????

	u8 xy8080_com;
	u8 xy8080_seg;
	bool update_bSendBusy_falg; //??????
#endif

#ifdef SUPPORT_MIC_ECHO_VOL_SET
u8  vMicEchoTemp;
u8  vMicVolTemp;
#endif
#ifdef SUPPORT_MIC_DELAY_SET
u16  vMicDelayTemp;
#endif
#ifdef SUPPORT_MIC_PR_CHECK
bool mic_precedence_flag;
#endif
s8 vbasstemp;
s8 vtretemp;
#ifdef SUPPORT_MID_SET
s8 vmidtemp;
#endif
u16 adkey_val;
u8 bat_sta;
u8 voice_sta;
u8 music_device;
u16 user_key_msg;
bool mic_is_online;
bool mic_is_online_energy;
bool jt_is_online;
bool mic_ad_in;
u16 ir_num;
u8 input_number_cnt;
u8 input_time_out;
u16 music_total_file_num;
bool power_on_set_vol_flag;
bool power_off_flag;

u8 bt_auto_to_play;
bool bt_tws_off;
bool bt_tws_discon;
bool bt_tws_key;
bool mute_flag;
bool aux_mute;
u8 disale_adkey;
u8 eq_mode;
bool rec_auto_play;
u16 rec_last_file_num;
bool play_rec_file_flag;
bool bRecPlayShowErroFlag;
bool bModeKeyPlayMp3Flag;
bool one_key_to_rec;
bool cur_file_is_RecFile;
bool led_onoff_flag;
u8 pa_delay_unmute;
u8 pa_mute_sta;
bool bt_tws_slave_con;
bool bt_tws_onoff;
bool mic_sw;
bool vocal_pp;
bool disable_flash_vm;
u16 Rec_File_Total_Num;
u8 bt_mode_init;
bool aux_online;
u8 bMusicBugCnt;
bool sw_off;
bool sw_off_flag;
u8 bSwOffDlyTimer;
u8 bSwOffDlyCnt;
u16 gw_led_msg;
bool key_long_hold_flag;
bool music_pause;
u8 voice_play_cnt;
s8 menu_mode;
u8 menu_mode_cnt;

s8 menu1_mode;
u8 menu1_mode_cnt;

s8 eq_menu;
u8 eq_menu_cnt;
s8 eq50hz_val;
s8 eq50hz_vmval;
s8 eq6khz_val;
s8 eq6khz_vmval;
bool key_msg;
#ifdef SUPPORT_MOS_POWER_ON
u8 bPowerMosFlag;
bool bPowerMosKeyFlag;
bool bPowerOffMosFlag;
#endif
#ifdef SUPPORT_KEY_LED_EN
bool bKeyLedFlag;
#endif
#ifdef SUPPORT_LOWPOWER_VOICE
u16 send_lowpower_cnt;
#endif
#ifdef SUPPORT_LOWPOWER_3Timer_POWEROFF
u16 bLowPowerOffTimer;
#endif

#ifdef SPI1_RGB_DEV_EN
u8 extern_led_rgb_flag;
#endif
#ifdef SUPPORT_BT_PHONE_DISCONNECT_TO_AUX
u8 bBtDisconnectCnt;
bool bBtDisconnectFlat;
#endif
#ifdef SUPPORT_VOICE_EN_OFF
u8 bVoiceFlag;
u8 bVoicePlayPauseFlag;
#endif
#ifdef SPI1_RGB_DEV_EN
bool bSpiIrFlag;
u8 bRgbColorVal;
#endif
#ifdef SUPPORT_CHANGE_PANEL_FLASH_ERRO
bool bMusicExitCnt;
#endif
#ifdef SUPPORT_STOP_KEY
bool bMusicStopFlag;
#endif
#ifdef SUPPORT_TONE_VOL_CONTROL
u8 bToneVol;
#endif
#ifdef SUPPORT_TEST_LED7
u8 bLed7High;
u8 bLed7Low;
#endif
s32 mic_energy;

#if defined SUPPORT_CD4051_PONTEN ||defined SUPPORT_PONTEN_EQ1_EQ5
u16 bEq1AdVal;
u16 bEq2AdVal;
u16 bEq3AdVal;
u16 bEq4AdVal;
u16 bEq5AdVal;
u8 bEq1Val;
u8 bEq2Val;
u8 bEq3Val;
u8 bEq4Val;
u8 bEq5Val;
s8 veq1temp;
s8 veq2temp;
s8 veq3temp;
s8 veq4temp;
s8 veq5temp;
u8 bCd4051TestVal;
u8 bCd4051ModeDly;

#endif
#ifdef SUPPORT_SPDIF_COAX_MODE
int bSpdifMode;
int prev_bSpdifMode;
#endif
#ifdef SUPPORT_COAX_MODE
u8 bSpdifMode;
#endif
#ifdef SUPPORT_USB_MODE_KEY
bool bMediaUsbFlag;
#endif
#ifdef SUPPORT_SD_MODE_KEY
bool bMediaSdFlag;
#endif
#ifdef SUPPORT_OK_MENU_KEY
u8 bMicMenu;
u8 bMicMenuCnt;
#endif
#ifdef SUPPORT_POWER_ON_AD_SHOW_DLY
u8 bPowerOnShowDlyCnt;
#endif
#ifdef SUPPORT_SOUNDSCAPES
u8 bSoundScapesVal;
#endif
#if AUDIO_OUTPUT_AUTOMUTE
bool bAutoMuteFlag;
#endif
#ifdef SUPPORT_VM_VOICE_ONOFF
u8 vVmVoiceOnoffCnt;
#endif
#ifdef SUPPORT_ENCODER
u8 disable_key_cnt;
#ifdef SUPPORT_ENCORD_LED
u8 bEncordSw;
#endif
#endif

#ifdef SUPPORT_LED7_UART_TEST_IO
u8 temp_8bit_led_data;
#endif

//=============================================//

bool linein_init_ok;
#ifdef SUPPORT_MULT_AUX_INPUT_MODE
int vMultModeVal;
int prev_vMultModeVal;
#endif
#ifdef MULT_AUX_INPUT_MODE_FROM_INTERNAL
int linein_source;
#endif

//=============================================//

bool hdmi_cec_init_flag;
bool hdmi_is_online;


#ifdef SUPPORT_VM_VOL
u8 vVolCnt;
#endif
#ifdef SUPPORT_VM_BASS
u8 vbassCnt;
#endif
#ifdef SUPPORT_VM_TREB
u8 vTreCnt;
#endif
#ifdef SUPPORT_VM_MICVOL
u8 vMicvolCnt;
#endif
#ifdef SUPPORT_VM_ECHO
u8 vEchoCnt;
#endif
#ifdef SUPPORT_VM_MICBASS
u8 vMicBassCnt;
#endif
#ifdef SUPPORT_VM_MICTRE
u8 vMicTreCnt;
#endif
#ifdef SUPPORT_VM_MICDELAY
u8 vMicDelayCnt;
#endif

#ifdef SUPPORT_VM_JT_VOL
u8 vJtVolCnt;
#endif

u32 vMicDelayTempVal;
u32 vMicEchoTempVal;




u32 cur_adc_sample_ch; //??�x??ADC??????????ADC???
#ifdef SUPPORT_MULT_INPUT_IN_LINEIN
u8 mult_mode;
u8 bLineChannelVal;
#endif
#ifdef SUPPORT_POWER_ONOFF_BAT
u8 bBatCnt;
#endif
#ifdef SUPPORT_POWER_ON_FIRST_DISABLE_BT_VOICE
u8 bPowerOnDisableBtVoiceCnt;
#endif

u8 spdif_pp_flag;

u8 is_forward;

//tws
#if TCFG_USER_TWS_ENABLE
bool tws_start_connect_flag;
#endif

#ifdef SUPPORT_MULTI_VOICE_MODE
u8 bVoiceVal;
#endif

u8 flag_10s;

u8 power_work_init
} type_use_ctl;
extern  type_use_ctl use_ctl;

#ifdef SUPPORT_USER_SPDIF_MODE
enum{
#ifdef SUPPORT_SPDIF_COAX_MODE
SPDIF_COAX,
#endif
#ifdef SUPPORT_SPDIF_OPT_MODE
SPDIF_OPT,
#endif
#ifdef SUPPORT_SPDIF_HDMI_MODE
SPDIF_HDMI,
#endif
MAX_SPDIF_MODE,
};
#endif

#ifdef SUPPORT_MULTI_VOICE_MODE
enum{
#ifdef SUPPORT_NULL_VOICE
VOICE_MODE_NULL,
#endif
#ifdef SUPPORT_EN_VOICE
VOICE_MODE_EN,
#endif
#ifdef SUPPORT_CH_VOICE
VOICE_MODE_CH,
#endif
VOICE_MODE_MAX,
};
#endif

#ifdef SUPPORT_MULT_AUX_INPUT_MODE
enum{
#ifdef SUPPORT_AUX0_MODE
MODE_AUX0,
#endif
#ifdef SUPPORT_AUX1_MODE
MODE_AUX1,
#endif
#ifdef SUPPORT_AUX2_MODE
MODE_AUX2,
#endif
MODE_AUX_MAX,
#if !defined SUPPORT_AUX0_MODE
MODE_AUX0,
#endif
#if !defined SUPPORT_AUX1_MODE
MODE_AUX1,
#endif
#if !defined SUPPORT_AUX2_MODE
MODE_AUX2,
#endif
MODE_IDLE = 0xFF,
};
#endif

typedef struct {
    u8  power;     //????????????
    u8  power_cur; //???AD????????
    u8  power_pre; //?????
    u8  status;
    u8  disp_flag;
    u16  bat_full_cnt;
    u8  bat_no_full_cnt;
    bool batlow_stop;
 #ifdef SUPPORT_LOWPOWER_VOL_AUTO_ADJUST
    u8 bBatState;
    u8 bBatStatePrev;
    u8 bBatStateTemp;
    u16 bBatStateCnt;
    u8 bBatUpToDn;
#endif
} typdef_led_bat;
extern typdef_led_bat led_bat;


enum
{
NOT_CHARGE,
IN_CHARGE,
};

enum{
POWER_NULL,
POWER_FULL,
POWER_4,
POWER_3,
POWER_2,
POWER_1,
POWER_H,
POWER_L,

POWER_LOW,
NO_POWER,

};


enum
{
    MAIN_WORK_MODE=0,
    MUSIC_WORK_MODE,
    LINE_WORK_MODE,
    RADIO_WORK_MODE,
    BT_WORK_MODE,
    PC_WORK_MODE,
    STANDBY_WORK_MODE,  //??????
    REC_WORK_MODE,
    RTC_WORK_MODE,
    NOP_WORK_MODE,   //????
    POWER_OFF_WORK_MODE,
    FM_AM_WORK_MODE,
    ECHO_WORK_MODE,
};
extern u8 workmode;

enum{
BAT_H,
BAT_L,
BAT_LOW,
};

enum{
MUSIC_DEV_NULL,
MUSIC_DEV_USB,
MUSIC_DEV_SD,
};

#ifdef SPI1_RGB_DEV_EN
enum{
	RGB_OFF,
	RGB_MODE_1,
    RGB_MODE_2,
    RGB_MODE_3


 #ifndef RGB_MAX
	RGB_MAX,
#endif
};
#endif

void pa_mute(void);
void pa_unmute(void);

#endif

/*-----------------------------------------------------
		customer add function
-----------------------------------------------------*/



void cfg_file_parse(u8 idx);
const u8 *bt_get_mac_addr();
void bt_get_tws_local_addr(u8 *addr);

STATUS *get_led_config(void);
STATUS *get_tone_config(void);
void get_random_number(u8 *ptr, u8 len);
extern void bt_get_vm_mac_addr(u8 *addr);
extern u8 get_max_sys_vol(void);
extern const char *bt_get_local_name();
extern u16 bt_get_tws_device_indicate(u8 *tws_device_indicate);
const char *bt_get_local_name();
extern void bt_update_mac_addr(u8 *addr);
extern void bt_set_local_name(char *name, u8 len);
extern void bt_reset_and_get_mac_addr(u8 *addr);
extern void bt_set_pair_code_en(u8 en);

#endif
