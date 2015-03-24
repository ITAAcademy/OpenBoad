#include "mainwindow.h"
#include "ui_mainwindow.h"

#define TIMER_VALUE         300
#define GLWIDGET_SIZE       640,480
#define WINDOW_POS          80,100,760,560
#define WINDOW_MARGING          20
                   //CENTER 335,100,760,558

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mpGLWidget = new GLWidget;

    mTimer = new QTimer(this);
    mTimerClr = new QTimer(this);
    mTimer->setSingleShot(true);
    mTimerClr->setSingleShot(true);
    connect(mTimer, SIGNAL(timeout()), this, SLOT(show_pause_menu()));
    connect(mTimerClr, SIGNAL(timeout()), this, SLOT(show_color_dialog()));

    connect(ui->button_Find, SIGNAL(pressed()), this, SLOT(search()));

    connect(ui->textEdit, SIGNAL(textChanged()), this, SLOT(onTextChanged()));

    ui->widget_Find->setVisible(false);
    ui->widget_delayTB->setVisible(false);
  //  drawWidget->setVisible(false);
  //  on_action_Show_triggered();


    if(mSettings.FirstRun())
    {
        setGeometry(QRect(WINDOW_POS));
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

void MainWindow::closeEvent(QCloseEvent*)
{
    mpGLWidget->close();
    mpQmlWidget.close();
    delete mpGLWidget;
}

void MainWindow::on_action_Show_triggered()
{
/*
 * QML
*/
    mpQmlWidget.show();
    mpQmlWidget.setFixedSize(GLWIDGET_SIZE);
    mpQmlWidget.move(pos().x() + width() + WINDOW_MARGING, pos().y());
    ui->action_Pause->setEnabled(true);
    ui->action_Play->setEnabled(true);
    ui->action_Stop->setEnabled(true);

/*
    mpGLWidget->setFixedSize(GLWIDGET_SIZE);
    mpGLWidget->move(pos().x() + width(), pos().y());
    mpGLWidget->show();
*/


}

void MainWindow::on_action_Hide_triggered()
{
   // mpGLWidget->hide();
    if(mpQmlWidget.IsPlay())
        mpQmlWidget.stopAnimated();
    mpQmlWidget.hide();
    ui->action_Pause->setEnabled(false);
    ui->action_Play->setEnabled(false);
    ui->action_Stop->setEnabled(false);

}

void MainWindow::moveEvent(QMoveEvent *event)
{
    mpQmlWidget.move(pos().x() + width() + WINDOW_MARGING, pos().y());
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    mpQmlWidget.move(pos().x() + width() + WINDOW_MARGING, pos().y());
}

void MainWindow::on_action_Font_triggered()
{   //call QtFontDialog & setFont to all elements (this)
    bool ok;
    QFont font;
    font = QFontDialog::getFont(&ok, QFont("Times",10,1,false), this);
    if (!ok)
        return;
    ui->menuBar->setFont(font);
    ui->textEdit->setFont(font);
    setFont(font);
}

void MainWindow::on_action_Board_Font_triggered()
{
    bool ok;
    QFont font;
    font = QFontDialog::getFont(&ok, QFont("Times",10,1,false), this);
    if (!ok)
        return;

    QString qmlFont;
    QFontInfo fontInf(font);
    qmlFont = QString::number(fontInf.pointSize()) + "pt \"" + font.family() + "\"";
    mpQmlWidget.setFont(qmlFont);
}

void MainWindow::on_action_Reset_default_triggered()
{   //set font to menuBar: Times, 8 pt, normal
    QFont font = (QFont("Times",10,1,false));
    ui->menuBar->setFont(font);
    ui->textEdit->setFont(font);
    ui->textEdit->setTextColor("#000000");
    QString temp = ui->textEdit->toPlainText();
    ui->textEdit->clear();
    ui->textEdit->insertPlainText(temp);
    setFont(font);
}

void MainWindow::on_action_Color_triggered()
{
    QColor colorm;
    colorm = QColorDialog::getColor();
    QString col = colorm.name();
    ui->textEdit->setTextColor(col);
    QString temp = ui->textEdit->toPlainText();
    ui->textEdit->clear();
    ui->textEdit->insertPlainText(temp);
}

void MainWindow::on_action_Board_Color_triggered()
{
    QColor colorm;
    colorm = QColorDialog::getColor();
    QString col = colorm.name();
    mpQmlWidget.setColor(col);
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
//    a temporary solution
//    ui->widget_delayTB->setVisible(!ui->widget_delayTB->isVisible());
//    a temporary solution

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
                QMessageBox::warning(this, "Error",tr("Ошыбка открытия файла")); //щ
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

void MainWindow::on_delayBtn_pressed()
{
    mTimer->start(TIMER_VALUE);
}

void MainWindow::on_delayBtn_released()
{
    if(mTimer->isActive()) {
        mTimer->stop();

        QString text = ui->action_delayTB->text();
        text += QString::number(ui->spinBox_delayTB->value() / 10);
        text += QString::number(ui->spinBox_delayTB->value() % 10);

        ui->textEdit->insertPlainText(text);
    }
}

void MainWindow::show_pause_menu()
{
    ui->widget_delayTB->setVisible(!ui->widget_delayTB->isVisible());
}

void MainWindow::on_backBtn_clicked()
{
    QString text = ui->action_backTB->text();
    ui->textEdit->insertPlainText(text);
}

void MainWindow::on_animationBtn_clicked()
{
    QString text = ui->action_animatedTB->text();
    ui->textEdit->insertPlainText(text);
}

void MainWindow::on_crossBtn_clicked()
{
    QString text = ui->action_crossTB->text();
    ui->textEdit->insertPlainText(text);
}

void MainWindow::on_colorBtn_pressed()
{
    mTimerClr->start(TIMER_VALUE);
}

void MainWindow::on_colorBtn_released()
{
    if(mTimerClr->isActive()) {
        mTimerClr->stop();

        QString text = ui->action_colorTB->text();
        textColorName = colorPkr.name();
        text += textColorName;
        text.remove(2,1);
        ui->textEdit->insertPlainText(text);
    }
}

void MainWindow::show_color_dialog()
{
    colorPkr = QColorDialog::getColor(Qt::black, this);

    if(colorPkr.isValid())
        textColorName = colorPkr.name();
}

void MainWindow::on_colorBtn_clicked()
{

//    QString text = ui->action_colorTB->text();
//    text += textColorName;
//    text.remove(2,1);
//    ui->textEdit->insertPlainText(text);
}

void MainWindow::on_clearBtn_clicked()
{
    QString text = ui->action_clearTB->text();
    ui->textEdit->insertPlainText(text);
}
void MainWindow::onTextChanged()
{
   // DrawData temp = mpQmlWidget.getDrawData();
    /*if(!ui->textEdit->toPlainText().isEmpty())
        this->inputText += ui->textEdit->toPlainText().at(ui->textEdit->toPlainText().size() - 1);
    mpQmlWidget.setDrawText(inputText);*/
    mpQmlWidget.setDrawText(ui->textEdit->toPlainText());
    emit mpQmlWidget.drawTextChanged();
    //mpQmlWidget.Encode(mpQmlWidget.grabFramebuffer());
    /*mpGLWidget->textArray.clear();
    mpGLWidget->textArray.append(str);
    mpGLWidget->updateGL();
   if (mpGLWidget->mIsAnimatedStart){
      mpGLWidget->pauseAnimated();
   }*/
}
void MainWindow::on_action_Play_triggered()
{
    if(mpQmlWidget.IsPlay())
        ui->action_Play->setText("Play");
    mpQmlWidget.drawAnimated();
   /* mpGLWidget->textArray.clear();
    mpGLWidget->textArray.append(str);
    mpGLWidget->setFixedSize(GLWIDGET_SIZE);
    mpGLWidget->move(pos().x() + width(), pos().y());
    mpGLWidget->show();
    mpGLWidget->drawAnimated();
    */
}
void MainWindow::on_action_Stop_triggered()
{
    mpQmlWidget.stopAnimated();
    ui->action_Play->setText("Play");
   // mpGLWidget->stopAnimated();
}
void MainWindow::on_action_Pause_triggered()
{
    ui->action_Play->setText("Resume");
    mpQmlWidget.pauseAnimated();
    //mpGLWidget->pauseAnimated();
}




