//Sensor para nivel de agua------------- Daniel Torres //.-.// ------------------- 19/05/22

//V4 - Actualizacion de activado automatico
//Restructura de codigo .-.
//Anadido de eeprom para almacenamiento de auto en falla de energia


//-------------------- Variables Configurables ---------------------------------------------------------------

#define tiempantalla 70     //Tiempo en segundos (/2) de pantalla activa
#define botonpresionado 5   //Tiempo de presionado (/2) necesario para activar bomba o automatico
#define tiempodelectura 20  //Tiempo de lectura estandar de sensores
#define tiempoparo 480      //Tiempo de paro de seguridad (Segundos)
#define lecturaenbomba 15   //tiempo de lectura (/2) de sensores con bomba activada
#define niveldeact 2        //Nivel de posibilidad de activado de bomba '<' o '=' (1=0%, 2=25%, 3=50%)
#define sensorauto 1        //Sensor de activacion para auto
#define timeespauto 120      //Tiempo de espera para encendido en auto

//-------------------------------------------------------------------------------------------------------------

#define sens1 A0  //0% 
#define sens2 A1  //25%
#define sens3 A2  //50%
#define sens4 A3  //75%
#define sens5 A4  //100%
#define sens6 A5  //100% (seguridad)
#define sensc 2   //comun 5v

#define pin1 13   //Comun lcd
#define pin2 12   //25%
#define pin3 11   //50%
#define pin4 10   //75%
#define pin5 9    //100%
#define pin6 8    //Contorno

#define led1 7   //screen
#define led2 6   //rojo
#define led3 1   //verde
#define rele 5   

#define boton1 4  //Boton principal (Activado de pantalla)
#define boton2 3  //Seleccion (activado de bomba)

#include <EEPROM.h>

bool estadolcd1=LOW, estadolcd2=HIGH, boton1activo=HIGH, automatico=0, estbtn1;
bool estsens1, estsens2, estsens3, estsens4, estsens5, estsens6, estadobtn1, estadobtn2;
bool condled=true, condauto=false;
bool nothing=false, rojo=true, bombaactivada=false, unicociclo=true,paroemerg=false,bombaautoact=false;
int segundos=0, cuentaboton1=0, cuentaboton2=0, cuentabomba=0, cuentalecbomba=0, cuentacondauto=0, cuentaauto=0;
short cuenta, mlecbomba=0, conteoled=0;
int condicion=0, interval = 500;
unsigned long previousMillis;

void setup() {

  //Serial.begin(9600);
  
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
  pinMode(boton1, INPUT);
  pinMode(boton2, INPUT);
  pinMode(sensc, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(rele, OUTPUT);

  digitalWrite(led1, HIGH);

  lecturasens();

  automatico = EEPROM.read(5); //Lugar de almacenamiento de bool auto

}

void loop() {

  //------------------------------------------------------------ Funcion TIME --------------------------------------------

  unsigned long currentMillis = millis();    
  if ((unsigned long)(currentMillis - previousMillis) >= interval) //ciclo principal
  {
    previousMillis = millis();
    cuenta++;
    cuentalecbomba++;
    
    if((cuenta%2)==0)
      segundos++;
    if(cuenta>=tiempodelectura){
      cuenta=0;
      lecturasens();
    }

    if(boton1activo==true){  //Condicion de pantalla activa
      cuentaboton1++;
      if(condled==true){
        conteoled++;
      }
      
      if(cuentaboton1>=tiempantalla){
        boton1activo=false;
        cuentaboton1=0;
        digitalWrite(led1, LOW);
      }
      if(conteoled>=5){
        condled=false;
        digitalWrite(led2, LOW);
        if(bombaactivada==false){
          digitalWrite(led3, LOW);
        }
      }
    }
    
    if(nothing==true && rojo==true){ //accion tanque vacio
      digitalWrite(led1, LOW);
      if((cuenta%2)==0){
        digitalWrite(led2, HIGH);
      }
      else{
        digitalWrite(led2, LOW);
      } 
    }
    else{
      digitalWrite(led2, LOW);
    }


    estbtn1 = digitalRead(boton1); //Boton activar bomba     //AUTO*******************************
    if(estbtn1 == HIGH){  
      cuentaauto++;
    }
    else{
      cuentaauto=0;
    }

    if(cuentaauto>=botonpresionado){
      cuentaauto=0;
      digitalWrite(led1, LOW);
      
      if(automatico==0){ //apagado?
        EEPROM.update(5, 1);
        automatico=1;
        for(int i=0; i<=4; i++){
          digitalWrite(led3, HIGH);
          delay(500);
          digitalWrite(led3, LOW);
          delay(500);
        } 
      }
      else{ //encendido?
        EEPROM.update(5, 0);
        automatico=0;
        for(int i=0; i<=4; i++){
          digitalWrite(led2, HIGH);
          delay(500);
          digitalWrite(led2, LOW);
          delay(500);
        } 
      }
      
    }

    if(condicion<=sensorauto && automatico==true){
      cuentacondauto++;
    }
    else{
      cuentacondauto=0;
    }
    if(cuentacondauto>=timeespauto){
      bombaautoact=true;
    }
      

                                                                

    estadobtn2 = digitalRead(boton2); //Boton activar bomba     //Boton activado bomba //////////////////////////////////////
    if(estadobtn2 == HIGH){  
      cuentaboton2++;
    }
    else{
      cuentaboton2=0;
    }
     
    if((cuentaboton2>=botonpresionado && bombaactivada==false)){
      bombaactivada=true;
      digitalWrite(led1, LOW);
      digitalWrite(led3, HIGH);
      delay(2000);
      cuentaboton2=0;
    }
      
    if(bombaactivada==true && cuentaboton2>=4){
      digitalWrite(rele, LOW);
      digitalWrite(led3, LOW);
      for(int i=0; i<=3; i++){
        digitalWrite(led2, HIGH);
        delay(700);
        digitalWrite(led2, LOW);
        delay(300);
      }
      paroemerg=true;
    }


    if(bombaactivada==true && (condicion>=4 && condicion<7)){
      cuentabomba++;
    }
    else{
      cuentabomba=0;
    }

    if(cuentalecbomba>=lecturaenbomba){
      cuentalecbomba=0;
      mlecbomba=1;
    }
    
  }//-------------------------------- Fin De TIME ------------------------------------
  

  estadobtn1 = digitalRead(boton1); //Boton seleccion 
  if(estadobtn1 == HIGH){  
    boton1activo = true;
    condled = true;
    rojo=false;
    
    if(automatico==0){ //apagado?
      digitalWrite(led2, HIGH);
    }
    if(automatico==1){ //encendido?
      digitalWrite(led3, HIGH);
    }
    
    digitalWrite(led1, HIGH);
    lecturasens();
  }

  if(bombaactivada==true){  //*************************Activado de bomba
    bomba();
  }

  if(bombaautoact==true){  //si el sensor detecta agua baja
    bombaactivada=true;
    //Serial.println("Activada-----");
  }

  controllcd(); //Control de lcd
  /*
  Serial.print("Cond: ");
  Serial.print(condicion);
  Serial.print("   bomba: ");
  Serial.print(digitalRead(rele));
  Serial.print("   ledverd: ");
  Serial.print(digitalRead(led3));
  Serial.print("    auto: ");
  Serial.print(automatico);
  
  Serial.print("    cuenta: ");
  Serial.println(cuentacondauto);
  */

}


//------------------------------------------------ Funcion Lectura y condiciones ---------------------------------------------

void lecturasens(){

  digitalWrite(sensc, HIGH); //Enciende Comun-Signal
  delay(50);
  
  estsens1 = digitalRead(sens1);
  estsens2 = digitalRead(sens2);
  estsens3 = digitalRead(sens3);
  estsens4 = digitalRead(sens4);
  estsens5 = digitalRead(sens5);
  estsens6 = digitalRead(sens6);
  
  digitalWrite(sensc, LOW);  //Apaga Comun-Signal

  if(estsens1==LOW){ //vacio
    condicion=0;
    nothing=true;
  }
  if(estsens1==HIGH){ //25
    nothing=false;
    condicion=1;
  }
      
  if(estsens2==HIGH){ //50%
    condicion=2;
    nothing=false;
  }
     
  if(estsens3==HIGH){ //50%
    condicion=3;
    nothing=false;
  }
   
  if(estsens4==HIGH){ //75%
    condicion=4;
    nothing=false;
  }
      
  if(estsens5==HIGH){//100%
    
    cuentacondauto=0;
    condicion=5;
  }
    
    

  if(estsens6==HIGH)  //100% Seguro
    condicion=6;
    
      
  if(boton1activo==false && bombaactivada==false)
    condicion=7;
      
}
    
//----------------------------- Fin Lectura ---------------------------


//------------------------------------------------ Funcion Control LCD ---------------------------------------------
void controllcd(){
    switch(condicion){
      case 0:  //solo contorno
      digitalWrite(pin1, !estadolcd1);
      digitalWrite(pin2, !estadolcd1);
      digitalWrite(pin3, !estadolcd1);
      digitalWrite(pin4, !estadolcd1);
      digitalWrite(pin5, !estadolcd1);
      digitalWrite(pin6, estadolcd1);
      delay(10);
    break;
    case 1:  //25%
      digitalWrite(pin1, !estadolcd1);
      digitalWrite(pin2, estadolcd1);
      digitalWrite(pin3, !estadolcd1);
      digitalWrite(pin4, !estadolcd1);
      digitalWrite(pin5, !estadolcd1);
      digitalWrite(pin6, estadolcd1);
      delay(10);
    break;

    case 2: //50%
      digitalWrite(pin1, !estadolcd1);
      digitalWrite(pin2, estadolcd1);
      digitalWrite(pin3, estadolcd1);
      digitalWrite(pin4, !estadolcd1);
      digitalWrite(pin5, !estadolcd1);
      digitalWrite(pin6, estadolcd1);
      rojo=true;
      delay(10);
    break;

    case 3:  //50%
      digitalWrite(pin1, !estadolcd1);
      digitalWrite(pin2, estadolcd1);
      digitalWrite(pin3, estadolcd1);
      digitalWrite(pin4, !estadolcd1);
      digitalWrite(pin5, !estadolcd1);
      digitalWrite(pin6, estadolcd1);
      delay(10);
    break;

    case 4:  //75%
      digitalWrite(pin1, !estadolcd1);
      digitalWrite(pin2, estadolcd1);
      digitalWrite(pin3, estadolcd1);
      digitalWrite(pin4, estadolcd1);
      digitalWrite(pin5, !estadolcd1);
      digitalWrite(pin6, estadolcd1);
      delay(10);
    break;

    case 5:  //100%
      digitalWrite(pin1, !estadolcd1);
      digitalWrite(pin2, estadolcd1);
      digitalWrite(pin3, estadolcd1);
      digitalWrite(pin4, estadolcd1);
      digitalWrite(pin5, estadolcd1);
      digitalWrite(pin6, estadolcd1);
      delay(10);
    break;

    case 6:  //100%
      digitalWrite(pin1, !estadolcd1);
      digitalWrite(pin2, estadolcd1);
      digitalWrite(pin3, estadolcd1);
      digitalWrite(pin4, estadolcd1);
      digitalWrite(pin5, estadolcd1);
      digitalWrite(pin6, estadolcd1);
      delay(10);
    break;

    case 7:  //Apagado
      digitalWrite(pin1, LOW);
      digitalWrite(pin2, LOW);
      digitalWrite(pin3, LOW);
      digitalWrite(pin4, LOW);
      digitalWrite(pin5, LOW);
      digitalWrite(pin6, LOW);
      //delay(10);
    break;
      
  }
  
  estadolcd1=!estadolcd1;
}


//------------------------------------------------ Funcion Lectura ---------------------------------------------

void bomba(){      //Chequeo de sensores y bomba activa

  if(mlecbomba==1){  //condicion de lectura de sensor cada cierto tiempo
    mlecbomba=0;
    lecturasens();
  }
  
  
  if(unicociclo==true){
    
    unicociclo=false;
    
    if(condicion <= niveldeact){  //Condicion de activado de bomba
      for(int i=0; i<=3; i++){
        digitalWrite(led3, HIGH);
        delay(500);
        digitalWrite(led3, LOW);
        delay(500);
      }
      digitalWrite(led3, HIGH);
      digitalWrite(rele, HIGH);
      //Serial.println("Activada*******************");
    }
    
    if(condicion>2 && condicion<7){
      digitalWrite(rele, LOW);
      bombaactivada=false;
      bombaautoact=false;
      unicociclo=true;
      digitalWrite(led3, LOW);
      for(int i=0; i<=3; i++){
        digitalWrite(led2, HIGH);
        delay(250);
        digitalWrite(led2, LOW);
        delay(250);
      }
    }
  }


  if(condicion==5 || condicion==6 || paroemerg==true){ //LLeno
     if(cuentabomba>=tiempoparo || condicion==6 || paroemerg==true){
      digitalWrite(rele, LOW);
      //Serial.println("Desactivada++++++++++++++++");
      //cuentacondauto=0;
      bombaactivada=false;
      bombaautoact=false;
      digitalWrite(led3, LOW);
      unicociclo=true;
      paroemerg=false;

      
     }
  }
}





/*
 * - 6 |  105%
 * - 5 |  100%
 * - 4 |  75
 * - 3 |  50
 * - 2 |  50
 * - 1 |  25%
 *     |____________|
 *    
 *    
 * 
 * 
 * 
 *    1 2 3 4 5 6
 *    | | | | | |
 *   |------------|
 *   |            |
 *   |            |
 *   |------------|
 *
 */
