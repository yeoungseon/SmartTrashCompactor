#include <AccelStepper.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define mtr1Pin1  22     // IN1 driver 1 (압축 모터)
#define mtr1Pin2  23    
#define mtr1Pin3  24     
#define mtr1Pin4  25     

#define mtr4Pin1  34     // IN1 driver 4 (문 개폐 모터)
#define mtr4Pin2  35    
#define mtr4Pin3  36     
#define mtr4Pin4  37     

int inputPin = 7;         // 인체 감지 센서 핀
int pirState = LOW;       // 인체 감지 센서 초기 상태
int val1 = 0;             // 인체 감지 센서 상태
int echoPin = 8;          // 초음파 센서 에코 핀
int trigPin = 9;          // 초음파 센서 트리거 핀
int Ebtn = LOW;           // 비상 버튼 상태
int btn = LOW;            // 봉투 버튼 상태
double Distance, Duration;

AccelStepper stepper1(4, mtr1Pin1, mtr1Pin2, mtr1Pin3, mtr1Pin4); // 압축 모터
AccelStepper stepper4(4, mtr4Pin1, mtr4Pin2, mtr4Pin3, mtr4Pin4); // 문 개폐 모터
LiquidCrystal_I2C lcd(0x27, 16, 2); // LCD 설정 (주소는 사용중인 LCD에 따라 다를 수 있음)

void setup() {
  pinMode(inputPin, INPUT);
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Distance:");

  stepper1.setMaxSpeed(200);
  stepper1.setAcceleration(50);
  stepper1.setCurrentPosition(0);

  stepper4.setMaxSpeed(100);
  stepper4.setAcceleration(50);
  stepper4.setCurrentPosition(0);
}

void loop() {
  // 초음파 센서로 거리 측정
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  Duration = pulseIn(echoPin, HIGH, 100000);
  Distance = Duration * 0.0172; // cm 단위 거리

  if (Duration == 0) {
    lcd.clear();
    lcd.print("..too close!");
    delay(600);
    lcd.clear();
    lcd.print("Distance:");
  } else {
    lcdDisplay(Distance);
  }

  delay(600);

  // 비상 버튼이 눌리면 모든 모터 정지
  if (Ebtn == HIGH) {
    stepper1.stop();
    stepper4.stop();
    return;
  }

  // 인체 감지 시 문 개폐 모터 작동
  val1 = digitalRead(inputPin);
  if (val1 == HIGH) {
    Serial.println("문이 열립니다.");
    stepper4.moveTo(2000);
    stepper4.runToPosition();
    Serial.println("문이 닫힙니다.");
    delay(3000);
    stepper4.moveTo(0); // 문 닫기
    stepper4.runToPosition();
  }

  // 쓰레기가 90% 이상 찼을 때 압축 모터 작동
  if (Distance < 10) { // 예: 쓰레기가 센서에 매우 근접한 경우
    Serial.println("압축 모터 작동 시작!");
    stepper1.moveTo(2000); // 압축 실행
    stepper1.runToPosition();
    Serial.println("압축 모터 작동 종료!");
    delay(3000);
    stepper1.moveTo(0); // 압축판 원래 위치로 이동
    stepper1.runToPosition();
  }

  // 봉투 버튼을 눌렀을 때 압축판을 원래 위치로 이동
  if (btn == HIGH) {  
    Serial.println("압축판 원위치로 이동 시작!");
    stepper1.moveTo(0);
    stepper1.runToPosition();
    Serial.println("압축판 원위치로 이동 완료!");
  }

  // 쓰레기 압축 중 과부하 감지 시 정지 (예: 토크 센서 사용)
  if (Distance < 5) { // 예: 압력이 임계값에 도달한 경우
    if (/* 토크 센서 판독값 조건 */) {
      Serial.println("압축 모터 정지 - 과부하 감지!");
      stepper1.stop();
    }
  }
}

void lcdDisplay(double d) {
  lcd.setCursor(6, 1);
  lcd.print(d);
  lcd.print(" cm");
}
