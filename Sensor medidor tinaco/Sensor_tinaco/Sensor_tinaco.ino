//Sensor para nivel de agua-----------------------Daniel Torres--- 24/2/21

#define sens1 A0  //0%
#define sens2 A1  //25%
#define sens3 A2  //50%
#define sens4 A3  //75%
#define sens5 A4  //100%
#define sens6 A5  //100% segur
#define sensc 2   //comun 5v

#define pin1 8   //Comun lcd
#define pin2 9   //25%
#define pin3 10  //50%
#define pin4 11  //75%
#define pin5 12  //100%
#define pin6 13  //Contorno 

#define led1 7   //screen
#define led2 6   //rojo
#define rele 5   

#define boton1 4  //Boton principal
#define boton2 3  //Seleccion


bool estadolcd1=LOW, estadolcd2=HIGH;
bool estsens1, estsens2, estsens3, estsens4, estsens5, estsens6;
int segundos=0, decada=0;
short cuenta;
int condicion=4, interval = 500;
unsigned long previousMillis;

void setup() {
  pinMode(pin1, OUTPUT); //pin como salida
  pinMode(pin2, OUTPUT); 
  pinMode(pin3, OUTPUT); 
  pinMode(pin4, OUTPUT);
  pinMode(pin5, OUTPUT); 
  pinMode(pin6, OUTPUT);  
  pinMode(sens1, INPUT);
  pinMode(sens2, INPUT);
  pinMode(sens3, INPUT);
  pinMode(sens4, INPUT);
  pinMode(sens5, INPUT);
  pinMode(sens6, INPUT);
  pinMode(sensc, OUTPUT);
  pinMode(led1, OUTPUT);

  digitalWrite(led1, HIGH);

}

void loop() {

  unsigned long currentMillis = millis();
  if ((unsigned long)(currentMillis - previousMillis) >= interval) //ciclo principal
  {
    previousMillis = millis();
    cuenta++;
    if((cuenta%2)==0)
      segundos++;
    if(cuenta>=20){
      decada++;
      cuenta=0;
      lecturasens();
    }
  }
  
  controllcd();


}

void lecturasens(){

  digitalWrite(sensc, HIGH);
  delay(10);
  estsens1 = digitalRead(sens1);
  estsens2 = digitalRead(sens2);
  estsens3 = digitalRead(sens3);
  estsens4 = digitalRead(sens4);
  estsens5 = digitalRead(sens5);
  estsens6 = digitalRead(sens6);

  if(estsens1==LOW){
    condicion=0;
  }
  else{
    if(estsens1==HIGH)
      condicion=1;
    if(estsens2==HIGH)
      condicion=2;
    if(estsens3==HIGH)
      condicion=3;
    if(estsens4==HIGH)
      condicion=4;
    if(estsens5==HIGH)
      condicion=5;
  }
    
}

void controllcd(){
    switch(condicion){
      case 0:  //solo contorno
      digitalWrite(pin1, estadolcd1);
      digitalWrite(pin2, estadolcd1);
      digitalWrite(pin3, estadolcd1);
      digitalWrite(pin4, estadolcd1);
      digitalWrite(pin5, estadolcd1);
      digitalWrite(pin6, !estadolcd1);
      delay(10);
    break;
    case 1:  //solo contorno
      digitalWrite(pin1, estadolcd1);
      digitalWrite(pin2, estadolcd1);
      digitalWrite(pin3, estadolcd1);
      digitalWrite(pin4, estadolcd1);
      digitalWrite(pin5, estadolcd1);
      digitalWrite(pin6, !estadolcd1);
      delay(10);
    break;

    case 2: //25%
      digitalWrite(pin1, estadolcd1);
      digitalWrite(pin2, !estadolcd1);
      digitalWrite(pin3, estadolcd1);
      digitalWrite(pin4, estadolcd1);
      digitalWrite(pin5, estadolcd1);
      digitalWrite(pin6, !estadolcd1);
      delay(10);
    break;

    case 3:  //50%
      digitalWrite(pin1, estadolcd1);
      digitalWrite(pin2, !estadolcd1);
      digitalWrite(pin3, !estadolcd1);
      digitalWrite(pin4, estadolcd1);
      digitalWrite(pin5, estadolcd1);
      digitalWrite(pin6, !estadolcd1);
      delay(10);
    break;

    case 4:  //75%
      digitalWrite(pin1, estadolcd1);
      digitalWrite(pin2, !estadolcd1);
      digitalWrite(pin3, !estadolcd1);
      digitalWrite(pin4, !estadolcd1);
      digitalWrite(pin5, estadolcd1);
      digitalWrite(pin6, !estadolcd1);
      delay(10);
    break;

    case 5:  //100%
      digitalWrite(pin1, estadolcd1);
      digitalWrite(pin2, !estadolcd1);
      digitalWrite(pin3, !estadolcd1);
      digitalWrite(pin4, !estadolcd1);
      digitalWrite(pin5, !estadolcd1);
      digitalWrite(pin6, !estadolcd1);
      delay(10);
    break;
      
  }
  
  estadolcd1=!estadolcd1;
}



/*
 *    1 2 3 4 5 6
 *    | | | | | |
 *   |------------|
 *   |            |
 *   |            |
 *   |------------|
 *
 */
