#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "segname_compin.h"
#include <QMainWindow>
#include <qDebug>
#include <QTableWidget>
#include <QWidget>
#include <QStandardItemModel>
#include <QString>
#include <QMouseEvent>
#include <QPoint>
#include <QPainter>
#include <QImage>
#include <lcddriver.h>

#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#include <QTextStream>

#include <QProcess>
#include <QDir>
#include "data4test.h"

#include <QStateMachine>
#include <QTime>
#include "display_s.h"
#include "display.h"

enum WorkMode{ DisplayMode = 0,SettingMode=1,CheckMode=2};

namespace Ui {
class Lcd;
}

class Lcd : public QMainWindow
{
    Q_OBJECT
    Q_PROPERTY(bool TableEditabled READ isTableEditabled WRITE setTableEditabled )

public:
    //Q_INVOKABLE
    bool isTableEditabled(){return V_TableEditabled;}
    void setTableEditabled(bool tempin){V_TableEditabled = tempin;}
private:
    bool V_TableEditabled;

public:
    explicit Lcd(QWidget *parent = 0,\
                        QString DIR_IMG=QString(""),\
                        int PosX=0,int PosY=0,\
                        int ComCtr=0,int PinCtr=0,\
                        Display *Disp_=0);
    ~Lcd();
    void InitSegXY_Model();
    void ClrSegXY_Model();
    void SetSegXY_View(QStandardItemModel *ModelSeg_XY);
public slots:
    void slotPaintAtXY();
private:
    QTableView *ViewSeg_XY;
    QStandardItemModel *ModelSeg_XY;
    Ui::Lcd *ui;
    enum WorkMode Mode;
    QPainter *LcdBmpPaint;
//    LcdDriver *Scrn;
    Display *Disp;
    const QString LCD_IMG_DIR;
    const int ImgPosX,ImgPosY;
    const int LcdComCtr,LcdPinCtr;
    Data4Test *data4test;
    QPoint *Point4Check;
    QStateMachine *machine;
    QImage *LcdImage;
    int CurScreanNum;
protected:
    void paintEvent(QPaintEvent *event = 0);
    void mousePressEvent(QMouseEvent *event = 0);
private slots:
    void on_actionSave_triggered();
    void on_actionLoad_triggered();
    void on_actionSettingMode_triggered();
    void on_actionDisplayMode_2_triggered();
    void on_actionCheckMode_triggered();
    void on_updatescrean();
    void on_pushButton_clicked();
};

#endif // MAINWINDOW_H
