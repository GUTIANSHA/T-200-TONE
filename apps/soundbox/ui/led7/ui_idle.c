#include "includes.h"
#include "ui/ui_api.h"

#if (TCFG_UI_ENABLE&&(CONFIG_UI_STYLE == STYLE_JL_LED7))
static void ui_idle_main(void *hd, void *private) //主界面显示
{
    if (!hd) {
        return;
    }
    LCD_API *dis = (LCD_API *)hd;//清显示
    dis->lock(1);
    dis->clear();
    dis->setXY(0, 0);
    dis->clear_icon(0xffff);

    dis->lock(0);
}

#ifdef SUPPORT_UI_XY8080
static void *ui_open_idle(void *hd)
{
    void *private = NULL;
    ui_set_auto_reflash(200);//设置主页500ms自动刷新
    return private;
}
#endif

#ifdef SUPPORT_UI_UART_XY8080
#include "user_cfg.h"
static void *ui_open_idle(void *hd)
{
	void *private = NULL;
    if (!hd) {
        return NULL;
    }
    LCD_API *dis = (LCD_API *)hd;//清显示
    dis->lock(1);
    dis->clear();
    dis->setXY(0, 0);
    dis->clear_icon(0xffff);
    dis->lock(0);
	return private;
}
#endif

const struct ui_dis_api idle_main = {
    .ui      = UI_IDLE_MENU_MAIN,
#if defined SUPPORT_UI_XY8080 || defined SUPPORT_UI_UART_XY8080
		.open	 = ui_open_idle,
#else
		.open	 = NULL,
#endif
    .ui_main = ui_idle_main,
    .ui_user = NULL,
    .close   = NULL,
};
#endif
