/*
 ********************************************************************************
Universidad del Valle de Guatemala
 * curso: Electronica Digital 2
Autor: Gabriel Andrade
compilador: XC8
proyecto: laboratorio 02
hardware: PIC 16F887
creado: 24/01/2023
última modificación: 25/01/2023
 ********************************************************************************
 */

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF        // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF          // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF         // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF       // Brown Out Reset Selection bits (BOR enabled)
#pragma config IESO = OFF        // Internal External Switchover bit (Internal/External Switchover mode is enabled)
#pragma config FCMEN = OFF       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
#pragma config LVP = OFF         // Low Voltage Programming Enable bit (RB3/PGM pin has PGM function, low voltage programming enabled)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

#define _XTAL_FREQ 4000000

////////////////////////////////////////////////////////////////////////////////
// LIBRERIAS 
////////////////////////////////////////////////////////////////////////////////

#include <xc.h>
#include <stdio.h>

#include "adc.h"            // Libreria para el manejo del ADC
#include "LCD.h"            // Libreria para el control de la pantalla LCD
#include "map_function.h"   // Libreria de conversiones
#include "uart.h"           // Libreria para transmicion y recepcion de datos

////////////////////////////////////////////////////////////////////////////////
// VARIABLES 
////////////////////////////////////////////////////////////////////////////////

//VARIABLES PARA USO DE LA PANTALLA LCD
char buffer_LCD[30];        //buffer para las cadenas de la pantalla lcd 
unsigned char contador = 0; //variable para contador de la terminal

//VARIABLES PARA MANEJO DEL ADC Y VISUALIZACION 
unsigned char valor_ch0;    //variable para guardar resultado del ADC 1
unsigned char valor_ch1;    //variable para guardar el resultado del ADC 2
unsigned char voltaje1;     //valor mapeado del ADC 1 
unsigned char voltaje2;     //valor mapeado del ADC 2 
unsigned char unidad_V1;    //Unidades del ADC 1 mostrado como voltaje 
unsigned char decima_V1;    //decimas del ADC 1 mostrado como voltaje
unsigned char centesima_V1; //centesimas del ADC 1 mostrado como voltaje 
unsigned char unidad_V2;    //Unidades del ADC 2 mostrado como voltaje 
unsigned char decima_V2;    //decimas del ADC 2 mostrado como voltaje
unsigned char centesima_V2; //centesimas del ADC 2 mostrado como voltaje

//VARIABLES PARA USO DEL UART
char buffer_UART[30];       //buffer para las cadenas de transmicion del UART 
char dato_rx;               //dato recibido de la terminal

////////////////////////////////////////////////////////////////////////////////
// PROTOTIPOS DE FUNCIONES
////////////////////////////////////////////////////////////////////////////////
void setupINTOSC(void);
void ADC_CONV_V(void);
void VISUAL_LCD(void);
void TX_RX(void);

////////////////////////////////////////////////////////////////////////////////
// CODIGO PRINCIPAL
////////////////////////////////////////////////////////////////////////////////

void main()
{
    setupINTOSC();      // Configuracion del oscilador 
    
    //Inicializacion de los registros 
    ADC_Init(AN0);      // Inicializa el ADC para el puerto AN0
    ADC_Init(AN1);      // Inicializa el ADC para el puerto AN1
    Uart_Init(9600);    // Inicializa el UART a 9600 baudios 
    Lcd_Init();         // Inicializa la pantalla LCD  
    Lcd_Clear(); //Limpiar pantalla
    
    //bucle infinito 
    while(1)
    {
        ADC_CONV_V();       // Conversion del valor del ADC a voltaje 
        VISUAL_LCD();       // Mensajes que visualizar en la pantalla LCD
        TX_RX();            // Recepcion y transmicion de mensajes 
    }
}

////////////////////////////////////////////////////////////////////////////////
// FUNCIONES
////////////////////////////////////////////////////////////////////////////////

void setupINTOSC(void) 
{   
    ANSELH = 0;
    ANSELbits.ANS0 = 1;
    ANSELbits.ANS1 = 1;
    
    PORTA = 0;
    
    
    //puerto del contador (salida)
    TRISB = 0;
    PORTB = 0;
    
    TRISD = 0; 
    PORTD = 0;
    
    TRISC = 0; 
    PORTC = 0;
    

    // oscilador a 4MHz
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 0;
    
     //Seleccion de Oscilador interno
    OSCCONbits.SCS = 1;
}

void ADC_CONV_V(void)
{
  valor_ch0 = ADC_Read(0);    // Lectura del canal 0
  valor_ch1 = ADC_Read(1);    // Lectura del canal 1
  voltaje1 = map(valor_ch0, 0, 255, 0, 100);  //mapeo
  voltaje2 = map(valor_ch1, 0, 255, 0, 100);  //mapeo 
          
  //conversiones para mostrar el ADC 1 como voltaje  
  unidad_V1 = (voltaje1*5)/100;         // unidades voltaje 1
  decima_V1 = ((voltaje1*5)/10)%10;     // decimas voltaje 1
  centesima_V1 = (voltaje1*5)%10;       // centesimas voltaje 1 
       
  //conversiones para mostrar el ADC 2 como voltaje 
  unidad_V2 = (voltaje2*5)/100;         // unidades voltaje 2
  decima_V2 = ((voltaje2*5)/10)%10;     // decimas voltaje 2
  centesima_V2 = (voltaje2*5)%10;       // centesimas voltaje 2 
}

void VISUAL_LCD(void)
{
    
 Lcd_Set_Cursor(1,1);   //direccion donde se escribira 
 Lcd_Write_String( "S1:    S2: cont:");  //imprimir mensaje
        
 Lcd_Set_Cursor(2,1);   //direccion donde se escribira 
 sprintf(buffer_LCD, "%u.%u%u  %u.%u%u    %u  ", unidad_V1, decima_V1, //mensaje 
 centesima_V1, unidad_V2, decima_V2, centesima_V2, contador);   //variables 
 Lcd_Write_String(buffer_LCD);  //imprimir mensaje 
        
 __delay_ms(150);   //delay de funcionamiento 
}

void TX_RX(void)
{
sprintf(buffer_UART, "Valor ADC1: %u , Valor ADC2: %u\r\n", //mensaje a transmitir
         valor_ch0, valor_ch1); //variables
 Uart_Send_String(buffer_UART); //mandar mensaje 
 __delay_ms(150);               //delay de funcionamiento 
        
        if(Uart_Available() >0) //verifica si se esta mandando un dato 
        {
            dato_rx = Uart_Read(); //guardamos en una variable el dato
            if(dato_rx == '+') //verificamos el caracter
            {
                contador = contador + 1; //si se cumple incrementamos 
            }
            if(dato_rx == '-') //verificamor el caracter
            {
                contador = contador - 1; // si se cumple decrementamos 
            }
        }
        PORTE = contador;   // asignamos el valor del contador al puerto 
}