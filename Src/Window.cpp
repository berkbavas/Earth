#include "Window.h"
#include "Controller.h"

#include <QDateTime>
#include <QKeyEvent>

#include <QDebug>

Earth::Window::Window(QWindow *parent)
    : QOpenGLWindow(QOpenGLWindow::UpdateBehavior::NoPartialUpdate, parent)

{
    QSurfaceFormat format = QSurfaceFormat::defaultFormat();
    format.setMajorVersion(4);
    format.setMinorVersion(3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setSamples(4);
    format.setSwapInterval(1);
    setFormat(format);

    connect(this, &QOpenGLWindow::frameSwapped, this, [=]() { update(); });
}

void Earth::Window::initializeGL()
{
    initializeOpenGLFunctions();

    mCurrentTime = QDateTime::currentMSecsSinceEpoch();
    mPreviousTime = mCurrentTime;

    QtImGui::initialize(this);

    mController = new Controller;
    mController->SetWindow(this);
    mController->Init();
}

void Earth::Window::resizeGL(int w, int h)
{
    glViewport(0, 0, width(), height());

    mController->Resize(w, h);
}

void Earth::Window::paintGL()
{
    mCurrentTime = QDateTime::currentMSecsSinceEpoch();
    float ifps = (mCurrentTime - mPreviousTime) * 0.001f;
    mPreviousTime = mCurrentTime;

    mController->Render(ifps);
}

void Earth::Window::keyPressEvent(QKeyEvent *event)
{
    mController->KeyPressed(event);
}

void Earth::Window::keyReleaseEvent(QKeyEvent *event)
{
    mController->KeyReleased(event);
}

void Earth::Window::mousePressEvent(QMouseEvent *event)
{
    mController->MousePressed(event);
}

void Earth::Window::mouseReleaseEvent(QMouseEvent *event)
{
    mController->MouseReleased(event);
}

void Earth::Window::mouseMoveEvent(QMouseEvent *event)
{
    mController->MouseMoved(event);
}

void Earth::Window::wheelEvent(QWheelEvent *event)
{
    mController->WheelMoved(event);
}
