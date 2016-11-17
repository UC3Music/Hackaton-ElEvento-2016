
#include <Servo.h>
#define SONIDO_IZQ 2 //Sensor digital utilizado en el final del mango del hacha
#define SONIDO_DCHA 3 //Sensor adicional-No usado en la competicion
#define MI 11 //Motor izquierdo
#define MD 12 //Motor derecho

//Activacion sonido pistola de juguete-Objeto de reto de la competicion
#define FIRE 49

//Uso opcional de arranque con boton arcade con luz
#define BOTON_ARCADE 4 
#define LED_ARCADE 13

Servo servoDcha;
Servo servoIzq;


//Variables globales
bool arranque=false; //Bandera usada para el arranque mediante boton arcade
volatile int cont=0; //Contador de picos de sonido, Tipo volatile debido a su uso con interrupciones
int cont_hits=0; //Contador de numero de golpes realizados
int tap_time=600; //Tiempo mnimo que ha de pasar entre una serie de golpes y la siguiente orden
int vel=45; //Offset para controlar la velocidad de los servomotores

long unsigned int time =0; //Variable de tiempo a usar junto con la funcion millis()



void setup() {
  
  //pinMode(SONIDO_DCHA, INPUT);
  //pinMode(BOTON_ARCADE, INPUT_PULLUP);
  //pinMode(LED_ARCADE,OUTPUT);
  //pinMode(FIRE,OUTPUT);

  pinMode(SONIDO_IZQ, INPUT_PULLUP);
  attachInterrupt(0, contador, CHANGE); //Cada vez que se reciba un cambio en el bit, se ejecuta la funcion contador. Se puede sustituir por un Rising or falling segun el caso
  delay(500);
  
  servoDcha.attach(MD);
  servoIzq.attach(MI);
  
  //Orden inicial para parar los sermotores.
  servoDcha.write(88);
  servoIzq.write(88);
}

void loop() {

  //Funciones de testeo
  //test();
  //test_sensor();

  /*
  //Secuencia de arranque con boton arcade
  while(arranque==false)
  {
    digitalWrite(LED_ARCADE,HIGH);
    delay(50);

    digitalWrite(LED_ARCADE,LOW);
    delay(50);
    
    
    if(digitalRead(BOTON_ARCADE)==0)
    {
      arranque=true;
      digitalWrite(FIRE,HIGH);
      digitalWrite(LED_ARCADE,HIGH);
      delay(500);
      digitalWrite(FIRE,LOW);
      delay(11000);//Tiempo de espera para que la pistola de juguete ejecute su melodia
      
    }
    delay(400);
  }
  */

  //Si el contador del sensor de sonido ha detectado algo  
  if(cont>0)
  {
    cont_hits++;//Incremento de una sola unidad en el contador de golpes
    //Serial.println(cont_hits);
    time=millis(); //Reinicio del contador de tiempo entre golpe y golpe
    delay(200);
    cont=0;
  }
  
  //Si entre un golpe y otro ha pasado mas tiempo del establecido. Se considera una secuencia completa y se llama a Estado para ejecutar la orden
  if(millis()-time>=tap_time)
  {
    Estado(cont_hits);
    cont_hits=0;
    time=millis();
  }

  
}



//Maquina de estados del robot
void Estado(int cont_hits){
  switch(cont_hits)
  {
    case 0:
      break;
    case 1:
      Palante();
      break;

    case 2:
      Izquierda();
      break;

    case 3:
      Derecha();
      break;

    default:
      Parada();
  }
}


//Funciones auxiliares del movimiento del robot
void Parada (){
  servoDcha.write(88);
  servoIzq.write(88);
}
void Patras(){
  servoDcha.write(180);
  servoIzq.write(0);
}
void Palante(){
  servoDcha.write(0);
  servoIzq.write(180);
}
void PivoteDerecha(){
  servoDcha.write(88);
  servoIzq.write(180);
}
void PivoteIzquierda(){
  servoDcha.write(0);
  servoIzq.write(88);
}
void Derecha(){
  servoDcha.write(180-vel);
  servoIzq.write(180-vel);
}
void Izquierda(){
  servoDcha.write(0+vel);
  servoIzq.write(0+vel);
}


//Funcion de incremento de cambio de umbral en el sensor de sonido. Llamada mediante una interrupcion
void contador() {
  cont++;
}


//Funcion de testeo de los movimientos del robot
void test()
{
   Patras();
   delay(5000);
   Parada();
   delay(3000);
   Palante();
   delay(5000);
   Parada();
   delay(3000);
   PivoteDerecha();
   delay(5000);
   Parada();
   delay(3000);
   PivoteIzquierda();
   delay(5000);
   Parada();
   delay(3000);
   Derecha();
   delay(5000);
   Parada();
   delay(3000);
   Izquierda();
   delay(5000);
   Parada();
   delay(3000);
  
}

//Funcion de testeo de sensores del robot
void test_sensor()
{
 if(digitalRead(SONIDO_IZQ)==0)
 {
  digitalWrite(13,HIGH);
 }
else
{

  digitalWrite(13,LOW);
}
}


