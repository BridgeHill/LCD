#include "lcd.h"
#include "ui_lcd.h"

Lcd::Lcd(QWidget *parent,QString DIR_IMG,int PosX,int PosY,\
                       int ComCtr,int PinCtr,Display *Disp_) :
    QMainWindow(parent),LCD_IMG_DIR(DIR_IMG),\
    ImgPosX(PosX),ImgPosY(PosY),LcdComCtr(ComCtr),\
    LcdPinCtr(PinCtr),Disp(Disp_),ui(new Ui::Lcd)
{
    ui->setupUi(this);
//    this->setFixedSize(1000,700);
    ModelSeg_XY = new QStandardItemModel(this);
    InitSegXY_Model();
    ClrSegXY_Model();
    ViewSeg_XY = new QTableView(this);
    SetSegXY_View(ModelSeg_XY);
//    Scrn = new LcdDriver(LcdComCtr,LcdPinCtr);
//    Disp = new Display_S(LcdComCtr,LcdPinCtr,D4T);
    Mode = SettingMode;
    Point4Check = new QPoint(0,0);
//    Mode= DisplayMode;
    connect(ViewSeg_XY,SIGNAL(clicked(const QModelIndex &)),this,SLOT(slotPaintAtXY()));
//    connect(Scrn,SIGNAL(updatescrean(int)),this,SLOT(on_updatescrean()));
//    Scrn->Screan2();
    connect(&(Disp->Driver),SIGNAL(updatescrean(int)),this,SLOT(on_updatescrean()));

    LcdImage = new QImage;
    LcdImage->load(LCD_IMG_DIR);
    CurScreanNum = 0;
    //========================
    machine = new QStateMachine;

    QState *DisableEdit = new QState();
    DisableEdit->assignProperty((ui->pushButton_2), "text", "Disabled");
    DisableEdit->setObjectName("Disable");
    DisableEdit->assignProperty(this,"TableEditabled",false);

    QState *EnableEdit = new QState();
    EnableEdit->assignProperty((ui->pushButton_2), "text", "Enabled");
    EnableEdit->setObjectName("Enable");
    EnableEdit->assignProperty(this,"TableEditabled",true);

    DisableEdit->addTransition((ui->pushButton_2), SIGNAL(clicked()), EnableEdit);
    EnableEdit->addTransition((ui->pushButton_2), SIGNAL(clicked()), DisableEdit);
    machine->addState(EnableEdit);
    machine->addState(DisableEdit);
    machine->setInitialState(DisableEdit);
    machine->start();
    //========================
}

Lcd::~Lcd()
{
    delete ui;
    delete ViewSeg_XY;
    delete ModelSeg_XY;
//    delete Scrn;
//    delete Disp;
    delete Point4Check;
    delete LcdImage;
    delete machine;
}

QString QPoint2QS(const QPoint &point)
{
    int pixel_x = point.x();
    int pixel_y = point.y();
    return QString("(")+QString::number(pixel_x)\
            +QString(",")+QString::number(pixel_y)\
            +QString(")");
}

void Lcd::InitSegXY_Model()
{
    QStringList Hlabel;
    for(int i = 1;i <= LcdPinCtr;i++)
    {
        QString SegStem = QString("PIN") + QString::number(i);
        Hlabel.push_back(SegStem);
    }
    int HlabelCnt = Hlabel.count();
    ModelSeg_XY->setColumnCount(HlabelCnt);
    ModelSeg_XY->setHorizontalHeaderLabels(Hlabel);
    QStringList Vlabel;
    for(int i = 1;i <= LcdComCtr;i++)
    {
        QString ComStem = QString("COM") + QString::number(i);
        Vlabel.push_back(ComStem);
    }
    int VlabelCnt = Vlabel.count();
    ModelSeg_XY->setRowCount(VlabelCnt);
    ModelSeg_XY->setVerticalHeaderLabels(Vlabel);
}

void Lcd::ClrSegXY_Model()
{
    for(int row=0;row<LcdComCtr;row++)
        for(int column=0;column<LcdPinCtr;column++)
        {
            QStandardItem *item1 = new QStandardItem();
            item1->setData(QVariant(0),Qt::UserRole + 1);//内容个数置0
            item1->setText(QString(""));
            ModelSeg_XY->setItem(row,column,item1);
        }
}

void Lcd::SetSegXY_View(QStandardItemModel *ModelSeg_XY)
{
    ViewSeg_XY->setModel(ModelSeg_XY);
    ViewSeg_XY->setGeometry(1100,40,250,300);
    ViewSeg_XY->resizeColumnsToContents();
    ViewSeg_XY->resizeRowsToContents();
    ViewSeg_XY->setEditTriggers(QAbstractItemView::NoEditTriggers);//禁止双击修改
    ViewSeg_XY->setSelectionMode(QAbstractItemView::SingleSelection);//单个选中//
}

void Lcd::slotPaintAtXY()
{
    this->update();
}

void Lcd::on_actionSave_triggered()
{
    QDomDocument LcdIni;
    QDomProcessingInstruction instruction = LcdIni.createProcessingInstruction\
            ("xml","version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"");
    LcdIni.appendChild(instruction);

    QDomElement Lcd_1 = LcdIni.createElement("Lcd_1");
    LcdIni.appendChild(Lcd_1);

    QDomElement Dir = LcdIni.createElement("DIR");
    Dir.setAttribute("LCD_IMG_DIR",QString(LCD_IMG_DIR));
    Lcd_1.appendChild(Dir);

    QDomElement Pos = LcdIni.createElement("POS");
    Pos.setAttribute("POSX",QString::number(ImgPosX));
    Pos.setAttribute("POSY",QString::number(ImgPosY));
    Lcd_1.appendChild(Pos);

    QDomElement Ctr = LcdIni.createElement("CTR");
    Ctr.setAttribute("COMCTR",QString::number(LcdComCtr));
    Ctr.setAttribute("PINCTR",QString::number(LcdPinCtr));
    Lcd_1.appendChild(Ctr);

    QDomElement Segments = LcdIni.createElement("SEGMENTS");
    Lcd_1.appendChild(Segments);
    for(int row=0;row<LcdComCtr;row++)
        for(int column=0;column<LcdPinCtr;column++)
        {
            QStandardItem *tempptr = ModelSeg_XY->item(row,column);
            int tempsum = tempptr->data(Qt::UserRole + 1).toInt();

            QDomElement Segment = LcdIni.createElement("SEGMENT");
            Segment.setAttribute("COM",QString::number(row));
            Segment.setAttribute("PIN",QString::number(column));
            Segment.setAttribute("NUMBER",QString::number(tempsum));
            for(int i=0;i<tempsum;i++)
            {
                QPoint point1 = tempptr->data(Qt::UserRole+1+1+i).toPoint();
                int tempX = point1.x()- ImgPosX;
                int tempY = point1.y()- ImgPosY;
                Segment.setAttribute(QString("X")+QString::number(i),QString::number(tempX));
                Segment.setAttribute(QString("Y")+QString::number(i),QString::number(tempY));
            }
            Segments.appendChild(Segment);
        }
    QFile xmlfile(":/xml/lcdini.xml");
    if(!xmlfile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Failed to open file for writing";
        return ;
    }
    else
    {
        QTextStream out(&xmlfile);
        out.setCodec("UTF-8");
        out << LcdIni.toString();
        out.flush();
        xmlfile.close();
        qDebug() << "Finished";
    }
}

void Lcd::on_actionSettingMode_triggered()
{
    Mode = SettingMode;
    this->update();
}

void Lcd::on_actionDisplayMode_2_triggered()
{
    Mode = DisplayMode;
    this->update();
}

void Lcd::on_updatescrean()
{
    this->update();
}

void Lcd::on_pushButton_clicked()
{
    if(property("TableEditabled").toBool())
    {
        if(ViewSeg_XY->currentIndex().isValid())
        {
            QStandardItem *curModelItem = ModelSeg_XY->item\
                    (ViewSeg_XY->currentIndex().row(),ViewSeg_XY->currentIndex().column());
            curModelItem->setData(QVariant(0),Qt::UserRole + 1);//内容个数置0
            curModelItem->setText(QString(""));
        //    ViewSeg_XY->resizeColumnsToContents();
        //    ViewSeg_XY->resizeRowsToContents();
            this->update();
        }
    }
}

void Lcd::on_actionLoad_triggered()
{
    //打开文件用于读取xml
    QFile xmlfile(":/xml/lcdini.xml");
    if(!xmlfile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Failed to open files xml";
        return ;
    }
    QDomDocument LcdIni;
    if(!LcdIni.setContent(&xmlfile))
    {
        qDebug() << "Failed to load document";
        return ;
    }
    xmlfile.close();

    QDomNodeList Segment = LcdIni.elementsByTagName("SEGMENT");

    for(int i=0;i<Segment.count();i++)
    {
        int row = Segment.item(i).toElement().attributeNode("COM").value().toInt();
        int column = Segment.item(i).toElement().attributeNode("PIN").value().toInt();
        int tempnumber = Segment.item(i).toElement().attributeNode("NUMBER").value().toInt();

        QStandardItem *tempptr = ModelSeg_XY->item(row,column);
        tempptr->setData(tempnumber,Qt::UserRole + 1);

        for(int j=0;j<tempnumber;j++)
        {
            int tempx = Segment.item(i).toElement().attributeNode(QString("X")+QString::number(j)).value().toInt();
            int tempy = Segment.item(i).toElement().attributeNode(QString("Y")+QString::number(j)).value().toInt();
            QPoint temppoint(tempx+ImgPosX,tempy+ImgPosY);
            tempptr->setData(temppoint,Qt::UserRole + 1+1+j);
        }

        QString tempQS;
        for(int k=0;k<tempnumber;k++)
        {
            tempQS += QPoint2QS(tempptr->data(Qt::UserRole+1+1+k).toPoint());
        }
        tempptr->setText(tempQS);
    }

    ViewSeg_XY->resizeColumnsToContents();
    ViewSeg_XY->resizeRowsToContents();
    this->update();
}

void Lcd::on_actionCheckMode_triggered()
{
    Mode = CheckMode;
    this->update();
}



