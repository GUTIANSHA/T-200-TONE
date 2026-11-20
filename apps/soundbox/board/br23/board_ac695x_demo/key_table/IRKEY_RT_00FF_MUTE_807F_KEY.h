


#ifdef IRKEY_RT_00FF_MUTE_807F_KEY


/***********************************************************
 *				bt 模式的 irkey table
 ***********************************************************/
#if TCFG_APP_BT_EN
const u16 bt_key_ir_table[KEY_IR_NUM_MAX][KEY_EVENT_MAX] = {
    [0] = {
        KEY_STANBY,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [1] = {
        KEY_CHANGE_MODE,		KEY_VOICE_ONOFF,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [2] = {
        KEY_MUTE,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [3] = {
        KEY_MUSIC_PP,		KEY_RGB_MODE,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [4] = {
        KEY_MUSIC_PREV,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [5] = {
        KEY_MUSIC_NEXT,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [6] = {
        KEY_EQ_MODE,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [7] = {
        KEY_VOL_DOWN,		KEY_VOL_DOWN,		KEY_VOL_DOWN,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [8] = {
        KEY_VOL_UP,		KEY_VOL_UP,		KEY_VOL_UP,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [9] = {
        KEY_IR_NUM_0,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [10] = {
        KEY_NULL,		KEY_MIC_ONOFF,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
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
    [21] = {
        KEY_REC_MODE,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [22] = {
        KEY_COME_TO_PLAY_REC_FILE,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [23] = {
        KEY_AUX_MODE,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [24] = {
        KEY_FM_MODE,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [25] = {
        KEY_MUSIC_PREV,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [26] = {
        KEY_MUSIC_NEXT,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [27] = {
        KEY_MUSIC_PP,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [28] = {
        KEY_LED_ONOFF,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [29] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [30] = {
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
        KEY_STANBY,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [1] = {
        KEY_CHANGE_MODE,		KEY_VOICE_ONOFF,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [2] = {
        KEY_MUTE,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [3] = {
        KEY_FM_SCAN_ALL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [4] = {
        KEY_FM_PREV_STATION,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [5] = {
        KEY_FM_NEXT_STATION,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [6] = {
        KEY_EQ_MODE,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [7] = {
        KEY_VOL_DOWN,		KEY_VOL_DOWN,		KEY_VOL_DOWN,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [8] = {
        KEY_VOL_UP,		KEY_VOL_UP,		KEY_VOL_UP,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [9] = {
        KEY_IR_NUM_0,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [10] = {
        KEY_NULL,		KEY_MIC_ONOFF,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [11] = {
        KEY_FM_SCAN_ALL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
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
    [21] = {
        KEY_REC_MODE,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [22] = {
        KEY_COME_TO_PLAY_REC_FILE,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [23] = {
        KEY_AUX_MODE,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [24] = {
        KEY_FM_MODE,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [25] = {
        KEY_FM_SCAN_ALL_DOWN,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [26] = {
        KEY_FM_SCAN_UP,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [27] = {
        KEY_MUSIC_PP,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [28] = {
        KEY_LED_ONOFF,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [29] = {
        KEY_FM_PREV_STATION,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [30] = {
        KEY_FM_NEXT_STATION,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },

};
#endif

/***********************************************************
 *				linein 模式的 irkey table
 ***********************************************************/
#if TCFG_APP_LINEIN_EN
const u16 linein_key_ir_table[KEY_IR_NUM_MAX][KEY_EVENT_MAX] = {
    [0] = {
        KEY_STANBY,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [1] = {
        KEY_CHANGE_MODE,		KEY_VOICE_ONOFF,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [2] = {
        KEY_MUTE,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
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
        KEY_EQ_MODE,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [7] = {
        KEY_VOL_DOWN,		KEY_VOL_DOWN,		KEY_VOL_DOWN,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [8] = {
        KEY_VOL_UP,		KEY_VOL_UP,		KEY_VOL_UP,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [9] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [10] = {
        KEY_NULL,		KEY_MIC_ONOFF,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
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
    [21] = {
        KEY_REC_MODE,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [22] = {
        KEY_COME_TO_PLAY_REC_FILE,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [23] = {
        KEY_AUX_MODE,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [24] = {
        KEY_FM_MODE,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [25] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [26] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [27] = {
        KEY_MUSIC_PP,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [28] = {
        KEY_LED_ONOFF,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
     [29] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [30] = {
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
        KEY_STANBY,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [1] = {
        KEY_CHANGE_MODE,		KEY_VOICE_ONOFF,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [2] = {
        KEY_MUTE,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [3] = {
        KEY_MUSIC_PP,		KEY_RGB_MODE,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [4] = {
        KEY_MUSIC_PREV,		KEY_MUSIC_FR,		KEY_MUSIC_FR,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [5] = {
        KEY_MUSIC_NEXT,		KEY_MUSIC_FF,		KEY_MUSIC_FF,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [6] = {
        KEY_EQ_MODE,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [7] = {
        KEY_VOL_DOWN,		KEY_VOL_DOWN,		KEY_VOL_DOWN,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [8] = {
        KEY_VOL_UP,		KEY_VOL_UP,		KEY_VOL_UP,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [9] = {
        KEY_IR_NUM_0,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [10] = {
        KEY_MUSIC_CHANGE_REPEAT,		KEY_MIC_ONOFF,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [11] = {
        KEY_MUSIC_CHANGE_DEV,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
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
    [21] = {
        KEY_REC_MODE,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [22] = {
        KEY_MUSIC_PLAYE_REC_FILE,		KEY_MUSIC_DELETE_FILE_ONLY,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [23] = {
        KEY_AUX_MODE,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [24] = {
        KEY_FM_MODE,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [25] = {
        KEY_MUSIC_PREV,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [26] = {
        KEY_MUSIC_NEXT,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [27] = {
        KEY_MUSIC_PP,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [28] = {
        KEY_LED_ONOFF,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
     [29] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [30] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
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
    [21] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [22] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [23] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [24] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [25] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [26] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [27] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [28] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
     [29] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [30] = {
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
        KEY_MUSIC_MODE,	KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
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
    [21] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [22] = {
        KEY_REC_STOP,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [23] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [24] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [25] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [26] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [27] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [28] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
     [29] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [30] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },

};
#endif

/***********************************************************
 *				rtc 模式的 irkey table
 ***********************************************************/
#if TCFG_APP_RTC_EN
const u16 rtc_key_ir_table[KEY_IR_NUM_MAX][KEY_EVENT_MAX] = {
 
};
#endif

/***********************************************************
 *				spdif 模式的 irkey table
 ***********************************************************/
#if TCFG_APP_SPDIF_EN
const u16 spdif_key_ir_table[KEY_IR_NUM_MAX][KEY_EVENT_MAX] = {

};
#endif

/***********************************************************
 *				idle 模式的 irkey table
 ***********************************************************/
const u16 idle_key_ir_table[KEY_IR_NUM_MAX][KEY_EVENT_MAX] = {
    [0] = {
        KEY_STANBY,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
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
    [21] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [22] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [23] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [24] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [25] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [26] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
     [27] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [28] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
     [29] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    [30] = {
        KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL,		KEY_NULL
    },
    
};

#endif



