#include "effect_reg.h"
#include "app_config.h"
#include "audio_enc.h"
/**********************混响功能选配*********************************/
#if (TCFG_MIC_EFFECT_ENABLE)

#if 0
#define MIC_EFFECT_CONFIG 	  	  0	\
								| BIT(MIC_EFFECT_CONFIG_REVERB) \
								| BIT(MIC_EFFECT_CONFIG_NOISEGATE) \
								| BIT(MIC_EFFECT_CONFIG_EQ) \
								| BIT(MIC_EFFECT_CONFIG_HOWLING) \
								| BIT(MIC_EFFECT_CONFIG_HOWLING_TRAP) \

							| BIT(MIC_EFFECT_CONFIG_REVERB) \
								| BIT(MIC_EFFECT_CONFIG_PITCH) \
								| BIT(MIC_EFFECT_CONFIG_DVOL) \
								| BIT(MIC_EFFECT_CONFIG_LINEIN) \
								| BIT(MIC_EFFECT_CONFIG_ENERGY_DETECT) \

#elif  TCFG_MIC_VOICE_CHANGER_ENABLE

#define MIC_EFFECT_CONFIG 	  	  0	\
							| BIT(MIC_EFFECT_CONFIG_REVERB) \
								| BIT(MIC_EFFECT_CONFIG_DVOL) \
								| BIT(MIC_EFFECT_CONFIG_LINEIN) \
								| BIT(MIC_EFFECT_CONFIG_ENERGY_DETECT) \
								| BIT(MIC_EFFECT_CONFIG_EQ) \
								| BIT(MIC_EFFECT_CONFIG_PITCH) \
								| BIT(MIC_EFFECT_CONFIG_NOISEGATE) \								

//								| BIT(MIC_EFFECT_CONFIG_HOWLING) \								
								
								
#else
#define MIC_EFFECT_CONFIG 	  	  0	\
							| BIT(MIC_EFFECT_CONFIG_REVERB) \
								| BIT(MIC_EFFECT_CONFIG_DVOL) \
								| BIT(MIC_EFFECT_CONFIG_LINEIN) \
								| BIT(MIC_EFFECT_CONFIG_EQ) \
								| BIT(MIC_EFFECT_CONFIG_ENERGY_DETECT) \
								| BIT(MIC_EFFECT_CONFIG_NOISEGATE) \	
								| BIT(MIC_EFFECT_CONFIG_HOWLING)

#endif
	
/*
								| BIT(MIC_EFFECT_CONFIG_REVERB) \
								| BIT(MIC_EFFECT_CONFIG_NOISEGATE) \
								| BIT(MIC_EFFECT_CONFIG_EQ) \
								| BIT(MIC_EFFECT_CONFIG_HOWLING) \
								| BIT(MIC_EFFECT_CONFIG_HOWLING_TRAP) \
*/
								/* | BIT(MIC_EFFECT_CONFIG_PITCH) \ */
/* | BIT(MIC_EFFECT_CONFIG_ENERGY_DETECT) \ */
/* | BIT(MIC_EFFECT_CONFIG_LINEIN) \ */
/* | BIT(MIC_EFFECT_CONFIG_DVOL) \ */

/*********************************************************************/

const struct __mic_effect_parm effect_parm_default = {
    .effect_config = MIC_EFFECT_CONFIG,///混响通路支持哪些功能
    .effect_run = MIC_EFFECT_CONFIG,///混响打开之时， 默认打开的功能
    .sample_rate = MIC_EFFECT_SAMPLERATE,
};

const struct __mic_stream_parm effect_mic_stream_parm_default = {
#if (TCFG_MIC_EFFECT_SEL == MIC_EFFECT_REVERB)
		.mic_gain			= 0,
#else
		.mic_gain			= USER_MIC_GAIN,//4,
#endif
    .sample_rate 		= MIC_EFFECT_SAMPLERATE,//采样率
#if (RECORDER_MIX_EN)
    .point_unit  		= 160,//一次处理数据的数据单元， 单位点 4对齐(要配合mic起中断点数修改)
    .dac_delay			= 20,//dac硬件混响延时， 单位ms 要大于 point_unit*2
#else
#if 1// (TCFG_MIC_EFFECT_SEL == MIC_EFFECT_REVERB)
    .point_unit  		= REVERB_LADC_IRQ_POINTS,//一次处理数据的数据单元， 单位点 4对齐(要配合mic起中断点数修改)
    .dac_delay			= (int)((REVERB_LADC_IRQ_POINTS * 2) / (TCFG_REVERB_SAMPLERATE_DEFUAL / 1000.0)), //6,//8,//8,//10,//dac硬件混响延时， 单位ms 要大于 point_unit*2 // (REVERB_LADC_IRQ_POINTS*1.5)/(TCFG_REVERB_SAMPLERATE_DEFUAL/1000.0)
#else
//    .point_unit  		= ((MIC_EFFECT_SAMPLERATE / 1000) * 4), //一次处理数据的数据单元， 单位点 4对齐(要配合mic起中断点数修改)
//    .dac_delay			= 10,//dac硬件混响延时， 单位ms 要大于 point_unit*2
#endif
#endif
};

#if TCFG_MIC_VOICE_CHANGER_ENABLE
const EF_REVERB_FIX_PARM effect_echo_fix_parm_default = {
    .sr = MIC_EFFECT_SAMPLERATE,		////采样率
    .max_ms = 220,				////所需要的最大延时，影响 need_buf 大小
};
#else
const EF_REVERB_FIX_PARM effect_echo_fix_parm_default = {
    .sr = MIC_EFFECT_SAMPLERATE,		////采样率
    .max_ms =  200,	// 150			////所需要的最大延时，影响 need_buf 大小

};
#endif

static const u16 r_dvol_table[] = {
    0	, //0
	3000,//1
    3600, //3
    3800, //5
    4000,//7
    4200, //9
    4400,//11
    4500, //12
    4600, //13
    4700,//14
    4800, //15
    5100, //16
    5300, //17
    5500, //18
    5800, //19
    6500, //20
    6713, //21
    7200, //22
    8025, //23
    8700, //24
    8900,//25
    9200,//26
    9592, //25
    9700,
    10200,//26
    10600,
    11000,
    11466,//27
	13000,//28
    15200,//29
    16000,//30
    16384 //31
};

audio_dig_vol_param effect_dvol_default_parm = {
#if (SOUNDCARD_ENABLE)
    .vol_start = 0,//30,
#else
    .vol_start = 30,
#endif
    .vol_max = ARRAY_SIZE(r_dvol_table) - 1,
    .ch_total = 1,
    .fade_en = 1,
    .fade_points_step = 2,
    .fade_gain_step = 2,
    .vol_list = (void *)r_dvol_table,
};



const struct __effect_dodge_parm dodge_parm = {
#ifdef SUPPORT_DODGE_MIC_PR_FY_A12	
    .dodge_in_thread = 4500,//触发闪避的能量阈值
    .dodge_in_time_ms = 20,//能量值持续大于dodge_in_thread 就触发闪避
    .dodge_out_thread = 3000,//退出闪避的能量阈值
    .dodge_out_time_ms = 500,//能量值持续小于dodge_out_thread 就退出闪避
#elif defined SUPPORT_DODGE_MIC_PR_JY_TB_15
    .dodge_in_thread = 1000,//触发闪避的能量阈值
    .dodge_in_time_ms = 10,//能量值持续大于dodge_in_thread 就触发闪避
    .dodge_out_thread = 1000,//退出闪避的能量阈值
    .dodge_out_time_ms = 500,//能量值持续小于dodge_out_thread 就退出闪避
#elif defined SUPPORT_DODGE_MIC_PR_JY_TB_12
    .dodge_in_thread = 800,//触发闪避的能量阈值
    .dodge_in_time_ms = 20,//能量值持续大于dodge_in_thread 就触发闪避
    .dodge_out_thread = 1000,//退出闪避的能量阈值
    .dodge_out_time_ms = 500,//能量值持续小于dodge_out_thread 就退出闪避    
#elif defined SUPPORT_DODGE_MIC_PR_KM_280
    .dodge_in_thread = 800,//触发闪避的能量阈值
    .dodge_in_time_ms = 20,//能量值持续大于dodge_in_thread 就触发闪避
    .dodge_out_thread = 1000,//退出闪避的能量阈值
    .dodge_out_time_ms = 500,//能量值持续小于dodge_out_thread 就退出闪避    
#else
    .dodge_in_thread = 800,//触发闪避的能量阈值
    .dodge_in_time_ms = 1,//能量值持续大于dodge_in_thread 就触发闪避
    .dodge_out_thread = 1000,//退出闪避的能量阈值
    .dodge_out_time_ms = 500,//能量值持续小于dodge_out_thread 就退出闪避
#endif
	
};



#endif//TCFG_MIC_EFFECT_ENABLE


