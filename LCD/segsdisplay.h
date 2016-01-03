#ifndef SEGSDISPLAY_H
#define SEGSDISPLAY_H

#include "segname_compin.h"
#include "compintable.h"

class Segsdisplay:public ComPinTable
{
public:
    Segsdisplay();
    ~Segsdisplay();
    void DisplayNum(int data,int num);
    int DisplayLed1(bool light);
};

#endif // SEGSDISPLAY_H

