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
    drawDate();
    drawSteps();
    drawWeather();
    drawBattery();
    display.drawBitmap(120, 77, WIFI_CONFIGURED ? wifi : wifioff, 26, 18, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    if(BLE_CONFIGURED){
        display.drawBitmap(100, 75, bluetooth, 13, 21, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    }
}

void WatchyChurchYear::drawTime(){
    display.setFont(&DSEG7_Classic_Bold_53);
    display.setCursor(5, 53+5);
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
}

void WatchyChurchYear::drawDate(){
    display.setFont(&Seven_Segment10pt7b);

    int16_t  x1, y1;
    uint16_t w, h;

    
    // Get date of week
    String dayOfWeek = dayStr(currentTime.Wday);
    // Get Year
    String dateYear = String(tmYearToCalendar(currentTime.Year));
    // Get Month
    String dateMonth;
    if (currentTime.Month < 10)
    {
      dateMonth = "0" + currentTime.Month;
    } else
    {
      dateMonth = currentTime.Month;
    }
    

    // Build date
    String dateString = dayOfWeek.substring(0,3) + " " + dateYear + "-" + dateMonth; 


    char buffer [100];
    sprintf(buffer, "%s %d-%s-%d", dayOfWeek.substring(0,3), tmYearToY2k(currentTime.Year),currentTime.Month, currentTime.Day);
    // Display date
    //display.printf("a");
    display.printf("%s", buffer);
    
    

    String month = monthShortStr(currentTime.Month);
    //display.getTextBounds(month, 60, 110, &x1, &y1, &w, &h);
    //display.setCursor(85 - w, 110);
    //display.println(month);

    //display.setFont(&DSEG7_Classic_Bold_25);
    //display.setCursor(5, 120);
    //if(currentTime.Day < 10){
    //display.print("0");
    //}
    //display.println(currentTime.Day);
    //display.setCursor(5, 150);
    //display.println(tmYearToCalendar(currentTime.Year));// offset from 1970, since year is stored in uint8_t
}

void WatchyChurchYear::drawSteps(){
    // reset step counter at midnight
    if (currentTime.Hour == 0 && currentTime.Minute == 0){
      sensor.resetStepCounter();
    }
    uint32_t stepCount = sensor.getCounter();
    display.drawBitmap(10, 165, steps, 19, 23, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    display.setCursor(35, 190);
    display.println(stepCount);
}

void WatchyChurchYear::drawBattery(){
    display.drawBitmap(154, 73, battery, 37, 21, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
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

    for(int8_t batterySegments = 0; batterySegments < batteryLevel; batterySegments++){
        display.fillRect(159 + (batterySegments * BATTERY_SEGMENT_SPACING), 78, BATTERY_SEGMENT_WIDTH, BATTERY_SEGMENT_HEIGHT, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    }
}

void WatchyChurchYear::drawWeather(){

    //weatherData currentWeather = getWeatherAndDateData();
    weatherData currentWeather = getWeatherData();

    int8_t temperature = currentWeather.temperature;
    int16_t weatherConditionCode = currentWeather.weatherConditionCode;

    display.setFont(&DSEG7_Classic_Regular_39);
    int16_t  x1, y1;
    uint16_t w, h;
    display.getTextBounds(String(temperature), 0, 0, &x1, &y1, &w, &h);
    if(159 - w - x1 > 87){
        display.setCursor(159 - w - x1, 150);
    }else{
        display.setFont(&DSEG7_Classic_Bold_25);
        display.getTextBounds(String(temperature), 0, 0, &x1, &y1, &w, &h);
        display.setCursor(159 - w - x1, 136);
    }
    display.println(temperature);
    display.drawBitmap(165, 110, currentWeather.isMetric ? celsius : fahrenheit, 26, 20, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
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
    display.drawBitmap(145, 158, weatherIcon, WEATHER_ICON_WIDTH, WEATHER_ICON_HEIGHT, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
}


// Added my own version of the getWeatherData
// Let's call it something else getWeatherAndDateData

//weatherData WatchyChurchYear::getWeatherAndDateData(){
//    if(weatherIntervalCounter >= WEATHER_UPDATE_INTERVAL){ //only update if WEATHER_UPDATE_INTERVAL has elapsed i.e. 30 minutes
//        if(connectWiFi()){//Use Weather API for live data if WiFi is connected 
//            // Get step counter value
//            uint32_t stepCount = sensor.getCounter();
//
//            HTTPClient http;
//            http.setConnectTimeout(3000);//3 second max timeout
//            String weatherQueryURL = AZURE_FUNCTION_URL + "&steps=" + String(stepCount);
//            http.begin(weatherQueryURL.c_str());
//            int httpResponseCode = http.GET();
//            if(httpResponseCode == 200) {
//                // Old weather code
//                String payload = http.getString();
//                JSONVar responseObject = JSON.parse(payload);
//                currentWeather.temperature = int(responseObject["weather"]["temperature"]);
//                currentWeather.weatherConditionCode = int(responseObject["weather"]["weatherId"]);
//                // My code copied for time set
//                const time_t FUDGE(10);//fudge factor to allow for upload time, etc. (seconds, YMMV)
//                tmElements_t tm;
//                tm.Month = int(responseObject["dateTime"]["month"]);
//                tm.Day = int(responseObject["dateTime"]["day"]);
//                tm.Year = int(responseObject["dateTime"]["year"]) - YEAR_OFFSET;
//                tm.Hour = int(responseObject["dateTime"]["hour"]);
//                tm.Minute = int(responseObject["dateTime"]["minute"]);
//                tm.Second = 0;
//
//                time_t t = makeTime(tm) + FUDGE;
//                RTC.set(t);   
//            }else{
//                //http error
//            }
//            http.end();
//            //turn off radios
//            WiFi.mode(WIFI_OFF);
//            btStop();
//        }else{//No WiFi, use RTC Temperature
//            uint8_t temperature = RTC.temperature() / 4; //celsius
//            if(strcmp(TEMP_UNIT, "imperial") == 0){
//                temperature = temperature * 9. / 5. + 32.; //fahrenheit
//            }
//            currentWeather.temperature = temperature;
//            currentWeather.weatherConditionCode = 800;
//        }
//        weatherIntervalCounter = 0;
//    }else{
//        weatherIntervalCounter++;
//    }
//    // TS inserting code to reset steps there
//    tmElements_t tm;
//    if (tm.Hour < 3)
//    {
//        sensor.resetStepCounter();
//    }
//    return currentWeather;
//}
