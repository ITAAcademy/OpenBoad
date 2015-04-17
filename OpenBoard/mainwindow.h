﻿#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFont>
#include <QColorDialog>
#include <QFontDialog>
#include <QFileDialog>
#include <QColor>
#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include <QSettings>
#include <QMessageBox>
#include <QElapsedTimer>
#include <QMenu>
#include <QAction>

#include <QDebug>

#include "qmlwidget.h"
#include "settings.h"
#include "parser/parser.h"
#include "editWidget/mytextedit.h"
#include "youtube/youtubewrapper.h"

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:

//    board
    void closeEvent(QCloseEvent* event);
    void moveEvent(QMoveEvent *event);
    void resizeEvent(QResizeEvent *event);
    void on_action_Show_triggered();
    void on_action_Hide_triggered();
    void on_action_Play_triggered();
    void on_action_Pause_triggered();
    void on_action_Stop_triggered();
    void on_action_youTube_triggered();

    void onTextChanged();

//    settings
    void on_action_Font_triggered();
    void on_action_Color_triggered();
    void on_action_Board_Font_triggered();
    void on_action_Board_Color_triggered();
    void on_action_Reset_default_triggered();

//    edit
    void on_action_Undo_triggered();
    void on_action_Redo_triggered();
    void on_action_Cut_triggered();
    void on_action_Copy_triggered();
    void on_action_Paste_triggered();
    void on_action_Select_all_triggered();
    void on_action_Find_triggered();
        void search();

//    file
    void on_action_Exit_triggered();
    bool on_action_Save_as_triggered();
    bool on_action_Save_triggered();
    void on_action_Open_triggered();
    void on_action_New_triggered();

//    toolbar
    void on_backBtn_clicked();
    void on_animationBtn_clicked();
    void on_crossBtn_clicked();
    void on_colorBtn_clicked();
    void on_clearBtn_clicked();

//    longpress on button delay
    void on_delayBtn_pressed();
    void on_delayBtn_released();
    void show_pause_menu();

//    longpress on button color
    void on_colorBtn_pressed();
    void on_colorBtn_released();
    void show_color_dialog();



private:

    Ui::MainWindow  *ui;

 //   GLWidget        *mpGLWidget;
    QmlWidget       *mpQmlWidget;
    MyTextEdit       *textEdit;
    QString         curFile;
    pSettings       mSettings;
    QTimer          *mTimer;
    QTimer          *mTimerClr;
    QString         textColorName;
    QColor          colorPkr;
    QString         inputText;
    QList <Unit*>mUnitList;
    Parser          mParser;
    QThread drawThread;
    bool saveFile();
    bool maybeSave();
    YouTubeWrapper *youtube;


};

#endif // MAINWINDOW_H
