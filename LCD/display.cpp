#include "display.h"


Display::Display(int ComCtr,int PinCtr):Driver(ComCtr,PinCtr)
{
    ;
}
void Display::Display_Null()
{
    Driver.ClrAll();
    Driver.Update();
}
void Display::Display_Full()
{
    Driver.SetAll();
    Driver.Update();
}
