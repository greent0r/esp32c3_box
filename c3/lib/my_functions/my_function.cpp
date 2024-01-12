#include "my_function.hpp"

/*api相关*/
const char* adcode = "";
const char* now_weather_url = "https://restapi.amap.com/v3/weather/weatherInfo?&key=378ce98ed47c440c91b9ae77eb7fe75e&city=";
const char* day_weather_url = "https://restapi.amap.com/v3/weather/weatherInfo?key=378ce98ed47c440c91b9ae77eb7fe75e&extensions=all&city=";
const char* city = "";

/*日期信息*/
int year  = 0;
int month = 0;
int date  = 0;
const char* day = "";

/*天气信息*/
const char* now_temp = "";
const char* now_weather = "";
const char* now_humidity = "";
int         now_humidity_int = 0;

const char* day0_min_temp = "";
const char* day0_max_temp = "";
const char* day0_dayweather = "";
const char* day0_nightweather = "";

const char* day1_min_temp = "";
const char* day1_max_temp = "";
const char* day1_dayweather = "";
const char* day1_nightweather = "";

const char* day2_min_temp = "";
const char* day2_max_temp = "";
const char* day2_dayweather = "";
const char* day2_nightweather = "";

/*是否更新数据的标志*/
int update_flag = 0;

/*时间结构体*/
struct tm timeinfo;

/*时间服务器配置*/
const char* ntpServer = "pool.ntp.org";//时间服务器
const long  gmtOffset_sec = 8 * 3600; // 时区偏移量，单位为秒
const int   daylightOffset_sec = 0;   //秒偏移量

/*校对时钟*/
void time_init(){
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

/*更新日期*/
void date_get(){
  if (!getLocalTime(&timeinfo))
    return;
  year = timeinfo.tm_year + 1900;
  month = timeinfo.tm_mon + 1;
  date = timeinfo.tm_mday;
  
  if     (timeinfo.tm_wday == 0)
    day = "星期天";
  else if(timeinfo.tm_wday == 1)
    day = "星期一";
  else if(timeinfo.tm_wday == 2)
    day = "星期二";
  else if(timeinfo.tm_wday == 3)
    day = "星期三";
  else if(timeinfo.tm_wday == 4)
    day = "星期四";   
  else if(timeinfo.tm_wday == 5)
    day = "星期五";
  else if(timeinfo.tm_wday == 6)
    day = "星期六";          
}

/*更新天气*/
void weather_get()
{
  HTTPClient http1;  
  HTTPClient http2;  
  StaticJsonDocument<768> doc1;
  StaticJsonDocument<2048> doc2;
  
  http1.begin((String)now_weather_url + (String)adcode);
  http1.GET();
  String response = "";
  response = http1.getString();
  deserializeJson(doc1, response);
  
  now_temp     = doc1["lives"][0]["temperature"].as<const char*>();
  now_weather  = doc1["lives"][0]["weather"].as<const char*>();
  now_humidity = doc1["lives"][0]["humidity"].as<const char*>();
  now_humidity_int = atoi(doc1["lives"][0]["humidity"].as<const char*>());

  http1.end(); 

  http2.begin((String)day_weather_url + (String)adcode);
  http2.GET();
  response = http2.getString();
  deserializeJson(doc2, response);

  day0_min_temp     = doc2["forecasts"][0]["casts"][0]["nighttemp"].as<const char*>();
  day0_max_temp     = doc2["forecasts"][0]["casts"][0]["daytemp"].as<const char*>();
  day0_dayweather   = doc2["forecasts"][0]["casts"][0]["dayweather"].as<const char*>();
  day0_nightweather = doc2["forecasts"][0]["casts"][0]["nightweather"].as<const char*>();

  day1_min_temp     = doc2["forecasts"][0]["casts"][1]["nighttemp"].as<const char*>();
  day1_max_temp     = doc2["forecasts"][0]["casts"][1]["daytemp"].as<const char*>();
  day1_dayweather   = doc2["forecasts"][0]["casts"][1]["dayweather"].as<const char*>();
  day1_nightweather = doc2["forecasts"][0]["casts"][1]["nightweather"].as<const char*>();

  day2_min_temp     = doc2["forecasts"][0]["casts"][2]["nighttemp"].as<const char*>();
  day2_max_temp     = doc2["forecasts"][0]["casts"][2]["daytemp"].as<const char*>();
  day2_dayweather   = doc2["forecasts"][0]["casts"][2]["dayweather"].as<const char*>();
  day2_nightweather = doc2["forecasts"][0]["casts"][2]["nightweather"].as<const char*>();

  http2.end(); 
}
