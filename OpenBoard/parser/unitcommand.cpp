#include "unitcommand.h"



UnitCommand::UnitCommand() : Unit()
{
    Type = "";
}

UnitCommand::~UnitCommand()
{

}

void UnitCommand::setUnitCommandType(QString str)
{
    Type = str;
}

void UnitCommand::changeColor(QmlWidget *canvas)
{
    canvas->setFillColor(QColor(unit_data));
    qDebug() << "void UnitCommand::changeColor(QmlWidget *canvas)";
}

void UnitCommand::nextLine(QmlWidget *canvas)
{
    canvas->nextRow();
}

void UnitCommand::boardClean(QmlWidget *canvas)
{
    canvas->clearCanvas();
}

void UnitCommand::moveLeft(QmlWidget *canvas, int n){
    canvas->moveCursor(-n);
qDebug() <<"Moved left";

//INSERT CODE
}
void UnitCommand::moveRight(QmlWidget* canvas, int n){
qDebug() <<"Moved right " << n;
canvas->moveCursor(n);
//INSERT CODE
}

void UnitCommand::deletePreChar(QmlWidget *canvas)
{
    canvas->crossOutLastSymbol();
}
void UnitCommand::deleteLeft(QmlWidget *canvas, int n){
//INSERT CODE
    canvas->deleteFromBuffer(-n);
}
void UnitCommand::deleteRight(QmlWidget* canvas, int n){
//INSERT CODE
    canvas->deleteFromBuffer(n);
}


void UnitCommand::erasePreChar(QmlWidget *canvas)
{
    //canvas->crossOutLastSymbol();
    canvas->crossOutWithAnimation();
}
void UnitCommand::clearPreChar(QmlWidget *canvas)
{
    //canvas->crossOutLastSymbol();
   canvas->crossOutLastSymbol();
}

void UnitCommand::pause(QmlWidget *canvas)
{
    //QThread::msleep(unit_data.toULong()*100);
    canvas->update();
    canvas->pause(unit_data.toULong()*100);
}

void UnitCommand::backSlash(QmlWidget *canvas)
{
    canvas->insertToBuffer('\\');
    canvas->moveCursor();
    canvas->update();
}

void UnitCommand::draw(QmlWidget *canvas)
{
    if (Type == "ChangeColor")
    {
        changeColor(canvas);
        return;
    }
    if (Type == "NextLine")
    {
        nextLine(canvas);
        return;
    }
    if (Type == "BoardClean")
    {
        boardClean(canvas);
        return;
    }
    if (Type == "ClearPreChar")
    {
        clearPreChar(canvas);
    }
    if (Type == "MoveLeft")
    {
        moveLeft(canvas, unit_data.toInt());
    }
    if (Type == "MoveRight")
    {
         moveRight(canvas, unit_data.toInt());

    }
    if (Type == "DeletePreChar")
    {
        deletePreChar(canvas);
        return;
    }
    if (Type == "ErasePreChar")
    {
        erasePreChar(canvas);
        return;
    }
    if (Type == "Pause")
    {
        pause(canvas);
        return;
    }
    if (Type == "BackSlash")
    {
        backSlash(canvas);
        return;
    }
    if (Type=="DeleteLeft")
    {
        deleteLeft(canvas, unit_data.toInt());
        return;
    }
    if (Type=="DeleteRight")
    {
        deleteRight(canvas, unit_data.toInt());
        return;
    }
}

QString UnitCommand::getUnitCommandType()
{
    return Type;
}
