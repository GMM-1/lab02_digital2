/*
 ********************************************************************************
Universidad del Valle de Guatemala
 * curso: Electronica Digital 2
Autor: Gabriel Andrade
compilador: XC8
proyecto: laboratorio 02
hardware: PIC 16F887
creado: 24/01/2023
última modificación: 
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
#include <xc.h>
#include <stdio.h>

#include "adc.h"                        // Libreria para el manejo del ADC
#include "lcd.h"                        // Libreria para el control de la pantalla LCD
#include "map_function.h"               // Libreria de conversiones

char buffer[20];

////////////////////////////////////////////////////////////////////////////////
// PROTOTIPOS DE FUNCIONES
////////////////////////////////////////////////////////////////////////////////
void setupINTOSC(void);

////////////////////////////////////////////////////////////////////////////////
// CODIGO PRINCIPAL
////////////////////////////////////////////////////////////////////////////////

void main()
{
    setupINTOSC();        // Configuracion del oscilador 
    ADC_Init(AN0);       // Inicializa el ADC para el puerto AN0
    ADC_Init(AN1);      // Inicializa el ADC para el puerto AN1
    Lcd_Init();        // Inicializa la pantalla LCD
    
    while(1)
    {
        unsigned char contador = 0;
        unsigned char valor_ch0 = ADC_Read(0);    // Lectura del canal 0
        unsigned char valor_ch1 = ADC_Read(1);    // Lectura del canal 1
        unsigned char voltaje1 = map(valor_ch0, 0, 255, 0, 100);  //conversion %
        unsigned char voltaje2 = map(valor_ch1, 0, 255, 0, 100);  //conversion %
        
       
        unsigned char unidad_V1 = (voltaje1*5)/100;
        unsigned char decima_V1 = ((voltaje1*5)/10)%10;
        unsigned char centesima_V1 = (voltaje1*5)%10; 
        
        unsigned char unidad_V2 = (voltaje2*5)/100;
        unsigned char decima_V2 = ((voltaje2*5)/10)%10;
        unsigned char centesima_V2 = (voltaje2*5)%10; 
        
        Lcd_Set_Cursor(1,1);
        sprintf(buffer, "S1:    S2:    cont:");
        Lcd_Write_String(buffer);
        
        Lcd_Set_Cursor(2,1);
        sprintf(buffer, "%u.%u%u  %u.%u%u    %u  ", unidad_V1, decima_V1, 
                centesima_V1, unidad_V2, decima_V2, centesima_V2, contador);
        Lcd_Write_String(buffer);
        
        Lcd_Set_Cursor(3,1);
        sprintf(buffer, " voltajes y contador ");
        Lcd_Write_String(buffer);
        
        Lcd_Set_Cursor(4,1);
        sprintf(buffer, "   LABORATORIO 02   ");
        Lcd_Write_String(buffer);
        __delay_ms(150);
    }
}

////////////////////////////////////////////////////////////////////////////////
// FUNCIONES
////////////////////////////////////////////////////////////////////////////////

void setupINTOSC(void) {
    //Seleccion de Oscilador interno
    OSCCONbits.SCS = 1;

    // oscilador a 4MHz
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 0;
}
