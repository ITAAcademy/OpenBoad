#include "shaderprogramwrapper.h"


QString ShaderProgramWrapper::getInfo() const
{
    return info;
}
ShaderProgramWrapper::ShaderProgramWrapper(OGLWidget *pWidget)
{
    parentWidget=pWidget;
    glf=parentWidget->getOglFuncs();
}



ShaderProgramWrapper::ShaderProgramWrapper()
{

}
bool ShaderProgramWrapper::save(QDataStream &stream)
{
    stream << fragCode << vertCode;
}
bool ShaderProgramWrapper::load(QDataStream &stream)
{
    stream >> fragCode;
    stream >> vertCode;
    initShader(fragCode,vertCode,false);
    if (!isInited())return false;
        return true;
}

ShaderProgramWrapper::~ShaderProgramWrapper()
{
     //qDebug() <<"ShaderProgramWrapper::~ShaderProgramWrapper()";
     bool exp = errorStatus==0;
    // qDebug() << "succ";
    if(exp)
    {
        //qDebug() <<"~ShaderProgramWrapper begin";
        //glf->useShader(0);
        glf->glDeleteShader(ShaderProgram);

    }
   // qDebug() <<"~ShaderProgramWrapper end";
}

int ShaderProgramWrapper::initShader(QString fShaderFilePath, QString vShaderFilePath, bool isFilePath)
{
    inited = false;
    //glf->glUseProgram(0);
    glf->glDeleteShader(ShaderProgram);

    QString fragmentShaderCode;
    QString vertexShaderCode;
    fragmentShaderCode=fShaderFilePath;
    vertexShaderCode=vShaderFilePath;
if (isFilePath)
{
    QFile fragmentShadeFile(fShaderFilePath);

    QFile vertexShaderFile(vShaderFilePath);


    errorStatus=0;
             if(fragmentShadeFile.open(QIODevice::ReadOnly | QIODevice::Text))
             {
                 QTextStream in(&fragmentShadeFile);
                 fragmentShaderCode = in.readAll();
                  fragmentShadeFile.close();
                 //qDebug() << "fragmentShaderCode:"<<fragmentShaderCode;
             }
            else
             {
                qDebug() << "error on fragmentShader.glsl resource open";
                errorStatus=-1;
                return -1;

             }


             if(vertexShaderFile.open(QIODevice::ReadOnly | QIODevice::Text))
             {
                 QTextStream in(&vertexShaderFile);
                 vertexShaderCode = in.readAll();
                  vertexShaderFile.close();
             }
             else
             {
                    qDebug() << "error on vertexShader.glsl resource open";
                    errorStatus=-1;
                    return -1;

             }

          fragCode = fragmentShaderCode;
         vertCode=vertexShaderCode;
}

              ShaderProgram = glf->glCreateProgram();
              GLuint vertexShaderObj = glf->glCreateShader(GL_VERTEX_SHADER);
              GLuint fragmentShaderObj = glf->glCreateShader(GL_FRAGMENT_SHADER);

              const GLchar* p[1];
              p[0] = vertexShaderCode.toLatin1().constData();
              GLint Lengths[1];
              Lengths[0]= vertexShaderCode.length();
              glf->glShaderSource(vertexShaderObj, 1, p, Lengths);

              const GLchar* p2[1];
              p2[0] = fragmentShaderCode.toLatin1().constData();
              GLint Lengths2[1];
              Lengths2[0]= fragmentShaderCode.length();
              glf->glShaderSource(fragmentShaderObj, 1, p2, Lengths2);

              glf->glCompileShader(vertexShaderObj);
              glf->glCompileShader(fragmentShaderObj);

              //Get compile status and errors detected by compiler
              GLint success;
              glf->glGetShaderiv(vertexShaderObj, GL_COMPILE_STATUS, &success);
              if (!success) {
                  GLchar infoLog[1024];
                  glf->glGetShaderInfoLog(vertexShaderObj, sizeof(infoLog), NULL, infoLog);
                   qDebug() <<"Error compiling shader type:" << "GL_VERTEX_SHADER" << infoLog;
                   info = "Error compiling shader type: GL_VERTEX_SHADER   " + QString(infoLog);
                    errorStatus=-1;
                  return -1;

              }
              glf->glGetShaderiv(fragmentShaderObj, GL_COMPILE_STATUS, &success);
              if (!success) {
                  GLchar infoLog[1024];
                  glf->glGetShaderInfoLog(fragmentShaderObj, sizeof(infoLog), NULL,infoLog);
                   qDebug() <<"Error compiling shader type:" << "GL_FRAGMENT_SHADER" << infoLog;
                    info = "Error compiling shader type: GL_FRAGMENT_SHADER   " + QString(infoLog);
                    errorStatus=-1;
                  return -1;

              }

         glf->glAttachShader(ShaderProgram,vertexShaderObj);
          glf->glAttachShader(ShaderProgram,fragmentShaderObj);
        glf->glLinkProgram(ShaderProgram);
        glf->glDeleteShader(vertexShaderObj);
        glf->glDeleteShader(fragmentShaderObj);
        glf->glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &success);
        GLchar errorLog[1024];
        //if (success == 0) {
        if (success == 0) {
            glf->glGetProgramInfoLog(ShaderProgram, sizeof(errorLog), NULL, errorLog);
             qDebug() << "Error linking shader program:" << errorLog;
             info = "Error linking shader program   " + QString(errorLog);
              errorStatus=-1;
            return -1;

        }
        else
            //qDebug() << "Linking shader program success";
        glf->glValidateProgram(ShaderProgram);
          //qDebug() << "Shader supported:"<<parentWidget->isShaderSupported();
            //glLinkProgram(program);
            //glUseProgram(program);
          inited = true;
    return 0; // 0

}

bool ShaderProgramWrapper::setUniform(QString name, QVariant value)
{
    GLint unifrom = glf->glGetUniformLocation( ShaderProgram, name.toLatin1().data());
    if(unifrom == -1)
        return false;

    switch (value.type()) {
    case QMetaType::Int:
        //qDebug() << "set uniform int";
        glf->glUniform1i(unifrom, value.toInt());
        break;
    case QMetaType::Double:
    case QMetaType::Float:
        //qDebug() << "set uniform double\float";
        glf->glUniform1f(unifrom, value.toFloat());
        break;
    case QMetaType::QVector2D:
{
        //QVector<int> arr = value.value<QVector<int>>();
        QVector2D arr = value.value<QVector2D>();
        //QString vectorType = typeid(arr.[0]).name();
        //if(vectorType.compare("int"))
        glf->glUniform2f(unifrom,arr.x(),arr.y() );
}
         break;
    case QMetaType::QVector3D:
{
        //QVector<int> arr = value.value<QVector<int>>();
        QVector3D arr = value.value<QVector3D>();
        //QString vectorType = typeid(arr.[0]).name();
        //if(vectorType.compare("int"))
        glf->glUniform3f(unifrom,arr.x(),arr.y(),arr.z() );
    }
        break;
    case QMetaType::QVector4D:
{
        //QVector<int> arr = value.value<QVector<int>>();
        QVector4D arr = value.value<QVector4D>();
        //QString vectorType = typeid(arr.[0]).name();
        //if(vectorType.compare("int"))
        glf->glUniform4f(unifrom,arr.x(),arr.y(),arr.z(),arr.w() );
}
        break;
case QMetaType::Bool:
        glf->glUniform1i(unifrom, value.toBool());
        break;
    default:
        return false;
    }
    return true;

}
bool ShaderProgramWrapper::setUniform(QString name, float val1,float val2){
    setUniform(name,QVector2D(val1,val2));
}

OGLWidget *ShaderProgramWrapper::getParentWidget()
{
 return parentWidget;
}

GLuint ShaderProgramWrapper::getShaderProgram()
{
    return ShaderProgram;
}

bool ShaderProgramWrapper::isInited()
{
    return inited;
}


