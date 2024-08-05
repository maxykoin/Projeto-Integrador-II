#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

bool estado;
bool calibrar;

int btnL = 2;
int btnD = 3;

int mosfet = 4;
int sensor = 13;

int pneuA = 22; 
int pneuV = 24;

int s0 = 6;
int s1 = 7;
int s2 = 8;
int s3 = 9;
int out = 10;

int contA = 0;
int contV = 0;
int contC = 0;

int ledA;
int ledV;
int ledC;

int mediaR;
int mediaB;
int mediaG;

int mediaRR;
int mediaBR;
int mediaGR;

int mediaRB;
int mediaBB;
int mediaGB;

int mediaRC;
int mediaBC;
int mediaGC;

LiquidCrystal_I2C lcd(0x27,16,2);

void setup(){
  estado = false;
  calibrar = false;
  
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  
  pinMode(btnL, INPUT_PULLUP);
  pinMode(btnD, INPUT_PULLUP);
  pinMode(mosfet,OUTPUT);
  pinMode(sensor, INPUT);
  pinMode(pneuA, OUTPUT);
  pinMode(pneuV, OUTPUT);
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
  pinMode(out, INPUT);
  pinMode(ledA, OUTPUT);
  pinMode(ledV, OUTPUT);
  pinMode(ledC, OUTPUT);
  
  digitalWrite(s0, HIGH);
  digitalWrite(s1, LOW);

  while(!calibrar){
    lcd.setCursor(1,0);
    lcd.print("Aperte o botao");
    lcd.setCursor(2,1);
    lcd.print("para iniciar");
    
    if(digitalRead(btnL) == LOW){
      calibraSensor();
    }
  }
  
  lcd.clear();
}

void loop(){
  if(digitalRead(btnL) == LOW){
    lcd.clear();
    estado = true;
  } else {
    lcd.setCursor(1,0);
    lcd.print("Aperte o botao");
    lcd.setCursor(2,1);
    lcd.print("para ligar");
    digitalWrite(mosfet,LOW);
  }

  while(estado == true){
    ligado();
    if (digitalRead(btnD) == LOW){
      lcd.clear();
      contA = 0;
      contV = 0;
      contC = 0;
      estado = false;
    }
  }
}

void ligado(){
  Serial.println("loop");
  Serial.println(digitalRead(btnD));
  
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
  delay(100);
  analogWrite(mosfet, pwm);
  Serial.println(pwm);

  if(digitalRead(sensor) == LOW){
    digitalWrite(mosfet,LOW);
    String cor = analisaCor();
    delay(1000);
    dispCor(cor);
    pneumatica(cor);
    delay(100);
    analogWrite(mosfet,pwm);    
    delay(3000);
    digitalWrite(pneuA, LOW);
    digitalWrite(pneuV, LOW);
  }
}

String analisaCor(){
  String cor = "";
  
  while(cor == ""){
    int red = freqCor(0,0);
    int blue = freqCor(0, 1);
    int green = freqCor(1, 1);
    
    Serial.println(cor);
    if((blue>(mediaBC-10) && blue<(mediaBC+10)) && (red>(mediaRC-10) && red<(mediaRC+10)) && (green>(mediaGC-10) && green<(mediaGC+10))){
      contC++;
      cor = "Cinza";
    } else if((blue>(mediaBB-10) && blue<(mediaBB+10)) && (red>(mediaRB-10) && red<(mediaRB+10)) && (green >(mediaGB-10) && green<(mediaGB+10))){
      contA++;
      cor = "Azul";
    } else if ((blue>(mediaBR-10) && blue<(mediaBR+10)) && (red>(mediaRR-10) && red<(mediaRR+10)) && (green >(mediaGR-10) && green<(mediaGR+10))){
      contV++;
      cor = "Vermelho";
    } else {
      cor = "";
      lcd.clear();
      lcd.setCursor(6, 0);
      lcd.print("Cor:");
      lcd.setCursor(3,1);
      lcd.print("Analisando");
    }
  }

  Serial.println(cor);
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
    digitalWrite(pneuA, HIGH);
  } else if (cor == "Vermelho"){
    Serial.println("Atuador Vermelho");
    digitalWrite(pneuV, HIGH);
  } else {
    ;
  }
}

void dispCor(String cor){
  lcd.clear();
  lcd.setCursor(6, 0);
  lcd.print("Cor:");

  if(cor == "Azul"){
    lcd.setCursor(6, 1);
    digitalWrite(ledA, HIGH);
  } else if(cor == "Vermelho"){
    lcd.setCursor(4, 1);
    digitalWrite(ledV, HIGH);
  } else {
    digitalWrite(ledC, HIGH);
    lcd.setCursor(5, 1);
  }

  lcd.print(cor);
  delay(1500);
  digitalWrite(ledA, LOW);
  digitalWrite(ledV, LOW);
  digitalWrite(ledC, LOW);
  lcd.clear();
}

void calibraSensor(){
    int i;
    int controlLoop = true;
    int calibrouB = true;
    int calibrouR = true;
    int calibrouC = true;

    lcd.clear();
    lcd.setCursor(4,0);
    lcd.print("Vermelho");
    delay(1000);
    
    mediaR = mediaB = mediaG = 0;

    while(calibrouR){
        if(digitalRead(sensor) == LOW){
            for(i=0; i<20; i++){
                int red = freqCor(0,0);
                int blue = freqCor(0, 1);
                int green = freqCor(1, 1);
                mediaR +=red;
                mediaB +=blue;
                mediaG += green;
                delay(50);
            }
            calibrouR = false;
        }
    }
       
    mediaRR = mediaR/20;
    mediaBR = mediaB/20;
    mediaGR = mediaG/20;

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("R");
    lcd.setCursor(0,1);
    lcd.print(mediaRR);
    lcd.setCursor(5,0);
    lcd.print("G");
    lcd.setCursor(5,1);
    lcd.print(mediaGR);
    lcd.setCursor(11,0);
    lcd.print("B");
    lcd.setCursor(11,1);
    lcd.print(mediaBR);
    delay(5000);


    lcd.clear();    
    lcd.setCursor(6, 0);
    lcd.print("Azul");
    delay(1000);
    
    mediaR = mediaB = mediaG = 0;
    
    while(calibrouB){
        if(digitalRead(sensor) == LOW){
            for(i=0; i<20; i++){
                int red = freqCor(0,0);
                int blue = freqCor(0, 1);
                int green = freqCor(1, 1);
                mediaR += red;
                mediaB += blue;
                mediaG += green;
                delay(50);
            }
            calibrouB = false;
        }
    }
       
    mediaRB = mediaR/20;
    mediaBB = mediaB/20;
    mediaGB = mediaG/20;
        
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("R");
    lcd.setCursor(0,1);
    lcd.print(mediaRB);
    lcd.setCursor(5,0);
    lcd.print("G");
    lcd.setCursor(5,1);
    lcd.print(mediaGB);
    lcd.setCursor(11,0);
    lcd.print("B");
    lcd.setCursor(11,1);
    lcd.print(mediaBB);
    delay(5000);

      
    lcd.clear();    
    lcd.setCursor(5,0);
    lcd.print("Cinza");
    delay(1000);
    
    mediaR = mediaB = mediaG = 0;
    
    while(calibrouC){
        if(digitalRead(sensor) == LOW){
            for(i=0; i<20; i++){
            int red = freqCor(0,0);
            int blue = freqCor(0, 1);
            int green = freqCor(1, 1);
            mediaR += red;
            mediaB += blue;
            mediaG += green;
            delay(50);
            }
            calibrouC = false;
        }
    }
       
    mediaRC = mediaR/20;
    mediaBC = mediaB/20;
    mediaGC = mediaG/20;
        
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("R");
    lcd.setCursor(0,1);
    lcd.print(mediaRC);
    lcd.setCursor(5,0);
    lcd.print("G");
    lcd.setCursor(5,1);
    lcd.print(mediaGC);
    lcd.setCursor(11,0);
    lcd.print("B");
    lcd.setCursor(11,1);
    lcd.print(mediaBC);
    delay(5000);
    lcd.clear();

    calibrar = true;
}
