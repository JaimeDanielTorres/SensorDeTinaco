// Jaime Daniel Torres Contreras - Control para bomba de agua   -------  28/12/20
//V1 - Pruebas de graficos y touch
// TFT shield LCD 2.4" Chip ILI8357

#include <Adafruit_GFX.h>    // Libreria de graficos
#include <Adafruit_TFTLCD.h> // Libreria de LCD 
#include <TouchScreen.h>     // Libreria del panel tactil

 // 4 pines del panel tactil
#define YP A3  // must be an analog pin, use "An" notation! A1
#define XM A2  // must be an analog pin, use "An" notation! A2
#define YM 9   // can be a digital pin 7
#define XP 8   // can be a digital pin 6

 // 5 pines del panel tactil
#define LCD_CS A3   // Definimos los pines del LCD
#define LCD_CD A2   // para poder visualizar elementos graficos
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET A4 //opcional


#define MINPRESSURE 1      // Definimos la presion máxima y minima que podemos realizar sobre el panel
#define MAXPRESSURE 1000


TouchScreen ts = TouchScreen(XP, YP, XM, YM, 364);  //Instancia del panel tactil (Pin XP, YP, XM, YM, Resistencia del panel) 

short TS_MINX = 117; // LEFT - Coordenadas del panel tactil para delimitar
short TS_MINY = 90;  //        el tamaño de la zona donde podemos presionar
short TS_MAXX = 894; // RT     y que coincida con el tamaño del LCD
short TS_MAXY = 887; // BOT     

#define LGRAY   0xBEBE
#define DGRAY   0x4949
#define	BLACK   0x0000  // Definimos los colores 
#define	BLUE    0x001F  // que utilizaremos para 
#define	RED     0xF800  // el texto y los elementos graficos
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF


Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET); // Instancia LCD 

int X; // Variables que almacenaran la coordenada
int Y; // X, Y donde presionemos y la variable Z 
int Z; // almacenara la presion realizada

int a = 0; // variable "flag" para control rebotes

void setup(void) 
{ 

  Serial.begin(9600);
  
  tft.begin(0x9341); // Iniciamos el LCD especificando el controlador ILI9341. 

  tft.setRotation(1);
  
  tft.fillScreen(BLUE); // Pintamos la pantalla
  //tft.drawRect(0, 0, 320, 119, RED); // Dibujamos un "boton" (xi,yi,xtam, ytam)
  
  tft.setCursor(75,45);  // Colocamos el cursor(x,y)
  tft.setTextSize(5);    // Especificamos el tamaño del texto
  tft.setTextColor(GREEN); // Definimos el color del texto 
  tft.println("MANUAL");  // Escribimos por pantalla     

   tft.fillRect(0, 120, 320, 120, LGRAY); // Dibujamos nuestro boton relleno de color (xi,yi,xtam, ytam)
        
   tft.setCursor(100,160);    // Colocamos el cursor
   tft.setTextSize(5);      // Especificamos el tamaño del texto
   tft.setTextColor(DGRAY); // Definimos el color del texto 
   tft.println("AUTO");    // Escribimos por pantalla
  
}

void loop()
{          
    lecturaPanel(); // Realizamos lectura del panel para detectar presion y coordenadas
      //
    if((X > 40 && X < 200) && (Y > 20 && Y < 80) && (Z > MINPRESSURE && Z < MAXPRESSURE)) //IF pulse eje X entre 40 y 160 AND eje Y entre 20 y 60 AND presion establecida
    {
      if (a == 0) // Si la variable flag esta en 0
      {
        tft.fillRect(60, 20, 180, 60, RED); // Dibujamos nuestro boton relleno de color (xi,yi,xtam, ytam)
        
        tft.setCursor(90,25);    // Colocamos el cursor
        tft.setTextSize(3);      // Especificamos el tamaño del texto
        tft.setTextColor(WHITE); // Definimos el color del texto 
        tft.println("BOTON");    // Escribimos por pantalla
        tft.setCursor(90,50);    // Colocamos el cursor
        tft.println("PULSADO");  // Escribimos por pantalla   

        a = 1;  // Ponemos la variable flag en 1
        delay(150);
      }
      else if (a == 1) // Si la variable flag esta en 1
      {
         tft.fillRect(41, 21, 158, 58, CYAN); // Dibujamos el fondo de nuestro boton en cyan
         
         tft.setCursor(60,35);  // Colocamos el cursor
         tft.setTextSize(4);    // Especificamos el tamaño del texto
         tft.setTextColor(RED); // Definimos el color del texto 
         tft.println("BOTON");  // Escribimos por pantalla
         
         a = 0; // Ponemos la variable flag en 0 para evitar los rebotes
         delay(150);
      }
    }    
}

void lecturaPanel()
{

    TSPoint p = ts.getPoint(); // Realizamos lectura de las coordenadas
  
    pinMode(XM, OUTPUT); // La librería usa estos pines como entrada y salida por lo que es necesario declararlos como salida justo
    pinMode(YP, OUTPUT); // despues de realizar una lectura de coordenadas.      
  
    // Mapeamos los valores analogicos leidos del panel tactil (0-1023) a 320x240
    Y = map(p.x, TS_MINX, TS_MAXX, 0, 240);  //240
    X = map(p.y, TS_MAXY, TS_MINY, 0, 320);  //320
    Z = p.z;


    Serial.print("X = "); Serial.print(X);
     Serial.print("\tY = "); Serial.print(Y);
     Serial.print("\tPX = "); Serial.print(p.y);
     Serial.print("\tPY = "); Serial.print(p.x);
     Serial.print("\tPressure = "); Serial.println(p.z);

}

/*

const int XP=8,XM=A2,YP=A3,YM=9; //240x320 ID=0x9341
const int TS_LEFT=121,TS_RT=896,TS_TOP=85,TS_BOT=880;
PORTRAIT CALIBRATION     240 x 320
x = map(p.x, LEFT=127, RT=896, 0, 240)
y = map(p.y, TOP=97, BOT=885, 0, 320)
Touch Pin Wiring XP=8 XM=A2 YP=A3 YM=9
LANDSCAPE CALIBRATION    320 x 240
x = map(p.y, LEFT=85, RT=880, 0, 320)
y = map(p.x, TOP=896, BOT=121, 0, 240)

*/
