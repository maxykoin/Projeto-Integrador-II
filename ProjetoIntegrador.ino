#include <Ultrasonic.h>
#include <LiquidCrystal.h>

int btnL = 2;
int btnD = 3;
int pot = A0;
int motorH = 4;
int motorA = 5;
int pneuA; // definir
int pneuV; // definir

int s0 = 6;
int s1 = 7;
int s2 = 8;
int s3 = 9;
int out = 10;

bool estado;

int contA = 0;
int contV = 0;
int contC = 0;

// LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
Ultrasonic ultrasonic(13, 12);
void setup(){
  Serial.begin(9600);
  // lcd.begin(16, 2);

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

  estado = false;
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
  // lcd.setCursor(3, 0);
  // contagem de peças de cada cor

  int pwm = 255;  //(analogRead(pot) / 1023) * 255;
  int cm = ultrasonic.read();
  analogWrite(motorH, pwm);
  analogWrite(motorA, 0);
    // só um if se a gente não ligar o motor no analisaCor()
    if(cm <= 5){ // definir cm quando a caixa estiver pronta
      Serial.println(cm);
      analogWrite(motorH, 0);
      analogWrite(motorA, 0);
      analisaCor();
      delay(1000);
    }
  String cor = analisaCor();
  pneumatica(cor);
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
  // dispCor(cor);
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

String pneumatica(String cor){ 
  if(cor == "Azul"){
    Serial.println("Atuador Azul");
    delay(1000);
  } else if (cor == "Vermelho"){
    Serial.println("Atuador Vermelho");
    delay(1000);
  }
}

/* void dispCor(char cor){
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Peça:");
  lcd.setCursor(3, 1);
  lcd.print(cor);
} */