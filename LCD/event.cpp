#include "lcd.h"
#include "ui_lcd.h"
extern QString QPoint2QS(const QPoint &point);

void Lcd::mousePressEvent(QMouseEvent *event)
{
    int tempX = event->pos().x() - ImgPosX;
    int tempY = event->pos().y() - ImgPosY;
    if((tempX>=0)&&(tempY>=0)&&(tempX<LcdImage->width())&&(tempY<LcdImage->height()))
    {
        if(SettingMode == Mode)
        {
            if(property("TableEditabled").toBool())
            {
                if(ViewSeg_XY->currentIndex().isValid())
                {
                    QStandardItem *curModelItem = ModelSeg_XY->item\
                            (ViewSeg_XY->currentIndex().row(),ViewSeg_XY->currentIndex().column());

                    QPoint point1(event->pos().x(),event->pos().y());
                    int tempsum = curModelItem->data(Qt::UserRole + 1).toInt();
                    curModelItem->setData(QVariant(point1),Qt::UserRole+1+1+tempsum);
                    curModelItem->setData(tempsum+1,Qt::UserRole + 1);//更改内容长度
                    tempsum = curModelItem->data(Qt::UserRole + 1).toInt();//取更改后的长度
                    QString tempQS;
                    for(int i=0;i<tempsum;i++)
                    {
                        tempQS += QPoint2QS(curModelItem->data(Qt::UserRole+1+1+i).toPoint());
                    }
                    curModelItem->setText(tempQS);

        //            ViewSeg_XY->resizeColumnsToContents();
        //            ViewSeg_XY->resizeRowsToContents();
                    this->update();
                }
            }
        }else if(CheckMode == Mode)
        {
            Point4Check->setX(event->pos().x());
            Point4Check->setY(event->pos().y());
            this->update();
        }
    }
}


void Lcd::paintEvent(QPaintEvent *event)
{
    QTime time1,time2,time3;

    //复位TableView的底色
    for(int row=0;row<LcdComCtr;row++)
        for(int column=0;column<LcdPinCtr;column++)
    {
        QStandardItem *TempModelItem = ModelSeg_XY->item(row,column);
        TempModelItem->setBackground(QBrush(QColor(BACKGRONDCOLOR)));
    }

    LcdBmpPaint = new QPainter(this);
    if(SettingMode == Mode)
    {//设置模式下
        ViewSeg_XY->show();
        ui->pushButton->show();
        ui->pushButton_2->show();
        QImage *LcdImage1 = new QImage(*LcdImage);
        QPoint BmpPos(ImgPosX,ImgPosY);
        LcdBmpPaint->drawImage(BmpPos,*LcdImage1);
        delete LcdImage1;
        QColor RedCol(255,0,0,255);
        QPen PenR;
        PenR.setColor(RedCol);
        PenR.setWidth(7);
        LcdBmpPaint->setPen(PenR);

        QPen PenB;
        PenB.setColor(QColor(0,0,255,255));
        PenB.setWidth(7);

        //根据表格内容绘制图片
        for(int row=0;row<LcdComCtr;row++)
            for(int column=0;column<LcdPinCtr;column++)
            {
                if((row == ViewSeg_XY->currentIndex().row())&&(column == ViewSeg_XY->currentIndex().column()) )
                    LcdBmpPaint->setPen(PenB);
                QStandardItem *tempptr = ModelSeg_XY->item(row,column);
                int tempsum = tempptr->data(Qt::UserRole + 1).toInt();
                for(int i=0;i<tempsum;i++)
                {
                    QPoint point1 = tempptr->data(Qt::UserRole +1+1+i).toPoint();
                    LcdBmpPaint->drawPoint(point1);
                }
                LcdBmpPaint->setPen(PenR);
            }
    }
    else if(DisplayMode == Mode)    //显示模式
    {
        ViewSeg_XY->hide();
        ui->pushButton->hide();
        ui->pushButton_2->hide();
    //根据Scrn->Table 和 Mode  更改QImage 然后显示出来

        QImage *LcdImage2 = new QImage(*LcdImage);

        for(int row=0;row<LcdComCtr;row++)
            for(int column=0;column<LcdPinCtr;column++)
            {//对坐标点染色
                if(Disp->Driver.Content(row,column) == 1)
                {
                    QStandardItem *tempptr = ModelSeg_XY->item(row,column);
                    int tempsum = tempptr->data(Qt::UserRole + 1).toInt();
                    for(int i=0;i<tempsum;i++)
                    {
                        QPoint point1 = tempptr->data(Qt::UserRole + 1+1+i).toPoint();
                        int tempX = point1.x()- ImgPosX;
                        int tempY = point1.y()- ImgPosY;
                        if((tempX>=0)&&(tempY>=0)&&(tempX<LcdImage2->width())&&(tempY<LcdImage2->height()))
                        {
                            point1.setX(tempX);//偏移出图片和窗口的相对位置
                            point1.setY(tempY);
                            if(FRONTCOLOR == LcdImage2->pixel(point1))//check this
                            {
                                LcdImage2->setPixel(point1,DIRTYCOLOR);
                            }
                        }
                    }
                }
            }

        long long dirtytimes = 0;
        do
        {
            time1.start(); //开始计时，以ms为单位
            dirtytimes = 0;
            //对染色的点使用算法
            for(long  x=1;x < LcdImage2->width()-1;x++)
                for(long y=1;y < LcdImage2->height()-1;y++)
                {
                    if(DIRTYCOLOR == LcdImage2->pixel(x,y))
                    {//将染色点的四周 前景色点 染色

                        if(FRONTCOLOR == LcdImage2->pixel(x,y-1))
                        {
                            LcdImage2->setPixel(x,y-1,DIRTYCOLOR);
                            dirtytimes++;
                        }


                        if(FRONTCOLOR == LcdImage2->pixel(x,y+1))
                        {
                            LcdImage2->setPixel(x,y+1,DIRTYCOLOR);
                            dirtytimes++;
                        }


                        if(FRONTCOLOR == LcdImage2->pixel(x-1,y))
                        {
                            LcdImage2->setPixel(x-1,y,DIRTYCOLOR);
                            dirtytimes++;
                        }


                        if(FRONTCOLOR == LcdImage2->pixel(x+1,y))
                        {
                            LcdImage2->setPixel(x+1,y,DIRTYCOLOR);
                            dirtytimes++;
                        }

                    }
                }
        qDebug("dirtytimes1:%d",dirtytimes);
            time2.start(); //开始计时，以ms为单位
            for(long  x=LcdImage2->width()-2;x>=1;x--)
                for(long y=LcdImage2->height()-2;y >=1;y--)
                {
                    if(DIRTYCOLOR == LcdImage2->pixel(x,y))
                    {//将染色点的四周 前景色点 染色

                        if(FRONTCOLOR == LcdImage2->pixel(x,y-1))
                        {
                            LcdImage2->setPixel(x,y-1,DIRTYCOLOR);
                            dirtytimes++;
                        }


                        if(FRONTCOLOR == LcdImage2->pixel(x,y+1))
                        {
                            LcdImage2->setPixel(x,y+1,DIRTYCOLOR);
                            dirtytimes++;
                        }


                        if(FRONTCOLOR == LcdImage2->pixel(x-1,y))
                        {
                            LcdImage2->setPixel(x-1,y,DIRTYCOLOR);
                            dirtytimes++;
                        }


                        if(FRONTCOLOR == LcdImage2->pixel(x+1,y))
                        {
                            LcdImage2->setPixel(x+1,y,DIRTYCOLOR);
                            dirtytimes++;
                        }

                    }
                }
        qDebug("dirtytimes2:%d",dirtytimes);
            for(long  x=1;x < LcdImage2->width()-1;x++)
                for(long y=LcdImage2->height()-2;y >=1;y--)
                {
                    if(DIRTYCOLOR == LcdImage2->pixel(x,y))
                    {//将染色点的四周 前景色点 染色

                        if(FRONTCOLOR == LcdImage2->pixel(x,y-1))
                        {
                            LcdImage2->setPixel(x,y-1,DIRTYCOLOR);
                            dirtytimes++;
                        }


                        if(FRONTCOLOR == LcdImage2->pixel(x,y+1))
                        {
                            LcdImage2->setPixel(x,y+1,DIRTYCOLOR);
                            dirtytimes++;
                        }


                        if(FRONTCOLOR == LcdImage2->pixel(x-1,y))
                        {
                            LcdImage2->setPixel(x-1,y,DIRTYCOLOR);
                            dirtytimes++;
                        }


                        if(FRONTCOLOR == LcdImage2->pixel(x+1,y))
                        {
                            LcdImage2->setPixel(x+1,y,DIRTYCOLOR);
                            dirtytimes++;
                        }

                    }
                }
        qDebug("dirtytimes3:%d",dirtytimes);
            for(long  x=LcdImage2->width()-2;x>=1;x--)
                for(long y=1;y<LcdImage2->height()-1;y++)
                {
                    if(DIRTYCOLOR == LcdImage2->pixel(x,y))
                    {//将染色点的四周 前景色点 染色

                        if(FRONTCOLOR == LcdImage2->pixel(x,y-1))
                        {
                            LcdImage2->setPixel(x,y-1,DIRTYCOLOR);
                            dirtytimes++;
                        }


                        if(FRONTCOLOR == LcdImage2->pixel(x,y+1))
                        {
                            LcdImage2->setPixel(x,y+1,DIRTYCOLOR);
                            dirtytimes++;
                        }


                        if(FRONTCOLOR == LcdImage2->pixel(x-1,y))
                        {
                            LcdImage2->setPixel(x-1,y,DIRTYCOLOR);
                            dirtytimes++;
                        }


                        if(FRONTCOLOR == LcdImage2->pixel(x+1,y))
                        {
                            LcdImage2->setPixel(x+1,y,DIRTYCOLOR);
                            dirtytimes++;
                        }
                    }
                }
        qDebug("dirtytimes4:%d",dirtytimes);
        }while(0&&dirtytimes);//while(dirtytimes);速度优化

        for(long  x=0;x < LcdImage2->width();x++)
            for(long y=0;y < LcdImage2->height();y++)
            {//对所有非染色点 用背景色填充
                if(DIRTYCOLOR != LcdImage2->pixel(x,y))
                {
                    LcdImage2->setPixel(x,y,BACKGRONDCOLOR);
                }
            }

        for(long  x=0;x < LcdImage2->width();x++)
            for(long y=0;y < LcdImage2->height();y++)
            {//对所有染色点 用前景色填充
                if(DIRTYCOLOR == LcdImage2->pixel(x,y))
                {
                    LcdImage2->setPixel(x,y,FRONTCOLOR);
                }
            }
        time3.start(); //开始计时，以ms为单位
        QPoint BmpPos2(ImgPosX,ImgPosY);
        LcdBmpPaint->drawImage(BmpPos2,*LcdImage2);
//        LcdBmpPaint->drawImage(BmpPos2,LcdImage2->scaledToHeight(700));
        delete LcdImage2;
    }else if(CheckMode == Mode)
    {//检查模式下

        ViewSeg_XY->show();
        ui->pushButton->hide();
        ui->pushButton_2->hide();
        QImage *LcdImage2 = new QImage(*LcdImage);

        int tempX0 = Point4Check->x()-ImgPosX;
        int tempY0 = Point4Check->y()-ImgPosY;
        if((tempX0>=0)&&(tempY0>=0)&&(tempX0<LcdImage2->width())&&(tempY0<LcdImage2->height()))
        {
            QPoint point0(tempX0,tempY0);
            if(BACKGRONDCOLOR == LcdImage2->pixel(point0))
            {//
                ;
            }
            else if(FRONTCOLOR == LcdImage2->pixel(point0))
            {
                LcdImage2->setPixel(point0,DIRTYCOLOR);
                long long dirtytimes = 0;
                do
                {
                    dirtytimes = 0;
                    //对染色的点使用算法
                    for(long  x=0;x < LcdImage2->width();x++)
                        for(long y=0;y < LcdImage2->height();y++)
                        {
                            if(DIRTYCOLOR == LcdImage2->pixel(x,y))
                            {//将染色点的四周 前景色点 染色
                                if(y>0)
                                {
                                //    qDebug("(x,y-1)color is :%x",LcdImage2->pixel(x,y-1));
                                    if(FRONTCOLOR == LcdImage2->pixel(x,y-1))
                                    {
                                        LcdImage2->setPixel(x,y-1,DIRTYCOLOR);
                                        dirtytimes++;
                                    }
                                }
                                if(y < (LcdImage2->height()-1))
                                {
                                    if(FRONTCOLOR == LcdImage2->pixel(x,y+1))
                                    {
                                        LcdImage2->setPixel(x,y+1,DIRTYCOLOR);
                                        dirtytimes++;
                                    }
                                }
                                if(x>0)
                                {
                                    if(FRONTCOLOR == LcdImage2->pixel(x-1,y))
                                    {
                                        LcdImage2->setPixel(x-1,y,DIRTYCOLOR);
                                        dirtytimes++;
                                    }
                                }
                                if(x < (LcdImage2->width()-1))
                                {
                                    if(FRONTCOLOR == LcdImage2->pixel(x+1,y))
                                    {
                                        LcdImage2->setPixel(x+1,y,DIRTYCOLOR);
                                        dirtytimes++;
                                    }
                                }
                            }
                        }

                    for(long  x=0;x < LcdImage2->width();x++)
                        for(long y=LcdImage2->height()-1;y >=0;y--)
                        {
                            if(DIRTYCOLOR == LcdImage2->pixel(x,y))
                            {//将染色点的四周 前景色点 染色
                                if(y>0)
                                {
                                //    qDebug("(x,y-1)color is :%x",LcdImage2->pixel(x,y-1));
                                    if(FRONTCOLOR == LcdImage2->pixel(x,y-1))
                                    {
                                        LcdImage2->setPixel(x,y-1,DIRTYCOLOR);
                                        dirtytimes++;
                                    }
                                }
                                if(y < (LcdImage2->height()-1))
                                {
                                    if(FRONTCOLOR == LcdImage2->pixel(x,y+1))
                                    {
                                        LcdImage2->setPixel(x,y+1,DIRTYCOLOR);
                                        dirtytimes++;
                                    }
                                }
                                if(x>0)
                                {
                                    if(FRONTCOLOR == LcdImage2->pixel(x-1,y))
                                    {
                                        LcdImage2->setPixel(x-1,y,DIRTYCOLOR);
                                        dirtytimes++;
                                    }
                                }
                                if(x < (LcdImage2->width()-1))
                                {
                                    if(FRONTCOLOR == LcdImage2->pixel(x+1,y))
                                    {
                                        LcdImage2->setPixel(x+1,y,DIRTYCOLOR);
                                        dirtytimes++;
                                    }
                                }
                            }
                        }

                    for(long  x=LcdImage2->width()-1;x>=0;x--)
                        for(long y=LcdImage2->height()-1;y >=0;y--)
                        {
                            if(DIRTYCOLOR == LcdImage2->pixel(x,y))
                            {//将染色点的四周 前景色点 染色
                                if(y>0)
                                {
                                //    qDebug("(x,y-1)color is :%x",LcdImage2->pixel(x,y-1));
                                    if(FRONTCOLOR == LcdImage2->pixel(x,y-1))
                                    {
                                        LcdImage2->setPixel(x,y-1,DIRTYCOLOR);
                                        dirtytimes++;
                                    }
                                }
                                if(y < (LcdImage2->height()-1))
                                {
                                    if(FRONTCOLOR == LcdImage2->pixel(x,y+1))
                                    {
                                        LcdImage2->setPixel(x,y+1,DIRTYCOLOR);
                                        dirtytimes++;
                                    }
                                }
                                if(x>0)
                                {
                                    if(FRONTCOLOR == LcdImage2->pixel(x-1,y))
                                    {
                                        LcdImage2->setPixel(x-1,y,DIRTYCOLOR);
                                        dirtytimes++;
                                    }
                                }
                                if(x < (LcdImage2->width()-1))
                                {
                                    if(FRONTCOLOR == LcdImage2->pixel(x+1,y))
                                    {
                                        LcdImage2->setPixel(x+1,y,DIRTYCOLOR);
                                        dirtytimes++;
                                    }
                                }
                            }
                        }

                    for(long  x=LcdImage2->width()-1;x>=0;x--)
                        for(long y=0;y<LcdImage2->height();y++)
                        {
                            if(DIRTYCOLOR == LcdImage2->pixel(x,y))
                            {//将染色点的四周 前景色点 染色
                                if(y>0)
                                {
                                //    qDebug("(x,y-1)color is :%x",LcdImage2->pixel(x,y-1));
                                    if(FRONTCOLOR == LcdImage2->pixel(x,y-1))
                                    {
                                        LcdImage2->setPixel(x,y-1,DIRTYCOLOR);
                                        dirtytimes++;
                                    }
                                }
                                if(y < (LcdImage2->height()-1))
                                {
                                    if(FRONTCOLOR == LcdImage2->pixel(x,y+1))
                                    {
                                        LcdImage2->setPixel(x,y+1,DIRTYCOLOR);
                                        dirtytimes++;
                                    }
                                }
                                if(x>0)
                                {
                                    if(FRONTCOLOR == LcdImage2->pixel(x-1,y))
                                    {
                                        LcdImage2->setPixel(x-1,y,DIRTYCOLOR);
                                        dirtytimes++;
                                    }
                                }
                                if(x < (LcdImage2->width()-1))
                                {
                                    if(FRONTCOLOR == LcdImage2->pixel(x+1,y))
                                    {
                                        LcdImage2->setPixel(x+1,y,DIRTYCOLOR);
                                        dirtytimes++;
                                    }
                                }
                            }
                        }

                }while(dirtytimes);

                for(int row=0;row<LcdComCtr;row++)
                    for(int column=0;column<LcdPinCtr;column++)
                    {
                        QStandardItem *tempptr = ModelSeg_XY->item(row,column);
                        int tempsum = tempptr->data(Qt::UserRole + 1).toInt();
                        for(int i=0;i<tempsum;i++)
                        {
                            QPoint point1 = tempptr->data(Qt::UserRole + 1+1+i).toPoint();
                            int tempX = point1.x()- ImgPosX;
                            int tempY = point1.y()- ImgPosY;
                            if((tempX>=0)&&(tempY>=0)&&(tempX<LcdImage2->width())&&(tempY<LcdImage2->height()))
                            {
                                point1.setX(tempX);//偏移出图片和窗口的相对位置
                                point1.setY(tempY);
                                if(DIRTYCOLOR == LcdImage2->pixel(point1))//check this
                                {//检测Table对应的坐标处的颜色,如被染色,突显单元格,画点
                                    QStandardItem *PickedModelItem = ModelSeg_XY->item(row,column);
                                    PickedModelItem->setBackground(QBrush(QColor(PICKEDCOLOR)));

                                    int tempsum = tempptr->data(Qt::UserRole + 1).toInt();
                                    for(int i=0;i<tempsum;i++)
                                    {
                                        QPoint point2 = tempptr->data(Qt::UserRole + 1+1+i).toPoint();
                                        int tempX = point2.x()- ImgPosX;
                                        int tempY = point2.y()- ImgPosY;
                                        if((tempX>=0)&&(tempY>=0)&&(tempX<LcdImage2->width())&&(tempY<LcdImage2->height()))
                                        {
                                            point1.setX(tempX);//偏移出图片和窗口的相对位置
                                            point1.setY(tempY);
                                            if(FRONTCOLOR == LcdImage2->pixel(point1))//check this
                                            {
                                                LcdImage2->setPixel(point1,DIRTYCOLOR);
                                            }
                                        }
                                    }

                                    long long dirtytimes = 0;
                                    do
                                    {
                                        dirtytimes = 0;
                                        //对染色的点使用算法
                                        for(long  x=0;x < LcdImage2->width();x++)
                                            for(long y=0;y < LcdImage2->height();y++)
                                            {
                                                if(DIRTYCOLOR == LcdImage2->pixel(x,y))
                                                {//将染色点的四周 前景色点 染色
                                                    if(y>0)
                                                    {
                                                    //    qDebug("(x,y-1)color is :%x",LcdImage2->pixel(x,y-1));
                                                        if(FRONTCOLOR == LcdImage2->pixel(x,y-1))
                                                        {
                                                            LcdImage2->setPixel(x,y-1,DIRTYCOLOR);
                                                            dirtytimes++;
                                                        }
                                                    }
                                                    if(y < (LcdImage2->height()-1))
                                                    {
                                                        if(FRONTCOLOR == LcdImage2->pixel(x,y+1))
                                                        {
                                                            LcdImage2->setPixel(x,y+1,DIRTYCOLOR);
                                                            dirtytimes++;
                                                        }
                                                    }
                                                    if(x>0)
                                                    {
                                                        if(FRONTCOLOR == LcdImage2->pixel(x-1,y))
                                                        {
                                                            LcdImage2->setPixel(x-1,y,DIRTYCOLOR);
                                                            dirtytimes++;
                                                        }
                                                    }
                                                    if(x < (LcdImage2->width()-1))
                                                    {
                                                        if(FRONTCOLOR == LcdImage2->pixel(x+1,y))
                                                        {
                                                            LcdImage2->setPixel(x+1,y,DIRTYCOLOR);
                                                            dirtytimes++;
                                                        }
                                                    }
                                                }
                                            }

                                        for(long  x=0;x < LcdImage2->width();x++)
                                            for(long y=LcdImage2->height()-1;y >=0;y--)
                                            {
                                                if(DIRTYCOLOR == LcdImage2->pixel(x,y))
                                                {//将染色点的四周 前景色点 染色
                                                    if(y>0)
                                                    {
                                                    //    qDebug("(x,y-1)color is :%x",LcdImage2->pixel(x,y-1));
                                                        if(FRONTCOLOR == LcdImage2->pixel(x,y-1))
                                                        {
                                                            LcdImage2->setPixel(x,y-1,DIRTYCOLOR);
                                                            dirtytimes++;
                                                        }
                                                    }
                                                    if(y < (LcdImage2->height()-1))
                                                    {
                                                        if(FRONTCOLOR == LcdImage2->pixel(x,y+1))
                                                        {
                                                            LcdImage2->setPixel(x,y+1,DIRTYCOLOR);
                                                            dirtytimes++;
                                                        }
                                                    }
                                                    if(x>0)
                                                    {
                                                        if(FRONTCOLOR == LcdImage2->pixel(x-1,y))
                                                        {
                                                            LcdImage2->setPixel(x-1,y,DIRTYCOLOR);
                                                            dirtytimes++;
                                                        }
                                                    }
                                                    if(x < (LcdImage2->width()-1))
                                                    {
                                                        if(FRONTCOLOR == LcdImage2->pixel(x+1,y))
                                                        {
                                                            LcdImage2->setPixel(x+1,y,DIRTYCOLOR);
                                                            dirtytimes++;
                                                        }
                                                    }
                                                }
                                            }

                                        for(long  x=LcdImage2->width()-1;x>=0;x--)
                                            for(long y=LcdImage2->height()-1;y >=0;y--)
                                            {
                                                if(DIRTYCOLOR == LcdImage2->pixel(x,y))
                                                {//将染色点的四周 前景色点 染色
                                                    if(y>0)
                                                    {
                                                    //    qDebug("(x,y-1)color is :%x",LcdImage2->pixel(x,y-1));
                                                        if(FRONTCOLOR == LcdImage2->pixel(x,y-1))
                                                        {
                                                            LcdImage2->setPixel(x,y-1,DIRTYCOLOR);
                                                            dirtytimes++;
                                                        }
                                                    }
                                                    if(y < (LcdImage2->height()-1))
                                                    {
                                                        if(FRONTCOLOR == LcdImage2->pixel(x,y+1))
                                                        {
                                                            LcdImage2->setPixel(x,y+1,DIRTYCOLOR);
                                                            dirtytimes++;
                                                        }
                                                    }
                                                    if(x>0)
                                                    {
                                                        if(FRONTCOLOR == LcdImage2->pixel(x-1,y))
                                                        {
                                                            LcdImage2->setPixel(x-1,y,DIRTYCOLOR);
                                                            dirtytimes++;
                                                        }
                                                    }
                                                    if(x < (LcdImage2->width()-1))
                                                    {
                                                        if(FRONTCOLOR == LcdImage2->pixel(x+1,y))
                                                        {
                                                            LcdImage2->setPixel(x+1,y,DIRTYCOLOR);
                                                            dirtytimes++;
                                                        }
                                                    }
                                                }
                                            }

                                        for(long  x=LcdImage2->width()-1;x>=0;x--)
                                            for(long y=0;y<LcdImage2->height();y++)
                                            {
                                                if(DIRTYCOLOR == LcdImage2->pixel(x,y))
                                                {//将染色点的四周 前景色点 染色
                                                    if(y>0)
                                                    {
                                                    //    qDebug("(x,y-1)color is :%x",LcdImage2->pixel(x,y-1));
                                                        if(FRONTCOLOR == LcdImage2->pixel(x,y-1))
                                                        {
                                                            LcdImage2->setPixel(x,y-1,DIRTYCOLOR);
                                                            dirtytimes++;
                                                        }
                                                    }
                                                    if(y < (LcdImage2->height()-1))
                                                    {
                                                        if(FRONTCOLOR == LcdImage2->pixel(x,y+1))
                                                        {
                                                            LcdImage2->setPixel(x,y+1,DIRTYCOLOR);
                                                            dirtytimes++;
                                                        }
                                                    }
                                                    if(x>0)
                                                    {
                                                        if(FRONTCOLOR == LcdImage2->pixel(x-1,y))
                                                        {
                                                            LcdImage2->setPixel(x-1,y,DIRTYCOLOR);
                                                            dirtytimes++;
                                                        }
                                                    }
                                                    if(x < (LcdImage2->width()-1))
                                                    {
                                                        if(FRONTCOLOR == LcdImage2->pixel(x+1,y))
                                                        {
                                                            LcdImage2->setPixel(x+1,y,DIRTYCOLOR);
                                                            dirtytimes++;
                                                        }
                                                    }
                                                }
                                            }

                                    }while(0&&dirtytimes);//while(dirtytimes);
                                }
                            }
                        }
                    }
            }

        }

        QPoint BmpPos(ImgPosX,ImgPosY);
        LcdBmpPaint->drawImage(BmpPos,*LcdImage2);
        delete LcdImage2;

        QPen PenR;
        PenR.setColor(QColor(255,0,0,255));
        PenR.setWidth(7);

        QPen PenB;
        PenB.setColor(QColor(0,0,255,255));
        PenB.setWidth(7);

        LcdBmpPaint->setPen(PenR);
        LcdBmpPaint->drawPoint(*Point4Check);
    }

    LcdBmpPaint->end();
    delete LcdBmpPaint;

    int time_Diff1 = time1.elapsed(); //返回从上次start()或restart()开始以来的时间差，单位ms
    double f1 = time_Diff1/1000.0;
    qDebug("use time1:%fs",f1);

    int time_Diff2 = time2.elapsed(); //返回从上次start()或restart()开始以来的时间差，单位ms
    double f2 = time_Diff2/1000.0;
    qDebug("use time2:%fs",f2);

    int time_Diff3 = time3.elapsed(); //返回从上次start()或restart()开始以来的时间差，单位ms
    double f3 = time_Diff3/1000.0;
    qDebug("use time3:%fs",f3);

}

