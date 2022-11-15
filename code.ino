/*
AccelStepper stepper = AccelStepper(연결방식, STEP핀, DIR핀);
stepper.setMaxSpeed(1000); //최대속도
stepper.setCurrentPosition(0); //현재 스탭을 설정
stepper.currentPosition(); //현재 스탭을 반환
stepper.setSpeed(200); //스탭모터의 방향과 속도를 설정
stepper.runSpeed(); //스탭모터 작동(고정속도)
stepper.setAcceleration(2000); //가속량을 설정함
stepper.moveTo(1000); //목표스탭량을 설정함
stepper.runToPosition(); //가속도를 붙혀서 스탭모터제어
*/
double Distance, Duration;//초음파센서에서 거리측정을 위한 변수
#include <AccelStepper.h>//모터 가감속 라이브러리
#include <Wire.h>//led 출력을 위한 라이브러리
#include <LiquidCrystal_I2C.h>//led 출력을 위한 라이브러리, 다운필요(해당 lcd를 위한 라이브러리 찾기)
#define mtr1Pin1  22     // IN1 driver 1
#define mtr1Pin2  23    // IN2 driver 1
#define mtr1Pin3  24     // IN3 driver 1
#define mtr1Pin4  25     // IN4 driver 1 : 판, 프레스 이동 모터 추가필요

#define mtr4Pin1  34     // IN1 driver 4
#define mtr4Pin2  35    // IN2 driver 4
#define mtr4Pin3  36     // IN3 driver 4
#define mtr4Pin4  37    // IN4 driver 4 : 문 개폐 모터

//dc모터 추가 필요

int inputPin=7;//인체센서 핀
int pirState=LOW;//인체센서 초기상태
int val1=0;//인체감지센서 상태
int echoPin=;//내부 초음파 센서(프레스 작동, 이동)//핀 어떤걸 쓸건지 결정필요
int trigPin=;//내부 초음파 센서(프레스 작동, 이동)//핀 어떤걸 쓸건지 결정필요
int Ebtn=LOW;//비상버튼
int btn=LOW;//봉투버튼//버튼 어떤거 쓸건지 확인필요: 3버튼이나 2버튼
AccelStepper stepper1 = AccelStepper(4, mtr1Pin1, mtr1Pin2, mtr1Pin3, mtr1Pin4);

void setup() {
  pinMode(inputPin,INPUT);
  pinMode(echoPin,INPUT);
  pinMode(trigPin,OUTPUT);
  lcd.init(), lcd.backlight();                 // lcd 초기화, 백라이트 켜기 !
  lcd.setCursor(0,0),lcd.print("Distance:");//lcd에 표시하기
  Serial.begin(9600);//시리얼모니터로 결과보기
  stepper1.setMaxSpeed(200); //
  stepper1.setAcceleration(50); //50~300사이에서 적당한값 찾기!
  stepper1.setCurrentPosition(0); //현재 스탭을 설정
  }

void loop() {
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);              // 트리거 단자 안정화 시간?
  digitalWrite(triggerPin, HIGH);    // 트리거 단자 10㎲ 동안 HIGH로 전환
  delayMicroseconds(10);            // 바로 다음부터 초음파가 발사됨
  digitalWrite(triggerPin, LOW);     // 트리거 단자 LOW로 전환
  Duration = pulseIn(echoPin, HIGH, 100000);     // 0.1초로 타임아웃 설정
  Distance = Duration*0.0172; //distance는 현재 cm 상태, %로 바꿔줘야한다
  if(Duration == 0){
//Serial.print("__Duration__ ="), Serial.println(Duration);
  lcd.clear(),lcd.print("..too close!");
  lcd.setCursor(3,1),lcd.print(":");
  lcd.setCursor(5,1),lcd.print(" __Er__");
  delay(600);
  lcd.clear(),lcd.print("Distance:");
} 
  else 
    lcdDisplay(Distance);
//Serial.print("_Dist, _Duration =");
//Serial.print(Distance), Serial.print(", "), Serial.println(Duration);
  delay(600);   //다음 측정까지 0.6초 간격을 둠
  
  if (Ebtn==HIGH){
    stepper1.runSpeed(0);
    }//dc모터, 개폐를 제외한 모든 모터 정지
    
  val1=digitalRead(inputPin);
  if(val1==HIGH)
  {
    Serial.println("문이 열립니다.");
    stepper4.moveTo(2000); //목표스탭량을 설정함
    stepper4.runToPosition(); //가속도를 붙혀서 스탭모터제어
    Serial.println("문이 닫힙니다.");
    delay(3000);
  }//인체감지되면 문열리기
  else{
    stepper4.moveTo(0);
    }//문 정지하기
  //밑의 코드는 모터가 1-2-3-3-2-1로 구동하는 코드 센서X 그냥 구동코드
  //시계방향으로 10바퀴 회전
  
  if(d>90){
  Serial.println("시계방향으로 10바퀴 회전 시작!");
  stepper1.moveTo(2000); //목표스탭량을 설정함
  stepper1.runToPosition(); //가속도를 붙혀서 스탭모터제어
  Serial.println("시계방향으로 10바퀴 회전 종료!");
  delay(3000);
  }//쓰레기 센서가 90%이상이면 이동
  
  if(btn==HIGH){  
  Serial.println("반시계방향으로 10바퀴 회전 시작!");
  stepper1.moveTo(-2000); //목표스탭량을 설정함
  stepper1.runToPosition(); //가속도를 붙혀서 스탭모터제어
  Serial.println("반시계방향으로 10바퀴 회전 종료!");
  delay(3000);
  }//봉투버튼 눌리면 정상위치로 이동
  
  if(d>30 && d<85)
  {
    if(){}//토크센서반영해서 작동
  }//프레스작동
}

void lcdDisplay(double d) {
//Serial.print("_dist="), Serial.println(d); // 디버깅 후 Serial 기능들 정지시킴
  lcd.setCursor(6,1), lcd.print(d), lcd.print(" %"); }
