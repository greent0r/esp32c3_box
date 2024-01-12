#include "gui_guider.h"


void ui_init_style(lv_style_t * style)
{
  if (style->prop_cnt > 1)
    lv_style_reset(style);
  else
    lv_style_init(style);
}

void init_scr_del_flag(lv_ui *ui)
{
	ui->weather_del = true;
}

void setup_ui(lv_ui *ui)
{
  init_scr_del_flag(ui);
  setup_scr_weather(ui);
  lv_scr_load(ui->weather);
}



