
#include "includes.h"
#include "app_config.h"
#include "asm/cpu.h"
#include "asm/irq.h"
#include "asm/clock.h"
#include "system/timer.h"
#include "system/init.h"
#include "gpio.h"

#include "debug.h"


struct timer_hdl {
    u32 ticks;
    int index;
    int prd;
    u32 fine_cnt;
    void *power_ctrl;
    struct list_head head;
};

static struct timer_hdl hdl;


#define __this  (&hdl)

#define APP_TIMER_CLK           clk_get("timer")
#define MAX_TIME_CNT            0x7fff
#define MIN_TIME_CNT            0x100

#define TIMER0_UNIT_MS           2
#define MAX_TIMER0_PERIOD_MS     (1000/TIMER_UNIT_MS)

static const u32 timer_div[] = {    //预分频选择位
    /*0000*/    1,
    /*0001*/    4,
    /*0010*/    16,
    /*0011*/    64,
    /*0100*/    2,
    /*0101*/    8,
    /*0110*/    32,
    /*0111*/    128,
    /*1000*/    256,
    /*1001*/    4 * 256,
    /*1010*/    16 * 256,
    /*1011*/    64 * 256,
    /*1100*/    2 * 256,
    /*1101*/    8 * 256,
    /*1110*/    32 * 256,
    /*1111*/    128 * 256,
};

#ifdef SUPPORT_ENCODER
#ifdef SUPPORT_ENCODER_2_IO
extern void ENC_Encoder(void);
#if defined SUPPORT_DOUBLE_ENCODER
extern void ENC_Encoder_2(void);
#endif
#endif
#ifdef SUPPORT_ENCODER_AD_IO
extern void ENC_AD_INIT();
extern void ENC_AD_SCAN();
#endif
#endif


___interrupt
static void idle_timer0_isr()
{
    static u32 cnt = 0;
    JL_TIMER0->CON |= BIT(14);  //PCLR：在此处写入‘1’将清除PND中断请求标志

    ++cnt;

	
    #ifdef SUPPORT_ENCODER
    #ifdef SUPPORT_ENCODER_AD_IO
    if ((cnt % 10) == 0)
    {
        ENC_AD_SCAN();
    }
    #endif
    #endif

    if ((cnt % 25) == 0) {

	 #ifdef SUPPORT_ENCODER
	 #ifdef SUPPORT_ENCODER_2_IO
	 	ENC_Encoder();
	 #endif
	 #if defined SUPPORT_DOUBLE_ENCODER
	 	ENC_Encoder_2();
	 #endif
	 
	 #endif
	 
		
    }	
	
    if (cnt == 500) {
        cnt = 0;
    }
}

int user_timer0_init()
{
    u32 prd_cnt;
    u8 index;

    for (index = 0; index < (sizeof(timer_div) / sizeof(timer_div[0])); index++) {
//        prd_cnt = TIMER_UNIT_MS * (APP_TIMER_CLK / 1000) / timer_div[index];
	#if defined SUPPORT_ENCODER_USE_TIMER0_SET
	 		prd_cnt = 0.04 * (APP_TIMER_CLK / 1000) / timer_div[index];    // 0.04 ms
	#else
	 		prd_cnt = 0.04 * (APP_TIMER_CLK / 1000) / timer_div[index];    // 0.04 ms   //根据单位时间选择预分频位，并算出装载值
    #endif
        if (prd_cnt > MIN_TIME_CNT && prd_cnt < MAX_TIME_CNT) {
            break;
        }
    }

	// prd_cnt=prd_cnt/2;
	
    __this->index   = index;
    __this->prd     = prd_cnt;

    JL_TIMER0->CNT = 0;       //CNT计数清零
    JL_TIMER0->PRD = prd_cnt; //   2ms
    request_irq(IRQ_TIME0_IDX, 3, idle_timer0_isr, 0);
    JL_TIMER0->CON = (index << 4) | BIT(0) | BIT(3);        //工作模式选为定时/计数模式，选择OSC时钟作为timer的驱动源

//    log_info("PRD : 0x%x / %d", JL_TIMER2->PRD, clk_get("timer"));

    return 0;
}


static void user_timer0_uninit()
{
    JL_TIMER0->CON &= ~(BIT(1) | BIT(0));
}





static void idle_timer1_open()
{
    JL_TIMER1->CON |= BIT(0);
}

static void idle_timer1_close()
{
    JL_TIMER1->CON &= ~(BIT(1) | BIT(0));
}


#if defined SUPPORT_ENCODER
late_initcall(user_timer0_init);
#endif