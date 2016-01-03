#include "lcddriver.h"
#include <math.h>
#include <QtMath>

LcdDriver::LcdDriver(const int ComCtr,const int PinCtr):\
    LcdComCtr(ComCtr),LcdPinCtr(PinCtr)
{
    Table = new QVector<int>(ComCtr*PinCtr);
    Table->fill(0);
}

LcdDriver::~LcdDriver()
{
    delete Table;
}

int LcdDriver::Set(short c1,short p1)
{
    int *data = Table->data();
    return (data[c1*LcdPinCtr+p1] = 1);
}
int LcdDriver::Clr(short c1,short p1)
{
    int *data = Table->data();
    return(data[c1*LcdPinCtr+p1] = 0);
}
int LcdDriver::Roll(short c1,short p1)
{
    int *data = Table->data();
    return(data[c1*LcdPinCtr+p1] = -1);
}

int LcdDriver::Set(long c1p1)
{
    int *data = Table->data();
    data[(c1p1>>16)*LcdPinCtr+(c1p1&0xffff)] = 1;
    return 1;
}
int LcdDriver::Clr(long c1p1)
{
    int *data = Table->data();
    return(data[(c1p1>>16)*LcdPinCtr+(c1p1&0xffff)] = 0);
}
int LcdDriver::Roll(long c1p1)
{
    int *data = Table->data();
    return(data[(c1p1>>16)*LcdPinCtr+(c1p1&0xffff)] = -1);
}

void LcdDriver::ClrAll()
{
    Table->fill(0);
}

void LcdDriver::SetAll()
{
    Table->fill(1);
}

int LcdDriver::Content(short c1,short p1)
{
    int *data = Table->data();
    return data[c1*LcdPinCtr+p1];
}
int LcdDriver::Content(long c1p1)
{
    int *data = Table->data();
    return data[(c1p1>>16)*LcdPinCtr+(c1p1&0xffff)];
}

void LcdDriver::DisplaySign(bool sign)
{
    if(sign)
        Set(SEG_S53);
    else
        Clr(SEG_S53);
}

void LcdDriver::DisplayNum(int data,int num,bool dp)
{
    for(int tempi=0;tempi<8;tempi++)
    {
        if((0!=num)||(0!=tempi))////第一个数码管没有小数点,掩码无效
        {
            long temppos = Seg_Num[num][tempi];//得到位
            if(SegMask[data] & ((0x01)<< tempi))
            {
                Set(temppos);
            }
            else
            {
                Clr(temppos);
            }
        }
    }

    if(num!=0)
    {//第一个数码管没有小数点
        if(dp)
            Set(Seg_Num[num][0]);
        else
            Clr(Seg_Num[num][0]);
    }
}
void LcdDriver::DisplayData(int data)
{
    if((data>99999999)||(data<-99999999))
    {
        qDebug("too big or too small");
        return;
    }
    int temp = (data>0)?(data):(-data);
    int displaytimes = 0;
    for(int i=7;i>=0;i--)
    {
        int ForDivMod = 1;
        for(int j=0;j<i;j++)
            ForDivMod *= 10;
        int num = temp / ForDivMod;
        if(num||displaytimes)
        {
            DisplayNum(num,i,false);
            displaytimes++;
        }
        temp = temp % ForDivMod;
    }

    if(displaytimes == 0)
        DisplayNum(0,0,false);
    DisplaySign(data<0);
}

void LcdDriver::Update()
{
    emit updatescrean(1);
}
