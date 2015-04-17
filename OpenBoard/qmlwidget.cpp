#include "qmlwidget.h"
/*
 *scroll
 *
int k = canvas->property("scroll").toInt() - 10;
canvas->setProperty("scroll", k);
*/
QmlWidget::QmlWidget(QWidget *parent) :
    QQuickWidget(parent)
{
    //qRegisterMetaType<DrawData>("DrawData");
     engine()->rootContext()->setContextProperty(QLatin1String("forma"), this);
    //setSource(QUrl("../OpenBoard/draw.qml"));
    setSource(QUrl("qrc:/draw.qml"));
    m_encoder = new Encoder(this);
<<<<<<< HEAD

    audioRecorder = new QAudioRecorder(this);
    QAudioEncoderSettings audioSettings;
    audioSettings.setCodec("audio/pcm");
    audioSettings.setQuality(QMultimedia::HighQuality);
    audioSettings.setChannelCount(2);
    audioSettings.setSampleRate(44100);
    audioSettings.setEncodingMode(QMultimedia::ConstantQualityEncoding );

    audioRecorder->setOutputLocation(QUrl::fromLocalFile("test"));
    QString container = "audio/x-wav";
    audioRecorder->setEncodingSettings(audioSettings, QVideoEncoderSettings(), container);
    audioRecorder->setAudioInput(audioRecorder->audioInputs().at(0));
    probe = new QAudioProbe;
    connect(probe, SIGNAL(audioBufferProbed(QAudioBuffer)),
            this, SLOT(processBuffer(QAudioBuffer)));
    connect(audioRecorder, SIGNAL(error(QMediaRecorder::Error)), this,
            SLOT(displayErrorMessage()));
    probe->setSource(audioRecorder);

=======
>>>>>>> origin/yuriy
    bRecord = false;
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint | Qt::WindowTitleHint);
    this->setResizeMode(QQuickWidget::SizeRootObjectToView );
    /*
     * init
    */
    lineHeight = 25;
    marginLeft = 20;
    marginTop = 40;
    maxWidth; //размер поле, где выводится текст
    pt = 36;
    font = "LC Chalk";
    isWord  =  true;
    x = marginLeft;
    y = lineHeight + pt;
    indexW = 1;
    indexRow = 0;
    scroll = 0;
    delay = 1000/3;
    //listStr[0] = 0;
    indexInLsit = 1;
    deleteWT = 0;
    fMetrics = new QFontMetrics(QFont(font));
    curStatus = STOP;
    tickTimer.setSingleShot(true);
<<<<<<< HEAD
    fps_timer = new QTimer();
    //fps_timer->setInterval(1000/32);
       fps_timer->setInterval(1000/31);
    fps_timer->setSingleShot(true);
    connect(fps_timer, SIGNAL(timeout()), this, SLOT ( fps_control() ));
    fps_stabilitron = 0;
=======
>>>>>>> origin/yuriy
    /*
     * canvas set
    */

    canvas = rootObject()->findChild<QObject *>("mcanvas");
    maxWidth = width() - marginLeft;
    QMetaObject::invokeMethod(canvas, "init");


    /*
     *cursor ALPHA
     *
    */
    //setCursor(Qt::NoArrow);
    /*QPixmap pix(1,1);
    QColor color(0,0,0,0);
    pix.fill(color);
    QCursor mCursor(pix);
    setCursor(mCursor);*/
<<<<<<< HEAD
    /*QString fileName = QFileDialog::getSaveFileName();
    qDebug() << "qweqw";
    audioRecorder->setOutputLocation(QUrl::fromLocalFile(fileName));*/
   // audioRecorder->record();
=======


>>>>>>> origin/yuriy

}

QmlWidget::~QmlWidget()
{
    delete m_encoder;
    if(fMetrics != 0)
        delete fMetrics;
<<<<<<< HEAD
    delete audioRecorder;
=======
>>>>>>> origin/yuriy
}

void QmlWidget::moveEvent(QMoveEvent *event)
{

}

void QmlWidget::paintEvent(QPaintEvent *event)
{
    if(curStatus == PLAY && bRecord)
<<<<<<< HEAD
    {
    //    m_encoder->encodeVideoFrame(this->grabFramebuffer());
        /*if (fps_stabilitron % 25 == 0) {
            while (m_encoder->encodedFrameCount() != fps_stabilitron) { qApp->processEvents(); }
            m_encoder->encodeVideoFrame(this->grabFramebuffer());

        }
            fps_stabilitron++;*/
    }
=======
        m_encoder->encodeVideoFrame(this->grabFramebuffer());
>>>>>>> origin/yuriy
}

void QmlWidget::resizeEvent(QResizeEvent *envent)
{
    QQuickWidget::resizeEvent(envent);
    maxWidth = width() - marginLeft;
}

void QmlWidget::mousePressEvent(QMouseEvent *event)
{
    //crossOutLastSymbol();
}

QString QmlWidget::getDrawText()
{
    return drawText;
}

void QmlWidget::setDrawText(QString data)
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
<<<<<<< HEAD
    settings.setMinimumKeyframeInterval(2);
=======
    settings.setMinimumKeyframeInterval(25);
>>>>>>> origin/yuriy
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
    // audio, possible future adventures
    AudioCodecSettings settings;
    settings.setSampleRate( 44100);
    settings.setChannelCount(2);
    settings.setSampleFormat(EncoderGlobal::Signed16);

    return settings;
}

void QmlWidget::drawAnimated(bool record)
{
    if(curStatus == this->PAUSE)
    {
        //m_recorder->resume();
        curStatus = PLAY;
        return;
    }
    if(record)
    {
        QString fileName = QFileDialog::getSaveFileName(this, tr("Choose file..."), qApp->applicationDirPath(), tr("Videos (*.avi *.mp4)"));
        if(!fileName.size())
            return;

        m_encoder->setVideoCodecSettings(videoCodecSettings());
<<<<<<< HEAD
        m_encoder->setAudioCodecSettings(audioCodecSettings());
        m_encoder->setEncodingMode(Encoder::VideoAudioMode);
       // m_encoder->setEncodingMode(Encoder::VideoMode);
=======
       // m_encoder->setAudioCodecSettings(audioCodecSettings());
       // m_encoder->setEncodingMode(Encoder::VideoAudioMode);
        m_encoder->setEncodingMode(Encoder::VideoMode);
>>>>>>> origin/yuriy
        m_encoder->setVideoCodec(EncoderGlobal::H264);
        m_encoder->setAudioCodec(EncoderGlobal::MP3);
        m_encoder->setOutputPixelFormat(EncoderGlobal::YUV420P);
        m_encoder->setFilePath( fileName );
        m_encoder->setVideoSize(this->size());
        m_encoder->setFixedFrameRate(25);
<<<<<<< HEAD
        fps_timer->start();
        m_encoder->start();
        audioRecorder->record();
=======
        m_encoder->start();
>>>>>>> origin/yuriy
    }
    curStatus = PLAY;
    bRecord = record;
    qDebug() << "Start play";
}

void QmlWidget::stopAnimated()
{
    m_encoder->stop();
<<<<<<< HEAD
    audioRecorder->stop();
    fps_timer->stop();
=======
>>>>>>> origin/yuriy
    curStatus = STOP;
    bRecord = false;
    qDebug() << "Stop play";
}

void QmlWidget::pauseAnimated()
{
    curStatus = PAUSE;
    qDebug() << "Pause play";
    //m_recorder->pause();
}
bool QmlWidget::isRecord() const
{
    return bRecord;
}


void QmlWidget::clearCanvas()
{
    QMetaObject::invokeMethod(canvas, "clear");
    symbolPositionList.clear();
    x = marginLeft;
    y = marginTop;
}

void QmlWidget::drawFigure(int x, int y, int width, int height, QmlWidget::FigureType type, bool fill = true)
{

    QMetaObject::invokeMethod(canvas, "drawFigure",
            Q_ARG(QVariant, QVariant(x)),
            Q_ARG(QVariant, QVariant(y)),
            Q_ARG(QVariant, QVariant(width)),
            Q_ARG(QVariant, QVariant(height)),
            Q_ARG(QVariant, QVariant(type)),
            Q_ARG(QVariant, QVariant(fill)));
}

void QmlWidget::nextRow()
{
    y += lineHeight + pt;
    x = marginLeft;
}

void QmlWidget::crossOutLastSymbol()
{
   // QPoint delPos = symbolPositionList.at(symbolPositionList.length() - 1 - deleteWT);
    //int k = 0;
    //QString str = listWords.right(1 + deleteWT);
    deleteWT++;
    /*int widht = fMetrics->width(str)*1.5;
    int deltaY = deltaY = (widht - x) /(maxWidth - marginLeft);

    if(x - widht <= marginLeft)
    {
        deltaY++;
    }

    tx = maxWidth *deltaY - (widht - x + marginLeft);

    ty -= (lineHeight + pt)*deltaY;

    fillText("/", delPos.x(), delPos.y());
    fillText("\\", delPos.x(), delPos.y());*/
}

void QmlWidget::generateFrames()
{
    //generate 500 frames for test
    for (int i = 0; i < 500; ++i) {
        QImage frame(QSize(1024, 768), QImage::Format_RGB16);
        frame.fill(Qt::yellow);
        QPainter painter(&frame);
        painter.setPen(QPen(Qt::red, 3));
        painter.drawRect(i, i, 30, 30);
        m_encoder->encodeVideoFrame(frame);
        //this is needed to prevent RAM overflow on some computers
        if (i % 20 == 0) {
            while (m_encoder->encodedFrameCount() != i) { qApp->processEvents(); }
        }

        qApp->processEvents();
    }

    m_encoder->stop();

}
<<<<<<< HEAD

void QmlWidget::processBuffer(const QAudioBuffer& buffer)
{
    if(curStatus == PLAY && bRecord)
    {
        m_encoder->encodeAudioData(QByteArray(buffer.constData<char>(), buffer.byteCount()));
      //  qDebug() << "AUDIO";
    }
   // qDebug() << "AUDIO";
}

void QmlWidget::displayErrorMessage()
{
    qDebug() << (audioRecorder->errorString());
}

void QmlWidget::fps_control()
{
    if(curStatus == PLAY && bRecord)
    {
        m_encoder->encodeVideoFrame(this->grabFramebuffer());
        /*if (fps_stabilitron % 25 == 0) {
            while (m_encoder->encodedFrameCount() != fps_stabilitron) { qApp->processEvents(); }
            m_encoder->encodeVideoFrame(this->grabFramebuffer());

        }
            fps_stabilitron++;*/
    }
    fps_timer->start();
}

=======
>>>>>>> origin/yuriy
QColor QmlWidget::getMainFillColor() const
{
    return mainFillColor;
}

void QmlWidget::setMainFillColor(const QColor &value)
{
    mainFillColor = value;
    setFillColor(value);
}

int QmlWidget::getDelay() const
{
    return delay;
}

void QmlWidget::setDelay(int value)
{
    delay = value;
}

int QmlWidget::getCountDeleteWT() const
{
    return deleteWT;
}

QFont QmlWidget::getTextFont() const
{
    return textFont;
}

void QmlWidget::setTextFont(const QFont &value)
{
    textFont = value;
    fMetrics = new QFontMetrics(value);
    pt = value.pointSize();
    QVariant a = QVariant(value.family());
    QVariant b = QVariant(value.pointSize());
    QVariant c = QVariant(" "); //style
    QMetaObject::invokeMethod(canvas, "initFont",
            Q_ARG(QVariant, b),
            Q_ARG(QVariant, a),
            Q_ARG(QVariant, c));
}

void QmlWidget::setFillColor(QColor col)
{
    QString sColor = QString("rgba(%1, %2, %3, %4)").arg(col.red()).arg(col.green()).arg(col.blue()).arg(col.alpha());
    fillColor = col;
    QMetaObject::invokeMethod(canvas, "setColor",Q_ARG(QVariant, QVariant(sColor)));
}

void QmlWidget::setFillGradient(int x, int y, int width, int height, GradientSetting color)
{
    QMetaObject::invokeMethod(canvas, "setGradientStyle",
            Q_ARG(QVariant, QVariant(x)),
            Q_ARG(QVariant, QVariant(y)),
            Q_ARG(QVariant, QVariant(width)),
            Q_ARG(QVariant, QVariant(height)),
            Q_ARG(QVariant, QVariant(color.list)));
}

QmlWidget::StatusDraw QmlWidget::getStatus() const
{
    return curStatus;
}

<<<<<<< HEAD
=======


>>>>>>> origin/yuriy
void QmlWidget::recreate()
{
    this->create();
}

void QmlWidget::fillText( QString str, int x, int y)
{
    QMetaObject::invokeMethod(canvas, "fillText",
            Q_ARG(QVariant, QVariant(str)),
            Q_ARG(QVariant, QVariant(x)),
                              Q_ARG(QVariant, QVariant(y)));
}

void QmlWidget::fillAnimationText(QString str, int x, int y, float time)
{
    /*QTimer timer;
    timer.setInterval(time);*/
    GradientSetting color;
    color.addColorStop(0,0,255,0);
    color.addColorStop(0.85f,0,255,0,255);
    int widthX = fMetrics->width(str) + x + 35;
    int widthT = fMetrics->width(str) + 35;
    int heightT = fMetrics->height() + y + 35;
    float nx = x - widthT;
    while(nx <= widthX)
    {
        setFillGradient(x - widthT, y, 250, heightT, color);
        fillText(str, x, y);
   //     this->update();
        qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
        nx += widthT/time;
       // Sleep(time);
    }
}

void QmlWidget::isLastRow()
{
    if(y  + scroll*indexRow > this->height() )
    {
        scroll = -(lineHeight + pt);
        QMetaObject::invokeMethod(canvas, "isLastRow",
                Q_ARG(QVariant, QVariant(scroll)),
                Q_ARG(QVariant, QVariant((lineHeight + pt)*indexRow)),
                Q_ARG(QVariant, QVariant(y)));
       indexRow++;
    }
}
QPoint QmlWidget::drawWrapText(QString str)
{
    if(deleteWT != 0)
    {
        if(deleteWT >= symbolPositionList.length())
            deleteWT = symbolPositionList.length() - 1;
        QPoint delPos = symbolPositionList.at(symbolPositionList.length() - 1 - deleteWT);
       // QPoint delPos2 = symbolPositionList.at(symbolPositionList.length() - 1);

        if(deleteWT != 1)
        {
            drawFigure(delPos.x(), delPos.y() - fMetrics->height()/4,x, y - fMetrics->height()/4, LINE, 0);
        }
        else{
            delPos = symbolPositionList.at(symbolPositionList.length() - 1);
            QColor temp = fillColor;
            setFillColor(QColor("#ff0000"));
            fillText("/", delPos.x(), delPos.y());
            fillText("\\", delPos.x(), delPos.y());
            setFillColor(temp);
        }
        deleteWT = 0;
    }
    QPoint res(x,y);
    QVariant returnedValue;
    QVariant msg = str;
    int widht = fMetrics->width(str)*1.125 ;//+ fMetrics->leftBearing(str.at(0)) + fMetrics->rightBearing(str.at(0));

    if(widht + x > maxWidth)
    {
        if(isWord)
        {
            fillText("-", x, y);
            listWords += "-";
            listStr.push_back( listWords.length() );
            indexInLsit++;
        }
        nextRow();
    }
    isLastRow();
    //fillAnimationText(str, x, y, 6);
    fillText(str, x, y);
    listWords += str;
    x += widht;
    symbolPositionList.push_back(res);
    tickTimer.start(delay);
    while (tickTimer.isActive()) {
       qApp->processEvents();
    }
    return res;
}

