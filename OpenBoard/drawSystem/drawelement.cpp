#include "drawElement.h"
#include "../TimeLine/listcontroll.h"
class ListControll;
OGLWidget *DrawElement::getDrawWidget() const
{
    return pDrawWidget;
}

bool DrawElement::setDrawWidget(OGLWidget *value)
{


    if(value == pDrawWidget && value != NULL)
    {
      //  qDebug() << "VALUE  " << fboWrapper.errorStatus;
        if(fboWrapper.errorStatus != 0 &&  pDrawWidget->isInit()){
            setFBOWrapper(pDrawWidget->initFboWrapper(pDrawWidget->getWax(),pDrawWidget->getWay()));//TODO
            for (int i = 0; i <effects.length(); i++)
                effects[i].setShaderWrapper(pDrawWidget->getShaderPrograms()[effects[i].getShaderWrapperIndex()]);
        }
        return false;
    }


    if(pDrawWidget != NULL)
    {
        disconnect(pDrawWidget, SIGNAL(startSignal()), this, SLOT(start()));
        disconnect(pDrawWidget, SIGNAL(stopSignal()), this, SLOT(stop()));
        disconnect(pDrawWidget, SIGNAL(pauseSignal()), this, SLOT(pause()));
    }


    pDrawWidget = value;

    connect(pDrawWidget, SIGNAL(startSignal()), this, SLOT(start()));
    connect(pDrawWidget, SIGNAL(stopSignal()), this, SLOT(stop()));
    connect(pDrawWidget, SIGNAL(pauseSignal()), this, SLOT(pause()));

    for (int i = 0; i <effects.length(); i++)
        effects[i].setShaderWrapper(pDrawWidget->getShaderPrograms()[effects[i].getShaderWrapperIndex()]);

   // qDebug() << "before alpha effect created";
   // qDebug() << "pDrawWidget->getShaderPrograms().length:"<<pDrawWidget->getShaderPrograms().length();


    if(fboWrapper.errorStatus != 0  && pDrawWidget->isInit())
    {
       // qDebug()<<"BEFORE SETFBOWRAPPER";
        setFBOWrapper(pDrawWidget->initFboWrapper(pDrawWidget->getWax(),pDrawWidget->getWay()));//TODO

    qDebug()<<"AFTER SETFBOWRAPPER";

 //qDebug()<<"AFTER SETFBOWRAPPER";

}
    return true;
}

QString DrawElement::getKey() const
{
    return key;
}

void DrawElement::setKey(const QString &value)
{
    key = value;
}
DrawElement::DrawElement(OGLWidget *drawWidget, QObject *parent) : QObject(parent)
{
    pDrawWidget = drawWidget;
    bPause = false;
    bPlay = false;
    x = 0;
    y = 0;
    z = 0;
    width = 100;
    height = 100;
    keyCouter = 0;
    lifeTime = -1;
    icon = QImage();
    typeId = Element_type::Empty;
    fboWrapper.errorStatus = -1;

    if(pDrawWidget != NULL)
    {
        connect(pDrawWidget, SIGNAL(startSignal()), this, SLOT(start()));
        connect(pDrawWidget, SIGNAL(stopSignal()), this, SLOT(stop()));
        connect(pDrawWidget, SIGNAL(pauseSignal()), this, SLOT(pause()));
    }
   // qDebug() << "RRRR BEFORE";
   // if (pDrawWidget)
//fboWrapper=pDrawWidget->initFboWrapper();
 //qDebug() << "RRRR AFTER";
}

void DrawElement::copy(DrawElement *elm)
{
    lifeTime = elm->getLifeTime();
    tickTime = elm->getTickTime();
    startDrawTime = elm->getStartDrawTime();
    x = elm->getX();
    y = elm->getY();
    z = elm->getZ();
    width = elm->getSize().width();
    height = elm->getSize().height();
    //typeId = elm->getTypeId();
}

DrawElement::~DrawElement()
{


 //qDebug() << "delete DrawElement";
    if(pDrawWidget != NULL)
    {
        pDrawWidget->deleteFBO(fboWrapper);
        disconnect(pDrawWidget, SIGNAL(startSignal()), this, SLOT(start()));
        disconnect(pDrawWidget, SIGNAL(stopSignal()), this, SLOT(stop()));
        disconnect(pDrawWidget, SIGNAL(pauseSignal()), this, SLOT(pause()));
    }
}

void DrawElement::paint()
{
  //  qDebug() << "paint on buffer:"<<fboWrapper.frameBuffer;
    if(fboWrapper.errorStatus == 0)
    {
      bool drawToSecondBuffer=false;
     // bool currentDrawToSecondBuffer=false;
        if (effects.isEmpty())
        {
              pDrawWidget->bindBuffer(fboWrapper.frameBuffer);
              draw();//Draw original image one time without any effects
              //qDebug() << "EFFECTS EMPTY !!!";
        }
        else
        {
            if (effects.length()>1)
            {
                drawToSecondBuffer=true;
                pDrawWidget->clearFrameBuffer(pDrawWidget->getPingPongFBO());
                //pDrawWidget->clearFrameBuffer(pDrawWidget->getMainFBO());
            }
            unsigned int playTime = pDrawWidget->getTimeLine()->getPlayTime();

            int effectsUsedInOneTime=0;
         //   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); glEnable( GL_BLEND );
            for (int i=0;i<effects.length();i++)
            {

                unsigned int beginAtTime;
                if(!effects[i].getAnchorToEnd())
                    beginAtTime = effects[i].getStartTimeMS()+ startDrawTime ;
                else
                {
                    beginAtTime = lifeTime + startDrawTime - effects[i].getEffectTimeHowLong();
                }

                unsigned int endAtTime = beginAtTime + effects[i].getEffectTimeHowLong();
                float keyFrame = 1;
                if(endAtTime-beginAtTime!=0)
                    keyFrame=(float)(playTime-beginAtTime)/(endAtTime-beginAtTime);

                if (playTime >= beginAtTime && playTime <= endAtTime)
                {
                    qDebug() << keyFrame;
                    if(drawToSecondBuffer)
                    {
                        //qDebug() << "FIRST DRAW TO PING-PONG FRAME BUFFER";
                        pDrawWidget->bindBuffer(pDrawWidget->getPingPongFBO().frameBuffer);
                       // qDebug() << "Shader program ("<<i<<"):"<<effects[i].getShaderWrapper()->getShaderProgram();
                        pDrawWidget->getOglFuncs()->glUseProgram(effects[i].getShaderWrapper()->getShaderProgram());
                      //  float keyFrame = (float)(pDrawWidget->getTimeLine()->getPlayTime()-startDrawTime)/lifeTime;//MOVE UP LATER
                        ShaderEffect::setUniformAnimationKey(pDrawWidget,effects[i],keyFrame);
                        ShaderEffect::setUniformResolution(pDrawWidget,effects[i],fboWrapper.tWidth,fboWrapper.tHeight);
                        ShaderEffect::setUniformReverse(pDrawWidget,effects[i],effects[i].getReverse());

                        if (i==0)
                            draw();
                        else
                            pDrawWidget->drawTexture(0,0,fboWrapper.tWidth,fboWrapper.tHeight,
                                                    fboWrapper.bindedTexture,0,1,1,z );
                    }
                    else
                    {
                   // qDebug() << "FIRST DRAW TO MAIN FRAME BUFFER";
                    pDrawWidget->bindBuffer(fboWrapper.frameBuffer);
                    pDrawWidget->getOglFuncs()->glUseProgram(effects[i].getShaderWrapper()->getShaderProgram());
                   // float keyFrame = (float)(pDrawWidget->getTimeLine()->getPlayTime()-startDrawTime)/lifeTime;//MOVE UP LATER
                    ShaderEffect::setUniformAnimationKey(pDrawWidget,effects[i],keyFrame);
                    ShaderEffect::setUniformResolution(pDrawWidget,effects[i],
                                                       pDrawWidget->getPingPongFBO().tWidth,pDrawWidget->getPingPongFBO().tHeight);
                    ShaderEffect::setUniformReverse(pDrawWidget,effects[i],effects[i].getReverse());

                    if (i==0)
                        draw();
                    else
                    pDrawWidget->drawTexture(0,0,pDrawWidget->getPingPongFBO().tWidth,
                                             pDrawWidget->getPingPongFBO().tHeight,
                                            pDrawWidget->getPingPongFBO().bindedTexture,0,1,1,z );


                    }
                    effectsUsedInOneTime++;
                    drawToSecondBuffer=!drawToSecondBuffer;
                }

            }

            if (effectsUsedInOneTime==0){
                pDrawWidget->bindBuffer(fboWrapper.frameBuffer);
                draw();//Draw original image one time without any effects

            }
            else
            if (!drawToSecondBuffer)
            {
                pDrawWidget->bindBuffer(fboWrapper.frameBuffer);
                // float keyFrame = (float)(pDrawWidget->getTimeLine()->getPlayTime()-startDrawTime)/lifeTime;//MOVE UP LATER
                pDrawWidget->drawTexture(0,0,pDrawWidget->getPingPongFBO().tWidth,
                pDrawWidget->getPingPongFBO().tHeight,
                pDrawWidget->getPingPongFBO().bindedTexture,0,1,1,z );


            }
        }
        //pDrawWidget->getOglFuncs()->glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
        pDrawWidget->getOglFuncs()->glUseProgram(0);
        pDrawWidget->bindBuffer(0);


       /* int keyUnifrom = pDrawWidget->context()->functions()->glGetUniformLocation(
                    pDrawWidget->getShaderPrograms()[0].getShaderProgram(),"animationKey");
        float keyFrame = (float)(pDrawWidget->getTimeLine()->getPlayTime()-startDrawTime)/lifeTime;

        qDebug() << "KEY FRAME:"<<keyFrame;


        pDrawWidget->context()->functions()->glUseProgram( pDrawWidget->getShaderPrograms()[0].getShaderProgram());
        pDrawWidget->context()->functions()->glUniform1f(keyUnifrom,keyFrame);
*/
        if(aspectRatio)
            pDrawWidget->paintBufferOnScreen(fboWrapper,x, y, width, width, z);
        else
            pDrawWidget->paintBufferOnScreen(fboWrapper,x, y, width, height, z);
        // pDrawWidget->context()->functions()->glUseProgram(0);
    }
    else
        qWarning() << "In curent draw element fboWraper is not init!!!";
}

void DrawElement::draw()
{

}

FBOWrapper DrawElement::getFBOWrapper()
{
 return fboWrapper;
}

DrawElement::setFBOWrapper(FBOWrapper wrapper)
{
    this->fboWrapper=wrapper;
}

bool DrawElement::load(QString path)
{
    QFile appFile(path);
    if(!appFile.exists())
        return false;
    lastPath = path;
    appFile.open(QFile::ReadOnly);
    this->load(&appFile);   
    appFile.close();

}

bool DrawElement::load(QIODevice* device)
{
    /*QDataStream stream(device);

    int temp_type;
    stream >> temp_type  >> key >> lifeTime >> tickTime >> startDrawTime >> x >> y >> z >> width >> height >> keyCouter;
   typeId = static_cast<Element_type>(temp_type);
    icon = load_image(stream);
    load_add(stream);*/
    loadTypeId(device);
    loadRest(device);
}
bool DrawElement::loadTypeId(QIODevice* device)
{
    QDataStream stream(device);
    int temp_type;
    stream >> temp_type  ;
   typeId = static_cast<Element_type>(temp_type);
}
bool DrawElement::loadRest(QIODevice* device)
{
    //qDebug() << "load rest begin";
    QDataStream stream(device);
    stream  >> key >> lifeTime >> tickTime >> startDrawTime >> x >> y >> z >> width >> height >> keyCouter;
    //if (typeId == Element_type::Image)
        icon = load_image(stream);

    int effectsLength = 0;
    stream >> effectsLength;
    qDebug() << "effectsLength:"<<effectsLength;
    effects.clear();
    effects.resize(effectsLength);

   /* load_add(stream);
    //qDebug() << "load add";
    int effectsLength = 0;
    stream >> effectsLength;
    //qDebug() << "effectsLength:"<<effectsLength;*/

    for (int i = 0 ; i < effectsLength;i++)
    {
        effects[i].load(stream);
    }

     qDebug() << "load rest end";
    load_add(stream);
    qDebug() << "load add";


}

bool DrawElement::save(QIODevice* device)
{
    QDataStream stream(device);

    int temp_type = static_cast<int>(typeId);
    int resultStatus=0;
    stream << temp_type << key << lifeTime << tickTime << startDrawTime << x << y << z << width << height << keyCouter;
    //if (typeId == Element_type::Image)
        //save_image(stream, icon);
    if (!lastPath.isEmpty())
       resultStatus = save_image(stream,lastPath,icon.format());
    else
       save_image(stream, icon );

    stream << effects.length();
    for (int i = 0 ; i < effects.length();i++)
        effects[i].save(stream);

    save_add(stream);

}

bool DrawElement::save(QString path)
{
    QFile appFile(QString(path + "." + type));
    appFile.open(QFile::WriteOnly);
    this->save(&appFile);
    appFile.close();
    /*QImage icon;
    int lifeTime;
    int tickTime;
    int startDrawTime;
    int x, y, z, width, height;
    int keyCouter;
    QTimer lifeTimer;
    QElapsedTimer tickTimer;

    OGLWidget *pDrawWidget;
    bool bPause;
    */

}

void DrawElement::clear()
{

}

bool DrawElement::reloadLastDone()
{
    if(lastPath.isNull())
        return false;
    QByteArray ba;
    QDataStream stream( &ba, QIODevice::ReadWrite );
    //qDebug() << "MY1" << lifeTime;
    stream << key << lifeTime << tickTime << startDrawTime << x << y << z << width << height << keyCouter;
    stream.device()->reset();
    load(lastPath);
    stream >> key >> lifeTime >> tickTime >> startDrawTime >> x >> y >> z >> width >> height >> keyCouter;
    //qDebug() << "MY2" << lifeTime;
}

void DrawElement::setLastPath(QString path)
{
    lastPath=path;
}

QRect DrawElement::getRect()
{
    return QRect(x, y, width, height);
}

void DrawElement::setPosition(QPoint pos)
{
    x = pos.x();
    y = pos.y();
}

 DrawElement* DrawElement::getCopy(QObject parentamana)
 {
    /* switch(typeId)
     {
     case Element_type::Image :
     //6666666677777777
          DrawImageElm *elm = new DrawImageElm(drawWidget, parent);
          elm->load(path);
                 elm->setTypeId(Element_type::Image);
                 elm->setKey(target.baseName());
          return (DrawElement*) elm;
     break;
     case Element_type::Text

     }
      DrawElement *elm = new DrawElement(drawWidget, parentamana);

     return (DrawElement) this; //777777
     */
     DrawElement *elm;
     memcpy(elm, this,sizeof(DrawElement));
            return elm;
 }

void DrawElement::setPosition(int x, int y)
{
    this->x = x;
    this->y = y;
}

void DrawElement::setDelay(int d)
{
    delay=d;
}

void DrawElement::setSize(QSize pos)
{
    width = pos.width();
    height = pos.height();
}

void DrawElement::setSize(int width, int height)
{
    this->width = width;
    this->height = height;
}

void DrawElement::setRect(QRect rect)
{
    x = rect.x();
    y = rect.y();
    width = rect.width();
    height = rect.height();
}

void DrawElement::setRect(int x, int y, int width, int height)
{
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
}
int DrawElement::getX() const
{
    return x;
}

void DrawElement::setX(int value)
{
    x = value;
}

int DrawElement::getY() const
{
    return y;
}

void DrawElement::setY(int value)
{
    y = value;
}

int DrawElement::getZ() const
{
    return z;
}

void DrawElement::setZ(int value)
{
    z = value;
}

int DrawElement::getLifeTime() const
{
    return lifeTime;
}

void DrawElement::setLifeTime(int value)
{
    lifeTime = value;
}

int DrawElement::getStartDrawTime() const
{
    return startDrawTime;
}

void DrawElement::setStartDraw(int value)
{
    startDrawTime = value;
}

QImage DrawElement::getIcon() const
{
    return icon;
}

void DrawElement::setIcon(const QImage &value)
{
    icon = value;
}

bool DrawElement::getPause() const
{
    return bPause;
}

void DrawElement::setPause(bool value)
{
    bPause = value;
}

void DrawElement::setType(const char name[])
{
    strcpy(type, name);
}

QString DrawElement::getType()
{
    return QString(type);
}

void DrawElement::setTypeId( Element_type val)
{
    this->typeId = val;
}

Element_type DrawElement::getTypeId()
{
    return this->typeId;
}

void DrawElement::pause()
{
    bPause = true;
}

void DrawElement::stop()
{
    ////qDebug() << "stop";
    bPause = false;
    keyCouter = 0;
    bPause = false;
    bPlay = false;
    tickTimer.restart();
}

void DrawElement::start()
{
    if (!bPause)
    keyCouter = 0;
    bPause = false;
    bPlay = true;

    /*
     * NOT NEED SLOW
     *
     * reloadLastDone();
     *
     */


    //qDebug() << "START" << lastPath;
}

void DrawElement::restart()
{
    keyCouter = 0;
    tickTimer.restart();
}

bool DrawElement::load_add(QDataStream &stream)
{

}

bool DrawElement::save_add( QDataStream &stream)
{

}

bool DrawElement::save_image(QDataStream &stream, QImage img)
{
    stream << img.size().width() << img.size().height() << (int)img.format();

    QByteArray ba;
    QBuffer buffer(&ba); // QBuffer inherits QIODevice
    buffer.open(QIODevice::WriteOnly);
    img.save(&buffer,"PNG");

    buffer.close();
    stream << ba.length() << ba;
    //stream.writeRawData((const char*)img.bits(), img.byteCount());
    //qDebug() <<"image saved:"<<img.byteCount();
}
bool DrawElement::save_image(QDataStream &stream, QString filePath,QImage::Format format)
{
    stream << (int)0 << (int)0 << (int)0;
QFile file(filePath);
file.open(QIODevice::ReadOnly);
if (filePath.isEmpty())return -1;
    QByteArray ba = file.readAll();

    stream << ba.length() << ba;
    file.close();
    qDebug() << "SAVE_IMAGE";
    //stream.writeRawData((const char*)img.bits(), img.byteCount());
    //qDebug() <<"image saved:"<<img.byteCount();
}

QMovie *DrawElement::load_Movie(QDataStream &stream)
{
    int w, h, format;
    stream >> w >> h >> format;
    QByteArray ba;
    int baLength=0;
    stream >> baLength;
    stream >> ba;
    QMovie *mov;
    QBuffer buffer(&ba); // QBuffer inherits QIODevice
    buffer.open(QIODevice::ReadOnly);
    mov = new QMovie(&buffer, ba);
//img_temp.load(&buffer,"PNG");

buffer.close();
    //stream.readRawData((char*)img_temp.bits(), img_temp.byteCount());
    return mov;
}

QImage DrawElement::load_image(QDataStream &stream)
{
    int w, h, format;
    stream >> w >> h >> format;
    QByteArray ba;
    int baLength=0;
    stream >> baLength;
    stream >> ba;
    QImage img_temp(w, h, (QImage::Format)format);
    img_temp.loadFromData(ba);
    //img_temp.save("qwewqewqewqewqewq,png");

    //stream.readRawData((char*)img_temp.bits(), img_temp.byteCount());
    return img_temp;
}


int DrawElement::getTickTime() const
{
    return tickTime;
}

void DrawElement::setTickTime(int value)
{
    tickTime = value;
    // //qDebug()<<"tickTime3:"<<tickTime;
}

QSize  DrawElement::getSize()
{
    return QSize(width,height);
}


