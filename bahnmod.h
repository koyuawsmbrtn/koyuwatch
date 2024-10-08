#include "icons.h"

void KoyuWatchy::drawBahn(int variant, float batt) {


    // ** SETUP **

  int16_t  x1, y1, lasty;
  uint16_t w, h;
  String textstring;
  
  // ** DRAW **

  //drawbg
  display.fillScreen((variant == 0) ? GxEPD_WHITE : GxEPD_BLACK);
  display.fillRoundRect(2, 2, 196, 196, 8, GxEPD_BLACK);
  display.fillRoundRect(6, 6, 188, 188, 5, (variant == 0) ? GxEPD_WHITE : GxEPD_BLACK);

  display.setFont(&DIN_1451_Engschrift_Regular64pt7b);
  display.setTextColor((variant == 0) ? GxEPD_BLACK : GxEPD_WHITE);
  display.setTextWrap(false);

  //draw hours
  textstring = currentTime.Hour;
  display.getTextBounds(textstring, 0, 0, &x1, &y1, &w, &h);
  display.setCursor(183 - w, 100 - 5);
  display.print(textstring);

  //draw minutes
  if (currentTime.Minute < 10) {
    textstring = "0";
  } else {
    textstring = "";
  }
  textstring += currentTime.Minute;
  display.getTextBounds(textstring, 0, 0, &x1, &y1, &w, &h);
  display.setCursor(183 - w, 100 + 3 + h);
  display.print(textstring);

  // draw battery
  #ifdef ARDUINO_ESP32S3_DEV
  bool bat_enabled = true;
  if (bat_enabled) {
    float battc = batt + 0.31;
    if (battc > 1.00) {
      battc = 1;
    }
    display.fillRoundRect(16, 16, 34, 12, 4, (variant == 0) ? GxEPD_BLACK : GxEPD_WHITE);
    display.fillRoundRect(49, 20, 3, 4, 2, (variant == 0) ? GxEPD_BLACK : GxEPD_WHITE);
    display.fillRoundRect(18, 18, 30, 8, 3, (variant == 0) ? GxEPD_WHITE : GxEPD_BLACK);
    if (battc > 0) {
      display.fillRoundRect(20, 20, 26 * battc, 4, 2, (variant == 0) ? GxEPD_BLACK : GxEPD_WHITE);
    }
    if(USB_PLUGGED_IN){
      display.drawBitmap(56, 12, charge, 16, 18, (variant == 0) ? GxEPD_BLACK : GxEPD_WHITE);
    } else {
      display.setFont(&DIN_1451_Engschrift_Regular12pt7b);
      int battp = battc * 100;
      battp = (int)battp;
      textstring = String(battp)+"%";
      display.getTextBounds(textstring, 0, 0, &x1, &y1, &w, &h);
      display.setCursor(56, 12 + h);
      display.print(textstring);
    }
  } else {
    if(USB_PLUGGED_IN){
      display.drawBitmap(16, 12, charge, 16, 18, (variant == 0) ? GxEPD_BLACK : GxEPD_WHITE);
    }
  }
  #endif
  
   
  lasty = 200 - 16;

  //draw weather
  weatherData currentWeather = getWeatherData();
  int8_t temperature = currentWeather.temperature;
  int16_t weatherConditionCode = currentWeather.weatherConditionCode;
  if (WIFI_CONFIGURED) {
    textstring = String(temperature)+" °C";
  } else {
    temperature = temperature / 1.5;
    temperature = (int)temperature;
    textstring = String(temperature) + " °C";
  }
  display.setFont(&DIN_1451_Engschrift_Regular12pt7b);
  display.getTextBounds(textstring, 0, 0, &x1, &y1, &w, &h);
  display.fillRoundRect(16, lasty - h - 2, w + 7, h + 4, 2, (variant == 0) ? GxEPD_BLACK : GxEPD_WHITE);
  display.setCursor(19, lasty - 3);
  display.setTextColor((variant == 0) ? GxEPD_WHITE : GxEPD_BLACK);
  display.print(textstring);
  display.setTextColor((variant == 0) ? GxEPD_BLACK : GxEPD_WHITE);
  lasty += -8 - h;

  // draw year
  textstring = currentTime.Year + 1970;
  display.getTextBounds(textstring, 0, 0, &x1, &y1, &w, &h);
  display.setCursor(16, lasty);
  display.print(textstring);
  lasty += -20;

  // draw date
  textstring = monthShortStr(currentTime.Month);
  textstring += " ";
  textstring += currentTime.Day;
  display.getTextBounds(textstring, 0, 0, &x1, &y1, &w, &h);
  display.setCursor(16, lasty);
  display.print(textstring);
  lasty += -20;

  // draw day
  textstring = dayStr(currentTime.Wday);
  display.getTextBounds(textstring, 0, 0, &x1, &y1, &w, &h);
  display.setCursor(16, lasty);
  display.print(textstring);
  lasty += -40;

  // draw watch name
  textstring = "Watchy";
  display.getTextBounds(textstring, 0, 0, &x1, &y1, &w, &h);
  display.setCursor(16, lasty);
  display.print(textstring);
  lasty += -20;
  textstring = "Leonie's";
  display.getTextBounds(textstring, 0, 0, &x1, &y1, &w, &h);
  display.setCursor(16, lasty);
  display.print(textstring);
  lasty += -20;

}
