#include "Tetrahedron.h"
#include "ui_Tetrahedron.h"
#include <gl/GLU.h>
#include <QMouseEvent>
#include <QColorDialog>

Tetrahedron::Tetrahedron(QWidget *parent) :
    QGLWidget(parent),
    ui(new Ui::Tetrahedron)
{
    setFormat(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer));

    rotationX = -21.0;
    rotationY = -57.0;
    rotationZ = 0.0;
    faceColor[0] = Qt::red;
    faceColor[1] = Qt::green;
    faceColor[2] = Qt::blue;
    faceColor[3] = Qt::yellow;
    leftDown = false;
    rightDown = false;
}

Tetrahedron::~Tetrahedron()
{
    delete ui;
}

void Tetrahedron::initializeGL()
{
    qglClearColor(Qt::black);
    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}

void Tetrahedron::resizeGL(int width, int height)
{
    glViewport(0,0,width,height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLfloat x = GLfloat(width)/height;
    glFrustum(-x,+x,-1.0,+1.0,4.0,15.0);
    glMatrixMode(GL_MODELVIEW);
}

void Tetrahedron::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    draw();
}

void Tetrahedron::draw()
{
    static const GLfloat p1[3]= {0.0,-1.0,+2.0};
    static const GLfloat p2[3]={+1.73205081,-1.0,-1.0};
    static const GLfloat p3[3]={-1.73205081,-1.0,-1.0};
    static const GLfloat p4[3]={0.0,+2.0,0.0};

    static const GLfloat *const coords[4][3]={
        {p1,p2,p3},{p1,p3,p4},{p1,p4,p2},{p2,p4,p3}
    };
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0,0.0,-10.0);
    glRotatef(rotationX,1.0,0.0,0.0);
    glRotatef(rotationY,0.0,1.0,0.0);
    glRotatef(rotationZ,0.0,0.0,1.0);

    for(int i=0;i<4;i++){
        glLoadName(i);
        glBegin(GL_TRIANGLES);
        qglColor(faceColor[i]);
        for(int j=0;j<3;j++){
            glVertex3f(coords[i][j][0],coords[i][j][1],coords[i][j][2]);
        }
        glEnd();
    }
}

void Tetrahedron::mouseMoveEvent(QMouseEvent *e)
{
    GLfloat dx =GLfloat(e->x() - lastPos.x())/width();
    GLfloat dy = GLfloat(e->y()-lastPos.y())/height();
    if(leftDown){
        rotationX+=180*dy;
        rotationY+=180*dx;
        updateGL();
    }else if(rightDown){
        rotationX+=180*dy;
        rotationZ+=180*dx;
        updateGL();
    }
    lastPos = e->pos();
}

void Tetrahedron::mousePressEvent(QMouseEvent *e)
{
    lastPos = e->pos();
    if (e->button() & Qt::LeftButton) {
        leftDown = true;
        rightDown = false;
    }
    if (e->button()&Qt::RightButton) {
        rightDown = true;
        leftDown = false;
    }
}

void Tetrahedron::mouseDoubleClickEvent(QMouseEvent *e)
{
    int face = faceAtPosition(e->pos());
    if(face !=-1){
        QColor color = QColorDialog::getColor(faceColor[face],this);
        if(color.isValid()){
            faceColor[face]=color;
            updateGL();
            resizeGL(width(), height());
        }
    }
}

int Tetrahedron::faceAtPosition(const QPoint &pos)
{
    const int MaxSize = 512;
    GLuint buffer[MaxSize];
    GLint viewport[4];

    makeCurrent();
    glGetIntegerv(GL_VIEWPORT,viewport);
    glSelectBuffer(MaxSize,buffer);
    glRenderMode(GL_SELECT);

    glInitNames();
    glPushName(0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPickMatrix(GLdouble(pos.x()),GLdouble(viewport[3]-pos.y()),5.0,5.0,viewport);
    GLfloat x = GLfloat(width())/height();
    glFrustum(-x,x,-1.0,1.0,4.0,15.0);
    draw();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    if(!glRenderMode(GL_RENDER)){
        return -1;
    }
    return buffer[3];
}

void Tetrahedron::mouseReleaseEvent(QMouseEvent *e)
{
    leftDown = false;
    rightDown = false;
}