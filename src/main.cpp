//----Set volume----
//  myDFPlayer.volume(10);  //Set volume value (0~30).
// myDFPlayer.volumeUp(); //Volume Up
//  myDFPlayer.volumeDown(); //Volume Down

//----Set different EQ----
//  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
//  myDFPlayer.EQ(DFPLAYER_EQ_POP);
//  myDFPlayer.EQ(DFPLAYER_EQ_ROCK);
//  myDFPlayer.EQ(DFPLAYER_EQ_JAZZ);
//  myDFPlayer.EQ(DFPLAYER_EQ_CLASSIC);
//  myDFPlayer.EQ(DFPLAYER_EQ_BASS);

//----Thiết lặp sử dụng thẻ SD----
//  myDFPlayer.outputDevice(DFPLAYER_DEVICE_U_DISK);
//  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
//  myDFPlayer.outputDevice(DFPLAYER_DEVICE_AUX);
//  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SLEEP);
//  myDFPlayer.outputDevice(DFPLAYER_DEVICE_FLASH);

//----Mp3 control----
//  myDFPlayer.sleep();     //sleep
//  myDFPlayer.reset();     //Reset the module
//  myDFPlayer.enableDAC();  //Kích hoạt On-chip DAC
//  myDFPlayer.disableDAC();  //Tắt On-chip DAC
//  myDFPlayer.outputSetting(true, 15); //Cài đặt output, kích hoạt output và cài đặt mức đầu ra là 15

//----Mp3 play----
//  myDFPlayer.next();  //Mở bài mp3 tiếp theo
//
//  myDFPlayer.previous();  //Mở bài mp3 trước
//
//  myDFPlayer.play(1);  //Mở bài mp3 đầu tiên
//
//  myDFPlayer.loop(1);  //Dừng bài mp3 đầu tiên
//
//  myDFPlayer.pause();  //tạm dừng mp3
//
//  myDFPlayer.start();  //Bắt đầu mp3 từ trạng thái tạm dừng
//
//  myDFPlayer.playFolder(15, 4);  //Mở 1 bài mp3 cụ thể trong SD:/15/004.mp3; tên Folder (1~99); tên File (1~255)
//
//  myDFPlayer.enableLoopAll(); //Lặp lại tất cả các file mp3
//
//  myDFPlayer.disableLoopAll(); //Dừng tất cả các file mp3
//
//  myDFPlayer.playMp3Folder(4); //Mở 1 bài mp3 cụ thể trong folder mp3 trong thẻ SD:/MP3/0004.mp3; Tên File(0~65535)
//
//  myDFPlayer.advertise(3); //Giới thiệu 1 bài mp3 cụ thể trong SD:/ADVERT/0003.mp3; Tên File (0~65535)
//
//  myDFPlayer.stopAdvertise(); //Dừng giới thiệu
//
//  myDFPlayer.playLargeFolder(2, 999); //Mở 1 bài mp3 cụ thể trong thẻ SD:/02/004.mp3; Tên Folder (1~10); tên File (1~1000)
//
//  myDFPlayer.loopFolder(5); //Lặp tất cả các file mp3 tron thư mục SD:/05.
//
//  myDFPlayer.randomAll(); //Random tất cả các bài mp3
//
//  myDFPlayer.enableLoop(); //bật vòng loop
//
//  myDFPlayer.disableLoop(); //tắt loop
//
//  //----Read imformation----
//  Serial.println(myDFPlayer.readState()); //đọc trạng thái mp3 hiện tại
//  Serial.println(myDFPlayer.readVolume()); //đọc giá trị volume hiện tại
//  Serial.println(myDFPlayer.readEQ()); //đọc cài đặt EQ
//  Serial.println(myDFPlayer.readFileCounts()); //Đọc tất cả các file có trong thẻ nhớ
//  Serial.println(myDFPlayer.readCurrentFileNumber()); //Đọc sô các file phát hiện tại
//  Serial.println(myDFPlayer.readFileCountsInFolder(3)); //Đọc số file trong folder SD:/03

#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include "IRremote.hpp"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define IR_RECEIVE_PIN 34
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_ADDRESS 0x3C
#define OLED_RESET -1

static const uint8_t PIN_MP3_TX = 26; // Connects to module's RX
static const uint8_t PIN_MP3_RX = 27; // Connects to module's TX
SoftwareSerial softwareSerial(PIN_MP3_RX, PIN_MP3_TX);
DFRobotDFPlayerMini player;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int isPause = 0;
int stt = 1; // stt bai hat

int page = 0; // page 0 la setting, page 1 la track
int vol = 0;  // am luong
int EQ = 3;   // ko biet EQ la gi

// OLED function

void drawSetting()
{

  display.setCursor(40, 0);
  display.print("Setting");
  display.setCursor(0, 20);
  display.print("Volume");
  display.setCursor(100, 20);
  display.print(player.readEQ());
  display.setCursor(0, 40);
  display.print("EQ");
  display.setCursor(100, 40);
  display.print(player.readVolume());
}
void drawInforTrack()
{
  display.setCursor(30, 20);
  display.print("Track: ");
  display.print(player.readFileCounts());
  display.print(" / ");
  display.print(player.readCurrentFileNumber());
}
void drawNext()
{
  int16_t x = 91;     // X coordinate of the top-left corner
  int16_t y = 40;     // Y coordinate of the top-left corner
  int16_t width = 4;  // Width of the rectangle
  int16_t height = 9; // Height of the rectangle
  display.fillRect(x, y, width, height, SSD1306_WHITE);
  int16_t x0 = 86, y0 = 40; // Top vertex (centered at the top)
  int16_t x1 = 86, y1 = 48; // Bottom-left vertex
  int16_t x2 = 91, y2 = 44; // Bottom-right vertex
  display.fillTriangle(x0, y0, x1, y1, x2, y2, SSD1306_WHITE);
}
void drawPre()
{
  int16_t x = 33;     // X coordinate of the top-left corner
  int16_t y = 40;     // Y coordinate of the top-left corner
  int16_t width = 4;  // Width of the rectangle
  int16_t height = 9; // Height of the rectangle
  display.fillRect(x, y, width, height, SSD1306_WHITE);
  int16_t x0 = 42, y0 = 40; // Top vertex (centered at the top)
  int16_t x2 = 37, y1 = 48; // Bottom-left vertex
  int16_t x1 = 42, y2 = 44; // Bottom-right vertex
  display.fillTriangle(x0, y0, x1, y1, x2, y2, SSD1306_WHITE);
}
void drawPause()
{
  int16_t x = 67;      // X coordinate of the top-left corner
  int16_t y = 37;      // Y coordinate of the top-left corner
  int16_t width = 6;   // Width of the rectangle
  int16_t height = 17; // Height of the rectangle
  display.fillRect(x, y, width, height, SSD1306_WHITE);
  int16_t x1 = 55;      // X coordinate of the top-left corner
  int16_t y1 = 37;      // Y coordinate of the top-left corner
  int16_t width1 = 6;   // Width of the rectangle
  int16_t height1 = 17; // Height of the rectangle
  display.fillRect(x1, y1, width1, height1, SSD1306_WHITE);
}
void drawRun()
{
  int16_t x0 = 57, y0 = 37; // Top vertex (centered at the top)
  int16_t x1 = 57, y1 = 51; // Bottom-left vertex
  int16_t x2 = 71, y2 = 44; // Bottom-right vertex
  display.fillTriangle(x0, y0, x1, y1, x2, y2, SSD1306_WHITE);
}
void drawTrack()
{
  if (isPause == 0)
  {
    drawPause();
  }
  else
  {
    drawRun();
  }
  drawInforTrack();
  drawNext();
  drawPre();
}

void setup()
{

  Serial.begin(9600);
  // Software serial initialize ...
  softwareSerial.begin(9600);
  // IR remote initialize ...
  IrReceiver.begin(IR_RECEIVE_PIN);
  // DFPlayer initialize ...
  while (!player.begin(softwareSerial))
  {
    Serial.println("Connecting to DFPlayer Mini failed!");
  }

  // OLED peripherals
  pinMode(35, INPUT);
  pinMode(32, INPUT);
  pinMode(33, INPUT);
  pinMode(25, INPUT);
  pinMode(5, INPUT);
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  drawSetting();
  display.display();
  // End OLED setup

  // // Start communication with DFPlayer Mini
  delay(2000); // DFPlayer take about 2s to init the system
  Serial.println("System is running...");
  player.volume(25); // Set volume of speaker
  player.enableDAC();
  player.randomAll(); // Play randomly all songs
}

void loop()
{
  Serial.begin(9600);
  if (digitalRead(5) == 1 && isPause == 0)
  {
    Serial.println(" play ");
    player.randomAll(); // Play randomly all songs
  }
  if (digitalRead(35) == 0)
    page = !page;
  if (digitalRead(32) == 0)
  {
    isPause = !isPause;
    if (isPause == 1)
    {
      player.pause();
    }
    else if (isPause == 0)
      player.start();
  }
  if (digitalRead(33) == 0)
  {
    delay(100);
    player.previous();
  }
  if (digitalRead(25) == 0)
  {
    delay(100);
    player.next();
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  if (page == 1)
  {
    display.clearDisplay();
    drawSetting();
    // display.display();
  }
  else
  {
    display.clearDisplay();
    drawTrack();
  }

  if (IrReceiver.decode())
  {
    uint64_t command = IrReceiver.decodedIRData.decodedRawData;
    Serial.print("0x");
    Serial.println(command, HEX);
    delay(100);
    switch (command)
    {
    case 0xBB44FF00: // PREV SONG
      player.previous();

      break;

    case 0xBF40FF00: // NEXT SONG
      player.next();

      break;
    case 0xBC43FF00: // pause/play

      isPause = !isPause;
      if (isPause == 1)
      {
        player.pause();
      }
      else if (isPause == 0)
        player.start();

      break;
    case 0xF807FF00: // volume -
      player.volumeDown();

      break;
    case 0xEA15FF00: // volumn +
      player.volumeUp();

      break;
    case 0xE916FF00:
      page = !page;
      break;
    default:
      break;
    }
    IrReceiver.resume();
  }
  display.display();
  delay(100);
}
