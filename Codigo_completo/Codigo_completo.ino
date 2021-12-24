#include <Servo.h>
#include <LiquidCrystal.h>

#define STEP 9      // pin STEP de A4988 a pin 4
#define DIR 10     // pin DIR de A4988 a pin 5
#define FINAL 8   //pin 7 para el final de carrera de la banda transportadora

Servo servo1;
Servo servo2;
Servo servo3;
long pasos;
char datos;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
byte p5[8] = {0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F};

void setup() 
{
  Serial.begin(9600);
  pinMode(STEP, OUTPUT);  // pin 4 para los pulsos (pasos) del motor
  pinMode(DIR, OUTPUT);   // pin 5 para el sentido de giro HIGH izquierda y LOW derecha
  pinMode(FINAL, INPUT_PULLUP);
  servo1.attach(11);
  servo2.attach(6);
  servo3.attach(3);
  servo1.write(0);
  servo2.write(0);
  servo3.write(0);
  setupLCD();
  hacerHome();
}

void loop() 
{
  if(Serial.available()){
    datos = Serial.read();    //Si se han recibido datos por serial, se guardan en la variable
  }
  
  if(datos == 'A'){
    Serial.println("Posicion 1");
    moverBanda(70, false);
    mensajeLCD("Dulce 1");
    servo1.write(180);
    delay(1000);
    servo1.write(0);
    hacerHome();
  }
  else if(datos == 'B'){
    Serial.println("Posicion 2");
    moverBanda(140, false);
    servo2.write(180);
    mensajeLCD("Dulce 2");
    delay(1000);
    servo2.write(0);
    hacerHome();
  }
  if(datos == 'C'){
    Serial.println("Posicion 3");
    moverBanda(210, false);
    mensajeLCD("Dulce 3");
    servo3.write(180);
    delay(1000);
    servo3.write(0);
    hacerHome();
  }
}

//Definición de funciones
void hacerHome(){
  while(digitalRead(FINAL) == HIGH){
    digitalWrite(DIR, HIGH);    //gira a la izquierda hasta llegar al final de carrera
    digitalWrite(STEP, HIGH);   //pulsos de 5 ms       
    delay(5);         
    digitalWrite(STEP, LOW);        
    delay(5);          
  }
}

void moverBanda(int distancia, bool sentido){   //distancia en mm a mover la banda y sentido de giro
  pasos = (long)(distancia * 200)/32;   //200 pasos equivalen a 32 mm

  if(sentido) digitalWrite(DIR, HIGH);
  else digitalWrite(DIR, LOW);
  
  for(int i = 0; i < pasos; i++){     
      digitalWrite(STEP, HIGH);       
      delay(5);          
      digitalWrite(STEP, LOW);        
      delay(5);          
    }
}

void setupLCD(){
  delay(100);
  lcd.createChar(4, p5); 
  //Establece el número de columnas y filas
  lcd.begin(16, 2);
}

void mensajeLCD(String texto){
  lcd.setCursor(0,0);
  lcd.print(texto);
  lcd.setCursor(0,1);

  for(int a ; a <18 ; a++){
    lcd.write(4);
    delay(750);
  }
}
