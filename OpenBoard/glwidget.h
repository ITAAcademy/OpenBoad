#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QVector>

class GLWidget : public QGLWidget
{
public:
    GLWidget();

public:
    static GLWidget *glRender();
    void drawText(QString str,int x,int y);
    QVector<QString> textArray;

protected:
    void initializeGL();
    void paintGL();
    void resizeGL();
};

#endif // GLWIDGET_H
