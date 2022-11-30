#include <SoftwareSerial.h>
#include "GodoxRemote.h"


// 핀 세팅
const int pinTx = 8;  // 블루투스 TX 연결 핀 번호
const int pinRx = 7;  // 블루투스 RX 연결 핀 번호
const int pinRF = 4;  // RF 송신기 연결 핀 번호

SoftwareSerial bluetooth(pinTx, pinRx); // 블루투스 송수신 핀 설정
GodoxRemote remote(pinRF);              // RF 송신 핀 설정

// 초기 조명 밝기
long lightValue = 65;

// 블루투스 수신 데이터
String rxDataString = "";


void setup() {
  bluetooth.begin(9600);
  Serial.begin(9600);

  // 조명 채널과 그룹 세팅
  remote.setChannel(0); // 채널 1
  remote.setGroup(0);   // 그룹 A

  // 신호 반복 횟수 및 주기 세팅
  remote.setRepeatNum(2);
  remote.setRepeatDelay(25); 

  // 조명 초기 밝기 세팅
  remote.setBrightness(lightValue);
}

void loop() {
  while (bluetooth.available()) {
    char rxData = char(bluetooth.read());  // 블루투스 데이터 수신

    // 조명 밝기값 수신 완료
    if (rxData == 'c') {
      lightValue = rxDataString.toInt();
      remote.setBrightness(lightValue);

      rxDataString = "";
      delay(10);
      continue;
    }

    rxDataString += rxData;
    delay(10);
  }
}
