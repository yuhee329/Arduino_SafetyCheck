#include <MsTimer2.h>           //타이머
#include <SoftwareSerial.h>     //통신
#include <LiquidCrystal_I2C.h>  //LCD

byte state;  // 버튼눌렀을 때
long interval = 10;
long previousMillis = 0;

LiquidCrystal_I2C lcd(0x3F, 16, 2);  //LCD설정
SoftwareSerial P_Serial(6, 7);       //통신핀 설정
int sensorData;                      //수신할 데이터

void setup() {
  P_Serial.begin(9600);  //6,7핀을 아두이노끼리 시리얼
  Serial.begin(9600);    //시리얼모니터
  pinMode(13, OUTPUT);   //조도 값에 따라 LED를 ON/OFF
  digitalWrite(13, LOW);
  lcd.init();         //LCD초기화
  lcd.backlight();    //LCD켜기
  pinMode(3, INPUT);  //인터럽트 1번핀
  attachInterrupt(1, isr, RISING);
  Serial.begin(9600);
}

void loop() {
  String data = "";  //문자열 초기화
  while (P_Serial.available()) {
    data = P_Serial.readStringUntil('\n');  //수신한 문자열을 data변수로 지정
  }
  sensorData = data.toInt();
  Serial.print("in data: ");
  Serial.println(sensorData);  //모니터로 확인
  lcd.setCursor(0, 0);
  lcd.print(sensorData);
  if (sensorData <= 80)  //80이하일 때 13번핀 ON
  {
    digitalWrite(13, HIGH);
  } else if (sensorData < 250) {  //250미만일 때 13번핀 off
    digitalWrite(13, LOW);
  } else {  //200이상일 때 LCD화면 밑에 Warning표시
    digitalWrite(13, LOW);
    lcd.setCursor(0, 1);
    lcd.print("Warning");
  }
  delay(1000);
  lcd.clear();
}

void isr() {  //인터럽트 함수
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;  //채터링현상을 방지하기 위해
    {
      state = !state;
      P_Serial.println('1');  //버튼 눌렀을 때 '1'을 송신
    }
  }
}