#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFont>
#include <QFontDialog>
#include <QColor>
#include <QColorDialog>
#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include <QSettings>

#include "glwidget.h"
#include "settings.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

//    board
    void on_action_Show_triggered();
    void on_action_Hide_triggered();

//    settings
    void on_action_Font_triggered();
    void on_action_Color_triggered();
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
    void on_action_clearTB_triggered();
    void on_action_colorTB_triggered();
    void on_action_backTB_triggered();
    void on_action_crossTB_triggered();
    void on_action_animatedTB_triggered();
    void on_action_delayTB_triggered();

private:

    Ui::MainWindow  *ui;

    GLWidget        *mpGLWidget;

    QString         curFile;
    QSlider         *sliderTB;
    QSpinBox        *spinBoxTB;
    pSettings       mSettings;

    QTimer          *timer;

    bool saveFile();
    bool maybeSave();
};

#endif // MAINWINDOW_H
