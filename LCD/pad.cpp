#include "pad.h"
#include "ui_pad.h"

pad::pad(QWidget *parent,Display *Disp_) :
    QWidget(parent),Disp(Disp_),
    ui(new Ui::pad)
{
    ui->setupUi(this);
}

pad::~pad()
{
    delete ui;
}

void pad::on_PageUpButton_clicked()
{
    Disp->Display_Full();
}

void pad::on_ButtonPageDown_clicked()
{
    Disp->Display_Null();
}
