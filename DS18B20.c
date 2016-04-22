/* 
 * File:   DS18B20.c
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
 
 #include "DS18B20.h"

// Available Functions:
float OneWireTemp(void); // Returns the temperature in celsius 
unsigned int OneWireReset(void); // Sends a reset pulse to the sensor 
void OneWireWriteBit(unsigned char); // write a single bit to the OneWire
unsigned char OneWireReadBit(void); // reads a single bit 
void OneWireWriteByte(unsigned char); // writes a byte 
unsigned char OneWireReadByte(void); // reads a byte 
unsigned char OneWireRead(void); // reads the current status of the bus
void OneWireHigh(void); // sets the bus high
void OneWireRelease(void); // releases the bus 



float OneWireTemp(){
    
    OneWireReset(); // Reset Pulse 
    OneWireWriteByte(0xCC); // Issue skip ROM command (CCh)
    OneWireWriteByte(0x44); // Convert T command (44h)
    while(!BUSIN); // DS will hold line low while making measurement
    OneWireReset(); // Start new command sequence 
    OneWireWriteByte(0xCC); // Issue skip ROM command 
    OneWireWriteByte(0xBE); // Read Scratchpad (BEh) - 15 bits
    unsigned char LSB = OneWireReadByte();
    unsigned char MSB = OneWireReadByte();
    OneWireReset(); // Stop Reading 
    unsigned int data = MSB;
    float temperature = (data << 8) | LSB;
    return (temperature/16); 
   
}

void OneWireHigh(){
    BUSDIR = 0; // Set as output
    BUSOUT = 1; // Set high 
}

void OneWireRelease(){
    BUSDIR = 0; // Set as output
    BUSOUT = 0; // Set low 
}

unsigned char OneWireRead(){
    return BUSIN;
}

unsigned int OneWireReset(){
    OneWireRelease();
    __delay_us(240); // 480uS Delay
    __delay_us(240);
    OneWireHigh();
    __delay_us(70); // wait 70 uS before reading
    unsigned int OW = OneWireRead(); // check for OneWire
    __delay_us(205); // 410 uS delay
    __delay_us(205);
    OneWireHigh(); // give bus back to OneWire
    return OW; 
}


void OneWireWriteBit(unsigned char b){
    if(b){
        OneWireRelease();
        __delay_us(6); // wait 6uS
        OneWireHigh();
        __delay_us(64); // wait 64uS
    }
    else{
        OneWireRelease();
        __delay_us(60); // wait 60uS
        OneWireHigh();
        __delay_us(10); // wait 10uS
    }
}

unsigned char OneWireReadBit(){
    OneWireRelease();
    __delay_us(6); // wait 6uS
    OneWireHigh();
    __delay_us(9); // wait 9uS
    unsigned char out = OneWireRead();
    __delay_us(55); // wait 55uS
    return out;
}

void OneWireWriteByte(unsigned char b){
    for(int i = 0; i < 8; i++){
        OneWireWriteBit(b & 0x01); // send LS bit first 
        b = b >> 1;
    }
}

unsigned char OneWireReadByte(void){
    unsigned char out; 
    for(int i = 0; i < 8; i++){ // read in LS bit first
        out = out >> 1; // get out ready for next bit
        if(OneWireReadBit() & 0x01) // if its a one 
            out = out | 0x80; // place a 1 
    }
    return out;
}