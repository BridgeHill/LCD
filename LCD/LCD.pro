#-------------------------------------------------
#
# Project created by QtCreator 2015-02-02T23:49:34
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LCD
TEMPLATE = app


SOURCES += main.cpp\
    data4test.cpp \
    event.cpp \
    lcddriver.cpp \
    Lcd.cpp \
    display.cpp \
    display_s.cpp \
    pad.cpp

HEADERS  += \
    segname_compin.h \
    main.h \
    data4test.h \
    lcddriver.h \
    lcd.h \
    display.h \
    display_s.h \
    pad.h


FORMS    += \
    lcd.ui \
    pad.ui

RESOURCES += \
    bmp.qrc
