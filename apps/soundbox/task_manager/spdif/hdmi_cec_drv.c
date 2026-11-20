#include "spdif/hdmi_cec.h"
#include "spdif/hdmi_cec_api.h"
#include "app_config.h"
#include "asm/clock.h"
#include "app_config.h"
#include "gpio.h"
#include "system/timer.h"
#include "user_cfg.h"
#include "app_main.h"
#include "app_task.h"
#include "audio_config.h"
#if ((TCFG_SPDIF_ENABLE) && (TCFG_HDMI_ARC_ENABLE))

//#pragma const_seg(".HDMI_const")
//#pragma code_seg(".HDMI_code")
#if 1//kph_add
#include "key_event_deal.h"
#ifdef SUPPORT_HDMI_VOL_CHANGE
void cee_send_main_vol_status();
#endif
#endif
u32 cec_transmit_frame(unsigned char *buffer, int count);


typedef enum {
    ARC_STATUS_REQ_ADD,
    ARC_STATUS_WAIT_ONLINE,
    ARC_STATUS_REPORT_ADD,
    ARC_STATUS_STB,
    ARC_STATUS_INIT,
    ARC_STATUS_REQ_INIT,
    ARC_STATUS_INIT_SUCCESS,
    ARC_STATUS_TERMINATE,
    ARC_STATUS_REQ_TERMINATE,
    ARC_STATUS_REPORT_TERMINATE,
} ARC_STATUS;

static void cec_set_wait_time(u32 us);

static void cec_set_line_state(const u8 state)
{
    gpio_direction_output(TCFG_HDMI_CEC_PORT, state);
}

static u32 cec_get_line_state()
{
    return gpio_read(TCFG_HDMI_CEC_PORT);
}

static u8 CEC_Version_cmd[3] = {0x00, CEC_VERSION, 0x04}; //0x04 :1.3a ????
static u8 Report_Power_Status_Cmd[3] = {0x00, 0x90, 0x00}; //0x00:ON ; 0x01:OFF
static u8 Standby_Cmd[2] = {0x00, STANDBY};

static u8 PollingMessage_cmd[1] = {0x55};
//0x25

static u8 Report_Physical_Add_Cmd1[7] = {0x5F, REPORT_PHYSICAL_ADDRESS, 0x20, 0x00, 0x05};
static u8 Report_Physical_Add_Cmd2[7] = {0x5F, REPORT_PHYSICAL_ADDRESS, 0x30, 0x00, 0x05};
static u8 Report_Physical_Add_Cmd[7] = {0x5F, REPORT_PHYSICAL_ADDRESS, 0x10, 0x00, 0x05};


static u8 Device_Vendor_Id_Cmd[5] = {0x5f, DEVICE_VENDOR_ID, 0x45, 0x58, 0x50};
static u8 Set_System_Audio_Mode_cmd[] = {0x50, SET_SYSTEM_AUDIO_MODE, 0x01};
static u8 Broadcast_System_Audio_Mode_cmd[] = {0x5F, SET_SYSTEM_AUDIO_MODE, 0x01};


static u8 Set_Osd_Name_cmd[] = {0x50, CECOP_SET_OSD_NAME, 0x41, 0x75, 0x64, 0x69, 0x6f, 0x20, 0x53, 0x79, 0x73, 0x74, 0x65, 0x6d}; //["Audio System"]

//static u8 Set_Osd_Name_cmd[] = {0x50, 0x47, 0x53, 0x6F, 0x75, 0x6E, 0x64, 0x62, 0x61, 0x72, 0x0F, 0x82, 0x00, 0x00}; //["Audio System"]

static u8 System_Audio_Mode_Status_cmd[] = {0x50, CECOP_SYSTEM_AUDIO_MODE_STATUS, 0x01};
//static u8 Report_Audio_Status_cmd[] = {0x50, CECOP_REPORT_AUDIO_STATUS, 0xB2};// bit7 off/on bit6-0 volume
static u8 Report_Audio_Status_cmd[] = {0x50, CECOP_REPORT_AUDIO_STATUS, 0x32};

static u8 Report_Power_Status_cmd[] = {0x50, CECOP_REPORT_POWER_STATUS, 0x00};// 0:on 1 standby 2: S2O 3: o2s



static u8 Terminate_Arc_cmd[2] = {0x50, CECOP_ARC_TERMINATE};
static u8 Initiate_Arc_cmd[2] = {0x50, INITIATE_ARC};
static u8 Report_Short_Audio_Desc_cmd[8] = {0x50, REPORT_SHORT_AUDIO_DESCRIPTOR, 0x15, 0x17, 0x50, 0x3E, 0x06, 0xC0}; //AC-3 6Ch (48 44 32k) 640kbps/ DTS 7Ch (48 44K)  1536kbps

static u8 Give_Tuner_Device_Status_cmd[3] = {0x50, CECOP_GIVE_TUNER_DEVICE_STATUS, 0x01};
static u8 Feature_Abort_cmd[3] = {0x50, CECOP_FEATURE_ABORT, 0x00};

#define     CEC_IDLE    0
#define     CEC_TX      1
#define     CEC_RX      2

static u8 cec_work_mode;
static s8 cec_state;
static u16 high_period_time;
enum {
    CEC_SUCC,
    NAK_HEAD,
    NAK_DATA,
};

struct cec_msg_frame {
    u8 *msg;
    u8 msg_len;
    u8 tx_count;
    u8 ack;
    u8 broadcast;
};

static struct cec_msg_frame cec_frame;

struct hdmi_arc_t {
    u8 msg_count;
    u8 state;
    u8 logic_addr;
    u8 rx_count;
    u8 rx_msg;
    u8 idle_time;
    u16 time_id;
    u8 cec_ok;

    u8 msg[17];

};

static struct hdmi_arc_t arc_devcie;
#define __this  (&arc_devcie)

typedef enum {
    CEC_XMIT_STARTBIT,
    CEC_XMIT_BIT7,
    CEC_XMIT_BIT6,
    CEC_XMIT_BIT5,
    CEC_XMIT_BIT4,
    CEC_XMIT_BIT3,
    CEC_XMIT_BIT2,
    CEC_XMIT_BIT1,
    CEC_XMIT_BIT0,
    CEC_XMIT_BIT_EOM,
    CEC_XMIT_BIT_ACK,
    CEC_XMIT_ACK_PU,
    CEC_XMIT_CHECK_ACK,
    CEC_XMIT_ACK_END,

    CEC_WAIT_IDLE,
    CEC_RECEIVE,
    CEC_CONTINUE_BIT,
} CEC_XMIT_STATE ;

void user_change_report_physical_add_cmd(u8 addr1, u8 addr2)
{
    printf("addr1 == %x  addr2 == %x\n",addr1,addr2);
    if(addr1 == 0x10 || addr1 == 0x20 || addr1 == 0x30){
        printf("user_change_cmd");
        put_buf(Report_Physical_Add_Cmd, sizeof(Report_Physical_Add_Cmd));
        Report_Physical_Add_Cmd[2] = addr1;
        Report_Physical_Add_Cmd[3] = addr2;
        Report_Physical_Add_Cmd1[2] = addr1;
        Report_Physical_Add_Cmd1[3] = addr2;
        Report_Physical_Add_Cmd2[2] = addr1;
        Report_Physical_Add_Cmd2[3] = addr2;
        put_buf(Report_Physical_Add_Cmd, sizeof(Report_Physical_Add_Cmd));
    }
    else{
        Report_Physical_Add_Cmd[2] = 0x10;
        Report_Physical_Add_Cmd[3] = 0x00;
        Report_Physical_Add_Cmd1[2] = 0x20;
        Report_Physical_Add_Cmd1[3] = 0x00;
        Report_Physical_Add_Cmd2[2] = 0x30;
        Report_Physical_Add_Cmd2[3] = 0x00;
    }
}

#ifdef HDMI_CEC_DRV_ADD
u32 cec_transmit_frame(unsigned char *buffer, int count);
static u8 Broadcast_System_Audio_Mode_off[] = {0x5F, CECOP_SET_SYSTEM_AUDIO_MODE, 0x00};
static u8 Broadcast_System_Audio_Mode_on[] = {0x5F, CECOP_SET_SYSTEM_AUDIO_MODE, 0x01};

/**
 * @brief 把电视从ARC模式中释放出来
 */
void arc_send_close_cec_ctl()   
{
    cec_transmit_frame(Terminate_Arc_cmd,2);
    os_time_dly(20);
    cec_transmit_frame(Terminate_Arc_cmd,2);
    os_time_dly(20);
    cec_transmit_frame(Terminate_Arc_cmd,2);
    os_time_dly(20);
    cec_transmit_frame(Broadcast_System_Audio_Mode_off,3);
    os_time_dly(10);
    cec_transmit_frame(Broadcast_System_Audio_Mode_off,3);

}

/**
 * @brief 初始化CEC发送一下命令，防止二次进入没声音
 */
void arc_send_open_cec_ctl()
{
    cec_transmit_frame(Initiate_Arc_cmd,2);
    os_time_dly(20);
    cec_transmit_frame(Initiate_Arc_cmd,2);
    os_time_dly(20);
    cec_transmit_frame(Initiate_Arc_cmd,2);
    os_time_dly(20);
    cec_transmit_frame(Broadcast_System_Audio_Mode_on,3);
    os_time_dly(10);
    cec_transmit_frame(Broadcast_System_Audio_Mode_on,3);
}
#endif

static void cec_start_bit()
{
    cec_state = CEC_XMIT_STARTBIT;
    gpio_direction_output(TCFG_HDMI_CEC_PORT, 0);
    gpio_set_pull_down(TCFG_HDMI_CEC_PORT, 0);

    const u16 low_time = 3700;
    const u16 high_time = 4500 - low_time;
    cec_set_wait_time(low_time);
    high_period_time = high_time;

}
static void cec_logic_bit0()
{
    const u16 low_time = 1500;
    const u16 high_time = 2400 - low_time;
    cec_set_wait_time(low_time);
    cec_set_line_state(0);
    high_period_time = high_time;
}
static void cec_logic_bit1()
{
    const u16 low_time = 600;
    const u16 high_time = 2400 - low_time;
    cec_set_wait_time(low_time);
    cec_set_line_state(0);
    high_period_time = high_time;
}
static void cec_end_bit()
{
    const u16 low_time = 600;
    cec_set_line_state(0);
    cec_set_wait_time(low_time);
}
static void cec_ack_bit()
{
    const u16 low_time = 1500;
    const u16 high_time = 1800 - low_time;
    cec_set_wait_time(low_time);
    cec_set_line_state(0);
    high_period_time = high_time;
}
static void transmit_complete(u32 state)
{
    gpio_set_direction(TCFG_HDMI_CEC_PORT, 1);
    cec_work_mode = CEC_IDLE;
    cec_frame.ack = state;
    cec_state = CEC_RECEIVE;
}
static void tx_bit_isr()
{
    if (high_period_time) {
        cec_set_line_state(1);
        cec_set_wait_time(high_period_time);
        high_period_time = 0;
        cec_state++;
        return;
    }
    switch (cec_state) {
    case CEC_WAIT_IDLE:
        cec_start_bit();
        break;

    case CEC_XMIT_STARTBIT:
        break;

    case CEC_XMIT_ACK_END:
        if (cec_frame.ack) {
            cec_set_line_state(1);
            if (cec_frame.tx_count == cec_frame.msg_len) {
                transmit_complete(CEC_SUCC);
                break;
            } else {
                cec_state = CEC_XMIT_BIT7;
            }
        } else {
            if (cec_frame.tx_count == 1) {
                transmit_complete(NAK_HEAD);
            } else {
                transmit_complete(NAK_DATA);
            }
            break;
        }
    case CEC_XMIT_BIT7 ... CEC_XMIT_BIT0:
        if (cec_frame.msg[cec_frame.tx_count] & BIT(7 - (cec_state - CEC_XMIT_BIT7))) {
            cec_logic_bit1();
        } else {
            cec_logic_bit0();
        }
        break;

    case CEC_XMIT_BIT_EOM:
        cec_frame.tx_count++;
        if (cec_frame.tx_count == cec_frame.msg_len) {
            cec_logic_bit1();
        } else {
            cec_logic_bit0();
        }
        break;

    case CEC_XMIT_BIT_ACK:
        cec_state++;
        cec_end_bit();
        break;

    case CEC_XMIT_ACK_PU:
        gpio_set_direction(TCFG_HDMI_CEC_PORT, 1);
        cec_set_wait_time(1000 - 600);
        cec_state++;
        break;

    case CEC_XMIT_CHECK_ACK:
        cec_frame.ack = cec_frame.broadcast ? 1 : !cec_get_line_state();
        cec_state++;
        cec_set_wait_time(2400 - 1000);
        break;
    }

}

#define     CAP_DIV         64
#define     CAP_CLK         24000000
#define     CEC_TMR         JL_TIMER0
#define     CEC_TMR_IDX     IRQ_TIME0_IDX

#define     CAP_RISING_EDGE    0b10
#define     CAP_FALLING_EDGE   0b11
static void change_capture_mode(u32 mode)
{
    CEC_TMR->PRD = 0;
    CEC_TMR->CNT = 0;
    CEC_TMR->CON = ((0b0011 << 4) | BIT(3));
    CEC_TMR->CON |= mode;
}
static u16 get_captrue_time()
{
    return CEC_TMR->PRD;
}
//New initiator wants to send a frame wait time
#define     _ms *1//000
#define     CEC_NEW_FRAME_WAIT_TIME            	(5*(2.4 _ms))//(5*(2.4 _ms))
#define     CEC_RETRANSMIT_FRAME                (3*(2.4 _ms))

static void cec_set_wait_time(u32 us)
{
    /* us = 3700; */
    CEC_TMR->CON = BIT(14);
    u32 clk = clk_get("timer") / 320000;
    /* printf("clk %d %d %d", clk, us, us / 100 * clk); */
    CEC_TMR->CON = ((0b0110 << 4) | BIT(3));
    CEC_TMR->CNT = 0;
    CEC_TMR->PRD = us / 100 * clk;
    CEC_TMR->CON |= BIT(0);
}
static u32 cec_bit_time_check(u16 t, u16 min, u16 max)
{
    const u16 _min = min * (CAP_CLK / 1000000) / CAP_DIV;
    const u16 _max = max * (CAP_CLK / 1000000) / CAP_DIV;
    if ((t > _min) && (t < _max)) {
        return 1;
    } else {
        return 0;
    }
}
static s8 cec_get_bit_value(const u16 low_time, const u16 high_time)
{
    if (cec_bit_time_check(low_time + high_time, 2050, 2750)) {
        if (cec_bit_time_check(low_time, 400, 800)) {
            return 1;
        } else if (cec_bit_time_check(low_time, 1300, 1700)) {
            return 0;
        }
    }
    return -1;
}

static void cec_cmd_response();
static void rx_bit_isr()
{
    static u16 low_time = 0;
    static u16 high_time = 0;
    static u16 eom = 0;
    static u8 follower = 0;
    u32 mode = CEC_TMR->CON & 0x3;
    CEC_TMR->CON = 0;
    static u8 user_cmd = 0;;
    static u8 user_bit_cnt = 0;

    if (high_period_time) {
        cec_set_line_state(1);
        cec_set_wait_time(high_period_time);
        high_period_time = 0;
        cec_state = CEC_XMIT_ACK_END;
        return;
    }

    if (cec_state == CEC_XMIT_ACK_END) {
        gpio_set_direction(TCFG_HDMI_CEC_PORT, 1);
        __this->rx_count ++;
        if (eom) {
            __this->rx_msg = 1;
            u16 ttmp;
            ttmp = usr_timeout_add(NULL, cec_cmd_response, CEC_NEW_FRAME_WAIT_TIME, 0);
//			printf("****ttmp = 0x%x\n", ttmp);
        } else {
            cec_state = CEC_CONTINUE_BIT;
        }

        change_capture_mode(CAP_FALLING_EDGE);
        return;
    }

    u16 time = get_captrue_time();

    if (mode == CAP_RISING_EDGE) {
        low_time = time;
        time = 0;
        mode = CAP_FALLING_EDGE;
    } else {
        mode = CAP_RISING_EDGE;
    }

    change_capture_mode(mode);

    if (time == 0) {
        return;
    }

    if (cec_state == CEC_RECEIVE) {
        cec_state = CEC_XMIT_STARTBIT;
        low_time = 0;
        high_time = 0;
        return;
    }

    s8 bit;

    high_time = time;

    switch (cec_state) {
    case CEC_XMIT_STARTBIT:
        if (low_time && high_time) {
            if (cec_bit_time_check(low_time, 3500, 3900) &&
                cec_bit_time_check(low_time + high_time, 4300, 4700)) {
                cec_state ++;
            }
        }

        follower = 0;
        __this->rx_count = 0;
        low_time = 0;
        high_time = 0;
        break;

    case CEC_CONTINUE_BIT:
        cec_state = CEC_XMIT_BIT7;
        break;

    case CEC_XMIT_BIT7 ... CEC_XMIT_BIT0:
        bit = cec_get_bit_value(low_time, high_time);
        if (bit == 0) {
            user_cmd &= ~BIT(7-user_bit_cnt);
            __this->msg[__this->rx_count] &= ~BIT(7 - (cec_state - CEC_XMIT_BIT7));
//            cec_state++;
        } else if (bit == 1) {
            user_cmd |= BIT(7-user_bit_cnt);
            __this->msg[__this->rx_count] |= BIT(7 - (cec_state - CEC_XMIT_BIT7));
//            cec_state++;
        } else if (bit == -1) { //error bit
            user_bit_cnt=0;
            user_cmd=0x00;
            cec_state = CEC_XMIT_STARTBIT;
        }
        user_bit_cnt++;
        if (user_bit_cnt==8)
        {
            // if (user_cmd != 0)
            // {
            //     printf("\r\n++++++++++++++++++++++++++++\r\n");
            //     printf("\r\ncmd==%d\r\n",user_cmd);
            // }
            #ifdef SUPPORT_SPDIF_HDMI_MODE
            if (user_cmd == 0x36 && app_check_curr_task(APP_SPDIF_TASK) && use_ctl.bSpdifMode == SPDIF_HDMI)   //根据文档说明，电视待机或关机会发送0x36，尚未验证
            {
                printf("\r\ncmd==%d\r\n",user_cmd);
            //    app_task_switch_to(APP_IDLE_TASK);
                if (__this->state <= ARC_STATUS_INIT) {
                    __this->cec_ok = 0;
                    __this->state = ARC_STATUS_REQ_ADD;
                }
            }
            user_bit_cnt=0;
            user_cmd=0x00;
            #endif
            
        }
        cec_state++;
        break;

    case CEC_XMIT_BIT_EOM:
        cec_state++;
        bit = cec_get_bit_value(low_time, high_time);
        if (bit == 0) {
            eom = 0;
        } else if (bit == 1) {
            eom = 1;
        } else if (bit == -1) { //error bit
            cec_state = CEC_XMIT_STARTBIT;
        }
        if (__this->rx_count == 0) { //head block
            follower = __this->msg[0] & 0xf;
        }

        if (follower == __this->logic_addr) {
            cec_ack_bit();//send ack
        } else {
            /* printf(" %d %d %d", __this->rx_count, follower, __this->logic_addr); */
            __this->rx_count = 0;
        }
        break;

    case CEC_XMIT_BIT_ACK:
        cec_state = CEC_XMIT_BIT7;
        break;
    }

}
#define     TIME_PRD    30
___interrupt
static void cec_timer_isr()
{
    CEC_TMR->CON |= BIT(14);

    if (cec_work_mode == CEC_TX) 
    {
        tx_bit_isr();
        if (cec_work_mode == CEC_IDLE) 
        {
            change_capture_mode(CAP_FALLING_EDGE);
        }
    } 
    else 
    {
        if (__this->idle_time < (150 / TIME_PRD)) 
        {
            __this->idle_time = 150 / TIME_PRD;
        }
        rx_bit_isr();
    }

}


u32 cec_transmit_frame(unsigned char *buffer, int count)
{
    if (cec_work_mode != CEC_IDLE) {
        return 1;
    }
    __this->idle_time = 1;
    cec_work_mode = CEC_TX;
    CEC_TMR->CON = BIT(14);
    cec_frame.ack = -1;
    cec_frame.msg = buffer;
    cec_frame.msg_len = count;
    cec_frame.tx_count = 0;
    cec_frame.broadcast = 0;
    //printf("CEC_SEND_CMD:");
    //put_buf(buffer,count);
    if ((buffer[0] & 0xf) == CEC_LOGADDR_UNREGORBC) {
        cec_frame.broadcast = 1;
    }

    if ((buffer[0] & 0xf) == __this->logic_addr) {
        cec_frame.broadcast = 1;
    }

    gpio_set_direction(TCFG_HDMI_CEC_PORT, 1);
    if (cec_get_line_state()) {
        /* g_printf("start"); */
        cec_start_bit();
    } else {
        /* g_printf("wait idle"); */
        high_period_time = 0;
        cec_state = CEC_WAIT_IDLE;
        change_capture_mode(CAP_RISING_EDGE);
    }

    return 0;
}


static void decode_cec_command()
{
    printf("CEC command ");
    put_buf(__this->msg, __this->rx_count);
    u8 intiator = __this->msg[0] >> 4;
    switch (__this->msg[1]) {
    case CECOP_FEATURE_ABORT:
        printf("__this->state < ARC_STATUS_INIT=0x%x\n", __this->state);
        if (__this->state <= ARC_STATUS_INIT) {
//		 	putchar('2');
            __this->cec_ok = 0;
            __this->state = ARC_STATUS_REQ_ADD;
        }
        break;
    case CECOP_GIVE_PHYSICAL_ADDRESS:
        printf("Report_Physical_Add_Cmd\n");
        static u8 flag=0;
        printf("flag1 =============================== %d\n",flag);

        if(flag == 0){
        cec_transmit_frame(Report_Physical_Add_Cmd, 5);
        flag=1;
        }
        else if(flag == 1){
        cec_transmit_frame(Report_Physical_Add_Cmd1, 5);
        flag=2;
        }
        else{
        cec_transmit_frame(Report_Physical_Add_Cmd2, 5);
        flag = 0;
        }
        break;
    case CECOP_GIVE_DEVICE_VENDOR_ID:
        cec_transmit_frame(Device_Vendor_Id_Cmd, 5);
        break;
    case GET_CEC_VERSION:
        CEC_Version_cmd[0] = intiator;
        cec_transmit_frame(CEC_Version_cmd, 3);
        break;
    case CECOP_ARC_REQUEST_INITIATION://接收到来自电视请求ARC通信的消息
        cec_transmit_frame(Initiate_Arc_cmd, 2);//响应电视开始ARC通信的请求
        __this->state = ARC_STATUS_INIT;
        __this->idle_time = 3500 / TIME_PRD;
        break;
    case CECOP_ARC_REPORT_INITIATED:
        __this->cec_ok = 1;
        __this->idle_time = 500 / TIME_PRD;
        break;
    case CECOP_ARC_REPORT_TERMINATED:   //接收到来自电视停止ARC通信的消息
        // cec_transmit_frame(Terminate_Arc_cmd,2);
        __this->idle_time = 3500 / TIME_PRD;
        if (__this->state <= ARC_STATUS_INIT) {
            __this->cec_ok = 0;
            __this->state = ARC_STATUS_REQ_ADD;
        }
        break;
    case CECOP_ARC_REQUEST_TERMINATION:   //接收到来自电视请求停止ARC通信的消息
        cec_transmit_frame(Terminate_Arc_cmd,2);//响应停止要求
        __this->idle_time = 3500 / TIME_PRD;
        if (__this->state <= ARC_STATUS_INIT) {
            __this->cec_ok = 0;
            __this->state = ARC_STATUS_REQ_ADD;
        }
        break;

    case CECOP_SET_STREAM_PATH:
        break;
    case CECOP_GIVE_OSD_NAME:
        cec_transmit_frame(Set_Osd_Name_cmd, 14);
        break;

    case CECOP_REQUEST_SHORT_AUDIO:
        __this->cec_ok = 1;
        cec_transmit_frame(Report_Short_Audio_Desc_cmd, sizeof(Report_Short_Audio_Desc_cmd));
        __this->idle_time = 3500 / TIME_PRD;
        break;

    case CECOP_SYSTEM_AUDIO_MODE_REQUEST:
//        cec_transmit_frame(Set_System_Audio_Mode_cmd, sizeof(Set_System_Audio_Mode_cmd));
        cec_transmit_frame(Broadcast_System_Audio_Mode_cmd, sizeof(Set_System_Audio_Mode_cmd));
        break;
    case CECOP_GIVE_AUDIO_STATUS:
    #ifdef SUPPORT_HDMI_VOL_CHANGE
        cee_send_main_vol_status();
    #else
        cec_transmit_frame(Report_Audio_Status_cmd, sizeof(Report_Audio_Status_cmd));
    #endif
        break;

    case CECOP_GIVE_SYSTEM_AUDIO_MODE_STATUS:
        cec_transmit_frame(System_Audio_Mode_Status_cmd, sizeof(System_Audio_Mode_Status_cmd));
        break;
    case CECOP_TUNER_DEVICE_STATUS:
        cec_transmit_frame(Give_Tuner_Device_Status_cmd, sizeof(Give_Tuner_Device_Status_cmd) / sizeof(Give_Tuner_Device_Status_cmd[0]));
        break;
    case CECOP_GIVE_DEVICE_POWER_STATUS:
        cec_transmit_frame(Report_Power_Status_cmd, sizeof(Report_Power_Status_cmd));
        break;


#ifdef SUPPORT_HDMI_VOL_CHANGE
	case CECOP_USER_CONTROL_PRESSED:
		if (__this->cec_ok)
		{
			if(__this->msg[2]==0x41)
			{
                app_common_key_var_event(KEY_VOL_UP);
			}
			else if(__this->msg[2]==0x42)
			{
                app_common_key_var_event(KEY_VOL_DOWN);
			}
		}
		break;
#endif
    default:
        printf("CEC command not Response");
        put_buf(__this->msg, __this->rx_count);
        break;
    }
}


static void cec_cmd_response()
{
    if (__this->rx_count > 1) 
    {
        decode_cec_command();
    }

//    putchar('1');
    if (__this->idle_time < (150 / TIME_PRD)) {
        __this->idle_time = 150 / TIME_PRD;
    }

    if (cec_work_mode == CEC_IDLE) {
        cec_state = CEC_RECEIVE;
    }
    if (__this->msg_count != 0) {
        __this->msg_count = 0xff;
    }
    __this->rx_msg = 0;
}




static void cec_timer_loop()
{
    if (__this->rx_msg) 
    {
        return;
    }

    if (cec_work_mode) 
    {
        return;
    }

    if (__this->idle_time) 
    {
        __this->idle_time--;
        return;
    }

    if (__this->msg_count && cec_frame.ack != CEC_SUCC) 
    {
        __this->msg_count = 0;
        r_printf("send cec msg nak %d", cec_frame.ack);
    } 
    
    static u8 prev_state = 0xff;
    if (prev_state !=  __this->state)
    {
        prev_state=__this->state;
        printf("ARC_STATUS[%d]\n", __this->state);
    }

    // printf("[%d]", __this->state);
    switch (__this->state) 
    {

    case ARC_STATUS_REQ_ADD:
        if (__this->msg_count == 0xff) 
        {
            __this->state = ARC_STATUS_REPORT_ADD;
            __this->msg_count = 0;
            break;
        }
        __this->cec_ok = 0;
        cec_transmit_frame(PollingMessage_cmd, 1);
        __this->msg_count ++;

        if (__this->msg_count == 2) 
        {
            __this->idle_time = 50 / TIME_PRD;
        }
        if (__this->msg_count > 3) 
        {
            __this->state = ARC_STATUS_REPORT_ADD;
            __this->idle_time = 50 / TIME_PRD;
            __this->msg_count = 0;
        }
        break;

    case ARC_STATUS_REPORT_ADD:
        __this->logic_addr = 5;
        if (__this->msg_count == 0xff) 
        {
            __this->state = ARC_STATUS_WAIT_ONLINE;
            __this->msg_count = 0;
            break;
        }
//		__this->cec_ok=1;
         static u8 flag=0;
         printf("flag2 =============================== %d\n",flag);
         if(flag == 0)
         {
            cec_transmit_frame(Report_Physical_Add_Cmd, 5);
            flag=1;
        }
        else if(flag == 1)
        {
            cec_transmit_frame(Report_Physical_Add_Cmd1, 5);
            flag=2;
        }
        else
        {
            cec_transmit_frame(Report_Physical_Add_Cmd2, 5);
            flag = 0;
        }
        __this->msg_count ++;
        if (__this->msg_count > 1) 
        {
            __this->state = ARC_STATUS_WAIT_ONLINE;
            __this->msg_count = 0;
        }
        break;

    case ARC_STATUS_WAIT_ONLINE:
        if (__this->msg_count == 0xff) 
        {
            __this->state = ARC_STATUS_STB;
            __this->msg_count = 0;
            break;
        }
        cec_transmit_frame(Device_Vendor_Id_Cmd, 5);
        __this->msg_count ++;
        if (__this->msg_count > 1) {
            __this->state = ARC_STATUS_STB;
            __this->msg_count = 0;
        }
        break;

    case ARC_STATUS_STB:
        if (__this->msg_count == 0xff) {
            __this->state = ARC_STATUS_INIT;
            __this->msg_count = 0;
            break;
        }
        
//        cec_transmit_frame(Set_System_Audio_Mode_cmd, 3);
        cec_transmit_frame(Broadcast_System_Audio_Mode_cmd, 3);
        // __this->cec_ok = 1;

        __this->msg_count ++;
        __this->idle_time = 300 / TIME_PRD;
        if (__this->msg_count > 1) {
            __this->state = ARC_STATUS_INIT;
        }
        break;

    case ARC_STATUS_INIT:
        if (__this->cec_ok) {
            break;
        }
        #if 0//def HDMI_CEC_DRV_ADD
            arc_send_open_cec_ctl();
        #endif
        #if 1
        __this->msg_count ++;
        if (__this->msg_count > 20)
        {
            __this->msg_count = 0;
            __this->state = ARC_STATUS_REQ_ADD;
        }else
        {
            if (!(__this->msg_count%2))
            {
                cec_transmit_frame(Initiate_Arc_cmd, 2);
                // __this->idle_time = 3500 / TIME_PRD;
            }else{
                cec_transmit_frame(Broadcast_System_Audio_Mode_cmd, 3);
                // __this->idle_time = 300 / TIME_PRD;
            }
            
            // __this->state = ARC_STATUS_INIT_SUCCESS;
        }
        #else
        cec_transmit_frame(Initiate_Arc_cmd, 2);
        __this->idle_time = 3500 / TIME_PRD;
        #endif
        // __this->state = ARC_STATUS_INIT_SUCCESS;
        break;
    }
}



void hdmi_cec_init(void)
{
    printf("hdmi_cec_init");
    use_ctl.hdmi_cec_init_flag=1;
    memset(__this, 0, sizeof(*__this));

    __this->logic_addr = 5;
    cec_state = CEC_RECEIVE;
    cec_work_mode = CEC_IDLE;
    __this->cec_ok = 0;

    //port
    gpio_set_die(TCFG_HDMI_CEC_PORT, 1);
    gpio_set_direction(TCFG_HDMI_CEC_PORT, 1);
    gpio_set_pull_up(TCFG_HDMI_CEC_PORT, 1);
    gpio_set_pull_down(TCFG_HDMI_CEC_PORT, 0);


    u32 clk = clk_get("timer");
    printf("clk %d %d", clk, CAP_CLK);
    printf("TCFG_HDMI_CEC_PORT:[%d]", TCFG_HDMI_CEC_PORT);

#if CEC_TMR_IDX  == IRQ_TIME2_IDX
    //capture input channel
    JL_IOMAP->CON1 |= BIT(30);
    JL_IOMAP->CON2 &= ~(0x7F << 16);
    JL_IOMAP->CON2 |= (TCFG_HDMI_CEC_PORT << 16);

    CEC_TMR->CON = BIT(14);
#elif CEC_TMR_IDX  == IRQ_TIME3_IDX
    JL_IOMAP->CON1 |= BIT(31);
    JL_IOMAP->CON2 &= ~(0x7F << 16);
    JL_IOMAP->CON2 |= (TCFG_HDMI_CEC_PORT << 16);

    CEC_TMR->CON = BIT(14);
#elif CEC_TMR_IDX  == IRQ_TIME0_IDX
    JL_IOMAP->CON1 |= BIT(28);
    JL_IOMAP->CON2 &= ~(0x7F << 16);
    JL_IOMAP->CON2 |= (TCFG_HDMI_CEC_PORT << 16);

    CEC_TMR->CON = BIT(14);
#else
#error "please config time input channle"
#endif
    // return;
    change_capture_mode(CAP_FALLING_EDGE);
    request_irq(CEC_TMR_IDX, 2, cec_timer_isr, 0);
    __this->time_id = sys_timer_add(NULL, cec_timer_loop, TIME_PRD);
    g_printf("add timer %x", __this->time_id);
}



void hdmi_cec_close(void)
{
    printf("hdmi_cec_close");
#ifdef HDMI_CEC_DRV_ADD
    arc_send_close_cec_ctl();
#endif
    use_ctl.hdmi_cec_init_flag=0;
    
    CEC_TMR->CON = BIT(14);
    __this->state = ARC_STATUS_REQ_ADD;
    __this->cec_ok = 0;

    gpio_set_direction(TCFG_HDMI_CEC_PORT, 1);
    gpio_set_pull_up(TCFG_HDMI_CEC_PORT, 0);
    gpio_set_die(TCFG_HDMI_CEC_PORT, 0);

    g_printf("del timer %x", __this->time_id);
    if (__this->time_id) {
        sys_timer_del(__this->time_id);
        __this->time_id = 0;
    }

}
#ifdef SUPPORT_HDMI_CH_DELAY_OPEN
u8 get_cec_ok_flag()
{
    return __this->cec_ok;
}
#endif


#ifdef SUPPORT_HDMI_VOL_CHANGE
void cee_send_main_vol_status()
{
    if (!(app_check_curr_task(APP_SPDIF_TASK) && use_ctl.bSpdifMode==SPDIF_HDMI))
    {
        return;
    }
    
    if (__this->cec_ok)
    {
        s16 hdmi_cec_led_vol;
        hdmi_cec_led_vol = use_ctl.led_vol*HDMI_MAX_VOL /LED_SHOW_MAX_VOL;

        if (!hdmi_cec_led_vol && use_ctl.led_vol)
        {
            hdmi_cec_led_vol = 1;
        }
        Report_Audio_Status_cmd[2] = hdmi_cec_led_vol;

        cec_transmit_frame(Report_Audio_Status_cmd, sizeof(Report_Audio_Status_cmd));
    }
}
#endif


#endif


#ifdef SUPPORT_SPDIF_HDMI_MODE

#include "printf.h"
#include "cpu.h"
#include "asm/iic_hw.h"
#include "asm/iic_soft.h"
#include "timer.h"
//#include "app_config.h"
#include "event.h"
#include "system/includes.h"


#if TCHFG_HW_I2C_ENABLE
#define iic_dev                             0
#define iic_init(iic)                       hw_iic_init(iic)
#define iic_uninit(iic)                     hw_iic_uninit(iic)
#define iic_start(iic)                      hw_iic_start(iic)
#define iic_stop(iic)                       hw_iic_stop(iic)
#define iic_tx_byte(iic, byte)              hw_iic_tx_byte(iic, byte)
#define iic_rx_byte(iic, ack)               hw_iic_rx_byte(iic, ack)
#define iic_read_buf(iic, buf, len)         hw_iic_read_buf(iic, buf, len)
#define iic_write_buf(iic, buf, len)        hw_iic_write_buf(iic, buf, len)
#define iic_suspend(iic)                    hw_iic_suspend(iic)
#define iic_resume(iic)                     hw_iic_resume(iic)
#else
#define iic_dev                             0
#define iic_init(iic)                       soft_iic_init(iic)
#define iic_uninit(iic)                     soft_iic_uninit(iic)
#define iic_start(iic)                      soft_iic_start(iic)
#define iic_stop(iic)                       soft_iic_stop(iic)
#define iic_tx_byte(iic, byte)              soft_iic_tx_byte(iic, byte)
#define iic_rx_byte(iic, ack)               soft_iic_rx_byte(iic, ack)
#define iic_read_buf(iic, buf, len)         soft_iic_read_buf(iic, buf, len)
#define iic_write_buf(iic, buf, len)        soft_iic_write_buf(iic, buf, len)
#define iic_suspend(iic)                    soft_iic_suspend(iic)
#define iic_resume(iic)                     soft_iic_resume(iic)
#endif

void audio_block_decode(u8 *data, int length)
{
    char *audio_format[] = {
        "reserved",
        "Linear pulse-code modulation (LPCM)",
        "AC-3",
        "MPEG-1 (Layers 1 and 2)",
        "MP3",
        "MPEG-2",
        "AAC LC",
        "DTS",
        "ATRAC",
        "DSD (One-Bit) Audio, Super Audio CD",
        "DD+",
        "DTS-HD",
        "MAT/MLP/Dolby TrueHD",
        "DST Audio",
        "Microsoft WMA Pro",
        "Extension, see Byte 3"
    };

    int audio_format_code = data[1] >> 3;
    int num_of_channels = (data[1] & 0x7) + 1;
    printf("    %s\n", audio_format[audio_format_code]);
    printf("      Max channels: %d\n", num_of_channels);

    char *freq[] = {
        "res",
        "192",
        "176",
        "96",
        "88",
        "48",
        "44.1",
        "32"
    };
    printf("      Supported sample rates (kHz): ");
    for (int i = 0; i < 7; i++) {
        if (data[2] & (1 << i)) {
            printf("%s ", freq[6 - i]);
        }
    }


    char *bits[] = {
        "24",
        "20",
        "16"
    };
    printf("      Supported sample sizes (bits): ");
    for (int i = 0; i < 3; i++) {
        if (data[3] & (1 << i)) {
            printf("%s ", bits[2 - i]);
        }
    }
}

int hdmi_dcc_read(u8 *buffer, u8 addr, u32 len)
{
    int ret = 1;
    int ack_try = 0;

#ifdef HDMI_IIC_READ_DATA_FIX
__step_1:
	iic_start(iic_dev);

    wdt_clear();
    ret = iic_tx_byte(iic_dev, 0xa0);
    if (!ret) {
        if (ack_try++ < 10) {
            delay(1000);
            goto __step_1;
        } else {
            goto __exit_error;
        }
    }

    ret = iic_tx_byte(iic_dev, addr);
    if (!ret) {
        if (ack_try++ < 10) {
            delay(1000);
            goto __step_1;
        } else {
            goto __exit_error;
        }
    }
	iic_start(iic_dev);

    ret = iic_tx_byte(iic_dev, 0xa1);
    if (!ret) {
        if (ack_try++ < 10) {
            delay(1000);
            goto __step_1;
        } else {
            goto __exit_error;
        }
    }

    for (int i = 0; i < (len - 1); i ++) {
        buffer[i] = iic_rx_byte(iic_dev, 1);
    }
    buffer[len - 1] = iic_rx_byte(iic_dev, 0);
    ret = 0;

__exit_error:
    iic_stop(iic_dev);
    return ret;
#else
__step_1:
	iic_start(iic_dev);

    ret = iic_tx_byte(iic_dev, 0xa0);
    if (!ret) {
        if (ack_try++ < 10) {
            delay(5000);
            goto __step_1;
        } else {
            goto __exit_error;
        }
    }

    ack_try = 0;
    ret = iic_tx_byte(iic_dev, addr);
    if (!ret) {
        if (ack_try++ < 10) {
            delay(5000);
            goto __step_1;
        } else {
            goto __exit_error;
        }
    }
	iic_start(iic_dev);

    ack_try = 0;
    ret = iic_tx_byte(iic_dev, 0xa1);
    if (!ret) {
        if (ack_try++ < 10) {
            delay(5000);
            goto __step_1;
        } else {
            goto __exit_error;
        }
    }

    for (int i = 0; i < (len - 1); i ++) {
        buffer[i] = iic_rx_byte(iic_dev, 1);
    }
    buffer[len - 1] = iic_rx_byte(iic_dev, 0);
    ret = 0;

__exit_error:
    iic_stop(iic_dev);
    return ret;
#endif
}
int hdmi_dcc_dump_edid_info()
{
    printf("%s() %d", __func__, __LINE__);

#if 1

    iic_init(iic_dev);
	u8 *edid_data = (u8 *)malloc(128);
    memset(edid_data, 0, 128);
    for (int i = 0; i < 128; i += 8) {
        if (hdmi_dcc_read(edid_data + i, 0x80 + i, 8) == 0) {
            put_buf(edid_data + i, 8);
        }
    }
    if (edid_data[0] == 0x02) {
        printf("CTA-861 Extension Block\n");
    }

    if (edid_data[1] == 0x03) {
        printf("  Revision: 3\n");
    }

    int byte_number = edid_data[2];
    if (edid_data[3] & (1 << 4)) {
        printf("  Supports YCbCr 4:4:2\n");
    }

    if (edid_data[3] & (1 << 5)) {
        printf("  Supports YCbCr 4:4:4\n");
    }

    if (edid_data[3] & (1 << 6)) {
        printf("  Basic audio support\n");
    }

    if (edid_data[3] & (1 << 7)) {
        printf("  Underscans IT Video Formats by default\n");
    }

    printf(" Native detailed modes:%d\n", edid_data[3] & 0xf);
    int offset = 4;
    u32 addres = 0;

    while (offset < byte_number) {
        int block_head = edid_data[offset];
        int block_type = block_head >> 5;
        int block_length = block_head & 0x1f;

        if (block_length == 0) {
            break;
        }

        if (block_type == 1) {
            printf("Audio Data Block:\n");
            audio_block_decode(edid_data + offset, block_length);

        } else if (block_type == 3) {

            printf("Vendor-Specific Data Block OUI %02X-%02X-%02X\n",
                   edid_data[offset + 1], edid_data[offset + 2], edid_data[offset + 3]);

            if (edid_data[offset + 1] == 0x03 &&
                edid_data[offset + 2] == 0x0c &&
                edid_data[offset + 3] == 0x00) {

                printf("     Source physical address: %02X %02X\n",
                       edid_data[offset + 4], edid_data[offset + 5]);
                addres = (edid_data[offset + 4] << 8) | edid_data[offset + 5] ;
            }
        }

        offset += block_length;
        offset += 1;
    }
	if(addres){
		printf("addres= 0x%x", addres);
		extern void user_change_report_physical_add_cmd(u8 addr1, u8 addr2);
		user_change_report_physical_add_cmd(addres>>8, addres);
	}else{
		printf("\n addres read err \n");
	}
    free(edid_data);	
	
#if 0	
    u8 dcc_block[8];
    for (int i = 0; i < 32; i++) {
        hdmi_dcc_read(dcc_block, i * 8, 8);
        printf("ADDR %x ", i * 8);
        put_buf(dcc_block, 8);
    }
    //Header: 00 FF FF FF FF FF FF 00
    hdmi_dcc_read(dcc_block, 0, 8);
    printf("header:");
    put_buf(dcc_block, 8);

    //生产商ID（地址08,09)
    hdmi_dcc_read(dcc_block, 0x08, 2);
    printf("Vendor ID:");
    put_buf(dcc_block, 2);

    //Version ID
    hdmi_dcc_read(dcc_block, 0x81, 1);
    printf("version ID:");
    put_buf(dcc_block, 2);

    //VSDB
    hdmi_dcc_read(dcc_block, 0xa7, 6);
    printf("VSDB info:");
    put_buf(dcc_block, 6);
#endif

#endif
	return 0;
}

#endif

