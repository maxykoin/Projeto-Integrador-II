#include <Ultrasonic.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

int btnL = 2;
int btnD = 3;
int motorH = 4;
int motorA = 5;
int pneuA = 14; 
int pneuV = 15;

int s0 = 6;
int s1 = 7;
int s2 = 8;
int s3 = 9;
int out = 10;

bool estado;

int contA = 0;
int contV = 0;
int contC = 0;

int mediaR;
int mediaB;
int mediaG;

int mediaRR;
int mediaBR;
int mediaGR;

int mediaRG;
int mediaBG;
int mediaGG;

int mediaRB;
int mediaBB;
int mediaGB;

Ultrasonic ultrasonic(13, 12);
LiquidCrystal_I2C lcd(0x27,16,2); // porta 20 e 21

void setup(){
  estado = false;

  Serial.begin(9600);
  lcd.init();
  lcd.backlight();

  pinMode(btnL, INPUT_PULLUP);
  pinMode(btnD, INPUT_PULLUP);
  pinMode(motorH, OUTPUT);
  pinMode(motorA, OUTPUT);
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
  pinMode(out, INPUT);
  pinMode(pneuA, OUTPUT);
  pinMode(pneuV, OUTPUT);

  digitalWrite(s0, HIGH);
  digitalWrite(s1, LOW);

  calibraSensor();
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
  digitalWrite(pneuV, HIGH);

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
  analogWrite(motorH, pwm);
  analogWrite(motorA, 0);
  Serial.println(cm);
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

  if((blue>(mediaBG-15) && blue<(mediaBG+15)) && (red>(mediaRG-15) && red<(mediaRG+15)) && (green>(mediaGG-15) && green<(mediaGG+15))){
    contC++;
    cor = "Cinza";
  } else if((blue>(mediaBB-15) && blue<(mediaBB+15)) && (red>(mediaRB-15) && red<(mediaRB+15)) && (green >(mediaGB-15) && green<(mediaGB+15))){
    contA++;
    cor = "Azul";
  } else if ((blue>(mediaBR-15) && blue<(mediaBR+15)) && (red>(mediaRR-15) && red<(mediaRR+15)) && (green >(mediaGR-15) && green<(mediaGR+15))){
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
    digitalWrite(pneuA, HIGH);
    delay(1000);
  } else if (cor == "Vermelho"){
    Serial.println("Atuador Vermelho");
    digitalWrite(pneuV, LOW);
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
  delay(500);
  lcd.clear();
}

void calibraSensor(){
  int i;
  int controlLoop = true;
  int byteRead;
  int calibrouB = true;
  int calibrouR = true;
  int calibrouG = true;
  
  Serial.write("Voce esta na funcao de calibracao do TCS230"); 
  Serial.println();  
  Serial.write("Digite 1 para vermelho - 2 para azul - 3 para verde - 4 para fechar a calibracao - 5 para reinicializar a calibracao");  
  Serial.println(); 
  delay(100);
  
  
  while(controlLoop){
   
      if (Serial.available()){
        byteRead = Serial.read(); //le bytwe mais recente no buffer da serial
        Serial.write(byteRead);   //reenvia para o computador o dado recebido
      }
      
      if( (byteRead == '1') && calibrouR){ 
        calibrouR = false;
        Serial.print("calibrando cor vermelho");
        Serial.println();
        
        mediaR = mediaB = mediaG = 0;
        for(i=0; i<20; i++){
          int red = freqCor(0,0);
          int blue = freqCor(0, 1);
          int green = freqCor(1, 1);
          mediaR +=red;
          mediaB +=blue;
          mediaG += green;
          delay(50);
        }
       
        mediaRR = mediaR/20;
        mediaBR = mediaB/20;
        mediaGR = mediaG/20;
        Serial.print("calibracao cor vermelha finalizada");
        Serial.println();
        Serial.print(mediaRR),DEC;
        Serial.println();
        Serial.print(mediaBR,DEC);
        Serial.println();
        Serial.print(mediaGR,DEC);
        Serial.println();
        
        Serial.write("Digite 1 para vermelho - 2 para azul - 3 para verde - 4 para fechar a calibracao - 5 para reinicializar a calibracao");  
        Serial.println();
      }
      
      if( (byteRead == '2') && calibrouB ){ 
        calibrouB=false;
        Serial.print("calibrando cor azul");
        Serial.println();
        
        mediaR = mediaB = mediaG = 0;
        for(i=0; i<20; i++){
          int red = freqCor(0,0);
          int blue = freqCor(0, 1);
          int green = freqCor(1, 1);
          mediaR += red;
          mediaB += blue;
          mediaG += green;
          delay(50);
        }
       
        mediaRB = mediaR/20;
        mediaBB = mediaB/20;
        mediaGB = mediaG/20;
        Serial.print("calibracao cor azul finalizada");
        Serial.println();
        Serial.print(mediaRB,DEC);
        Serial.println();
        Serial.print(mediaBB,DEC);
        Serial.println();
        Serial.print(mediaGB,DEC);
        Serial.println();
        Serial.write("Digite 1 para vermelho - 2 para azul - 3 para verde - 4 para fechar a calibracao - 5 para reinicializar a calibracao");  
        Serial.println();
      }
      
      
      if( (byteRead == '3') && calibrouG ){ 
        calibrouG = false;
        Serial.print("calibranado a cor verde");
        Serial.println();
        mediaR = mediaB = mediaG = 0;
        for(i=0; i<20; i++){
          int red = freqCor(0,0);
          int blue = freqCor(0, 1);
          int green = freqCor(1, 1);
          mediaR += red;
          mediaB += blue;
          mediaG += green;
          delay(50);
        }
       
        mediaRG = mediaR/20;
        mediaBG = mediaB/20;
        mediaGG = mediaG/20;
        Serial.print("calibracao cor verde finalizada");
        Serial.print(mediaRG,DEC);
        Serial.println();
        Serial.print(mediaBG,DEC);
        Serial.println();
        Serial.print(mediaGG,DEC);
        Serial.println();
        
        Serial.write("Digite 1 para vermelho - 2 para azul - 3 para verde - 4 para fechar a calibracao - 5 para reinicializar a calibracao");  
        Serial.println();
      }
      
      if(byteRead == '4' ){ 
        controlLoop = false;
      }
      
      if(byteRead == 'g' ){ 
        calibrouR = true;
        calibrouB = true;
        calibrouG = true;
      }
  }
}