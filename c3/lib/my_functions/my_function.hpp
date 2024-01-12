#ifndef _MY_FUNCTION_H
#define _MY_FUNCTION_H

#include <lvgl.h>
#include <TFT_eSPI.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Arduino.h>

/*api相关*/
extern const char* adcode;
extern const char* now_weather_url;
extern const char* day_weather_url;
extern const char* city;

/*日期信息*/
extern int year;
extern int month;
extern int date;
extern const char* day;

/*天气信息*/
extern const char* now_temp;
extern const char* now_weather;
extern const char* now_humidity;
extern int         now_humidity_int;

extern const char* day0_min_temp;
extern const char* day0_max_temp;
extern const char* day0_dayweather;
extern const char* day0_nightweather;

extern const char* day1_min_temp;
extern const char* day1_max_temp;
extern const char* day1_dayweather;
extern const char* day1_nightweather;

extern const char* day2_min_temp;
extern const char* day2_max_temp;
extern const char* day2_dayweather;
extern const char* day2_nightweather;

/*是否更新数据的标志*/
extern int update_flag;

/*时间结构体*/
extern struct tm timeinfo;

/*时间服务器配置*/
extern const char* ntpServer;
extern const long  gmtOffset_sec;
extern const int   daylightOffset_sec;

/*wifi信息*/
extern const char* ssid;
extern const char* password;

void date_get();
void weather_get();
void time_init();

#endif