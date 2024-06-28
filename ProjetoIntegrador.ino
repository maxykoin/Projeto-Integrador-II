#include <Ultrasonic.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

int btnL = 2;
int btnD = 3;
int motorH = 4;
int motorA = 5;
int pneuA; // A1 
int pneuV; // A2 

int s0 = 6;
int s1 = 7;
int s2 = 8;
int s3 = 9;
int out = 10;

bool estado;

int contA = 0;
int contV = 0;
int contC = 0;

Ultrasonic ultrasonic(13, 12);
LiquidCrystal_I2C lcd(0x27,16,2);

void setup(){
  estado = false;

  Serial.begin(9600);
  lcd.init();
  lcd.backlight();

  pinMode(btnL, INPUT_PULLUP);
  pinMode(btnD, INPUT_PULLUP);
  pinMode(motorH, OUTPUT);
  pinMode(motorA, OUTPUT);
  pinMode(pneuA, OUTPUT);
  pinMode(pneuV, OUTPUT);
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
  pinMode(out, INPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);

  digitalWrite(s0, HIGH);
  digitalWrite(s1, LOW);
}

void loop(){
  if(digitalRead(btnL) == LOW){
    estado = true;
  } else {
    analogWrite(motorH, 0);
    digitalWrite(motorA, LOW);
  }

  while(estado == true){
    ligado();
    if (digitalRead(btnD) == LOW){
      estado = false;
    }
  }
}

void ligado(){
  lcd.setCursor(0,0);
  lcd.print("Azul|");
  lcd.setCursor(0,1);
  lcd.print(contA);

  lcd.setCursor(5,0);
  lcd.print("Verm.|");
  lcd.setCursor(4,1);
  lcd.print("|");
  lcd.setCursor(5,1);
  lcd.print(contV);

  lcd.setCursor(11,0);
  lcd.print("Cinza");
  lcd.setCursor(10,1);
  lcd.print("|");
  lcd.setCursor(11,1);
  lcd.print(contC);

  int pwm = (analogRead(A0) / 4);
  int cm = ultrasonic.read();
  delay(100);
  Serial.println(pwm);
  analogWrite(motorH, pwm);
  analogWrite(motorA, 0);
  
  if(cm <= 5){ // definir cm quando a caixa estiver pronta
    analogWrite(motorH, 0);
    analogWrite(motorA, 0);
    analisaCor();
    delay(100);
    String cor = analisaCor();
    pneumatica(cor);
    delay(100);
    analogWrite(motorH, pwm);
    analogWrite(motorA, 0);
    delay(1000);
  }
}

String analisaCor(){
  String cor;
  int red = freqCor(0,0);
  int blue = freqCor(0, 1);
  int green = freqCor(1, 1);

  if(red >=60 && red <=100 && green >=60 && green <=100 && blue < red && blue < green){
    contC++;
    cor = "Cinza";
  }else if(blue < red && blue < green){
    contA++;
    cor = "Azul";
  }else if (red < blue && red < green){
    contV++;
    cor = "Vermelho";
  }
  Serial.println(cor);
  dispCor(cor);
  return cor;
}

int freqCor(int est2, int est3){
  int freqCor;
  
  digitalWrite(s2, est2);
  digitalWrite(s3, est3);

  if(digitalRead(out) == HIGH){
    freqCor = pulseIn(out, LOW);
  } else {
    freqCor = pulseIn(out, HIGH);
  }
  return freqCor;
}

void pneumatica(String cor){ 
  if(cor == "Azul"){
    Serial.println("Atuador Azul");
    digitalWrite(A1, HIGH);
    delay(1000);
  } else if (cor == "Vermelho"){
    Serial.println("Atuador Vermelho");
    digitalWrite(A1, HIGH);
    delay(1000);
  }
}

void dispCor(String cor){
  lcd.clear();
  lcd.setCursor(6, 0);
  lcd.print("Cor:");

  if(cor == "Azul"){
    lcd.setCursor(6, 1);
  } else if(cor == "Vermelho"){
    lcd.setCursor(4, 1);
  } else {
    lcd.setCursor(5, 1);
  }

  lcd.print(cor);
}