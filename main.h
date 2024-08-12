#ifndef MAIN_H
#define MAIN_H

#include <Watchy.h>
#include "vis.h"
#include "qr.h"
#include "sadbun.h"
#include "revolut.h"

RTC_DATA_ATTR int face = 0;
RTC_DATA_ATTR int faces_vars[6][2] = {{1,0},{1,0},{1,0},{1,0},{2,0},{1,0}}; //variants, current
int maxfaces = 2;

class KoyuWatchy : public Watchy {
  using Watchy::Watchy;
  public:
    void drawWatchFace();
    void drawWrapText(String text);
    void drawCentered(String text, int y2);
    void drawWeather();
    void drawBahn(int variant, float batt);
    virtual void handleButtonPress();
};

#include "bahnmod.h"

void KoyuWatchy::handleButtonPress() {
  if (guiState == WATCHFACE_STATE) {
    //Up and Down switch watch faces
    uint64_t wakeupBit = esp_sleep_get_ext1_wakeup_status();
    if (wakeupBit & UP_BTN_MASK) {
      face--;
      if (face < 0 ) { face = maxfaces; }
      RTC.read(currentTime);
      showWatchFace(false);
    }
    if (wakeupBit & DOWN_BTN_MASK) {
      face++;
      if (face > maxfaces ) { face = 0; }
      RTC.read(currentTime);
      showWatchFace(false);
    }
    if (wakeupBit & BACK_BTN_MASK) {
      face = 0;
      RTC.read(currentTime);
      showWatchFace(false);
      return;
    }
    if (wakeupBit & MENU_BTN_MASK) {
      Watchy::handleButtonPress();
      return;
    }
  } else {Watchy::handleButtonPress();}
}

void KoyuWatchy::drawWatchFace() {
  // ** UPDATE **
  //resets step counter at midnight everyday
  if (currentTime.Hour == 00 && currentTime.Minute == 00) {
    sensor.resetStepCounter();
  }

  // ** GET BATTERY **
  float batt = (getBatteryVoltage() - 3.3);
  if (batt > 1.00) { batt = 1; } else if (batt < 0) { batt = 0; }
  
  if (batt < 0.1) {
    display.fillScreen(GxEPD_WHITE);
    display.drawBitmap(0,0,sadbun,200,200,GxEPD_BLACK);
  } else {
    if (face == 0) {
      // ** DRAW WATCHFACE **
      drawBahn(faces_vars[face][1], batt);
    }
    if (face == 1) {
      display.fillScreen(GxEPD_WHITE);
      display.drawBitmap(0, 0, epd_bitmap_chart, 200, 200, GxEPD_BLACK);
    }
    if (face == 2) {
      display.fillScreen(GxEPD_WHITE);
      display.drawBitmap(0, 0, revolut_qr_code, 200, 200, GxEPD_BLACK);
    }
  }
}

#endif