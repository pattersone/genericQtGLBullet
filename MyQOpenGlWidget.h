#ifndef MYQOPENGLWIDGET_H
#define MYQOPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include "btBulletDynamicsCommon.h"

class MyQOpenGLWidget : public QOpenGLWidget
{

public:
    MyQOpenGLWidget(QWidget *parent);
    ~MyQOpenGLWidget();
    void updateModelMat(float scale, btVector3& transl);
    void resetModelMat();
    void initShaders();
    void initVertexBuffers();

protected:
    void initializeGL();                //Virtual functions to implement.
    void resizeGL(int w, int h);
    void paintGL();

private:
    void checkCwd();

    QOpenGLVertexArrayObject vao;
    QOpenGLBuffer vbo_pos, vbo_n, vbo_tc, vbo_pos_simple;
    QOpenGLShaderProgram *shaderProg;

    QMatrix4x4 viewMat, modelMat, projMat;      //These will be passed to the shaders. They are column-major.
    int viewMat_unifLoc, modelMat_unifLoc, projMat_unifLoc;
    int vertexCount;
};

#endif //MYQOPENGLWIDGET_H