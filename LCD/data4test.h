#ifndef DATA4TEST
#define DATA4TEST

#include <QObject>
#include <QTimer>

class Data4Test : public QObject
{
    Q_OBJECT
public:
    Data4Test();
public slots:
    void MySlots();
private:
    QTimer *timer;

public:
    int datacount;
};

#endif // DATA4TEST

