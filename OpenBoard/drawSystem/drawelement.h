#ifndef DrawElement_H
#define DrawElement_H
//#pragma once
#include <QObject>
#include <QImage>
#include <QTimer>
#include <QElapsedTimer>
#include <QThread>
#include <QPoint>
#include <QSize>
#include <QRect>
#include <qvector.h>
#include <shadereffect.h>
#include "../openglwidget.h"
//#include "../TimeLine/group.h"

class Group;

enum Element_type { Text, Video, Audio, Image,Brushh,Empty,Last_forProtect};

struct AnimStateTime{
    int state; //no anim = 0,begin = 1,end = 2, begin & end =3
    int time;
    AnimStateTime(int a,int b)
    {
        state = a;
        time = b;
    }
    AnimStateTime()
    {
        state = 0;
        time = 10;
    }
};


class DrawElement : public QObject
{

    Q_OBJECT

protected:

    QString block_border_color = "white";
    Group *group_wich_el_belong = NULL;
    int blockIndex = -1;
    int blockColumn = -1;
    AnimStateTime anim_state_time;
    FBOWrapper fboWrapper;

    float border = 0;
    QImage icon;
    QByteArray iconData;

    QString key;
    QVector<ShaderEffect> effects;
    int lifeTime = 0;
    int tickTime;
    int playTimeUntilFreeze;//zero value can crash program if another value not assigned before play.. causing by div by zero exception :(

    int delay;
    int startDrawTime;
    int x, y, z, width, height;
    int keyCouter = 0;

    bool aspectRatio = false;
    QTimer lifeTimer;
    QElapsedTimer tickTimer;
    char type[50];
    Element_type typeId;
    OGLWidget *pDrawWidget;
    bool bPause;
    bool bPlay;
    bool effectChange = false;

    QString lastPath;
    virtual void draw();
public:
    QVector<ShaderEffect> getEffects();
    void setAnimStateTime(AnimStateTime);
    void setAnimStateTime(int , int);
    void setAnimStateTime(QPoint);
    void setAnimState(int);
    void setAnimTime(int);
    QPoint getAnimStateTimePoint();
    AnimStateTime getAnimStateTime();

    FBOWrapper getFBOWrapper();
    explicit DrawElement(OGLWidget *drawWidget = NULL, QObject *parent = 0);
    virtual ~DrawElement();
    void paint(); //new use
    bool load(QString path);
    bool save(QString path);
    void clear();
    bool reloadLastDone();

    void setLastPath(QString path);
    bool load(QIODevice* device, float version);
    bool loadTypeId(QIODevice* device);
    bool loadRest(QIODevice* device, float version);
 bool save(QIODevice* device, QProgressBar *bar);

    QRect getRect();

    DrawElement* getCopy(QObject parentamana);

    void setPosition(QPoint pos);
    void setPosition( int x, int y);
    void setDelay(int d);
    virtual void setSize( QSize pos );
    virtual void setSize( int width, int height);
    QSize getSize();

    void setRect (QRect rect);
    void setRect ( int x, int y, int width, int height);

    int getX() const;
    void setX(int value);

    int getY() const;
    void setY(int value);

    int getZ() const;
    void setZ(int value);

     int getLifeTime() ;
     int setLifeTime(int value, bool feedBack = false, bool visual = true, bool use_value = true);


    int getStartDrawTime() const;
    int setStartDraw(int value);

    QImage getIcon() const;
    void setIcon(const QImage &value);

    bool getPause() const;
    void setPause(bool value);

    void setType( const char name[]);
    QString getType();

    void setTypeId( Element_type);
    Element_type getTypeId();

    virtual int getTickTime() const;
    virtual void setTickTime(int value);

    OGLWidget *getDrawWidget() const;
    virtual bool setDrawWidget(OGLWidget *value);

    QString getKey() const;
    void setKey(const QString &value);

    void copy(DrawElement *elm);
    setFBOWrapper(FBOWrapper wrapper);
    bool save_image(QDataStream &stream, QString filePath);
    int getBlockIndex() const;
    void setBlockIndex(int value);

    int getBlockColumn() const;
    void setBlockColumn(int value);

    Group *getGroupWichElBelong() const;
    void setGroupWichElBelong(Group *value);

    QString getBlockBorderColor() const;
    void setBlockBorderColor(const QString &value);

    setEffects(QVector<ShaderEffect> effects);
    addEffect(ShaderEffect effect);
    clearEffects();
    int getPlayTimeUntilFreeze() const;
    void setPlayTimeUntilFreeze(int value);

    float getBorder() const;
     void setBorder(float value);

signals:
    void playTimeUntilFreezeChangeSignal(int col,int ind, int value);
    void dontUseThisValue();
    void borderColorChangedSignal(int col, int ind ,QString color);
    void sizeChangedSignal(int col, int ind, int newSize, bool visual = true);

public slots:



    QMovie *load_Movie(QDataStream &stream);
protected slots:
    void pause();
    void stop();
    void start();
    void restart();

    bool save_image(QDataStream &stream, QImage img);
    QImage load_image(QDataStream &stream);

protected:
    virtual bool load_add(QDataStream &stream, float version);
    virtual bool save_add(QDataStream &stream);


};

#endif // DrawElement_H
