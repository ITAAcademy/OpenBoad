
#include "openglwidget.h"
//#include <qglfunctions.h>
#include "drawSystem/drawsystem.h"
#include "../TimeLine/listcontroll.h"
//#include "../Prompter/promptercontroll.h"

/*
 *scroll
 *
int k = canvas->property("scroll").toInt() - 10;
canvas->setProperty("scroll", k);
*/

GLuint OGLWidget::loadTexture(QImage img,bool doubleSize){

    int nextIndex = 0;
    int index = 0;
    if (doubleSize)img = twiceImageSizeWithouScaling(img);
    if(img.isNull()) // QCoreApplication::applicationDirPath()+"/star.png"
    {
        //loads correctly
        qWarning() << "ERROR LOADING IMAGE";// + QCoreApplication::applicationDirPath()+"/star.png";
        return -1;
    }

    ////qDebug() << "GL_ERROR_STATUS LOAD_TEXTURE:   "<< glGetError();

    ////qDebug() << "GL_ERROR_STATUS LOAD_TEXTURE:   "<< glGetError();
    // //qDebug() << "image converted to GL format";

    QImage GL_formatted_image = convertToGLFormat(img);


    /* if(GL_formatted_image.isNull())
        qWarning() << "IMAGE IS NULL";
    else
    {
        qWarning() << "IMAGE NOT NULL";
    }*/ //Yura - meni  mishaye
    //generate the texture name
    //glEnable(GL_TEXTURE_2D); // Enable texturing
    GLuint texture;

    glGenTextures(1, &texture); // Obtain an id for the texture

    glBindTexture(GL_TEXTURE_2D, texture); // Set as the current texture
    // glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, pixelBufferIDs[index]);

    //qDebug(" after  glBindTexture(GL_TEXTURE_2D, texture);");
    //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    // glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    //int DATA_SIZE =  GL_formatted_image.width() *  GL_formatted_image.height() * 4 ;

    glTexImage2D(GL_TEXTURE_2D, 0, 4, GL_formatted_image.width(), GL_formatted_image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, GL_formatted_image.bits());
    //   glTexImage2D(GL_TEXTURE_2D, 0, 4, GL_formatted_image.width(), GL_formatted_image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);


    //glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, pixelBufferIDs[nextIndex]);

    // map the buffer object into client's memory
    // Note that glMapBufferARB() causes sync issue.
    // If GPU is working with this buffer, glMapBufferARB() will wait(stall)
    // for GPU to finish its job. To avoid waiting (stall), you can call
    // first glBufferDataARB() with NULL pointer before glMapBufferARB().
    // If you do that, the previous data in PBO will be discarded and
    // glMapBufferARB() returns a new allocated pointer immediately
    // even if GPU is still working with the previous data.
    /*glBufferData(GL_PIXEL_UNPACK_BUFFER_ARB, DATA_SIZE, 0, GL_STREAM_DRAW_ARB);
                      GLubyte* ptr = (GLubyte*)glMapBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, GL_WRITE_ONLY_ARB);

                      if(ptr)
                              {
                                  // update data directly on the mapped buffer
                                  //updatePixels(ptr, DATA_SIZE);
                                  memcpy(ptr,GL_formatted_image.bits(),DATA_SIZE);
                                  glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER_ARB); // release pointer to mapping buffer
                              }
                      glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, 0);*/



    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);

    //qDebug("before int realIndex = index; ");
    /*GLenum error = glGetError();
       if(error != 0)
           sucsessLoadTexture = true;
        else
           sucsessLoadTexture = false;
*/
    //if(glGetError())
    // return -1;

    return texture;
    //bind the texture ID
}

void OGLWidget::deleteTexture(GLuint index)
{
    glDeleteTextures(1, &index);
}

int OGLWidget::loadTextureFromFile(QString path)
{
    QImage img(path);
    if(img.isNull()) // QCoreApplication::applicationDirPath()+"/star.png"
    {
        //loads correctly
        qWarning() << "ERROR LOADING IMAGE  " + path;
        return 0;
    }
    return loadTexture(img);

}

void OGLWidget::drawTexture(int x, int y, int width, int height, GLuint texture, int angle, float scaleX, float scaleY, int z, bool centreScaling){
    //loadTextures();
    // glLoadIdentity();
    glEnable(GL_TEXTURE_2D);
    qglColor(Qt::white);
    glBindTexture(GL_TEXTURE_2D, texture);

    int cx=x+width/2,cy=y+height/2;

    glPushMatrix();

    if (centreScaling)
        glTranslatef(cx, cy, 0);
    glScalef(scaleX, scaleY, 1.f);
    glRotatef(angle, 0, 0, 1);
    if (centreScaling)
        glTranslatef(-cx, -cy, 0);

    glBegin(GL_QUADS);
    //Draw Picture
    glTexCoord2i(0,0); glVertex3i(x, y+height, z);
    glTexCoord2i(0,1); glVertex3i(x,y, z);
    glTexCoord2i(1,1); glVertex3i(x+width,y,z);
    glTexCoord2i(1,0); glVertex3i(x+width, y+height, z);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

}
void OGLWidget::drawTexture(int x, int y, int width, int height, int index, int angle, float scaleX, float scaleY, int z, bool centreScaling)
{
    drawTexture(x, y, width, height, index, angle, scaleX, scaleY, z,centreScaling);
    //// //qDebug() << "void OGLWidget::drawTexture(int x, int y, int width, int height, int index)";
}


QList<DrawElement *> &OGLWidget::getList()
{
    //qDebug () << "get list";
    if(!curentList)
        return list_1;
    else
        return list_2;
}

void OGLWidget::setList(const QList<DrawElement *> &value)
{
    QList<DrawElement*> tempList = value;
    DrawElement *temp = nullptr;

    for (int k = 0; k <tempList.length();k++)
        for (int i = 1; i < tempList.length();i++)
        {
            if (tempList[i-1]->getZ()>tempList[i]->getZ())
            {
                temp=tempList[i];
                tempList[i]=tempList[i-1];
                tempList[i-1]=temp;
            }
        }

    //qDebug() << "SET LIST:";
    for( DrawElement* elm : tempList)
    {
        if(elm != NULL && !timeLine->isBlocked)//&& timeLine->getMaxTrackTime() > 0)
        {
            elm->setDrawWidget(this);   //12345
            // qDebug() << "elm.z:"<<elm->getZ();

        }
        else
            break;
    }
    if(curentList)
    {
        list_1 = tempList;
    }
    else
    {
        list_2 = tempList;
    }
    curentList = !curentList;
}
void OGLWidget::paintBrushInBuffer(GLuint& texture,Brush& currentBrushOfDrawSystem,FBOWrapper& fboWrapper,
                                   QVector<QPoint> coords,QVector<BrushBeginingIndex> brushes,int keyFrame)
{


    if (fboWrapper.errorStatus==-1)qDebug() << "BAD BUFFER";
    // qDebug()<<"binded buffer in paint brush:"<< fboWrapper.frameBuffer;
    // glBindFramebuffer(GL_FRAMEBUFFER , fboWrapper.frameBuffer); // Bind our frame buffer for rendering
    int recordedBrushN = 0;
    glBindFramebuffer(GL_FRAMEBUFFER, fboWrapper.frameBuffer);
    //glDisable(GL_DEPTH_TEST);
    ////qDebug() << "keyFrame:"<<keyFrame;
    //glBindFramebuffer(GL_FRAMEBUFFER, fboWrapper.frameBuffer);
    //bool isBrushUsed = false;
    //TEST
    glBegin(GL_TRIANGLES);

    glVertex2f(-1.0f, -0.5f);    // lower left vertex
    glVertex2f( 1.0f, -0.5f);    // lower right vertex
    glVertex2f( 0.0f,  0.5f);    // upper vertex

    glEnd();

    for (; recordedBrushN < brushes.length(); )
    {
        ////qDebug() << "brushes["<<recordedBrushN<<"].pointIndex"<<brushes[recordedBrushN].pointIndex;
        if (brushes[recordedBrushN].pointIndex==keyFrame){
            //qDebug() << "KEY_FRAME:"<<keyFrame;
            // //qDebug() << "mouse play index:"<<keyFrame;

            currentBrushOfDrawSystem = brushes[recordedBrushN].brush;
            if(currentBrushOfDrawSystem.img.isNull())
                qDebug() << "ISNULL";
                texture = loadTexture(currentBrushOfDrawSystem.color_img, true);
            qDebug() << "recordedBrushN:"<<recordedBrushN;

            // isBrushUsed=true;
            ////qDebug() << "recordedBrushN:"<<recordedBrushN;
            break;
        }
        recordedBrushN++;
    }
    if (shaderSupported)
        useShader(mainShader);

    glBindTexture(GL_TEXTURE_2D,texture);
    // if (isBrushUsed) {
    //   //qDebug() << "recordedBrushN:" << recordedBrushN;
    //  currentBrushOfDrawSystem = brushes[recordedBrushN].brush;
    //}

    ////qDebug() << "recordedBrushN:"<<recordedBrushN;
    //if (recordedBrushN>=brushes.length())recordedBrushN=brushes.length()-1;
    // //qDebug() << "recordedBrushN:"<<recordedBrushN;


    QSize brushTextureSize = getTextureSize();
    int BRUSH_SIZE=currentBrushOfDrawSystem.size + currentBrushOfDrawSystem.size_delta/2 - rand()%(int)(currentBrushOfDrawSystem.size_delta + 1);
    ////qDebug() << "LOADED BRUSH SIZE DRAW:"<<BRUSH_SIZE;
    float scaleX=1,scaleY=1;
    float randScalePtX = 0;
    float randScalePtY = 0;
    if(currentBrushOfDrawSystem.afinn != 0)
    {

        randScalePtX = (int)currentBrushOfDrawSystem.afinn/2 - rand() % (1 + (int)currentBrushOfDrawSystem.afinn);
        randScalePtY = (int)currentBrushOfDrawSystem.afinn/2 - rand() % (1 + (int)currentBrushOfDrawSystem.afinn);
    }
    // qDebug() << "randScalePtX_" << m_manager.getAffine() << "__AFTER" << randScalePtX << " " << randScalePtY;

    float MAX_SCALE = 2;
    //100 - 2 (MAX_SCALE)
    //1 - x
    //x = 2/100=0.02
    if(randScalePtX!=0)
        scaleX=MAX_SCALE/randScalePtX;
    if(randScalePtY!=0)
        scaleY=MAX_SCALE/randScalePtY;
    double koff = 1;
    if(brushTextureSize.height() != 0)
        koff = brushTextureSize.width()/brushTextureSize.height();
    int maxAngle = currentBrushOfDrawSystem.angle_delta;

    int angle = 0;
    if (maxAngle > 0){
        angle=maxAngle/2 - rand() % (maxAngle);
    }
    int maxDispers = (int)currentBrushOfDrawSystem.dispers;
    int i=1;
    if (maxDispers>0 && currentBrushOfDrawSystem.count>0) i=currentBrushOfDrawSystem.count;
    if (shaderSupported)
    {
        int imgUniform = glGetUniformLocation(mainShader->getShaderProgram(),"vUV");
        glUniform2i(imgUniform,0,0);

        int bloorStepUnifrom = glGetUniformLocation(mainShader->getShaderProgram(),"radial_blur");
        glUniform1f(bloorStepUnifrom,currentBrushOfDrawSystem.blur/100);

        int colorUniform = glGetUniformLocation(mainShader->getShaderProgram(),"toColor");
        QColor color = currentBrushOfDrawSystem.color_main;
        float r = (float)color.red()/255;
        float g = (float)color.green()/255;
        float b = (float)color.blue()/255;
        float a = (float)currentBrushOfDrawSystem.opacity/100;
        // //qDebug() << r << g << b << a;
        glUniform4f(colorUniform,r,g,b,a);
    }
    for (;i>0;i--)
    {
        int dispersX = 0;
        int dispersY = 0;
        if ((int)currentBrushOfDrawSystem.dispers>0){
            dispersX = maxDispers/2 - rand() % (maxDispers);
            dispersY = maxDispers/2 - rand() % (maxDispers);
        }
        int xPos = 0;
        int yPos = 0;
        // //qDebug()<<"mouseRecorder.getBrushBeginings().length():"<<brushes.length();
        // //qDebug() << "xPos:"<<xPos;
        // //qDebug() << "yPos:"<<yPos;
        xPos=coords[keyFrame].x();
        yPos=coords[keyFrame].y();
        // //qDebug() << "AFTER123";
        // //qDebug() << "x:"<<xPos;
        // //qDebug() << "y:"<<yPos;
        ////qDebug() << "keyFrame:"<<keyFrame;
        // if (mousePlayIndex >= coords.length()-1)
        //  {
        // isMousePlay=false;
        //     stopShowLastDrawing();
        //  mousePlayIndex=0;
        //  }
        //else
        // mousePlayIndex++;


        drawTexture(xPos-BRUSH_SIZE/2 + dispersX ,yPos-BRUSH_SIZE/koff/2 + dispersY,BRUSH_SIZE,BRUSH_SIZE/koff,
                    texture,angle,scaleX,scaleY, 1000 + i);
    }
    if (shaderSupported)
        useShader(0);

    // glEnable(GL_DEPTH_TEST);

    // glBindTexture(GL_TEXTURE_2D,0);
    //glBindFramebuffer(GL_FRAMEBUFFER , 0); // Unbind our texture
}


bool OGLWidget::isShowLastDrawing(){
    return showingLastDrawing;
}
void OGLWidget::setShowLastDrawing(bool val){
    showingLastDrawing=val;
}
void OGLWidget::stopShowLastDrawing(){
    showingLastDrawing=false;
    currentLastDrawingPointIterator=0;
    emit stopShowLastDrawingSignal();
}

bool OGLWidget::getShowLastDrawing(){
    return showingLastDrawing;
}

void OGLWidget::slotBlockEdited()
{
    //qDebug() << "1234566666666666666";
    // ms_for_debug.show();
    /*QPoint t = timeLine->getSelectedBlockPoint();
editingRectangle.rect = timeLine->getDrawRect(t.x(), t.y());
*/
    selElm = NULL;
}


RectangleEditor* OGLWidget::getEditingRectangle()
{
    return &editingRectangle;
}

void OGLWidget::setEditingRectangle(const RectangleEditor &value)
{
    editingRectangle = value;
}
void OGLWidget::hideEvent(QHideEvent *)
{
    init=false;
    //qDebug() << "HIDE EVENT";
}
void OGLWidget::showEvent(QShowEvent * event)
{
    //if(!isNeedShowTimeLine())

}

void OGLWidget::zoomGrid(int val)
{
    int h = windowGrid.getCellHeight();
    int w = windowGrid.getCellWidth();
    if (h+val>0 && h+val<windowGrid.getHeight())
    {
        h+=val;
        if (w+val>0 && w+val<windowGrid.getWidth())
            w+=val;
    }
    windowGrid.setCellWidth(w);
    windowGrid.setCellHeight(h);
    qDebug() << "h"<<h;
    updateGrid();
    enableGrid();
    //QTimer::singleShot(100, this, SLOT(disableGrid()));


}
void OGLWidget::disableGrid(){
    gridEnabled=false;
}
void OGLWidget::enableGrid(){
    gridEnabled=true;
}

void OGLWidget::processMouse()
{
    if(isMousePress) {
        int x1 = editingRectangle.rect.x();
        int y1 = editingRectangle.rect.y();
        int x2 = editingRectangle.rect.x()+editingRectangle.rect.width();
        int y2 = editingRectangle.rect.y()+editingRectangle.rect.height();

        int leftTopCornerX1=x1-editingRectangle.cornerSize/2;
        int leftTopCornerY1=y1-editingRectangle.cornerSize/2;
        int leftTopCornerX2=x1 + editingRectangle.cornerSize/2;
        int leftTopCornerY2=y1 + editingRectangle.cornerSize/2;

        int rightTopCornerX1=x2-editingRectangle.cornerSize/2;
        int rightTopCornerY1=y1-editingRectangle.cornerSize/2;
        int rightTopCornerX2=x2 + editingRectangle.cornerSize/2;
        int rightTopCornerY2=y1 + editingRectangle.cornerSize/2;

        int leftBottomCornerX1=x1-editingRectangle.cornerSize/2;
        int leftBottomCornerY1=y2-editingRectangle.cornerSize/2;
        int leftBottomCornerX2=x1 + editingRectangle.cornerSize/2;
        int leftBottomCornerY2=y2 + editingRectangle.cornerSize/2;

        int rightBottomCornerX1=x2-editingRectangle.cornerSize/2;
        int rightBottomCornerY1=y2-editingRectangle.cornerSize/2;
        int rightBottomCornerX2=x2 + editingRectangle.cornerSize/2;
        int rightBottomCornerY2=y2 + editingRectangle.cornerSize/2;


        QVector2D vec(qCos((editingRectangle.angle * M_PI)/180), qSin((editingRectangle.angle * M_PI)/180));
        vec.normalize();
        vec *= -( qMin(editingRectangle.rect.width(), editingRectangle.rect.height()))/4;
        int CX = vec.x() + x1 + (x2 - x1)/2;
        int CY = vec.y() + y1 + (y2 - y1)/2;

    /*    qDebug() << CX << " " << mousePos.x();
        qDebug() << CY << " " << mousePos.y();*/

        if(CX + editingRectangle.circleSize > mousePos.x() && CX - editingRectangle.circleSize < mousePos.x())
            if(CY + editingRectangle.circleSize > mousePos.y() && CY - editingRectangle.circleSize < mousePos.y())
        {
            editingRectangle.editingRectangleMode = EDIT_RECTANGLE_ROTATION;
           // return;
        }



        //editingRectangle.setX(0);
        // editingRectangle.setY(0);



        if (editingRectangle.editingRectangleMode==EDIT_RECTANGLE_UNBINDED &&
                editingRectangle.isEditingRectangleVisible && !forceEditBoxDisable && !able_drawing)
        {

            editingRectangle.currentCornerResize=RESIZE_CORNER_NONE;
            if((mousePos.x() >= leftTopCornerX1 && mousePos.x() <= leftTopCornerX2)
                    && (mousePos.y() >= leftTopCornerY1 && mousePos.y() <= leftTopCornerY2))
                editingRectangle.currentCornerResize=ResizeCorner::RESIZE_CORNER_TOP_LEFT;
            if ((mousePos.x() >= rightTopCornerX1 && mousePos.x() <= rightTopCornerX2)
                    && (mousePos.y() >= rightTopCornerY1 && mousePos.y() <= rightTopCornerY2))
                editingRectangle.currentCornerResize=ResizeCorner::RESIZE_CORNER_TOP_RIGHT;
            if ((mousePos.x() >= leftBottomCornerX1 && mousePos.x() <= leftBottomCornerX2)
                    && (mousePos.y() >= leftBottomCornerY1 && mousePos.y() <= leftBottomCornerY2))
                editingRectangle.currentCornerResize=ResizeCorner::RESIZE_CORNER_BOTTOM_LEFT;
            if ((mousePos.x() >= rightBottomCornerX1 && mousePos.x() <= rightBottomCornerX2)
                    && (mousePos.y() >= rightBottomCornerY1 && mousePos.y() <= rightBottomCornerY2))
                 editingRectangle.currentCornerResize=ResizeCorner::RESIZE_CORNER_BOTTOM_RIGHT;

            if (editingRectangle.currentCornerResize!=RESIZE_CORNER_NONE)
            {
                editingRectangle.editingRectangleMode=EDIT_RECTANGLE_RESIZE;
            }
            else if ((mousePos.x() >= x1) && (mousePos.x() <= x2) &&
                     (mousePos.y() >= y1) && (mousePos.y() <= y2))
            {
                editingRectangle.editingRectangleMode=EDIT_RECTANGLE_MOVE;
            }


        }
        switch(editingRectangle.editingRectangleMode){

        case EDIT_RECTANGLE_MOVE:
        {
            gridEnabled=true;
            //m_manager.setAbleToDraw(false);
            // // //qDebug()<<"EDIT_RECTANGLE_MOVE width"<<editingRectangle.rect.width();
            //if (isPainting)
            /*if (pressedShift){
                QPoint closestPoint = windowGrid.closeToLCP(QPoint(editingRectangle.rect.x(),editingRectangle.rect.y()));
                x = closestPoint.x();
                y = closestPoint.y();
                editingRectangle.rect.setX(x);
                editingRectangle.rect.setY(y);
            }*/

            if(pressedShift)
            {
              //  QPoint closestPoint = windowGrid.closeToLCP(mousePos);
                 QPoint closestPoint = windowGrid.closeToLCP(QPoint(mousePos.x() - mousePressPos.x(),mousePos.y() - mousePressPos.y()));
                editingRectangle.rect.moveTo(closestPoint);
            }
            else
                editingRectangle.rect.moveTo(mousePos.x() - mousePressPos.x(), //-editingRectangle.rect.width()/2
                                             mousePos.y() - mousePressPos.y() ); //-editingRectangle.rect.height()/

            break;
        }
        case EDIT_RECTANGLE_RESIZE:
        {
            gridEnabled=true;
           // m_manager.setAbleToDraw(false);
            int x = mousePos.x();
            int y = mousePos.y();
            if (pressedShift){
                QPoint closestPoint = windowGrid.closeToLCP(mousePos);
                x = closestPoint.x();
                y = closestPoint.y();
            }
            switch(editingRectangle.currentCornerResize)
            {
            //NEED ONLY TWO CASE - TOP_LEFT AND DEFAULT
            case RESIZE_CORNER_TOP_LEFT:
                qDebug() << "POP       1";
                editingRectangle.rect.setX(x);
                editingRectangle.rect.setY(y);
                break;
            case RESIZE_CORNER_TOP_RIGHT:
                qDebug() << "POP       2";
                editingRectangle.rect.setWidth(x-editingRectangle.rect.x());
                editingRectangle.rect.setY(y);
               // editingRectangle.rect.setHeight(y-editingRectangle.rect.y());
                break;
            case RESIZE_CORNER_BOTTOM_LEFT:
                qDebug() << "POP       3";
                editingRectangle.rect.setX(x);
                editingRectangle.rect.setHeight(y-editingRectangle.rect.y());
                break;
            case RESIZE_CORNER_BOTTOM_RIGHT:
                qDebug() << "POP       4";
                editingRectangle.rect.setWidth(x-editingRectangle.rect.x());
                editingRectangle.rect.setHeight(y-editingRectangle.rect.y());
                break;
            }

            // //qDebug()<<"EDIT_RECTANGLE_RESIZE";
            //  if (isPainting)


            break;
        }
        }
        testRectangle();
        //qDebug()<<"curStatus:"<<curStatus;
        if ((curStatus != PLAY) &&  able_drawing  && (prevMousePos != mousePos) )
        {
            paintBrushInBuffer(mouseFBO);
            qDebug() << "paintBrushInBuffer";
        }
    }

    if (showingLastDrawing && curStatus!=PLAY )
    {
        if (drawBrushElm->getBrushes().length()<=0)
        {
            stopShowLastDrawing();
            return;
        }
        int recordedBrushN = 0;
        for (; recordedBrushN < drawBrushElm->getBrushes().length(); )
        {
            ////qDebug() << "brushes["<<recordedBrushN<<"].pointIndex"<<brushes[recordedBrushN].pointIndex;
            if (drawBrushElm->getBrushes()[recordedBrushN].pointIndex==currentLastDrawingPointIterator){
                // //qDebug() << "KEY_FRAME:"<<keyFrame;
                // //qDebug() << "mouse play index:"<<keyFrame;

                currentBrushOfLastDrawing =drawBrushElm->getBrushes()[recordedBrushN].brush;
                    brushTexture = loadTexture(drawBrushElm->getBrushes()[recordedBrushN].brush.color_img, true);
                break;
            }
            recordedBrushN++;
        }
        glBindFramebuffer(GL_FRAMEBUFFER , mouseFBO.frameBuffer);
        paintBrushInBuffer(brushTextureCurrentPlayed,currentBrushOfLastDrawing,mouseFBO,drawBrushElm->getCoords(),drawBrushElm->getBrushes(),currentLastDrawingPointIterator);
        glBindFramebuffer(GL_FRAMEBUFFER ,0);
        currentLastDrawingPointIterator++;
        if (currentLastDrawingPointIterator>=drawBrushElm->getCoords().length())
        {
            stopShowLastDrawing();
        }
        // currentLastDrawingPointIterator++;

    }

}

ShaderProgramWrapper *OGLWidget::getMainShader()
{
    return mainShader;
}
ShaderProgramWrapper *OGLWidget::getTestShader()
{
    return test;
}

void OGLWidget::initPBO()
{
    glGenBuffers(2, pixelBufferIDs);
}

void OGLWidget::initShaderPrograms()
{
    mainShader = new ShaderProgramWrapper(this);
    test = new ShaderProgramWrapper(this);
    //shaderWindow.setParent(this);
    connect(&shaderWindow, SIGNAL(test()), this, SLOT(testInit()));

    if (mainShader->initShader(MAIN_FRAGMENT_SHADER_PATH,MAIN_VERTEX_SHADER_PATH)==0)
		shaderSupported=true;
    else
        QMessageBox::information(
                    this,tr("OpenBoard"),
                    tr("Shaders aren't supported. Some features won't work.\n Try update drivers of videocard") );
    // mainShader->initShader(MAIN_FRAGMENT_SHADER_PATH,MAIN_VERTEX_SHADER_PATH);


    ShaderProgramWrapper *alphaShader = new ShaderProgramWrapper(this);
    alphaShader->initShader(ALPHA_FRAGMENT_SHADER_PATH,ALPHA_VERTEX_SHADER_PATH);
    shaderPrograms.push_back(alphaShader);

    ShaderProgramWrapper *spinShader = new ShaderProgramWrapper(this);
    spinShader->initShader(SPIN_FRAGMENT_SHADER_PATH,SPIN_VERTEX_SHADER_PATH);
    shaderPrograms.push_back(spinShader);

    ShaderProgramWrapper *pixelizationShader = new ShaderProgramWrapper(this);
    pixelizationShader->initShader(PIXELIZATION_FRAGMENT_SHADER_PATH,PIXELIZATION_VERTEX_SHADER_PATH);
    shaderPrograms.push_back(pixelizationShader);

    ShaderProgramWrapper *circlesShader = new ShaderProgramWrapper(this);
    circlesShader->initShader(CIRCLES_FRAGMENT_SHADER_PATH,CIRCLES_VERTEX_SHADER_PATH);
    shaderPrograms.push_back(circlesShader);

    ShaderProgramWrapper *turnThePageShader = new ShaderProgramWrapper(this);
    turnThePageShader->initShader(TURNTHEPAGE_FRAGMENT_SHADER_PATH,TURNTHEPAGE_VERTEX_SHADER_PATH);
    shaderPrograms.push_back(turnThePageShader);

    ShaderProgramWrapper *randomSquaresShader = new ShaderProgramWrapper(this);
    randomSquaresShader->initShader(RANDOMSQUARES_FRAGMENT_SHADER_PATH,RANDOMSQUARES_VERTEX_SHADER_PATH);
    shaderPrograms.push_back(randomSquaresShader);

    ShaderProgramWrapper *tresholdShader = new ShaderProgramWrapper(this);
    tresholdShader->initShader(TRESHOLD_FRAGMENT_SHADER_PATH,TRESHOLD_VERTEX_SHADER_PATH);
    shaderPrograms.push_back(tresholdShader);

    ShaderProgramWrapper *slideShader = new ShaderProgramWrapper(this);
    slideShader->initShader(SLIDE_FRAGMENT_SHADER_PATH,SLIDE_VERTEX_SHADER_PATH);
    shaderPrograms.push_back(slideShader);

    ShaderProgramWrapper *crossShader = new ShaderProgramWrapper(this);
    crossShader->initShader(CROSS_FRAGMENT_SHADER_PATH,CROSS_VERTEX_SHADER_PATH);
    shaderPrograms.push_back(crossShader);



    /* glUseProgram(crossShader->getShaderProgram());
     crossShader->setUniformResolution(wax,way);
      glUseProgram(0);*/

    //shaderSupported= false;
}

void OGLWidget::testInit()
{
    if(test->initShader(shaderWindow.getFragmentValue(), shaderWindow.getVertextValue(), false) == -1)
    {
        shaderWindow.setStatus(false);
        shaderWindow.setLog(test->getInfo());
    }
    else
    {
        shaderWindow.setStatus(true);
        shaderWindow.setLog("OK");
    }
}

QVector<ShaderProgramWrapper*> OGLWidget::getShaderPrograms()
{
    return shaderPrograms;
}

bool OGLWidget::isShaderSupported()
{
    return shaderSupported;
}

void OGLWidget::setShaderSupported(bool value)
{
    shaderSupported=value;
}
bool OGLWidget::isHiddenByButton() const
{
    return hiddenByButton;
}

void OGLWidget::setHiddenByButton(bool value)
{
    hiddenByButton = value;
}


void OGLWidget::setFrameRate(int fps)
{
    frameRate=fps;
}

OGLWidget::OGLWidget(QWidget *parent, QGLFormat format) :
    QGLWidget(format,parent)
{

   // glFormat.setProfile( QGLFormat::CoreProfile ); // Requires >=Qt-4.8.0
    //glFormat.setSampleBuffers( true );
    qDebug() <<  "OGL WIDGET COnstructor start";
    setFixedSize(GLWIDGET_SIZE);
    wax=width(); way=height(); // начальный размер окна
    init = false;
    timeLine = new ListControll(this);
    timeLine->setP_drawWidget(this);

    effectManager = new EffectsManager(this);
    prompterManager = new PrompterManager(this);
    //showPrompterManager();
    connect(timeLine,SIGNAL(stopSignal()),this,SIGNAL(stopSignal()));
    connect(timeLine,SIGNAL(playSignal()),this,SIGNAL(startSignal()));
    connect(timeLine,SIGNAL(pauseSignal()),this,SIGNAL(pauseSignal()));

    connect(timeLine,SIGNAL(scalePointerXChanged(int)),prompterManager,SLOT(determineAndSelectCurrentLineByTime(int)));

    connect(this,SIGNAL(windowUpdating(int)),timeLine,SLOT(addMsToTimerValue(int)));

    connect(getTimeLine(),SIGNAL(imageLoadedPictureSizeSignal(QSize)),this,SLOT(imageLoadedPictureSizeSlot(QSize)));


    connect(timeLine,SIGNAL(blockEditedSignal()),this,SLOT(slotBlockEdited()));

    connect(timeLine,SIGNAL(focusFoundSignal()),this,SLOT(hideBrushManager()));
    connect(timeLine,SIGNAL(focusFoundSignal()),this,SLOT(hideEffectsManager()));


    selElm = NULL; //because it undefined

    //qRegisterMetaType<DrawData>("DrawData");
    // engine()->rootContext()->setContextProperty(QLatin1String("forma"), this);
    m_encoder = new AV_REncoder(this);
    editingRectangle.rect.setWidth(100);
    editingRectangle.rect.setHeight(100);
    editingRectangle.rect.setX(50);
    editingRectangle.rect.setY(50);
    editingRectangle.cornerSize=15;
    bRecord = false;
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowTitleHint);
    qDebug() <<  "OGL WIDGET MID";
    //this->setResizeMode(QQuickWidget::SizeRootObjectToView ); //TODO
    /*
     * init
    */

    //
    //startMarker.value=getMainFillColor();



    maxWidth; //размер поле, где выводится текст




    delay = 1000/3;
    //listStr[0] = 0;

    curStatus = STOP;
    //tickTimer.setSingleShot(false);
    // tickTimer.setInterval(1000/25);
    //mouseTimer.setInterval(drawBrushElm->SPEED_OF_RECORDING_MS);
    // mouseTimer.setSingleShot(false);
    realDelay = 0;

    // maxWidth = width() - marginLeft;

    //stringList.reserve(600000);




    //OPENGL
    //setFormat(QGLFormat(QGL::SampleBuffers | QGL::DoubleBuffer | QGL::DepthBuffer)); // Двойная буферизация

    /*  QGLFormat format;
     format.setProfile(QGLFormat::CompatibilityProfile);
     format.setVersion(3,0);
     format.setDoubleBuffer(true);
     setFormat(format); */

    //glDepthFunc(GL_LEQUAL); // Буфер глубины
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateWindow()));
    timer->start(5);
    //connect(&mouseTimer, SIGNAL(timeout()), this, SLOT(storeMousePos()));
    //mouseTimer.start();



    //for loading current text in to fist init block
    /*DrawTextElm drawTTElements(this);
   QList <Unit*>mUnitList;

   Unit unit;
   mUnitList.append(&unit);
   drawTTElements.setUnitList(mUnitList);
   drawTTElements.setUnParsestring("", "");
   drawTTElements.save("curent");
 // timeLine->loadCurrentTextInTheFirstBlockWhenInit();
*/
    getTimeLine()->setIsProjectChanged(false);
    oglFuncs=this;
    windowGrid=Grid(GRID_CELL_SIZE,wax,way);
    qDebug() <<  "OGL WIDGET COnstructor end";
    connect(effectManager,SIGNAL(hideSignal()),this,SLOT(applyEffectsToCurrentBlock()));
    connect(effectManager,SIGNAL(showSignal()),this,SLOT(loadEffectFromCurrentBlockToEffectManager()));
    //qDebug() << "@@@@@@@@@@@@@@@@@@@@@@@@@@  glGetString(GL_VERSION)  " << glGetString(GL_VERSION) ;
}

void OGLWidget::bindBuffer(GLuint buffer){

    glBindFramebuffer(GL_FRAMEBUFFER,buffer);
}

QOpenGLFunctions_3_0* OGLWidget::getOglFuncs(){
    return oglFuncs;
}

OGLWidget::~OGLWidget()
{
    qDebug() <<" delete mainShader; begin";
    //   if(mainShader != nullptr)        // CRASH !!!!!!!!!!!!!!!!! on delete QString
    //  delete mainShader;
    qDebug() <<" delete mainShader; end";
    this->stopAnimated();
    qApp->processEvents();
    if(timeLine != NULL)
        delete timeLine;
    if (effectManager!=NULL)
        delete effectManager;
    if (prompterManager!=NULL)
        delete prompterManager;
    if(m_encoder != NULL);
    delete m_encoder;

    if (drawBrushElm !=NULL)
        delete drawBrushElm;

    for (int i =0;i<shaderPrograms.length();i++)
        delete shaderPrograms[i];
}



void OGLWidget::resizeGL(int nWidth, int nHeight)
{
    // qDebug()<<"resize";
    if(isInit()){
        //  makeCurrent();
        //glMatrixMode(GL_PROJECTION);
        // glLoadIdentity();
        //glViewport(0, 0, (GLint)nWidth, (GLint)nHeight);
    }
    wax=nWidth;
    way=nHeight;
    // qDebug() << "end resize";
}
void OGLWidget::paintBufferOnScreen( FBOWrapper buffer,int x, int y, int width, int height, int z, int angle, double scaleX, double scaleY){

    if (buffer.errorStatus==-1)qDebug() << "BAD BUFFER";

    drawTexture(x, y, width, height, buffer.bindedTexture, angle, scaleX, scaleY, z);
    /*glColor3f(1.0,1.0,1.0);
    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D,buffer.bindedTexture);
    // qglColor(Qt::green);//TO SEE TEXTURE
    glBegin(GL_QUADS);
    //Draw Picture
    glTexCoord2i(0,0); glVertex3i(x, y+height, z);
    glTexCoord2i(0,1); glVertex3i(x,y, z);
    glTexCoord2i(1,1); glVertex3i(x+width,y,z);
    glTexCoord2i(1,0); glVertex3i(x+width, y+height, z);

    glEnd();
    // glDeleteTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);*/
}


void OGLWidget::paintBrushInBuffer(FBOWrapper fboWrapper) {
    if (able_drawing)
    {
           qDebug() << "paint brush in buffer";
        GLint currentBuff;
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &currentBuff);

        glBindFramebuffer(GL_FRAMEBUFFER , fboWrapper.frameBuffer); // Bind our frame buffer for rendering
        //glPushAttrib(GL_VIEWPORT_BIT | GL_ENABLE_BIT); // Push our glEnable and glViewport states
        //glViewport(0, 0, window_width, window_height); // Set the size of the frame buffer view port

        //glClearColor (0.0f, 0.0f, 1.0f, 1.0f); // Set the clear colour
        //glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the depth and colour buffers
        //glLoadIdentity();  // Reset the modelview matrix

        //fillText("DEBUG TEXT TO BUFER",QColor(Qt::white),40,40);
        // int PointSize = 10;

        // glPointSize(PointSize);
        //glLineWidth(PointSize);
        // glEnable(GL_TEXTURE_2D);
        //// //qDebug() << "before index";
        //// //qDebug() << "paint brush in buffer";
        GLuint texture = brushTexture;
        //// //qDebug()<<"texture:"<<texture;
        // if (!ismouseWasPressedBeforeDrag)

        /* glBegin (GL_POINTS);
        glColor3f (1.0, 0.4, 0.4);
        glVertex3f (mousePos.x(), mousePos.y(),0.0);
        // //qDebug() << mousePos.x();
        glEnd();*/
        //qglColor(m_manager.getColor());
        if (shaderSupported)
            useShader(mainShader);

        glBindTexture(GL_TEXTURE_2D,texture);
        QSize brushTextureSize = getTextureSize();
        int BRUSH_SIZE=m_manager.getSize() + m_manager.getSizeDelta()/2 - rand()%(int)(m_manager.getSizeDelta() + 1);
        float scaleX=1,scaleY=1;
        float randScalePtX = 0;
        float randScalePtY = 0;
        if(m_manager.getAffine() != 0)
        {
            randScalePtX = m_manager.getAffine()/2 - rand() % (1 + (m_manager.getAffine()));
            randScalePtY = m_manager.getAffine()/2 - rand() % (1 + (m_manager.getAffine()));
        }
        // qDebug() << "randScalePtX_" << m_manager.getAffine() << "__AFTER" << randScalePtX << " " << randScalePtY;

        float MAX_SCALE = 2;
        //100 - 2 (MAX_SCALE)
        //1 - x
        //x = 2/100=0.02
        if(randScalePtX!=0)
            scaleX=MAX_SCALE/randScalePtX;
        if(randScalePtY!=0)
            scaleY=MAX_SCALE/randScalePtY;
        // qDebug() << "scale x:"<<scaleX;
        // qDebug () << "scale y:"<<scaleY;
        // // //qDebug() <<"scaleX:"<<scaleX;
        // // //qDebug() <<"scaleY:"<<scaleY;
        //// //qDebug() << "brushSize.width():"<<brushTextureSize.width();
        //// //qDebug() << "brushSize.height():"<<brushTextureSize.height();

        double koff = 1;
        if (brushTextureSize.height()>0)koff=brushTextureSize.width()/brushTextureSize.height();
        int maxAngle = m_manager.getAngleDelta();

        int angle = 0;
        if (maxAngle > 0){
            angle=maxAngle/2 - rand() % (maxAngle);
        }
        int maxDispers = (int)m_manager.getDisepers();
        int i=1;
        if (maxDispers>0 && m_manager.getCount()>0) i=m_manager.getCount();

        if (shaderSupported)
        {
            int imgUniform = glGetUniformLocation(mainShader->getShaderProgram(),"vUV");
            glUniform2i(imgUniform,0,0);


            int bloorStepUnifrom = glGetUniformLocation(mainShader->getShaderProgram(),"radial_blur");
            glUniform1f(bloorStepUnifrom,m_manager.getCreatedBrush().blur/100);




            int colorUniform = glGetUniformLocation(mainShader->getShaderProgram(),"toColor");
            QColor color = m_manager.getColor();
            float r = (float)color.red()/255;
            float g = (float)color.green()/255;
            float b = (float)color.blue()/255;
            float a = (float)m_manager.getOpacity()/100;
            // //qDebug() << r << g << b << a;
            glUniform4f(colorUniform,r,g,b,a);

        }
        for (;i>0;i--)
        {
            int dispersX = 0;
            int dispersY = 0;
            if ((int)m_manager.getDisepers()>0){
                dispersX = maxDispers/2 - rand() % (maxDispers);
                dispersY = maxDispers/2 - rand() % (maxDispers);
            }
            int xPos = 0;
            int yPos = 0;
            xPos = mousePos.x();
            yPos = mousePos.y();

            ////qDebug() <<"mousePos.x() = " << mousePos.x() << " mousePos.y() = " << mousePos.y();

            drawTexture(xPos-BRUSH_SIZE/2 + dispersX ,yPos-BRUSH_SIZE/koff/2 + dispersY,BRUSH_SIZE,BRUSH_SIZE/koff,
                        texture,angle,scaleX,scaleY, 1000);
        }
        if (shaderSupported)
            useShader(0);
        /*
    else{
      glEnable(GL_TEXTURE_2D);
       glBindTexture(GL_TEXTURE_2D,texture);
      QSize brushTextureSize = getTextureSize();
      int BRUSH_SIZE=30;
      // //qDebug() << "brushSize.width():"<<brushTextureSize.width();
      // //qDebug() << "brushSize.height():"<<brushTextureSize.height();
      double koff = brushTextureSize.width()/brushTextureSize.height();

      QPainterPath path;
          path.moveTo(prevMousePos);
          path.lineTo(mousePos);
          QPointF from = prevMousePos;
      for (int i=0;i<100;i+=20 )
      {
        QPointF to = path.pointAtPercent(i);
      drawTexture(from.x()-BRUSH_SIZE/2 ,from.y()-BRUSH_SIZE/koff/2,BRUSH_SIZE,BRUSH_SIZE/koff,texture);
      drawTexture(to.x()-BRUSH_SIZE/2 ,to.y()-BRUSH_SIZE/koff/2,BRUSH_SIZE,BRUSH_SIZE/koff,texture);
      from=to;
      }
      glDisable(GL_TEXTURE_2D);
       prevMousePos.setX(mousePos.x());
       prevMousePos.setY(mousePos.y());
    }*/
        // glDisable(GL_TEXTURE_2D);



        glBindTexture(GL_TEXTURE_2D,0);
        glBindFramebuffer(GL_FRAMEBUFFER , currentBuff); // Unbind our texture
        //FRAMEBUFFER PART


        //saveFrameBufferToTexture();





        //glPopAttrib(); // Restore our glEnable and glViewport states
    }


}


QImage OGLWidget::twiceImageSizeWithouScaling(QImage img)
{
    QSize originSize = img.size();
    QImage result(originSize.width()*2,originSize.height()*2,img.format());
    QPoint destPos(originSize.width()/2, originSize.height()/2);
    result.fill(QColor(0,0,0,0));
    QPainter painter(&result);
    painter.drawImage(destPos, img);
    painter.end();
    return result;
}

void OGLWidget::showLCP()
{
    glPointSize(10);
    glBegin(GL_POINTS);
    for (QPoint lcp : windowGrid.getLCP()){
        glVertex2f(lcp.x(),lcp.y());
    }
    glEnd();

}



int OGLWidget::initDepthTexture(GLuint &fbo_depth, int width, int height) {
    /*
     * LAST
     */
    /*glGenRenderbuffers(1, &fbo_depth); // Generate one render buffer and store the ID in fbo_depth
    glBindRenderbuffer(GL_RENDERBUFFER , fbo_depth); // Bind the fbo_depth render buffer
    GLenum error = glGetError();

    qDebug() << "GL_ERROR_STATUS glBindRenderbuffer:"<<error;
    //glRenderbufferStorage(GL_RENDERBUFFER , GL_DEPTH_COMPONENT, wax, way); // Set the render buffer storage to be a depth component, with a width and height of the window

    error = glGetError();
    qDebug() << "GL_ERROR_STATUS glRenderbufferStorage:"<<error;

    glFramebufferRenderbuffer(GL_FRAMEBUFFER , GL_DEPTH_ATTACHMENT , GL_RENDERBUFFER ,fbo_depth); // Set the render buffer of this buffer to the depth buffer
     //error = glGetError();
    ////qDebug() << "GL_ERROR_STATUS glFramebufferRenderbuffer:"<<error;
    glBindRenderbuffer(GL_RENDERBUFFER , 0); // Unbind the render buffer
     error = glGetError();*/
    //qDebug() << "GL_ERROR_STATUS glBindRenderbuffer:"<<error;

    /*
     * NEW
     *//*
      glGenTextures(1, &fbo_depth);
      glBindTexture(GL_TEXTURE_2D, fbo_depth);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
      //NULL means reserve texture memory, but texels are undefined
      //glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);*/

    glGenRenderbuffers(1, &fbo_depth);
    glBindRenderbuffer(GL_RENDERBUFFER, fbo_depth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, fbo_depth);

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
    return glGetError();
}

int OGLWidget::initTexture(GLuint &texture,int width,int height) {
    makeCurrent();
    glEnable(GL_TEXTURE_2D);
    //glDeleteTextures(1,&fbo_texture);
    /* glGenTextures(1, &texture); // Generate one texture

    glBindTexture(GL_TEXTURE_2D, texture); // Bind the texture fbo_texture

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL); // Create a standard texture with the width and height of our window

    // Setup the basic texture parameters
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
*/
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //NULL means reserve texture memory, but texels are undefined
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);
    // Unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
    return glGetError();
}


FBOWrapper OGLWidget::initFboWrapper(int width, int height, bool visibleOnly, bool forceLoad) {


    makeCurrent();
    FBOWrapper fboWrapper;

    //qDebug() << "1111";

    if(!isInit() && !forceLoad)
    {
        fboWrapper.errorStatus = -1;
        return fboWrapper;
    }


    glGetError();
    //qDebug () << "                                                                  INIT FBO";
    // qDebug() << "2";


    fboWrapper.errorStatus=0;
    fboWrapper.tWidth=width;
    fboWrapper.tHeight=height;
    GLuint fbo=0;
    GLuint depth_buffer=0;
    GLuint fbo_texture=0;
    // qDebug() << "fbo:"<<fbo;
    // qDebug()<< "depth_buffer:"<<depth_buffer;
    // qDebug() << "fbo_texture:"<<fbo_texture;
    GLenum error = glGetError();

    //qDebug() << "GL_ERROR_STATUS initFrameBuffer:"<<error;
    //  glEnable(GL_DEPTH_TEST);

    // qDebug() << "GL_ERROR_STATUS initFrameBuffer:"<<error;
    if(error != 0)
    {
        fboWrapper.errorStatus = error;
        return fboWrapper;
    }
    //qDebug() << 1;
    if(visibleOnly)
        if(!isVisible() || !isInit())
        {
            fboWrapper.errorStatus = -1;
            return fboWrapper;
        }

    glGenFramebuffers(1, &fbo); // Generate one frame buffer and store the ID in fbo

    //qDebug() << 2;

    //qDebug() << 3;
    error = glGetError();
    if(error != NULL)
    {
        fboWrapper.errorStatus = error;
        return fboWrapper;
    }

    error = initTexture(fbo_texture, width, height); // Initialize our frame buffer texture
    if(error != NULL)
    {
        fboWrapper.errorStatus = error;
        qDebug() << "GL_ERROR_STATUS0_COLOR_TEXTURE:"<<error;
        return fboWrapper;
    }
    glBindFramebuffer(GL_FRAMEBUFFER , fbo); // Bind our frame buffer

    initDepthTexture(depth_buffer, width, height); // Initialize our frame buffer depth buffer
    if(error != NULL)
    {
        fboWrapper.errorStatus = error;
        qDebug() << "GL_ERROR_STATUS0_DEPTH:"<<error;
        return fboWrapper;
    }
    //qDebug () << "texture inited id:"<<fbo_texture;

    error = glGetError();
    if(error != NULL)
    {
        fboWrapper.errorStatus = error;
        qDebug() << "GL_ERROR_STATUS1:"<<error;
        return fboWrapper;
    }
    //qDebug() << "GL_ERROR_STATUS initFrameBufferTexture:"<<error;
    //qDebug() << "attachment.count():"<<attachment.count();
    //attachment.append(GL_COLOR_ATTACHMENT0+attachment.count());
    attachment.append(GL_COLOR_ATTACHMENT0);
    glFramebufferTexture2D(GL_FRAMEBUFFER , attachment.last() , GL_TEXTURE_2D, fbo_texture, 0); // Attach the texture fbo_texture to the color buffer in our frame buffer

    //glDrawBuffer(GL_COLOR_ATTACHMENT1);

    //glFramebufferTexture2D(GL_FRAMEBUFFER , GL_DEPTH_ATTACHMENT_EXT , GL_TEXTURE_2D , depth_buffer, 0); // Attach the depth buffer fbo_depth to our frame buffer
    // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depth_buffer, 0);
    error = glGetError();
    if(error != NULL)
    {
        fboWrapper.errorStatus = error;
        qDebug() << "GL_ERROR_STATUS2:"<<error;
        return fboWrapper;
    }
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER ); // Check that status of our generated frame buffer

    if (status != GL_FRAMEBUFFER_COMPLETE ) // If the frame buffer does not report back as complete
    {

        qDebug() << "Couldn't create frame buffer\n" ; // Output an error to the console
        //exit(0); // Exit the application
        fboWrapper.errorStatus=-1;

    }


    glBindFramebuffer(GL_FRAMEBUFFER , 0); // Unbind our frame buffer

    fboWrapper.bindedTexture=fbo_texture;
    fboWrapper.frameBuffer=fbo;
    fboWrapper.depthStensilBuffer = depth_buffer;
    GLenum err = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (err != GL_FRAMEBUFFER_COMPLETE) {
        qWarning() << "Can't create framebuffer:" << err;
        fboWrapper.errorStatus=-1;
    }



    return fboWrapper;
}

void OGLWidget::initializeGL()
{
    // qDebug() << "initializeGL";

    initializeOpenGLFunctions();
    makeCurrent();
    glEnable(GL_DEPTH_TEST);
    qglClearColor(Qt::black); // Черный цвет фона
    // qDebug() << "initializeGLFunctions";
    //glEnable(GL_TEXTURE_2D);
    backGroundTexture = loadTextureFromFile(":/ThirdPart/images/start.png");

    if (isShaderSupported())
        m_manager.getCreatedBrush().color_img=BrushPainter::getInstance()->applyBlur(m_manager.getCreatedBrush());
    else
    {
        m_manager.getCreatedBrush().color_img=BrushPainter::getInstance()->applyColor(m_manager.getCreatedBrush());
        m_manager.getCreatedBrush().color_img=BrushPainter::getInstance()->applyBlur(m_manager.getCreatedBrush());
    }
        brushTexture = loadTexture(m_manager.getCreatedBrush().color_img,true);
    //loadTextureFromFile(":/ThirdPart/images/brush.png");
    //initFrameBuffer(); // Create our frame buffer object
    mouseFBO=initFboWrapper(wax, way, false, true);
    mainFBO=initFboWrapper(wax, way, false, true);
    pingpongFBO=initFboWrapper(wax,way,false,true);

    initPBO();
    //initShader();
    glViewport(0, 0, (GLint)wax, (GLint)way);
    setAutoBufferSwap(false);
    qApp->processEvents(QEventLoop::AllEvents, 1000);
    //qDebug() << "A";
    drawBrushElm = new DrawBrushElm(this, 0);//record mouse movement
    //qDebug() << "B";
    /* list_1.append(GenerationDrawElement("kaka.text", this, 0));
    list_1.append(GenerationDrawElement("brush.png", this, 0));*/


    initShaderPrograms();

    connect(&m_manager,SIGNAL(currentBrushChanged()),this,SLOT(brushParamsChanged()));


}

void OGLWidget::moveEvent(QMoveEvent *event)
{
    /*if(isVisible())
    {
        QPoint curentPos = this->pos();
        curentPos.setY(curentPos.y() + this->height());
        timeLine->setViewPosition(curentPos);
    }*/
}


void OGLWidget::destroy(bool destroyWindow, bool destroySubWindow){
    glDeleteFramebuffers(1,&mouseFBO.frameBuffer);
    glDeleteTextures(1,&mouseFBO.bindedTexture);
    //glDeleteRenderbuffers(1,&render_buf);

}
FBOWrapper OGLWidget::getMouseFBO(){
    return mouseFBO;
}
FBOWrapper OGLWidget::getMainFBO(){
    return mainFBO;
}
FBOWrapper OGLWidget::getPingPongFBO(){
    return pingpongFBO;
}
bool OGLWidget::isAbleDrawing(){
    return able_drawing;
}

void OGLWidget::setAbleDrawing(bool value)
{
    able_drawing = value;
}

void OGLWidget::clearFrameBuffer(FBOWrapper fboWrapper){

    clearFrameBuffer(fboWrapper.frameBuffer);
}

void OGLWidget::clearFrameBuffer(GLuint fbo){
    //const GLuint *value={0};
    makeCurrent();
    GLint curentBuff;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &curentBuff);

    glBindFramebuffer(GL_FRAMEBUFFER , fbo); // Bind our frame buffer for rendering
    glClearColor(0.0,0.0,0.0,0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // чистим буфер изображения и буфер глубины

    glBindFramebuffer(GL_FRAMEBUFFER, curentBuff); // Bind our frame buffer for rendering

    /*std::vector<GLubyte> emptyData(wax * way * 4, 0);
   glBindTexture(GL_TEXTURE_2D, fboWrapper.bindedTexture);

   glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, wax, way, GL_RGBA, GL_UNSIGNED_BYTE, &emptyData[0]);
   glBindRenderbuffer(GL_RENDERBUFFER, fboWrapper.depthStensilBuffer);
   glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, wax, way, GL_RGBA, GL_UNSIGNED_BYTE, &emptyData[0]);
     // glBindTexture(GL_TEXTURE_2D,0);
  glBindTexture(GL_TEXTURE_2D, 0);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        isClearFrameBuffer=false;
     // glBindFramebuffer(GL_FRAMEBUFFER , 0);
      // qDebug()<<"clearFrameBuffer";
//        glClearBufferfi(GL_FRAMEBUFFER, fboWrapper.frameBuffer, 0, 0 );*/
    // glBindFramebuffer(GL_FRAMEBUFFER , 0); // Bind our frame buffer for rendering


}
void OGLWidget::clearTexture(GLuint textureId){
    std::vector<unsigned char> emptyPixels(1024*1024*4, 0);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, wax, way, GL_RGBA, GL_UNSIGNED_BYTE, emptyPixels.data());
    glBindTexture(GL_TEXTURE_2D, 0);

}
void OGLWidget::deleteFBO(FBOWrapper wrapper)
{
    if(wrapper.errorStatus != 0)
        return;

    makeCurrent();
    glDeleteTextures(1,&wrapper.bindedTexture);
    glDeleteFramebuffers(1,&wrapper.frameBuffer);

}
void OGLWidget::updateGrid(){
    if (CROSS_SHADER<shaderPrograms.length())
    {
        useShader(shaderPrograms[CROSS_SHADER]);
        //qDebug() << "width:"<<windowGrid.getWidth();
        shaderPrograms[CROSS_SHADER]->setUniform("size",windowGrid.getCellWidth(),windowGrid.getCellHeight());
        shaderPrograms[CROSS_SHADER]->setUniform("resolution",windowGrid.getWidth(),windowGrid.getHeight());
        useShader(0);
    }
    windowGrid.processLCP();
}

void OGLWidget::encoderAddWaitFrame()
{
    m_encoder->addWaitForFrame();
}

void OGLWidget::paintGL()
{

    updateGrid();
    reloadScene();

    /*
 *CONFIG DONE => START DRAWING IN MAIN FBO
*/

    qglColor(Qt::white);

    bindBuffer(mainFBO.frameBuffer);

    drawTexture(0, 0, wax, way, backGroundTexture, 0, 1, 1, -100000); //background

    /*
 *  DRAW BLOCK
*/

    ////qDebug() << "PREDRAW";
    m_encoder->clearWaitForFrame(); // send encoder for wait media block
    for(int i = 0; !timeLine->isBlocked && i < getList().size(); i++)
    {
        ////qDebug() << "draw   " << i;
        if( getList()[i] != NULL && timeLine->getMaxTrackTime() > 0)
            getList()[i]->paint();
        //test->use();
    }

    /*
 *  DRAW BRUSHES
*/

    if(curStatus != PLAY)
        paintBufferOnScreen(mouseFBO,0, 0, wax, way, 1000);
    /*
 *  APPLY SHADER EFFECT
*/

    QVector<ShaderProgramWrapper*> shaders;
    //shaderPrograms[CROSS_SHADER]->setUniformResolution(wax,way);
    if(curStatus == STOP && gridEnabled)
        shaders.push_back(shaderPrograms[CROSS_SHADER]);
    if (test->isInited())
        shaders.append(test);
    drawGlobalShader(shaders);

    /*
 *  DRAW MAIN FRAME WITH APPLY SHADER EFFECT
*/
    bindBuffer(0);
    drawEditBox(1000);
    paintBufferOnScreen(mainFBO,0, 0, wax, way,0);

    //glDisable(GL_BLEND);
    GLuint error = glGetError();
    //showLCP();
    glFinish();
    //////////////////////////////
    //glUseProgram(0);
    useShader(0);
    glFlush();
    swapBuffers();

    if(bRecord)
    {
        m_encoder->setFrame(grabFrameBuffer());
        if(timeLine->isVisible())
            timeLine->hide();
    }

    init = true;


}
DrawElement *OGLWidget::getSelElm() const
{
    return selElm;
}

void OGLWidget::setSelElm(DrawElement *value)
{
    selElm = value;
}



void OGLWidget::drawGlobalShader( QVector<ShaderProgramWrapper*> shaders)
{

    bool drawToSecondBuffer = true;//shaders.length()>1 && shaders.length()%2==0;
    for (int i=0;i<shaders.length();i++)
    {
        //qDebug() << "FOR ";
        if(drawToSecondBuffer)
        {
            // qDebug() <<" SECOND";
            bindBuffer(pingpongFBO.frameBuffer);
            useShader(shaders[i]);
            paintBufferOnScreen(mainFBO,0, 0, mainFBO.tWidth,mainFBO.tHeight, -1);
            useShader(0);

        }
        else
        {
            qDebug() <<" FIRST";
            bindBuffer(mainFBO.frameBuffer);
            useShader(shaders[i]);
            paintBufferOnScreen(pingpongFBO,0, 0, pingpongFBO.tWidth,pingpongFBO.tHeight, -1);
            useShader(0);
        }
        drawToSecondBuffer=!drawToSecondBuffer;
    }
    if(!drawToSecondBuffer)
    {
        // qDebug() <<" SECOND";
        //useShader(0);
        bindBuffer(mainFBO.frameBuffer);
        paintBufferOnScreen(pingpongFBO,0, 0, pingpongFBO.tWidth,pingpongFBO.tHeight, -1 );

    }

}


void drawCircle(GLfloat x, GLfloat y, GLfloat z, GLfloat radius)
{
    int i;
    int triangleAmount = 1000;
    GLfloat twicePi = 2.0f * M_PI;

    glEnable(GL_LINE_SMOOTH);
    glLineWidth(5.0);

    glBegin(GL_LINES);
    glColor4f(1.0, 0.0, 0.0, 1.0);
    for(i = 0; i <= triangleAmount; i++)
    {
        glVertex3f( x, y, z);
        glVertex3f(x + (radius * cos(i * twicePi / triangleAmount)), y + (radius * sin(i * twicePi / triangleAmount)), z);
    }
    glEnd();
}

void OGLWidget::drawEditBox( int z)
{
    int x1 = editingRectangle.rect.x();
    int y1 = editingRectangle.rect.y();
    int x2 = editingRectangle.rect.x()+editingRectangle.rect.width();
    int y2 = editingRectangle.rect.y()+editingRectangle.rect.height();


    //qDebug() <<  editingRectangle.editingRectangleMode << "     " << editingRectangle.angle;

  //  editingRectangle.angle++;




    // m_manager.setAbleToDraw(true);

    int leftTopCornerX1=x1-editingRectangle.cornerSize/2;
    int leftTopCornerY1=y1-editingRectangle.cornerSize/2;
    int leftTopCornerX2=x1 + editingRectangle.cornerSize/2;
    int leftTopCornerY2=y1 + editingRectangle.cornerSize/2;

    int rightTopCornerX1=x2-editingRectangle.cornerSize/2;
    int rightTopCornerY1=y1-editingRectangle.cornerSize/2;
    int rightTopCornerX2=x2 + editingRectangle.cornerSize/2;
    int rightTopCornerY2=y1 + editingRectangle.cornerSize/2;

    int leftBottomCornerX1=x1-editingRectangle.cornerSize/2;
    int leftBottomCornerY1=y2-editingRectangle.cornerSize/2;
    int leftBottomCornerX2=x1 + editingRectangle.cornerSize/2;
    int leftBottomCornerY2=y2 + editingRectangle.cornerSize/2;

    int rightBottomCornerX1=x2-editingRectangle.cornerSize/2;
    int rightBottomCornerY1=y2-editingRectangle.cornerSize/2;
    int rightBottomCornerX2=x2 + editingRectangle.cornerSize/2;
    int rightBottomCornerY2=y2 + editingRectangle.cornerSize/2;

    if (editingRectangle.isEditingRectangleVisible && !forceEditBoxDisable && !able_drawing && getStatus()!= PLAY)
    {

        if(editingRectangle.editingRectangleMode == EDIT_RECTANGLE_ROTATION)
        {
            QVector2D vec(x1 + (x2 - x1)/2 - mousePos.x(), y1 + (y2-y1)/2 - mousePos.y());
            vec.normalize();
            vec *= -( qMin(editingRectangle.rect.width(), editingRectangle.rect.height()))/4;

            drawCircle(vec.x() + x1 + (x2 - x1)/2 , vec.y() + y1 + (y2-y1)/2, z, editingRectangle.circleSize);

            editingRectangle.angle = (180*qAcos(-vec.x()/vec.length()))/M_PI;
            if(vec.y() > 0)
            {
                editingRectangle.angle = 360 - editingRectangle.angle;
            }

        }
        else
        {
            QVector2D vec(qCos((editingRectangle.angle * M_PI)/180), qSin((editingRectangle.angle * M_PI)/180));
            vec.normalize();
            vec *= -( qMin(editingRectangle.rect.width(), editingRectangle.rect.height()))/4;

            drawCircle(vec.x() + x1 + (x2 - x1)/2 , vec.y() + y1 + (y2-y1)/2, z, editingRectangle.circleSize);
        }

        // paintBufferOnScreen(0, 0, wax, way);
        //rectangle
        glLineWidth(3);
        glColor3f(1.0f, 0.0f, 0.0f);
        glBegin(GL_LINES);
        glVertex3i(x1,y1, z);
        glVertex3i(x2,y1, z);

        glVertex3i(x2,y1, z);
        glVertex3i(x2,y2, z);

        glVertex3i(x2,y2, z);
        glVertex3i(x1,y2, z);

        glVertex3i(x1,y2, z);
        glVertex3i(x1,y1, z);
        glEnd();


        glColor3f(0.0f, 1.0f, 0.0f);
        glLineWidth(3);
        glBegin(GL_QUADS);   //We want to draw a quad, i.e. shape with four sides
        // glColor3i(1, 0, 0); //Set the colour to red
         //left top corner
        glVertex3i(leftTopCornerX1, leftTopCornerY1, z);            //Draw the four corners of the rectangle
        glVertex3i(leftTopCornerX2, leftTopCornerY1, z);
        glVertex3i(leftTopCornerX2, leftTopCornerY2, z);
        glVertex3i(leftTopCornerX1, leftTopCornerY2, z);
         //right top corner
        glVertex3i(rightTopCornerX1, rightTopCornerY1, z);            //Draw the four corners of the rectangle
        glVertex3i(rightTopCornerX2, rightTopCornerY1, z);
        glVertex3i(rightTopCornerX2, rightTopCornerY2, z);
        glVertex3i(rightTopCornerX1, rightTopCornerY2, z);
        //left bottom corner
       glVertex3i(leftBottomCornerX1, leftBottomCornerY1, z);            //Draw the four corners of the rectangle
       glVertex3i(leftBottomCornerX2, leftBottomCornerY1, z);
       glVertex3i(leftBottomCornerX2, leftBottomCornerY2, z);
       glVertex3i(leftBottomCornerX1, leftBottomCornerY2, z);
       //right bottom corner
      glVertex3i(rightBottomCornerX1, rightBottomCornerY1, z);            //Draw the four corners of the rectangle
      glVertex3i(rightBottomCornerX2, rightBottomCornerY1, z);
      glVertex3i(rightBottomCornerX2, rightBottomCornerY2, z);
      glVertex3i(rightBottomCornerX1, rightBottomCornerY2, z);

        glEnd();

    }
}

void OGLWidget::reloadScene()
{
    //glDrawBuffer(GL_COLOR_ATTACHMENT1);
    makeCurrent();
    clearFrameBuffer(mainFBO);
    clearFrameBuffer(pingpongFBO);
    glBindFramebuffer(GL_FRAMEBUFFER , 0);
    // useShader(test);
    //// qDebug() << "isClearFrameBuffer:"<<isClearFrameBuffer;
    if(isClearMouseFrameBuffer)
    {
        clearFrameBuffer(mouseFBO);
        isClearMouseFrameBuffer=false;
    }
    //glDrawBuffer(GL_COLOR_ATTACHMENT0);
    glClearColor(0.0,0.0,0.0,0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // чистим буфер изображения и буфер глубины

    //glClearStencil(0);
    timeLine->update();

    glMatrixMode(GL_PROJECTION); // устанавливаем матрицу
    //  glShadeModel(GL_SMOOTH);
    // Сглаживание точек
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    // Сглаживание линий
    //glEnable(GL_LINE_SMOOTH);
    //glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    // glEnable(GL_POLYGON_SMOOTH);
    // glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    // glEnable(GL_MULTISAMPLE);

    glLoadIdentity(); // загружаем матрицу
    glOrtho(0,wax,way,0, -1000000, 200000); // подготавливаем плоскости для матрицы
    glEnable(GL_BLEND);
    glEnable(GL_ALPHA_TEST);

    // glDepthMask(false);
    glDepthFunc(GL_LEQUAL); // this maybe problem
    glAlphaFunc(GL_GREATER,0);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
    glEnable(GL_DEPTH_TEST);
    //glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    //glBlendEquation ( GL_FUNC_ADD ) ;
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void OGLWidget::paintEvent(QPaintEvent *event)
{

}

void OGLWidget::useShader(ShaderProgramWrapper *shader){
    //qDebug() << "before useShader currentShaderStack len:"<<currentShaderStack.length();
    if (shader==NULL){
        if (currentShaderStack.isEmpty())
            glUseProgram(0);
        else
        {
            currentShaderStack.pop();//Discard last shader
            if (currentShaderStack.isEmpty()) glUseProgram(0);
            else
                glUseProgram(currentShaderStack.last()->getShaderProgram());
        }
    }
    else {
        currentShaderStack.push(shader);
        glUseProgram(shader->getShaderProgram());
    }
    // qDebug() << "after useShader currentShaderStack len:"<<currentShaderStack.length();
}

void OGLWidget::disableShader(){
    // qDebug() << "disableShader:"<<currentShaderStack.length();
    glUseProgram(0);
}
void OGLWidget::enableShader(){
    // qDebug() << "enableShader:"<<currentShaderStack.length();
    if (currentShaderStack.length()>0)
        glUseProgram(currentShaderStack.last()->getShaderProgram());

}


void OGLWidget::resizeEvent(QResizeEvent *envent)
{
    resizeGL(envent->size().width(), envent->size().height());
    //maxWidth = width() - marginLeft;
}


void OGLWidget::closeEvent(QCloseEvent *event)
{
    //useShader(0);
    disableShader();
    stopAnimated();

    pause(500);
    isClose = true;
    m_manager.close();
    shaderWindow.close();
}

void OGLWidget::hideBrushManager()
{
    m_manager.hide();
    setIsBrushWindowOpened(false);
}

void OGLWidget::applyEffectsToCurrentBlock()
{
    QPoint blockIndex = effectManager->getCurrentBlockIndex();
    if (blockIndex.x()==-1 || blockIndex.y()==-1)return;
    // qDebug() << "CUR BLOCK(SAVE):"<<blockIndex;
    // qDebug() << "apply effect begin";
    QVector<Effect> blockEffects = effectManager->getDataListValues();
    QVector<ShaderEffect> timeLineEffects;
    qDebug() << "applyEffectsToCurrentBlock:"<<blockEffects.length();
    for (int i=0;i<blockEffects.length();i++){
        Effect *blockEffect = &blockEffects[i];
        int shaderProgramIndex = (int)blockEffect->getPropetrie("effect_type");
        switch (shaderProgramIndex){
        case ALPHA_SHADER:
        case SPIN_SHADER:
        case CIRCLES_SHADER:
        case PIXELIZATION_SHADER:
        case TURNTHEPAGE_SHADER:
        case RANDSQUARES_SHADER:
        case TRESHOLD_SHADER:
        case SLIDE_SHADER:

        case TRANSFORMATIONS_EFFECT:
            ShaderProgramWrapper* shaderWrapper=NULL;
            if (shaderProgramIndex!=TRANSFORMATIONS_EFFECT)
                shaderWrapper=shaderPrograms[shaderProgramIndex];
             ShaderEffect sEffect(shaderWrapper,shaderProgramIndex);

            int startTime = qFloor(blockEffect->getPropetrie("start_time"));
            int endTime = qFloor(blockEffect->getPropetrie("end_time"));
            bool reverse = blockEffect->getPropetrie("inversion");
            int count = blockEffect->getPropetrie("count");
            int elmSize = blockEffect->getPropetrie("elementSize");
            int rotateAngle = blockEffect->getPropetrie("rotate_angle");
            bool movingEnabled = blockEffect->getPropetrie("move_to_enable");
            int toPosX = blockEffect->getPropetrie("to_x_pos");
            int toPosY = blockEffect->getPropetrie("to_y_pos");

            double toScaleX = blockEffect->getPropetrie("to_x_scale");
            double toScaleY = blockEffect->getPropetrie("to_y_scale");

            sEffect.setStartTimeMS(startTime);
           // qDebug() << "SHOW   " << blockEffect->getPropetrie("start_time");
            sEffect.setEffectTimeHowLong(endTime-startTime);
            sEffect.setReverse(reverse);
            sEffect.setShaderWrapperIndex(shaderProgramIndex);
            sEffect.setCount(count);
            sEffect.setElementSize(elmSize);
            sEffect.setRotateAngle(rotateAngle);
            sEffect.setMoving(movingEnabled);
            sEffect.setToPosX(toPosX);
            sEffect.setToPosY(toPosY);
            sEffect.setToScaleX(toScaleX);
            sEffect.setToScaleY(toScaleY);
            timeLineEffects.push_back(sEffect);
            break;
        }
    }
   // qDebug() << "apply effect after for";

   // qDebug() << "selectedBlockIndex" << blockIndex.x()<<" "<<blockIndex.y();
    timeLine->getBlock(blockIndex)->setEffects(timeLineEffects);
 //   qDebug() << "apply effect end";
}
void OGLWidget::loadEffectFromCurrentBlockToEffectManager()
{
    effectManager->setCurrentBlockIndex(timeLine->getSelectedBlockPoint());

    QPoint blockIndex = effectManager->getCurrentBlockIndex();
    // qDebug() << "CUR BLOCK(LOAD):"<<blockIndex;
    DrawElement *currentBlock = timeLine->getBlock(blockIndex);
    effectManager->clearEffects();
    effectManager->setBlockTime(currentBlock->getLifeTime());
    int indexes[shaderPrograms.length()+1];//1 -need additional value for
    for (int i = 0; i < shaderPrograms.length()+1;i++)
        indexes[i]=0;
    qDebug() << "loadEffectFromCurrentBlockToEffectManager:"<<currentBlock->getEffects().length();
    for (ShaderEffect  currentEffect : currentBlock->getEffects())
    {
        //  qDebug() << "cur EFFECT start TIME:"<<currentEffect.getStartTimeMS();
        //qDebug() << "cur EFFECT howlong TIME:"<<currentEffect.getEffectTimeHowLong();
        Effect effect;
        int currentProgramIndex = currentEffect.getShaderWrapperIndex();
        indexes[currentProgramIndex]++;
        QString nameStart;
        switch(currentProgramIndex){
        case ALPHA_SHADER:
            nameStart="alpha";
            break;
        case SPIN_SHADER:
            nameStart="spin";
            break;
        case PIXELIZATION_SHADER:
            nameStart="pixelization";
            break;
        case CIRCLES_SHADER:
            nameStart="circles";
            break;
        case TURNTHEPAGE_SHADER:
            nameStart="turnPage";
            break;
        case RANDSQUARES_SHADER:
            nameStart="squares";
            break;
        case TRESHOLD_SHADER:
            nameStart="treshold";
            break;
        case SLIDE_SHADER:
            nameStart="slide";
            break;
        case TRANSFORMATIONS_EFFECT:
            nameStart="transformation";
                    break;
        default:
            nameStart="effect";
            break;
        }
        effect.setName(nameStart+QString::number(indexes[currentProgramIndex]));


        effect.setPropetrie("start_time",currentEffect.getStartTimeMS());
        effect.setPropetrie("end_time",currentEffect.getStartTimeMS()+currentEffect.getEffectTimeHowLong());
        effect.setPropetrie("inversion",currentEffect.getReverse());
        effect.setPropetrie("effect_type",currentEffect.getShaderWrapperIndex());
        effect.setPropetrie("count",currentEffect.getCount());
        effect.setPropetrie("elementSize",currentEffect.getElementSize());
        effect.setPropetrie("rotate_angle",currentEffect.getRotateAngle());
        effect.setPropetrie("to_x_pos",currentEffect.getToPosX());
        effect.setPropetrie("to_y_pos",currentEffect.getToPosY());
        effect.setPropetrie("to_x_scale",currentEffect.getToScaleX());
        effect.setPropetrie("to_y_scale",currentEffect.getToScaleY());
        effect.setPropetrie("moving",currentEffect.getMoving());
        effectManager->addEffect(effect);
    }
    //effectManager->update();


}

void OGLWidget::hideEffectsManager(){
    effectManager->hide();
    isEffectsManagerOpened=false;

}
void OGLWidget::showEffectsManager(){
    effectManager->show();
    isEffectsManagerOpened=true;
}

void OGLWidget::showPrompterManager(){
    prompterManager->show();
    isPrompterManagerOpened=true;
}
void OGLWidget::hidePrompterManager(){
    prompterManager->hide();
    isPrompterManagerOpened=false;
}



void OGLWidget::mousePressEvent(QMouseEvent *event)
{

    getTimeLine()->emitFocusLostSignal();
    if(event->button() == Qt::RightButton)
    {
        if (getIsBrushWindowOpened()){
            m_manager.hide();
            setIsBrushWindowOpened(false);
        }
        else
        {
            m_manager.setPosition(QCursor::pos());
            qDebug() << "SHOW_BRUSH_MANAGER";
            m_manager.show();
            setIsBrushWindowOpened(true);
        }
    }
    if(event->button() == Qt::LeftButton)
    {
        //isMousePlay = false;
        stopShowLastDrawing();
        effectManager->hide();
        if (getIsBrushWindowOpened())
        {
            m_manager.hide();
            setIsBrushWindowOpened(false);
        }
        else{
            mousePos.setX(event->x());
            mousePos.setY(event->y());
            prevMousePos.setX(mousePos.x());
            prevMousePos.setY(mousePos.y());
            isMousePress=true;

            ismouseWasPressedBeforeDrag=false;

            mousePressPos = QPoint(mousePos.x() - editingRectangle.rect.x(),
                                   mousePos.y() - editingRectangle.rect.y());

            QRect temp = editingRectangle.rect;
            /*if (temp.x() <= mousePos.x() && temp.x() + temp.width() >= mousePos.x()
                    && temp.y() <= mousePos.y() && temp.y() + temp.height() >= mousePos.y()  )
                isPainting = false;*/

            //for rectangle resize
           /* GLint x1 = editingRectangle.rect.x();
            GLint y1 = editingRectangle.rect.y();*/

            /*int leftTopCornerX1=x1-editingRectangle.cornerSize/2;
            int leftTopCornerY1=y1-editingRectangle.cornerSize/2;
            int leftTopCornerX2=x1 + editingRectangle.cornerSize/2;
            int leftTopCornerY2=y1 + editingRectangle.cornerSize/2;*/

       //if( editingRectangle.editingRectangleMode==EDIT_RECTANGLE_RESIZE)

          /*  if ((mousePos.x() >= leftTopCornerX1 && mousePos.x() <= leftTopCornerX2)
                    && (mousePos.y() >= leftTopCornerY1 && mousePos.y() <= leftTopCornerY2))
                isPainting = false;*/
        }
    }

    //int x = event->x();
    //int y = event->y();
    //drawImage();



}

void OGLWidget::testRectangle()
{
#define MAX_DELTA 10

    if(editingRectangle.rect.x() > this->width() - MAX_DELTA)
        editingRectangle.rect.moveTo(this->width() - MAX_DELTA, editingRectangle.rect.y());
    if(editingRectangle.rect.y() > this->height() - MAX_DELTA)
        editingRectangle.rect.moveTo(editingRectangle.rect.x(), this->height()  - MAX_DELTA);

    if(editingRectangle.rect.x() < -editingRectangle.rect.width() + MAX_DELTA)
        editingRectangle.rect.moveTo(-editingRectangle.rect.width() + MAX_DELTA, editingRectangle.rect.y());
    if(editingRectangle.rect.y() < -editingRectangle.rect.height() + MAX_DELTA)
        editingRectangle.rect.moveTo(editingRectangle.rect.x(), -editingRectangle.rect.height() + MAX_DELTA);

    if(editingRectangle.rect.width() > this->width()*2)
        editingRectangle.rect.setWidth(this->width() *2);
    if(editingRectangle.rect.height() > this->height() *2)
        editingRectangle.rect.setHeight(this->height()*2);

    /*
     *
     *    if(editingRectangle.rect.x() > this->width() - editingRectangle.rect.width())
        editingRectangle.rect.moveTo(this->width() - editingRectangle.rect.width(), editingRectangle.rect.y());
    if(editingRectangle.rect.y() > this->height() - editingRectangle.rect.height())
        editingRectangle.rect.moveTo(editingRectangle.rect.x(), this->height()  - editingRectangle.rect.height());

    if(editingRectangle.rect.x() < 0)
        editingRectangle.rect.moveTo(0, editingRectangle.rect.y());
    if(editingRectangle.rect.y() < 0)
        editingRectangle.rect.moveTo(editingRectangle.rect.x(), 0);

    if(editingRectangle.rect.width() > this->width())
        editingRectangle.rect.setWidth(this->width());
    if(editingRectangle.rect.height() > this->height())
        editingRectangle.rect.setHeight(this->height());
    */

}

bool OGLWidget::isInit()
{
    return init;
}

void OGLWidget::setFileNameForRecords(QString value)
{
    fileNameForRecords = value;
}

void OGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    gridEnabled=false;
    //int x = event->x();
    //int y = event->y();
    //drawImage();
    isMousePress=false;
    if (editingRectangle.editingRectangleMode==EDIT_RECTANGLE_RESIZE)
        editingRectangle.rect=editingRectangle.rect.normalized();
    editingRectangle.editingRectangleMode=EDIT_RECTANGLE_UNBINDED;

    /*
     * MIN_SIZE
     */

    if(editingRectangle.rect.width() < MIN_RECT_SIZE)
        editingRectangle.rect.setWidth(MIN_RECT_SIZE);
    if(editingRectangle.rect.height() < MIN_RECT_SIZE)
        editingRectangle.rect.setHeight(MIN_RECT_SIZE);

    testRectangle();

    /* if(editingRectangle.rect.width() > this->width())
        editingRectangle.rect.setWidth(this->width());
    if(editingRectangle.rect.height() > this->height())
        editingRectangle.rect.setHeight(this->height());
    */

}

void OGLWidget::mouseMoveEvent ( QMouseEvent * event ){
    //mouseDrag
    if (event->buttons() & Qt::LeftButton) {
        mousePos.setX(event->x());
        mousePos.setY(event->y());

        ismouseWasPressedBeforeDrag=true;
        //// //qDebug()<<"mouseRecorderTimer.elapsed():"<<mouseRecorderTimer.elapsed();

    }
}

void OGLWidget::keyReleaseEvent(QKeyEvent *event)
{
    switch(event->key () )
    {
    case Qt::Key_Control:
        pressedCtrl = false;
        break;
    case Qt::Key_Shift:
        pressedShift = false;
        break;
    case Qt::Key_Plus:
        pressedPlus=false;
        break;
    case Qt::Key_Minus:
        pressedMinus=false;

    }

}

bool OGLWidget::event(QEvent *e)
{
    //qDebug() << "event";

    switch(e->type())
    {
    // ...

    case QEvent::WindowActivate :
        // timeLine->setFocus();
        break ;

    case QEvent::WindowDeactivate :
        break ;
        // ...

    } ;

    return QGLWidget::event(e) ;
}


void OGLWidget::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "keyPressEvent";
    switch(event->key())
    {
    case(Qt::Key_Control):
        pressedCtrl = true;
        break;
    case Qt::Key_Shift:
        pressedShift = true;
        break;
    case Qt::Key_Plus:
        pressedPlus=true;
        break;
    case Qt::Key_Minus:
        pressedMinus=true;
        break;
    }

    if (pressedCtrl)
    {
        if ( pressedShift ) //keySequence[0] == Qt::Key_Control && keySequence[1] == Qt::Key_Shift  )
        {

            qDebug() << event->key ();
            switch(event->key ())
            {
            case Qt::Key_S : case 1067 :
                timeLine->emitSaveProject();
                break;
            case Qt::Key_O : case 1065 :
                timeLine->emitOpenProject();
                break;
            case Qt::Key_N : case 1058 :
                timeLine->emitNewProject();
                break;
            }
        }
        else
            switch(event->key ())
            {
            case 43 :
                timeLine->zoomPlus();
                break;
            case 45 :
                emit timeLine->zoomMinus();
                break;
            }
    }
    if (pressedShift){
        if (pressedMinus)zoomGrid(-ZOOM_STEP);
        else if (pressedPlus) zoomGrid(ZOOM_STEP);
    }


}

void OGLWidget::ShowHideShaderWindow()
{
    if(shaderWindow.isVisible())
        shaderWindow.hide();
    else
        shaderWindow.show();
}

QSize OGLWidget::getTextureSize()
{
    glEnable(GL_TEXTURE_2D);
    QSize textureSize;
    GLint width;
    GLint height;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
    textureSize.setWidth(width);
    textureSize.setHeight(height);
    return textureSize;
}

void OGLWidget::clearAudioList()
{
    audioList.clear();
}
/*
void OGLWidget::addAudioToList(QByteArray arr, void *obj)
{
    QMutableMapIterator i = audioList.find();
    if(i != QMap::end())
        ((QList <QByteArray>)i.value()).append(arr);
    else
    {
        QList <QByteArray> listArr;
        listArr.append(arr);
        audioList.insert(obj, listArr);
    }
    //audioList.append(arr);

}
*/
void OGLWidget::addAudioToList(void*obj, QByteArray arr)
{
    if(this->bRecord)
        m_encoder->addToAudioBuffer(obj, arr);
}
QString OGLWidget::getDrawText()
{
    return drawText;
}

void OGLWidget::setDrawText(QString data)
{
    drawText = data;
}



bool OGLWidget::drawAnimated(bool record)
{

    if(curStatus == this->PAUSE)
    {
        //m_recorder->resume();
        curStatus = PLAY;
        if(bRecord)
            timeLine->hide();
        pause(300);
        m_encoder->pause();

        getTimeLine()->play();
        return true;
    }


    if(record)
    {
        editingRectangle.isEditingRectangleVisible = false;

        //QString fileName = QFileDialog::getSaveFileName(this, tr("Choose file..."), qApp->applicationDirPath(), tr("Videos (*.avi *.mp4)"), 0, QFileDialog::DontUseNativeDialog);
        //qDebug() << "OKKKKKKKKKKKKKKKKKKKKKKK";
        //  //qDebug() << "SHOW_FILE_NAME " << fileName;
        m_encoder->setFileName(fileNameForRecords);
        m_encoder->setGrabWidget(this);

        m_encoder->startRecord();
        // QTimer::singleShot(1,m_encoder, SLOT(startRecord() ) );
        // //qDebug() << "Start record into file";
        timeLine->hide();
    }
    curStatus = PLAY;
    bRecord = record;
    //tickTimer.start();
    // //qDebug() << "Start play";
    emit startSignal();
    getTimeLine()->play();
    return true;
}

void OGLWidget::stopAnimated()
{
    //pause(100);
    if(init)
    {
        qDebug() << "STOP";
        if(bRecord)
        {
            if(m_encoder->getBPause())
                m_encoder->pause();
            m_encoder->stop();
            timeLine->show();
        }
        curStatus = STOP;
        //tickTimer.stop();

        //m_encoder->stop();
        //clearFrameBuffer(mouseFBO);
        isClearMouseFrameBuffer=true;
        getTimeLine()->stop();


        // double t_animationPersentOfCross = animationPersentOfCross;

    }

    /*    int t_delay = delay;

    animationPersentOfCross = 1;
    delay = 1;
    while(busy)
    {
         qApp->processEvents();
    }
    animationPersentOfCross = t_animationPersentOfCross;
    delay = t_delay;
*/
    bRecord = false;

    //   pause(200);
    qDebug() << "Stop play" << timeLine->getPlayTime();
    emit stopSignal();

}

void OGLWidget::pauseAnimated()
{
    m_encoder->pause();

    curStatus = PAUSE;



    // //qDebug() << "Pause play";
    emit pauseSignal();
    if(bRecord)
        timeLine->show();
    //m_recorder->pause();
}

void OGLWidget::brushParamsChanged()
{
    if (!able_drawing)return;
    if (isShaderSupported())
        m_manager.getCreatedBrush().color_img=BrushPainter::getInstance()->applyBlur(m_manager.getCreatedBrush());
    else
    {
        m_manager.getCreatedBrush().color_img=BrushPainter::getInstance()->applyColor(m_manager.getCreatedBrush());
         m_manager.getCreatedBrush().color_img=BrushPainter::getInstance()->applyBlur(m_manager.getCreatedBrush());
   }
        brushTexture = loadTexture(m_manager.getCreatedBrush().color_img,true);
    //while (!isInit())
    //qDebug() << "waiting for init";
    drawBrushElm->addBrush(m_manager.getCreatedBrush());
    //qDebug() << "brushParamsChanged";

}
bool OGLWidget::isRecord() const
{
    return bRecord;
}

bool OGLWidget::isNeedShowTimeLine()
{
    if(timeLine->isVisible() && !(curStatus == PLAY && bRecord))
        return true;
    else
        return false;
}





void OGLWidget::drawFigure(int x, int y, int x2, int y2, /*OGLWidget::*/FigureType type, bool fill , QColor col , float size )
{
    //// //qDebug()<<"void OGLWidget::drawFigure";
    glDisable(GL_BLEND);
    qglColor(col);
    glLineWidth(size);
    //glPointSize(200.0f);
    glBegin(GL_LINES);

    glVertex2i( x,y);
    glVertex2i( x2,y2);
    // //qDebug()<<"x:"<<x;
    // //qDebug()<<"y:"<<y;
    // //qDebug()<<"x2:"<<x2;
    //glVertex3f( 0,0,0);
    // glVertex3f( 100,100,0);
    glEnd();
    glDisable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);

    /*QString sColor = QString("rgba(%1, %2, %3, %4)").arg(col.red()).arg(col.green()).arg(col.blue()).arg(col.alpha());
    QMetaObject::invokeMethod(canvas, "drawFigure",
            Q_ARG(QVariant, QVariant(x)),
            Q_ARG(QVariant, QVariant(y)),
            Q_ARG(QVariant, QVariant(width)),
            Q_ARG(QVariant, QVariant(height)),
            Q_ARG(QVariant, QVariant(type)),
            Q_ARG(QVariant, QVariant(fill)),
                              Q_ARG(QVariant, QVariant(size)),
                              Q_ARG(QVariant, QVariant(sColor)));*/
    /*switch (type){
    case LINE:


        glEnable(GL_LINE_SMOOTH);
        glHint(GL_LINE_SMOOTH_HINT,  GL_NICEST);

        glBegin(GL_LINES);
           glVertex3f( x,y,0);
           glVertex3f( x+width,y+height,0);
         //glVertex3f( 0,0,0);
        // glVertex3f( 100,100,0);
           glEnd();
        glDisable(GL_LINE_SMOOTH);
        break;
    }*/

}

void OGLWidget::drawRectangle(int x, int y, int x2, int y2, QColor col,bool fill,float lineSize)
{
    //// //qDebug()<<"void OGLWidget::drawFigure";
    glDisable(GL_BLEND);
    qglColor(col);
    glLineWidth(lineSize);
    //glPointSize(200.0f);
    glBegin(GL_QUADS);
    glVertex2i( x,y);
    glVertex2i( x2,y);
    glVertex2i( x2,y2);
    glVertex2i( x,y2);
    glEnd();
    glDisable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);

    /*QString sColor = QString("rgba(%1, %2, %3, %4)").arg(col.red()).arg(col.green()).arg(col.blue()).arg(col.alpha());
    QMetaObject::invokeMethod(canvas, "drawFigure",
            Q_ARG(QVariant, QVariant(x)),
            Q_ARG(QVariant, QVariant(y)),
            Q_ARG(QVariant, QVariant(width)),
            Q_ARG(QVariant, QVariant(height)),
            Q_ARG(QVariant, QVariant(type)),
            Q_ARG(QVariant, QVariant(fill)),
                              Q_ARG(QVariant, QVariant(size)),
                              Q_ARG(QVariant, QVariant(sColor)));*/
    /*switch (type){
    case LINE:


        glEnable(GL_LINE_SMOOTH);
        glHint(GL_LINE_SMOOTH_HINT,  GL_NICEST);

        glBegin(GL_LINES);
           glVertex3f( x,y,0);
           glVertex3f( x+width,y+height,0);
         //glVertex3f( 0,0,0);
        // glVertex3f( 100,100,0);
           glEnd();
        glDisable(GL_LINE_SMOOTH);
        break;
    }*/

}






void OGLWidget::generateFrames()
{

}
bool OGLWidget::getIsPrompterManagerOpened()
{
    return isPrompterManagerOpened;
}

int OGLWidget::getCurStatus() const
{
    return (int) curStatus;
}

void OGLWidget::setCurStatus(const StatusDraw &value)
{
    curStatus = value;
}




bool OGLWidget::getIsBrushWindowOpened() const
{
    return isBrushWindowOpened;
}

void OGLWidget::setIsBrushWindowOpened(bool value)
{
    isBrushWindowOpened = value;
}

int OGLWidget::getWax()
{
    return wax;
}

int OGLWidget::getWay()
{
    return way;
}






bool OGLWidget::getBusy() const
{
    return busy;
}

void OGLWidget::setBusy( bool value)
{
    busy = value;
}

void OGLWidget::pause(int ms)
{
    if( ms < 1)
        return;
    QTimer tickTimer;
    tickTimer.setSingleShot(true);
    tickTimer.start(ms);
    while (tickTimer.isActive()) {
        qApp->processEvents(QEventLoop::EventLoopExec);
        if(curStatus == STOP)
            break;
    }
}


void  OGLWidget::updateWindow(){
    //isCrossingNow=true;
    //if (timeLine->getPointedBlocks().size())
    /// if(!timeLine->isBlocked)
    current_millisecs = QDateTime::currentMSecsSinceEpoch();
    if ((current_millisecs - last_mouse_process) >= 1000/MOUSE_PROCESS_DELAY_MS)
    {
        processMouse();
        storeMousePos();
        last_mouse_process = current_millisecs;
    }

    QPoint t = timeLine->getSelectedBlockPoint();

    if(curStatus != PLAY && t.x() >= 0 && !timeLine->isBlocked)
    {
        editingRectangle.isEditingRectangleVisible = true;

        if(timeLine->getBlock(t) != selElm )
        {
            //clearBuffer();
            selElm = timeLine->getBlock(t);
            editingRectangle.rect = timeLine->getDrawRect(t.x(), t.y());
            editingRectangle.angle = selElm->getRotationAngle();
        }
        else
        {

            // if (!timeLine->getIsEditBlockShow())
            {
                QRect t2 = editingRectangle.rect;
                selElm->setX( t2.x());
                selElm->setY( t2.y());
                selElm->setSize(t2.width(), t2.height());
                selElm->setRotationAngle(editingRectangle.angle);
            }
        }
    }
    else
    {
        // //qDebug() << "SBLOCK " << t;
        selElm = NULL;
        editingRectangle.isEditingRectangleVisible = false;

    }


    current_millisecs = QDateTime::currentMSecsSinceEpoch();
    if ((current_millisecs - last_milisecs_drawn) >= 1000/frameRate)
    {
        emit windowUpdating(1000/frameRate);
        setList(timeLine->getPointedBlocksDE());
        last_milisecs_drawn=current_millisecs;
        updateGL();

    }
}



int OGLWidget::getDelay() const
{
    return delay;
}

void OGLWidget::setDelay(int value)
{
    delay = value;
    //animationPersentOfCross = (double)1/(value*0.6);
    // //qDebug() << animationPersentOfCross;
}







OGLWidget::StatusDraw OGLWidget::getStatus() const
{
    return curStatus;
}

void OGLWidget::recreate()
{
    this->create();
}
void OGLWidget::clear(int x,int y,int width,int height){

}
void OGLWidget::myRenderText( QGLWidget* w, int x, int y,int z, const QString& text, const QColor& col , const QFont& font , float scale )
{
    qDebug() << scale;
    if (text.isEmpty()) return;

    // //qDebug() <<"myRenderText begin";
    /*glMatrixMode( GL_PROJECTION );
    glPushMatrix();
    glLoadIdentity();
    glOrtho( 0, w->width(), w->height(), 0, 1000000, -2000000 );
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    //glLoadIdentity();*/
    glPushMatrix();

    QFontMetrics fm(font);



    QRect rect = fm.boundingRect( text);
    if (rect.width()>wax)rect.setWidth(wax);
    if (rect.height()>way)rect.setHeight(way);

    QPixmap pixmap( rect.size() );
    pixmap.fill( Qt::black );
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::TextAntialiasing);
    painter.setPen( Qt::white );
    painter.setFont( font );
    painter.drawText( -rect.left(), -rect.top(), text );
    QImage img = pixmap.toImage().convertToFormat(QImage::Format_ARGB32);
    //img.setAlphaBuffer( true );
    for ( int i = 0; i < img.height(); i++ ) {
        QRgb* rgb = (QRgb*) img.scanLine(i);
        for ( int j = 0; j < img.width(); j++ ) {
            rgb[j] = qRgba( col.red(), col.green(), col.blue(), qRed(rgb[j]) );
        }
    }
    img = QGLWidget::convertToGLFormat(img);

    // glEnable(GL_BLEND);
    //   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glRasterPos3i( x, y, z );
    //glScalef(getWax()/rect.width(), getWay()/rect.height(), 0);
    //glPixelZoom(getWax()/rect.width(), getWax()/rect.width());
    glDrawPixels( rect.width(), rect.height(), GL_RGBA, GL_UNSIGNED_BYTE, img.bits() );
    //glDisable(GL_BLEND);
    /*glMatrixMode( GL_PROJECTION );
    glPopMatrix();
    glMatrixMode( GL_MODELVIEW );*/
    //  glPopMatrix();
    //  //qDebug() <<"myRenderText end";

    /*
     *
     * IMAGE VERSION
    QFontMetrics fm(font);
    QRect rect = fm.boundingRect( text);

    QImage img(rect.size(), QImage::Format_RGBA8888);
    img.fill("transparent");
    QPainter painter(&img);
    painter.setPen( Qt::white );
    painter.setFont( font );
    painter.drawText( -rect.left(), -rect.top(), text );
    painter.end();
    img = QGLWidget::convertToGLFormat(img);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glRasterPos3i( x, y, z );
    glDrawPixels( rect.width(), rect.height(), GL_RGBA, GL_UNSIGNED_BYTE, img.bits() );
    glDisable(GL_BLEND);
    glPopMatrix();

     */
}

void OGLWidget::drawTextFromTexture( int x, int y,int z, const QString& text,GLuint index, const QColor& col , const QFont& font , float scaleX,float scaleY )
{
    //  qDebug() << "DTFT:"<<x << "     " << y;
    qglColor(col);

    if (text.isEmpty()) return;

    glPushMatrix();

    QFontMetrics fm(font);

    QRect rect = fm.boundingRect( text);
    if (rect.width()>wax )rect.setWidth(wax );
    if (rect.height()>way  )rect.setHeight(way);

    QPixmap pixmap( rect.size() );
    pixmap.fill( Qt::black );
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::TextAntialiasing);
    painter.setPen( Qt::white );
    // qDebug() << "AAAAAAAAAAAAAAAAAAA font is underline = " << font.underline(); //-=-=-=
    // qDebug() << "AAAAAAAAAAAAAAAAAAA font is bold = " << font.bold();
    painter.setFont( font );

    painter.drawText( -rect.left(), -rect.top(), text );
    QImage img = pixmap.toImage().convertToFormat(QImage::Format_ARGB32);
    for ( int i = 0; i < img.height(); i++ ) {
        QRgb* rgb = (QRgb*) img.scanLine(i);
        for ( int j = 0; j < img.width(); j++ ) {
            rgb[j] = qRgba( col.red(), col.green(), col.blue(), qRed(rgb[j]) );
        }
    }
    // img = QGLWidget::convertToGLFormat(img);
    // qDebug() << "before drawQImageFromTexture";

    drawQImageFromTexture(x,y,img,index,z, true,scaleX,scaleY,false);

    //glRasterPos3i( x, y, z );
    //glDrawPixels( rect.width(), rect.height(), GL_RGBA, GL_UNSIGNED_BYTE, img.bits() );

    qglColor(Qt::white);

}


void OGLWidget::drawQImage(int x, int y, QImage img, int z)
{
    glPushMatrix();
    QImage res = QGLWidget::convertToGLFormat(img);

    // glEnable(GL_BLEND);
    //   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glRasterPos3i( x, y + res.height(), z );
    glDrawPixels( res.width(), res.height(), GL_RGBA, GL_UNSIGNED_BYTE, res.bits() );
    //glDisable(GL_BLEND);
    /*glMatrixMode( GL_PROJECTION );
    glPopMatrix();
    glMatrixMode( GL_MODELVIEW );*/
    glPopMatrix();
}

void OGLWidget::drawQImageFromTexture(int x, int y, QImage img, GLuint index, int z, bool inverseY, float scaleX, float scaleY,bool centreScaling)
{
    if(img.isNull())
    {
        qDebug() << "BAD IMAGE";
        return;
    }
    glPushMatrix();
    QImage res = img;
    res = QGLWidget::convertToGLFormat(res);

    makeCurrent();
    //  glBindFramebuffer(GL_FRAMEBUFFER , fboWrapper.frameBuffer); // Bind our frame buffer for rendering
    // glClearColor(0.1f, 0.0f, 0.0f, 0.0f); // устанавливаем фоновый цвет

    glBindTexture(GL_TEXTURE_2D, index);
    if (inverseY)
        glTexSubImage2D(GL_TEXTURE_2D, 0, x, way - y, img.width(), img.height(), GL_RGBA, GL_UNSIGNED_BYTE, res.bits());
    else
        glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, img.width(), img.height(), GL_RGBA, GL_UNSIGNED_BYTE, res.bits());
    drawTexture(0, 0, wax, way, index, 0, scaleX, scaleY, z,centreScaling);
    // glBindTexture(GL_TEXTURE_2D,0);
    glBindTexture(GL_TEXTURE_2D, 0);
    // glEnable(GL_BLEND);
    //   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glRasterPos3i( x, y + res.height(), z );
    //glDrawPixels( res.width(), res.height(), GL_RGBA, GL_UNSIGNED_BYTE, res.bits() );
    //glDisable(GL_BLEND);
    /*glMatrixMode( GL_PROJECTION );
    glPopMatrix();
    glMatrixMode( GL_MODELVIEW );*/
    glPopMatrix();
}

QSize OGLWidget::imageLoadedPictureSizeSlot(QSize value)
{
    int width =  this->getWax() ;
    int height = this->getWay();
    int width2 = value.width();
    int height2 = value.height();
    float scaleWidth = (float) width / width2;
    float scaleHeight = (float) height / height2;
    if (scaleWidth < 1 || scaleHeight < 1)
    {
        if (scaleWidth < scaleHeight)
        {
            value.setHeight(value.height()*scaleWidth);
            value.setWidth(value.width()*scaleWidth);
        }
        else
        {
            value.setHeight(value.height()*scaleHeight);
            value.setWidth(value.width()*scaleHeight);
        }
    }
    editingRectangle.rect.setSize(value);
    return value;
}

bool OGLWidget::crossTextV2()
{
    /*
    if(deleteWT != 0 && !symbolPositionList.isEmpty())
    {
        // //qDebug() << deleteWT << "WT == symbolPositionList" << symbolPositionList.size();
        if(deleteWT > symbolPositionList.length())
            deleteWT = symbolPositionList.length();

        QPoint delPos = symbolPositionList.at(symbolPositionList.length() - deleteWT); // -2 is popravka
        if(deleteWT != 1)
        {
            int i = 0;
            while(listWords.at(listWords.size() - i - 1) <= 0x20 && i != listWords.size() - 1)
            {
                i++;
            }
            int pos2 = symbolPositionList.length() - i - 1;

            int n = pos2;
            int m = pos2;
            int lastH = symbolPositionList[n].y();
            deleteWT--;
            while(deleteWT != 0 && n > 0)
            {

                while ( --n > 0 && lastH == symbolPositionList[n].y() && listWords[n] > 0x20 && deleteWT != 0)
                {
                    deleteWT--;
                }
                // //qDebug() << "startNEW with:  "<< hex << listWords[n + 1];
                delPos = symbolPositionList.at(m); // -2 is popravka
                int x2 = delPos.x() + fMetrics->width(listWords[m]);;
                int y2 = delPos.y() - fMetrics->height()/4;
                if( n == 0 )
                    delPos = symbolPositionList.at(n); // -2 is popravka
                else
                    delPos = symbolPositionList.at(n + 1); // -2 is popravka
                int x1 = delPos.x();
                int y1 = delPos.y() - fMetrics->height()/4;
                if(crossWithAnimation)
                {
                    // //qDebug() << "set animation speed";
                    drawAnimationFigure(x1, y1, x2, y2, LINE, 0);
                }
                else
                    drawFigure(x1, y1, x2, y2, LINE, 0);
                lastH = symbolPositionList[n].y();
                while ( n > 0 && listWords[n] <= 0x20)
                    n--;
                m = n;
                if(n != 0)
                    n++;
            }
        }
        else{
            int i = 1;
            while(symbolPositionList.length() != i && listWords.at(listWords.size() - i) == ' ')
                i++;
            delPos = symbolPositionList.at(symbolPositionList.length() - i);
            QColor temp = fillColor;
            setFillColor(QColor("#ff0000"));
            fillText("/", delPos.x(), delPos.y());
            fillText("\\", delPos.x(), delPos.y());
            setFillColor(temp);
        }
        deleteWT = 0;
    }
    else
        deleteWT = 0;
    deleteWT = 0;
    crossWithAnimation = false;*/
}

QPoint OGLWidget::drawWrapText(QString str)
{
    /* busy = true;
    //if(!crossTextV2())
     //   return QPoint(0, 0);
    crossTextV2();
    int width = fMetrics->width(str)*1.125 ;//+ fMetrics->leftBearing(str.at(0)) + fMetrics->rightBearing(str.at(0));

    if(str[0] <= 0x20){
         listChars.clear();
         widthToClean=0;
     }
        if(width + x > maxWidth)
        {
         // //qDebug()<<listChars;

                //fillText("-", x, y);
                //listChars += str;
                //listWords.remove(indexInList,1);
          int clearWidth = fMetrics->width(listChars); //

             // if(y  + scroll*indexRow > this->height() )
             clear(x-widthToClean,y-fMetrics->height(), this->width(),fMetrics->height()*1.25);

             // listStr.push_back( listChars.length() );//не юзається.
              // indexInList++;//не юзається.
          //indexInList+=listChars.length();
             nextRow();
             isLastRow();
            fillText(listChars,fillColor,x,y);
           // listWords += listChars.trimmed();
           // listWords +=listChars;
            //listStr.push_back( listWords.length() );

            listChars.clear();
            widthToClean=0;
            x+=clearWidth;
        }
    isLastRow();
    //fillAnimationText(str, x, y, 6);
    fillText(str,fillColor,x, y);
    QPoint res(x,y);
    listWords += str;
    listChars += str.trimmed();
    x += width;
    widthToClean+=width;
    symbolPositionList.push_back(res);
    //tickTimer.start(delay);
    /*while (tickTimer.isActive()) {
       qApp->processEvents();
    }*/
    // busy = false;
    //   // //qDebug() << "Y: " << y;
    // // //qDebug() << "Y: " << y;
    //return res;*/
}








void OGLWidget::storeMousePos()
{
    if (isMousePress && able_drawing && curStatus!=PLAY){

        if (drawBrushElm->getCoords().length()==0)drawBrushElm->addBrush(m_manager.getCreatedBrush());
        drawBrushElm->addCoord(QPoint(mousePos.x(),mousePos.y()));
        // //qDebug()<<"position stored:"<<QCursor::pos();
    }
}
bool OGLWidget::isShowTextCursor() const
{
    return showTextCursor;
}

void OGLWidget::setShowTextCursor(bool value)
{
    showTextCursor = value;
}





ListControll* OGLWidget::getTimeLine()
{
    return timeLine;
}
PrompterManager* OGLWidget::getPrompterManager(){
    return prompterManager;
}


void OGLWidget::testWrap(int kIndexOfRow)
{
    /* int i = kIndexOfRow;
    int length = stringList.length();
    while(stringList.length() > i)
    {
        QString &str =  stringList[i];
        int width = fMetrics->width(stringList[i]) ;//+ fMetrics->leftBearing(str.at(0)) + fMetrics->rightBearing(str.at(0));
        ////// //qDebug() <<"str:"<< maxWidth;
        if(width > maxWidth)
        {

            int j = stringList[i].size() - 1;
            while( j >= 0 && str[j] > ' ')
            {
             //   // //qDebug() << str[j];
                j--;
            }


            if(j < 0)
                j = stringList[i].length() - 1;

         //   // //qDebug() <<"strSize:    " << stringList.size() << "    SIZE_i    " << stringList[i] << "     " << j;
            nextRow(j, i, false);
            wrapShift.append(getFirstSymbolOfString(i + 1));
                // //qDebug() << "WRAPSHIFT"<< wrapShift;

        }
        i++;
    }*/
}
void OGLWidget::setCellSize(int size){
    windowGrid.setSize(size);
}

void OGLWidget::displayText(QString const &text, QColor color,QFont font)
{

    QImage textimg(1000, 1000, QImage::Format_RGBA8888);
    QPainter painter(&textimg);
    painter.fillRect(0, 0, 1000, 1000, QColor(0,0,0,0));
    painter.setBrush(color);
    painter.setPen(color);
    painter.setFont(font);
    painter.drawText(0, 0, text);

    glBindTexture(GL_TEXTURE_2D, 0);
}
