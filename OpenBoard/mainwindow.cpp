#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mpGLWidget = new GLWidget;

    //fixed toolBar
    ui->mainToolBar->setMovable(false);
    //enable to hide toolBar (rightMouseClick)
    ui->mainToolBar->setContextMenuPolicy(Qt::CustomContextMenu);

    sliderTB = new QSlider(Qt::Horizontal, this);
    spinBoxTB = new QSpinBox(this);
    connect(spinBoxTB, SIGNAL(valueChanged(int)), sliderTB, SLOT(setValue(int)));
    connect(sliderTB, SIGNAL(valueChanged(int)), spinBoxTB, SLOT(setValue(int)));
    //sliderTB->setRange(0,400);
    //sliderTB->setTickInterval(1);
    ui->mainToolBar->addWidget(sliderTB);
    ui->mainToolBar->addWidget(spinBoxTB);

    if(mSettings.FirstRun())
    {


        setGeometry(QRect(335,100,760,558));
        setFont(QFont("Times",10,1,false));


        mSettings.setMainWindowRect(geometry());
        mSettings.setMainWindowTitle(windowTitle());
        mSettings.setMainWindowFont(font());

    }
    else
    {
        setWindowTitle(mSettings.getMainWindowTitle());
        setGeometry(mSettings.getMainWindowRect());
        setFont(mSettings.getMainWindowFont());


    }
}

MainWindow::~MainWindow()
{
    mSettings.setMainWindowRect(geometry());
    mSettings.setMainWindowFont(font());
    delete ui;
}
void MainWindow::on_menu_Board_triggered()
{

}
void MainWindow::on_action_Show_triggered()
{
    mpGLWidget->setFixedSize(450,450);

    mpGLWidget->move(pos().x() + width(), pos().y());

    mpGLWidget->show();
}
void MainWindow::on_action_Hide_triggered()
{
    mpGLWidget->hide();
}
void MainWindow::on_action_Font_triggered()
{   //call QtFontDialog & setFont to all elements (this)
    bool ok;
    QFont font;
    font = QFontDialog::getFont(&ok, QFont( "Times", 10 ), this);
    if (!ok)
        return;
    setFont(font);
}

void MainWindow::on_action_Reset_default_triggered()
{   //set font to menuBar: Times, 8 pt, normal
    ui->menuBar->setFont(QFont("Times",8,1,false));
}

void MainWindow::on_action_Color_triggered()
{   //call QtColorDialog
    QColor colorm;
    colorm = QColorDialog::getColor();
}

bool MainWindow::saveFile()
{

    QFile file(curFile);
    if(file.open(QFile::WriteOnly))
    {
        file.write(ui->textEdit->toPlainText().toUtf8());
        return true;
    }
    else
    {
        QMessageBox::warning(this, "Error",
                             tr("Ощибка записи файла")
                             .arg(curFile)
                             .arg(file.errorString()));
        return false;
    }
}

bool MainWindow::maybeSave()
{
    if(ui->textEdit->document()->isModified())
    {
        QMessageBox::StandardButton ret =
                QMessageBox::warning(
                    this,
                    "File",
                    tr("Документ был изменен, сохранить изменения?"),
                    QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if (ret == QMessageBox::Yes)
        {
          return on_action_Save_triggered();
        }
        else  if(ret == QMessageBox::Cancel)
           return false;

    }
    return true;
}

bool MainWindow::on_action_Save_as_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "D:/",
                                                    tr("Text Files (*.txt);;All Files (*.*)"));

    if(!fileName.isEmpty())
    {
      curFile = fileName;
      return saveFile();
    }
    return false;
}

bool MainWindow::on_action_Save_triggered()
{
    if (curFile.isEmpty())
        return on_action_Save_as_triggered();
    else
        return saveFile();
}

void MainWindow::on_action_Open_triggered()
{
    if (maybeSave())
    {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "D:/",
                                                              tr("Text Files (*.txt);;All Files (*.*)"));
        if(!fileName.isEmpty())
        {
            QFile file(fileName);
            if(file.open(QFile::ReadOnly))
            {
                ui->textEdit->setPlainText(file.readAll());
            }
            else
            {
                QMessageBox::warning(this, "Error",tr("Ощибка открытия файла"));
            }
        }
    }
}


void MainWindow::on_action_New_triggered()
{
    if (maybeSave())
    {
        ui->textEdit->clear();
    }

}
