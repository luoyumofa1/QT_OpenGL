#include "myopengl.h"

unsigned int VBO, VAO,EBO;
unsigned int shaderProgram;

//float vertices[]={
//    //x y z r g b
//    -0.5f,-0.5f,0.0f,1.0f,0.0f,0.0f,
//    0.5f,-0.5f,0.0f,1.0f,0.0f,0.0f,
//    0.0f,0.5f,0.0f,1.0f,0.0f,0.0f,
//};


unsigned int indices[]={
    0,1,3,
    0,1,4,
    0,2,3,
    0,2,4,
    5,1,3,
    5,1,4,
    5,2,3,
    5,2,4,
};

float ww,hh;
ArcBallT arcBall(ww,hh);
ArcBallT* ArcBall =&arcBall;

myopengl::myopengl(QWidget *parent) : QOpenGLWidget(parent)
{
    point m1(0,0.5,0,1,0,0);
    point m2(0.5,0,0,1,0,0);
    point m3(-0.5,0,0,1,0,0);
    point m4(0,-0.5,0,1,0,0);
    point m5(0,0,0.5,0,1,0);
    point m6(0,0,-0.5,0,0,1);


    mypoint.push_back(m1);
    mypoint.push_back(m2);
    mypoint.push_back(m3);
    mypoint.push_back(m4);
    mypoint.push_back(m5);
    mypoint.push_back(m6);

}



myopengl::~myopengl()
{
    makeCurrent();
    glDeleteBuffers(1,&VBO);
    glDeleteBuffers(1,&VAO);
    doneCurrent();
}


void myopengl::initializeGL()
{
    qDebug()<<"myopengl::initializeGL";
    initializeOpenGLFunctions();
    //设定opengl背景颜色
    glClearColor(0.2f,0.3f,0.3f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    //创建VBO与VAO对象，并赋予ID
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    glGenBuffers(1,&EBO);
    //绑定VBO与VAo对象
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    //为当前绑定到target的缓冲区对象创建一个新的数据存储
    //如果data不是NULL，则使用此指针的数据初始化数据存储
    //针对VBO的部分
    glBufferData(GL_ARRAY_BUFFER,mypoint.size()*sizeof(point),&mypoint[0],GL_STATIC_DRAW);

    //针对VAO的部分
    //1 告知显卡如何解析缓冲中的属性值，第三个是是否需要标准化
    glVertexAttribPointer(0, 3, GL_DOUBLE,GL_FALSE, sizeof(point), (void*)0);
    //2 开启VAO管理的第一个属性值
    glEnableVertexAttribArray(0);
    //1 告知显卡如何解析缓冲中的属性值，第三个是是否需要标准化
    glVertexAttribPointer(1, 3, GL_DOUBLE,GL_FALSE, sizeof(point), (void*)(3*sizeof(double)));
    //2 开启VAO管理的第一个属性值
    glEnableVertexAttribArray(1);


    //针对EBO的部分
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);

    //对VAO与VBO解绑
    //glBindBuffer(GL_ARRAY_BUFFER,0);
    //glBindVertexArray(0);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0)

    bool success;
    //顶点着色器
    shadeProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,":/shaders/shapes.vert");
    //片段着色器
    shadeProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,":/shaders/shapes.frag");
    success=shadeProgram.link();
    if(!success){
        qDebug()<<"ERR"<<shadeProgram.log();
    }
}

void myopengl::paintGL()
{
    //这个矩阵的执行顺序是从下往上的

    //matrix.translate(xff,yff,zff);
    //unsigned int time=QTime::currentTime().msec();
    //matrix.rotate(quaternion);
    //matrix.scale(scale);

    if(mypoint.empty()){
        //绘制导入文件数据
        glClearColor(0.2f,0.3f,0.3f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        shadeProgram.setUniformValue("matrix",mScale*mTranslat*mRotate);
        shadeProgram.bind();
        glBindVertexArray(VAO);
        glPointSize(5) ;
        glDrawArrays(GL_POINTS,0,data.size());

    }
    else
    {
        glClearColor(0.2f,0.3f,0.3f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        shadeProgram.setUniformValue("matrix",mScale*mTranslat*mRotate);
        shadeProgram.bind();
        glBindVertexArray(VAO);
        glPointSize(7) ;
        glDrawArrays(GL_POINTS,0,mypoint.size());

        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
        //glDrawElements(GL_TRIANGLES,mypoint.size(),GL_UNSIGNED_INT,&indices);
    }


}


void myopengl::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::RightButton){
        ArcBall->isRClicked = true;
        if(!check){
            xf=event->pos().x();
            yf=event->pos().y();
            check=true;
        }
    }

    if(event->button() & Qt::LeftButton)
    {
        ArcBall->isClicked = true;
    }
}


void myopengl::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::RightButton)
    {
        if(check){
            xl=event->pos().x();
            yl=event->pos().y();
            xff=(xl-xf)/8000;
            yff=(yf-yl)/8000;
            mTranslat.translate(xff,yff,zff);
            update();
        }
    }
    if(event->buttons() & Qt::LeftButton)
    {
        ArcBall->MousePt.s.X = event->pos().x();
        ArcBall->MousePt.s.Y = event->pos().y();
        ArcBall->upstate();

        //使用4*4矩阵旋转
//        QMatrix4x4 mat(ArcBall->Transform.s.M00,ArcBall->Transform.s.M01,ArcBall->Transform.s.M02,ArcBall->Transform.s.M03,ArcBall->Transform.s.M10,ArcBall->Transform.s.M11,ArcBall->Transform.s.M12,ArcBall->Transform.s.M13,ArcBall->Transform.s.M20,ArcBall->Transform.s.M21,ArcBall->Transform.s.M22,ArcBall->Transform.s.M23,ArcBall->Transform.s.M30,ArcBall->Transform.s.M31,ArcBall->Transform.s.M32,ArcBall->Transform.s.M33);
//        mRotate=mat;



//        // 四元数旋转 记得去arcball.cpp改变函数Vector3fAng名称
//        float xx=ArcBall->Quat.s.X;
//        float yy=ArcBall->Quat.s.Y;
//        float zz=ArcBall->Quat.s.Z;
//        float ww=ArcBall->Quat.s.W;//返回值为弧度
//        //QQuaternion m(xx,yy,zz,ww);
//        mRotate.rotate(ww,xx,yy,zz);
//        mRotate.rotate(cos(ww/2),sin(ww/2)*xx,sin(ww/2)*yy,sin(ww/2)*zz);



        update();
    }


}

void myopengl::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() &Qt::RightButton){
        check=false;
        qDebug()<<"this  stop";
    }
    if(event->button() &Qt::LeftButton){
        ArcBall->isClicked = false;
        ArcBall->isRClicked = false;
        ArcBall->upstate();
    }
}

void myopengl::mouseDoubleClickEvent(QMouseEvent *event)
{
    // 如果是鼠标左键按下
    if(event->button() == Qt::LeftButton){

    }
}

void myopengl::wheelEvent(QWheelEvent *event)
{
    int num=event->angleDelta().y();
    Scale+=num/1000.0;
    if(Scale>=3)
        Scale=3;
    if(Scale<=0.06)
        Scale=0.06;
    mScale.setToIdentity();
    mScale.scale(Scale);
    update();
    //qDebug()<<mScale;


}

void myopengl::resizeGL(int w, int h)
{
    //qDebug()<<"myopengl::resizeGL";
    Q_UNUSED(w);
    Q_UNUSED(h);
    ww=w;
    hh=h;
    ArcBall->setBounds(w,h);

}
