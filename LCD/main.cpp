#include "lcd.h"
#include "pad.h"
#include "display.h"
#include "display_s.h"

#include <QApplication>
#include "main.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString DIR_IMG(":/img/meterlcd.bmp");
    int xmlPosX=50,xmlPosY=50,xmlComCtr=1,xmlPinCtr=9;
    LoadXML(DIR_IMG,xmlPosX,xmlPosY,xmlComCtr,xmlPinCtr);

    QFile testfile(DIR_IMG);
    if(!testfile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Failed to open files DIR_IMG";
        return -11;
    }
    if((xmlPosX<0)||(xmlPosY<0)||(xmlComCtr<=0)||(xmlPinCtr<=0))
    {
        qDebug("input error");
        return -12;
    }
    Data4Test *D4T = new Data4Test;
    Display_S *Disp_temp= new Display_S(xmlComCtr,xmlPinCtr,D4T);

    Lcd lcd1(0,DIR_IMG,xmlPosX,xmlPosY,xmlComCtr,xmlPinCtr,Disp_temp);
    lcd1.show();

    pad pad1(0,Disp_temp);
    pad1.show();

    return a.exec();
}
