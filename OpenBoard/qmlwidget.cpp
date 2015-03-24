#include "qmlwidget.h"



QmlWidget::QmlWidget(QWidget *parent) :
    QQuickWidget(parent)
{
    //qRegisterMetaType<DrawData>("DrawData");
    engine()->rootContext()->setContextProperty(QLatin1String("forma"), this);
    isPlay = false;
    setSource(QUrl("qrc:/draw.qml"));
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint | Qt::WindowTitleHint);
    this->setResizeMode(QQuickWidget::SizeRootObjectToView );
    m_screenGrabber = new ScreenGrabber();
    m_audioGrabber = new AudioGrabber();
    m_encoder = new Encoder();
    m_recorder = new Recorder();
    //setCursor(Qt::NoArrow);
    QPixmap pix(1,1);
    QColor color(0,0,0,0);
    pix.fill(color);
    QCursor mCursor(pix);
    setCursor(mCursor);
    /*
     *cursor ALPHA
     *
    */
    connect(m_recorder->encoder(), SIGNAL(stateChanged(Encoder::State)), this, SLOT(onState(Encoder::State)));
    connect(m_encoder, SIGNAL(stateChanged(Encoder::State)), this, SLOT(onState(Encoder::State)));

    /*
     *
     *
    */
    m_screenGrabber->setLatency(70);

    AudioFormat format;
    format.setChannelCount(2);
    format.setSampleRate(44100);
    format.setFormat(AudioFormat::SignedInt16);

    m_audioGrabber->setDeviceIndex(0);
    m_audioGrabber->setFormat(format);

    m_recorder->encoder()->setVideoCodecSettings(videoCodecSettings());
    m_recorder->encoder()->setAudioCodecSettings(audioCodecSettings());
    m_recorder->encoder()->setVideoCodec(EncoderGlobal::H264);
    m_recorder->encoder()->setAudioCodec(EncoderGlobal::MP3);
    m_recorder->encoder()->setOutputPixelFormat(EncoderGlobal::YUV420P);
   // m_player = new AVPlayer(this);
}

QmlWidget::~QmlWidget()
{
    /*delete m_screenGrabber;
    delete m_audioGrabber;
    delete m_encoder;
    delete m_recorder;*/
}

void QmlWidget::moveEvent(QMoveEvent *event)
{
    QRect captureRect(this->pos(), this->size() );
    m_screenGrabber->setCaptureRect(captureRect);
}

QString QmlWidget::getDrawText()
{
    return drawText;
}

void QmlWidget::setDrawText(QString &data)
{
    drawText = data;
}


VideoCodecSettings QmlWidget::videoCodecSettings() const
{
    //x264 loseless fast preset
    VideoCodecSettings settings;
    settings.setCoderType(EncoderGlobal::Vlc);
    settings.setFlags(EncoderGlobal::LoopFilter);
    settings.setMotionEstimationComparison(1);
    settings.setPartitions(EncoderGlobal::I4x4 | EncoderGlobal::P8x8);
    settings.setMotionEstimationMethod(EncoderGlobal::Hex);
    settings.setSubpixelMotionEstimationQuality(3);
    settings.setMotionEstimationRange(16);
    settings.setGopSize(250);
    settings.setMinimumKeyframeInterval(25);
    settings.setSceneChangeThreshold(40);
    settings.setIQuantFactor(0.71f);
    settings.setBFrameStrategy(1);
    settings.setQuantizerCurveCompressionFactor(0.6f);
    settings.setMinimumQuantizer(0);
    settings.setMaximumQuantizer(69);
    settings.setMaximumQuantizerDifference(4);
    settings.setDirectMvPredictionMode(EncoderGlobal::SpatialMode);
    settings.setFlags2(EncoderGlobal::FastPSkip);
    settings.setConstantQuantizerMode(0);
    settings.setPFramePredictionAnalysisMethod(EncoderGlobal::NoWpm);

    return settings;
}

AudioCodecSettings QmlWidget::audioCodecSettings() const
{
    AudioCodecSettings settings;
    settings.setSampleRate(m_audioGrabber->format().sampleRate());
    settings.setChannelCount(m_audioGrabber->format().channelCount());
    settings.setSampleFormat(EncoderGlobal::Signed16);

    return settings;
}

void QmlWidget::drawAnimated()
{
    if(isPlay)
    {
        m_recorder->resume();
        return;
    }
    QString fileName = QFileDialog::getSaveFileName(this, tr("Choose file..."), qApp->applicationDirPath(), tr("Videos (*.avi *.mp4)"));
    if(!fileName.size())
        return;
   // QString filePath = ui->filePathLabel->text() == tr("None") ? "" : ui->filePathLabel->text();
    m_recorder->setImageGrabber(m_screenGrabber);
    m_recorder->setAudioGrabber(m_audioGrabber);
    m_recorder->encoder()->setFilePath(fileName);

    QRect captureRect(this->pos(), this->size() );
    m_screenGrabber->setCaptureRect(captureRect);
    m_recorder->encoder()->setVideoSize(captureRect.size());
    m_recorder->encoder()->setEncodingMode(/*Encoder::VideoAudioMode :*/ Encoder::VideoMode);
    m_recorder->start();
    isPlay = true;
}

void QmlWidget::stopAnimated()
{
    m_recorder->stop();
    isPlay = false;
}

void QmlWidget::pauseAnimated()
{
    m_recorder->pause();
}

void QmlWidget::onState(Encoder::State state)
{
    /*if (state == Encoder::ActiveState) {
            generateFrames();
        }*/
}

QString QmlWidget::getFont()
{
    return boardFont;
}

QString QmlWidget::getColor()
{
    return boardColor;
}

void QmlWidget::setFont(QString &font)
{
    boardFont = font;
    qDebug() << boardFont;
}

void QmlWidget::setColor(QString &color)
{
    boardColor = color;
    qDebug() << boardColor;
}

void QmlWidget::generateFrames()
{
    //generate 500 frames

    for (int i = 0; i < 500; ++i) {
        QImage frame(QSize(1024, 768), QImage::Format_RGB16);
        frame.fill(Qt::yellow);
        QPainter painter(&frame);
        painter.setPen(QPen(Qt::red, 3));
        painter.drawRect(i, i, 30, 30);
        //ui->statusBar->showMessage(QString::number(i/5) + "%");
        m_encoder->encodeVideoFrame(frame);

        //this is needed to prevent RAM overflow on some computers
        if (i % 20 == 0) {
            while (m_encoder->encodedFrameCount() != i) { qApp->processEvents(); }
        }

        qApp->processEvents();
    }

    m_encoder->stop();

}
bool QmlWidget::IsPlay() const
{
    return isPlay;
}
/*
QString QmlWidget::getColor()
{
    return boardFontColor;
}

QString QmlWidget::getFont()
{
    return boardFont;
}
*/
