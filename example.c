/* 
 * File:   example.c
 * Author: Jason McGuire (j.mcguire.2015@ieee.org)
 * Description: A library for the DS18B20 Temperature Sensor
 * Using the Dallas OneWire communication protocol.
 * See example.c for implementation of the library
 * Sample Product: https://www.sparkfun.com/products/11050
 * Created on April 20, 2016, 9:08 AM
 
License Information:
This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
 
 
 */
 
 #include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <p18f25k20.h>
#include <math.h>
#include <string.h>
#include "LCD4bit.h"
#include "DS18B20.h"


// CONFIG1H
#pragma config FOSC = INTIO67   // Oscillator Selection bits
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit

// CONFIG2L
#pragma config PWRT = ON          // Power-up Timer Enable bit
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits
#pragma config BORV = 30        // Brown Out Reset Voltage bits

// CONFIG2H
#pragma config WDTEN = OFF      // Watchdog Timer Enable bit
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits

// CONFIG3H
#pragma config CCP2MX = PORTC   // CCP2 MUX bit 
#pragma config PBADEN = ON      // PORTB A/D Enable bit
#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator Enable bit
#pragma config HFOFST = OFF     // HFINTOSC Fast Start-up
#pragma config MCLRE = OFF       // MCLR Pin Enable bit

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit
#pragma config XINST = OFF      // Extended Instruction Set Enable bit

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection Block 0
#pragma config CP1 = OFF        // Code Protection Block 1
#pragma config CP2 = OFF        // Code Protection Block 2
#pragma config CP3 = OFF        // Code Protection Block 3

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit
#pragma config CPD = OFF        // Data EEPROM Code Protection bit

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection Block 0
#pragma config WRT1 = OFF       // Write Protection Block 1
#pragma config WRT2 = OFF       // Write Protection Block 2
#pragma config WRT3 = OFF       // Write Protection Block 3

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit
#pragma config WRTB = OFF       // Boot Block Write Protection bit
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection Block 0
#pragma config EBTR1 = OFF      // Table Read Protection Block 1
#pragma config EBTR2 = OFF      // Table Read Protection Block 2
#pragma config EBTR3 = OFF      // Table Read Protection Block 3

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit

#define _XTAL_FREQ  8000000

char str[80];
void main(void)
{
    
    // Set up RA5
    ANS4 = 0; // RA5 is a digital input
    
    OSCCON = 0b01101100; // device goes to sleep on instruction, 8MHz clock
    TRISB=0;                             // CLEAR PORTB
    PORTB=0;                             // Configure PORTB as output
    TRISC=0;                             // Clear PORTC
    PORTC=0x0F;                         // Configure Port C as I/O port
    lcd_init();                              // LCD initialization
    float temp;
    
    while(1)
    {
        temp = OneWireTemp();
        temp = temp*9/5 + 32; // convert to F 
        lcdclr();
        sprintf(str,"Temp: %3.2fF", temp);
        firstline(); // Set cursor to first line
        lcdstr(str); // Print the first string
        for(int i=0; i<10; i++){
            __delay_ms(50); // Pause for a second
        }
    }
}