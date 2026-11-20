#include "key_event_deal.h"
#include "key_driver.h"
#include "app_config.h"
#include "board_config.h"
#include "app_task.h"

#ifdef CONFIG_BOARD_AC695X_DEMO
//  SUPPORT_KPH_IR_FUNCTION
#ifdef IRKEY_GW_00FF_807F_21KEY
#include "IRKEY_GW_00FF_807F_21KEY.h"
#elif defined IRKEY_FY_FFFD_33KEY
#include "IRKEY_FY_FFFD_33KEY.h"
#elif defined IRKEY_YS_00FF_807F_21KEY
#include "IRKEY_YS_00FF_807F_21KEY.h"
#elif defined IRKEY_RT_00FF_MUTE_807F_KEY
#include "IRKEY_RT_00FF_MUTE_807F_KEY.h"
#elif defined IRKEY_YH_7F00_7F80_KEY
#include "IRKEY_YH_7F00_7F80_KEY.h"
#elif defined IRKEY_JY_00BF
#include "IRKEY_JY_00BF.h"
#elif defined IRKEY_JY_00FF
#include "IRKEY_JY_00FF.h"
#elif defined IRKEY_JY_00FF_MENU
#include "IRKEY_JY_00FF_MENU.h"
#elif defined IRKEY_JY_00FF_807F_MENU
#include "IRKEY_JY_00FF_807F_MENU.h"

#elif defined IRKEY_KM_00FF
#include "IRKEY_KM_00FF_KEY_TABLE.h"

#elif 0
/***********************************************************
 *				bt 模式的 irkey table
 ***********************************************************/
#if TCFG_APP_BT_EN
const u16 bt_key_ir_table[KEY_IR_NUM_MAX][KEY_EVENT_MAX] = {
    [0] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [1] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [2] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [3] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [4] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [5] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [6] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [7] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [8] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [9] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [10] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [11] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [12] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [13] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [14] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [15] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [16] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [17] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [18] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [19] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [20] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
};
#endif

/***********************************************************
 *				fm 模式的 irkey table
 ***********************************************************/
#if TCFG_APP_FM_EN
const u16 fm_key_ir_table[KEY_IR_NUM_MAX][KEY_EVENT_MAX] = {
    [0] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [1] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [2] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [3] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [4] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [5] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [6] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [7] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [8] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [9] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [10] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [11] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [12] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [13] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [14] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [15] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [16] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [17] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [18] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [19] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [20] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
};
#endif

/***********************************************************
 *				linein 模式的 irkey table
 ***********************************************************/
#if TCFG_APP_LINEIN_EN
const u16 linein_key_ir_table[KEY_IR_NUM_MAX][KEY_EVENT_MAX] = {
    [0] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [1] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [2] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [3] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [4] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [5] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [6] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [7] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [8] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [9] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [10] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [11] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [12] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [13] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [14] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [15] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [16] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [17] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [18] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [19] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [20] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
};
#endif

/***********************************************************
 *				music 模式的 irkey table
 ***********************************************************/
#if TCFG_APP_MUSIC_EN
const u16 music_key_ir_table[KEY_IR_NUM_MAX][KEY_EVENT_MAX] = {
    [0] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [1] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [2] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [3] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [4] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [5] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [6] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [7] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [8] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [9] = {
        KEY_IR_NUM_0,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [10] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [11] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [12] = {
        KEY_IR_NUM_1,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [13] = {
        KEY_IR_NUM_2,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [14] = {
        KEY_IR_NUM_3,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [15] = {
        KEY_IR_NUM_4,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [16] = {
        KEY_IR_NUM_5,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [17] = {
        KEY_IR_NUM_6,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [18] = {
        KEY_IR_NUM_7,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [19] = {
        KEY_IR_NUM_8,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [20] = {
        KEY_IR_NUM_9,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
};
#endif

/***********************************************************
 *				pc 模式的 irkey table
 ***********************************************************/
#if TCFG_APP_PC_EN
const u16 pc_key_ir_table[KEY_IR_NUM_MAX][KEY_EVENT_MAX] = {
    [0] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [1] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [2] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [3] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [4] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [5] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [6] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [7] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [8] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [9] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [10] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [11] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [12] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [13] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [14] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [15] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [16] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [17] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [18] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [19] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [20] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
};
#endif

/***********************************************************
 *				record 模式的 irkey table
 ***********************************************************/
#if TCFG_APP_RECORD_EN
const u16 record_key_ir_table[KEY_IR_NUM_MAX][KEY_EVENT_MAX] = {
    [0] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [1] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [2] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [3] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [4] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [5] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [6] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [7] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [8] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [9] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [10] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [11] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [12] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [13] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [14] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [15] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [16] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [17] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [18] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [19] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [20] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
};
#endif

/***********************************************************
 *				rtc 模式的 irkey table
 ***********************************************************/
#if TCFG_APP_RTC_EN
const u16 rtc_key_ir_table[KEY_IR_NUM_MAX][KEY_EVENT_MAX] = {
    [0] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [1] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [2] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [3] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [4] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [5] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [6] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [7] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [8] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [9] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [10] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [11] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [12] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [13] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [14] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [15] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [16] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [17] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [18] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [19] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [20] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
};
#endif

/***********************************************************
 *				spdif 模式的 irkey table
 ***********************************************************/
#if TCFG_APP_SPDIF_EN
const u16 spdif_key_ir_table[KEY_IR_NUM_MAX][KEY_EVENT_MAX] = {
    [0] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [1] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [2] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [3] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [4] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [5] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [6] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [7] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [8] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [9] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [10] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [11] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [12] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [13] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [14] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [15] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [16] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [17] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [18] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [19] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [20] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
};
#endif

/***********************************************************
 *				idle 模式的 irkey table
 ***********************************************************/
const u16 idle_key_ir_table[KEY_IR_NUM_MAX][KEY_EVENT_MAX] = {
    [0] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [1] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [2] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [3] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [4] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [5] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [6] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [7] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [8] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [9] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [10] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [11] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [12] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [13] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [14] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [15] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [16] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [17] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [18] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [19] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [20] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
};
#endif

#endif

