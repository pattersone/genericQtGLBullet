#include <unistd.h>     //For getcwd().

#include "MyQOpenGlWidget.h"
#include "mainwindow.h"
#include "obj_parser.h"
#include "assert.h"

//-----------------------------------------------------
MyQOpenGLWidget::MyQOpenGLWidget(QWidget *parent) : QOpenGLWidget(parent),
                                                    vao(), vbo_pos(), vbo_n(), vbo_tc(), vbo_pos_simple(), shaderProg(0),
                                                    viewMat(), modelMat(), projMat(), viewMat_unifLoc(-1),
                                                    modelMat_unifLoc(-1), projMat_unifLoc(-1), vertexCount(0)
{
    QSurfaceFormat glFormat;
    glFormat.setDepthBufferSize(24);
    glFormat.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    glFormat.setSwapInterval(1);
    glFormat.setVersion(4,1);
    glFormat.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(glFormat);         // This should be here but is required at least on Mac.
    setFormat(glFormat);                                // Must be called before the glWidget or the MainWindow (its parent) is shown.

    //TODO: Resize window according to MainWindow's size.
}

//-----------------------------------------------------
MyQOpenGLWidget::~MyQOpenGLWidget()
{
    makeCurrent();      //Ensures OpenGL framebuffer object is bound to the rendering context.

    delete shaderProg;
    vbo_pos.destroy();
    vbo_n.destroy();
    vbo_tc.destroy();
    vao.destroy();

    doneCurrent();      //Releases the context.
}

//-----------------------------------------------------
//Set up rendering context, load shaders, initialize matrices that will be sent to the shaders (PVM).
void MyQOpenGLWidget::initializeGL()
{
    QOpenGLFunctions *functs = QOpenGLContext::currentContext()->functions();
    functs->glClearColor(0.51, 0.5, 0.52, 1.0);

    initShaders();
    initVertexBuffers();

    QVector3D eyePt, viewPt, up;
    eyePt.setX(0.0);  eyePt.setY(3.0);   eyePt.setZ(12.0);
    viewPt.setX(0.0);  viewPt.setY(3.0);  viewPt.setZ(0.0);
    up.setX(0.0);      up.setY(1.0);      up.setZ(0.0);

    viewMat.lookAt(eyePt, viewPt, up);
    projMat.perspective(60.0, width()/height(), 0.1, 100.0);
}

//-----------------------------------------------------
void MyQOpenGLWidget::checkCwd()
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    { qDebug() << "Current working dir: " << cwd; }
    else
    { qCritical() << "getcwd() call error."; }
}

//-----------------------------------------------------
void MyQOpenGLWidget::initShaders()
{
    checkCwd();

    delete shaderProg;    //Causes memory leak.
    modelMat_unifLoc = -1;
    viewMat_unifLoc = -1;
    projMat_unifLoc = -1;
    QString vsName, fsName;

    vsName = QString("../shaders/vs.glsl");
    fsName = QString("../shaders/fs.glsl");


    shaderProg = new QOpenGLShaderProgram(this);

    if (shaderProg->addShaderFromSourceFile(QOpenGLShader::Vertex, vsName))
        { qDebug() << "Loaded vertex shader file."; }
    if (shaderProg->addShaderFromSourceFile(QOpenGLShader::Fragment, fsName))
        { qDebug() << "Loaded fragment shader file."; }
    if (shaderProg->link())
        { qDebug() << "Linked shader program."; }
    qDebug() << "Shader prog log: " << shaderProg->log();

    modelMat_unifLoc = shaderProg->uniformLocation(QString("modelMat"));
    if (modelMat_unifLoc == -1)
        { qDebug() << "Shader uniform 'modelMat' cannot be found: " << modelMat_unifLoc << ". Note that the compiler will not assign a location if the uniform variable is not used."; }
    viewMat_unifLoc = shaderProg->uniformLocation(QString("viewMat"));
    if (viewMat_unifLoc == -1)
        { qDebug() << "Shader uniform 'viewMat' cannot be found: " << viewMat_unifLoc; }
    projMat_unifLoc = shaderProg->uniformLocation(QString("projMat"));
    if (projMat_unifLoc == -1)
     { qDebug() << "Shader uniform 'projMat' cannot be found: " << projMat_unifLoc; }

}

//-----------------------------------------------------
void MyQOpenGLWidget::initVertexBuffers()
{

    GLfloat* vp = NULL;    // array of vertex points
    GLfloat* vn = NULL;    // array of vertex normals
    GLfloat* vt = NULL;    // array of texture coordinates

    checkCwd();

    assert (load_obj_file("../models/teapot.obj", vp, vt, vn, vertexCount));

    if (!vao.isCreated())
    { vao.create(); }
    if (vao.isCreated())
    { vao.bind(); }

    vbo_pos.create();
    vbo_pos.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    vbo_pos.bind();
    vbo_pos.allocate(vp, 3*vertexCount*sizeof(GLfloat));

    vbo_n.create();
    vbo_n.bind();
    vbo_n.allocate(vn, 3*vertexCount*sizeof(GLfloat));

    vbo_tc.create();
    vbo_tc.bind();
    vbo_tc.allocate(vt, 2*vertexCount*sizeof(GLfloat));

    shaderProg->bind();

    vbo_pos.bind();
    shaderProg->enableAttributeArray("vertexPosition");
    shaderProg->setAttributeArray("vertexPosition", GL_FLOAT, 0, 3);

    vbo_n.bind();
    shaderProg->enableAttributeArray("vertexNormal");
    shaderProg->setAttributeArray("vertexNormal", GL_FLOAT, 0, 3);

    vbo_tc.bind();
    shaderProg->enableAttributeArray("vertexTexcoord");
    shaderProg->setAttributeArray("vertexTexcoord", GL_FLOAT, 0, 2);

    delete vp;
    delete vn;
    delete vt;

}

//-----------------------------------------------------
//Update projection matrix.
void MyQOpenGLWidget::resizeGL(int w, int h)
{
    qDebug() << "resizeGL(): w: " << w << ", h: " << h;
    projMat.setToIdentity();                                //CLEAR OUT OLD INFO FIRST!
    projMat.perspective(60.0, w/h, 0.1, 100.0);
}

//-----------------------------------------------------
void MyQOpenGLWidget::paintGL()
{
    QOpenGLFunctions *functs = QOpenGLContext::currentContext()->functions();

    makeCurrent();              //Ensures OpenGL framebuffer object is bound to the rendering context.

//    qDebug() << "paintGL()";
//    qDebug() << "View mat: " << viewMat;
//    qDebug() << "Model mat: " << modelMat;
//    qDebug() << "Proj mat: " << projMat;

   // functs->glClear(GL_COLOR_BUFFER_BIT);
    functs->glEnable(GL_DEPTH_TEST);   // enable depth-testing
    functs->glDepthFunc(GL_LESS);      // depth-testing interprets a smaller value as "closer"
    functs->glEnable(GL_CULL_FACE);    // cull face
    functs->glCullFace(GL_BACK);       // cull back face
    functs->glFrontFace(GL_CCW);       // set counter-clock-wise vertex order to mean the front

    shaderProg->bind();
    shaderProg->setUniformValue(viewMat_unifLoc, viewMat);
    shaderProg->setUniformValue(modelMat_unifLoc, modelMat);
    shaderProg->setUniformValue(projMat_unifLoc, projMat);

    vao.bind();
    functs->glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    vao.release();

    doneCurrent();              //Releases the context.
}

//-----------------------------------------------------
void MyQOpenGLWidget::updateModelMat(float scale, btVector3& transl)
{
    modelMat.setToIdentity();
    modelMat.translate(transl.x(), transl.y(), transl.z());
}

//-----------------------------------------------------
void MyQOpenGLWidget::resetModelMat()
{
    modelMat.setToIdentity();
}
