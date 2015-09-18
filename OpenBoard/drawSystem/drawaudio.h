#ifndef AUDIO_H
#define AUDIO_H
#pragma once
#include "drawelement.h"
#include "../encoder/ffmpeghelp.h"

class DrawAudioElm  :public DrawElement
{
    Q_OBJECT
    QImage image;
    FFmpegHelp decoder;
    int delta;
    GLuint textureIndex = -1;
    int failedLoad = 10;
    QString audioPath;

public:
    explicit DrawAudioElm( OGLWidget *drawWidget, QObject *parent = 0);
    ~DrawAudioElm();
    void draw();

    void setAudioFile( QString path);
    bool load_add(QDataStream &stream);
    bool save_add(QDataStream &stream);
    bool setDrawWidget(OGLWidget *value);
    QString getFilePath();
protected slots:
    void start();
    void stop();

};

#endif // VIDEO_H
