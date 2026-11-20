
#include "system/includes.h"
#include "media/includes.h"
#include "media/effects_adj.h"
#include "app_config.h"
#include "app_online_cfg.h"
#include "online_db/online_db_deal.h"
#include "media/audio_eq_drc_apply.h"
#include "config/config_interface.h"

#include "user_cfg.h"
#include "audio_effect/audio_eff_default_parm.h"

//eq_cfg_hw.bin中播歌eq曲线，当作用户自定义模式，参与效果切换.
#define EQ_FILE_CP_TO_CUSTOM  1

#if (TCFG_EQ_ENABLE != 0)

      struct eq_seg_info eq_tab_normal[] = {
    {0, EQ_IIR_TYPE_BAND_PASS, 31,    0, 0.7f},
    {1, EQ_IIR_TYPE_BAND_PASS, 62,    0, 0.7f},
    {2, EQ_IIR_TYPE_BAND_PASS, 125,   0, 0.7f},
    {3, EQ_IIR_TYPE_BAND_PASS, 250,   0, 0.7f},
    {4, EQ_IIR_TYPE_BAND_PASS, 500,   0, 0.7f},
    {5, EQ_IIR_TYPE_BAND_PASS, 1000,  0, 0.7f},
    {6, EQ_IIR_TYPE_BAND_PASS, 2000,  0, 0.7f},
    {7, EQ_IIR_TYPE_BAND_PASS, 4000,  0, 0.7f},
    {8, EQ_IIR_TYPE_BAND_PASS, 8000,  0, 0.7f},
    {9, EQ_IIR_TYPE_BAND_PASS, 16000, 0, 0.7f},
};

const struct eq_seg_info eq_tab_rock[] = {
    {0, EQ_IIR_TYPE_BAND_PASS, 31,		1, 0.7f},
    {1, EQ_IIR_TYPE_BAND_PASS, 62,		0, 0.7f},
    {2, EQ_IIR_TYPE_BAND_PASS, 125,		2, 0.7f},
    {3, EQ_IIR_TYPE_BAND_PASS, 250,		3, 0.7f},
    {4, EQ_IIR_TYPE_BAND_PASS, 500,		1, 0.7f},
    {5, EQ_IIR_TYPE_BAND_PASS, 1000,	0, 0.7f},
    {6, EQ_IIR_TYPE_BAND_PASS, 2000,	0, 0.7f},
    {7, EQ_IIR_TYPE_BAND_PASS, 4000,	0, 0.7f},
    {8, EQ_IIR_TYPE_BAND_PASS, 8000,	-2, 0.7f},
    {9, EQ_IIR_TYPE_BAND_PASS, 16000,	-2, 0.7f},
};

const struct eq_seg_info eq_tab_pop[] = {
    {0, EQ_IIR_TYPE_BAND_PASS, 31,		-2, 0.7f},
    {1, EQ_IIR_TYPE_BAND_PASS, 62,		-2, 0.7f},
    {2, EQ_IIR_TYPE_BAND_PASS, 125,		-2, 0.7f},
    {3, EQ_IIR_TYPE_BAND_PASS, 250,		-1, 0.7f},
    {4, EQ_IIR_TYPE_BAND_PASS, 500,		1, 0.7f},
    {5, EQ_IIR_TYPE_BAND_PASS, 1000,	2, 0.7f},
    {6, EQ_IIR_TYPE_BAND_PASS, 2000,	1, 0.7f},
    {7, EQ_IIR_TYPE_BAND_PASS, 4000,	2, 0.7f},
    {8, EQ_IIR_TYPE_BAND_PASS, 8000,	1, 0.7f},
    {9, EQ_IIR_TYPE_BAND_PASS, 16000,	-1, 0.7f},
};

const struct eq_seg_info eq_tab_classic[] = {
    {0, EQ_IIR_TYPE_BAND_PASS, 31,     0, 0.7f},
    {1, EQ_IIR_TYPE_BAND_PASS, 62,     1, 0.7f},
    {2, EQ_IIR_TYPE_BAND_PASS, 125,    1, 0.7f},
    {3, EQ_IIR_TYPE_BAND_PASS, 250,    1, 0.7f},
    {4, EQ_IIR_TYPE_BAND_PASS, 500,    -1, 0.7f},
    {5, EQ_IIR_TYPE_BAND_PASS, 1000,   -1, 0.7f},
    {6, EQ_IIR_TYPE_BAND_PASS, 2000,   -1, 0.7f},
    {7, EQ_IIR_TYPE_BAND_PASS, 4000,   1, 0.7f},
    {8, EQ_IIR_TYPE_BAND_PASS, 8000,   1, 0.7f},
    {9, EQ_IIR_TYPE_BAND_PASS, 16000,  -1, 0.7f},

};

const struct eq_seg_info eq_tab_country[] = {
    {0, EQ_IIR_TYPE_BAND_PASS, 31,    		-3, 0.7f},
    {1, EQ_IIR_TYPE_BAND_PASS, 62,     1, 0.7f},
    {2, EQ_IIR_TYPE_BAND_PASS, 125,    1, 0.7f},
    {3, EQ_IIR_TYPE_BAND_PASS, 250,    		-3, 0.7f},
    {4, EQ_IIR_TYPE_BAND_PASS, 500,    		-3, 0.7f},
    {5, EQ_IIR_TYPE_BAND_PASS, 1000,   		-3, 0.7f},
    {6, EQ_IIR_TYPE_BAND_PASS, 2000,   		1, 0.7f},
    {7, EQ_IIR_TYPE_BAND_PASS, 4000,   		-3, 0.7f},
    {8, EQ_IIR_TYPE_BAND_PASS, 8000,   		2, 0.7f},
    {9, EQ_IIR_TYPE_BAND_PASS, 16000,  		2, 0.7f},
};

const struct eq_seg_info eq_tab_jazz[] = {
    {0, EQ_IIR_TYPE_BAND_PASS, 31,    		-2, 0.7f},
    {1, EQ_IIR_TYPE_BAND_PASS, 62,     		2, 0.7f},
    {2, EQ_IIR_TYPE_BAND_PASS, 125,    		-2, 0.7f},
    {3, EQ_IIR_TYPE_BAND_PASS, 250,    		2, 0.7f},
    {4, EQ_IIR_TYPE_BAND_PASS, 500,    		-2, 0.7f},
    {5, EQ_IIR_TYPE_BAND_PASS, 1000,   		2, 0.7f},
    {6, EQ_IIR_TYPE_BAND_PASS, 2000,   		-2, 0.7f},
    {7, EQ_IIR_TYPE_BAND_PASS, 4000,   		-1, 0.7f},
    {8, EQ_IIR_TYPE_BAND_PASS, 8000,   		2, 0.7f},
    {9, EQ_IIR_TYPE_BAND_PASS, 16000,  		-4, 0.7f},
};
struct eq_seg_info eq_tab_custom[] = {
    {0, EQ_IIR_TYPE_BAND_PASS, 31,    0, 0.7f},
    {1, EQ_IIR_TYPE_BAND_PASS, 62,    0, 0.7f},
    {2, EQ_IIR_TYPE_BAND_PASS, 125,   0, 0.7f},
    {3, EQ_IIR_TYPE_BAND_PASS, 250,   0, 0.7f},
    {4, EQ_IIR_TYPE_BAND_PASS, 500,   0, 0.7f},
    {5, EQ_IIR_TYPE_BAND_PASS, 1000,  0, 0.7f},
    {6, EQ_IIR_TYPE_BAND_PASS, 2000,  0, 0.7f},
    {7, EQ_IIR_TYPE_BAND_PASS, 4000,  0, 0.7f},
    {8, EQ_IIR_TYPE_BAND_PASS, 8000,  0, 0.7f},
    {9, EQ_IIR_TYPE_BAND_PASS, 16000, 0, 0.7f},
};

// 默认系数表,用户可修改
const struct eq_seg_info *eq_type_tab[EQ_MODE_MAX] = {
    eq_tab_normal, eq_tab_rock, eq_tab_pop, eq_tab_classic, eq_tab_jazz, eq_tab_country, eq_tab_custom
};

// 默认系数表，每个表对应的总增益,用户可修改
float globa_gain_tab[EQ_MODE_MAX] = {0, 0, 0, 0, 0, 0, 0};


/*
 *通话下行eq系数表
 * */

const struct eq_seg_info phone_eq_tab_normal[] = {
    {0, EQ_IIR_TYPE_HIGH_PASS, 200,   0, 0.7f},
    {1, EQ_IIR_TYPE_BAND_PASS, 300,   0, 0.7f},
    {2, EQ_IIR_TYPE_BAND_PASS, 400,   0, 0.7f},
};

/*
 *通话上行eq系数表
 * */
const struct eq_seg_info ul_eq_tab_normal[] = {
    {0, EQ_IIR_TYPE_HIGH_PASS, 200,   0, 0.7f},
    {1, EQ_IIR_TYPE_BAND_PASS, 300,   0, 0.7f},
    {2, EQ_IIR_TYPE_BAND_PASS, 400,   0, 0.7f},
};

#ifdef SUPPORT_VOL_ADJUST_EQ_GAIN
const char  eq_gain[]={-12,-11,-10,-9,-8,-7,-6,-5,-4,-3,-2,-1,0};
void eq_gain_set(s8 val)
{
static int gain;
	gain=val*12/30;

    cur_eq_set_global_gain(AEID_MUSIC_EQ, eq_gain[gain]);
}
#endif


void eq_globa_gain_set(int val)
{
	cur_eq_set_global_gain(AEID_MUSIC_EQ, val);
}

int eq_CUSTOM_globa_gain_get()
{
	return globa_gain_tab[EQ_MODE_CUSTOM];
}


static u8 eq_mode = 0;
//eq效果表切换
int eq_mode_sw(void)
{
#ifdef SUPPORT_HW_EQ_MODE
	use_ctl.eq_mode++;
	#if  EQ_MAX
        if(use_ctl.eq_mode>EQ_MAX)
	#else
        if(use_ctl.eq_mode>5)
	#endif
		use_ctl.eq_mode=0;


	if(use_ctl.eq_mode==0)	
	        eq_mode = EQ_MODE_CUSTOM;	
	else
	        eq_mode = use_ctl.eq_mode;


#else
    eq_mode++;
    if (eq_mode >= ARRAY_SIZE(eq_type_tab)) {
        eq_mode = 0;
    }
#endif	
    struct eq_seg_info *seg = eq_type_tab[eq_mode];

    u8 nsection = ARRAY_SIZE(eq_tab_normal);
    if (nsection > mSECTION_MAX) {
        log_e("ERROR nsection:%d > mSECTION_MAX:%d ", nsection, mSECTION_MAX);
        return -1;//
    }

    printf("=====22==jsuo=====cp_eq_file_seg_to_custom_tab ==\n");
	
    printf("music_mode.eq_parm.global_gain == %d\n",(int)((int)(globa_gain_tab[eq_mode]*100)/100));
	
    /* music_mode.eq_parm.seg_num = nsection; */
    /* music_mode.eq_parm.global_gain = globa_gain_tab[eq_mode]; */
#ifdef SUPPORT_FM_EQ_GAIN
	if (app_get_curr_task()==5)
	{
		cur_eq_set_global_gain(AEID_MUSIC_EQ, 0);
	}
	else
	{
		cur_eq_set_global_gain(AEID_MUSIC_EQ, globa_gain_tab[EQ_MODE_CUSTOM]);
	}
#else
	cur_eq_set_global_gain(AEID_MUSIC_EQ, globa_gain_tab[eq_mode]);
#endif
    for (int i = 0; i < nsection; i++) {
        /* memcpy(&music_mode.eq_parm.seg[i], &seg[i], sizeof(struct eq_seg_info)); */
        cur_eq_set_update(AEID_MUSIC_EQ, &seg[i], nsection, 1);
    }

    return 0;
}
//指定设置某个eq效果表
int eq_mode_set(EQ_MODE mode)
{
	eq_mode = mode;
    if (eq_mode >= ARRAY_SIZE(eq_type_tab)) {
        eq_mode = 0;
    }
    struct eq_seg_info *seg = eq_type_tab[eq_mode];
    u8 nsection = ARRAY_SIZE(eq_tab_normal);
    if (nsection > mSECTION_MAX) {
        log_e("ERROR nsection:%d > mSECTION_MAX:%d ", nsection, mSECTION_MAX);
        return -1;//
    }
	
    /* music_mode.eq_parm.seg_num = nsection; */
    /* music_mode.eq_parm.global_gain = globa_gain_tab[eq_mode]; */
#ifdef SUPPORT_FM_EQ_GAIN
	if (app_get_curr_task()==5)
	{
		cur_eq_set_global_gain(AEID_MUSIC_EQ, 0);
	}
	else
	{
		cur_eq_set_global_gain(AEID_MUSIC_EQ, globa_gain_tab[EQ_MODE_CUSTOM]);
	}
#else
	cur_eq_set_global_gain(AEID_MUSIC_EQ, globa_gain_tab[eq_mode]);
#endif
    for (int i = 0; i < nsection; i++) {
        /* memcpy(&music_mode.eq_parm.seg[i], &seg[i], sizeof(struct eq_seg_info)); */
        cur_eq_set_update(AEID_MUSIC_EQ, &seg[i], nsection, 1);
    }

    return 0;
}
//返回某个eq效果模式标号
EQ_MODE eq_mode_get_cur(void)
{
    return eq_mode;
}

/*----------------------------------------------------------------------------*/
/**@brief    设置用户自定义模式某段eq信息
   @param
   @param
   @return
   @note
*/
/*----------------------------------------------------------------------------*/
int eq_mode_set_custom_seg(struct eq_seg_info *seg)
{
    struct eq_seg_info *tar_seg = eq_tab_custom;
    u8 index = seg->index;
    if (index > ARRAY_SIZE(eq_tab_custom)) {
        log_e("index %d > max_nsection %d", index, ARRAY_SIZE(eq_tab_custom));
        return -1;
    }
    memcpy(&tar_seg[index], seg, sizeof(struct eq_seg_info));
    return 0;
}
/*----------------------------------------------------------------------------*/
/**@brief    获取某个模式eq表内，某一段eq的信息
   @param
   @param
   @return  返回eq信息
   @note
*/
/*----------------------------------------------------------------------------*/
struct eq_seg_info *eq_mode_get_seg(EQ_MODE mode, u8 index)
{
    if (mode >= ARRAY_SIZE(eq_type_tab)) {
        return NULL;
    }
    struct eq_seg_info *seg = eq_type_tab[mode];
    return &seg[index];
}

/*
 *更新某段自定义系数表，某段eq的中心截止频率以及增益
 * */
int eq_mode_set_custom_info(u16 index, int freq, float gain)
{
    struct eq_seg_info *seg = eq_mode_get_seg(EQ_MODE_CUSTOM, index);//获取某段eq系数
    if (!seg) {
        return -1;
    }
    seg->freq = freq;//修改freq gain
    seg->gain = gain;
    eq_mode_set_custom_seg(seg);//重设系数

    eq_mode_set(EQ_MODE_CUSTOM);//设置更新系数
    return 0;
}
void cp_eq_file_seg_to_custom_tab()
{
#if EQ_FILE_CP_TO_CUSTOM
    u8 nsection = music_mode.eq_parm.seg_num;
    struct eq_seg_info *seg = eq_tab_custom;
    if (nsection > ARRAY_SIZE(eq_tab_custom)) {
        log_e("music nsection:%d > custom nsection:%d\n", nsection, ARRAY_SIZE(eq_tab_custom));
        return ;
    }

    printf("=======jsuo=====cp_eq_file_seg_to_custom_tab ==\n");
	
    printf("music_mode.eq_parm.global_gain == %d\n",(int)((int)(music_mode.eq_parm.global_gain*100)/100));
	for (int i = 0;   i < ARRAY_SIZE(globa_gain_tab); ++  i)
	{
		globa_gain_tab[i] = music_mode.eq_parm.global_gain;
	}
    memcpy(seg, music_mode.eq_parm.seg, sizeof(struct eq_seg_info)*nsection);
    memcpy(eq_tab_normal, music_mode.eq_parm.seg, sizeof(struct eq_seg_info)*nsection);

    eq_mode=EQ_MODE_CUSTOM;
#endif
}


int eq_init(void)
{
    audio_eq_init();
    audio_drc_init();
	
    return 0;
}
__initcall(eq_init);


#ifdef SUPPORT_MULT_EQ_FILE

#define EQ_FILE_NAME           SDFILE_RES_ROOT_PATH"eq_cfg_hw1.bin"


void user_eq_change(u8 flag)
{
 //   printf("user_eq_change %d mode == %d\n",flag,eq_mode_get_cur());

        eff_file_switch(EQ_FILE_NAME);	
}

#endif


#endif
