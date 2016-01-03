#include "data4test.h"

Data4Test::Data4Test():datacount(0)
{
    timer = new QTimer(this);   //新建一个定时器

    connect(timer,SIGNAL(timeout()),this,SLOT(MySlots()));

    timer->start(1000);
}

void Data4Test::MySlots()
{
    datacount++;
}

