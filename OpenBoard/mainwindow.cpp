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
    //fixed toolBar
    ui->mainToolBar->setMovable(false);
    //enable to hide toolBar (rightMouseClick)
    ui->mainToolBar->setContextMenuPolicy(Qt::CustomContextMenu);

    mpGLWidget = new GLWidget;

    sliderTB = new QSlider(Qt::Horizontal, this);
    spinBoxTB = new QSpinBox(this);


    connect(sliderTB, SIGNAL(valueChanged(int)), spinBoxTB, SLOT(setValue(int)));
    connect(spinBoxTB, SIGNAL(valueChanged(int)), sliderTB, SLOT(setValue(int)));


    //sliderTB->setRange(0,400);
    //sliderTB->setTickInterval(1);

    ui->mainToolBar->addWidget(sliderTB);
    ui->mainToolBar->addWidget(spinBoxTB);
    ui->widget_Find->setVisible(false);

    ui->widget_Find->setVisible(false);
    connect(ui->button_Find, SIGNAL(pressed()), this, SLOT(search()));


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

void MainWindow::on_action_Undo_triggered()
{
    ui->textEdit->undo();
}

void MainWindow::on_action_Redo_triggered()
{
    ui->textEdit->redo();
}

void MainWindow::on_action_Cut_triggered()
{
    ui->textEdit->cut();
}

void MainWindow::on_action_Copy_triggered()
{
    ui->textEdit->copy();
}

void MainWindow::on_action_Paste_triggered()
{
    ui->textEdit->paste();
}


void MainWindow::on_action_Select_all_triggered()
{
    if(ui->textEdit->hasFocus()) {
        ui->textEdit->selectAll();
    }
    if(ui->lineEdit_Find->hasFocus()) {
        ui->lineEdit_Find->selectAll();
    }
}

void MainWindow::on_action_Find_triggered()
{
    ui->widget_Find->setVisible(!ui->widget_Find->isVisible());
    if(ui->widget_Find->isVisible()) {
        ui->lineEdit_Find->setFocus();
        ui->lineEdit_Find->selectAll();
    }
    else {
        ui->textEdit->setFocus();
    }
}

void MainWindow::on_action_Exit_triggered()
{
    if(maybeSave())
    {
        this->close();
    }
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
                             tr("Ощибка записи файла") //щ
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

void MainWindow::search()
{
    QString str = ui->lineEdit_Find->text();

    QList<QTextEdit::ExtraSelection> extraSelections;

    ui->textEdit->moveCursor(QTextCursor::Start);
    QColor color = QColor(Qt::yellow).lighter(130);

    if(ui->check_case_sensitive->isChecked() == false
            && ui->check_whole_words->isChecked() == false)
    {
        while(ui->textEdit->find(str))
        {
            QTextEdit::ExtraSelection extra;
            extra.format.setBackground(color);

            extra.cursor = ui->textEdit->textCursor();
            extraSelections.append(extra);
        }
    }

    else if(ui->check_case_sensitive->isChecked() == true
            && ui->check_whole_words->isChecked() == false)
    {
        while(ui->textEdit->find(str, QTextDocument::FindCaseSensitively))
        {
            QTextEdit::ExtraSelection extra;
            extra.format.setBackground(color);

            extra.cursor = ui->textEdit->textCursor();
            extraSelections.append(extra);
        }
    }

    else if(ui->check_case_sensitive->isChecked() == false
            && ui->check_whole_words->isChecked() == true)
    {
        while(ui->textEdit->find(str, QTextDocument::FindWholeWords))
        {
            QTextEdit::ExtraSelection extra;
            extra.format.setBackground(color);

            extra.cursor = ui->textEdit->textCursor();
            extraSelections.append(extra);
        }
    }

    else if (ui->check_case_sensitive->isChecked() == true
            && ui->check_whole_words->isChecked() == true)
    {
        while(ui->textEdit->find(str, QTextDocument::FindWholeWords | QTextDocument::FindCaseSensitively))
        {
            QTextEdit::ExtraSelection extra;
            extra.format.setBackground(color);

            extra.cursor = ui->textEdit->textCursor();
            extraSelections.append(extra);
        }
    }

    ui->textEdit->setExtraSelections(extraSelections);
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
                QMessageBox::warning(this, "Error",tr("Ощибка открытия файла")); //щ
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


void MainWindow::on_action_clearTB_triggered()
{
    QString text = ui->action_clearTB->text();
    if(ui->textEdit->hasFocus()) {
        ui->textEdit->insertPlainText(text);
    }
}

void MainWindow::on_action_colorTB_triggered()
{
    QString text = ui->action_colorTB->text();
    if(ui->textEdit->hasFocus()) {
        ui->textEdit->insertPlainText(text);
    }
}

void MainWindow::on_action_backTB_triggered()
{
    QString text = ui->action_backTB->text();
    if(ui->textEdit->hasFocus()) {
        ui->textEdit->insertPlainText(text);
    }
}

void MainWindow::on_action_crossTB_triggered()
{
    QString text = ui->action_crossTB->text();
    if(ui->textEdit->hasFocus()) {
        ui->textEdit->insertPlainText(text);
    }
}

void MainWindow::on_action_animatedTB_triggered()
{
    QString text = ui->action_animatedTB->text();
    if(ui->textEdit->hasFocus()) {
        ui->textEdit->insertPlainText(text);
    }
}

void MainWindow::on_action_delayTB_triggered()
{
    QString text = ui->action_delayTB->text();
    if(ui->textEdit->hasFocus()) {
        ui->textEdit->insertPlainText(text);
    }
}

