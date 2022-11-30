#include <SoftwareSerial.h>
#include "GodoxRemote.h"


// 핀 세팅
const int pinTx = 8;  // 블루투스 TX 연결 핀 번호
const int pinRx = 7;  // 블루투스 RX 연결 핀 번호
const int pinRF = 4;  // RF 송신기 연결 핀 번호

SoftwareSerial bluetooth(pinTx, pinRx); // 블루투스 송수신 핀 설정
GodoxRemote remote(pinRF);              // RF 송신 핀 설정

// 초기 조명 밝기
int lightValue = 65;

// 블루투스 수신 데이터
String rxDataString = "";


// 조명 밝기 Down 함수
void lightDown() {
  // 조명 밝기가 25% 이하일 때는 더이상 밝기 Down 불가능
  if (lightValue <= 25) {
    bluetooth_write(25);
    return;
  }

  remote.setBrightness(--lightValue);
  bluetooth_write(lightValue);
  return;
}

// 조명 밝기 Up 함수
void lightUp() {
  // 조명 밝기가 100% 이상일 때는 더이상 밝기 Up 불가능
  if (lightValue >= 100) {
    bluetooth_write(100);
    return;
  }

  remote.setBrightness(++lightValue);
  bluetooth_write(lightValue);
  return;
}

// 블루투스 전송 함수
void bluetooth_write(int p_lightValue) {
  String txDataString = String(p_lightValue);
  txDataString += '\r';
  bluetooth.println(txDataString);
}


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

    // 블루투스 수신 종료 코드
    if (rxData == 'c') {
      rxDataString = "";
      continue;
    }

    rxDataString += rxData;

    // 65를 수신하면 조명 밝기 65%로 초기화
    if (rxDataString == "65") {
      lightValue = 65;
      remote.setBrightness(lightValue);
      Serial.println(lightValue);
      delay(10);
      continue;
    }
    // 0을 수신하면 밝기 1% Down
    if (rxDataString == "0") {
      lightDown();
      Serial.println(lightValue);
      delay(10);
      continue;
    }
    // 1을 수신하면 밝기 1% Up
    if (rxDataString == "1") { 
      lightUp();
      Serial.println(lightValue);
      delay(10);
      continue;
    }
  }
}
