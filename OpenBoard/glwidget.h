#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QtOpenGL/QGLWidget>
#include <QVector>
#include <QTimer>

#include <QElapsedTimer>
#include "windows.h"

class GLWidget : public QGLWidget
{
public:
    GLWidget();

public:
    bool    mIsAnimatedStart;

    static GLWidget *glRender();
    void drawText(QString str,int x,int y);
    void drawAnimated();
    void stopAnimated();
    void pauseAnimated();

    QVector<QString> textArray;
    QString strstr;


protected:
    void initializeGL();
    void paintGL();
    void resizeGL();


private:
    int     index;

    QElapsedTimer mElapsedTimer;
    QTimer *mTimer;

};

#endif // GLWIDGET_H
