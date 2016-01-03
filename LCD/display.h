#ifndef DISPLAY_H
#define DISPLAY_H

#include "lcddriver.h"

class Display
{
  public:
           Display(int ComCtr,int PinCtr);
           virtual void Key_Display()=0;
           virtual void Roll_Display()=0;

           void Display_Null();
           void Display_Full();

//  protected:
  public:
           LcdDriver Driver;
 };

#endif // DISPLAY_H

