#ifndef CAMERA_H
#define CAMERA_H

#include "Node.h"

#include <QMatrix4x4>
#include <QMouseEvent>
#include <QTimer>

class Camera : public Node
{
    Q_OBJECT
protected:
    explicit Camera(QObject *parent = nullptr);
    virtual ~Camera();

signals:
    void activeChanged(bool active);

public:
    virtual QMatrix4x4 getVP();
    virtual QMatrix4x4 projection() = 0;

    virtual void mouseDoubleClicked(QMouseEvent *) = 0;
    virtual void mousePressed(QMouseEvent *) = 0;
    virtual void mouseReleased(QMouseEvent *) = 0;
    virtual void mouseMoved(QMouseEvent *) = 0;
    virtual void wheelMoved(QWheelEvent *) = 0;
    virtual void keyPressed(QKeyEvent *) = 0;
    virtual void keyReleased(QKeyEvent *) = 0;
    virtual void update(float) = 0;
    virtual void reset() = 0;

    virtual void resize(int width, int height);

    bool active() const;
    void setActive(bool newActive);

    int width() const;
    void setWidth(int newWidth);

    int height() const;
    void setHeight(int newHeight);

    float zNear() const;
    void setZNear(float newZNear);

    float zFar() const;
    void setZFar(float newZFar);

protected:
    bool mActive;
    int mWidth;
    int mHeight;
    float mZNear;
    float mZFar;
};

#endif // CAMERA_H
