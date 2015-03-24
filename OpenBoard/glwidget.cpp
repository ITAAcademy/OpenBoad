#include "glwidget.h"
#include <QFont>
#include <QDebug>
#include <QThread>
#include <QChar>

static GLWidget *msRender = 0;

GLWidget *GLWidget::glRender()
{
    return msRender;
}

GLWidget::GLWidget()
    :strstr()
    ,index(0)
    ,mIsAnimatedStart(false)

{
    msRender = this;
}

void GLWidget::initializeGL()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glClearColor(0.5,0.5,0.5,1);
    glOrtho(0, 450, 600, 0, 0, 1);
    glEnable(GL_SMOOTH);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_TEXTURE_2D);
}
void GLWidget::paintGL()
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glMatrixMode( GL_PROJECTION );
   glLoadIdentity();
   glViewport( 0, 0, width(), height() );
   glOrtho( 0.0, width(), height(), 0.0, -1.0, 1.0 );
   glMatrixMode( GL_MODELVIEW );

   //for rendering text into widget in two cases: if mIsAnimatedStart=true and mIsAnimatedStart=false
   for(int i = 0; i < textArray.size(); ++i){
        QString tmp = textArray[i];
        if (mIsAnimatedStart){
          if(index <= tmp.length()) {
              strstr.append(tmp[index]);
              int x = 10;
              drawText(strstr,x,i);
              x += 10;
              index += 1;
        }else  {
            mTimer->stop();
            drawText(strstr,1,1);
            index = 0;
            mIsAnimatedStart = false;
        }
        }else{
          drawText(tmp,1,1);
        }
        ++i;
      }
}
void GLWidget::resizeGL()
{
}

void GLWidget::drawText(QString str,int x,int y)
{
    x = 10;
    y = 20;
    GLWidget::glRender()->renderText( x, y, 0.0, str, QFont("Arial", 12, QFont::Bold, false) );
}

void GLWidget::drawAnimated()
{
    mIsAnimatedStart = true;
    mTimer = new QTimer(this);
    mTimer->start(300);
    connect(mTimer, SIGNAL(timeout()), this, SLOT(repaint()));
}
void GLWidget::stopAnimated()
{
    mTimer->stop();
    mIsAnimatedStart = false;
    strstr = "";
    index = 0;
}

void GLWidget::pauseAnimated()
{
    mTimer->stop();
    mIsAnimatedStart = false;
}
