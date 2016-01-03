#ifndef MAIN_H
#define MAIN_H

#define LCD_XML_DIR (":/xml/lcdini.xml")//配置文件路径

#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#include <QTextStream>
#include "data4test.h"

void LoadXML(QString &DIR_IMG,int &PosX,int &PosY,int &ComCtr,int &PinCtr)
{
    //打开配置文件用于读取xml
    QFile xmlfile(LCD_XML_DIR);
    if(!xmlfile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Failed to open files LCD_XML_DIR";
        return ;
    }
    QDomDocument LcdIni;
    if(!LcdIni.setContent(&xmlfile))
    {
        qDebug() << "Failed to load document";
        return ;
    }
    xmlfile.close();

    QDomNodeList Dir = LcdIni.elementsByTagName("DIR");
    DIR_IMG = Dir.item(0).toElement().attributeNode("LCD_IMG_DIR").value();

    QDomNodeList Pos = LcdIni.elementsByTagName("POS");
    PosX = Pos.item(0).toElement().attributeNode("POSX").value().toInt();
    PosY = Pos.item(0).toElement().attributeNode("POSY").value().toInt();

    QDomNodeList Ctr = LcdIni.elementsByTagName("CTR");
    ComCtr = Ctr.item(0).toElement().attributeNode("COMCTR").value().toInt();
    PinCtr = Ctr.item(0).toElement().attributeNode("PINCTR").value().toInt();

}
#endif // MAIN_H

