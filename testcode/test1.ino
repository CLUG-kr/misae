#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#define DHTPIN A1
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

int redPin = 7;
int greenPin = 6;
int bluePin = 5;

int dust_sensor = A0;
float dust_value = 0;
float dustDensityug = 0;

double five_dust[50] = {0};
double recent_dust = 0, total_dust = 0;

int sensor_led = 12;
int sampling = 280;
int waiting = 40;
float stop_time = 9680;

byte humi[8] = {     // 물컵모양 출력
  0b00000,
  0b10001,
  0b10001,
  0b10001,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
};
 byte temp[8] = {     // 온도계 모양 출력
  0b00100,
  0b01010,
  0b01010,
  0b01010,
  0b01010,
  0b10001,
  0b11111,
  0b01110,
};
 byte char_temp[8] = {     // 온도 단위 출력
  0b10000,
  0b00110,
  0b01001,
  0b01000,
  0b01000,
  0b01000,
  0b01001,
  0b00110,
};

 byte cube[8] = {     // 세제곱
  0b11100,
  0b00100,
  0b11100,
  0b00100,
  0b11100,
  0b00000,
  0b00000,
  0b00000,
};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(sensor_led, OUTPUT);
  pinMode(4, OUTPUT);
  
  lcd.init();
  lcd.backlight();
  lcd.createChar(1, temp);         // 온도계모양 출력
  lcd.createChar(2, humi);          // 물컵 모양 출력
  lcd.createChar(3, char_temp);     // 온도 단위 출력
  lcd.createChar(4, cube);          // 세제곱 단위 출력

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int h = dht.readHumidity();
  int t = dht.readTemperature();
  
  digitalWrite(sensor_led, LOW);
  delayMicroseconds(sampling);

  dust_value = analogRead(dust_sensor);

  delayMicroseconds(waiting);

  digitalWrite(sensor_led, HIGH);
  delayMicroseconds(stop_time);
  
  recent_dust = (0.17 * (dust_value * (5.0 / 1024)) - 0.1) * 1000 * ;    // 미세먼지 값 계산
  five_dust[49] = recent_dust;   // 새로운 미세먼지 값 입력
  total_dust = five_dust[49];               // 5개의 미세먼지 값을 저장할 변수
  
  for(int i=0; i<49; i++)
  {
    total_dust += five_dust[i];
    five_dust[i] = five_dust[i+1];  // 0~4번째까지 미세먼지 값 저장을 위해 4번째 배열 비워주기
  }

  if(five_dust[0] != 0)
  {
    dustDensityug = total_dust / 50;
    Serial.print("Dust Density [ug/m^3]: ");            // 시리얼 모니터에 미세먼지 값 출력    
    Serial.println(dustDensityug);
  }                                   
  

  Serial.println(dust_value);
  
//  Serial.print("Fine Dust Density(ug/m^3): ");
//  Serial.println(dustDensityug);
//  Serial.print("humidity(%): ");
//  Serial.print(h);
//  Serial.print("  temparture(C): ");
//  Serial.println(t);

  lcd.setCursor(0,0);             // 1번째, 1라인  
  lcd.write(byte(1));             // 온도계 출력
  lcd.setCursor(2,0);             // 3번째, 1라인
  lcd.print((int)t);              // 온도 출력
  lcd.setCursor(5,0);             // 6번째 1라인
  lcd.write(byte(3));             // 온도 단위 출력
  
  lcd.setCursor(8,0);             // 9번째, 1라인
  lcd.write(byte(2));             // 물컵 출력
  lcd.setCursor(10,0);            // 11번째, 1라인
  lcd.print(h);                   // 습도 출력
  lcd.setCursor(13,0);            // 15번째, 1라인
  lcd.print("%");                 // % 출력
  
  lcd.setCursor(0,1);             // 1번째, 2라인
  lcd.print("F.D");               // fine dust 글자 출력
  lcdClear();
  lcd.setCursor(4,1);             // 6번째, 2라인
  lcd.print(dustDensityug);       // 미세먼지 출력
  lcd.setCursor(11,1);
  lcd.print("ug/m");
  lcd.setCursor(15,1);
  lcd.write(byte(4));

  if(dustDensityug <= 30.0){
    setColor(0,0,255);    // Blue
  } else if(30.0 < dustDensityug && dustDensityug <= 80.0){
    setColor(0,255,0);    // Green
  } else if(80.0 < dustDensityug && dustDensityug <= 150.0){
    setColor(192,128,0);  // Yellow
  } else{
    setColor(255,0,0);    // Red
  }
  delay(500);
  
}

void setColor(int red, int green, int blue)
{
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}

void lcdClear(){
  lcd.setCursor(3,1);
  lcd.print("        ");
}




