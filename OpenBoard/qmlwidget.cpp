#include "qmlwidget.h"
/*
 *scroll
 *
int k = canvas->property("scroll").toInt() - 10;
canvas->setProperty("scroll", k);
*/
QmlWidget::QmlWidget(QWidget *parent) :
    QQuickWidget(parent)
{
    //qRegisterMetaType<DrawData>("DrawData");
    engine()->rootContext()->setContextProperty(QLatin1String("forma"), this);
   // setSource(QUrl("draw.qml"));
    setSource(QUrl("qrc:/draw.qml"));

    m_encoder = new AV_REncoder(this);

    bRecord = false;
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint | Qt::WindowTitleHint);
    this->setResizeMode(QQuickWidget::SizeRootObjectToView );
    /*
     * init
    */
    lineHeight = 25;
    marginLeft = 20;
    marginTop = 40;
    maxWidth; //размер поле, где выводится текст
    pt = 36;
    font = "LC Chalk";
    isWord  =  true;
    x = marginLeft;
    y = lineHeight + pt;
    indexW = 1;
    indexRow = 0;
    scroll = 0;
    delay = 1000/3;
    //listStr[0] = 0;
    indexInList = 1;
    deleteWT = 0;
    fMetrics = new QFontMetrics(QFont(font));
    curStatus = STOP;
    tickTimer.setSingleShot(false);
    tickTimer.setInterval(1000/25);
    realDelay = 0;
    /*
     * canvas set
    */

    canvas = rootObject()->findChild<QObject *>("mcanvas");
    maxWidth = width() - marginLeft;
    QMetaObject::invokeMethod(canvas, "init");


   // connect(&tickTimer, SIGNAL(timeout()), this, SLOT(drawBuffer()));
    stringList.append("");
    //stringList.reserve(600000);
    indexRowInList = 0;
    cursorIndex = 0;

    /*
     *cursor ALPHA
     *
    */
    //setCursor(Qt::NoArrow);
    /*QPixmap pix(1,1);
    QColor color(0,0,0,0);
    pix.fill(color);
    QCursor mCursor(pix);
    setCursor(mCursor);*/
    /*QString fileName = QFileDialog::getSaveFileName();
    qDebug() << "qweqw";
    audioRecorder->setOutputLocation(QUrl::fromLocalFile(fileName));*/
   // audioRecorder->record();

}

QmlWidget::~QmlWidget()
{

}

void QmlWidget::moveEvent(QMoveEvent *event)
{

}

void QmlWidget::paintEvent(QPaintEvent *event)
{
    if(bRecord)
    {
       // m_encoder->mutex.lock();
        m_encoder->setFrame(grabFramebuffer());
        //m_encoder->mutex.unlock();
    }
 //   if(curStatus == PLAY && bRecord)
    {
    //    m_encoder->encodeVideoFrame(this->grabFramebuffer());
        /*if (fps_stabilitron % 25 == 0) {
            while (m_encoder->encodedFrameCount() != fps_stabilitron) { qApp->processEvents(); }
            m_encoder->encodeVideoFrame(this->grabFramebuffer());

        }
            fps_stabilitron++;*/
    }
}

void QmlWidget::resizeEvent(QResizeEvent *envent)
{
    QQuickWidget::resizeEvent(envent);
    maxWidth = width() - marginLeft;
}

void QmlWidget::closeEvent(QCloseEvent *event)
{
    stopAnimated();
    pause(500);
    if(canvas != NULL)
        delete canvas;
    if(fMetrics != NULL)
        delete fMetrics;
    isClose = true;
}

void QmlWidget::mousePressEvent(QMouseEvent *event)
{
    //crossOutLastSymbol();
}

QString QmlWidget::getDrawText()
{
    return drawText;
}

void QmlWidget::setDrawText(QString data)
{
    drawText = data;
}



void QmlWidget::drawAnimated(bool record)
{
    if(curStatus == this->PAUSE)
    {
        //m_recorder->resume();
        curStatus = PLAY;
        m_encoder->pause();
        return;
    }
    if(record)
    {
        QString fileName = QFileDialog::getSaveFileName(this, tr("Choose file..."), qApp->applicationDirPath(), tr("Videos (*.avi *.mp4)"));
        if(!fileName.size())
            return;
        m_encoder->setFileName(fileName);
        m_encoder->setGrabWidget(this);
        m_encoder->startRecord();
        qDebug() << "Start record into file";
    }
    curStatus = PLAY;
    bRecord = record;
    tickTimer.start();
    qDebug() << "Start play";
}

void QmlWidget::stopAnimated()
{
    curStatus = STOP;
    tickTimer.stop();
    m_encoder->stop();
    // max speed // stop draw function
    double t_animationSpeed = animationSpeed;
/*    int t_delay = delay;

    animationSpeed = 1;
    delay = 1;
    while(busy)
    {
         qApp->processEvents();
    }
    animationSpeed = t_animationSpeed;
    delay = t_delay;
*/
    bRecord = false;
 //   pause(200);
    qDebug() << "Stop play";

}

void QmlWidget::pauseAnimated()
{
    curStatus = PAUSE;
    qDebug() << "Pause play";
    m_encoder->pause();
    //m_recorder->pause();
}
bool QmlWidget::isRecord() const
{
    return bRecord;
}


void QmlWidget::clearCanvas()
{
    QMetaObject::invokeMethod(canvas, "clear");
    symbolPositionList.clear();
    listWords.clear();
    listStr.clear();
    indexRow = 0;
    indexW = 1;
    x = marginLeft;
    y = lineHeight + pt;
    scroll = 0;
    //listStr[0] = 0;
    indexInList = 1;
    indexRowInList = 0;


}
void QmlWidget::clearBuffer()
{
     colors.clear();
    cross.clear();
    cross.append(2); // для визова зачеркування якщо стрічка зацінчується
    stringList.clear();
    stringList.append("");
    cursorIndex = 0;
    deleteWT = 0;
    crossWithAnimation = false;
}

void QmlWidget::clearSymbol(int index){
QMetaObject::invokeMethod(canvas, "clearSymbol",  Q_ARG(QVariant, QVariant(index)));
}

void QmlWidget::drawFigure(int x, int y, int width, int height, QmlWidget::FigureType type, bool fill = true, QColor col = "#FF0000", float size = 2)
{
    QString sColor = QString("rgba(%1, %2, %3, %4)").arg(col.red()).arg(col.green()).arg(col.blue()).arg(col.alpha());
    QMetaObject::invokeMethod(canvas, "drawFigure",
            Q_ARG(QVariant, QVariant(x)),
            Q_ARG(QVariant, QVariant(y)),
            Q_ARG(QVariant, QVariant(width)),
            Q_ARG(QVariant, QVariant(height)),
            Q_ARG(QVariant, QVariant(type)),
            Q_ARG(QVariant, QVariant(fill)),
                              Q_ARG(QVariant, QVariant(size)),
                              Q_ARG(QVariant, QVariant(sColor)));
}

void QmlWidget::drawAnimationFigure(int x, int y, int width, int height, QmlWidget::FigureType type, bool fill)
{
    float persent;
    while(persent < 1)
    {
        drawFigure(x, y, x + (width - x)*persent, height, type, fill);
     //   qDebug() << delPos.x() << "             " << delPos.x() + (maxWidth - delPos.x())*persent;
        persent += animationSpeed;
        this->repaint();
        pause(10);
        if(curStatus == STOP )
            return;
    }
    drawFigure(x, y, width, height, type, fill);
}

void QmlWidget::crossOutLastSymbol( int n)
{
   // QPoint delPos = symbolPositionList.at(symbolPositionList.length() - 1 - deleteWT);
    //int k = 0;
    //QString str = listWords.right(1 + deleteWT);

    /*int widht = fMetrics->width(str)*1.5;
    int deltaY = deltaY = (widht - x) /(maxWidth - marginLeft);

    if(x - widht <= marginLeft)
    {
        deltaY++;
    }

    tx = maxWidth *deltaY - (widht - x + marginLeft);

    ty -= (lineHeight + pt)*deltaY;

    fillText("/", delPos.x(), delPos.y());
    fillText("\\", delPos.x(), delPos.y());*/
    //if()
    deleteWT += n;

}

void QmlWidget::crossOutWithAnimation(int n)
{
  //  qDebug() << "URAAAA!!!  " << deleteWT;
    crossOutLastSymbol(n);
    crossWithAnimation = true;
}

void QmlWidget::generateFrames()
{
}
int QmlWidget::getCursorIndex() const
{
    return cursorIndex;
}

void QmlWidget::setCursorIndex(int value)
{
    cursorIndex = value;
}


bool QmlWidget::getBusy() const
{
    return busy;
}

void QmlWidget::pause(int ms)
{
    if( ms < 1)
        return;
    QTimer tickTimer;
    tickTimer.setSingleShot(true);
    tickTimer.start(ms);
    while (tickTimer.isActive()) {
      qApp->processEvents(QEventLoop::EventLoopExec);
      if(curStatus == STOP)
        break;
    }
}

void QmlWidget::update()
{
    busy = true;
    crossText();
    drawBuffer();
}

QColor QmlWidget::getMainFillColor() const
{
    return mainFillColor;
}

void QmlWidget::setMainFillColor(const QColor &value)
{
    mainFillColor = value;
    setFillColor(value);
}

int QmlWidget::getDelay() const
{
    return delay;
}

void QmlWidget::setDelay(int value)
{
    delay = value;
    animationSpeed = (double)1/(value*0.6);
    qDebug() << animationSpeed;
}

int QmlWidget::getCountDeleteWT() const
{
    return deleteWT;
}

QFont QmlWidget::getTextFont() const
{
    return textFont;
}

void QmlWidget::setTextFont(const QFont &value)
{
    textFont = value;
    if(fMetrics != NULL)
        delete fMetrics;
    fMetrics = new QFontMetrics(value);
    pt = value.pointSize();
    QVariant a = QVariant(value.family());
    QVariant b = QVariant(value.pointSize());
    QString style = " ";
    if(value.italic())
        style.append("italic ");

    if(value.bold())
        style.append("bold ");

    QVariant c = QVariant(style); //style

    QMetaObject::invokeMethod(canvas, "initFont",
            Q_ARG(QVariant, b),
            Q_ARG(QVariant, a),
            Q_ARG(QVariant, c),
            Q_ARG(QVariant, value.underline()),
            Q_ARG(QVariant, value.strikeOut()));
}

void QmlWidget::setFillColor(QColor col)
{
    QString sColor = QString("rgba(%1, %2, %3, %4)").arg(col.red()).arg(col.green()).arg(col.blue()).arg(col.alpha());
    fillColor = col;
    QMetaObject::invokeMethod(canvas, "setColor",Q_ARG(QVariant, QVariant(sColor)));
}

void QmlWidget::setFillGradient(int x, int y, int width, int height, GradientSetting color)
{
    QMetaObject::invokeMethod(canvas, "setGradientStyle",
            Q_ARG(QVariant, QVariant(x)),
            Q_ARG(QVariant, QVariant(y)),
            Q_ARG(QVariant, QVariant(width)),
            Q_ARG(QVariant, QVariant(height)),
            Q_ARG(QVariant, QVariant(color.list)));
}

QmlWidget::StatusDraw QmlWidget::getStatus() const
{
    return curStatus;
}

void QmlWidget::recreate()
{
    this->create();
}
void QmlWidget::clear(int x,int y,int width,int height){
    QMetaObject::invokeMethod(canvas, "clearRectangle",
            Q_ARG(QVariant, QVariant(x)),
            Q_ARG(QVariant, QVariant(y)),
            Q_ARG(QVariant, QVariant(width)),
            Q_ARG(QVariant, QVariant(height))
                  );
}
void QmlWidget::fillText( QString str, int x, int y)
{
    QVariant arg1 = str;
    QVariant arg2 = x;
    QVariant arg3 = y;
   /* qDebug() << "ARG1:  " << str.size();
    qDebug() << "ARG2:  " << arg2;
    qDebug() << "ARG3:  " << arg3;
    if(arg1.isValid() && arg2.isValid() && arg3.isValid())
       if(canvas != NULL)
   */
    QMetaObject::invokeMethod(canvas, "fillText",
        Q_ARG(QVariant, arg1),
        Q_ARG(QVariant, arg2),
        Q_ARG(QVariant, arg3));


    if(textFont.strikeOut())
    {
        float x2 = x + fMetrics->width(str);
        float y2 = y - fMetrics->height()/4;
        drawFigure(x, y2 ,x2 , y2,LINE, false, fillColor);
    }

    if(textFont.underline())
    {
        float x2 = x + fMetrics->width(str) ;
        float y2 = y + fMetrics->height()*0.15;
        drawFigure(x ,y2 ,x2 , y2,LINE, false, fillColor);
    }
}

void QmlWidget::fillAnimationText(QString str, int x, int y, float time)
{
    /*QTimer timer;
    timer.setInterval(time);*/
    GradientSetting color;
    color.addColorStop(0,0,255,0);
    color.addColorStop(0.85f,0,255,0,255);
    int widthX = fMetrics->width(str) + x + 35;
    int widthT = fMetrics->width(str) + 35;
    int heightT = fMetrics->height() + y + 35;
    float nx = x - widthT;
    while(nx <= widthX)
    {
        setFillGradient(x - widthT, y, 250, heightT, color);
        fillText(str, x, y);
   //     this->update();
        qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
        nx += widthT/time;
       // Sleep(time);
    }
}



bool QmlWidget::crossTextV2()
{
    /*
    if(deleteWT != 0 && !symbolPositionList.isEmpty())
    {
        qDebug() << deleteWT << "WT == symbolPositionList" << symbolPositionList.size();
        if(deleteWT > symbolPositionList.length())
            deleteWT = symbolPositionList.length();

        QPoint delPos = symbolPositionList.at(symbolPositionList.length() - deleteWT); // -2 is popravka
        if(deleteWT != 1)
        {
            int i = 0;
            while(listWords.at(listWords.size() - i - 1) <= 0x20 && i != listWords.size() - 1)
            {
                i++;
            }
            int pos2 = symbolPositionList.length() - i - 1;

            int n = pos2;
            int m = pos2;
            int lastH = symbolPositionList[n].y();
            deleteWT--;
            while(deleteWT != 0 && n > 0)
            {

                while ( --n > 0 && lastH == symbolPositionList[n].y() && listWords[n] > 0x20 && deleteWT != 0)
                {
                    deleteWT--;
                }
                qDebug() << "startNEW with:  "<< hex << listWords[n + 1];
                delPos = symbolPositionList.at(m); // -2 is popravka
                int x2 = delPos.x() + fMetrics->width(listWords[m]);;
                int y2 = delPos.y() - fMetrics->height()/4;
                if( n == 0 )
                    delPos = symbolPositionList.at(n); // -2 is popravka
                else
                    delPos = symbolPositionList.at(n + 1); // -2 is popravka
                int x1 = delPos.x();
                int y1 = delPos.y() - fMetrics->height()/4;
                if(crossWithAnimation)
                {
                    qDebug() << "set animation speed";
                    drawAnimationFigure(x1, y1, x2, y2, LINE, 0);
                }
                else
                    drawFigure(x1, y1, x2, y2, LINE, 0);
                lastH = symbolPositionList[n].y();
                while ( n > 0 && listWords[n] <= 0x20)
                    n--;
                m = n;
                if(n != 0)
                    n++;
            }
        }
        else{
            int i = 1;
            while(symbolPositionList.length() != i && listWords.at(listWords.size() - i) == ' ')
                i++;
            delPos = symbolPositionList.at(symbolPositionList.length() - i);
            QColor temp = fillColor;
            setFillColor(QColor("#ff0000"));
            fillText("/", delPos.x(), delPos.y());
            fillText("\\", delPos.x(), delPos.y());
            setFillColor(temp);
        }
        deleteWT = 0;
    }
    else
        deleteWT = 0;
    deleteWT = 0;
    crossWithAnimation = false;*/
}

QPoint QmlWidget::drawWrapText(QString str)
{
    busy = true;
    //if(!crossTextV2())
     //   return QPoint(0, 0);
    crossTextV2();
    int width = fMetrics->width(str)*1.125 ;//+ fMetrics->leftBearing(str.at(0)) + fMetrics->rightBearing(str.at(0));

    if(str[0] <= 0x20){
         listChars.clear();
         widthToClean=0;
     }
        if(width + x > maxWidth)
        {
         qDebug()<<listChars;

                //fillText("-", x, y);
                //listChars += str;
                //listWords.remove(indexInList,1);
          int clearWidth = fMetrics->width(listChars); //

             // if(y  + scroll*indexRow > this->height() )
             clear(x-widthToClean,y-fMetrics->height(), this->width(),fMetrics->height()*1.25);

             // listStr.push_back( listChars.length() );//не юзається.
              // indexInList++;//не юзається.
          //indexInList+=listChars.length();
             nextRow();
             isLastRow();
            fillText(listChars,x,y);
           // listWords += listChars.trimmed();
           // listWords +=listChars;
            //listStr.push_back( listWords.length() );

            listChars.clear();
            widthToClean=0;
            x+=clearWidth;
        }
    isLastRow();
    //fillAnimationText(str, x, y, 6);
    fillText(str, x, y);
    QPoint res(x,y);
    listWords += str;
    listChars += str.trimmed();
    x += width;
    widthToClean+=width;
    symbolPositionList.push_back(res);
    tickTimer.start(delay);
    /*while (tickTimer.isActive()) {
       qApp->processEvents();
    }*/
    busy = false;
 //   qDebug() << "Y: " << y;
   // qDebug() << "Y: " << y;
    return res;
}

void QmlWidget::drawBuffer()
{
    busy = true;
    framDelayTimer.restart();
    //if(!crossTextV2())
     //   return QPoint(0, 0);
    //int width = fMetrics->width(str)*1.125 ;//+ fMetrics->leftBearing(str.at(0)) + fMetrics->rightBearing(str.at(0));
    //qDebug() << "DRAW";
    clearCanvas();
    int maxElm = (height()/(lineHeight + pt)) - 1;
    int CurRow = convertTextBoxToBufferIndex(cursorIndex).y();
    if(CurRow >= indexRowInList + maxElm)
    {
        indexRowInList += CurRow - (indexRowInList + maxElm) + 1;
    }
    if(CurRow < indexRowInList)
    {
        indexRowInList = CurRow;
    }
 //   qDebug() << "START draw with indexRowInList " << indexRowInList << "MAX elm " << maxElm << "CUR " << CurRow;
    int i = indexRowInList;
    while( i < stringList.length() && i < indexRowInList + maxElm)
    {
        //qDebug() << stringList[i] << "@";
        int localX=x;
        QStringList tabulationStr = stringList[i].split("\t");
        //TODO SET TEXT COLOR TO CANVAS COLOR
        setFillColor(getMainFillColor());
        for(int j = 0; j < tabulationStr.size(); j++)
        {
            fillText(tabulationStr[j], x, y);
            x += fMetrics->width(tabulationStr[j] + "\t");
        }
        qDebug() << "C:"<<colors.length();
    for (int k = 0 ; k< colors.length();k++)
    {
        int columnOfColorStrBegin;
        int columnOfColorStrEnd;

           int rowOfColorStrBegin =  convertTextBoxToBufferIndex(colors[k].startIndex).y();//Рядок в якому починається стрічка з кольором
           int rowOfColorStrEnd = 0;
           //Якщо не дійшли до останнього кольору, то встановл. рядок кінця стрічи з початку наступної кольорової
           if (k<colors.length()-1)rowOfColorStrEnd=convertTextBoxToBufferIndex(colors[k+1].startIndex ).y();
           //Якщо останній колір, то така стрічка закінчується в останньому рядку
           else rowOfColorStrEnd=stringList.length()-1;
           //Якщо ColorIndex в цій стрічці відсутній, то переходим на інший ColorIndex
             if (!(i>=rowOfColorStrBegin && i<=rowOfColorStrEnd))
                 continue;
           //Якщо на рядку, в якому починається кольорова стрічка — то стовпчик початку берем одразу з QList<ColorIndex> colors
           if (i==rowOfColorStrBegin)
               columnOfColorStrBegin =  convertTextBoxToBufferIndex(colors[k].startIndex ).x();
           //Інакше стовпчик початку - нульовий стовпчик . Інфа 100%
           else columnOfColorStrBegin = 0;
           //Якщо на останньому кольорі, то стовпчик кінця — індекс останнього символу стрічки.
             if (k==colors.length()-1) columnOfColorStrEnd =  stringList[i].length();
             else
                 //якщо не останній колір і на останній стрічці, то стовпчик кінця — стовпчик початку наступної стрічки з кольором
           if (i==stringList.length()-1)  columnOfColorStrEnd =  convertTextBoxToBufferIndex(colors[k+1].startIndex ).x();
            //Якщо не останній колір і не остання стрічка, то стовпчик кінця —
           else
           {
               //Якщо в цій самій стрічці починається інша кольорова стрічка то кінцевий стовпчик поточної
               //Кольорової стрічки - це почато наступної
               if (convertTextBoxToBufferIndex(colors[k+1].startIndex ).y()==i)
               columnOfColorStrEnd =  convertTextBoxToBufferIndex(colors[k+1].startIndex ).x();
               //Інакше КС(кінцевий стовпчик) — це кінець стрічки
               else columnOfColorStrEnd =  stringList[i].length();
           }
             QString textToWarp;
            //Якщо перша кольорова стрічка, то додаєм до локального Х ширину стрічки, що йшла до кольорової стрічки
            if (k==0)
           textToWarp= stringList[i].mid(0,columnOfColorStrBegin);
           else  {
                int  columnOfColorStrBeginPrev=0;
                if (convertTextBoxToBufferIndex(colors[k-1].startIndex ).y()==i)
                    columnOfColorStrBeginPrev=convertTextBoxToBufferIndex(colors[k-1].startIndex ).x();
                textToWarp= stringList[i].mid(columnOfColorStrBeginPrev,columnOfColorStrBegin-columnOfColorStrBeginPrev);
            }
            localX+=fMetrics->width(textToWarp);
            setFillColor(colors[k].value);
            QString textToFill = stringList[i].mid(columnOfColorStrBegin,columnOfColorStrEnd-columnOfColorStrBegin);
            fillText(textToFill,localX,y);
           // localX+=fMetrics->width(textToFill);
            //setFillColor(QColor(255,255,255));//Костиль, удалити, вистачить верхнього setColor, добавити на початок colors колір канви
             qDebug() << "columnOfColorStrEnd:" << columnOfColorStrEnd;
             qDebug() << "columnOfColorStrBegin:" << columnOfColorStrBegin;
            qDebug()<<"textToFill:"<<textToFill;
              qDebug()<< "textToWarp:" << textToWarp;
              qDebug()<<"rowOfColorStrBegin:"<<rowOfColorStrBegin;
    }
        /*
         * next row
         */
        y += lineHeight + pt;
        x = marginLeft;
        localX=marginLeft;
        i++;
    } 
    crossTextDraw();
    if((delay - framDelayTimer.elapsed()) > 0)
        realDelay = (delay - framDelayTimer.elapsed());
    else
        realDelay = 0;
    busy = false;
}

void QmlWidget::insertToBuffer(const QChar ch)
{
    QPoint convertedIndex = convertTextBoxToBufferIndex(cursorIndex);
    qDebug() << convertedIndex << " " << stringList.size() << " " << ch;
    QString &str =  stringList[convertedIndex.y()];
    if (convertedIndex.x()>=str.length())
        str.append(ch);
    else
    str.insert(convertedIndex.x(), ch);
    cross.insert(cursorIndex - convertedIndex.y(), 0);

    testWrap(convertedIndex.y());
    listChars.append(ch);

    emit drawTextChanged();
    pause(realDelay);

}

void QmlWidget::deleteFromBuffer(int n)
{
    update();
    int mustDell = qAbs(n);
    int crossCursor = cursorIndex - convertTextBoxToBufferIndex(cursorIndex).y();
    int i = n;
    while( i != 0)
    {
        cross.removeAt(crossCursor  + i);
        if(i > 0)
            i--;
        else
            i++;
        qDebug() << "QQQ" << crossCursor + i << "   " << " ::  " << cross;
    }
    while(mustDell > 0)
    {
        QPoint convertedIndex = convertTextBoxToBufferIndex(cursorIndex);
        cross.insert(cursorIndex - convertedIndex.y(), 0);
        qDebug() << convertedIndex << "DELL   " << mustDell;
        QString &str =  stringList[convertedIndex.y()];
        int realDell;
        if(n > 0)
        {
            if(mustDell > str.length() - convertedIndex.x())
                realDell = str.length() - convertedIndex.x();
            else
                realDell = mustDell;
            str.remove(convertedIndex.x(), realDell);
            mustDell -= realDell;
            if(mustDell > 0)
            {
                if(convertedIndex.y() + 1 < stringList.length())
                {
                  //  cursorIndex--;
                    mustDell--;
                    stringList[convertedIndex.y()].append(stringList[convertedIndex.y() + 1]);
                    stringList.removeAt(convertedIndex.y() + 1);
                }
                else
                    break;
            }
        }
        else
        {
            if(mustDell > convertedIndex.x())
                realDell = convertedIndex.x();
            else
                realDell = mustDell;
            str.remove(convertedIndex.x() - realDell, realDell);
            cursorIndex -= realDell;
            mustDell -= realDell;
            if(mustDell > 0)
            {
                if(convertedIndex.y() != 0)
                {
                    cursorIndex--;
                    mustDell--;
                    stringList[convertedIndex.y() - 1].append(str);
                    stringList.removeAt(convertedIndex.y());
                }
                else
                    break;
            }
        }

    }
    /*
    if (convertedIndex.x()>=str.length())
        str.append(ch);
    else

    testWrap(convertedIndex.y());
    listChars.append(ch);
*/
    emit drawTextChanged();
    pause(delay);
}

void QmlWidget::moveCursor(int n)
{
    update();
    cursorIndex += n;
    if(cursorIndex < 0)
        cursorIndex = 0;
 //   qDebug() << "Cursor move to n " << n <<"=== cur state " << cursorIndex << "QPOINT  " << convertTextBoxToBufferIndex(cursorIndex);

}

QPoint QmlWidget::convertTextBoxToBufferIndex(int index, bool symbol)
{
    int i = 0;
    int sumLength = 0;
    int numParagraph = 0;
    while( i < stringList.length())
    {
      //  qDebug() <<"stringList:"<<stringList.length();
        int lenNext;
        if(symbol)
            lenNext = stringList[i].length();
        else
            lenNext = stringList[i].length() + 1;
        sumLength += lenNext;
     //   qDebug() <<"sumLength:"<<sumLength;
//        qDebug() <<"index:"<<index;
//        qDebug() <<"I:"<<i;

        if(sumLength > index)
        {
            int colum = index - (sumLength - lenNext);
            int row = i;
           return QPoint( colum, row);
        }
        i++;
    }
    return QPoint(stringList[i - 1].length(), i - 1);
}

int QmlWidget::getRowFromTextBoxIndex(int index, bool symbol)
{
    int i = 0;
    int sumLength = 0;
    while( i < stringList.length())
    {
        int lenNext;
        if(symbol)
            lenNext = stringList[i].length();
        else
            lenNext = stringList[i].length() + 1;
        sumLength += lenNext;
        if(sumLength > index)
        {
           return i;
        }
        i++;
    }
}

void QmlWidget::testWrap(int kIndexOfRow)
{
    int i = kIndexOfRow;
    int length = stringList.length();
    while(stringList.length() > i)
    {
        QString &str =  stringList[i];
        //qDebug() <<"str:"<<stringList[i];
        int width = fMetrics->width(stringList[i]) ;//+ fMetrics->leftBearing(str.at(0)) + fMetrics->rightBearing(str.at(0));
        //qDebug() <<"str:"<< maxWidth;
        if(width > maxWidth)
        {
            int j = 0;
            while( j < stringList[i].length() && str[stringList[i].length() - j - 1] > 0x20){j++;};
                if(stringList.length() - 1 <= i)
                {};

            if(j >= stringList[i].length() - 1)
                j = stringList[i].length()/1.5f;

            nextRow(j, i);

        }
        i++;
    }
}

void QmlWidget::nextRow( int n, int Row)
{
    //isLastRow();
    QPoint convertedIndex;
    if(Row == -1)
    {
        if( n == -1)
            convertedIndex = convertTextBoxToBufferIndex(cursorIndex);
        else
            convertedIndex = convertTextBoxToBufferIndex(n);
    }
    else
        convertedIndex = QPoint(n, Row);

    int i = convertedIndex.y() + 1;
    QString lastStr = stringList[i - 1].right(stringList[i - 1].length() - convertedIndex.x());

    //qDebug() << "           LASTSTR    " << lastStr << "         " << convertedIndex.x();
    if(i >= stringList.length())
        stringList.append(lastStr);
    else
        stringList.insert(i, lastStr);
    stringList[i - 1].resize(convertedIndex.x());
    moveCursor();
    /* last work
    moveCursor(lastStr.length() + 1);
    */
    testWrap(i);
    emit drawTextChanged();

}

void QmlWidget::isLastRow()
{
    int maxElm = (height()/lineHeight + pt);

}

bool QmlWidget::crossTextDraw()
{
    int y;
    int x1, x2, x;
    bool lastGood = false;
    bool needNextRow = false;
    for(int i = 0; i < cross.length(); i++)
    {
     //   qDebug() << "CROSS\n" << cross;
        if(cross[i] != 0)
        {
            QPoint conv = convertTextBoxToBufferIndex(i, true);
        //    conv = convertTextBoxToBufferIndex(i + conv.y());
            if(!lastGood)
            {
                x1 = marginLeft + fMetrics->width(stringList[conv.y()].left(conv.x()));
                x = i;
                y = /*(lineHeight + pt)* */ conv.y();
                lastGood = true;
                continue;
            }
            if(conv.y() == y)
            {
                if(i != cross.length() - 1)
                    continue;
            }
            {
                needNextRow = true;
            }
        }
        if(lastGood)
        {
            QPoint conv = convertTextBoxToBufferIndex(i - 1, true);
        //    conv = convertTextBoxToBufferIndex(i + conv.y() + 1);
            ++y *= lineHeight + pt;
            y -= 0.25f * fMetrics->height();// first paid + midle

            x2 = marginLeft + fMetrics->width(stringList[conv.y()].left(conv.x() + 1));
            if( cross[i - 1] == -1)
            {
                qDebug() << "set animation speed";
                drawAnimationFigure(x1, y, x2, y, LINE, 0);

                for( int j = x; j < i; j++) // convert to cross without animation
                    cross[j] = 1;
            }
            else
                drawFigure(x1, y, x2, y, LINE, 0);
            lastGood = false;
            if(needNextRow)
                i--;
        }
    }
}

bool QmlWidget::crossText()
{
  //  qDebug() << "www " << deleteWT << cross;
    int spacePaid = 1;
    int row = convertTextBoxToBufferIndex(cursorIndex).y();
    while(deleteWT > 0)
    {
        //qDebug() << "DW " << deleteWT << convertTextBoxToBufferIndex(cursorIndex);
        int cursor = cursorIndex - row;
       /*   space paid
        *
        *  while( cursor - spacePaid >= 0)
        {
            QPoint convert = convertTextBoxToBufferIndex(cursor - spacePaid, true);
            //qDebug() << "\nSYMBOL                 :::" << cursor  - spacePaid <<"         " << stringList[convert.y()][convert.x()];
            if(stringList[convert.y()][convert.x()] <= 0x20)
                spacePaid++;
            else
                break;
        }*/
        cursor -= spacePaid;
        if(cursor >= 0 && cursor < cross.length())
        {
            if(crossWithAnimation)
                cross[cursor] = -1;
            else
                cross[cursor] = 1;
        }

        deleteWT--;
        spacePaid++;
    }
}
