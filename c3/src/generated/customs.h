#ifndef WIDGET_INIT_H
#define WIDGET_INIT_H

#include <lvgl.h>
#include <Arduino.h>
#include <stdio.h>
#include "gui_guider.h"
#include "my_function.hpp"

extern const lv_img_dsc_t * weather_animimg_1_imgs[8];
extern const lv_img_dsc_t * mika_animimg_1_imgs[45];
extern lv_timer_t *lvgl_task_clock;
extern lv_timer_t *lvgl_task_update;

void weather_update();
void update_weather_img(lv_obj_t* obj, const char* weather);

#endif
