#include "listcontroll.h"



QPoint ListControll::getSelectedBlockPoint() const
{

    if (tracks.size() > 0 && selectedBlockPoint.x() >= 0 && tracks.length() > selectedBlockPoint.x()  && tracks[selectedBlockPoint.x()].block.length() > selectedBlockPoint.y())
        return selectedBlockPoint;
    return QPoint(-1,-1); // ALL BAD
}

void ListControll::setSelectedBlockPoint(const QPoint &value)
{
    if(value != selectedBlockPoint)
    {
        selectedBlockPoint = value;
        emit updateSelectedBlock(value);
    }
       // qDebug() << selectedBlockPoint;
     qDebug() <<"FFFFFFFFFFFF: col = " << selectedBlockPoint.x() << " ind = " << selectedBlockPoint.y();
}

void ListControll::setSelectedBlockPoint(int col, int ind)
{
    setSelectedBlockPoint(QPoint(col,ind));
    // qDebug() <<"FFFFFFFFFFFF: col = " << selectedBlockPoint.x() << " ind = " << selectedBlockPoint.y();
}



float ListControll::getScaleScrollChildren() const
{
    return scale_scroll_children;
}

void ListControll::setScaleScrollChildren(const float &value)
{
    scale_scroll_children = value;
    if (scale_scroll_children <zoom_speed)
        scale_scroll_children = zoom_speed;
}

void ListControll::changeScaleScrollChildren(const float &value)
{
    scale_scroll_children += value;
    if (scale_scroll_children <zoom_speed)
        scale_scroll_children = zoom_speed;
}



float ListControll::getZoomSpeed() const
{
    return zoom_speed;
}

void ListControll::setZoomSpeed(float value)
{
    zoom_speed = value;
}
void ListControll::recountMaxTrackTime()
{
    maxTrackTime = 0;
    for (int i=0; i< tracks.size(); i++)
        if (maxTrackTime < tracks[i].time)
            maxTrackTime = tracks[i].time;
}

QString ListControll::getBlockKey(int col, int i) const
{
    return tracks[col].block[i].key;
}

void ListControll::setBlockKey(int col, int i, QString name)
{
    tracks[col].block[i].key = name;
   // test[col][i] = name;
}

void ListControll::removeBlock(int col, int i)
{
    isBlocked = true;
    setSelectedBlockPoint(QPoint(-1,-1));
    if (tracks[col].block.size() > i)
    {
   // tracks[col].block.removeAt(i);

  // if (maxTrackTime == tracks[col].time)
   {
       int temp = tracks[col].block[i].draw_element->getLifeTime();
             //  testWidth[col][i];
       tracks[col].block[i].clear();
       tracks[col].block.removeAt(i);
        tracks[col].time -= temp;
   // testColumnWidth[col] -= temp;
    recountMaxTrackTime();

   }
   /*else
   {
       int temp = tracks[col].block[i].draw_element->getLifeTime();
       tracks[col].block[i].clear();
       tracks[col].block.removeAt(i);
        tracks[col].time -= temp;
   }*/
  // if (selectedBlockPoint == QPoint(col,i))       selectedBlock = NULL;
    }
    calcPointedBlocks();
    isBlocked = false;
}

void ListControll::addNewBlock(int col, QString str, DrawElement *element)
{
  /*  QString open = QFileDialog::getOpenFileName();
    DrawElement *elm = GenerationDrawElement(open);
    if(elm == NULL)
        return;
    // qDebug() << "9999999999999999999999999999999999999999999999" << elm->getType();*/
    setSelectedBlockPoint(QPoint(-1,-1));
    Element temp;
    temp.key = str;
    if(element != NULL)
    {
        temp.draw_element = element;
    }
    //temp.draw_element->setLifeTime(def_min_block_width);
    if(temp.draw_element->getLifeTime() < def_min_block_width)
        temp.draw_element->setLifeTime(def_min_block_width);
    temp.draw_element->setZ(col);
    tracks[col].block.append(temp);
    tracks[col].time += temp.draw_element->getLifeTime();
   // testWidth[col].append(200);
    //testColumnWidth[col]+=200;
  //  // qDebug() << "SIZE   " << test.size();
    if (maxTrackTime <  tracks[col].time)
        maxTrackTime =  tracks[col].time;
    calcPointedBlocks();
}


void ListControll::addNewTrack( )
{
    QList <Element>  temp;
   // temp.append(Element());
 //   temp.append(Element());
  //  temp.append(Element());
       int  temp_traclwidth =   def_min_block_width*temp.size();
       Track trak(temp_traclwidth,temp);
        tracks.append(trak);

   if (maxTrackTime < temp_traclwidth)
       maxTrackTime = temp_traclwidth; //1234
   calcPointedBlocks();
}

void ListControll::loadFromFile()
{

    QPoint p = getSelectedBlockPoint();
  /*  if (p.x() > -1)
       tracks[p.x()].time -= tracks[p.x()].block[p.y()].draw_element->getLifeTime();*/
   QString open = QFileDialog::getOpenFileName();
//qDebug() <<"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA " + open;
   DrawElement *elm = GenerationDrawElement(open);
   if(elm == NULL)
   {
       qDebug() << "loadFromFile(): elm == NULL";
       return;
   }
   // qDebug() << "9999999999999999999999999999999999999999999999" << elm->getType();

   Element &temp = tracks[p.x()].block[p.y()];
   int life_time = temp.draw_element->getLifeTime();
   //int new_life_time = elm->getLifeTime();
   int start_time = temp.draw_element->getStartDrawTime();
   delete temp.draw_element;
   temp.key = elm->getKey();
   temp.draw_element = elm;
   temp.draw_element->setLifeTime(life_time);
   temp.draw_element->setStartDraw(start_time);
   temp.draw_element->setZ(p.x());
   emit loadFromFileSignal();
   emit updateSelectedBlock(selectedBlockPoint);
   emit loadFromFileSignal();
/*
   if(new_life_time > 1000)
   {
       setBlockTime(p.x(),p.y(), new_life_time);
   }
*/
}

bool ListControll::removeLastBlock(int col)
{
    isBlocked = true;
    setSelectedBlockPoint(QPoint(-1,-1));
    if (tracks[col].block.size())
    {
  // tracks[col].block.pop_back();

   if (maxTrackTime == tracks[col].time)
   {
       int temp = tracks[col].block.last().draw_element->getLifeTime();
       tracks[col].block.last().clear();
      tracks[col].block.pop_back();
    tracks[col].time -= temp;
    recountMaxTrackTime();

   }
   else
   {
       int temp = tracks[col].block.last().draw_element->getLifeTime();
       tracks[col].block.last().clear();
      tracks[col].block.pop_back();
    tracks[col].time -= temp;
   }
  // if (selectedBlockPoint.x() == col)       selectedBlock = NULL;
   pointed_block.clear();
   isBlocked = false;
    return true;
    }
    isBlocked = false;

    return false;
}

bool ListControll::removeLastTrack()
{
    isBlocked = true;
    if (tracks.size())
    {
    int lastColTime = tracks.last().time;

    int last_ind = tracks.size()-1;
//tracks[last_ind].deleteClearBlocks();
 tracks.pop_back();
    if(last_ind == 0)
    {
        maxTrackTime = 0;
        pointed_block.clear();
        selectedBlockPoint=QPoint(-1,-1);
    }
   else
    {
   // if (maxTrackTime == lastColTime)
        //recountMaxTrackTime();
    this->calcPointedBlocks();
    }
recountMaxTrackTime();


  // if (selectedBlockPoint.x() == tracks.size() - 1)        selectedBlock = NULL;
    isBlocked = false;
    return true;
    }
    isBlocked = false;
    return false;
}

bool ListControll::removeTrack(int col)
{
    isBlocked = true;
    if (tracks.size()>col)
    {
    int lastColTime = tracks[col].time;
    tracks.removeAt(col);
    if (maxTrackTime == lastColTime)
        recountMaxTrackTime();

  //  if (selectedBlockPoint.x() == tracks.size() - 1)     selectedBlock = NULL;
    calcPointedBlocks();
    isBlocked = false;
    return true;
    }
    isBlocked = false;
    return false;
}

 void ListControll::reverseBlocks(int col, int init_pos, int end_pos)
 {
   Element temp = tracks[col].block[init_pos];
   tracks[col].block[init_pos] = tracks[col].block[end_pos];
  //   testWidth[col][init_pos] = testWidth[col][end_pos];
     tracks[col].block[end_pos] = temp;
 }

void ListControll::setBlocks(int col,const QList <Element> &value)
{
    tracks[col].block = value;
}

void ListControll::setBlockTime(int col, int i,int value)
{
  //   = value;
    if(def_min_block_width > value)
        value = def_min_block_width;

        tracks[col].time += value - tracks[col].block[i].draw_element->getLifeTime();  ;
      tracks[col].block[i].draw_element->setLifeTime(value);    
    recountMaxTrackTime();
    // qDebug() << "DDDDD  tracks[col].block[i].draw_element->getLifeTime()=" <<   tracks[col].block[i].draw_element->getLifeTime();
}

void ListControll::setBlockStartTime(int col, int i,int value)
{
    tracks[col].block[i].draw_element->setStartDraw(value);
}

int ListControll::getBlockStartTime(int col, int i)
{
    return tracks[col].block[i].draw_element->getStartDrawTime();
}

void ListControll::setBlockDrawElemet(DrawElement *elm, int col, int i)
{
    tracks[col].block[i].draw_element = elm;
}

void ListControll::setBlockIcon(int col, int i, QImage icon)
{
     tracks[col].block[i].draw_element->setIcon(icon);
}

QImage ListControll::getBlockIcon(int col, int i)
{
    return   tracks[col].block[i].draw_element->getIcon();
}



 void ListControll::setDrawX(int col, int i, int value)
 {
     tracks[col].block[i].draw_element->setX(value);

 }

 int ListControll::getDrawX(int col, int i)
 {
     return tracks[col].block[i].draw_element->getX() ;
 }

 void ListControll::setDrawY(int col, int i, int value)
 {
    tracks[col].block[i].draw_element->setY (value);
 }

 int ListControll::getDrawY(int col, int i)
 {
     return tracks[col].block[i].draw_element->getY();
 }

 void ListControll::setDrawZ(int col, int i, int value)
 {
     tracks[col].block[i].draw_element->setZ(value);
 }

 int ListControll::getDrawZ(int col, int i)
 {
     return tracks[col].block[i].draw_element->getZ() ;
 }

 void ListControll::setDrawSize(int col, int i, int width, int height)
 {
     tracks[col].block[i].draw_element->setSize(width,height);
 }

 QSize ListControll::getDrawSize(int col, int i)
 {
     return tracks[col].block[i].draw_element->getSize() ;
 }

 QRect ListControll::getDrawRect(int col, int i)
 {
     return tracks[col].block[i].draw_element->getRect();
 }




int ListControll::getBlockTime(int col, int i ) const
{
    return tracks[col].block[i].draw_element->getLifeTime();
}

Element ListControll::getBlock(int col, int i) const
{
    // qDebug() << "getBlock(int col, int i)" << col << " " << i;
    return tracks[col].block[i];
}

Element ListControll::getBlock(QPoint point) const
{
    return tracks[point.x()].block[point.y()];
}

int ListControll::getTrackTime( int col) const
{

    return tracks[col].time;
}

int ListControll::getMaxTrackTime( ) const
{
    return maxTrackTime;
}

int ListControll::getTrackSize(int col) const
{
    int temp = tracks[col].block.size();
    // qDebug()  << "FHFHHFHFHFHFH getTrackSize = " << temp;
    return temp;
}

 bool ListControll::isProjectChanged()
 {
     return isProjectChange;
 }

 void ListControll::setIsProjectChanged(bool value)
 {
     isProjectChange = value;
 }


ListControll::ListControll(QObject *parent) : QObject(parent), QQuickImageProvider(QQuickImageProvider::Image)
{
   resetProjectToDefault();
   /* QList <QString>  temp;
        temp.append("1");
        temp.append("2");
        temp.append("3");
        temp.append("4");
        temp.append("5");
        temp.append("6");
        test.append(temp);
    QList <int>  temp_int;
        temp_int.append(200);
        temp_int.append(200);
        temp_int.append(200);

        temp_int.append(200);
        temp_int.append(200);
        temp_int.append(200);
        testWidth.append(temp_int);
*/


    if (qgetenv("QT_QUICK_CORE_PROFILE").toInt()) {\
        QSurfaceFormat f = view.format();\
        f.setProfile(QSurfaceFormat::CoreProfile);\
        f.setVersion(4, 4);\
        view.setFormat(f);\
    }\
    //  view.connect(view.engine(), SIGNAL(quit()), &app, SLOT(quit()));
    new QQmlFileSelector(view.engine(), &view);\
    view.engine()->rootContext()->setContextProperty("timeControll", this);
    view.engine()->rootContext()->setContextProperty("viewerWidget", &view);
    cloneImg = new ImageClone(this);
    view.engine()->addImageProvider("imageProvider", cloneImg);//&image_provider);
    view.setSource(QUrl("qrc:/main.qml")); \
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setPersistentOpenGLContext(true);
    view.setColor("transparent");
    view.setMinimumHeight(205);
    view.setMinimumWidth(500);
    view.setHeight(view.minimumHeight());
    view.setWidth(800);
    view.setFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint | Qt::WindowTitleHint);



//view.setMaximumHeight(215);
    //loadCurrentTextInTheFirstBlockWhenInit();

}
void ListControll::loadCurrentTextInTheFirstBlockWhenInit()
{
    isBlocked = true;
    QPoint p(0,0);
    QString open = "curent.text";
    DrawElement *elm = GenerationDrawElement(open);
    if(elm == NULL)
        return;
    Element &temp = tracks[p.x()].block[p.y()];
    int life_time = temp.draw_element->getLifeTime();
    int start_time = temp.draw_element->getStartDrawTime();
    delete temp.draw_element;
    temp.key = elm->getKey();
    temp.draw_element = elm;
    temp.draw_element->setLifeTime(life_time);
    temp.draw_element->setStartDraw(start_time);
    temp.draw_element->setZ(p.x());
    recountMaxTrackTime();
    isBlocked = false;
    qDebug() <<"loadCurrentTextInTheFirstBlockWhenInit DONE!!!!!!!!!!!!!!!!!!!!!!!";
}

 void ListControll::moveWindow()
 {
    QPoint posMouse = QCursor::pos();
   // view.setPosition(  pos.x() + x , pos.y()  + y);
    view.setPosition( posMouse - framaMousePosition);
  //  setPrevMousePosition(posMouse);
   // qApp->processEvents(QEventLoop::AllEvents, 1000);

 }

 void ListControll::resizeWindowWidth(bool left)
 {
     QPoint posMouse = QCursor::pos();
     if (!left )
     {
         int tt = posMouse.x() - view.x();
         if (tt < view.minimumWidth())
             tt = view.minimumWidth();
       view.setWidth(tt)  ;
     }
     else
     {
         int tt = view.x();
       view.setX(posMouse.x());

       tt = view.width() + tt - view.x();
        if (tt < view.minimumWidth())
        {
            view.setX(view.x() + (tt - view.minimumWidth()));
            tt = view.minimumWidth();
        }

       view.setWidth(tt);
     }
 }

 void ListControll::resizeWindowHeight(bool up)
 {
     QPoint posMouse = QCursor::pos();
     if (!up )
     {
         int tt = posMouse.y() - view.y();
         if (tt < view.minimumHeight())
             tt = view.minimumHeight();
       view.setHeight(tt)  ;
     }
     else
     {
         int tt = view.y();
       view.setY(posMouse.y());

       tt = view.height() + tt - view.y();
        if (tt < view.minimumHeight())
        {
            view.setY(view.y() + (tt - view.minimumHeight()));
            tt = view.minimumHeight();
        }

       view.setHeight(tt);
     }
 }

 void  ListControll::setPrevMousePosition()
 {
     prevMousePosition = QCursor::pos();
 }

    QPoint  ListControll::getPrevMousePosition()
    {
        return prevMousePosition;
    }


 void  ListControll::setFramaMousePosition( const int x,const int y)
 {
     framaMousePosition.setX(x);
     framaMousePosition.setY(y);
 }

/*
void ListControll::setSelectedBlock(int col, int i)
{
      selectedBlock = getBlock(col,i);
      selectedBlockPoint = QPoint(col, i);
}

 Element ListControll::getSelectedBlock()
 {
    return selectedBlock;
 }*/


 void  ListControll::setFramaMousePosition( const QPoint x)
 {
     framaMousePosition = x;
 }

ListControll::~ListControll()
{

}

void ListControll::show()
{
     setSelectedBlockPoint(-1,-1);
    if (QGuiApplication::platformName() == QLatin1String("qnx") || \
          QGuiApplication::platformName() == QLatin1String("eglfs")) {\
        view.showFullScreen();\
    } else {\
        view.show();\
    }\
}

bool ListControll::save(QIODevice* device)
{
    QDataStream stream(device);
    stream << tracks.size() ;
    for (int i=0; i< tracks.size(); i++)
        tracks[i].save(device);
    return true;
}

bool ListControll::load(QIODevice* device)
{
    //QDataStream stream(device);

    tracks.clear();
    int tracks_size;
    QDataStream stream(device);
    stream >> tracks_size ;
    for (int i=0; i< tracks_size; i++)
    {
        Track temp;
         temp.load(device);
         tracks.append(temp);
    }
    recountMaxTrackTime();
    calcPointedBlocks();
    setSelectedBlockPoint(-1, -1);
    return true;
}

void ListControll::close()
{
        view.close();
}

void ListControll::hide()
{
    view.hide();
}

void ListControll::setFocus()
{
   // view.setPosition(view.position());
    if(view.isVisible())
    {
        view.setVisible(false);
        view.setWindowState(Qt::WindowNoState);
        view.setVisible(true);
    }

}

void ListControll::setViewPosition(QPoint pos)
{
    if(view.isVisible())
        view.setPosition(pos);
}

bool ListControll::isVisible()
{
    return view.isVisible();
}

bool ListControll::isActiveWindow()
{
    return view.isActive();
}



 void ListControll::setScalePointerPos( int x)
 {
     scale_pointer_pos = x;
 //// qDebug() << "RRRRRRRRRRRRRRR scale_pointer_pos=" << scale_pointer_pos;
 }

 int ListControll::getScalePointerPos( )
 {
     return scale_pointer_pos;
 }

 QList <Element> ListControll::getPointedBlocks( )
 {
     return pointed_block;
 }
 QList <DrawElement*> ListControll::getPointedBlocksDE( )
 {
     QList <DrawElement*> res;
     for(auto elm : pointed_block)
         res.append(elm.draw_element);
 //   qDebug() << "Curent            count of element in scene   =   " << pointed_block.size();
     return res;
 }

 void ListControll::calcPointedBlocks( )
 {
     pointed_block.clear();
   //  qDebug() << "SIZE          " << tracks[0].block.size();
     if(!isBlocked)
     {
         for (int i=0; i<tracks.size(); i++)
         {
             int blockXstart = 0;
             for (int y=0; y<tracks[i].block.size(); y++ )
             {
                 int blockXend =blockXstart + tracks[i].block[y].draw_element->getLifeTime();
                 if (scale_pointer_pos <= blockXend)
                 {
                     pointed_block.append(tracks[i].block[y]);
                     // qDebug() << "POP: " << i<< " "<<y;
                 break;
                 }
                  blockXstart = blockXend;
             }

         }
     }
      //  qDebug() << "qweqweqweqweqweqw";
/*
	*		show curent play element
*/
      // qDebug() << "FFFFFFFFFFFFFFF getPointedBlocks size" << pointed_block.size()
               //<< " scale_pointer_pos " << scale_pointer_pos;
     /* for(int i = 0; i <pointed_block.size(); i++)
      {
          // qDebug() << i <<  "   " << pointed_block[i].draw_element->getType();
      }*/

 }

 void ListControll::calcPointedBlocksAtTime(int ms )
 {
     pointed_block.clear();
     for (int i=0; i<tracks.size(); i++)
     {
         int blockXstart = 0;
         for (int y=0; y<tracks[i].block.size(); y++ )
         {
             int blockXend = blockXstart + tracks[i].block[y].draw_element->getLifeTime();
             if (ms <= blockXend)
             {
                 pointed_block.append(tracks[i].block[y]);
                 //// qDebug() << "POP: " << i<< " "<<y;
             break;
             }
              blockXstart = blockXend;
         }
     }
 }

void ListControll::calcPointedBlocksAtTime( )
 {
     int ms = getPlayTime();
     pointed_block.clear();
     for (int i=0; i<tracks.size(); i++)
     {
         int blockXstart = 0;
         for (int y=0; y<tracks[i].block.size(); y++ )
         {
             int blockXend =blockXstart + tracks[i].block[y].draw_element->getLifeTime();
             if (ms <= blockXend)
             {
                 pointed_block.append(tracks[i].block[y]);
                 //// qDebug() << "POP: " << i<< " "<<y;
             break;
             }
              blockXstart = blockXend;
         }
     }
 }

 QList <Element> ListControll::getPointedBlocksAtTime( )
 {
     return pointed_block;
 }

 void  ListControll::play()
 {
    // // qDebug() << "FFFFFFFFFFFFFFF  emit playSignal();";
    emit playSignal();
     timer.restart();
     if (isPlayPauseStop==3)
             time_sum = 0;
     isPlayPauseStop = 1;

 }
 void  ListControll::pause()
 {
    emit pauseSignal();
    time_sum += timer.elapsed();
    isPlayPauseStop = 2;
 }

void  ListControll::stop()
{
    emit stopSignal();
   // time_sum = 0;
    isPlayPauseStop = 3;
    calcPointedBlocks();

}

qint64 ListControll::getPlayTime()
{
    if (isPlayPauseStop ==1 )
        return time_sum + timer.elapsed();
    else
        if (isPlayPauseStop ==2 )
        return time_sum ;
            else
                return 0;
}

QImage ListControll::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{

   // QUrl url("http://lorempixel.com/" + id);
    //QNetworkReply* reply = manager->get(QNetworkRequest(url));
   // QEventLoop eventLoop;
   // QObject::connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
   // eventLoop.exec();
 //   if (reply->error() != QNetworkReply::NoError)
 //       return QImage();
    /*if(getSelectedBlock().draw_element->getIcon().isNull())
        return QImage(":/0.png");

    return getSelectedBlock().draw_element->getIcon();*/

            //= QImage::fromData(reply->readAll());
    /*size->setWidth(image.width());
    size->setHeight(image.height());*/
    // qDebug() << "

    if(!isBlocked)
    {
        QVector <QStringRef> argv = id.splitRef('+');
        Element elm = getBlock(argv[0].toInt(), argv[1].toInt());
        QImage img = elm.draw_element->getIcon();
       //qDebug() << "IMAGE  pp " << getBlock(argv[0].toInt(), argv[1].toInt()).draw_element->getKey();

        if(img.isNull())
        {
                   // qDebug() << "AAAAAAAAAAAAAAAAAA  id = " << id ;
            if(elm.draw_element->getTypeId() == Text)
                return QImage(":/iphone_toolbar_icons/Document-Icon.png");

            return QImage(":/icons/12video icon.png");
        }



        return img;
    }

}

 void ListControll::update()
 {
     emit updateSignal();
     if (this->getMaxTrackTime() < getPlayTime())
     {
         emit stopSignal();
         isPlayPauseStop = 3;
     }
 }



 int  ListControll::getTracksNumber()
 {
     return tracks.size();
 }

 void ListControll::sendUpdateModel()
 {
     emit updateModel();
 }

  void ListControll::emitNewProject()
  {
      emit newProjectSignel();
  }

    void ListControll::emitOpenProject()
    {
        emit openProjectSignel();
        qDebug() <<"AAAAAAAAAAAAAAAAAAAAAAAAAAAA";
    }

    void ListControll::emitSaveProject()
    {
        emit saveProjectSignel();
    }

    void ListControll::emitResetProject()
    {
        emit resetProjectSignel();
    }

    void ListControll::zoomMinus()
    {
        this->changeScaleScrollChildren(-zoom_speed);
        emit updateModel();
    }

    void  ListControll::zoomPlus()
    {
        this->changeScaleScrollChildren(zoom_speed);
         emit updateModel();
    }


    int  ListControll::resetProjectToDefault()
    {
        for (int i=0; i< tracks.size(); i++)
            tracks[i].clear();
        tracks.clear();
        maxTrackTime = 0;
        time_sum = 0;
        selectedBlockPoint = QPoint(-1,-1);
        addNewTrack( );
        recountMaxTrackTime();
        recountMaxTrackTime();
        calcPointedBlocks();
    }

    void ListControll::convertCurentBlockToText()
    {
        Element &elm = tracks[selectedBlockPoint.x()].block[selectedBlockPoint.y()];
        DrawTextElm *text = new DrawTextElm(NULL);
        text->copy(elm.draw_element);
        delete elm.draw_element;
        elm.draw_element = text;
        emit updateSelectedBlock(selectedBlockPoint);
    }


