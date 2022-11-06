//Sensor para nivel de agua-----------------------Daniel Torres--- 20/05/21

//Acomodo de niveles
//Nivel de apagado en sensor 5 o 6
// 


#define sens1 A0  //0%    --- A0
#define sens2 A1  //25%
#define sens3 A2  //50%
#define sens4 A3  //75%
#define sens5 A4  //100%
#define sens6 A5  //100% segur -- A5
#define sensc 2   //comun 5v

#define pin1 13   //Comun lcd  8
#define pin2 12  //25%
#define pin3 11  //50%
#define pin4 10  //75%
#define pin5 9  //100%
#define pin6 8  //Contorno 13

#define led1 7   //screen
#define led2 6   //rojo
#define led3 1   //verde
#define rele 5   

#define boton1 4  //Boton principal
#define boton2 3  //Seleccion

#define tiempantalla 70     //Tiempo en segundos (/2) de pantalla activa
#define botonpresionado 5  //Tiempo de presionado (/2) necesario para activar bomba
#define tiempodelectura 20  //Tiempo de lectura estandar de sensores
#define tiempoparo 480      //Tiempo de paro de seguridad
#define lecturaenbomba 15   //tiempo de lectura (/2) de sensores con bomba activada


bool estadolcd1=LOW, estadolcd2=HIGH, boton1activo=HIGH;
bool estsens1, estsens2, estsens3, estsens4, estsens5, estsens6, estadobtn1, estadobtn2;
bool nothing=false, rojo=true, bombaactivada=false, unicociclo=true,paroemerg=false;
int segundos=0, cuentaboton1=0, cuentaboton2=0, cuentabomba=0, cuentalecbomba=0;
short cuenta, mlecbomba=0;
int condicion=0, interval = 500;
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
  pinMode(boton1, INPUT);
  pinMode(boton2, INPUT);
  pinMode(sensc, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(rele, OUTPUT);

  digitalWrite(led1, HIGH);

  lecturasens();

}

void loop() {

  unsigned long currentMillis = millis();      //////tiempo
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
      if(cuentaboton1>=tiempantalla){
        boton1activo=false;
        cuentaboton1=0;
        digitalWrite(led1, LOW);
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

    estadobtn2 = digitalRead(boton2); //Boton activar bomba 
    if(estadobtn2 == HIGH){  //condicion de boton presionado
      cuentaboton2++;
      digitalWrite(led1, LOW);
     
      if(cuentaboton2>=botonpresionado && bombaactivada==false){
        bombaactivada=true;
        digitalWrite(led1, LOW);
        digitalWrite(led3, HIGH);
        delay(2000);
        cuentaboton2=0;
      }
      
      else if(bombaactivada==true && cuentaboton2>=4){
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
    }
    else{
      cuentaboton2=0;
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
    
  }//--------------------time--------------------------------------
  
  controllcd();

  estadobtn1 = digitalRead(boton1); //Boton seleccion 
  if(estadobtn1 == HIGH){  //condicion de boton presionado
    boton1activo = true;
    rojo=false;
    digitalWrite(led1, HIGH);
    lecturasens();
  }

  if(bombaactivada==true){  //desicion de activado de bomba
    bomba();
  }


}

void lecturasens(){

  digitalWrite(sensc, HIGH);
  delay(50);
  estsens1 = digitalRead(sens1);
  estsens2 = digitalRead(sens2);
  estsens3 = digitalRead(sens3);
  estsens4 = digitalRead(sens4);
  estsens5 = digitalRead(sens5);
  estsens6 = digitalRead(sens6);
  digitalWrite(sensc, LOW);

  if(estsens1==LOW){
    condicion=0;
    nothing=true;
  }
    if(estsens1==HIGH){
    nothing=false;
    condicion=1;
    }
      
    if(estsens2==HIGH){
      condicion=2;
      nothing=false;
    }
     
    if(estsens3==HIGH){
      condicion=3;
      nothing=false;
    }
    
    if(estsens4==HIGH){
      condicion=4;
      nothing=false;
    }
      
    if(estsens5==HIGH)
      condicion=5;

    if(estsens6==HIGH)
      condicion=6;
      
    if(boton1activo==false && bombaactivada==false)
      condicion=7;
      
  }
    
//-------------------------------------------------------------------------------------------------

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

void bomba(){      //Chequeo de sensores y bomba activa

  if(mlecbomba==1){  //condicion de lectura de sensor cada cierto tiempo
    mlecbomba=0;
    lecturasens();
  }
  
  
  if(unicociclo==true){
    
    unicociclo=false;
    
    if(condicion<=2){
      for(int i=0; i<=3; i++){
        digitalWrite(led3, HIGH);
        delay(500);
        digitalWrite(led3, LOW);
        delay(500);
      }
      digitalWrite(led3, HIGH);
      digitalWrite(rele, HIGH);
    }
    
    if(condicion>2 && condicion<7){
      digitalWrite(rele, LOW);
      bombaactivada=false;
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
      bombaactivada=false;
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
