#ifndef PAD_H
#define PAD_H

#include <QWidget>
#include <display.h>
#include <display_s.h>

namespace Ui {
class pad;
}

class pad : public QWidget
{
    Q_OBJECT

public:
    explicit pad(QWidget *parent = 0,Display *Disp_=0);
    ~pad();

private slots:
    void on_PageUpButton_clicked();

    void on_ButtonPageDown_clicked();

private:
    Ui::pad *ui;
    Display *Disp;
};

#endif // PAD_H
