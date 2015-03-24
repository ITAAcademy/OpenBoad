#ifndef QMLWIDGET_H
#define QMLWIDGET_H

#include <QObject>
#include <QtCore>
#include <QtQml>
#include <QtQuickWidgets>
#include <QFileDialog>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QDesktopServices>
#include <QUrl>
#include <QImage>
#include <QPainter>
#include <QMetaObject>

#include <AudioGrabber>
#include <CameraGrabber>
#include <Recorder>
#include <Encoder>
#include <ScreenGrabber>
#include <VideoCodecSettings>
#include <AudioCodecSettings>
#include <Recorder>
/*
#include <QtAV/QtAV>
#include <QtAVWidgets/QtAVWidgets>

using namespace QtAV;
*/

#define STREAM_FRAME_RATE 10
#define STREAM_DURATION 60

class ScreenGrabber;
class AudioGrabber;
class Encoder;
class Recorder;

struct DrawData{
    QString str;
    int index;
    bool IsPlay;
};

class QmlWidget : public QQuickWidget
{

    QString boardFont;
    QString boardColor;

    Q_OBJECT
    Q_PROPERTY(QString  drawText READ getDrawText WRITE setDrawText NOTIFY drawTextChanged)
public:
    explicit QmlWidget(QWidget *parent = 0);
    ~QmlWidget();
    void moveEvent(QMoveEvent *event);
    void drawMyText(QString str);
    void drawAnimated();
    void stopAnimated();
    void pauseAnimated();
    VideoCodecSettings videoCodecSettings() const;
    AudioCodecSettings audioCodecSettings() const;
    void setDrawText( QString &data );
    //Q_INVOKABLE void Encode( QImage img );
    QString getDrawText();

signals:
    void drawTextChanged();
public slots:
    void onState(Encoder::State state);
    Q_INVOKABLE QString getFont();
    Q_INVOKABLE QString getColor();
    void setFont(QString &font);
    void setColor(QString &color);
private:
    QString drawText;
    Recorder rec;
    ScreenGrabber *m_screenGrabber;
    AudioGrabber *m_audioGrabber;
    Encoder *m_encoder;
    Recorder *m_recorder;
    void generateFrames();
    bool isPlay;
    /*QtAV::VideoOutput *m_vo;
    QtAV::AVPlayer *m_player;*/
public:
    //DrawData        *drawData;
    bool IsPlay() const;
};



#endif // QMLWIDGET_H
