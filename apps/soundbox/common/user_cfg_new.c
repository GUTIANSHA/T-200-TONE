#include "app_config.h"
#include "user_cfg.h"
#include "fs.h"
#include "string.h"
#include "system/includes.h"
#include "vm.h"
#include "btcontroller_config.h"
#include "app_main.h"
#include "media/includes.h"
#include "audio_config.h"
#include "asm/pwm_led.h"
#include "aec_user.h"
#include "app_power_manage.h"


#define LOG_TAG_CONST       USER_CFG
#define LOG_TAG             "[USER_CFG]"
#define LOG_ERROR_ENABLE
#define LOG_DEBUG_ENABLE
#define LOG_INFO_ENABLE
#define LOG_DUMP_ENABLE
#define LOG_CLI_ENABLE
#include "debug.h"

#define USE_CONFIG_STATUS_SETTING            1        //状态设置，包括灯状态和提示音
#define USE_CONFIG_AUDIO_SETTING             1        //音频设置
#define USE_CONFIG_MIC_TYPE_SETTING          0        //MIC类型设置
#define USE_CONFIG_AUTO_OFF_SETTING          1        //自动关机时间设置
#define USE_CONFIG_COMBINE_VOL_SETTING       1		  //联合音量读配置

void lp_winsize_init(struct lp_ws_t *lp);
void bt_max_pwr_set(u8 pwr, u8 pg_pwr, u8 iq_pwr, u8 ble_pwr);
void app_set_sys_vol(s16 vol_l, s16  vol_r);

BT_CONFIG bt_cfg = {
    .edr_name        = {'j', 'l', '_', 's', 'o', 'u', 'n', 'd', 'b', 'o', 'x', '_', '1'},
    .mac_addr        = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff},
    .tws_local_addr  = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff},
    .rf_power        = 10,
    .dac_analog_gain = 25,
    .mic_analog_gain = 7,
    .tws_device_indicate = 0x6688,
    .ble_name        = {'j', 'l', '_', 's', 'o', 'u', 'n', 'd', 'b', 'o', 'x', '_', 'b', 'l', 'e'},
    .ble_mac_addr        = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff},
    .ble_rf_power        = 10,
};

AUDIO_CONFIG audio_cfg = {
    .max_sys_vol    = SYS_MAX_VOL,
    .default_vol    = SYS_DEFAULT_VOL,
    .tone_vol       = SYS_DEFAULT_TONE_VOL,
};

//======================================================================================//
//                                 		BTIF配置项表                               		//
//	参数1: 配置项名字                                			   						//
//	参数2: 配置项需要多少个byte存储														//
//	说明: 配置项ID注册到该表后该配置项将读写于BTIF区域, 其它没有注册到该表       		//
//		  的配置项则默认读写于VM区域.													//
//======================================================================================//
const struct btif_item btif_table[] = {
// 	 	item id 		   	   len   	//
    {CFG_BT_MAC_ADDR, 			6 },
    {CFG_BT_FRE_OFFSET,   		6 },   //测试盒矫正频偏值
    //{CFG_DAC_DTB,   			2 },
    //{CFG_MC_BIAS,   			1 },
    {0, 						0 },   //reserved cfg
};

//============================= VM 区域空间最大值 ======================================//
const int vm_max_size_config = VM_MAX_SIZE_CONFIG; //该宏在app_cfg中配置
//======================================================================================//

struct lp_ws_t lp_winsize = {
    .lrc_ws_inc = 480,      //260
    .lrc_ws_init = 400,
    .bt_osc_ws_inc = 480,
    .bt_osc_ws_init = 140,
    .osc_change_mode = 1,
};

#if CONFIG_TWS_PAIR_BY_BOTH_SIDES
static u8 tws_pair_by_both_sides_en = 0;

void bt_set_pair_code_en(u8 en)
{
    tws_pair_by_both_sides_en = en;
}
#endif

u16 bt_get_tws_device_indicate(u8 *tws_device_indicate)
{
#if CONFIG_TWS_PAIR_BY_BOTH_SIDES
    if (tws_pair_by_both_sides_en) {
        return bt_cfg.tws_device_indicate;
    } else {
        return bt_cfg.tws_device_indicate + 1;
    }
#else
    return bt_cfg.tws_device_indicate;
#endif
}

const u8 *bt_get_mac_addr()
{
    return bt_cfg.mac_addr;
}

void bt_update_mac_addr(u8 *addr)
{
    memcpy(bt_cfg.mac_addr, addr, 6);
}

static u8 bt_mac_addr_for_testbox[6] = {0};
void bt_get_vm_mac_addr(u8 *addr)
{
#if 0
    //中断不能调用syscfg_read;
    int ret = 0;

    ret = syscfg_read(CFG_BT_MAC_ADDR, addr, 6);
    if ((ret != 6)) {
        syscfg_write(CFG_BT_MAC_ADDR, addr, 6);
    }
#else

    memcpy(addr, bt_mac_addr_for_testbox, 6);
#endif
}
void bt_update_testbox_addr(u8 *addr)
{
    log_info("update textbox_addr");
    put_buf(addr, 6);
    memcpy(bt_mac_addr_for_testbox, addr, 6);
}
void bt_get_tws_local_addr(u8 *addr)
{
    memcpy(addr, bt_cfg.tws_local_addr, 6);
}

const char *sdk_version_info_get(void)
{
    extern u32 __VERSION_BEGIN;
    char *version_str = ((char *)&__VERSION_BEGIN) + 4;

    return version_str;
}

const char *bt_get_local_name()
{
    return (const char *)(bt_cfg.edr_name);
}

const char *bt_get_pin_code()
{
    return "0000";
}


void bt_reset_and_get_mac_addr(u8 *addr)
{
    get_random_number(addr, 6);
    syscfg_write(CFG_BT_MAC_ADDR, addr, 6);
    memcpy(bt_cfg.mac_addr, addr, 6);
}

u8 get_hdmi_online(void)
{
    return use_ctl.hdmi_is_online;
}


extern STATUS_CONFIG status_config;
extern struct charge_platform_data charge_data;
/* extern struct dac_platform_data dac_data; */
extern struct adkey_platform_data adkey_data;
extern struct led_platform_data pwm_led_data;
extern struct adc_platform_data adc_data;

#include "fm_emitter/fm_emitter_manage.h"

u8 get_max_sys_vol(void)
{
#if TCFG_APP_FM_EMITTER_EN
    return FM_EMITTER_MAX_VOL;
#else
    return (audio_cfg.max_sys_vol);
#endif
}

#if 1
u8 get_tone_vol(void)
{
    if (!audio_cfg.tone_vol) {
        return (get_max_sys_vol());
    }
    if (audio_cfg.tone_vol > get_max_sys_vol()) {
        return (get_max_sys_vol());
    }

    return (audio_cfg.tone_vol);
}
#endif

__BANK_INIT_ENTRY
void cfg_file_parse(u8 idx)
{
    u8 tmp[128] = {0};
    int ret = 0;

    memset(tmp, 0x00, sizeof(tmp));

    /*************************************************************************/
    /*                      CFG READ IN cfg_tools.bin                        */
    /*************************************************************************/

    //-----------------------------CFG_COMBINE_VOL----------------------------------//
#if (SYS_VOL_TYPE == VOL_TYPE_AD)
    audio_combined_vol_init(USE_CONFIG_COMBINE_VOL_SETTING);
#endif/*SYS_VOL_TYPE*/

    //-----------------------------CFG_BT_NAME--------------------------------------//
    ret = syscfg_read(CFG_BT_NAME, tmp, 32);
    if (ret < 0) {
        log_info("read bt name err\n");
    } else if (ret >= LOCAL_NAME_LEN) {
        memset(bt_cfg.edr_name, 0x00, LOCAL_NAME_LEN);
        memcpy(bt_cfg.edr_name, tmp, LOCAL_NAME_LEN);
        bt_cfg.edr_name[LOCAL_NAME_LEN - 1] = 0;
        log_info("read new cfg bt name config:%s\n", tmp);
    } else {
        memset(bt_cfg.edr_name, 0x00, LOCAL_NAME_LEN);
        memcpy(bt_cfg.edr_name, tmp, ret);
        log_info("read new cfg bt name config:%s\n", tmp);
    }
    /* g_printf("bt name config:%s\n", bt_cfg.edr_name); */
    log_info("bt name config:%s\n", bt_cfg.edr_name);

//-----------------------------CFG_BLE_NAME--------------------------------------//
    ret = syscfg_read(CFG_BLE_NAME, tmp, 32);
    if (ret < 0) {
        log_info("read ble name err\n");
    } else if (ret >= LOCAL_NAME_LEN) {
        memset(bt_cfg.ble_name, 0x00, LOCAL_NAME_LEN);
        memcpy(bt_cfg.ble_name, tmp, LOCAL_NAME_LEN);
        bt_cfg.ble_name[LOCAL_NAME_LEN - 1] = 0;
        log_info("read new cfg ble name config:%s\n", tmp);
    } else {
        memset(bt_cfg.ble_name, 0x00, LOCAL_NAME_LEN);
        memcpy(bt_cfg.ble_name, tmp, ret);
        log_info("read new cfg ble name config:%s\n", tmp);
    }
    /* g_printf("bt name config:%s\n", bt_cfg.edr_name); */
    log_info("ble name config:%s\n", bt_cfg.ble_name);

    //-----------------------------CFG_TWS_PAIR_CODE_ID----------------------------//
    ret = syscfg_read(CFG_TWS_PAIR_CODE_ID, &bt_cfg.tws_device_indicate, 2);
    log_info("new cfg tws pair code config:\n");
    log_info_hexdump(&bt_cfg.tws_device_indicate, 2);
    if (ret < 0) {
        log_debug("read pair code err\n");
        bt_cfg.tws_device_indicate = 0xFFFF;
    }
    /* g_printf("tws pair code config:\n"); */
    log_info("tws pair code config:\n");
    log_info_hexdump(&bt_cfg.tws_device_indicate, 2);

    //-----------------------------CFG_BT_RF_POWER_ID----------------------------//
    ret = syscfg_read(CFG_BT_RF_POWER_ID, &app_var.rf_power, 1);
    log_info("read new cfg rf config:%d\n", app_var.rf_power);
    if (ret < 0) {
        log_debug("read rf err\n");
        app_var.rf_power = 10;
    }
    /* g_printf("rf config:%d\n", app_var.rf_power); */
    log_info("rf config:%d\n", app_var.rf_power);

    //-----------------------------CFG_BLE_RF_POWER_ID----------------------------//
    ret = syscfg_read(CFG_BLE_RF_POWER_ID, &app_var.ble_rf_power, 1);
    log_info("read new cfg ble rf config:%d\n", app_var.rf_power);
    if (ret < 0) {
        log_debug("read ble rf err\n");
        app_var.ble_rf_power = 10;
    }
    /* g_printf("ble rf config:%d\n", app_var.ble_rf_power); */
    log_info("ble rf config:%d\n", app_var.ble_rf_power);

#if TCFG_APP_BT_EN
    bt_max_pwr_set(app_var.rf_power, 5, 8, app_var.ble_rf_power);
#endif

    //-----------------------------CFG_AEC_ID------------------------------------//
    /* g_printf("aec config:\n"); */
    log_info("aec config:\n");
    AEC_CONFIG aec;
    ret = syscfg_read(CFG_AEC_ID, &aec, sizeof(AEC_CONFIG));
    if (ret > 0) 
	{
        log_info_hexdump(&aec, sizeof(AEC_CONFIG));
        app_var.aec_mic_gain 	  = aec.mic_again;
        app_var.aec_dac_gain 	  = aec.dac_again;
        app_var.aec_mode 	      = aec.aec_mode;
        app_var.ul_eq_en 	 	  = aec.ul_eq_en;
        app_var.ndt_fade_in  	  = aec.ndt_fade_in;
        app_var.ndt_fade_out 	  = aec.ndt_fade_out;
        app_var.dt_fade_in   	  = aec.dt_fade_in;
        app_var.dt_fade_out  	  = aec.dt_fade_out;
        app_var.ndt_max_gain 	  = aec.ndt_max_gain;
        app_var.ndt_min_gain 	  = aec.ndt_min_gain;
        app_var.ndt_speech_thr 	  = aec.ndt_speech_thr;
        app_var.dt_max_gain    	  = aec.dt_max_gain;
        app_var.dt_min_gain    	  = aec.dt_min_gain;
        app_var.dt_speech_thr  	  = aec.dt_speech_thr;
        app_var.echo_present_thr  = aec.echo_present_thr;
        app_var.aec_dt_aggress    = aec.aec_dt_aggress;
        app_var.aec_refengthr     = aec.aec_refengthr;
        app_var.es_aggress_factor = aec.es_aggress_factor;
        app_var.es_min_suppress   = aec.es_min_suppress;
        app_var.ans_aggress 	  = aec.ans_aggress;
        app_var.ans_suppress      = aec.ans_suppress;

#if AEC_READ_CONFIG
#ifdef CONFIG_MEDIA_ORIGIN_ENABLE
        aec_cfg_fill(&aec);
#endif/*CONFIG_MEDIA_ORIGIN_ENABLE*/
#endif/*AEC_READ_CONFIG*/
    } 
	else 
	{
        log_debug("read aec err\n");
        app_var.aec_mic_gain = 0;
        app_var.aec_dac_gain = 22;
        app_var.aec_mode = 2;
        app_var.ul_eq_en = 1;
        app_var.ndt_fade_in = 1.3f;
        app_var.ndt_fade_out = 1.3f;
        app_var.dt_fade_in = 1.3f;
        app_var.dt_fade_out = 1.3f;
        app_var.ndt_max_gain = 6;
        app_var.ndt_min_gain = 3;
        app_var.ndt_speech_thr = -50.f;
        app_var.dt_max_gain = 12;
        app_var.dt_min_gain = 0;
        app_var.dt_speech_thr = -40.f;
        app_var.echo_present_thr = -70.f;
        app_var.aec_dt_aggress = 1;
        app_var.aec_refengthr = -70.f;
        app_var.es_aggress_factor = -3.f;
        app_var.es_min_suppress = 4;
        app_var.ans_aggress = 1.25f;
        app_var.ans_suppress = 0.09f;
    }
    log_info("aec_cfg mic_gain:%d dac_gain:%d", app_var.aec_mic_gain, app_var.aec_dac_gain);

    //-----------------------------CFG_MIC_TYPE_ID------------------------------------//
#if USE_CONFIG_MIC_TYPE_SETTING
    /* g_printf("mic_type_config:\n"); */
    log_info("mic_type_config:\n");
    MIC_TYPE_CONFIG mic_type;
    ret = syscfg_read(CFG_MIC_TYPE_ID, &mic_type, sizeof(MIC_TYPE_CONFIG));
    if (ret > 0) {
        log_info_hexdump(&mic_type, sizeof(MIC_TYPE_CONFIG));
        adc_data.mic_capless = mic_type.type;
#if ((TCFG_AUDIO_DAC_CONNECT_MODE == DAC_OUTPUT_FRONT_LR_REAR_LR) || (TCFG_AUDIO_DAC_CONNECT_MODE == DAC_OUTPUT_DUAL_LR_DIFF))
        if (adc_data.mic_capless == 1) { //四声道与双声道差分 使用不省电容接法mic_capless = 0
            log_error("adc_data.mic_capless err, CHECK !!!\n");
            ASSERT(0);
        }
#endif
        adc_data.mic_bias_res = mic_type.pull_up;
        adc_data.mic_ldo_vsel = mic_type.ldo_lev;
    } else {
        log_debug("read mic type err\n");
        mic_type.type = 0;
        adc_data.mic_capless = mic_type.type;
    }
#endif

#if TCFG_MC_BIAS_AUTO_ADJUST
    ret = syscfg_read(CFG_MC_BIAS, &adc_data.mic_bias_res, 1);
    log_info("mic_bias_res:%d\n", adc_data.mic_bias_res);
    if (ret != 1) {
        log_info("mic_bias_adjust NULL\n");
    }
    u8 mic_ldo_idx;
    ret = syscfg_read(CFG_MIC_LDO_VSEL, &mic_ldo_idx, 1);
    if (ret == 1) {
        adc_data.mic_ldo_vsel = mic_ldo_idx & 0x3;
        log_info("mic_ldo_vsel:%d,%d\n", adc_data.mic_ldo_vsel, mic_ldo_idx);
    } else {
        log_info("mic_ldo_vsel_adjust NULL\n");
    }
#endif

#if USE_CONFIG_STATUS_SETTING
    /* g_printf("status_config:\n"); */
    log_info("status_config:\n");
    STATUS_CONFIG *status = (STATUS_CONFIG *)tmp;
    ret = syscfg_read(CFG_UI_TONE_STATUS_ID, status, sizeof(STATUS_CONFIG));
    if (ret > 0) {
        memcpy((u8 *)&status_config, (u8 *)status, sizeof(STATUS_CONFIG));
        log_info_hexdump(&status_config, sizeof(STATUS_CONFIG));
    }
#endif

#if USE_CONFIG_AUDIO_SETTING
    /* g_printf("app audio_config:\n"); */
    log_info("app audio_config:\n");
    ret = syscfg_read(CFG_AUDIO_ID, (u8 *)&audio_cfg, sizeof(AUDIO_CONFIG));
    if (ret > 0) {
        log_info_hexdump((u8 *)&audio_cfg, sizeof(AUDIO_CONFIG));
        log_info("max vol:%d default vol:%d tone vol:%d\n", audio_cfg.max_sys_vol, audio_cfg.default_vol, audio_cfg.tone_vol);
#else
    {
#endif
        /* dac_data.max_ana_vol = audio_cfg.max_sys_vol; */
        /* if (dac_data.max_ana_vol > 30) { */
        /* dac_data.max_ana_vol = 30; */
        /* } */

        //// pc usb mic gain
        s8 usb_mic_gain = -1;
        ret = syscfg_read(VM_USB_MIC_GAIN, &usb_mic_gain, 1);
        if (ret < 0) {
            usb_mic_gain  = 7;
        }
        app_var.usb_mic_gain = usb_mic_gain;
        log_info("usb_mic_gain: %d\n", app_var.usb_mic_gain);

        s8 default_volume = -1;
        s8 music_volume = -1;
#if SYS_DEFAULT_VOL
        default_volume = audio_cfg.default_vol;
#else
        ret = syscfg_read(CFG_SYS_VOL, &default_volume, 1);
        if (ret < 0) {
            default_volume = -1;
        }
        ret = syscfg_read(CFG_MUSIC_VOL, &music_volume, 1);
        if (ret < 0) {
            music_volume = -1;
        }
#endif
        if (default_volume > audio_cfg.max_sys_vol) {
            default_volume = audio_cfg.max_sys_vol;
        }
        if (default_volume <= 0) {
            default_volume = audio_cfg.max_sys_vol / 2;
        }

#if (SMART_BOX_EN)
        app_var.music_volume = music_volume < 0 ? default_volume : music_volume;
#else
        app_var.music_volume = music_volume <= 0 ? default_volume : music_volume;
#endif
        app_var.wtone_volume = audio_cfg.tone_vol;
        app_var.call_volume = app_var.aec_dac_gain;
        app_var.opid_play_vol_sync = app_var.music_volume * 127 / audio_cfg.max_sys_vol;

        log_info("max vol:%d default vol:%d tone vol:%d vol_sync:%d\n", audio_cfg.max_sys_vol, default_volume, audio_cfg.tone_vol, app_var.opid_play_vol_sync);
    }

#if (USE_CONFIG_CHARGE_SETTING) && (TCFG_CHARGE_ENABLE)
    /* g_printf("app charge config:\n"); */
    log_info("app charge config:\n");
    CHARGE_CONFIG *charge = (CHARGE_CONFIG *)tmp;
    ret = syscfg_read(CFG_CHARGE_ID, charge, sizeof(CHARGE_CONFIG));
    if (ret > 0) {
        log_info_hexdump(charge, sizeof(CHARGE_CONFIG));
        log_info("sw:%d poweron_en:%d full_v:%d full_mA:%d charge_mA:%d\n",
                 charge->sw, charge->poweron_en, charge->full_v, charge->full_c, charge->charge_c);
        memcpy((u8 *)&charge_data, (u8 *)charge, sizeof(CHARGE_CONFIG));
    }
#endif

#if (USE_CONFIG_KEY_SETTING) && (TCFG_ADKEY_ENABLE || TCFG_IOKEY_ENABLE)
    /* g_printf("app key config:\n"); */
    log_info("app key config:\n");
    KEY_OP *key_msg = (KEY_OP *)tmp;
    ret = syscfg_read(CFG_KEY_MSG_ID, key_msg, sizeof(KEY_OP) * KEY_NUM_MAX);
    if (ret > 0) {
        log_info_hexdump(key_msg, sizeof(KEY_OP) * KEY_NUM);
        memcpy(key_table, key_msg, sizeof(KEY_OP) * KEY_NUM);
    }

    log_info("key_msg:");
    log_info_hexdump((u8 *)key_table, KEY_EVENT_MAX * KEY_NUM_MAX);
#endif


#if USE_CONFIG_LOWPOWER_V_SETTING
    /* g_printf("auto low power config:\n"); */
    log_info("auto low power config:\n");
    AUTO_LOWPOWER_V_CONFIG auto_lowpower;
    ret = syscfg_read(CFG_LOWPOWER_V_ID, &auto_lowpower, sizeof(AUTO_LOWPOWER_V_CONFIG));
    if (ret > 0) {
        app_var.warning_tone_v = auto_lowpower.warning_tone_v;
        app_var.poweroff_tone_v = auto_lowpower.poweroff_tone_v;
    }
    log_info("warning_tone_v:%d poweroff_tone_v:%d\n", app_var.warning_tone_v, app_var.poweroff_tone_v);
#else
    app_var.warning_tone_v = LOW_POWER_WARN_VAL;
    app_var.poweroff_tone_v = LOW_POWER_OFF_VAL;
    log_info("warning_tone_v:%d poweroff_tone_v:%d\n", app_var.warning_tone_v, app_var.poweroff_tone_v);
#endif

#if USE_CONFIG_AUTO_OFF_SETTING
    /* g_printf("auto off time config:\n"); */
    log_info("auto off time config:\n");
    AUTO_OFF_TIME_CONFIG auto_off_time;
    ret = syscfg_read(CFG_AUTO_OFF_TIME_ID, &auto_off_time, sizeof(AUTO_OFF_TIME_CONFIG));
    if (ret > 0) 
	{
        app_var.auto_off_time = auto_off_time.auto_off_time * 60;
        log_info("read new cfg auto time %d\n", auto_off_time.auto_off_time);
    } 
	else 
	{
        auto_off_time.auto_off_time = 3;
        app_var.auto_off_time = auto_off_time.auto_off_time * 60;
        log_info("read auto time cfg err\n");
    }
    log_info("auto_off_time:%d\n", app_var.auto_off_time);
#else
    app_var.auto_off_time =  TCFG_AUTO_SHUT_DOWN_TIME;
    log_info("auto_off_time:%d\n", app_var.auto_off_time);
#endif

#if TCFG_APP_MUSIC_EN
    ret = syscfg_read(CFG_MUSIC_MODE, &app_var.cycle_mode, 1);
    if (ret < 0) {
        log_info("read music play mode err\n");
    }
    if (app_var.cycle_mode >= FCYCLE_MAX || app_var.cycle_mode == 0) {
        app_var.cycle_mode = FCYCLE_ALL;
    }
#endif

    /*************************************************************************/
    /*                      CFG READ IN VM                                   */
    /*************************************************************************/
#if TCFG_APP_BT_EN
    u8 mac_buf[6];
    u8 ble_mac_buf[6];
    u8 mac_buf_tmp[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    u8 mac_buf_tmp2[6] = {0, 0, 0, 0, 0, 0};
#if TCFG_USER_TWS_ENABLE
    int len = syscfg_read(CFG_TWS_LOCAL_ADDR, bt_cfg.tws_local_addr, 6);
    if (len != 6) {
        get_random_number(bt_cfg.tws_local_addr, 6);
        syscfg_write(CFG_TWS_LOCAL_ADDR, bt_cfg.tws_local_addr, 6);
        log_info(">>>init mac tws addr!!!\n");
    }
    log_info("tws_local_mac:");
    log_info_hexdump(bt_cfg.tws_local_addr, sizeof(bt_cfg.tws_local_addr));

#if CONFIG_TWS_USE_COMMMON_ADDR
    ret = syscfg_read(CFG_TWS_COMMON_ADDR, mac_buf, 6);
    if (ret != 6 || !memcmp(mac_buf, mac_buf_tmp, 6))
#endif
#endif

#if TCFG_SOUNDBOX_TOOL_ENABLE
        do {
            ret = syscfg_read_btmac_blemac_from_bin(CFG_BT_MAC_ADDR, mac_buf, 6);
            if ((ret != 6) || !memcmp(mac_buf, mac_buf_tmp, 6) || !memcmp(mac_buf, mac_buf_tmp2, 6)) {
                ret = syscfg_read(CFG_BT_MAC_ADDR, mac_buf, 6);
                if ((ret != 6) || !memcmp(mac_buf, mac_buf_tmp, 6) || !memcmp(mac_buf, mac_buf_tmp2, 6)) {
                    get_random_number(mac_buf, 6);
                    syscfg_write(CFG_BT_MAC_ADDR, mac_buf, 6);
                    log_info(">>>init tws addr!!!\n");
                }
            }
            ret = 0;
            ret = syscfg_read_btmac_blemac_from_bin(CFG_BLE_MAC_ADDR, ble_mac_buf, 6);
            if ((ret != 6) || !memcmp(ble_mac_buf, mac_buf_tmp, 6) || !memcmp(ble_mac_buf, mac_buf_tmp2, 6)) {
                ret = syscfg_read(CFG_BLE_MAC_ADDR, ble_mac_buf, 6);
                if ((ret != 6) || !memcmp(ble_mac_buf, mac_buf_tmp, 6) || !memcmp(ble_mac_buf, mac_buf_tmp2, 6)) {
                    get_random_number(ble_mac_buf, 6);
                    syscfg_write(CFG_BLE_MAC_ADDR, ble_mac_buf, 6);
                    log_info(">>>init ble addr!!!\n");
                }
            }
        } while (0);
#else
        do {
            ret = syscfg_read(CFG_BT_MAC_ADDR, mac_buf, 6);
            if ((ret != 6) || !memcmp(mac_buf, mac_buf_tmp, 6) || !memcmp(mac_buf, mac_buf_tmp2, 6)) {
                get_random_number(mac_buf, 6);
                syscfg_write(CFG_BT_MAC_ADDR, mac_buf, 6);
                log_info(">>>init tws addr!!!\n");
            }
            ret = 0;
            ret = syscfg_read(CFG_BLE_MAC_ADDR, ble_mac_buf, 6);
            if ((ret != 6) || !memcmp(ble_mac_buf, mac_buf_tmp, 6) || !memcmp(ble_mac_buf, mac_buf_tmp2, 6)) {
                get_random_number(ble_mac_buf, 6);
                syscfg_write(CFG_BLE_MAC_ADDR, ble_mac_buf, 6);
                log_info(">>>init ble addr!!!\n");
            }
        } while (0);
#endif

    syscfg_read(CFG_BT_MAC_ADDR, bt_mac_addr_for_testbox, 6);

#if CONFIG_TWS_USE_COMMMON_ADDR
    if (!memcmp(bt_mac_addr_for_testbox, mac_buf_tmp, 6)) {
        get_random_number(bt_mac_addr_for_testbox, 6);
        syscfg_write(CFG_BT_MAC_ADDR, bt_mac_addr_for_testbox, 6);
        log_info(">>>init mac addr!!!\n");
    }
#endif

    log_info("mac:");
    log_info_hexdump(mac_buf, sizeof(mac_buf));
    memcpy(bt_cfg.mac_addr, mac_buf, 6);
    log_info("ble mac:");
    log_info_hexdump(ble_mac_buf, sizeof(ble_mac_buf));
    memcpy(bt_cfg.ble_mac_addr, ble_mac_buf, 6);


#if (CONFIG_BT_MODE != BT_NORMAL)
    const u8 dut_name[]  = "AC693x_DUT";
    const u8 dut_addr[6] = {0x12, 0x34, 0x56, 0x56, 0x34, 0x12};
    memcpy(bt_cfg.edr_name, dut_name, sizeof(dut_name));
    memcpy(bt_cfg.mac_addr, dut_addr, 6);
#endif

    /*************************************************************************/
    /*                      CFG READ IN isd_config.ini                       */
    /*************************************************************************/
    LRC_CONFIG lrc_cfg;
    ret = syscfg_read(CFG_LRC_ID, &lrc_cfg, sizeof(LRC_CONFIG));
    if (ret > 0) {
        log_info("lrc cfg:");
        log_info_hexdump(&lrc_cfg, sizeof(LRC_CONFIG));
        lp_winsize.lrc_ws_inc      = lrc_cfg.lrc_ws_inc;
        lp_winsize.lrc_ws_init     = lrc_cfg.lrc_ws_init;
        lp_winsize.bt_osc_ws_inc   = lrc_cfg.btosc_ws_inc;
        lp_winsize.bt_osc_ws_init  = lrc_cfg.btosc_ws_init;
        lp_winsize.osc_change_mode = lrc_cfg.lrc_change_mode;
    } else {
        log_info("read new lrc cfg err\n");
    }
    /* printf("%d %d %d \n",lp_winsize.lrc_ws_inc,lp_winsize.lrc_ws_init,lp_winsize.osc_change_mode); */
    lp_winsize_init(&lp_winsize);
#endif
}

extern void hci_vendor_update_name(void);
int bt_modify_name(u8 *new_name)
{
    u8 new_len = strlen(new_name);

    if (new_len >= LOCAL_NAME_LEN) 
	{
        new_name[LOCAL_NAME_LEN - 1] = 0;
    }

    if (strcmp(new_name, bt_cfg.edr_name)) 
	{
        syscfg_write(CFG_BT_NAME, new_name, LOCAL_NAME_LEN);
        memcpy(bt_cfg.edr_name, new_name, LOCAL_NAME_LEN);
        hci_vendor_update_name();
        log_info("mdy_name sucess\n");
        return 1;
    }
    return 0;
}


char *get_edr_name(void)
{
    return bt_cfg.edr_name;
}


#ifdef SUPPORT_KPH_DEF

/*-----------------------------------------------------
		customer add function
-----------------------------------------------------*/

#include "app_task.h"
#include "key_event_deal.h"
#include "asm/spi.h"
#include "btstack/avctp_user.h"

type_use_ctl use_ctl;
typdef_led_bat led_bat;

#if SUPPORT_USER_EQ_MODE
extern const char  tre_table[TRE_MAX+1];
extern const char  bass_table[BASS_MAX+1];
#endif

#ifdef SUPPORT_SD_DAT_MULTIPLEX_AD
u16 bSdCmdAdVal;
extern u32 adc_sample(u32 ch);
extern u8 sd_io_suspend(u8 sdx, u8 sd_io);
extern u8 sd_io_resume(u8 sdx, u8 sd_io);
#endif

u8 app_common_key_var_event(u32 key_var)
{
    struct sys_event e = {0};
    e.type = SYS_KEY_EVENT;
    e.u.key.type = KEY_DRIVER_TYPE_USER_MSG;
    e.u.key.event = key_var;
  //  e.arg  = (void *)DEVICE_EVENT_FROM_KEY;
    use_ctl.tws_key_disable = 15;
    use_ctl.key_msg = 1;
    sys_event_notify(&e);
    return true;
}


u8 app_common_key_var_event_tws_sync(u32 key_var,u32 key_val)
{
    struct sys_event e = {0};
    e.type = SYS_KEY_EVENT;
    e.u.key.type = KEY_DRIVER_TYPE_USER_MSG;
    e.u.key.event = key_var;
    e.u.key.value = key_val;
  //  e.arg  = (void *)DEVICE_EVENT_FROM_KEY;
    use_ctl.tws_key_disable = 15;
    use_ctl.key_msg = 1;
    sys_event_notify(&e);
    return true;
}

// ch: FL:BIT(0) FR:BIT(1) RL:BIT(2) RR:BIT(3)
// 例如：BIT(0)是 FL 左声道， BIT(1)是 FR 右声道， FLR BIT(0)|bit(1)是立体声
// MUTE:1:MUTE 0:UNMUTE
///audio_dac_mute_ch(BIT(0)|BIT(1),1);
void audio_dac_mute_ch(u8 ch,u8 mute)
{
    if (mute)
    {
        JL_ANA->DAA_CON1 &= ~((ch & 0xF) << 20);
        JL_ANA->DAA_CON1 &= ~((ch & 0xF) << 24);
    }
    else
    {
        JL_ANA->DAA_CON1 |= ((ch & 0xF) << 20);
        JL_ANA->DAA_CON1 |= ((ch & 0xF) << 24);
    }
}

#ifdef board_DG_1258_ac6951c
#include "asm/power/p33.h" 
#endif

#ifdef SUPPORT_AMP_PA_MUTE
void pa_mute(void)
{
   use_ctl.pa_mute_sta = 1;
#ifdef board_DG_1258_ac6951c
   p33_tx_1byte(R3_OSL_CON, 0);
#endif
   
   PA_MUTE(); 
}

void pa_unmute(void)
{
  use_ctl.pa_mute_sta = 0;
  if(use_ctl.pa_delay_unmute)return;
  #ifdef SUPPORT_PA_FM_MODE_1V5
   if (app_get_curr_task() == APP_FM_TASK) 
    {
		
#ifdef board_DG_1258_ac6951c
		   p33_tx_1byte(R3_OSL_CON, 0);
#endif
		
      FM_UNMUTE(); 
   }else{
   	
#ifdef board_DG_1258_ac6951c
	   p33_tx_1byte(R3_OSL_CON, 0);
#endif
	
      PA_UNMUTE(); 
   }
  #else
  PA_UNMUTE(); 
  #endif
}
#endif


#ifdef SUPPORT_ENCODER

#if defined ENCODE_PIN_USE_PA1PA6 

#define A_BIT     BIT(1)
#define B_BIT     BIT(6)
#define A_PORT     JL_PORTA
#define B_PORT     JL_PORTA

#define ENCODE_PORTB_IN()		A_PORT->DIE |= A_BIT;A_PORT->DIR |= A_BIT;A_PORT->PU|= A_BIT;A_PORT->PD&=~ A_BIT;  \
								B_PORT->DIE |= B_BIT;B_PORT->DIR |= B_BIT;B_PORT->PU|= B_BIT; B_PORT->PD&=~ B_BIT;  
#define  BM_DN_IS_H()			(A_PORT->IN & A_BIT)
#define  BM_UP_IS_H()			(B_PORT->IN & B_BIT)   

#elif defined ENCODE_PIN_USE_PA6PB8

#define A_BIT     BIT(6)
#define B_BIT     BIT(8)
#define A_PORT     JL_PORTA
#define B_PORT     JL_PORTB

#define ENCODE_PORTB_IN()		A_PORT->DIE |= A_BIT;A_PORT->DIR |= A_BIT;A_PORT->PU|= A_BIT;A_PORT->PD&=~ A_BIT;  \
								B_PORT->DIE |= B_BIT;B_PORT->DIR |= B_BIT;B_PORT->PU|= B_BIT; B_PORT->PD&=~ B_BIT;  
#define  BM_DN_IS_H()			(A_PORT->IN & A_BIT)
#define  BM_UP_IS_H()			(B_PORT->IN & B_BIT)   

#endif

#ifdef SUPPORT_ENCODER_2_IO

#ifdef SUPPORT_ENCORD_LED 

AT(.volatile_ram_code)
void Encoder_LED(void)
{
 static u8 cnt;
 

if (app_get_curr_task() == APP_IDLE_TASK)
{
	ENCORD_LED_OFF();
	return;
}

if(use_ctl.bEncordSw==0)
{
	ENCORD_LED_ON();
	return;
}

#ifdef SUPPORT_ENCORD_LED_FLASH 

cnt++;

 if(cnt<50)
 {
	ENCORD_LED_ON();
 }
 else if(cnt<100)
 {
	ENCORD_LED_OFF();
 } 	
 else
 	cnt=0;
#else
	ENCORD_LED_ON();
#endif 

}
#endif

//#define ENCODER_MODE_2
#ifndef KEY_Encoder_MENU_UP 
#define KEY_Encoder_MENU_UP KEY_MENU_SEL_UP
#endif

#ifndef KEY_Encoder_MENU_DN
#define KEY_Encoder_MENU_DN KEY_MENU_SEL_DN
#endif

void ENC_Encoder(void)
{
    static u8 Aold = 0, Bold = 0; //定义了两个变量用来储蓄上一次调用此方法是编码开关两引脚的电平
    static u8 st1 = 0, st2 = 0;   //定义了一个变量用来储蓄以前是否出现了两个引脚都为高电平的状态


	if (app_get_curr_task() == APP_IDLE_TASK)
		return;


	ENCODE_PORTB_IN();

    if ((BM_DN_IS_H() && BM_UP_IS_H())) 
	{
        st1 = 1;
    }

    else if (((!BM_DN_IS_H()) && (!BM_UP_IS_H()))) 
	{
        st2 = 1;
    }



    if (st1) 		//初始状态是A1B1
    {
        if (((!BM_DN_IS_H()) && (!BM_UP_IS_H()))) 		//执行的是A0B0这个状态的前一个状态
	 	{
	        if (Bold&&(!Aold)) 		//A0B0的前一个状态是A0B1	
	        {   //为高说明编码开关在向加大的方向转
                st1 = 0;

				if(use_ctl.disable_key_cnt!=0)return;//防止编码器转动一格跳两首
					
				#ifdef SUPPORT_ENCORD_LED // PC7
				use_ctl.bEncordSw=5;
				#endif				
				app_common_key_var_event(KEY_Encoder_MENU_UP);
				use_ctl.disable_key_cnt=3;					
            }

            if (Aold&&(!Bold)) 					//A1B0
	        {   								//为高说明编码开关在向减小的方向转
				st1 = 0;
				if(use_ctl.disable_key_cnt!=0)return;//防止编码器转动一格跳两首
				#ifdef SUPPORT_ENCORD_LED // PC7
				use_ctl.bEncordSw=5;
				#endif				
				use_ctl.disable_key_cnt=3;					
            }
        }
    }

    if (st2) //初始状态是A0B0
    {
        if (((BM_DN_IS_H()) && (BM_UP_IS_H())))  //执行的是A1B1这个状态的前一个状态
	 {
           
			if ((!Bold)&&Aold)	//A1B1的前一个状态是A1B0
			{   //为高说明编码开关在向加大的方向转
				st2 = 0;
				if(use_ctl.disable_key_cnt!=0)  
					return;//防止编码器转动一格跳两首
				#ifdef SUPPORT_ENCORD_LED 
					use_ctl.bEncordSw=5;
				#endif				
				app_common_key_var_event(KEY_Encoder_MENU_UP);
				use_ctl.disable_key_cnt=3;					
			}

			if ((!Aold)&&Bold)	//A1B1的前一个状态是A0B1
			{   //为高说明编码开关在向减小的方向转
				//printf("enter\n");
				st2 = 0;
			// 	if(use_ctl.disable_key_cnt!=0)
			// 		return;//防止编码器转动一格跳两首
			// #ifdef SUPPORT_ENCORD_LED 
			// 	use_ctl.bEncordSw=5;
			// #endif		
			// 			use_ctl.disable_key_cnt=1;		
						app_common_key_var_event(KEY_Encoder_MENU_DN);
											
			}
        }
    }

    if (BM_DN_IS_H()) 
	{
        Aold = 1;
    } 
	else 
	{
        Aold = 0;
    }

    if (BM_UP_IS_H()) 
	{
        Bold = 1;
    } 
	else 
	{
        Bold = 0;
    }
}



#if defined SUPPORT_DOUBLE_ENCODER




#if defined ENCODE2_PIN_USE_PR1PB10

#define ENCODE2_PORTB_IN()		rtc_port_pr_die(1, 1);   rtc_port_pr_in(1);rtc_port_pr_pu(1, 1);rtc_port_pr_pd(1, 0);JL_PORTB->DIE |= BIT(10);JL_PORTB->DIR |= BIT(10);JL_PORTB->PU|= BIT(10); JL_PORTB->PD&=~ BIT(10);  
#define  BM2_DN_IS_H()			(rtc_port_pr_read(1))   
#define  BM2_UP_IS_H()			(JL_PORTB->IN & BIT(10) )
#elif defined ENCODE2_PIN_USE_PA1PA5
#define ENCODE2_PORTB_IN()		JL_PORTA->DIE |= BIT(1);JL_PORTA->DIR |= BIT(1);JL_PORTA->PU|= BIT(1);JL_PORTA->DIE |= BIT(5);JL_PORTA->DIR |= BIT(5);JL_PORTA->PU|= BIT(5); JL_PORTA->PD&=~ BIT(5);  
#define  BM2_DN_IS_H()			(JL_PORTA->IN & BIT(1) )   
#define  BM2_UP_IS_H()			(JL_PORTA->IN & BIT(5) )
#elif defined ENCODE2_PIN_USE_PC7PA1
#define ENCODE2_PORTB_IN()		JL_PORTA->DIE |= BIT(1);JL_PORTA->DIR |= BIT(1);JL_PORTA->PU|= BIT(1);JL_PORTC->DIE |= BIT(7);JL_PORTC->DIR |= BIT(7);JL_PORTC->PU|= BIT(7); JL_PORTC->PD&=~ BIT(7);  
#define  BM2_DN_IS_H()			(JL_PORTA->IN & BIT(1) )   
#define  BM2_UP_IS_H()			(JL_PORTC->IN & BIT(7) )
#elif defined ENCODE2_PIN_USE_PC7PC3
#define ENCODE2_PORTB_IN()		JL_PORTC->DIE |= BIT(3);JL_PORTC->DIR |= BIT(3);JL_PORTC->PU|= BIT(3);JL_PORTC->PD&=~ BIT(3);JL_PORTC->DIE |= BIT(7);JL_PORTC->DIR |= BIT(7);JL_PORTC->PU|= BIT(7); JL_PORTC->PD&=~ BIT(7);  
#define  BM2_DN_IS_H()			(JL_PORTC->IN & BIT(7) )   
#define  BM2_UP_IS_H()			(JL_PORTC->IN & BIT(3) )
#elif defined ENCODE2_PIN_USE_PA3PC7 // MICVOL  
#define ENCODE2_PORTB_IN()		JL_PORTA->DIE |= BIT(3);JL_PORTA->DIR |= BIT(3);JL_PORTA->PU|= BIT(3);JL_PORTA->PD&=~ BIT(3);JL_PORTC->DIE |= BIT(7);JL_PORTC->DIR |= BIT(7);JL_PORTC->PU|= BIT(7); JL_PORTC->PD&=~ BIT(7);  
#define  BM2_DN_IS_H()			(JL_PORTA->IN & BIT(3) )   
#define  BM2_UP_IS_H()			(JL_PORTC->IN & BIT(7) )
#elif defined ENCODE2_PIN_USE_PA3PC2

#define A2_BIT     BIT(3)
#define B2_BIT     BIT(2)
#define A2_PORT     JL_PORTA
#define B2_PORT     JL_PORTC

#define ENCODE2_PORTB_IN()		A2_PORT->DIE |= A2_BIT;A2_PORT->DIR |= A2_BIT;A2_PORT->PU|= A2_BIT;A2_PORT->PD&=~ A2_BIT;  \
								B2_PORT->DIE |= B2_BIT;B2_PORT->DIR |= B2_BIT;B2_PORT->PU|= B2_BIT; B2_PORT->PD&=~ B2_BIT;  
#define  BM2_DN_IS_H()			(A2_PORT->IN & A2_BIT)
#define  BM2_UP_IS_H()			(B2_PORT->IN & B2_BIT)   

#elif defined ENCODE2_PIN_USE_PC5PA10

#define A2_BIT     BIT(5)
#define B2_BIT     BIT(10)
#define A2_PORT     JL_PORTC
#define B2_PORT     JL_PORTA

#define ENCODE2_PORTB_IN()		A2_PORT->DIE |= A2_BIT;A2_PORT->DIR |= A2_BIT;A2_PORT->PU|= A2_BIT;A2_PORT->PD&=~ A2_BIT;  \
								B2_PORT->DIE |= B2_BIT;B2_PORT->DIR |= B2_BIT;B2_PORT->PU|= B2_BIT; B2_PORT->PD&=~ B2_BIT;  
#define  BM2_DN_IS_H()			(A2_PORT->IN & A2_BIT)
#define  BM2_UP_IS_H()			(B2_PORT->IN & B2_BIT)   

#endif


#ifndef KEY_Encoder_MENU_MIC_UP 
#define KEY_Encoder_MENU_MIC_UP KEY_MENU_MIC_SEL_UP
#endif

#ifndef KEY_Encoder_MENU_MIC_DN
#define KEY_Encoder_MENU_MIC_DN KEY_MENU_MIC_SEL_DN
#endif


AT(.volatile_ram_code)
void ENC_Encoder_2(void)
{
    static u8 Aold = 0, Bold = 0; //定义了两个变量用来储蓄上一次调用此方法是编码开关两引脚的电平
    static u8 st1 = 0, st2 = 0; //定义了一个变量用来储蓄以前是否出现了两个引脚都为高电平的状态

if (app_get_curr_task() == APP_IDLE_TASK)
	return;


	ENCODE2_PORTB_IN();

    if ((BM2_DN_IS_H() && BM2_UP_IS_H())) {
        st1 = 1;
    }

    else if (((!BM2_DN_IS_H()) && (!BM2_UP_IS_H()))) {
        st2 = 1;
    }

    if (st1) 		//初始状态是A1B1
    {
        if (((!BM2_DN_IS_H()) && (!BM2_UP_IS_H()))) 		//执行的是A0B0这个状态的前一个状态
	 {
            //if (Bold)  //不能只判断B,就认为A为0的,不然音量加到最大后扭快点会有反弹现象,就是有减小再加回的现象
	     if (Bold&&(!Aold)) 		//A0B0的前一个状态是A0B1	
	     {   //为高说明编码开关在向加大的方向转
                st1 = 0;
                    app_task_put_key_msg(KEY_Encoder_MENU_MIC_UP, 1);//卸载了设备再执行
//	                app_common_key_var_event(KEY_Encoder_MENU_MIC_UP);
            }
            //if (Aold)  //不能只判断A,就认为B为0的,不然音量减到0后扭快点会有反弹现象,就是有加大再减小的现象
            if (Aold&&(!Bold)) 	//A0B0的前一个状态是A0B1
	     {   //为高说明编码开关在向减小的方向转
                st1 = 0;
                    app_task_put_key_msg(KEY_Encoder_MENU_MIC_DN, 1);//卸载了设备再执行
//	                app_common_key_var_event(KEY_Encoder_MENU_MIC_DN);
            }
        }
    }

    if (st2) //初始状态是A0B0
    {
        if (((BM2_DN_IS_H()) && (BM2_UP_IS_H())))  //执行的是A1B1这个状态的前一个状态
	 {
            //if (!Bold) 
            if ((!Bold)&&Aold)	//A1B1的前一个状态是A1B0
	     {   //为高说明编码开关在向加大的方向转
                st2 = 0;
                    app_task_put_key_msg(KEY_Encoder_MENU_MIC_UP, 1);//卸载了设备再执行
//	                app_common_key_var_event(KEY_Encoder_MENU_MIC_UP);
            }
            //if (!Aold) 
            if ((!Aold)&&Bold)	//A1B1的前一个状态是A0B1
	     {   //为高说明编码开关在向减小的方向转
                st2 = 0;
                    app_task_put_key_msg(KEY_Encoder_MENU_MIC_DN, 1);//卸载了设备再执行
//	                app_common_key_var_event(KEY_Encoder_MENU_MIC_DN);
            }
        }
    }

    if (BM2_DN_IS_H()) {
        Aold = 1;
    } else {
        Aold = 0;
    }

    if (BM2_UP_IS_H()) {
        Bold = 1;
    } else {
        Bold = 0;
    }

}

#endif

#endif
#endif


// SPI 5050 LED 
#ifdef SPI1_RGB_DEV_EN
#if defined SPI1_RGB_RT_32LED
#include"SPI1_RGB_RT_32LED.c"
#elif defined SPI_RGB_JY_28LED
#include"SPI_RGB_JY_28LED.c"
#elif defined SPI_RGB_JY_28LED_GB12
#include"SPI_RGB_JY_28LED_GB12.c"
#elif defined SPI_RGB_JY_28LED_GB15
#include"SPI_RGB_JY_28LED_GB15.c"
#elif defined SPI_RGB_JY_24LED_GB8
#include"SPI_RGB_JY_24LED_GB8.c"
#elif defined SPI_RGB_JY_40LED_GB15
#include"SPI_RGB_JY_40LED_GB15.c"
#elif defined SPI_RGB_JY_12LED
#include"SPI_RGB_JY_12LED.c"
#elif defined SPI_RGB_JY_24LED
#include"SPI_RGB_JY_24LED.c"
#elif defined SPI_RGB_JY_18LED
#include"SPI_RGB_JY_18LED.c"
#elif defined SPI_RGB_JY_56LED_TX_2210
#include"SPI_RGB_JY_56LED_TX_2210.c"
#elif defined SPI_RGB_JY_48LED_TX_2208
#include"SPI_RGB_JY_48LED_TX_2208.c"
#elif defined SPI_RGB_JY_118LED_TX_2810
#include"SPI_RGB_JY_118LED_TX_2810.c"
#elif defined SPI_RGB_JY_64LED_TX_2212
#include"SPI_RGB_JY_64LED_TX_2212.c"
#elif defined SPI_RGB_JY_124LED_TX_2310
#include"SPI_RGB_JY_124LED_TX_2310.c"
#elif defined SPI_RGB_JY_TX_2308
#include"SPI_RGB_JY_TX_2308.c"
#elif defined SPI_RGB_JY_TX_2728_32RGB
#include"SPI_RGB_JY_TX_2728_32RGB.c"
#elif defined SPI_RGB_JY_TX_2580_50RGB
#include"SPI_RGB_JY_TX_2580_50RGB.c"
#elif defined SPI_RGB_JY_88LED_TX_2608
#include"SPI_RGB_JY_88LED_TX_2608.c"
#elif defined SPI1_RGB_DG_1257_73
#include"SPI1_RGB_DG_1257_73.c"
#elif defined SPI1_RGB_DS4751C_FL872
#include"SPI1_RGB_DS4751C_FL872.c"
#elif defined SPI1_RGB_DG_1257_79
#include"SPI1_RGB_DG_1257_79.c"
#elif defined SPI1_RGB_DG_1258_106
#include"SPI1_RGB_DG_1258_106.c"
#elif defined SPI1_RGB_LH_808_20
#include"SPI1_RGB_LH_808_20.c"
#elif defined SPI1_RGB_DG_1263_2
#include"SPI1_RGB_DG_1263_2.c"
#elif defined SPI1_RGB_sub2100
#include"SPI1_RGB_sub2100.c"
#elif defined SPI_RGB_JY48LED_TX_2208
#include"SPI_RGB_JY48LED_TX_2208.c"
#elif defined SPI_RGB_JY_56LED_TX_2210
#include"SPI_RGB_JY_56LED_TX_2210.c"

#elif defined SPI1_RGB_TEST_NUMBERS_OF_RGB
#include"SPI1_RGB_TEST_NUMBERS_OF_RGB.c"

#elif defined SPI_RGB_KM_24LED
#include"SPI_RGB_KM_24LED.c"

#endif
#endif


#ifdef SUPPORT_PP_LED
#include "audio_dec_file.h"

AT(.volatile_ram_code)
void led_scan(void)
{
	static u8 cnt;
	
	 if (app_get_curr_task() == APP_IDLE_TASK) 
	 {
		LED1_OFF();
		return;
	 }
	 if ((app_get_curr_task() == APP_POWERON_TASK) ||(app_get_curr_task() == APP_LINEIN_TASK))
	 {
		LED1_ON();
		return;
	 }	
#if TCFG_APP_FM_EN	 
	  if (app_get_curr_task() == APP_FM_TASK) 
	 {
		 if(fm_scan.scan_flag) 
		 	{
			 	cnt++;
					if(cnt==20)
					{
						LED1_ON();
					}
					else if(cnt==40)
					{
					      cnt=0;
						LED1_OFF();
					}		 	
			}
		 else
		 	{
				LED1_ON();
				cnt=0;
			 }
		return;		 
	 }	 
#endif	  
	  if (app_get_curr_task() == APP_MUSIC_TASK) 
	 {
	  if(app_var.cycle_mode == FCYCLE_ALL)
	  	{
		 if (true == file_dec_is_play())
		 	{
			 	cnt++;
				if(cnt==50)
					{
						LED1_ON();
					}
					else if(cnt==100)
					{
					      cnt=0;
						LED1_OFF();
					}			
			}
		 else
		 	{
				LED1_ON();
				cnt=0;
			 }
	  	}
	  else if(app_var.cycle_mode == FCYCLE_ONE)
		    	{
			 	cnt++;
				if(cnt==20)
					{
						LED1_ON();
					}
					else if(cnt==40)
					{
					      cnt=0;
						LED1_OFF();
					}		 	
			}	  	
		return;		 
	 }	

	  if (app_get_curr_task() == APP_BT_TASK) 
	 {
		 	
	         if(BT_STATUS_PLAYING_MUSIC == get_bt_connect_status())    
		    	{
			 	cnt++;
				if(cnt==50)
					{
						LED1_ON();
					}
					else if(cnt==100)
					{
					      cnt=0;
						LED1_OFF();
					}						
		    	}
		    else if(BT_STATUS_CONNECTING == get_bt_connect_status())
		      {
		 		cnt=0;	
		 		LED1_ON();	
		    	}
		    else
		    	{
			 	cnt++;
				if(cnt==20)
					{
						LED1_ON();
					}
					else if(cnt==40)
					{
					      cnt=0;
						LED1_OFF();
					}		 	
			}
		return;
			
	 }	 	 	
}

#endif

#ifdef BAT_EXTERN_AD_EN
u16 battery_val;
u16 bat_val_buf[10];

u8 bat_check_cnt;
u8 static bat_same_cnt = 0;
bool bat_low_vol_down;
void battery_ad_check(void)  // 10ms
{
      u8 static bufcnt=0;
      u8 static offcnt=0;
      u8 static bCnt=0;




 #ifdef SUPPORT_SD_DAT_MULTIPLEX_AD
   battery_val = bSdCmdAdVal;
#else
      battery_val =adc_get_value(AD_CH_BAT);
#endif

     //printf("\n battery_val=%d",battery_val);
#ifdef SUPPORT_BAT_SHOW_TEST	  
   return;
#endif  
      bat_val_buf[bufcnt] = battery_val;
      bufcnt++;
      if(bufcnt>=10)bufcnt=0;
      if(bufcnt != 0)return;
						  // 100ms
      battery_val = ( bat_val_buf[0]+bat_val_buf[1]+bat_val_buf[2]+bat_val_buf[3]+bat_val_buf[4]+bat_val_buf[5]+\
                      bat_val_buf[6]+bat_val_buf[7]+bat_val_buf[8]+bat_val_buf[9] )/10;
    //   printf("\n battery_val=%d",battery_val);
     
	 #ifdef SUPPORT_BAT_SHOW_TEST     
	 	return;
	 #endif
	 
      //use_ctl.test = battery_val;
      #ifdef BAT_H_VAL
	      if(battery_val > BAT_FULL_VAL)   led_bat.power_cur= POWER_FULL; 
	      else if(battery_val > BAT_H_VAL)   led_bat.power_cur =  POWER_H; 
	      else if(battery_val > BAT_L_VAL)   led_bat.power_cur =  POWER_L; 
	      else if(battery_val > BAT_LOW_VAL)   led_bat.power_cur =  POWER_LOW; 
	      else led_bat.power_cur =  NO_POWER;
      #else
	      if(battery_val > BAT_FULL_VAL)   led_bat.power_cur= POWER_FULL;
	      #ifdef BAT_4_VAL
	      else if(battery_val > BAT_4_VAL)   led_bat.power_cur =  POWER_4;
	      #endif
	      else if(battery_val > BAT_3_VAL)   led_bat.power_cur =  POWER_3;
	      else if(battery_val > BAT_2_VAL)   led_bat.power_cur =  POWER_2;
	      else if(battery_val > BAT_1_VAL)   led_bat.power_cur =  POWER_1;
	      else if(battery_val > BAT_LOW_VAL) {  led_bat.power_cur =  POWER_LOW;  printf("11111111111111111111\n");}
	      else  led_bat.power_cur =  NO_POWER;
      #endif
      #ifdef SUPPOT_BAT_IN_CHARGE_DET
      if(led_bat.status == IN_CHARGE){
          if(battery_val > BAT_FULL_VAL_1)   led_bat.power_cur= POWER_FULL;
          else if(battery_val > BAT_4_VAL_1)   led_bat.power_cur =  POWER_4;
          else if(battery_val > BAT_3_VAL_1)   led_bat.power_cur =  POWER_3;
          else if(battery_val > BAT_2_VAL_1)   led_bat.power_cur =  POWER_2;
          else if(battery_val > BAT_1_VAL_1)   led_bat.power_cur =  POWER_1;
          else if(battery_val > BAT_LOW_VAL_1)   led_bat.power_cur =  POWER_LOW;
          else  led_bat.power_cur =  NO_POWER;
      }
      #endif
      
      if(led_bat.power_pre != led_bat.power_cur)
     {
          led_bat.power_pre = led_bat.power_cur;
          bat_same_cnt = 0;
      }
    else
      {
          bat_same_cnt++;
          if(bat_same_cnt > 4)  // 400ms
              led_bat.power = led_bat.power_cur;
      }
	static u8 cnt_idle=0;

#ifdef BAT_MUSIC_VOL_SET_VAL
	if (battery_val < BAT_MUSIC_VOL_SET_VAL)
	{
		eq_globa_gain_set( (eq_CUSTOM_globa_gain_get() - BAT_MUSIC_VOL_VAL) );
	}
#endif		 
	
#ifdef SUPPORT_POWER_ON_AD_SHOW_DLY
	// first  power on  disable  bat function run 
	if(use_ctl.bPowerOnShowDlyCnt) return;
#endif		 


      if(led_bat.status == NOT_CHARGE)
      {


           #if 1 //ndef SUPPORT_LOW_POWER_NO_STANDBY
           
           if(led_bat.power == NO_POWER)
	    {

              offcnt++;
              if(offcnt>15)
		{
		 #ifdef SUPPORT_MOS_POWER_ON			  	
		 	{
		 		app_common_key_var_event(KEY_STANBY_MOS);
		 	}
		 #else
		 	if (app_get_curr_task() != APP_IDLE_TASK) 
			{
				if (cnt_idle++>20)
				{
					app_common_key_var_event(KEY_STANBY);
				}
				else if ( cnt_idle==1 )
				{
					app_common_key_var_event(KEY_LOW_POWER);
				}
				
			 #if defined board_ac695x_JY_tx_2212_64led_ac6951c || defined board_ac695x_JY_tx_2310_64led_ac6951c || defined JY_tx_2208_7v4_48led_ac6951c || defined board_ac695x_JY_tx_2210_56led_ac6951c || defined board_ac695x_JY_TX_2208_48LED_AC6951C
			 	JL_PORTC->DIE |= BIT(4);JL_PORTC->DIR &= ~BIT(4);  JL_PORTC->PU &= ~BIT(4);JL_PORTC->PD &= ~BIT(4);JL_PORTC->OUT |= BIT(4);
			 #endif
		 	}		
		 #endif				  
              }
           }
		   else
		   {
              offcnt = 0;
           }
           #endif
		   
           #ifdef SUPPORT_LOWPOWER_VOICE
	           #ifdef SUPPORT_LOW_POWER_NO_STANDBY
		           if((led_bat.power == POWER_LOW)||(led_bat.power == NO_POWER))
	           #else
		           if(led_bat.power == POWER_LOW)
	           #endif
			   {
             // puts("\n lowe");
             // printf("\nlw=%04x\n",use_ctl.send_lowpower_cnt);
               #ifdef SUPPORT_FW_BAT_LOW_MODE
	                if((use_ctl.send_lowpower_cnt==1)||(use_ctl.send_lowpower_cnt==1))
			  {
		                  bat_low_vol_down=1;
		                  os_taskq_post_msg("MainTask", 1, MSG_BAT_L_VOL);
	                }
                #endif

	 	 #ifdef SUPPORT_LOWPOWER_VOL_AUTO_ADJUST
			 if(led_bat.bBatState!=22)
			 	{
					 led_bat.bBatState=22;
	 	 			audio_dac_vol_set(0x01,BIT(0),led_bat.bBatState,1);
	 	 			audio_dac_vol_set(0x01,BIT(1),led_bat.bBatState,1);	 
			 	}
	 	 #endif

				
				 if(use_ctl.send_lowpower_cnt)
				  use_ctl.send_lowpower_cnt--;
				
	              if(use_ctl.send_lowpower_cnt==0){ // 10s onice 

			 #ifdef SUPPORT_LOWPOWER_VOL_AUTO_ADJUST			 
			 	led_bat.bBatStateCnt = 0;			 
			 #endif
				  
	              #ifdef LOWPOWER_VOICE_TIMER
	                 use_ctl.send_lowpower_cnt=LOWPOWER_VOICE_TIMER*10;
			#else
	                 use_ctl.send_lowpower_cnt=100;
			#endif
			
			 #ifdef SUPPORT_LOWPOWER_3Timer_POWEROFF	
			 
		 	      if(use_ctl.bLowPowerOffTimer<3)
				  use_ctl.bLowPowerOffTimer++;
				  else  if(use_ctl.bLowPowerOffTimer==3)
				  {
					use_ctl.bLowPowerOffTimer=4;
					if (app_get_curr_task() != APP_IDLE_TASK) 
					{
						app_common_key_var_event(KEY_STANBY);

					}
				  }	
				  else
			 #endif
			
	                 app_common_key_var_event(KEY_LOW_POWER);
	              }
          	}
		else
		{

			 #ifdef SUPPORT_LOWPOWER_3Timer_POWEROFF				 
				use_ctl.bLowPowerOffTimer=0;
			 #endif
		
	              use_ctl.send_lowpower_cnt=5;
        }
      #endif
      }
      else
      {
           offcnt = 0;

	 #ifdef SUPPORT_LOWPOWER_VOL_AUTO_ADJUST // charge in  release again vol 
              if(led_bat.bBatState<31)
		{
//                     led_bat.bBatStateTemp = 31;
//      		       led_bat.bBatStateCnt = 0;
// 			audio_dac_vol_set(0x01,BIT(0),led_bat.bBatStateTemp,1);
// 			audio_dac_vol_set(0x01,BIT(1),led_bat.bBatStateTemp,1);
			led_bat.bBatState=31;
			audio_dac_vol_set(0x01,BIT(0),led_bat.bBatState,1);
			audio_dac_vol_set(0x01,BIT(1),led_bat.bBatState,1);	 
        }		   
	#endif
      }
}

#endif

#ifdef DCIN_DET
AT(.volatile_ram_code)
void DcIn_check(void)
{
    u8 static CheckCnt=0,cnt=0;
    DC_DET_IO_INIT();
    if( DC_IS_IN() ){
        CheckCnt++;
        if(CheckCnt > 10){
            led_bat.status = IN_CHARGE;
        }
    }else{
        CheckCnt = 0;
        led_bat.status = NOT_CHARGE;
    }
		
    #ifdef SUPORT_EXTERN_LED_BAT_SHOW
    if(led_bat.status == IN_CHARGE){
       if(led_bat.power == POWER_FULL){
           LED_BAT_FULL_ON();
           LED_CHARGE_OFF();
       }else{
           LED_BAT_FULL_OFF();
           LED_CHARGE_ON();
       }
    }else{
       LED_CHARGE_OFF();
       LED_BAT_FULL_OFF();
    }
    #endif


}

#ifdef SW_ONOFF_DET

void sw_onoff_check(void)
{
    u8 static CheckCnt,CheckCnt1;

	if(use_ctl.bSwOffDlyCnt)
		return;
	
    SW_DET_IO_INIT();
    if(SW_IS_OFF()){
        CheckCnt1=0;		
        CheckCnt++;
        if(CheckCnt > 5){
            use_ctl.sw_off = true;
        }
    }else{
        CheckCnt=0;
		
        CheckCnt1++;
        if(CheckCnt1 > 5){
        use_ctl.sw_off = false;
        }		
    }

//	  printf("/n--1--led_bat.status-%d\n",led_bat.status)   ;

//	  printf("/n--1--use_ctl.sw_off_flag-%d\n",use_ctl.sw_off_flag)   ;
	
    if(led_bat.status == IN_CHARGE)
    {
        //puts("DCIN ");
	   if(use_ctl.sw_off ==true) // 关机
	   {
             if (app_get_curr_task() != APP_IDLE_TASK&&(use_ctl.sw_off_flag == 0))			
	        {
				use_ctl.sw_off_flag=1;
				use_ctl.bSwOffDlyTimer=3;				
//				app_common_key_var_event(KEY_POWEROFF);
				
				app_task_switch_to(APP_IDLE_TASK);
	        }
	    }else{
	        if(use_ctl.sw_off_flag&&(app_get_curr_task() == APP_IDLE_TASK)){
		    {
	                use_ctl.sw_off_flag = 0;
	                app_common_key_var_event(KEY_STANBY_SW);
	            }
	        }
	    }
	}
	else
	{
        use_ctl.sw_off = false;
	}
}
#endif
#endif



#ifdef SUPPORT_SD_DAT_MULTIPLEX_AD

AT(.volatile_ram_code)
void sd_dat_ad_multiplex()
{
 if (app_get_curr_task() == APP_IDLE_TASK) 
	return;
 
if (sd_io_suspend(1, 2) == 0) // sdx:0-sd0 1-sd1  sd_io: 0-cmd 1-clk 2-dat
{ 

 	   // cmd io 口设输入
	gpio_set_pull_down(IO_PORTB_04, 0);
	gpio_set_pull_up(IO_PORTB_04, 1);
	gpio_set_die(IO_PORTB_04, 0);
	gpio_set_hd(IO_PORTB_04, 0);
	gpio_set_hd0(IO_PORTB_04, 0);
	gpio_direction_input(IO_PORTB_04);
	
//	printf("\n 000--bSdCmdAdVal==%d\n",bSdCmdAdVal);
	
	adc_suspend();
        adc_enter_occupy_mode(AD_CH_PB4);
        if (adc_occupy_run()) {
			
            bSdCmdAdVal = adc_get_occupy_value();
//	 	printf("\n 11--bSdCmdAdVal==%d\n",bSdCmdAdVal);
        }
		
        adc_exit_occupy_mode();
		
 	// cmd io 口设输出	

	    gpio_direction_input(IO_PORTB_04);
	    gpio_set_pull_up(IO_PORTB_04, 0);
	    gpio_set_pull_down(IO_PORTB_04, 0);
	    gpio_set_hd(IO_PORTB_04, 0);
	    gpio_set_hd0(IO_PORTB_04, 0);
	    gpio_set_die(IO_PORTB_04, 0);

 	   sd_io_resume(1, 2);//使用完，回复sd		
}	


}
#endif

#define SUPPORT_AD_CUT
#ifdef SUPPORT_AD_CUT
#define AD_VAL_MAX   1020
#define AD_VAL_CUT   16
#ifndef AD_VAL_JT_ADD
#define AD_VAL_JT_ADD   4
#endif

u8 adpot_cut(u16 val,u8 max)
{
    static u8 bStep,bVal;

      if(val<10) 
         bVal = 0;
       else{
          bStep=( AD_VAL_MAX-AD_VAL_CUT)/max;
          bVal = (val-AD_VAL_CUT) / bStep;
          if(bVal > max)bVal = max;  
       }
	   
   return bVal;
}
#endif

#ifdef  board_DG_1258_ac6951c  
static u8 kkkkk=0;
#endif


#if (defined VOL_IO_AD_EN || defined SUPPORT_4051_VOL)
void advol_check(void)
{
	//printf("advol_check");
	static u16 vol_ad_value_buffer[10];
	static u8 vol_buffer_cnt = 0;
	u16 get_val_all;
	u8 A;
	static bool set_vol = 0;

#ifdef SUPPORT_POWER_ON_AD_SHOW_DLY
	if(use_ctl.bPowerOnShowDlyCnt==4){
		app_common_key_var_event(KEY_MAIN_VOL_SET);
		return;
	}
#endif
	
	#ifdef SUPPORT_4051_VOL
	vol_ad_value_buffer[vol_buffer_cnt] = use_ctl.ch1_ad_get;
	#else
	vol_ad_value_buffer[vol_buffer_cnt] = adc_get_value(AD_CH_VOL);
	#endif
	vol_buffer_cnt = (vol_buffer_cnt >=(10-1))? 0:vol_buffer_cnt+1;
	
	if(vol_buffer_cnt == 0)
	{
		get_val_all = 0;
		for(u8 i=0;i<10;i++)
		{
			get_val_all += vol_ad_value_buffer[i];
		}
		use_ctl.ad_vol = get_val_all/10;
		#ifdef VOL_IO_AD_VAL_MIN
		if(use_ctl.ad_vol<VOL_IO_AD_VAL_MIN)use_ctl.ad_vol = VOL_IO_AD_VAL_MIN;
		#endif
		//printf("use_ctl.ad_vol == %d\n",use_ctl.ad_vol);
		if((use_ctl.ad_vol_pr>(use_ctl.ad_vol+20))||(use_ctl.ad_vol_pr<(use_ctl.ad_vol-20)))
		{
			set_vol = 1;
			use_ctl.ad_vol_pr = use_ctl.ad_vol;
			#ifdef VOL_IO_AD_VAL_MIN
			if(use_ctl.ad_vol<(VOL_IO_AD_VAL_MIN))use_ctl.sys_vol = 0;
			#else
			if(use_ctl.ad_vol<10)use_ctl.sys_vol = 0;
			#endif
			else
			{
				#ifdef SUPPORT_4051_VOL
					use_ctl.sys_vol = use_ctl.ad_vol_pr*LED_SHOW_MAX_VOL/CD4051_AD_MAX;
				#else
			
				#ifdef VOL_IO_AD_VAL_MIN
				use_ctl.sys_vol = (use_ctl.ad_vol - VOL_IO_AD_VAL_MIN) * LED_SHOW_MAX_VOL / (VOL_IO_AD_VAL_MAX - VOL_IO_AD_VAL_MIN);
				#else
				/*
				if(use_ctl.ad_vol>VOL_IO_AD_VAL_MAX/2){
					use_ctl.sys_vol = (LED_SHOW_MAX_VOL - (LED_SHOW_MAX_VOL / 3)) + (use_ctl.ad_vol-VOL_IO_AD_VAL_MAX/2)*(LED_SHOW_MAX_VOL / 3)/(VOL_IO_AD_VAL_MAX / 2);
				}else{
					use_ctl.sys_vol = use_ctl.ad_vol * (LED_SHOW_MAX_VOL - (LED_SHOW_MAX_VOL / 3))/(VOL_IO_AD_VAL_MAX / 2);
				}
				*/
				use_ctl.sys_vol = use_ctl.ad_vol * LED_SHOW_MAX_VOL/VOL_IO_AD_VAL_MAX;
				#endif

				#endif//SUPPORT_4051_VOL
			}   

			if(use_ctl.sys_vol > LED_SHOW_MAX_VOL)use_ctl.sys_vol = LED_SHOW_MAX_VOL;  

			if(use_ctl.led_vol!=use_ctl.sys_vol)
			{
				use_ctl.led_vol=use_ctl.sys_vol;
				app_common_key_var_event(KEY_MAIN_VOL_SET);
				
#ifdef  board_DG_1258_ac6951c  
				kkkkk = 4;
#endif
				
#ifdef SUPPORT_KEY_VOL_AD_RGB_
				if ( !use_ctl.bPowerOnShowDlyCnt && (user_rgb_adc_get_(0) == true ))
				{
					use_ctl.key_rgb_cnt_adc = 4;
					use_ctl.key_rgb_adc = BIT(0);
				}
#endif

			}
			else
			{
			set_vol = 0;
			}
			
		}
		else
		{
			return;
		}
	}
	else
	{
		if(set_vol == 0)
		return;
	}

}

#endif

#if SUPPORT_USER_EQ_MODE

#if (defined TRE_IO_AD_EN) || (defined SUPPORT_4051_TRE)
void adtre_check(void)
{
	u16 get_val;
	static u8 ad_tre,tre_cnt=0;
	static u32 tre_sum;
	static u16 pr_treval=2048;


#ifdef SUPPORT_POWER_ON_AD_SHOW_DLY
	if(use_ctl.bPowerOnShowDlyCnt==3){
		use_ctl.tre = ad_tre;
		app_common_key_var_event(KEY_TRE_SET);
		return;
	}
#endif

#ifdef SUPPORT_4051_TRE
	if(tre_cnt<10){
		tre_sum += use_ctl.ch5_ad_get;
		tre_cnt++;
		return;
	}else{
		get_val = tre_sum/10;
		tre_sum = 0;
		tre_cnt = 0;
	}
	if((get_val > (pr_treval+CHECK_DIFF))||(get_val < (pr_treval-CHECK_DIFF))){
		pr_treval = get_val;
	}

	ad_tre = pr_treval*TRE_MAX/CD4051_AD_MAX;
#else
	if(tre_cnt<10){
		tre_sum += adc_get_value(AD_CH_TRE);
		tre_cnt++;
		return;
	}else{
		get_val = tre_sum/10;
		tre_sum = 0;
		tre_cnt = 0;
	}
	if((get_val > (pr_treval+20))||(get_val < (pr_treval-20))){
		pr_treval = get_val;
		ad_tre = pr_treval*TRE_MAX/TRE_IO_AD_VAL_MAX;
	
		if(ad_tre>TRE_MAX){
			ad_tre = TRE_MAX;
		}
		
		//printf("===============ad_tre=%d %d\n",ad_tre,use_ctl.tre);
		
		if(ad_tre != use_ctl.tre){
			use_ctl.tre = ad_tre;
			app_common_key_var_event(KEY_TRE_SET);
		
#ifdef SUPPORT_KEY_TRE_AD_RGB_
																				  if ( !use_ctl.bPowerOnShowDlyCnt && user_rgb_adc_get_(1) == true )
																				  {
																					  use_ctl.key_rgb_cnt_adc = 4;
																					  use_ctl.key_rgb_adc = BIT(6);
																				  }
#endif
		}
		
	}else{
		return;
	}
	
#endif

}
#endif

#ifdef MID_IO_AD_EN

AT(.volatile_ram_code)
void admid_check(void)
{
   u16 get_val;
   static u8 ad_mid,cnt=0;
       static u16 get_val_temp,get_val_prev;
    static u8 cnt1;

 if (app_get_curr_task() == APP_IDLE_TASK) 
 	return;
   if (app_get_curr_task() == APP_POWERON_TASK) 
 	return;
   
   get_val = adc_get_value(AD_CH_MID);
   
   if(++cnt1<4)
   	{
 	get_val_temp+=get_val;
 	return;
   	}
   else
   {
	   get_val=get_val_temp/3;
	   get_val_temp=0;
	    cnt1=0;
   }
#ifdef SUPPORT_POWER_ON_AD_SHOW_DLY
	if(use_ctl.bPowerOnShowDlyCnt == 4)
		{
		   get_val = adc_get_value(AD_CH_MID);
		    use_ctl.mid = adpot_cut(get_val,MID_MAX);
			app_common_key_var_event(KEY_MID_SET);
		     return;
		}
#endif		 

   if((get_val >get_val_prev+(MID_IO_AD_VAL_MAX/(MID_MAX*2)))||(get_val <get_val_prev-(MID_IO_AD_VAL_MAX/(MID_MAX*2))))
   	{
		get_val_prev=get_val;
		  #ifdef SUPPORT_AD_CUT
		    ad_mid = adpot_cut(get_val,MID_MAX);
		 #else 
		 
		    ad_mid = get_val*MID_MAX/MID_IO_AD_VAL_MAX;
		    if(ad_mid>MID_MAX){
		         ad_mid = MID_MAX;
		    }
		 #endif
		 
		    if(ad_mid != use_ctl.mid)
		    {
		     //  if((cnt++)>5)
		       { 
		           use_ctl.mid = ad_mid;
		           app_common_key_var_event(KEY_MID_SET);
		       }
		    }
		    else
		    {
		       cnt=0;
		    }
   	}
}
#endif


#if (defined BASS_IO_AD_EN) || (defined SUPPORT_4051_BASS)
void adbass_check(void)
{
	u16 get_val;
	static u8 ad_bass,bass_cnt=0;
	static u32 bass_sum;
	static u16 pr_bassval=2048;

	
#ifdef SUPPORT_POWER_ON_AD_SHOW_DLY
	if(use_ctl.bPowerOnShowDlyCnt==3)
	{
		use_ctl.bass = ad_bass;		
		app_common_key_var_event(KEY_BASS_SET);	
		return;
	}	

#endif
	
#ifdef SUPPORT_4051_BASS
	if(bass_cnt<10){
		bass_sum += use_ctl.ch5_ad_get;
		bass_cnt++;
		return;
	}else{
		get_val = bass_sum/10;
		bass_sum = 0;
		bass_cnt = 0;
	}
	if((get_val > (pr_bassval+CHECK_DIFF))||(get_val < (pr_bassval-CHECK_DIFF))){
		pr_bassval = get_val;
	}

	ad_bass = pr_bassval*BASS_MAX/CD4051_AD_MAX;
	
#else

	if(bass_cnt<10){
		bass_sum += adc_get_value(AD_CH_BASS);
		bass_cnt++;
		return;
	}else{
		get_val = bass_sum/10;
		bass_sum = 0;
		bass_cnt = 0;
	}
	
	if((get_val > (pr_bassval+20))||(get_val < (pr_bassval-20))){
		pr_bassval = get_val;
		ad_bass = pr_bassval*BASS_MAX/BASS_IO_AD_VAL_MAX;

		if(ad_bass>BASS_MAX){
			ad_bass = BASS_MAX;
		}
			
		if(ad_bass != use_ctl.bass)
		{
			use_ctl.bass= ad_bass;
			app_common_key_var_event(KEY_BASS_SET);
			
#ifdef SUPPORT_KEY_BAS_AD_RGB_
		  if ( !use_ctl.bPowerOnShowDlyCnt && user_rgb_adc_get_(1) == true )
		  {
			  use_ctl.key_rgb_cnt_adc = 4;
			  use_ctl.key_rgb_adc = BIT(7);
		  }
#endif
		}
		
	}else{
		return;
	}
	
#endif
	

}
#endif

#if defined SUPPORT_PONTEN_EQ1_EQ5 
void ad_eq1_check(void)
{
	u16 get_val;
	static u8 ad_eq1,eq1_cnt=0;
	static u32 eq1_sum;
	static u16 pr_eq1val=2048;

	
	if(use_ctl.bPowerOnShowDlyCnt==3){
		use_ctl.bEq1Val = ad_eq1;		
		app_common_key_var_event(KEY_EQ1_SET);	
		return;
	}	
	
	if(eq1_cnt<5){
		eq1_sum += adc_get_value(AD_CH_EQ1);
		eq1_cnt++;
		return;
	}else{
		get_val = eq1_sum/5;
		eq1_sum = 0;
		eq1_cnt = 0;
	}

    //  printf("\n=======ad_eq1_check========%d-%d\n",get_val,use_ctl.bEq1Val);
	
	if((get_val > (pr_eq1val+20))||(get_val < (pr_eq1val-20))){
		pr_eq1val = get_val;
		ad_eq1 = pr_eq1val*AD_EQ_MAX/EQ_IO_AD_VAL_MAX;

		if(ad_eq1>AD_EQ_MAX){
			ad_eq1 = AD_EQ_MAX;
		}
			
		if(ad_eq1 != use_ctl.bEq1Val)
		{
		
			use_ctl.bEq1Val= ad_eq1;
			app_common_key_var_event(KEY_EQ1_SET);
		}
		
	}else{
		return;
	}
	

}

void ad_eq2_check(void)
{
	u16 get_val;
	static u8 ad_eq2,eq2_cnt=0;
	static u32 eq2_sum;
	static u16 pr_eq2val=2048;

	
	if(use_ctl.bPowerOnShowDlyCnt==3){
		use_ctl.bEq2Val = ad_eq2;		
		app_common_key_var_event(KEY_EQ2_SET);	
		return;
	}	
	
	if(eq2_cnt<5){
		eq2_sum += adc_get_value(AD_CH_EQ2);
		eq2_cnt++;
		return;
	}else{
		get_val = eq2_sum/5;
		eq2_sum = 0;
		eq2_cnt = 0;
	}
	if((get_val > (pr_eq2val+20))||(get_val < (pr_eq2val-20))){
		pr_eq2val = get_val;
		ad_eq2 = pr_eq2val*AD_EQ_MAX/EQ_IO_AD_VAL_MAX;
	}else{
		return;
	}
	
	if(ad_eq2>AD_EQ_MAX){
		ad_eq2 = AD_EQ_MAX;
	}
		
	if(ad_eq2 != use_ctl.bEq2Val)
	{
		use_ctl.bEq2Val= ad_eq2;
		app_common_key_var_event(KEY_EQ2_SET);
	}
}
void ad_eq3_check(void)
{
	u16 get_val;
	static u8 ad_eq3,eq3_cnt=0;
	static u32 eq3_sum;
	static u16 pr_eq3val=2048;

	
	if(use_ctl.bPowerOnShowDlyCnt==3){
		use_ctl.bEq3Val = ad_eq3;		
		app_common_key_var_event(KEY_EQ3_SET);	
		return;
	}	
	
	if(eq3_cnt<5){
		eq3_sum += adc_get_value(AD_CH_EQ3);
		eq3_cnt++;
		return;
	}else{
		get_val = eq3_sum/5;
		eq3_sum = 0;
		eq3_cnt = 0;
	}
	if((get_val > (pr_eq3val+20))||(get_val < (pr_eq3val-20))){
		pr_eq3val = get_val;
		ad_eq3 = pr_eq3val*AD_EQ_MAX/EQ_IO_AD_VAL_MAX;
	}else{
		return;
	}
	
	if(ad_eq3>AD_EQ_MAX){
		ad_eq3 = AD_EQ_MAX;
	}
		
	if(ad_eq3 != use_ctl.bEq3Val)
	{
		use_ctl.bEq3Val= ad_eq3;
		app_common_key_var_event(KEY_EQ3_SET);
	}
}
void ad_eq4_check(void)
{
	u16 get_val;
	static u8 ad_eq4,eq4_cnt=0;
	static u32 eq4_sum;
	static u16 pr_eq4val=2048;

	
	if(use_ctl.bPowerOnShowDlyCnt==3){
		use_ctl.bEq4Val = ad_eq4;		
		app_common_key_var_event(KEY_EQ4_SET);	
		return;
	}	
	
	if(eq4_cnt<5){
		eq4_sum += adc_get_value(AD_CH_EQ4);
		eq4_cnt++;
		return;
	}else{
		get_val = eq4_sum/5;
		eq4_sum = 0;
		eq4_cnt = 0;
	}
	if((get_val > (pr_eq4val+20))||(get_val < (pr_eq4val-20))){
		pr_eq4val = get_val;
		ad_eq4 = pr_eq4val*AD_EQ_MAX/EQ_IO_AD_VAL_MAX;
	}else{
		return;
	}
	
	if(ad_eq4>AD_EQ_MAX){
		ad_eq4 = AD_EQ_MAX;
	}
		
	if(ad_eq4 != use_ctl.bEq4Val)
	{
		use_ctl.bEq4Val= ad_eq4;
		app_common_key_var_event(KEY_EQ4_SET);
	}
}
void ad_eq5_check(void)
{
	u16 get_val;
	static u8 ad_eq5,eq5_cnt=0;
	static u32 eq5_sum;
	static u16 pr_eq5val=2048;

	
	if(use_ctl.bPowerOnShowDlyCnt==3){
		use_ctl.bEq5Val = ad_eq5;		
		app_common_key_var_event(KEY_EQ5_SET);	
		return;
	}	
	
	if(eq5_cnt<5){
		eq5_sum += adc_get_value(AD_CH_EQ5);
		eq5_cnt++;
		return;
	}else{
		get_val = eq5_sum/5;
		eq5_sum = 0;
		eq5_cnt = 0;
	}
	if((get_val > (pr_eq5val+20))||(get_val < (pr_eq5val-20))){
		pr_eq5val = get_val;
		ad_eq5 = pr_eq5val*AD_EQ_MAX/EQ_IO_AD_VAL_MAX;
	}else{
		return;
	}
	
	if(ad_eq5>AD_EQ_MAX){
		ad_eq5 = AD_EQ_MAX;
	}
		
	if(ad_eq5 != use_ctl.bEq5Val)
	{
		use_ctl.bEq5Val= ad_eq5;
		app_common_key_var_event(KEY_EQ5_SET);
	}
}

#endif

#endif



#ifdef MICVOL_IO_AD_EN
void admicvol_check(void)
{
   u16 get_val;
   static u8 ad_micvol,cnt=0;
    static u16 get_val_temp,get_val_prev;
    static u8 cnt1;

 if (app_get_curr_task() == APP_IDLE_TASK) 
 	return;
   if (app_get_curr_task() == APP_POWERON_TASK) 
 	return;
   
   get_val = adc_get_value(AD_CH_MICVOL);
   if(++cnt1<4)
   	{
 	get_val_temp+=get_val;
 	return;
   	}
   else
   {
	   get_val=get_val_temp/3;
	   get_val_temp=0;
	    cnt1=0;
   }
#ifdef SUPPORT_POWER_ON_AD_SHOW_DLY
	if(use_ctl.bPowerOnShowDlyCnt == 4)
		{	
		     get_val = adc_get_value(AD_CH_MICVOL);
		    use_ctl.mic_vol = adpot_cut(get_val,MICVOL_MAX);
			app_common_key_var_event(KEY_MIC_VOL_SET);
		     return;
		}
#endif		 

   if((get_val >get_val_prev+(MICVOL_IO_AD_VAL_MAX/(MICVOL_MAX*2)))||(get_val <get_val_prev-(MICVOL_IO_AD_VAL_MAX/(MICVOL_MAX*2))))
   	{
		get_val_prev=get_val;
		/*
		 #ifdef SUPPORT_AD_CUT
		    ad_micvol = adpot_cut(get_val,MICVOL_MAX);
		 #else 
		    ad_micvol = get_val*MICVOL_MAX/MICVOL_IO_AD_VAL_MAX;
		    if(ad_micvol > MICVOL_MAX)ad_micvol = MICVOL_MAX;
		 #endif
		*/
#ifdef MICVOL_IO_AD_VAL_MIN
		   if(get_val < MICVOL_IO_AD_VAL_MIN)ad_micvol = 0;
		   else ad_micvol = (get_val - MICVOL_IO_AD_VAL_MIN) * MICVOL_MAX / (MICVOL_IO_AD_VAL_MAX - MICVOL_IO_AD_VAL_MIN);
#else 
		   ad_micvol = get_val*MICVOL_MAX/MICVOL_IO_AD_VAL_MAX;
#endif
		if(ad_micvol > MICVOL_MAX)ad_micvol = MICVOL_MAX;

		
		//printf(" ad_micvol=%d %d  %d",get_val,ad_micvol,use_ctl.mic_vol);
	    if(ad_micvol != use_ctl.mic_vol)
	    {
	       { 
	          // printf("\n\n micvol=%d",ad_micvol);
	           use_ctl.mic_vol = ad_micvol;
	           app_common_key_var_event(KEY_MIC_VOL_SET);
			   
#ifdef  board_DG_1258_ac6951c  
								kkkkk = 4;
#endif

#ifdef SUPPORT_KEY_MIC_AD_RGB_
				if ( !use_ctl.bPowerOnShowDlyCnt && user_rgb_adc_get_(1) == true )
				{
					use_ctl.key_rgb_cnt_adc = 4;
					use_ctl.key_rgb_adc = BIT(1);
				}
#endif
	       }
	    }
	    else
	    {
	       cnt=0;
	    }
   	}
}
#endif

#ifdef ECHO_IO_AD_EN
void adecho_check(void)
{
   u16 get_val;
   static u8 ad_echo,cnt=0;
    static u16 get_val_temp,get_val_prev;
    static u8 cnt1;

 if (app_get_curr_task() == APP_IDLE_TASK) 
 	return;
     if (app_get_curr_task() == APP_POWERON_TASK) 
 	return;
 
   get_val = adc_get_value(AD_CH_ECHO);
   if(++cnt1<4)
   	{
 	get_val_temp+=get_val;
 	return;
   	}
   else
   {
	   get_val=get_val_temp/3;
	   get_val_temp=0;
	    cnt1=0;
   }

#ifdef SUPPORT_POWER_ON_AD_SHOW_DLY
	if(use_ctl.bPowerOnShowDlyCnt == 4)
		{
			get_val_prev=get_val;		
			use_ctl.echo = adpot_cut(get_val,ECHO_MAX);
			app_common_key_var_event(KEY_ECHO_SET);
	//	   printf("\n--00--echo=%d %d  %d",get_val,get_val_prev,use_ctl.echo);
		     return;
		}
#endif		 

	//	   printf("\n--111--echo=%d %d  %d",get_val,get_val_prev,use_ctl.echo);

   if((get_val >get_val_prev+(ECHO_IO_AD_VAL_MAX/(ECHO_MAX*2)))||(get_val <get_val_prev-(ECHO_IO_AD_VAL_MAX/(ECHO_MAX*2))))
   	{

		get_val_prev=get_val;
	  #ifdef SUPPORT_AD_CUT
	    ad_echo = adpot_cut(get_val,ECHO_MAX);
	 #else 
	    ad_echo = get_val*ECHO_MAX/ECHO_IO_AD_VAL_MAX;
	    if(ad_echo>ECHO_MAX){
	         ad_echo = ECHO_MAX;
	    }
	 #endif
	 
	 
	 
	   // r_printf(" ad_echo=%d  %d  %d",get_val,ad_echo,use_ctl.echo);
	    if(ad_echo != use_ctl.echo)
	    {
	       
	   //    if((cnt++)>5)
	       { 
	           //printf("\n echo=%d",ad_echo);
	           use_ctl.echo = ad_echo;
	           app_common_key_var_event(KEY_ECHO_SET);
			   
#ifdef SUPPORT_KEY_ECHO_AD_RGB_
												  if ( !use_ctl.bPowerOnShowDlyCnt && user_rgb_adc_get_(1) == true )
												  {
													  use_ctl.key_rgb_cnt_adc = 4;
													  use_ctl.key_rgb_adc = BIT(2);
												  }
#endif
	       }
	    }
	    else
	    {
	       cnt=0;
	    }
    }
}
#endif

#ifdef MIC_DELAY_IO_AD_EN
void ad_mic_delay_check(void)
{
   u16 get_val;
   static u8 ad_micdelay,cnt=0;
    static u16 get_val_temp,get_val_prev;
    static u8 cnt1;


 if (app_get_curr_task() == APP_IDLE_TASK) 
 	return;
    if (app_get_curr_task() == APP_POWERON_TASK) 
 	return;
  
   get_val = adc_get_value(AD_CH_MICDELAY);


   if(++cnt1<4)
   	{
 	get_val_temp+=get_val;
 	return;
   	}
   else
   {
	   get_val=get_val_temp/3;
	   get_val_temp=0;
	    cnt1=0;
   }

#ifdef SUPPORT_POWER_ON_AD_SHOW_DLY
	if(use_ctl.bPowerOnShowDlyCnt == 4)
		{	
     		get_val = adc_get_value(AD_CH_MICDELAY);
    		use_ctl.micdelay= adpot_cut(get_val,MIC_DELAY_MAX);
			app_common_key_var_event(KEY_MIC_DELAY_SET);
		     return;
		}
#endif		 

   if((get_val >get_val_prev+(MIC_DELAY_IO_AD_VAL_MAX/(MIC_DELAY_MAX*2)))||(get_val <get_val_prev-(MIC_DELAY_IO_AD_VAL_MAX/(MIC_DELAY_MAX*2))))
   	{
   
		get_val_prev=get_val;
	  #ifdef SUPPORT_AD_CUT
	    ad_micdelay = adpot_cut(get_val,MIC_DELAY_MAX);
	 #else 
	    ad_micdelay = get_val*MIC_DELAY_MAX/MIC_DELAY_IO_AD_VAL_MAX;
	    if(ad_micdelay>MIC_DELAY_MAX){
	         ad_micdelay = MIC_DELAY_MAX;
	    }
	 #endif
	 
	 
	   // r_printf(" ad_echo=%d  %d  %d",get_val,ad_echo,use_ctl.echo);
	    if(ad_micdelay != use_ctl.micdelay)
	    {
	       
	     //  if((cnt++)>5)
	       { 
	           //printf("\n echo=%d",ad_echo);
	           use_ctl.micdelay = ad_micdelay;
	           app_common_key_var_event(KEY_MIC_DELAY_SET);
	       }
	    }
	    else
	    {
	       cnt=0;
	    }
    }
}
#endif


#ifdef MIC_BASS_IO_AD_EN
void mic_bass_check(void)
{
   u16 get_val;
    static u16 get_val_temp,get_val_prev;
    static u8 cnt1;
  static u8 ad_mic_bass,cnt=0;



 if (app_get_curr_task() == APP_IDLE_TASK) 
 	return;
   if (app_get_curr_task() == APP_POWERON_TASK) 
 	return;

   get_val = adc_get_value(AD_CH_MIC_BASS);
   
   if(++cnt1<10)
   	{
 		get_val_temp+=get_val;
 		return;
   	}
   else
   {
	   get_val=get_val_temp/9;
	   get_val_temp=0;
	    cnt1=0;
   }
   
//  printf("===1============ad_mic_bass=%d-%d-%d\n",ad_mic_bass,use_ctl.mic_bass,use_ctl.bPowerOnShowDlyCnt);

#ifdef SUPPORT_POWER_ON_AD_SHOW_DLY
	if(use_ctl.bPowerOnShowDlyCnt ==2)
		{
		
		    use_ctl.mic_bass = ad_mic_bass;
			app_common_key_var_event(KEY_MIC_BASS_SET);
		    return;
		}
#endif
   
    if((get_val >get_val_prev+22)||(get_val <get_val_prev-22))
   	{
  
		get_val_prev = get_val;

	  #ifdef SUPPORT_AD_CUT
	    ad_mic_bass = adpot_cut(get_val,MIC_BASS_MAX);
	 #else 
	    ad_mic_bass = get_val_prev*MIC_BASS_MAX/MIC_BASS_IO_AD_VAL_MAX;
		
	    if(ad_mic_bass>MIC_BASS_MAX)
	    {
	         ad_mic_bass = MIC_BASS_MAX;
	    }
	 #endif

    #ifdef SUPPORT_POWER_ON_AD_SHOW_DLY
    	if(use_ctl.bPowerOnShowDlyCnt)return;
    #endif

	if(ad_mic_bass != use_ctl.mic_bass)
	{
       use_ctl.mic_bass = ad_mic_bass;
       app_common_key_var_event(KEY_MIC_BASS_SET);
	   
#ifdef SUPPORT_KEY_M_BAS_AD_RGB_
																	  if ( !use_ctl.bPowerOnShowDlyCnt && user_rgb_adc_get_(1) == true )
																	  {
																		  use_ctl.key_rgb_cnt_adc = 4;
																		  use_ctl.key_rgb_adc = BIT(5);
																	  }
#endif
    }
      }
	

}
#endif

#ifdef MIC_TRE_IO_AD_EN
void mic_tre_check(void)
{
   u16 get_val;
   static u8 ad_mic_tre,cnt=0;
    static u16 get_val_temp,get_val_prev;
    static u8 cnt1;

    if (app_get_curr_task() == APP_IDLE_TASK) 
 	return;
   if (app_get_curr_task() == APP_POWERON_TASK) 
 	return;

   get_val = adc_get_value(AD_CH_MIC_TRE);

   if(++cnt1<10)
   	{
 	get_val_temp+=get_val;
 	return;
   	}
   else
   {
	   get_val=get_val_temp/9;
	   get_val_temp=0;
	    cnt1=0;
   }

#ifdef SUPPORT_POWER_ON_AD_SHOW_DLY
	if(use_ctl.bPowerOnShowDlyCnt == 3)
		{
			use_ctl.mic_tre= ad_mic_tre;
			app_common_key_var_event(KEY_MIC_TRE_SET);
		     return;
		}
#endif		 

      if((get_val > get_val_prev+(MIC_TRE_IO_AD_VAL_MAX/MIC_TRE_MAX))||(get_val <get_val_prev-(MIC_TRE_IO_AD_VAL_MAX/MIC_TRE_MAX)))
   	{
		get_val_prev = get_val;

	    ad_mic_tre = get_val_prev*MIC_TRE_MAX/MIC_TRE_IO_AD_VAL_MAX;
		
	    if(ad_mic_tre>MIC_TRE_MAX){
	         ad_mic_tre = MIC_TRE_MAX;
	    }


	 #ifdef SUPPORT_POWER_ON_AD_SHOW_DLY
	 	if(use_ctl.bPowerOnShowDlyCnt)return;
	 #endif 
	 
	 	if(ad_mic_tre != use_ctl.mic_tre)
	 	{
	 		use_ctl.mic_tre = ad_mic_tre;
	 	   	app_common_key_var_event(KEY_MIC_TRE_SET);
			
#ifdef SUPPORT_KEY_M_TRE_AD_RGB_
															  if ( !use_ctl.bPowerOnShowDlyCnt && user_rgb_adc_get_(1) == true )
															  {
																  use_ctl.key_rgb_cnt_adc = 4;
																  use_ctl.key_rgb_adc = BIT(3);
															  }
#endif
	 	  
	 	}		
    }
	  

}
#endif

#ifdef JT_VOL_IO_AD_EN
void adjtvol_check(void)
{
   u16 get_val;
   static u8 ad_jtvol,cnt=0;
    static u16 get_val_temp,get_val_prev;
    static u8 cnt1;

 if (app_get_curr_task() == APP_IDLE_TASK) 
 	return;
     if (app_get_curr_task() == APP_POWERON_TASK) 
 	return;
 
 #ifdef SUPPORT_SD_DAT_MULTIPLEX_AD
   get_val = bSdCmdAdVal;
#else
   get_val = adc_get_value(AD_CH_JTVOL);
#endif
  // printf("adjtvol_check=%d",get_val);
 #ifdef SUPPORT_SD_DAT_MULTIPLEX_AD
   if(get_val<=JTVOL_IO_AD_VAL_MIN) ad_jtvol=0;
   else
#endif   	
#if 1
   if(++cnt1<15)
   	{
 	get_val_temp+=get_val;
 	return;
   	}
   else
   {
	   get_val=get_val_temp/14;
	   get_val_temp=0;
	    cnt1=0;
   }
#endif

#ifdef SUPPORT_POWER_ON_AD_SHOW_DLY
	if(use_ctl.bPowerOnShowDlyCnt)
		{
		     get_val_prev=get_val;	
		    use_ctl.jt_vol = (get_val-JTVOL_IO_AD_VAL_MIN)*JTVOL_MAX/(JTVOL_IO_AD_VAL_MAX-JTVOL_IO_AD_VAL_MIN);		 
		    if(use_ctl.jt_vol > JTVOL_MAX)use_ctl.jt_vol = JTVOL_MAX;			 
		//   printf("\n--00--echo=%d %d  %d",get_val,get_val_prev,use_ctl.jt_vol);
#ifdef JT_VOL_M62429_EN
								 int m62429_send_ctl(u8 ,u8 );
								 if(use_ctl.bPowerOnShowDlyCnt==1)
								 m62429_send_ctl(0,use_ctl.jt_vol);
						 
#endif
			
		     return;
		}
#endif		 

   if((get_val >get_val_prev+AD_VAL_JT_ADD)||(get_val <get_val_prev-AD_VAL_JT_ADD))
   	{

		get_val_prev=get_val;
		 #if 0//def SUPPORT_AD_CUT
		    ad_jtvol = adpot_cut((get_val-JTVOL_IO_AD_VAL_MIN),JTVOL_MAX);
		 #else 
		    ad_jtvol = (get_val_prev-JTVOL_IO_AD_VAL_MIN+10)*JTVOL_MAX/(JTVOL_IO_AD_VAL_MAX-JTVOL_IO_AD_VAL_MIN);
		 
		    if(ad_jtvol > JTVOL_MAX)ad_jtvol = JTVOL_MAX;
		 #endif
		 
		    
		  //  printf(" ad_micvol=%d %d  %d",get_val,ad_micvol,use_ctl.mic_vol);
		    if(ad_jtvol != use_ctl.jt_vol)
		    {
		       { 
		          // printf("\n\n micvol=%d",ad_micvol);
		           use_ctl.jt_vol = ad_jtvol;
		           app_common_key_var_event(KEY_JT_VOL_SET);
				   
#ifdef SUPPORT_KEY_GT_AD_RGB_
								   if ( !use_ctl.bPowerOnShowDlyCnt && user_rgb_adc_get_(1) == true )
								   {
									   use_ctl.key_rgb_cnt_adc = 4;
									   use_ctl.key_rgb_adc = BIT(4);
								   }
#endif

		       }
		    }
		    else
		    {
		       cnt=0;
		    }
	    }
}
#endif

#if defined SUPPORT_MIC_CHECK

 void mic_check(void)
 {
	static u8 cnt=0,cnt_2=0;

	 #if defined SUPPORT_MIC_ALWAY_ONLINE
	 	use_ctl.mic_is_online = true;
	 	return;
	 #endif

	MIC_IO_INIT();
	if(MIC_IS_IN())
	{
		cnt++;
		cnt_2 = 0;
	 #ifdef MIC_CHECK_TIMER
	 	if(cnt>20)
	 #else
	 	if(cnt>50)
	 #endif
		{
			cnt = 0;
			if(use_ctl.mic_is_online==false)
			{
				use_ctl.mic_is_online = true;
				app_common_key_var_event(KEY_REVERB_OPEN);
			}
		}
	}
	else
	{
	   cnt_2++;
	   cnt=0;
	   if(cnt_2>5)
	   {
		  cnt_2 = 0;
		  if(use_ctl.mic_is_online==true)
			{
				use_ctl.mic_is_online = false;
				app_common_key_var_event(KEY_REVERB_OPEN);
			}
	   }
	}
	MIC_IO_UNINIT();
 }


#endif

#ifdef SUPPORT_LED_FLASH
void	user_led_flash()
{
	static u8 cnt_flash=0;
	if (use_ctl.led_flash)
	{
		use_ctl.led_flash--;
		if (cnt_flash++<LED_FLASH_CNT)
		{
			FLASH_SET_POT;
		}
		else
		{
			if (cnt_flash>2*LED_FLASH_CNT)
			{
				cnt_flash=0;
			}
			FLASH_CLR_POT;
			
		}
	}
	else
	{
		cnt_flash=0;
		FLASH_INIT_POT;
	}
}
#endif

#if defined SUPPORT_JT_CHECK
 void jt_check(void)
 {
	static u8 cnt=0,cnt_2=0;
	JT_IO_INIT();
	if(JT_IS_IN())
	{
		cnt++;
		cnt_2  = 0;
		//printf("\n---jt check --\n");
		if(cnt>50)
		{
			cnt  = 0;
			if(use_ctl.jt_is_online==false)
			{
				use_ctl.jt_is_online = true;
				 #ifdef SUPPORT_JTLED_EN
				  JTLED_ON();  
				 #endif

				
			}
		}
	}
	else
	{
	   cnt_2++;
	   cnt	= 0;
	  // printf("L");
	   if(cnt_2>50)
	   {
		  cnt_2 = 0;
		  if(use_ctl.jt_is_online==true)
			{
				use_ctl.jt_is_online = false;
				 #ifdef SUPPORT_JTLED_EN
				  JTLED_OFF() ;
				 #endif
				
			}
	   }
	}
 }
#endif

#ifdef SUPPORT_LINEIN_DET
void linein_check(){
	static u8 cnt;

	if(LINEIN_IN()){
		cnt++;
		if(cnt>50){
			cnt=0;
			if(!use_ctl.aux_online)use_ctl.aux_online=true;
		}
	}else{
		cnt=0;
		if(use_ctl.aux_online)use_ctl.aux_online=false;
	}
}
#endif

#ifdef SUPPORT_NO_PLAY_MUTE

extern int tone_get_dec_status();
extern u8 get_bt_connect_status(void);
#include "audio_dec_file.h"

#ifdef SUPPORT_AUX_NOT_SIGANL_MUTE
extern s32 aux_engery_val;
#endif
AT(.volatile_ram_code)
void user_not_play_mute(void)
{
   if (app_get_curr_task() == APP_BT_TASK)
   {
       if(tone_get_dec_status() == 0)
       {
            if(use_ctl.mic_is_online==true)
            {
               if(use_ctl.mute_flag==false)pa_unmute();
            }
#ifdef JT_VOL_IO_AD_EN		
           else if(use_ctl.jt_vol==true)
            {
               if(use_ctl.mute_flag==false)pa_unmute();
            }	
#endif	
#if TCFG_LINEIN_DETECT_ENABLE		
           else if(use_ctl.aux_online==true)
            {
               if(use_ctl.mute_flag==false)pa_unmute();
            }	
#endif	

            else
            {
               #ifdef SUPPORT_VOL_0_MUTE_PA
               if(use_ctl.led_vol == 0){
                  pa_mute();
                  return;
               }
               #endif
               if(use_ctl.bt_sta<=BT_STATUS_CONNECTING){
                   pa_mute();
               }else{
                   if(use_ctl.mute_flag==false)pa_unmute();
               }
            }
       }
       else
       {
           if(!use_ctl.mute_flag)pa_unmute();
       }
   }
#ifdef board_dy_DS4751C_FL872_ac6951c_6ADKEY	
   else if (app_get_curr_task() == APP_POWERON_TASK)
   {
   		pa_unmute();
   }
#elif defined MUTE_POWER_ON		
   else if (app_get_curr_task() == APP_POWERON_TASK)
   {
   		pa_mute();
   }
#endif	
   else if (app_get_curr_task() == APP_MUSIC_TASK)
   {
       if(tone_get_dec_status() == 0)
       {
            if(use_ctl.mic_is_online==true)
            {
               if(use_ctl.mute_flag==false)pa_unmute();
            }
#ifdef JT_VOL_IO_AD_EN		
           else if(use_ctl.jt_vol==true)
            {
               if(use_ctl.mute_flag==false)pa_unmute();
            }	
#endif	
#if TCFG_LINEIN_DETECT_ENABLE		
           else if(use_ctl.aux_online==true)
            {
               if(use_ctl.mute_flag==false)pa_unmute();
            }	
#endif	
            else
            {
               #ifdef SUPPORT_VOL_0_MUTE_PA
               if(use_ctl.led_vol == 0){
                  pa_mute();
                  return;
               }
               #endif
               if(file_dec_is_pause()){
                   pa_mute();
               }else{
                   if(use_ctl.mute_flag==false)pa_unmute();
               }
            }
       }
       else
       {
          if(!use_ctl.mute_flag)pa_unmute();
       }
   }
   else if (app_get_curr_task() == APP_LINEIN_TASK)
   {
       #if 0//def SUPPORT_AUX_NOT_SIGANL_MUTE
	       static u8 not_siganl_cnt=0;
	       if(tone_get_dec_status() == 0)
	       {
	            if(use_ctl.mic_is_online==true)
	            {
	               if(use_ctl.mute_flag==false)pa_unmute();
	            }
	            else
	            {
	                #ifdef SUPPORT_VOL_0_MUTE_PA
	               if(use_ctl.led_vol == 0){
	                  pa_mute();
	                  return;
	               }
	               #endif
	                if(aux_engery_val<1500)
	                {
	                    not_siganl_cnt++;
	                    if(not_siganl_cnt>8)
	                    {
	                        pa_mute();
	                    }
	                }
	                else
	                {
	                    not_siganl_cnt = 0;
	                    if(!use_ctl.mute_flag)pa_unmute();
	                }
	            }
	       }
	       else
	       {
	            if(!use_ctl.mute_flag)pa_unmute();
	       }
       #else
	   
       if(tone_get_dec_status() == 0)
       {
            if(use_ctl.mic_is_online==true)
            {
               if(use_ctl.mute_flag==false)pa_unmute();
            }
#ifdef JT_VOL_IO_AD_EN		
           else if(use_ctl.jt_vol==true)
            {
               if(use_ctl.mute_flag==false)pa_unmute();
            }	
#endif		   
            else
            {
               #ifdef SUPPORT_VOL_0_MUTE_PA
               if(use_ctl.led_vol == 0){
                  pa_mute();
                  return;
               }
               #endif
#ifdef SUPPORT_LINEIN_DET
							  if(use_ctl.aux_online==false){
								  pa_mute();
							  }
							  else
#endif
				{
                   if(use_ctl.mute_flag==false)pa_unmute();
               }
            }
       }
       else
       {
          if(!use_ctl.mute_flag)pa_unmute();
       }
       #endif
       
   }
}
#endif


#ifdef SUPPORT_MIC_PR_CHECK

 u8 vol_backup_temp = 0xff;
 u8 vol_obj_val = 0xff;

AT(.volatile_ram_code)
void mic_precedence_check(void)
{
    
    static u8 mic_precedence_st = 1;
    static u8 delay_set_vol_cnt = 0;

 if (app_get_curr_task() == APP_IDLE_TASK) 
 	return;
  
		
    #if 1
	    if(use_ctl.mic_is_online==false)
		{
			use_ctl.mic_precedence_flag=0;
		}
    #endif

	
    #ifdef SUPPORT_MIC_PR_USE_IO_CHECK
	
   // JL_PORTA->DIE |= BIT(3);JL_PORTA->DIR |= BIT(3);JL_PORTA->PD &= ~BIT(3);JL_PORTA->PU |= BIT(3); 
    
    static u8 mic_check_cnt=0;
    MIC_PR_IO_CHECK_INIT();
    if(MIC_HAVE_SIGNAL()){
       mic_check_cnt++;
       if(mic_check_cnt>2){
          use_ctl.mic_precedence_flag=1;
       }
      // use_ctl.test = 1;
    }else{
      mic_check_cnt=0;
      use_ctl.mic_precedence_flag=0;
      //use_ctl.test = 0;
    }
	  
    #elif defined SUPPORT_MIC_PR_USE_AD_CHECK
	    // 话筒优先AD检测 
	    u16 mic_ad;
	    static u16 mic_ad_min=0,mic_ad_max=0,ad_cnt=0,mic_engery=0;
	    mic_ad = adc_get_value(AD_CH_MIC);
	    ad_cnt++;
	    if(ad_cnt>30){
	       ad_cnt=0;
	       if(mic_ad_max>=mic_ad_min)//  检测峰峰值
	       {
	          mic_engery = mic_ad_max-mic_ad_min;
	          //printf("\n miceng= %d",mic_engery);
	       }
	       mic_ad_min = 1023;
	       mic_ad_max = 0;
	       
	    }else{
	       if(mic_ad_min>mic_ad)
	           mic_ad_min=mic_ad;
	       if(mic_ad_max<mic_ad)
	          mic_ad_max=mic_ad;
	    }
	    if(mic_engery>5)
	    {
	       use_ctl.mic_precedence_flag=1;
	    }
	    else
	    {
	        use_ctl.mic_precedence_flag=0;
	    }
    #endif
   

    if(use_ctl.mic_sw)		//话筒优先标志位,按键触发
	{
        if(use_ctl.mic_precedence_flag)
		{
            if(use_ctl.led_vol>=0)
            {
                vol_obj_val = use_ctl.led_vol;
                vol_backup_temp = 0;
                use_ctl.led_vol_pr = vol_backup_temp;
                app_common_key_var_event(KEY_MIC_PRE_VOL_DEAL);
                delay_set_vol_cnt = 20;
            }
        }
		else
		{
            if(delay_set_vol_cnt)
			{
               delay_set_vol_cnt--;
               return;
            }
  	  		else
                delay_set_vol_cnt = 10;
		
            if(vol_obj_val != use_ctl.led_vol)
            {
                vol_obj_val = 0;
            }
            if(vol_backup_temp < vol_obj_val)
            {
                vol_backup_temp++;
                if(vol_backup_temp == vol_obj_val)	vol_obj_val = 0; 
                use_ctl.led_vol_pr = vol_backup_temp;
                app_common_key_var_event(KEY_MIC_PRE_VOL_DEAL);
            }
        }
    }
   else
   {
        if(delay_set_vol_cnt)
		{
            delay_set_vol_cnt--;
            return;
        }
               
        if(vol_obj_val != use_ctl.led_vol)
        {
            vol_obj_val = 0;
        }
        if(vol_backup_temp < vol_obj_val)
        {
            vol_backup_temp++;
            if(vol_backup_temp == vol_obj_val)vol_obj_val = 0; 
            use_ctl.led_vol_pr = vol_backup_temp;
            app_common_key_var_event(KEY_MIC_PRE_VOL_DEAL);
        }
    }
}
#endif



#ifdef REC_DISK_DEL
void rec_scan_disk_del(){
	u8 ret;
	static u8 ret_cnt=0;
	ret = music_app_check();
	if(!ret){
		if(ret_cnt++ > 50){
			app_task_switch_to(APP_LINEIN_TASK);
		}
	}else{
		ret_cnt = 0;
	}
}
#endif

#ifdef SUPPORT_NOSIGLE_AUTO_MUTE_AMP
AT(.volatile_ram_code)
void user_nosigle_auto_mute(void)
{
   if(tone_get_dec_status() == 0)
   	{

		#if defined SUPPORT_MIC_CHECK       
	            if((use_ctl.mic_is_online==true)&&(use_ctl.mic_precedence_flag))// 话筒插入说话中  
	            {
			 if(use_ctl.mute_flag==false)PA_UNMUTE();
	            }
	            else
		#endif			
		 if(use_ctl.bAutoMuteFlag)
		{
			PA_MUTE();	
		}
		else
		{
		   if(!use_ctl.mute_flag)
			PA_UNMUTE();	
		}
   	}
   else
	{
	   if(!use_ctl.mute_flag)
		PA_UNMUTE();	
	}   	
}
#endif

#ifdef SUPPORT_VM_MAIN
// 计时存贮 防止持续存储影响声音
AT(.volatile_ram_code)
void vm_vol_val(void)// 10msx20=200ms 
{
	if(use_ctl.bPowerOnShowDlyCnt)return;

#ifdef SUPPORT_VM_VOL
	if(use_ctl.vVolCnt)
	{
		use_ctl.vVolCnt--;
		if(use_ctl.vVolCnt==0)
		{
			syscfg_write(VM_MUSICVOL, &use_ctl.led_vol, 1);
			printf(">>>write ok\n");
		}
	}
#endif
	
#ifdef SUPPORT_VM_MICVOL
	if(use_ctl.vMicvolCnt)
	{
		  use_ctl.vMicvolCnt--;
		if(use_ctl.vMicvolCnt==0)
		{
			syscfg_write(VM_MICVOL, &use_ctl.mic_vol, 1);
		}
	}
#endif
#ifdef SUPPORT_VM_ECHO
	if(use_ctl.vEchoCnt)
	{
		  use_ctl.vEchoCnt--;
		if(use_ctl.vEchoCnt==0)
		{
			syscfg_write(VM_ECHO, &use_ctl.echo, 1);
		}
	}
#endif
#ifdef SUPPORT_VM_BASS
	if(use_ctl.vbassCnt)
	{
		  use_ctl.vbassCnt--;
		if(use_ctl.vbassCnt==0)
		{
			syscfg_write(VM_BASS, &use_ctl.bass, 1);
		}
	}
#endif
#ifdef SUPPORT_VM_TREB	 
	if(use_ctl.vTreCnt)
	{
		  use_ctl.vTreCnt--;
		if(use_ctl.vTreCnt==0)
		{
			syscfg_write(VM_TREB, &use_ctl.tre, 1);
		}
	}
#endif
#ifdef SUPPORT_VM_MID	 
	if(use_ctl.vMidCnt)
	{
		  use_ctl.vMidCnt--;
		if(use_ctl.vMidCnt==0)
		{
			syscfg_write(VM_MID, &use_ctl.mid, 1);
		}
	}
#endif

#ifdef SUPPORT_VM_XBASS
	if(use_ctl.vXbassCnt)
	{
		  use_ctl.vXbassCnt--;
		if(use_ctl.vXbassCnt==0)
		{
			syscfg_write(VM_XBASS, &use_ctl.xbass, 1);
		}
	}
#endif	
#ifdef SUPPORT_VM_MICBASS
	if(use_ctl.vMicBassCnt)
	{
		  use_ctl.vMicBassCnt--;
		if(use_ctl.vMicBassCnt==0)
		{
			syscfg_write(VM_MIC_BASS, &use_ctl.mic_bass, 1);
		}
	}
#endif	
#ifdef SUPPORT_VM_MICTRE
	if(use_ctl.vMicTreCnt)
	{
		  use_ctl.vMicTreCnt--;
		if(use_ctl.vMicTreCnt==0)
		{
			syscfg_write(VM_MIC_TRE, &use_ctl.mic_tre, 1);
		}
	}
#endif	

#ifdef SUPPORT_VM_MICDELAY
	if(use_ctl.vMicDelayCnt)
	{
		  use_ctl.vMicDelayCnt--;
		if(use_ctl.vMicDelayCnt==0)
		{
			syscfg_write(VM_MIC_DELAY, &use_ctl.micdelay, 1);
		}
	}
#endif	

#ifdef SUPPORT_VM_VOICE_ONOFF
	if(use_ctl.vVmVoiceOnoffCnt)
	{
		  use_ctl.vVmVoiceOnoffCnt--;
		if(use_ctl.vVmVoiceOnoffCnt==0)
		{
			syscfg_write(VM_VOICE_ONOFF, &use_ctl.bVoiceFlag, 1);
		}
	}
#endif
#ifdef SUPPORT_VM_EQ50HZ
	if(use_ctl.eq50hz_vmval!=use_ctl.eq50hz_val)
	{
		use_ctl.eq50hz_vmval = use_ctl.eq50hz_val;
		syscfg_write(VM_EQ50HZ, &use_ctl.eq50hz_vmval, 1);
	}
#endif
#ifdef SUPPORT_VM_EQ6KHZ
	if(use_ctl.eq6khz_vmval!=use_ctl.eq6khz_val)
	{
		use_ctl.eq6khz_vmval = use_ctl.eq6khz_val;
		syscfg_write(VM_EQ6KHZ, &use_ctl.eq6khz_vmval, 1);
	}
#endif

}
#endif		 


#ifdef	SUPPORT_M62429_16TIMEER
#ifndef	M62429_TIMEERX
#define	JL_TIMER_M62429_TIMEERX JL_TIMER0
#define	IRQ_TIME_M62429 IRQ_TIME0_IDX
#endif

static u8 m62429_ctl_1_level_data[]={
	0x15, 0x14, 0x13, 0x12, 
	0x11, 0x10, 0x0f, 0x0e, 
	0x0d, 0x0c, 0x0b, 0x0a, 
	0x09, 0x08, 0x07, 0x06, 
	0x05, 0x04, 0x03, 0x02, 0x01, 0x00
};

static u8 m62429_ctl_78_data[4]={ 3<<7,
									2<<7,
									1<<7,
									0<<7 };

	
static u8 m62429_ctl_ch[]={0,1};

static int m62429_ctl_data=0;

static bool m62429_sent=0;
//	0,1		ATT1,0~21 ATT2,0~3
int m62429_send_ctl(u8 ctl_ch,u8 level)
{
	if (m62429_ctl_data || m62429_sent)
	{
		return (-1);
	}
	m62429_sent=1;
	if ( ctl_ch == 1 )
	{
		m62429_ctl_data = BIT(1) | BIT(0) ;
		if (level>JTVOL_MAX)
		{
			level=JTVOL_MAX;
		}
		if (JTVOL_MAX>21)
		{
			level = JTVOL_MAX - level ;
			m62429_ctl_data |= ( ((level!=JTVOL_MAX)?( m62429_ctl_1_level_data[level/4] | m62429_ctl_78_data[level%4] ):(0)) <<2) ;
		}
		else
		{

			m62429_ctl_data |= (m62429_ctl_1_level_data[(21-level)]<<2) ;
		}
		m62429_ctl_data |= BIT(9) ;
		m62429_ctl_data |= BIT(10) ;
	}
	else if ( ctl_ch == 0 )
	{
		m62429_ctl_data = BIT(1) ;
		if (level>JTVOL_MAX)
		{
			level=JTVOL_MAX;
		}
		if (JTVOL_MAX>21)
		{
			level = JTVOL_MAX - level ;
			m62429_ctl_data |= ( ((level!=JTVOL_MAX)?( m62429_ctl_1_level_data[level/4] | m62429_ctl_78_data[level%4] ):(0)) <<2) ;
		}
		else
		{

			m62429_ctl_data |= (m62429_ctl_1_level_data[(21-level)]<<2) ;
		}
		m62429_ctl_data |= BIT(9) ;
		m62429_ctl_data |= BIT(10) ;
	}
	else
	{
		return (-1);
	}

	JL_TIMER_M62429_TIMEERX->CON |= BIT(0);

	
	return 0;
}

___interrupt
void m62429_timer_isr()
{
	JL_TIMER_M62429_TIMEERX->CON |= BIT(14);

	static u8 cnt_intx=0;
	int kkkkkkk=m62429_ctl_data;
	kkkkkkk &= (BIT(9)|BIT(10));
	if ( kkkkkkk != (BIT(9)|BIT(10)) || !m62429_sent)
	{
		cnt_intx = ( 2 + (11-2-2) + 2 )*4;
	}
	if ( cnt_intx< ( 2 + (11-2-2) + 2 )*4 )
	{
		switch (cnt_intx%4)
		{
			case 0:
				gpio_direction_output(M62429_CLK_PORT, 0);
				break;
			case 1:
				kkkkkkk = m62429_ctl_data;
				kkkkkkk = kkkkkkk & BIT(cnt_intx/4);
				gpio_direction_output(M62429_DAT_PORT, ((kkkkkkk)?1:0));
				break;
			case 2:
				gpio_direction_output(M62429_CLK_PORT, 1);
				break;
			case 3:
				kkkkkkk = cnt_intx;
				kkkkkkk = ( 2 + (10-2-2) + 2 )-kkkkkkk/4;
				gpio_direction_output(M62429_DAT_PORT, ((kkkkkkk)?0:1));
				break;
			default:
				cnt_intx = ( 2 + (10-2-2) + 2 )*4;
				break;
		}
		
		cnt_intx++;
	}
	else
	{
		cnt_intx=0;
		
		JL_TIMER_M62429_TIMEERX->CON &= ~BIT(0);
		JL_TIMER_M62429_TIMEERX->CNT = 0;
		
		gpio_set_direction(M62429_CLK_PORT, 0);
		gpio_set_pull_down(M62429_CLK_PORT, 0);
		gpio_set_pull_up(M62429_CLK_PORT, 0);
		gpio_direction_output(M62429_CLK_PORT, 0);
		
		gpio_set_direction(M62429_DAT_PORT, 0);
		gpio_set_pull_down(M62429_DAT_PORT, 0);
		gpio_set_pull_up(M62429_DAT_PORT, 0);
		gpio_direction_output(M62429_DAT_PORT, 1);

		m62429_ctl_data=0;
		m62429_sent=0;
	}
}

int m62429_timer_init()
{
	u32 prd_cnt;
	u8 index;
	JL_TIMER_M62429_TIMEERX->CON = 0;
	JL_TIMER_M62429_TIMEERX->CNT = 0;
	JL_TIMER_M62429_TIMEERX->PRD = 100; //2ms
	request_irq(IRQ_TIME_M62429, 3, m62429_timer_isr, 0);
	JL_TIMER_M62429_TIMEERX->CON = (0 << 4) | BIT(14) | BIT(3); // | BIT(0)

	
	gpio_set_direction(M62429_CLK_PORT, 0);
	gpio_set_pull_down(M62429_CLK_PORT, 0);
	gpio_set_pull_up(M62429_CLK_PORT, 0);
	gpio_direction_output(M62429_CLK_PORT, 0);
	
	gpio_set_direction(M62429_DAT_PORT, 0);
	gpio_set_pull_down(M62429_DAT_PORT, 0);
	gpio_set_pull_up(M62429_DAT_PORT, 0);
	gpio_direction_output(M62429_DAT_PORT, 1);
	
	return 0;
}


#endif


#ifdef SUPPORT_CD4051_PONTEN
#include "cd4051_ad_check.c"
#endif

#ifdef SUPPORT_MIC_BASS_TRE_SET
extern const char  mic_tre_table[MIC_TRE_MAX+1];
extern const char  mic_bass_table[MIC_BASS_MAX+1];
#endif

#ifdef SUPPORT_UI_UART_XY8080
u8 uart_data_XY8080[9]={0x87 ,0x3E ,0x04 ,0x28 ,0x21 ,0x2E ,0x4B ,0x1B ,0xA6};
u8 cnt_datstr=0;

bool Uart_XY8080_sendata(void)
{
	if ( cnt_datstr  )
	{
		return 1;
	}
	return 0;
}

bool resend_uart(void)
{
	if ( cnt_datstr  )
	{
		JL_UART0->CON0 &= ~BIT(0);
		cnt_datstr = 0;
		JL_UART0->CON0 |= BIT(13);
		JL_UART0->CON0 |= BIT(0);
		return 1;
	}
	JL_UART0->CON0 |= BIT(0);
	return 0;
}

SET_INTERRUPT
void Uart_XY8080_fun(void)
{
	JL_UART0->CON0 |= BIT(13);
	if ( cnt_datstr >= 9 )
	{
		cnt_datstr = 0;
		JL_UART0->CON0 &= ~BIT(0);
		return ;
	}
	JL_UART0->BUF = uart_data_XY8080[cnt_datstr];
	cnt_datstr++;
	
	return ;
}
bool uart_XY8080_init(void)
{
	JL_UART0->CON0 = 0;
	gpio_set_uart0(-1);
		
	JL_UART0->CON1 = 0;
		
	JL_UART0->CON0 |= BIT(13) | BIT(12) | BIT(10) | BIT(2);
	JL_UART0->BAUD = ((clk_get("uart") + 19200 / 2) / 19200) / 4 - 1;
	
	request_irq(IRQ_UART0_IDX, 1, Uart_XY8080_fun, 0);
	
	if (UI_UART_XY8080_PORT!=(-1))
		gpio_output_channle(UI_UART_XY8080_PORT, CH0_UT0_TX);
//	if (Uart__rx_PORT_RX!=(-1))
//		gpio_uart_rx_input(Uart__rx_PORT_RX, Uart__rx_SET_UARTX, Uart__rx_SET_UARTX);

	JL_UART0->CON0 |= BIT(0);
	return true;

}

#endif



#ifdef SUPPORT_VM_SAVE_WORK_MODE

void vm_save_work_mode(u8 workmode)
{
	syscfg_write(VM_WORK_MODE,&workmode,1);
}

u8 vm_read_work_mode()
{
	u8 ret;
	u8 work_mode;
	ret=syscfg_read(VM_WORK_MODE,&work_mode,1);
	if (ret!=1 || work_mode>=WORK_MODE_MAX)
	{
		work_mode = 0;
	}
	return work_mode;
}

int poweron_goto_vm_work_mode()
{

	int func_ret = false;
	u8 ret,work_mode;
	work_mode = vm_read_work_mode();
	
	switch (work_mode)
	{
	case WORK_BT_MODE:
		func_ret = app_task_switch_to(APP_BT_TASK);
		break;
	case WORK_FM_MODE:
		func_ret = app_task_switch_to(APP_FM_TASK);
		break;
	case WORK_AUX_MODE:
		// use_ctl.vMultModeVal = MODE_AUX0;

		//syscfg_read(VM_LINEIN_CH,&use_ctl.vMultModeVal,1);
		func_ret = app_task_switch_to(APP_LINEIN_TASK);
		break;
	#ifdef WORK_AUX1_MODE
	// case WORK_AUX1_MODE:
	// 	use_ctl.vMultModeVal = MODE_AUX1;
	// 	app_task_switch_to(APP_LINEIN_TASK);
	// 	break;
	#endif
	// case WORK_AUX2_MODE:
	// 	use_ctl.vMultModeVal = MODE_AUX2;
	// 	app_task_switch_to(APP_LINEIN_TASK);
	// 	break;
#ifdef 	SUPPORT_SPDIF_OPT_MODE
	case WORK_OPT_MODE:
		use_ctl.bSpdifMode = SPDIF_OPT;
		printf("enter:OPT\n");
		func_ret = app_task_switch_to(APP_SPDIF_TASK);
		break;
#endif
#ifdef 	SUPPORT_SPDIF_COAX_MODE
	case WORK_COAX_MODE:
		use_ctl.bSpdifMode = SPDIF_COAX;
		printf("enter:COA\n");
		func_ret = app_task_switch_to(APP_SPDIF_TASK);
		break;
#endif
#ifdef 	SUPPORT_SPDIF_HDMI_MODE
	case WORK_HDMI_MODE:
#if (!defined SUPPORT_ENTER_HDMI_NOT_CHECK) && (defined SUPPORT_SPDIF_HDMI_CHECK)
		if (get_hdmi_online())
		{
			use_ctl.bSpdifMode = SPDIF_HDMI;
			func_ret = app_task_switch_to(APP_SPDIF_TASK);
		}
#else
			use_ctl.bSpdifMode = SPDIF_HDMI;
			func_ret = app_task_switch_to(APP_SPDIF_TASK);
#endif
		break;
#endif
#ifdef SUPPORT_VM_SAVE_USB_SD_MODE
	case WORK_USB_MODE:
		struct __dev *dev;
		dev = dev_manager_check_by_logo("udisk0");
		if(dev){
			use_ctl.music_device = 1;
			func_ret = app_task_switch_to(APP_MUSIC_TASK);
		}
		break;
	case WORK_SD_MODE:
		dev = dev_manager_check_by_logo("sd0");
		if (dev == NULL)
		{
			dev = dev_manager_check_by_logo("sd1");
		}
		if(dev){
			use_ctl.music_device = 2;
			func_ret = app_task_switch_to(APP_MUSIC_TASK);
		}
		break;
#else
	case WORK_MUSIC_MODE:
		func_ret = app_task_switch_to(APP_MUSIC_TASK);
		break;
#endif
	default:
		func_ret = false;
		break;
	}
	
	return func_ret;
}
#endif






u16 time_cnt;
AT(.volatile_ram_code)
void user_scan(void)//10ms
{


	time_cnt++;
#ifdef SPI1_RGB_DEV_EN
#if defined SPI1_RGB_RT_32LED
	 user_dac_energy_scan();
#endif
#endif
#ifdef REC_DISK_DEL
	if(app_curr_task==APP_RECORD_TASK)
	{
		rec_scan_disk_del();
	}
#endif
#ifdef SUPPORT_LINEIN_DET
	linein_check();
#endif

   #ifdef SUPPORT_PP_LED
	  led_scan();
  #endif

  #ifdef BAT_EXTERN_AD_EN
  battery_ad_check();
  #endif


  #ifdef VOL_IO_AD_EN
  advol_check();
  #endif
  
  #if SUPPORT_USER_EQ_MODE
  #ifdef TRE_IO_AD_EN
  adtre_check();
  #endif
  #ifdef BASS_IO_AD_EN
  adbass_check();
  #endif
  
  #if defined SUPPORT_PONTEN_EQ1_EQ5 
  ad_eq1_check();
  ad_eq2_check();
  ad_eq3_check();
  ad_eq4_check();
  ad_eq5_check();
#endif

  #ifdef MID_IO_AD_EN
  admid_check();
  #endif

#endif

    #ifdef SUPPORT_MIC_CHECK
	mic_check();
	#endif

  #ifdef MICVOL_IO_AD_EN
  admicvol_check();
  #endif
  #ifdef ECHO_IO_AD_EN
  adecho_check();
  #endif
#ifdef MIC_DELAY_IO_AD_EN
 ad_mic_delay_check();
#endif

  #ifdef MIC_BASS_IO_AD_EN
  mic_bass_check( );
#endif
  #ifdef MIC_TRE_IO_AD_EN
  mic_tre_check( );
#endif

#ifdef JT_VOL_IO_AD_EN
 adjtvol_check();
#endif

  #if defined SUPPORT_JT_CHECK
   jt_check();
  #endif

#ifdef SUPPORT_LED_FLASH
			 user_led_flash(); // 100ms
#endif


 #ifdef SPI1_RGB_DEV_EN
#ifdef SUPPORT_LED_SIGLE
	 user_led_sigle(); // 100ms
#endif
#endif



  #ifdef SUPPORT_CD4051_PONTEN
  #if 0
  
     if((time_cnt%2)==0)
	cd4051_scan();

#else
 //    if((time_cnt%5)==0)
//	  cd4051_ad_check();

   if((time_cnt%2)==0)
		  cd4051_val_init();
   
   if((time_cnt%3)==0)
		  ad_eq1_check();
   
    if((time_cnt%4)==0)  
		  ad_eq2_check();
	
    if((time_cnt%3)==0)	
		  ad_eq3_check();
	
    if((time_cnt%4)==0)	
		  ad_eq4_check();
	
    if((time_cnt%4)==0)	
		  ad_eq5_check();
#endif	

#endif

  #ifdef SUPPORT_NOSIGLE_AUTO_MUTE_AMP
	 user_nosigle_auto_mute();
#endif

#ifdef SUPPORT_SD_DAT_MULTIPLEX_AD
   if((time_cnt%10)==0)
   {
    sd_dat_ad_multiplex();
   }  
#endif

  #ifdef SUPPORT_MIC_PR_CHECK
  mic_precedence_check();
  #endif
  #ifdef DCIN_DET
  DcIn_check();
  #ifdef SW_ONOFF_DET
  sw_onoff_check();
  #endif
  #endif
  
    #ifdef SUPPORT_ENCORD_LED 
	Encoder_LED();
	#endif				

 #ifdef SUPPORT_ENCODER
	    if(use_ctl.disable_key_cnt)  use_ctl.disable_key_cnt--;
#endif
#if defined  board_LH_808_20led_ac6951c  ||  defined  board_DG_1263_2led_ac6951c
	if(time_cnt==10  ||  time_cnt==25	  ||  time_cnt==40)
	{
		static u8 key_ctl_power_off=0;
		JL_PORTC->DIE |= BIT(4);JL_PORTC->DIR |= BIT(4);JL_PORTC->PD &= ~BIT(4);JL_PORTC->PU |= BIT(4);
		if (key_ctl_power_off>20)
		{
			JL_PORTA->DIE |= BIT(3);JL_PORTA->DIR &= ~BIT(3);JL_PORTA->OUT &= ~BIT(3);JL_PORTA->PD &= ~BIT(3);JL_PORTA->PU &= ~BIT(3);
		}
		else if (key_ctl_power_off>=4)
		{
			key_ctl_power_off++;
			if (app_get_curr_task()!=APP_IDLE_TASK)
			{
				app_task_switch_to(APP_IDLE_TASK);
			}
		}
		else if (key_ctl_power_off>=3)
		{
			key_ctl_power_off++;
			use_ctl.led_vol_pr=0;
			use_ctl.mute_flag=1;
			PA_MUTE();
			app_common_key_var_event(KEY_MIC_PRE_VOL_DEAL);
		}
		if (!(JL_PORTC->IN & BIT(4)))
		{
			if (key_ctl_power_off && (key_ctl_power_off<3))
			{
				key_ctl_power_off++;
			}
		}
		else
		{
			if (key_ctl_power_off<3)
			{
				JL_PORTA->DIE |= BIT(3);JL_PORTA->DIR &= ~BIT(3);JL_PORTA->OUT |= BIT(3);JL_PORTA->PD &= ~BIT(3);JL_PORTA->PU &= ~BIT(3);
				key_ctl_power_off=1;
			}
		}
	}
#endif

#ifdef  board_DG_1258_ac6951c  
static u8 cnt_DG_1258_LED=1;
	  if(time_cnt==0  ||  time_cnt==25)
	  {
		if (kkkkk)
		{
			kkkkk--;
			if (cnt_DG_1258_LED++)
			{
				cnt_DG_1258_LED=0;
				p33_tx_1byte(R3_OSL_CON, 0);rtc_port_pr_die(1, 1);rtc_port_pr_hd(1, 1);rtc_port_pr_out(1, !(cnt_DG_1258_LED) );
			}
			else
			{
				p33_tx_1byte(R3_OSL_CON, 0);rtc_port_pr_die(1, 1);rtc_port_pr_hd(1, 1);rtc_port_pr_out(1, !(cnt_DG_1258_LED) );
			}
		}
		else
		{
			cnt_DG_1258_LED=0;
			p33_tx_1byte(R3_OSL_CON, 0);rtc_port_pr_die(1, 1);rtc_port_pr_hd(1, 1);rtc_port_pr_out(1, !(cnt_DG_1258_LED) );
		}
	  }
#endif

  if(time_cnt>=50)//half  second
  {
	time_cnt=0;
	
	if(use_ctl.eq_menu_cnt)
	{
		use_ctl.eq_menu_cnt--;
		if(!use_ctl.eq_menu_cnt)use_ctl.eq_menu = 0;
	}

    #ifdef SUPPORT_NO_PLAY_MUTE
	    user_not_play_mute();
    #endif

	 #ifdef SUPPORT_POWER_ON_FIRST_DISABLE_BT_VOICE
	 	  if(use_ctl.bPowerOnDisableBtVoiceCnt)use_ctl.bPowerOnDisableBtVoiceCnt--;
	 #endif
	
       
	 #ifdef SUPPORT_POWER_ON_FIRST_DISABLE_BT_VOICE
	 	  if(use_ctl.bPowerOnDisableBtVoiceCnt)use_ctl.bPowerOnDisableBtVoiceCnt--;
	 #endif

	 #ifdef SUPPORT_POWER_ONOFF_BAT
	 	if(use_ctl.bBatCnt)
		 {
	 		use_ctl.bBatCnt--;
	 		if(use_ctl.bBatCnt==0)
	 		{
	 		   BAT_IO_SET_ON();
	 		}
	 	}
	 #endif


     /************数字键功能发送*****************/
     if(use_ctl.input_time_out)
	 {
          use_ctl.input_time_out--;
          if(use_ctl.input_time_out==0)
		  {
             use_ctl.input_number_cnt = 0;
             app_common_key_var_event(KEY_INPUT_TIMEOUT);
          }
     }

	 #ifdef SUPPORT_ENCORD_LED 
	 	if(use_ctl.bEncordSw)use_ctl.bEncordSw--;
	 #endif

#if (TCFG_MIC_EFFECT_ENABLE)
	 

     if(use_ctl.power_on_mic_init){
        use_ctl.power_on_mic_init--;
        if(use_ctl.power_on_mic_init==0)
        {	
	        app_common_key_var_event(KEY_REVERB_OPEN);
        }
     }
#endif

     if( use_ctl.voice_play_cnt) use_ctl.voice_play_cnt--;
    // use_ctl.test = app_var.music_volume;
    if(use_ctl.pa_delay_unmute)
	{
        use_ctl.pa_delay_unmute--;
        if(use_ctl.pa_delay_unmute==0)
		{
           pa_unmute();
        }
    }
	
#ifdef SUPORT_MENU_MODE
    if(use_ctl.menu_mode_cnt)
	{
       use_ctl.menu_mode_cnt--;
       if(use_ctl.menu_mode_cnt==0)use_ctl.menu_mode=0;
    }
#endif
#ifdef SUPORT_MENU1_MODE

    if(use_ctl.menu1_mode_cnt)
	{
       use_ctl.menu1_mode_cnt--;
       if(use_ctl.menu1_mode_cnt==0)use_ctl.menu1_mode=0;
    }
#endif
	
#ifdef SUPPORT_OK_MENU_KEY
    if(use_ctl.bMicMenuCnt)
	{
       use_ctl.bMicMenuCnt--;
       if(use_ctl.bMicMenuCnt==0)use_ctl.bMicMenu=0;
    }
#endif

#ifdef SUPPORT_POWER_ON_AD_SHOW_DLY
    if(use_ctl.bPowerOnShowDlyCnt)  use_ctl.bPowerOnShowDlyCnt--;
#endif		 

#ifdef SUPPORT_init_RGB_MODE_15s_change
static u8 kkkkkkkkkkkkkk=0;
if (	(light_mode_mark & 0x10)	&&	kkkkkkkkkkkkkk++>60	)
{
	kkkkkkkkkkkkkk=0;
	light_mode_mark ++;
	if ((light_mode_mark & 0x0f)>=RGB_MAX)
	{
		light_mode_mark =	0x10 | 1;
	}

}
#endif

#ifdef SUPPORT_KEY_RGB_ADKEY
		if (use_ctl.key_rgb_cnt_adkey )
		{
			use_ctl.key_rgb_cnt_adkey--;
		}
#endif

#ifdef SUPPORT_KEY_RGB_ADC
		if (use_ctl.key_rgb_cnt_adc )
		{
			use_ctl.key_rgb_cnt_adc--;
		}
#endif

#ifdef SPI1_RGB_DG_1257_79
extern u16 flash1cnt;
		if (flash1cnt<32768)
		{
			flash1cnt++;
		}
#endif


		static int flag_flash = 0;
		static int mute_flash = 0;
		static int mode_led = 0;


		if(app_get_curr_task()==APP_IDLE_TASK)
		{
			switch(mode_led)
			{
				case 1:JL_PORTC->DIE |= BIT(2);JL_PORTC->DIR |= BIT(2);JL_PORTC->OUT |= BIT(2);JL_PORTC->PD |= BIT(2);JL_PORTC->PU |= BIT(2);
					LED_OPT_off();
					LED_BT_off();
					LED_linein_off();
					if(!use_ctl.standby_out_to_spdif)
						use_ctl.bSpdifMode = SPDIF_COAX;
					break;
				case 2:JL_PORTC->DIE |= BIT(1);JL_PORTC->DIR |= BIT(1);JL_PORTC->OUT |= BIT(1);JL_PORTC->PD |= BIT(1);JL_PORTC->PU |= BIT(1);
					LED_COAX_off();
					LED_BT_off();
					LED_linein_off();
					if(!use_ctl.standby_out_to_spdif)
						use_ctl.bSpdifMode = SPDIF_OPT;
					break;
				case 3:JL_PORTC->DIE |= BIT(0);JL_PORTC->DIR |= BIT(0);JL_PORTC->OUT |= BIT(0);JL_PORTC->PD |= BIT(0);JL_PORTC->PU |= BIT(0);
					LED_COAX_off();
					LED_OPT_off();
					LED_linein_off();
					break;
				case 4:JL_PORTA->DIE |= BIT(12);JL_PORTA->DIR |= BIT(12);JL_PORTA->OUT |= BIT(12);JL_PORTA->PD |= BIT(12);JL_PORTA->PU |= BIT(12);
					LED_COAX_off();
					LED_OPT_off();
					LED_BT_off();
					break;

				default:
					break;
			}
		}

		else if(app_get_curr_task()==APP_SPDIF_TASK)
		{
			if (use_ctl.bSpdifMode==SPDIF_COAX)
			{
				mode_led = 1;
				LED_OPT_off();
				LED_BT_off();
				LED_linein_off();
				if(use_ctl.mute_flag)
				{
					if(mute_flash<=2)
					{
						mute_flash++;
						LED_COAX_on();
					}
					else if(mute_flash<=4)
					{
						mute_flash++;
						LED_COAX_off();
					}
					else
						mute_flash=0;
				}
				else
				{
					LED_COAX_on();
					mute_flash=0;
				}
			}
			if (use_ctl.bSpdifMode==SPDIF_OPT)
			{
				mode_led = 2;
				LED_COAX_off();
				LED_BT_off();
				LED_linein_off();
				if(use_ctl.mute_flag)
				{
					if(mute_flash<=2)
					{
						mute_flash++;
						LED_OPT_on();
					}
					else if(mute_flash<=4)
					{
						mute_flash++;
						LED_OPT_off();
					}
					else
						mute_flash=0;
				}
				else
				{
					LED_OPT_on();
					mute_flash=0;
				}
				
			}
		}
		else if(app_get_curr_task()==APP_BT_TASK)
		{
			mode_led = 3;
			LED_COAX_off();
			LED_OPT_off();
			LED_linein_off();

			if(use_ctl.bt_sta < BT_STATUS_CONNECTING)	//未连接
			{
				if(use_ctl.mute_flag)
				{
					if(mute_flash<=2)
					{
						mute_flash++;
						LED_BT_on();
					}
					else if(mute_flash<=4)
					{
						mute_flash++;
						LED_BT_off();
					}
					else
						mute_flash=0;
				}
				


				else if(flag_flash)
				{
					flag_flash = 0;
					LED_BT_off();
				}
				else
				{
					flag_flash = 1;
					LED_BT_on();
				}
			}
			else									//已连接
			{
				if(use_ctl.mute_flag)
				{
					if(mute_flash<=2)
					{
						mute_flash++;
						LED_BT_on();
					}
					else if(mute_flash<=4)
					{
						mute_flash++;
						LED_BT_off();
					}
					else
						mute_flash=0;
				}
				
				else if(flag_flash)
				{
					flag_flash = 0;
					LED_BT_off();
				}

				else
				{
					LED_BT_on();
					mute_flash=0;
				}
			}
		}
		else if(app_get_curr_task()==APP_LINEIN_TASK)
		{
			mode_led = 4;
			LED_COAX_off();
			LED_OPT_off();
			LED_BT_off();
			if(use_ctl.mute_flag)
			{
				if(mute_flash<=2)
				{
					mute_flash++;
					LED_linein_on();
				}
				else if(mute_flash<=4)
				{
					mute_flash++;
					LED_linein_off();
				}
				else
					mute_flash=0;
			}
			else
			{
				LED_linein_on();
				mute_flash=0;
			}

		}
		else
		{
			mode_led = 0 ;
			LED_COAX_off();
			LED_OPT_off();
			LED_BT_off();
			LED_linein_off();
		}



	
  }
  
}


#ifdef SUPPORT_SPDIF_HDMI_CHECK
#define HDMI_ARC_CHECK_TIME 	10	//N*10MS

void set_hdmi_online(u8 online)
{
    use_ctl.hdmi_is_online = !!online;
}


type_timer_id user_timer;
// AT(.volatile_ram_code)
AT(.volatile_ram_code)

void hdmi_check()
{
	u8 io_status;
	static u8 prev_status;
	static u16 hd_cnt = 0;
	HDMI_DET_IO_INIT();

	io_status = !!HDMI_IS_IN();

	if (io_status != prev_status)
	{
		hd_cnt++;
	}
	else
	{
		hd_cnt = 0;
	}

	if (hd_cnt < HDMI_ARC_CHECK_TIME) 
	{	//滤波计算
        return;
    }

	prev_status = io_status;

	if ((get_hdmi_online()) && (!io_status))
	{
		printf("\n!!!!!!!!!!!!hdmi_io_out!!!!!!!\n");
        set_hdmi_online(false);
#ifndef SUPPORT_HDMI_IN_NOT_TO_HDMI_MODE
		app_common_key_var_event(KEY_HDMI_OUT);
#endif
    } 
	else if ((!get_hdmi_online()) && (io_status)) 
	{
		printf("\n!!!!!!!!!!!!!hdmi_io_in!!!!!!!!!!!!\n");
        set_hdmi_online(true);
#ifndef SUPPORT_HDMI_IN_NOT_TO_HDMI_MODE
		app_common_key_var_event(KEY_HDMI_IN);
#endif
    }
}
#endif




void user_init(void)
{
u8 ret;

#if defined board_ac695x_JY_tx_2212_64led_ac6951c || defined board_ac695x_JY_tx_2310_64led_ac6951c || defined JY_tx_2208_7v4_48led_ac6951c || defined board_ac695x_JY_tx_2210_56led_ac6951c || defined board_ac695x_JY_TX_2208_48LED_AC6951C
	JL_PORTC->DIE |= BIT(4);JL_PORTC->DIR &= ~BIT(4);  JL_PORTC->PU &= ~BIT(4);JL_PORTC->PD &= ~BIT(4);JL_PORTC->OUT &= ~BIT(4);
#endif

#ifdef	SUPPORT_M62429_16TIMEER
		m62429_timer_init();
#endif

#ifdef SUPPORT_UI_XY8080
#ifdef XY8080_LED_DEBUG
		use_ctl.xy8080_com=0;
		use_ctl.xy8080_seg=0;
#endif
		TX_IO_INIT();
		use_ctl.start_send_date_falg=0;
		use_ctl.bSendBusy=0;
		idle_timer2_init();
#endif


#ifdef SUPPORT_LINEIN_DET
	LINEIN_DET_INIT();
#endif
#ifdef SUPPORT_POWER_IO_EN // POWERON-H ELSE L
	POWER_EN_ON();
#endif

#ifdef SUPPORT_LOWPOWER_VOL_AUTO_ADJUST
	led_bat.bBatStateTemp =  31; // 6.9V 25 step
#endif

  use_ctl.dac_sys_max_vol = LED_SYS_VOL;
  use_ctl.power_on_mic_init=8;
  
#ifdef SUPPORT_POWER_ON_AD_SHOW_DLY
	use_ctl.bPowerOnShowDlyCnt = 12;
#endif		 
  #ifdef MULT_IO_PA1PA2_OFF_PA2
    JL_PORTA->DIR |=BIT(2);  JL_PORTA->PU &= ~BIT(2);JL_PORTA->PD &= ~BIT(2);	
    JL_PORTA->DIR |=BIT(1);  JL_PORTA->PU &= ~BIT(1);JL_PORTA->PD &= ~BIT(1);	
  #endif
  #ifdef MULT_IO_PA4PA5_OFF_PA4
    JL_PORTA->DIR |=BIT(4);  JL_PORTA->PU &= ~BIT(4);JL_PORTA->PD &= ~BIT(4);	
  #endif
  #ifdef MULT_IO_PA0PC7_OFF_PA0
    JL_PORTA->DIR |=BIT(0);  JL_PORTA->PU &= ~BIT(0);JL_PORTA->PD &= ~BIT(0);	
    JL_PORTC->DIR |=BIT(7);  JL_PORTC->PU &= ~BIT(7);JL_PORTC->PD &= ~BIT(7);	
  #endif

#if !defined SUPPORT_MIC_CHECK||defined SUPPORT_MIC_ALWAY_ONLINE
	use_ctl.mic_is_online=1;
#endif

#ifdef SUPPORT_MULT_INPUT_IN_LINEIN
#ifdef MULT_INPUT_IN_DEFAULT
	use_ctl.mult_mode=MULT_INPUT_IN_DEFAULT;
#else
	use_ctl.mult_mode=MODE_AUX;
#endif
	use_ctl.bLineChannelVal=TCFG_LINEIN_LR_CH;
#endif


#ifdef SPI1_RGB_DEV_EN
#if defined SPI_RGB_JY_28LED ||defined SPI1_RGB_sub2100 ||defined SPI1_RGB_DEV_EN ||defined SPI1_RGB_DG_1257_73 ||defined SPI1_RGB_LH_808_20 ||defined SPI1_RGB_DG_1263_2
	light_mode_mark = 1;
	usr_timer_add(NULL,user_rgb_scan_mode,1,1);
#else 
	usr_timer_add(NULL,user_rgb_scan_mode,1,1);
#endif
#endif

#ifdef SUPPORT_VM_MAIN
	usr_timer_add(NULL,vm_vol_val,250,1);
#endif

#ifdef SUPPORT_init_RGB_MODE_15s_change
	light_mode_mark |= 0x10;

#endif

//---------------------------------------------------

  #ifdef BAT_EXTERN_AD_EN
  #ifndef SUPPORT_SD_DAT_MULTIPLEX_AD
     BAT_DET_IO_INIT();
     adc_add_sample_ch(AD_CH_BAT); 
   #endif
   #endif
   
  #ifdef VOL_IO_AD_EN
    VOL_AD_IO_INIT();
    use_ctl.led_vol = 0;
    adc_add_sample_ch(AD_CH_VOL);
  #endif

  #if SUPPORT_USER_EQ_MODE
  #ifdef TRE_IO_AD_EN
    TRE_AD_IO_INIT();
    use_ctl.tre = 0;
    adc_add_sample_ch(AD_CH_TRE); 
  #endif
  #ifdef BASS_IO_AD_EN
    BASS_AD_IO_INIT();
//    use_ctl.bass= 0;
    adc_add_sample_ch(AD_CH_BASS); 
  #endif
   #ifdef MID_IO_AD_EN
    MID_AD_IO_INIT();
    use_ctl.mid= 0;
    adc_add_sample_ch(AD_CH_MID); 
  #endif 
#endif 


   #ifdef MICVOL_IO_AD_EN    
    MICVOL_AD_IO_INIT();
    use_ctl.mic_vol = 0;
    adc_add_sample_ch(AD_CH_MICVOL); 
  #endif
  #ifdef ECHO_IO_AD_EN
	ECHO_AD_IO_INIT();
//	use_ctl.echo = 0;
	adc_add_sample_ch(AD_CH_ECHO); 
  #endif
  #ifdef MIC_DELAY_IO_AD_EN
	MIC_DELAY_AD_IO_INIT();
	use_ctl.micdelay= 0;
	adc_add_sample_ch(AD_CH_MICDELAY); 
#endif

  #ifdef MIC_BASS_IO_AD_EN
    MIC_BASS_AD_IO_INIT();
//    use_ctl.mic_bass= 0;
    adc_add_sample_ch(AD_CH_MIC_BASS); 
#endif

  #ifdef MIC_TRE_IO_AD_EN
    MIC_TRE_AD_IO_INIT();
    use_ctl.mic_tre= 0;
    adc_add_sample_ch(AD_CH_MIC_TRE); 
#endif


 #ifdef JT_VOL_IO_AD_EN
	JTVOL_AD_IO_INIT();
	adc_add_sample_ch(AD_CH_JTVOL); 
#endif

#ifdef SUPPORT_USE_DOUBLE_AD_IO_KEY
    ADK1_IO_INIT();
    adc_add_sample_ch(AD_CH_ADK1); 
#endif

//---------------------------------------------------
  #ifndef VOL_IO_AD_EN

  #ifdef SUPPORT_VM_VOL	
 	 	u8 bLedVolVal;
        ret = syscfg_read(VM_MUSICVOL, &bLedVolVal, 1);
		 

		 
        if ((bLedVolVal > LED_SHOW_MAX_VOL)||(bLedVolVal <= 0)||(ret ==4)) 
		{
         	bLedVolVal = LED_VOL_DETAULT_VAL; 
        }
 		use_ctl.led_vol= bLedVolVal;			
 
 #else
	  use_ctl.led_vol = LED_VOL_DETAULT_VAL;
  #endif	
 #endif
 
 #ifdef TRE_DEFAULT_VAL
 	#ifdef SUPPORT_VM_TREB
		 	u8 bTreVal;
	        ret = syscfg_read(VM_TREB, &bTreVal, 1);
  //        printf("\n --222------------ ret=%d\n",ret);
			
	        if ((bTreVal > TRE_MAX)||(bTreVal <= 0)||(ret ==4)) 
			{
	            bTreVal = TRE_DEFAULT_VAL; 
	        }	
	 	use_ctl.tre = bTreVal;		
	#else
	 	use_ctl.tre = TRE_DEFAULT_VAL;		
 	#endif	 
	use_ctl.vtretemp = tre_table[use_ctl.tre];
   user_define_eq_update(EQ_POS_TRE,use_ctl.vtretemp);			
 #endif 
 #ifdef BASS_DEFAULT_VAL
 	#ifdef SUPPORT_VM_BASS
		 u8 bBassVal;
	        ret = syscfg_read(VM_BASS, &bBassVal, 1);		
     //     printf("\n --33------------ ret=%d\n",ret);
			
	        if ((bBassVal > TRE_MAX)||(bBassVal < 0)||(ret ==4)) {
	        	    bBassVal = BASS_DEFAULT_VAL; 
	        	}
 		use_ctl.bass= bBassVal;			
	#else
 		use_ctl.bass= BASS_DEFAULT_VAL;
 	#endif	 
 	use_ctl.vbasstemp = bass_table[use_ctl.bass];
	   user_define_eq_update(EQ_POS_BASS,use_ctl.vbasstemp);			
 #endif
  
   #ifndef MICVOL_IO_AD_EN
    	#ifdef SUPPORT_VM_MICVOL
		 u8 bMicvolVal;
	        ret = syscfg_read(VM_MICVOL, &bMicvolVal, 1);	
			
	        if ((bMicvolVal > MICVOL_MAX)||(bMicvolVal <= 0)||(ret ==4)) {
	        	    bMicvolVal = MIC_VOL_DEFAULT_VAL; 
	        	}
 		use_ctl.mic_vol= bMicvolVal;			
	#else
		  use_ctl.mic_vol = MIC_VOL_DEFAULT_VAL;
 	#endif   
   #endif
   
  #ifndef ECHO_IO_AD_EN
  use_ctl.echo= MIC_ECHO_DEFAULT_VAL;
  #endif

  #ifdef  SUPPORT_MIC_BASS_TRE_SET 

  #ifndef MIC_TRE_IO_AD_EN
  #ifdef SUPPORT_VM_MICTRE
 	 u8 bMicTreVal;
         ret = syscfg_read(VM_MIC_TRE, &bMicTreVal, 1);		
    //      printf("\n --66------------ ret=%d\n",ret);
		 
         if ((bMicTreVal > MIC_TRE_MAX)||(bMicTreVal < 0)||(ret ==4)) {
         	    bMicTreVal = MIC_TRE_DEFAULT_VAL; 
         	}
 		use_ctl.mic_tre= bMicTreVal;			
 
  #else
 	  use_ctl.mic_tre= MIC_TRE_DEFAULT_VAL;
  #endif	
	use_ctl.vmictretemp= mic_tre_table[use_ctl.mic_tre];
	
  #endif
  #ifndef MIC_BASS_IO_AD_EN
  #ifdef SUPPORT_VM_MICBASS	 
 	 u8 bMicBassVal;
         ret = syscfg_read(VM_MIC_BASS, &bMicBassVal, 1);	
     //     printf("\n --77------------ ret=%d\n",ret);
		 
         if ((bMicBassVal > MIC_TRE_MAX)||(bMicBassVal < 0)||(ret ==4)) {
         	    bMicBassVal = MIC_BASS_DEFAULT_VAL; 
         	}
 		use_ctl.mic_bass= bMicBassVal;			
 
 #else
	use_ctl.mic_bass= MIC_BASS_DEFAULT_VAL;
  #endif	
	use_ctl.vmicbasstemp= mic_bass_table[use_ctl.mic_bass];	
  #endif  
#endif

  #ifndef MIC_DELAY_IO_AD_EN
  #ifdef SUPPORT_VM_MICDELAY
 	 u8 bMicDelayVal;
         ret = syscfg_read(VM_MIC_DELAY, &bMicDelayVal, 1);	
		 
         if ((bMicDelayVal > MIC_DELAY_MAX)||(bMicDelayVal < 0)||(ret ==4)) {
         	    bMicDelayVal = MIC_DELAY_DEFAULT_VAL; 
         	}
 		use_ctl.micdelay= bMicDelayVal;			
 #elif defined MIC_DELAY_DEFAULT_VAL
 	use_ctl.micdelay= MIC_DELAY_DEFAULT_VAL;
 #else
	use_ctl.micdelay= 0;
  #endif	
  #endif


#ifdef SUPPORT_PONTEN_EQ1_EQ5
	EQ1_AD_IO_INIT();
	adc_add_sample_ch(AD_CH_EQ1); 
	EQ2_AD_IO_INIT();
	adc_add_sample_ch(AD_CH_EQ2); 
	EQ3_AD_IO_INIT();
	adc_add_sample_ch(AD_CH_EQ3); 
	EQ4_AD_IO_INIT();
	adc_add_sample_ch(AD_CH_EQ4); 
	EQ5_AD_IO_INIT();		
	adc_add_sample_ch(AD_CH_EQ5); 
#endif



#ifdef SUPPORT_VOICE_EN_OFF
	use_ctl.bVoiceFlag=0;
#endif

#ifdef SUPPORT_VM_VOICE_ONOFF
 	 u8 bVoiceFlagVal;

         ret = syscfg_read(VM_VOICE_ONOFF, &bVoiceFlagVal, 1);				
         if ((bVoiceFlagVal > 1)||(bVoiceFlagVal < 0)) {
         	    bVoiceFlagVal = 0; 
         	}
 		use_ctl.bVoiceFlag= bVoiceFlagVal;		
#endif	


#ifdef SUPPORT_TONE_VOL_CONTROL
	use_ctl.bToneVol= LED_TONE_VOL_DETAULT_VAL;
#endif

#ifdef SUPPORT_CD4051_PONTEN
	CD4051_IO_AD_INIT();
	adc_add_sample_ch(CD4051_AD_CH);
	use_ctl.bEq1Val=6;
	use_ctl.bEq2Val=6;
	use_ctl.bEq3Val=6;
	use_ctl.bEq4Val=6;
	use_ctl.bEq5Val=6;
#endif

#if defined SUPPORT_ENCODER|| defined SUPPORT_MIC_NOISE_BUG
#ifdef SUPPORT_ENCODER_USE_TIMER0_SET
   //idle_timer2_init();
   //user_timer0_init();
#else
	usr_timer_add(NULL,ENC_Encoder,1,1);
#ifdef SUPPORT_DOUBLE_ENCODER
  usr_timer_add(NULL,ENC_Encoder_2,1,1);
#endif
#endif
#endif

  //---------------------------------------------------

}
/*-----------------------------------------------------
		customer add function
-----------------------------------------------------*/

#endif



