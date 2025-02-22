#include "unitsumbol.h"

UnitSumbol::UnitSumbol() : Unit()
{
    Size = 0;
    Color = "";
    Font = "";
    unitType = 0;
}

UnitSumbol::~UnitSumbol()
{

}

void UnitSumbol::draw(OGLWidget *canvas)
{
   // canvas->update();

    canvas->insertToBuffer(unit_data[0]);
    canvas->moveCursor();

}

void UnitSumbol::setUnitSumbolColor(QString color)
{
    Color = color;
}

void UnitSumbol::setUnitSumbolSize(int size)
{
    Size = size;
}

void UnitSumbol::setUnitSumbolType(QString str)
{
    Font = str;
}

QString UnitSumbol::getUnitSumbolColor()
{
    return Color;
}

QString UnitSumbol::getUnitSumbolType()
{
    return Font;
}

int UnitSumbol::getUnitSumbolSize()
{
    return Size;
}
