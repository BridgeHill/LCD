#ifndef DISPLAY_S_H
#define DISPLAY_S_H

#include "display.h"
#include "data4test.h"

class Display_S:public Display
{
public:
    Display_S(int ComCtr,int PinCtr,Data4Test *D4T);
    virtual void Key_Display();
    virtual void Roll_Display();
};

#endif // DISPLAY_TEST_H

