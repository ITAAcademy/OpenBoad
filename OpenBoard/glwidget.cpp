#include "glwidget.h"
#include <QFont>

static GLWidget *msRender = 0;

GLWidget *GLWidget::glRender()
{
    return msRender;
}

GLWidget::GLWidget()

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
   static int i = 0;

      for(int i = 0; i < textArray.size(); ++i)
          drawText(textArray[i],1,1);
      ++i;
}

void GLWidget::resizeGL()
{

}
void GLWidget::drawText(QString str,int x,int y){

    GLWidget::glRender()->renderText( 10.0, 20.0, 0.0, str, QFont("Arial", 12, QFont::Bold, false) );
}

