#ifndef LCDDRIVER_H
#define LCDDRIVER_H
#include <QObject>
#include <QVector>
#include "segname_compin.h"

class LcdDriver:public QObject
{
    Q_OBJECT

public:
    LcdDriver(int ComCtr,int PinCtr);
    ~LcdDriver();
    void Update();
signals:
    void updatescrean(int);
public:
    void DisplayData(int data);

private:
    void DisplaySign(bool sign);
    void DisplayNum(int data,int num,bool dp);

public:
    int Set(short c1,short p1);
    int Clr(short c1,short p1);
    int Roll(short c1,short p1);
    int Set(long c1p1);
    int Clr(long c1p1);
    int Roll(long c1p1);
    void ClrAll();
    void SetAll();

public:
    int Content(short c1,short p1);
    int Content(long c1p1);
    const int LcdComCtr;
    const int LcdPinCtr;

private:
    QVector<int> *Table;//0不显示  1 显示 -1闪烁
};

#endif // LCDDRIVER_H


