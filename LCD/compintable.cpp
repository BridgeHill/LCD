#include "compintable.h"


ComPinTable::ComPinTable()
{
    for(int c1 = ComBegin;c1 <= ComEnd;c1++)
        for(int p1 = PinBegin;p1 <= PinEnd;p1++)
            Table[c1][p1] = 0;
}

ComPinTable::ComPinTable(int mode)
{
    int temp = (mode>0)?(1):((mode==0)?(0):(-1));
    for(int c1 = ComBegin;c1 <= ComEnd;c1++)
        for(int p1 = PinBegin;p1 <= PinEnd;p1++)
        {
            Table[c1][p1] = temp;
        }
}

ComPinTable::~ComPinTable()
{

}

int ComPinTable::Set(int c1,int p1)
{
    return(Table[c1][p1] = 1);
}
int ComPinTable::Clr(int c1,int p1)
{
    return(Table[c1][p1] = 0);
}
int ComPinTable::Roll(int c1,int p1)//闪烁
{
    return(Table[c1][p1] = -1);
}

int ComPinTable::Set(int c1p1)
{
    return(Table[c1p1>>8][c1p1&&0xff] = 1);
}
int ComPinTable::Clr(int c1p1)
{
    return(Table[c1p1>>8][c1p1&&0xff] = 0);
}
int ComPinTable::Roll(int c1p1)//闪烁
{
    return(Table[c1p1>>8][c1p1&&0xff] = -1);
}

void ComPinTable::ClrAll()
{
    for(int c1=ComBegin;c1<=ComEnd;c1++)
        for(int p1=PinBegin;p1<=PinEnd;p1++)
        {
            Table[c1][p1] = 0;
        }
}

void ComPinTable::SetAll()
{
    for(int c1=ComBegin;c1<=ComEnd;c1++)
        for(int p1=PinBegin;p1<=PinEnd;p1++)
        {
            Table[c1][p1] = 1;
        }
}

int ComPinTable::Content(int c1,int p1)
{
    return Table[c1][p1];
}
