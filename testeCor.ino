int s0 = 6;
int s1 = 7;
int s2 = 8;
int s3 = 9;
int out = 10;

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

unsigned int red = 0;
unsigned int green = 0;
unsigned int blue = 0;

void setup()
{
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
  pinMode(out, INPUT);
  Serial.begin(9600);
  digitalWrite(s0, HIGH);
  digitalWrite(s1, LOW);

  calibraSensor();
}
 
void loop()
{
  //Detecta a cor
  int red = freqCor(0,0);
  int blue = freqCor(0, 1);
  int green = freqCor(1, 1);
  //Mostra valores no serial monitor
  Serial.print("Vermelho :");
  Serial.print(red, DEC);
  Serial.print(" Verde : ");
  Serial.print(green, DEC);
  Serial.print(" Azul : ");
  Serial.print(blue, DEC);
  Serial.println();

  if((blue>(mediaBG-15) && blue<(mediaBG+15)) && (red>(mediaRG-15) && red<(mediaRG+15)) && (green>(mediaGG-15) && green<(mediaGG+15))){
    Serial.println("Cinza");
  } else if((blue>(mediaBB-15) && blue<(mediaBB+15)) && (red>(mediaRB-15) && red<(mediaRB+15)) && (green >(mediaGB-15) && green<(mediaGB+15))){
    Serial.println("Azul");
  } else if ((blue>(mediaBR-15) && blue<(mediaBR+15)) && (red>(mediaRR-15) && red<(mediaRR+15)) && (green >(mediaGR-15) && green<(mediaGR+15))){
    Serial.println("Vermelho");
  } else {
    Serial.println("Indefinido");
  }
 
  delay(1000);
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