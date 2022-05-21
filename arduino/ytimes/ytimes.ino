#include <M5EPD.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include <NTPClient.h>

#include "ytimes_config.h"


#define ABAT 0
#define ATEM 1
#define AHUM 2
#define ATIME 3


M5EPD_Canvas canvas(&M5.EPD);


struct TextArea {
  char new_text[64], cur_text[64];
  int x, y, w, h, font_size;
  M5EPD_Canvas *canvas;

  void init(int x, int y, int w, int h, int font_size, M5EPD_Canvas *canvas)  {
    this->x = x, this->y = y, this->w = w, this->h = h;
    this->font_size = font_size;
    this->canvas = canvas;
    this->canvas->createRender(this->font_size, 256);
  }

  void updateText() {
    // if (strcmp(new_text, cur_text) == 0)
    //   return;
    canvas->setTextSize(font_size);
    canvas->drawString(new_text, x, y);
    // canvas->drawRect(x, y, w, h, 15);
    strcpy(cur_text, new_text);
    M5.EPD.WriteFullGram4bpp((uint8_t*)canvas->frameBuffer());
    M5.EPD.UpdateArea(x, y-1, w, h+2, UPDATE_MODE_GC16);
  }
};

TextArea areas[4];


void SyncTimeNTP() {
  WiFiUDP ntpUDP;
  NTPClient timeClient(ntpUDP, TIMEZONE_OFFSET);
  timeClient.begin();
  timeClient.update();
  time_t ttime = timeClient.getEpochTime();
  timeClient.end();

  tm *tmtime = localtime(&ttime);
  rtc_date_t rdate {
    static_cast<int8_t>(tmtime->tm_wday),
    static_cast<int8_t>(tmtime->tm_mon + 1),
    static_cast<int8_t>(tmtime->tm_mday),
    static_cast<int16_t>(tmtime->tm_year + 1900)
  };
  rtc_time_t rtime {
    static_cast<int8_t>(tmtime->tm_hour),
    static_cast<int8_t>(tmtime->tm_min),
    static_cast<int8_t>(tmtime->tm_sec)
  };
  M5.RTC.setDate(&rdate);
  M5.RTC.setTime(&rtime);
}


void UpdateBG() {
  canvas.drawJpgUrl(IMG_URL);
  canvas.pushCanvas(0,0,UPDATE_MODE_GC16);
}


void UpdateSHT30() {
  M5.SHT30.UpdateData();
  int tem = round(M5.SHT30.GetTemperature());
  int hum = round(M5.SHT30.GetRelHumidity());
  int bat = M5.getBatteryVoltage();

  sprintf(areas[ATEM].new_text, "%d", tem);
  sprintf(areas[AHUM].new_text, "%d", hum);

  // estimated battery level
  sprintf(areas[ABAT].new_text, "%3d", min(100, max(0, (bat-3200)/10)));
}


void UpdateTime() {
  rtc_time_t RTCtime;
  M5.RTC.getTime(&RTCtime);

  sprintf(
    areas[ATIME].new_text,
    "%02d:%02d",
    RTCtime.hour, RTCtime.min
  );
}


void setup() {
  M5.begin();
  M5.SHT30.Begin();
  SPIFFS.begin(true);
  M5.RTC.begin();

  M5.EPD.SetRotation(90);
  M5.EPD.Clear(true);

  canvas.loadFont("/iosevka-fixed-ss15-medium.ttf", SPIFFS);
  canvas.createCanvas(540, 960);
  areas[ATEM].init(36, 929, 12*2, 20, 24, &canvas);
  areas[AHUM].init(120, 929, 12*2, 20, 24, &canvas);
  areas[ABAT].init(442, 929, 12*3, 20, 24, &canvas);
  areas[ATIME].init(240, 929, 12*5, 20, 24, &canvas);

  WiFi.begin(WIFI_SSID, WIFI_PWD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}


void loop() {
  M5.update();
  UpdateBG();

  UpdateSHT30();
  areas[ATEM].updateText();
  areas[AHUM].updateText();
  areas[ABAT].updateText();

  SyncTimeNTP();
  UpdateTime();
  areas[ATIME].updateText();

  delay(10*1000);

  M5.shutdown(REFRESH_INTERVAL);
}
