#include <Wire.h>               /*~ Librería I2C ~*/
#include <LiquidCrystal_I2C.h>
#define LED_VERDE         0x02  // Puerto de conexión LED verde 100% de humedad.
#define LED_AZUL          0x03  // Puerto de conexión LED azul 75% de humedad.
#define LED_BLANCO        0x04    // Puerto de conexión LED blanco 50% de humedad.
#define LED_AMARILLO      0x05    // Puerto de conexión LED amarillo 25% de humedad.
#define LED_ROJO          0x06    // Puerto de conexión LED rojo 0% de humedad.
#define SOIL                A0    // Puerto analógico para el sensor de humedad. 
#define MAX_SOIL_VALUE       876    // Valor máximo obtenido en la simulación.

/*
  Nota: Algunas caracterírticas varían entre el sensor real del simulado,
        el real en la lectura del ADC muestra 0 en su máxima lectura de humedad
          caso contrario a la simulación, es necesario modificar la lógica de la lectura.
*/

/* Reemplazo en el código    condicional      se cumple/no se cumple */
#define controlRojo( a )   a <= 12      ? HIGH : LOW  // Si se recibe un valor menor a 12 entonces equivale a un HIGH(alto, 1, verdadero), en caso contrario un LOW(bajo, 0, falso).
#define controlAmarillo( a ) a > 12 && a <= 25  ? HIGH : LOW
#define controlBlanco( a )   a > 25 && a <= 50  ? HIGH : LOW
#define controlAzul( a )     a > 50 && a <= 75  ? HIGH : LOW
#define controlVerde( a )    a > 75 && a <= 100 ? HIGH : LOW
/*Más sobre el operador condicional: https://learn.microsoft.com/es-es/cpp/cpp/conditional-operator-q?view=msvc-170*/

byte caracter1 [ ] = {
  B00100,
  B01110,
  B11111,
  B11111,
  B11111,
  B11111,
  B01110,
  B00000              /*~ ⬛⬜⬜⬜⬛~*/
};
LiquidCrystal_I2C lcd ( 0x27, 16, 2 );
/* Variable que indica el valor analógico obtenido del sensor */
float Soilmoisture = 0;

void setup( void ) {
  
  lcd.init ( );             /*~ Inicializar la pantalla LCD ~*/
  
  lcd.createChar ( 0, caracter1 );    /*~ Indicar al programa que genere un caracter a partir del array de bits. ~*/
  //lcd.createChar ( 1, caracter2 );
  
  Serial.begin ( 115200 );  // Baudrate a 115200 bits/segundo.
  uint8_t LEDS [ ] = { LED_VERDE, LED_AZUL, LED_BLANCO, LED_AMARILLO, LED_ROJO }; // se declara un array cuyo cada miembro contiene el pin digital al cual está conectado su LED correspondiente.
  
  for ( uint8_t i = 0; i < sizeof( LEDS ); i++ ) {  // Se recorre el array; sizeof retorna el número de bytes que necesita en memoria, solo en este caso como cada miembro puede almacenar 8 bits, retorna el tamaño del array.
   
    pinMode ( LEDS [ i ], OUTPUT );         // Se declara cada pin almacenado en el array como salida.
    digitalWrite ( LEDS [ i ], LOW );       // Se inicializa cada pin almacenado en el array en BAJO.
    
  }
  
  pinMode ( SOIL, INPUT );  // El único pin de entrada en este caso es el que pertenece al sensor de humedad del suelo.
  
}

/* Función que retorna el porcentaje de humedad obtenida por el sensor */
float getSoilmoisture ( void ) {
  
  return ( float ) analogRead( SOIL ) * 100 / MAX_SOIL_VALUE; // Se retorna el valor correspondiente en una regla de 3 para calcular el 100% según la cantidad máxima que proporciona el sensor, en este caso es: 876;
      
}
/*

void indicator ( uint8_t value )  {
  
  if ( value <= 12 ) {
    
    
    digitalWrite ( LED_VERDE, LOW);
    digitalWrite ( LED_AZUL, LOW);
    digitalWrite ( LED_BLANCO, LOW);
    digitalWrite ( LED_AMARILLO, LOW);
    digitalWrite ( LED_ROJO, HIGH);
    
  }
  
  else if ( value > 12 && value <= 25 ) {
    
  digitalWrite ( LED_VERDE, LOW);
    digitalWrite ( LED_AZUL, LOW);
    digitalWrite ( LED_BLANCO, LOW);
    digitalWrite ( LED_AMARILLO, HIGH);
    digitalWrite ( LED_ROJO, LOW);
    
  }
  
   else if ( value > 25 && value <= 50 ) {
    
  digitalWrite ( LED_VERDE, LOW);
    digitalWrite ( LED_AZUL, LOW);
    digitalWrite ( LED_BLANCO, HIGH);
    digitalWrite ( LED_AMARILLO, LOW);
    digitalWrite ( LED_ROJO, LOW);
    
  }
  
   else if ( value > 50 && value <= 75 ) {
    
  digitalWrite ( LED_VERDE, LOW);
    digitalWrite ( LED_AZUL, HIGH);
    digitalWrite ( LED_BLANCO, LOW);
    digitalWrite ( LED_AMARILLO, LOW);
    digitalWrite ( LED_ROJO, LOW);
    
  }
  
   else if ( value > 75 && value <= 100 ) {
    
  digitalWrite ( LED_VERDE, HIGH);
    digitalWrite ( LED_AZUL, LOW);
    digitalWrite ( LED_BLANCO, LOW);
    digitalWrite ( LED_AMARILLO, LOW);
    digitalWrite ( LED_ROJO, LOW);
    
  }
  
}
*/

/* Modo de abreviar la función mediante macros */

void indicator ( uint8_t value )  {
  
  digitalWrite( LED_ROJO, controlRojo( value ) );
  digitalWrite( LED_AMARILLO, controlAmarillo ( value ) );
  digitalWrite( LED_BLANCO, controlBlanco ( value ) );
  digitalWrite( LED_AZUL, controlAzul ( value ) );
  digitalWrite( LED_VERDE, controlVerde ( value ) );  
  
}

void loop( void ) {
  lcd.backlight ( );        /*~ Encender la luz de fondo. ~*/
    delay ( 1000 );         /*~ Esperar 1 segundo. ~*/
  
  lcd.noBacklight ( );      /*~ Apagar la luz de fondo. ~*/
    delay ( 1000 );         /*~ Esperar 1 segundo. ~*/
  lcd.backlight ( );        /*~ Encender la luz de fondo. ~*/
  
   
   lcd.setCursor ( 5, 0 );    /*~ ( columnas, filas) Ubicamos el cursor en la primera posición(columna:0) de la primera línea(fila:0) ~*/
   lcd.write ( 0 );         /*~ Mostramos nuestro primer icono o caracter ~*/
    delay ( 1000 );       /*~ Esperar 1 segundo ~*/
   //lcd.clear ( );       /*~ Limpiar pantalla ~*/
  // lcd.setCursor ( 0, 1 );    /*~ ( columnas, filas) Ubicamos el cursor en la primera posición(columna:0) de la segunda línea(fila:1) ~*/
   //lcd.write ( 1 );           /*~ Mostramos nuestro segundo icono o caracter ~*/
    //delay ( 1000 );       /*~ Esperar 1 segundo ~*/
   //lcd.clear ( );       /*~ Limpiar pantalla ~*/
  
   lcd.setCursor ( 0, 0 );    /*~ ( columnas, filas) Ubicamos el cursor en la primera posición(columna:0) de la primera línea(fila:0) ~*/
   //lcd.print ( Soilmoisture); /*~ Mostrar una cadena de texto (no exceder 16 caracteres por línea)~*/
   delay ( 1000 );        /*~ Esperar 1 segundo ~*/         
    
  //for ( uint8_t i = 0; i < ( 40 ); i++ ) {  /*~ Este ciclo es para que se vea como se recorren los caracteres, si no es colocado se vería muy rápido ~*/
    
    //lcd.scrollDisplayRight ( ); /*~ Recorrer caracteres de derecha a izquierda ~*/
    //delay ( 100 );        /*~ Esperar 100 milisegundos ~*/
    
 //}
  
  //for ( uint8_t i = 0; i < ( 40 ); i++ ) {  /*~ Este ciclo es para que se vea como se recorren los caracteres, si no es colocado se vería muy rápido ~*/
    
   // lcd.scrollDisplayLeft ( ); /*~ Recorrer caracteres de izquierda a derecha ~*/
   // delay ( 100 );        /*~ Esperar 100 milisegundos ~*/
    
  //}
  
  lcd.clear ( );
  //Serial.println ( (float)analogRead ( A0 ) *100 / 876 );
  Soilmoisture = getSoilmoisture ( );   // Se obttiene el porcentaje de humedad.
  /* Se muestra en la consola serial. */
  Serial.print ( "La humedad del suelo es de: " );
  Serial.print ( Soilmoisture );
  Serial.println ( " %" );
  lcd.print ( Soilmoisture);
  /* Se manda encender los LEDs de acuerdo al nivel de humedad. */
  indicator ( Soilmoisture );
  
  delay ( 250 );
  
}
