#include "key_driver.h"
#include "adkey.h"
#include "gpio.h"
#include "system/event.h"
#include "app_config.h"


#if TCFG_ADKEY_ENABLE

static const struct adkey_platform_data *__this = NULL;

u8 ad_get_key_value(void);
//按键驱动扫描参数列表
struct key_driver_para adkey_scan_para = {
    .scan_time 	  	  = 20,				//按键扫描频率, 单位: ms
    .last_key 		  = NO_KEY,  		//上一次get_value按键值, 初始化为NO_KEY;
    .filter_time  	  = 5,				//按键消抖延时;
    .long_time 		  = 75,  			//按键判定长按数量
    .hold_time 		  = (75 + 15),  	//按键判定HOLD数量
#ifdef click__time
    .click_delay_time = click__time,	//按键被抬起后等待连击延时数量
#else
    .click_delay_time = 20,				//按键被抬起后等待连击延时数量
#endif
    .key_type		  = KEY_DRIVER_TYPE_AD,
    .get_value 		  = ad_get_key_value,
};

#ifdef SUPPORT_KPH_ADKEY_FUNCTION
const u16 ad_key_table[] = {
    USE_DEFINE_KEY_TABL
};
#endif


u8 ad_get_key_value(void)
{
    u8 i;
    u16 ad_data;

    if (!__this->enable)
    {
        return NO_KEY;
    }


    /* ad_data = adc_get_voltage(__this->ad_channel); */
    ad_data = adc_get_value(__this->ad_channel);
 
    //printf("ad_data:%d\n",ad_data);
  
    


#if defined USE_KEY_VALUE
	if(ad_data>1010)
	{		
		return NO_KEY; 
	}
    for (i = 0; i < sizeof(ad_key_table); i++) {
        if ((ad_data <= __this->ad_value[i]) && (__this->ad_value[i] < 0x3ffL)) {
            return __this->key_value[i];
        }
    }

#else		
	if(ad_data>1010)
	{
	    #ifdef SUPPORT_USE_DOUBLE_AD_IO_KEY
			ad_data = adc_get_value(AD_CH_ADK1);
			if(ad_data>1010)		
	    #endif
	
			return NO_KEY; 
	}
		
	for (i = 0; i < sizeof(ad_key_table); i++) 
	{
		if (ad_data <= ad_key_table[i] ) 
        {
			
		 #ifdef SUPPORT_USE_DOUBLE_AD_IO_KEY
		 		if(adc_get_value(TCFG_ADKEY_AD_CHANNEL)>1010)
		 	#if defined(ADK_USE_24KEY_MODE)
		 			i+=12;
		 	#else
		 			i+=10;		
		 	#endif
		 #endif	
            printf(">>>>>>>>>%d\n",i);

		return i;
		}
	}
	
#endif	
    return NO_KEY;
}

int adkey_init(const struct adkey_platform_data *adkey_data)
{
    __this = adkey_data;
    if (!__this) {
        return -EINVAL;
    }

    if (!__this->enable) {
        return KEY_NOT_SUPPORT;
    }
    adc_add_sample_ch(__this->ad_channel);          //注意：初始化AD_KEY之前，先初始化ADC
#if (TCFG_ADKEY_LED_IO_REUSE || TCFG_ADKEY_IR_IO_REUSE || TCFG_ADKEY_LED_SPI_IO_REUSE)
#else
    gpio_set_die(__this->adkey_pin, 0);
    gpio_set_direction(__this->adkey_pin, 1);
    gpio_set_pull_down(__this->adkey_pin, 0);
    if (__this->extern_up_en) 
    {
        gpio_set_pull_up(__this->adkey_pin, 0);
    } 
    else 
    {
        gpio_set_pull_up(__this->adkey_pin, 1);
    }
#endif

    return 0;
}

#if (TCFG_ADKEY_LED_IO_REUSE || TCFG_ADKEY_IR_IO_REUSE || TCFG_ADKEY_LED_SPI_IO_REUSE)

#if TCFG_ADKEY_IR_IO_REUSE
static u8 ir_io_sus = 0;
extern u8 ir_io_suspend(void);
extern u8 ir_io_resume(void);
#endif
#if TCFG_ADKEY_LED_IO_REUSE
static u8 led_io_sus = 0;
extern u8 led_io_suspend(void);
extern u8 led_io_resume(void);
#endif
#if TCFG_ADKEY_LED_SPI_IO_REUSE
static u8 led_spi_sus = 0;
extern u8 led_spi_suspend(void);
extern u8 led_spi_resume(void);
#endif
u8 adc_io_reuse_enter(u32 ch)
{
    if (ch == __this->ad_channel) {
#if TCFG_ADKEY_IR_IO_REUSE
        if (ir_io_suspend()) {
            return 1;
        } else {
            ir_io_sus = 1;
        }
#endif
#if TCFG_ADKEY_LED_IO_REUSE
        if (led_io_suspend()) {
            return 1;
        } else {
            led_io_sus = 1;
        }
#endif
#if TCFG_ADKEY_LED_SPI_IO_REUSE
        if (led_spi_suspend()) {
            return 1;
        } else {
            led_spi_sus = 1;
        }
#endif
        gpio_set_die(__this->adkey_pin, 0);
        gpio_set_direction(__this->adkey_pin, 1);
        gpio_set_pull_down(__this->adkey_pin, 0);
        if (__this->extern_up_en) {
            gpio_set_pull_up(__this->adkey_pin, 0);
        } else {
            gpio_set_pull_up(__this->adkey_pin, 1);
        }
    }
    return 0;
}

u8 adc_io_reuse_exit(u32 ch)
{
    if (ch == __this->ad_channel) {
#if TCFG_ADKEY_IR_IO_REUSE
        if (ir_io_sus) {
            ir_io_sus = 0;
            ir_io_resume();
        }
#endif
#if TCFG_ADKEY_LED_IO_REUSE
        if (led_io_sus) {
            led_io_sus = 0;
            led_io_resume();
        }
#endif
#if TCFG_ADKEY_LED_SPI_IO_REUSE
        if (led_spi_sus) {
            led_spi_sus = 0;
            led_spi_resume();
        }
#endif
    }
    return 0;
}

#endif



#endif  /* #if TCFG_ADKEY_ENABLE */





