#include "Watchy_ChurchYear.h"

#define DARKMODE false

const uint8_t BATTERY_SEGMENT_WIDTH = 7;
const uint8_t BATTERY_SEGMENT_HEIGHT = 11;
const uint8_t BATTERY_SEGMENT_SPACING = 9;
const uint8_t WEATHER_ICON_WIDTH = 48;
const uint8_t WEATHER_ICON_HEIGHT = 32;

void WatchyChurchYear::drawWatchFace(){
    display.fillScreen(DARKMODE ? GxEPD_BLACK : GxEPD_WHITE);
    display.setTextColor(DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    drawTime();
    //drawDate();
    drawSteps();
    drawWeather();
    drawBattery();
    // Draw Wifi
    display.drawBitmap(90, 40, WIFI_CONFIGURED ? wifi : wifioff, 26, 18, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    // Draw Bluetooth
    if(BLE_CONFIGURED){
        display.drawBitmap(100, 75, bluetooth, 13, 21, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    }
    // Draw weekname
    drawWeekName();
    // Draw Saint
    drawSaint();
}

void WatchyChurchYear::drawTime(){
    // Set font and cursor
    display.setFont(&DSEG7_Classic_Regular_15);
    display.setCursor(5, 25+5);

    // Display the int day
    if(currentTime.Day < 10){
      display.print("0");
    }
    display.print(currentTime.Day);

    // Print spacer
    display.print("-");

    // Display the month
    String month = monthShortStr(currentTime.Month);
    display.print(month);

    // Print Spacer
    display.print(" ");

    display.setFont(&DSEG7_Classic_Bold_25);
    // Display Time
    int displayHour;
    if(HOUR_12_24==12){
      displayHour = ((currentTime.Hour+11)%12)+1;
    } else {
      displayHour = currentTime.Hour;
    }
    if(displayHour < 10){
        display.print("0");
    }
    display.print(displayHour);
    display.print(":");
    if(currentTime.Minute < 10){
        display.print("0");
    }
    display.println(currentTime.Minute);


    // Show Day
    display.setFont(&Seven_Segment10pt7b);
    String dayOfWeek = dayStr(currentTime.Wday);
    display.println(dayOfWeek);
}

void WatchyChurchYear::drawSteps(){
    // reset step counter at midnight
    if (currentTime.Hour == 0 && currentTime.Minute == 0){
      sensor.resetStepCounter();
    }
    uint32_t stepCount = sensor.getCounter();
    display.drawBitmap(10, 65, steps, 19, 23, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    display.setCursor(40, 80);
    // Show thousands to 1 decimal place
    display.print(((float)((int)((stepCount / 10) * 10))) / 1000);
    display.println("k");
}

void WatchyChurchYear::drawBattery(){
    display.drawBitmap(124, 45, battery, 37, 21, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    display.fillRect(159, 78, 27, BATTERY_SEGMENT_HEIGHT, DARKMODE ? GxEPD_BLACK : GxEPD_WHITE);//clear battery segments
    int8_t batteryLevel = 0;
    float VBAT = getBatteryVoltage();
    if(VBAT > 4.1){
        batteryLevel = 3;
    }
    else if(VBAT > 3.95 && VBAT <= 4.1){
        batteryLevel = 2;
    }
    else if(VBAT > 3.80 && VBAT <= 3.95){
        batteryLevel = 1;
    }
    else if(VBAT <= 3.80){
        batteryLevel = 0;
    }

    display.setCursor(165, 50);
    display.setFont(&DSEG7_Classic_Regular_15);
    display.println(batteryLevel);

    for(int8_t batterySegments = 0; batterySegments < batteryLevel; batterySegments++){
        //display.fillRect(159 + (batterySegments * BATTERY_SEGMENT_SPACING), 78, BATTERY_SEGMENT_WIDTH, BATTERY_SEGMENT_HEIGHT, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
        display.fillRect(159 + (batterySegments * BATTERY_SEGMENT_SPACING), 78, BATTERY_SEGMENT_WIDTH, BATTERY_SEGMENT_HEIGHT, DARKMODE ? GxEPD_BLACK : GxEPD_WHITE);
    }
}

void WatchyChurchYear::drawWeather(){

    weatherData currentWeather = getWeatherData();

    int8_t temperature = currentWeather.temperature;
    int16_t weatherConditionCode = currentWeather.weatherConditionCode;

    display.setFont(&DSEG7_Classic_Regular_15);
    int16_t  x1, y1;
    uint16_t w, h;
    display.getTextBounds(String(temperature), 0, 0, &x1, &y1, &w, &h);
    if(159 - w - x1 > 87){
        display.setCursor(120 - w - x1, 80);
    }else{
        display.setFont(&DSEG7_Classic_Regular_15);
        display.getTextBounds(String(temperature), 0, 0, &x1, &y1, &w, &h);
        display.setCursor(159 - w - x1, 136);
    }
    display.print(temperature);
    if (currentWeather.isMetric == true)
    {
      display.println(" C");
    } else {
      display.println(" F");
    }
    //display.drawBitmap(100, 70, currentWeather.isMetric ? celsius : fahrenheit, 26, 20, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    
    
    const unsigned char* weatherIcon;

    //https://openweathermap.org/weather-conditions
    if(weatherConditionCode > 801){//Cloudy
    weatherIcon = cloudy;
    }else if(weatherConditionCode == 801){//Few Clouds
    weatherIcon = cloudsun;
    }else if(weatherConditionCode == 800){//Clear
    weatherIcon = sunny;
    }else if(weatherConditionCode >=700){//Atmosphere
    weatherIcon = atmosphere;
    }else if(weatherConditionCode >=600){//Snow
    weatherIcon = snow;
    }else if(weatherConditionCode >=500){//Rain
    weatherIcon = rain;
    }else if(weatherConditionCode >=300){//Drizzle
    weatherIcon = drizzle;
    }else if(weatherConditionCode >=200){//Thunderstorm
    weatherIcon = thunderstorm;
    }else
    return;
    display.drawBitmap(145, 60, weatherIcon, WEATHER_ICON_WIDTH, WEATHER_ICON_HEIGHT, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
}

void WatchyChurchYear::drawWeekName(){
  display.setFont(&DSEG7_Classic_Regular_15);
  String weekName = " ";
  display.setCursor(5, 110);
  display.print(weekName);
}

void WatchyChurchYear::drawSaint(){
  String saintName = " ";
  display.setFont(&DSEG7_Classic_Regular_15);
  display.setCursor(5, 135);
  display.print(saintName);
}
