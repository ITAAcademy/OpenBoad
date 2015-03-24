#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QVector>
#include <QTimer>

#include <QElapsedTimer>
#include "windows.h"

class GLWidget : public QGLWidget
{
public:
    GLWidget();

private:
    int index;

    QElapsedTimer mElapsedTimer;
    QTimer *mTimer;

public:
    QVector<QString> textArray;
    QString strstr;

    bool    mIsAnimatedStart;

    static GLWidget *glRender();
    void drawText(QString str,int x,int y);
    void drawAnimated();
    void stopAnimated();
    void pauseAnimated();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL();

};

#endif // GLWIDGET_H
