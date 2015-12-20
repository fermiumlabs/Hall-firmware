/*
   Human Machine Interface abstraction library
   For Hall and RDT apparatus
   Copyright 2015 fermiumlabs.com
   All Rights Reserved
   Written by: Davide Bortolami
 */

//12.34  mA||
//1234.45 O||Vr G=200
//-123.3 °c||P   100%
//12.34   V||Vh G=200


#ifndef HMI_abstraction_h
#define HMI_abstraction_h

#include "Arduino.h"
#include <LiquidCrystal.h>

//LiquidCrystal(rs, enable, d4, d5, d6, d7)
LiquidCrystal _lcd_unabstracted(1, 2, 3, 4, 5, 6); //cambiare numeri pin


class HMI_abstraction {
public:

        void Begin (void)
        {
                memset(_LCD_array, 0, (sizeof(_LCD_array)/sizeof(_LCD_array[0]))); //clear the array

                //lcd init code
                _lcd_unabstracted.begin(20, 4);
                _lcd_unabstracted.setCursor(0, 0);
                _lcd_unabstracted.noCursor();

                //create a custom char for omega
                byte _char_omega[8] = {
                        0b00000,
                        0b01110,
                        0b10001,
                        0b11011,
                        0b01010,
                        0b11011,
                        0b00000,
                        0b00000
                };
                _lcd_unabstracted.createChar(0, _char_omega);

        }

        //custom bootscreen for LCD
        void boot(void)
        {
                //the boot bypass our interface
                _lcd_unabstracted.setCursor(0, 0);
                _lcd_unabstracted.print(F("    Hall  Effect    "));
                _lcd_unabstracted.setCursor(0, 1);
                _lcd_unabstracted.print(F("Fermium Laboratories"));
                _lcd_unabstracted.setCursor(0, 2);
                _lcd_unabstracted.print(F("web: fermiumlabs.com"));
                _lcd_unabstracted.setCursor(0, 3);
                _lcd_unabstracted.print(F("Sample:   Ge P-doped"));
        }

        //writes a string to the LCD array
        void WriteString(char column, char row, char *src)
        {
                char lenght = strlen(src);
                for (char i=0; i != lenght; i++)
                {
                        _LCD_array[column+i][row][1] = src[i];
                }

        }

        //write a string to a custom position on the LCD
        void Write (char position, char* src)
        {
                char srctemp[10];
                strncpy(srctemp, src, strlen(src));

                switch (position) {
                case 1:
                        ClearPos(0,0,8);
                        WriteString(0,0,srctemp);
                        WriteString(6,0,"mA");
                        break;
                case 2:
                        break;
                case 3:
                        ClearPos(0,1,8);
                        WriteString(0,1,srctemp);
                        _LCD_array[6][1][1] = 0; // simbolo omega
                        break;
                case 4:
                        ClearPos(11,1,8);
                        WriteString(16,1,srctemp);
                        WriteString(18,1,"Vr G=");
                        break;
                case 5:
                        ClearPos(0,2,8);
                        WriteString(0,2,srctemp);
                        WriteString(7,2,"°c");
                        break;
                case 6:
                        ClearPos(11,2,8);
                        WriteString(18,2,"P      %%");
                        WriteString(15,2,srctemp);
                        break;
                case 7:
                        ClearPos(0,0,8);
                        WriteString(0,3,srctemp);
                        WriteString(7,3,"V");
                        break;
                case 8:
                        ClearPos(11,3,8);
                        WriteString(16,3,srctemp);
                        WriteString(18,3,"Vh G=");
                        break;
                default:
                        break;


                }

                Update();
        }

private:
        char _LCD_array[20][4][2];


        // clear X characters starting from a position
        void ClearPos (char startcolumn, char startrow, char lenght)
        {
                for (int i=0; i!= lenght; i++)
                {
                        _LCD_array[startcolumn+i][startrow][1] = 0;
                }
        }

        //update from MCU memory to LCD
        void Update (void)
        {
                for (char column=0; column != 20; column++)
                {
                        for (char row=0; row!=4; row++)
                        {
                                if(_LCD_array[column][row][1] != _LCD_array[column][row][0] )                 //if new value if different from old
                                {
                                        //lcd set cursor at column, row
                                        _lcd_unabstracted.setCursor(column, row);
                                        _lcd_unabstracted.write(_LCD_array[column][row][1]);
                                        _LCD_array[column][row][0] = _LCD_array[column][row][1];
                                }
                        }
                }
        }



};


#endif HMI_abstraction_h