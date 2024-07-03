//Pinos de conexao do modulo
#define s0 6
#define s1 7
#define s2 8
#define s3 9
#define out 10

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
}
 
void loop()
{
  //Detecta a cor
  color();
  //Mostra valores no serial monitor
  Serial.print("Vermelho :");
  Serial.print(red, DEC);
  Serial.print(" Verde : ");
  Serial.print(green, DEC);
  Serial.print(" Azul : ");
  Serial.print(blue, DEC);
  Serial.println();

  if(blue < red && blue < green){
    Serial.println("Azul");
  } else if (red < blue && red < green){
    Serial.println("Vermelho");
  } else {
    Serial.println("Indefinido");
  }
 
  delay(1000);
}
 
void color()
{
  //Rotina que le o valor das cores
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);
  //count OUT, pRed, RED
  red = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
  digitalWrite(s3, HIGH);
  //count OUT, pBLUE, BLUE
  blue = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
  digitalWrite(s2, HIGH);
  //count OUT, pGreen, GREEN
  green = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
}