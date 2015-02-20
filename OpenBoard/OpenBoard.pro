#-------------------------------------------------
#
# Project created by QtCreator 2015-02-08T15:10:16
#
#-------------------------------------------------

QT       += core gui opengl widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OpenBoard
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp \
    settings.cpp

HEADERS  += mainwindow.h \
    glwidget.h \
    settings.h

FORMS    += mainwindow.ui

RESOURCES += \
    icons.qrc
    RC_FILE = myapp.rc
