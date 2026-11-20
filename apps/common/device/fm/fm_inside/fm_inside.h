#ifndef _FM_INSIDE_H_
#define _FM_INSIDE_H_

#if(TCFG_FM_INSIDE_ENABLE == ENABLE)

#if DAC2IIS_EN
#define FM_DAC_OUT_SAMPLERATE  44100L
#else
#define FM_DAC_OUT_SAMPLERATE  44100L
#endif

/************************************************************
*                           FM调试说明
*真台少：                     假台多： 				      叠台多：
*减小  FMSCAN_CNR(主)         加大  FMSCAN_CNR(主)        减小：FMSCAN_AGC
*减小  FMSCAN_P_DIFFER(主)    加大  FMSCAN_P_DIFFER(主)
*加大  FMSCAN_N_DIFFER(次)    减小  FMSCAN_N_DIFFER(次)
*
*注意：不要插串口测试搜台数
*************************************************************/

#ifdef SUPPORT_FM_DATA_AMP  // DATA AMP 2-6
#define FMSCAN_SEEK_CNT_MIN  100 //最小过零点数 400左右
#define FMSCAN_SEEK_CNT_MAX  800 //最大过零点数 600左右
#define FMSCAN_960_CNR       30 // 34  //谐波96M的基础cnr 30~40
#define FMSCAN_1080_CNR      40  //谐波108M的基础cnr 30~40
#define FMSCAN_AGC 			 -55  //AGC阈值  -55左右,默认最大增益
#define FMSCAN_ADD_DIFFER 	 -67 //低于此值增加noise differ, -67左右

#define FMSCAN_CNR          		 1   //cnr  1以上
#define FMSCAN_P_DIFFER		 1   //power differ  1以上
#define FMSCAN_N_DIFFER   	       15   //noise differ  8左右

#define FM_IF                1   //0,1.875; 1,2.143; 2,1.5; 3,cnr低的中频听台

#elif defined  SUPPORT_FM_DATA_AMP_MODE_1 //8-12

#define FMSCAN_SEEK_CNT_MIN  100 //最小过零点数 400左右
#define FMSCAN_SEEK_CNT_MAX  800 //最大过零点数 600左右
#define FMSCAN_960_CNR       10 // 34  //谐波96M的基础cnr 30~40
#define FMSCAN_1080_CNR      60//谐波108M的基础cnr 30~40
#define FMSCAN_AGC 			 -1  //AGC阈值  -55左右,默认最大增益
#define FMSCAN_ADD_DIFFER 	 -10 //低于此值增加noise differ, -67左右

#define FMSCAN_CNR          1   //cnr  1以上
#define FMSCAN_P_DIFFER		1   //power differ  1以上
#define FMSCAN_N_DIFFER   	15   //noise differ  8左右

#define FM_IF                3 //0,1.875; 1,2.143; 2,1.5; 3,cnr低的中频听台

#elif defined  SUPPORT_FM_DATA_AMP_MODE_JL_20220519 // 更新cpu.a  修改参数

//过零点范围扩大，尽可能收到台，后期根据需求缩小过滤假台，可以使用建议值 400-600
#define FMSCAN_SEEK_CNT_MIN 100 //最小过零点数 400 左右
#define FMSCAN_SEEK_CNT_MAX 800 //最大过零点数 600 左右

//960 和 1080 两个特殊台，也根据需求修改，可以使用建议值
#define FMSCAN_960_CNR 0 // 34 //谐波 96M 的基础 cnr 30~40
#define FMSCAN_1080_CNR 0//谐波 108M 的基础 cnr 30~40

//AGC 参数可以使用-1，保证增益最大
#define FMSCAN_AGC -1 //AGC 阈值 -55 左右,默认最大增益

//修改库，不使用该参数判断是否使用 noise_differ
#define FMSCAN_ADD_DIFFER -99 //低于此值增加 noise differ, -67 左右

//cnr 应保证 1 以上，后期根据需求过滤假台
#ifdef FMSCAN_set_CNR
#define FMSCAN_CNR FMSCAN_set_CNR //cnr 1 以上
#else
#define FMSCAN_CNR 1 //cnr 1 以上
#endif

//修改库，不使用这两个参数
#define FMSCAN_P_DIFFER 0 //power differ 1 以上
#define FMSCAN_N_DIFFER 50 //noise differ 8 左右

//建议使用 2 或者 3，目前测试使用 2 效果较好，3 没有详细测试，后期可以试试
#define FM_IF 2 //0,1.875; 1,2.143; 2,1.5; 3,cnr 低的中频听台


#elif defined  SUPPORT_FM_DATA_AMP_MODE_MAX

#define FMSCAN_SEEK_CNT_MIN  100 //最小过零点数 400左右
#define FMSCAN_SEEK_CNT_MAX  800 //最大过零点数 600左右
#define FMSCAN_960_CNR       0 // 34  //谐波96M的基础cnr 30~40
#define FMSCAN_1080_CNR      0//谐波108M的基础cnr 30~40
#define FMSCAN_AGC 			 -1  //AGC阈值  -55左右,默认最大增益
#define FMSCAN_ADD_DIFFER 	 -99 //低于此值增加noise differ, -67左右

#define FMSCAN_CNR          1   //cnr  1以上
#define FMSCAN_P_DIFFER		0   //power differ  1以上
#define FMSCAN_N_DIFFER   	50   //noise differ  8左右

#define FM_IF                3 //0,1.875; 1,2.143; 2,1.5; 3,cnr低的中频听台

#else   



#define FMSCAN_SEEK_CNT_MIN  400 //最小过零点数 400左右
#define FMSCAN_SEEK_CNT_MAX  600 //最大过零点数 600左右
#define FMSCAN_960_CNR       34  //谐波96M的基础cnr 30~40
#define FMSCAN_1080_CNR      34  //谐波108M的基础cnr 30~40
#define FMSCAN_AGC 			 -1  //AGC阈值  -55左右,默认最大增益
#define FMSCAN_ADD_DIFFER 	 -67 //低于此值增加noise differ, -67左右

#define FMSCAN_CNR           2   //cnr  1以上
#define FMSCAN_P_DIFFER		 2   //power differ  1以上
#define FMSCAN_N_DIFFER   	 8   //noise differ  8左右

#define FM_IF                3   //0,1.875; 1,2.143; 2,1.5; 3,cnr低的中频听台

#endif


void fm_inside_init(void *priv);
bool fm_inside_set_fre(void *priv, u16 fre);
bool fm_inside_read_id(void *priv);
void fm_inside_powerdown(void *priv);
void fm_inside_mute(void *priv, u8 flag);

#endif
#endif // _FM_INSIDE_H_

