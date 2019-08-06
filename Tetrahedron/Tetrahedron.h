#ifndef TETRAHEDRON_H
#define TETRAHEDRON_H

#include <QMainWindow>
#include <QGLWidget>

namespace Ui {
class Tetrahedron;
}

class Tetrahedron : public QGLWidget
{
    Q_OBJECT

public:
    explicit Tetrahedron(QWidget *parent = nullptr);
    ~Tetrahedron();
protected:
    void paintGL();
    void mousePressEvent(QMouseEvent *e);
    void mouseDoubleClickEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);;
    void initializeGL();
    void resizeGL(int width, int height);

private:
    Ui::Tetrahedron *ui;
    int faceAtPosition(const QPoint &pos);

    GLfloat rotationX;
    GLfloat rotationY;
    GLfloat rotationZ;
    QColor faceColor[4];
    QPoint lastPos;
    void draw();
    bool leftDown;
    bool rightDown;
};

#endif // TETRAHEDRON_H
