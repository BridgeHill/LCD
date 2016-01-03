#include "segsdisplay.h"

Segsdisplay::Segsdisplay():ComPinTable()
{

}

Segsdisplay::~Segsdisplay()
{

}

void Segsdisplay::DisplayNum(int data,int num)
{
    //检查输入的有效性//无效报错
    for(int tempi=0;tempi<8;tempi++)
    {
        int temppos = Seg_Num[num][tempi];//得到位置
        if(SegMask[data] & ((0x01)<< tempi))
            Set(temppos);
        else
            Clr(temppos);
    }
}

int Segsdisplay::DisplayLed1(bool light){return (light)?(Set(Seg_Led1)):(Clr(Seg_Led1));}

