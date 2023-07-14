#ifndef MYOPENGL_H
#define MYOPENGL_H

#include<QOpenGLWidget>
#include<QOpenGLFunctions_3_3_Core>
#include<qdebug.h>
#include<QMouseEvent>
#include<QWheelEvent>
#include <QQuaternion>
#include<Qvector>
#include"point.h"
#include<QOpenGLShaderProgram>
#include<QTimer>
#include<ctime>
#include<qdebug.h>
#include"arcball.h"
class myopengl:public QOpenGLWidget,QOpenGLFunctions_3_3_Core
{
public:
    explicit myopengl(QWidget *parent = nullptr);
    ~myopengl();

public:
    virtual void initializeGL();
    virtual void paintGL();
    virtual void resizeGL(int w, int h);

    void mousePressEvent(QMouseEvent* event);//鼠标按下事件
    void mouseMoveEvent(QMouseEvent *event);//鼠标移动事件
    void mouseReleaseEvent(QMouseEvent *event);//鼠标释放事件
    void mouseDoubleClickEvent(QMouseEvent *event);//鼠标双击事件
    void wheelEvent(QWheelEvent *event);//滚轮事件

public:
    QVector<point>mypoint;
    QVector<QVector<point>>da;
    QVector<point>data;
    QVector<point>dc1;



public:
    QOpenGLShaderProgram shadeProgram;
    QTimer timer;
public:
    //操作所需要的平移，旋转，缩放矩阵
    QMatrix4x4 matrix,mTranslat,mScale,mRotate;
    float Scale=1.0f;
    float xff,yff,zff,xf,xl,yf,yl,zf,zl;
    double xmax=0,ymax=0,zmax=0;
    bool check=false;
    QQuaternion quaternion;

};

#endif // MYOPENGL_H
