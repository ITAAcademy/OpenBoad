QT       += widgets core gui opengl quick widgets quickwidgets #av avwidgets

SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp \
    settings.cpp \
    qmlwidget.cpp


HEADERS  += mainwindow.h \
    glwidget.h \
    settings.h \
    qmlwidget.h

FORMS    += mainwindow.ui

RESOURCES += \
    icons.qrc
    RC_FILE = myapp.rc


greaterThan(QT_MAJOR_VERSION, 4) {
    LIBS += -L../../libsQt5
} else {
    LIBS += -L../../libsQt4
}

unix {
    DEFINES += QTMEL_NOT_INSTALLED
    LIBS += -lpthread -lX11
    QMAKE_CXXFLAGS += -std=c++0x
}

CONFIG(debug, debug|release) {
    win32: LIBS += -lqtmeld1
    unix: LIBS += -lqtmeld
} else {
    win32: LIBS += -lqtmel1
    unix: LIBS += -lqtmel
}
