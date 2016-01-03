#ifndef COMPINTABLE_H
#define COMPINTABLE_H

#include "segname_compin.h"

class ComPinTable
{
public:
    ComPinTable();
    ComPinTable(int mode);
    ~ComPinTable();
    int Set(int c1,int p1);
    int Clr(int c1,int p1);
    int Roll(int c1,int p1);
    int Set(int c1p1);
    int Clr(int c1p1);
    int Roll(int c1p1);
    void ClrAll();
    void SetAll();
    int Content(int c1,int p1);
private:
    int Table[COMCTR][PINCTR];//0不显示  1 显示 -1闪烁
};

#endif // COMPINTABLE_H
