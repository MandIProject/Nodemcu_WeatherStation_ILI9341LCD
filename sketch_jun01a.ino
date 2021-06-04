#include <ArduinoHttpClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <TimeLib.h>
#include <SPI.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Fonts/FreeMonoBoldOblique12pt7b.h>
#include <Fonts/FreeSerif9pt7b.h>
#include "Weather.h"

#define TFT_DC D1
#define TFT_CS D2
#define DEFAULT_NAME "Connecting..."

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

void setup() 
{
  Serial.begin(9600); 
  tft.begin();
  tft.setRotation(2);
  tft.fillScreen(ILI9341_WHITE);
  connectWifi();
}

void loop() 
{
  tft.fillScreen(ILI9341_WHITE);
  blynk();
  openWeather();
  formatData();
}

void connectWifi()
{
  WiFi.begin(ssid, pass);
  
  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    tft.drawRGBBitmap(60,40,appIconBitmap,IMAGE_WIDTH,IMAGE_HEIGHT);
    tft.setFont(&FreeMonoBoldOblique12pt7b);
    tft.setCursor(40,250);
    tft.setTextColor(ILI9341_BLACK);
    tft.print(DEFAULT_NAME);
    delay(1000);
  }

  Serial.println("\nConnected");
  Serial.println(WiFi.localIP());
  
  delay(1000);
}

void formatData()
{
  DynamicJsonBuffer jsonBuff;
  JsonObject& root = jsonBuff.parseObject(jsonBuffer);
  
  unsigned long t = root["dt"];
  char Time[32];
  sprintf(Time, "%02d:%02d:%02d", hour(t), minute(t), second(t));
  strcat(Time, " UTC");
  char Date[32];
  sprintf(Date, "%02d:%02d:%02d", day(t), month(t), year(t));
  int Hour = (int)hour(t);

  if(Hour >= 0 && Hour <= 18)
    Hour = Hour + 5;
  else if(Hour > 18)
    Hour = Hour + 5 - 24;
  
  String temp = root["main"]["temp"];
  temp = temp + "°C";
  String feels_like = root["main"]["feels_like"]; 
  feels_like ="Feels:" + feels_like + "°C";
  String Humid = root["main"]["humidity"]; 
  Humid += "%";
  String cityName = root["name"];
  String windSpeed = root["wind"]["speed"]; 
  windSpeed += "Km/h";
  String weather = root["weather"][0]["main"];
  String weatherDescription = root["weather"][0]["description"];           

  Serial.print("\n");
  Serial.print(temp);
  Serial.print("\nCity: ");
  Serial.print(cityName);

  drawcurr_time_date(Time,Date);
  drawTemp_CityName(cityName,temp);
  drawfeels_like_humid(Humid,feels_like);
  drawWind_Weather(weather,windSpeed,weatherDescription);
  if(Hour >=0 && Hour <= 5)
    drawMoon();
  else if(Hour >=18)
    drawMoon();
  else if(Hour >=16 && Hour <18)
    drawEvening();
  else
    drawSun();
}

void drawSun()
{
  int16_t x1,y1;
  uint16_t w,h;
  tft.drawRGBBitmap(60,40,sunnyBitmap,IMAGE_WIDTH,IMAGE_HEIGHT);
  tft.setFont(&FreeMonoBoldOblique12pt7b);
  tft.getTextBounds("Morning time", 40, 200, &x1, &y1, &w, &h);
  tft.setCursor(40,200);
  tft.setTextColor(ILI9341_BLACK);
  tft.print("Morning time");
  delay(5000);
  tft.fillRect(x1,y1,w,h,ILI9341_WHITE);
  tft.drawRect(x1,y1,w,h,ILI9341_WHITE);
}

void drawEvening()
{
  int16_t x1,y1;
  uint16_t w,h;
  tft.drawRGBBitmap(60,40,eveningBitmap,IMAGE_WIDTH,IMAGE_HEIGHT);
  tft.setFont(&FreeMonoBoldOblique12pt7b);
  tft.getTextBounds("Evening time", 40, 200, &x1, &y1, &w, &h);
  tft.setCursor(40,200);
  tft.setTextColor(ILI9341_BLACK);
  tft.print("Evening time");
  delay(5000);
  tft.fillRect(x1,y1,w,h,ILI9341_WHITE);
  tft.drawRect(x1,y1,w,h,ILI9341_WHITE);
}

void drawMoon()
{
  int16_t x1,y1;
  uint16_t w,h;
  tft.drawRGBBitmap(60,40,moonBitmap,IMAGE_WIDTH,IMAGE_HEIGHT);
  tft.setFont(&FreeMonoBoldOblique12pt7b);
  tft.getTextBounds("Night time", 40, 200, &x1, &y1, &w, &h);
  tft.setCursor(40,200);
  tft.setTextColor(ILI9341_BLACK);
  tft.print("Night time");
  delay(5000);
  tft.fillRect(x1,y1,w,h,ILI9341_WHITE);
  tft.drawRect(x1,y1,w,h,ILI9341_WHITE);
}

void drawcurr_time_date(char curr_time[32], char curr_date[32])
{
  int16_t x1,y1,x2,y2;
  uint16_t w,h,w1,h1;

  tft.drawRGBBitmap(60,40,clockBitmap,IMAGE_WIDTH,IMAGE_HEIGHT);
  tft.setFont(&FreeMonoBoldOblique12pt7b);
  tft.getTextBounds(curr_time, 30, 200, &x1, &y1, &w, &h);
  tft.getTextBounds(curr_date, 50, 250, &x2, &y2, &w1, &h1);

  tft.setCursor(30,200);
  tft.setTextColor(ILI9341_BLACK);
  tft.print(curr_time);
  tft.setCursor(50,250);
  tft.setTextColor(ILI9341_BLACK);
  tft.print(curr_date);
  delay(5000);
  tft.fillRect(x1,y1,w,h,ILI9341_WHITE);
  tft.drawRect(x1,y1,w,h,ILI9341_WHITE);
  tft.fillRect(x2,y2,w1,h1,ILI9341_WHITE);
  tft.drawRect(x2,y2,w1,h1,ILI9341_WHITE);
}

void drawWind_Weather(String weather, String windSpeed, String weatherDescription)
{
  int16_t  x1, y1, x2, y2, x3, y3;
  uint16_t w, h, w1, h1, w2, h2;
  
  tft.drawRGBBitmap(60,40,windBitmap,IMAGE_WIDTH,IMAGE_HEIGHT);
  tft.setFont(&FreeMonoBoldOblique12pt7b);
  
  tft.getTextBounds(windSpeed, 10, 200, &x1, &y1, &w, &h);
  tft.getTextBounds(weather, 10, 240, &x2, &y2, &w1, &h1);
  tft.getTextBounds(weatherDescription, 10, 280, &x3, &y3, &w2, &h2);
  tft.setCursor(10,200);
  tft.setTextColor(ILI9341_BLACK);
  tft.print(windSpeed);
  tft.setCursor(10,240);
  tft.setTextColor(ILI9341_BLACK);
  tft.print(weather);
  tft.setCursor(10,280);
  tft.setTextColor(ILI9341_BLACK);
  tft.print(weatherDescription);
  delay(5000);
  tft.fillRect(x1,y1,w,h,ILI9341_WHITE);
  tft.drawRect(x1,y1,w,h,ILI9341_WHITE);
  tft.fillRect(x2,y2,w1,h1,ILI9341_WHITE);
  tft.drawRect(x2,y2,w1,h1,ILI9341_WHITE);
  tft.fillRect(x3,y3,w2,h2,ILI9341_WHITE);
  tft.drawRect(x3,y3,w2,h2,ILI9341_WHITE);
}

void drawfeels_like_humid(String Humid, String feels_like)
{
  int16_t  x1, y1, x2, y2;
  uint16_t w, h, w1, h1;
  
  tft.drawRGBBitmap(60,40,humidityBitmap,IMAGE_WIDTH,IMAGE_HEIGHT);
  tft.setFont(&FreeMonoBoldOblique12pt7b);
  
  tft.getTextBounds(Humid, 95, 200, &x1, &y1, &w, &h);
  tft.getTextBounds(feels_like, 40, 260, &x2, &y2, &w1, &h1);
  tft.setCursor(95,200);
  tft.setTextColor(ILI9341_BLACK);
  tft.print(Humid);
  tft.setCursor(40,260);
  tft.setTextColor(ILI9341_BLACK);
  tft.print(feels_like);
  delay(5000);
  tft.fillRect(x1,y1,w,h,ILI9341_WHITE);
  tft.drawRect(x1,y1,w,h,ILI9341_WHITE);
  tft.fillRect(x2,y2,w1,h1,ILI9341_WHITE);
  tft.drawRect(x2,y2,w1,h1,ILI9341_WHITE);
}

void drawTemp_CityName(String cityName, String temp)
{
  int16_t  x1, y1, x2, y2;
  uint16_t w, h, w1, h1;
  
  tft.drawRGBBitmap(60,40,dragonBitmap,IMAGE_WIDTH,IMAGE_HEIGHT);
  tft.setFont(&FreeMonoBoldOblique12pt7b);
  
  tft.getTextBounds(cityName, 50, 200, &x1, &y1, &w, &h);
  tft.getTextBounds(temp, 90, 260, &x2, &y2, &w1, &h1);
  tft.setCursor(50,200);
  tft.setTextColor(ILI9341_BLACK);
  tft.print(cityName);
  tft.setCursor(90,260);
  tft.setTextColor(ILI9341_BLACK);
  tft.print(temp);
  delay(5000);
  tft.fillRect(x1,y1,w,h,ILI9341_WHITE);
  tft.drawRect(x1,y1,w,h,ILI9341_WHITE);
  tft.fillRect(x2,y2,w1,h1,ILI9341_WHITE);
  tft.drawRect(x2,y2,w1,h1,ILI9341_WHITE);
}

void openWeather()
{
  WiFiClient wifi;
  HttpClient client = HttpClient(wifi, serverAddress2, 80);

  client.get(path);

  // read the status code and body of the response
  int statusCode = client.responseStatusCode();
  String response = client.responseBody();
  jsonBuffer = response;
}

void blynk()
{
  WiFiClient wifi;
  HttpClient client = HttpClient(wifi, serverAddress1, 80);
  
  client.get(String("/") +auth+ String("/get/V2"));

  // read the status code and body of the response
  int statusCode = client.responseStatusCode();
  String response = client.responseBody();
  response.remove(0,2);
  int responseLen = response.length();
  response.remove(responseLen-2,2);
  responseLen = response.length();

  char kPath[responseLen+1];
  response.toCharArray(kPath, responseLen+1);

  strcpy(path, kPath);
}
