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
#include "Wire.h"
#include "IRremote.hpp"
#define IR_RECEIVE_PIN 15

static const uint8_t PIN_MP3_TX = 26; // Connects to module's RX
static const uint8_t PIN_MP3_RX = 27; // Connects to module's TX
SoftwareSerial softwareSerial(PIN_MP3_RX, PIN_MP3_TX);
DFRobotDFPlayerMini player;

int playerPause = 0;

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

  // // Start communication with DFPlayer Mini
  delay(2000); // DFPlayer take about 2s to init the system
  Serial.println("System is running...");
  player.volume(30);  // Set volume of speaker
  player.randomAll(); // Play randomly all songs
  player.enableLoopAll();
}

void loop()
{
  // volumeState = analogRead(volume_pin);
  // volume = (float) volumeState/4095;
  // player.volume((int) (volume * 30) );
  // Serial.println(volume * 30);
  // delay(5000);
  // if(nextState == 1)
  // {
  //   player.next();
  //   nextState = 0;
  // }

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
      delay(500);

      break;

    case 0xBF40FF00: // NEXT SONG
      player.next();
      delay(500);
      break;
    case 0xBC43FF00: // pause/play

      playerPause = !playerPause;
      if (playerPause == 1)
      {
        player.pause();
      }
      else if (playerPause == 0)
      {
        player.start();
      }
      delay(500);

      break;
    case 0xF807FF00: // volume +
      player.volumeUp();
      delay(500);
      break;
    case 0xEA15FF00: // volumn -
      player.volumeDown();
      delay(500);
      break;

    default:
      break;
    }
    IrReceiver.resume();
  }
}
