#ifndef VIDEORENCODER_H
#define VIDEORENCODER_H

#include <QObject>
#include <QWidget>
#include <QString>
#include <QThread>
#include <QDebug>
#include <QPainter>
#include <QMutex>
#include <QMutexLocker>
#include <QLockFile>

#include <QtMultimedia/QAudioRecorder>
#include <QtMultimedia/QAudioProbe>
#include <QGLWidget>


#include <Encoder>
#include <VideoCodecSettings>

#include <QMutex>
#include <ScreenGrabber>
#include <AudioCodecSettings>


class AV_REncoder : public QThread
{
    Q_OBJECT

    Encoder *m_encoder = NULL;

    QAudioRecorder *audioRecorder = NULL;
    QAudioProbe *probe = NULL;

    QElapsedTimer tick;

    QMutex mutex;
    QMutexLocker *locker;

    QImage frame;
    unsigned char *data;
    bool bRun;
    bool bPause;
    QString fileName;
public:
    bool newImage = false;
    explicit AV_REncoder(QObject *parent = 0);
    ~AV_REncoder();

    VideoCodecSettings videoCodecSettings() const;
    AudioCodecSettings audioCodecSettings() const;
    QGLWidget *grabWidget;
    void setFileName( QString path);
    void setGrabWidget( QGLWidget *widget);
    void startRecord();
    void pause();
    void stop();
    void encodeAudioData(QByteArray array);
    void run();
    QImage getFrame() const;
    void setFrame(const QImage &value);

private slots:
    void onStart();
    void displayErrorMessage();
    void processAudioBuffer(const QAudioBuffer& buffer);

    void initAudio();
    void initVideo();
public slots:

signals:

public slots:
};

#endif // VIDEORENCODER_H
