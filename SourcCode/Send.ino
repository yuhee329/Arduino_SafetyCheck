#include <SoftwareSerial.h>
SoftwareSerial P_Serial(6, 7);

int LUX = A0;  // analog A0에 조도연결
int LUX_ADC;   // 조도함수 선언
int interruptData;

void setup() {

  Serial.begin(9600);
  P_Serial.begin(9600);  //6,7
  pinMode(9, OUTPUT);    //부저 9핀
}
void loop() {

  LUX_ADC = analogRead(LUX);  //조도값 읽기

  Serial.print("Lux is ");
  Serial.println(LUX_ADC);    // 시리얼 모니터에 조도값 출력
  P_Serial.println(LUX_ADC);  //p_시리얼에 조도값 송신
  delay(1000);

  String data = "";
  while (P_Serial.available()) {
    data = P_Serial.readStringUntil('\n');
  }
  interruptData = data.toInt();
  Serial.println(interruptData);
  if (interruptData == 1)   //버튼(인터럽트)을 누를 시 1의 값을 받고 1초간 부저가 울리고 꺼진다.
  {
    digitalWrite(9, HIGH);
    delay(1000);
  } else {
    digitalWrite(9, LOW);
  }
}
