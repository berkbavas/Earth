#include "Controller.h"
#include "Window.h"

#include <QApplication>
#include <QDebug>
#include <QDir>

Controller::Controller(QObject *parent)
    : QObject(parent)
    , mImGuiWantsMouseCapture(false)
    , mSuccess(true)
    , mUpdateCamera(false)
    , mDistance(40.0)
    , mRoll(0)
    , mHeading(0)
    , mTilt(0)
{}

void Controller::init()
{
    initializeOpenGLFunctions();
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);

    mShaderManager = ShaderManager::instance();
    mRendererManager = RendererManager::instance();

    mSun = Sun::instance();
    mEarth = Earth::instance();

    mManagers << mShaderManager;
    mManagers << mRendererManager;
    for (const auto &manager : qAsConst(mManagers))
    {
        if (!manager->init())
        {
            mSuccess = false;
            return;
        }
    }

    mCamera = new DummyCamera;
    mCamera->setPosition(QVector3D(0, 0, mDistance));
    mCamera->setVerticalFov(60.0f);
    mCamera->setZNear(0.1f);
    mCamera->setZFar(100000.0f);

    mRendererManager->setCamera(mCamera);

    mMouse.pressedButton = Qt::NoButton;
    mMouse.dx = 0;
    mMouse.dy = 0;
    mMouse.dz = 0;
    mMouse.x = 0;
    mMouse.y = 0;
    mMouse.z = 0;

    mEarth->setPosition(QVector3D(0, 0, 0));
}

void Controller::render(float ifps)
{
    if (!mSuccess)
        return;

    if (mUpdateCamera)
    {
        //        auto rotation = mCamera->rotation();
        //        rotation = QQuaternion::fromAxisAndAngle(QVector3D(0, 1, 0), mMouse.dx * ifps * (mDistance - 10)) * rotation;
        //        rotation = rotation * QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0), mMouse.dy * ifps * (mDistance - 10));

        //        auto position = rotation * QVector3D(0, 0, mDistance);

        //        mCamera->setRotation(rotation);
        //        mCamera->setPosition(position);

        //        mMouse.dx = 0.0f;
        //        mMouse.dy = 0.0f;
        //        mMouse.dz = 0.0f;
        //        mUpdateCamera = false;

        mRoll += (mDistance - 10) * mMouse.dz * ifps;
        mHeading += (mDistance - 10) * mMouse.dx * ifps;
        mTilt += (mDistance - 10) * mMouse.dy * ifps;

        auto rotation = QQuaternion::fromAxisAndAngle(QVector3D(0, 0, 1), mRoll);

        rotation = QQuaternion::fromAxisAndAngle(QVector3D(0, 1, 0), mHeading);
        rotation = rotation * QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0), mTilt);

        auto position = rotation * QVector3D(0, 0, mDistance);
        mCamera->setRotation(rotation);
        mCamera->setPosition(position);

        mMouse.dx = 0.0f;
        mMouse.dy = 0.0f;
        mMouse.dz = 0.0f;
        mUpdateCamera = false;
    }

    //    if (mUpdateCameraRotation)
    //    {
    //        auto rotation = QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0), -30 * mMouse.dw * ifps) * mCamera->rotation();
    //        auto position = QVector3D(0, 0, 10) + rotation * QVector3D(0, 0, 40);

    //        mCamera->setRotation(rotation);
    //        mCamera->setPosition(position);

    //        mMouse.dw = 0.0f;
    //        mUpdateCameraRotation = false;
    //    }

    mRendererManager->render(ifps);

    // ImGUI stuff
    mImGuiWantsMouseCapture = ImGui::GetIO().WantCaptureMouse;
    mImGuiWantsKeyboardCapture = ImGui::GetIO().WantCaptureKeyboard;

    QtImGui::newFrame();

    mSun->drawGUI();
    mEarth->drawGUI();

    mRendererManager->drawGUI();

    glViewport(0, 0, mWindow->width(), mWindow->height());
    ImGui::Render();
    QtImGui::render();
}

void Controller::wheelMoved(QWheelEvent *event)
{
    if (!mSuccess)
        return;

    if (mImGuiWantsMouseCapture)
        return;

    if (event->angleDelta().y() < 0)
        mDistance = 1.05 * mDistance;

    if (event->angleDelta().y() > 0)
        mDistance = mDistance / 1.05;

    mDistance = qBound(10.0f + 2 * mCamera->zNear(), mDistance, 1000.0f);

    mUpdateCamera = true;
}

void Controller::mousePressed(QMouseEvent *event)
{
    if (!mSuccess)
        return;

    if (mImGuiWantsMouseCapture)
        return;

    if (event->button() == Qt::LeftButton)
    {
        mMouse.x = event->position().x();
        mMouse.y = event->position().y();
        mMouse.pressedButton = Qt::LeftButton;
    }

    if (event->button() == Qt::MiddleButton)
    {
        mMouse.z = event->position().y();
        mMouse.pressedButton = Qt::MiddleButton;
    }

    if (event->button() == Qt::RightButton)
    {
        mMouse.w = event->position().y();
        mMouse.pressedButton = Qt::RightButton;
    }

    mRendererManager->mousePressed(event);
}

void Controller::mouseReleased(QMouseEvent *event)
{
    if (!mSuccess)
        return;

    mMouse.pressedButton = Qt::NoButton;

    mRendererManager->mouseReleased(event);
}

void Controller::mouseMoved(QMouseEvent *event)
{
    if (!mSuccess)
        return;

    if (mImGuiWantsMouseCapture)
        return;

    if (mMouse.pressedButton == Qt::LeftButton)
    {
        mMouse.dx += mMouse.x - event->position().x();
        mMouse.dy += mMouse.y - event->position().y();

        mMouse.x = event->position().x();
        mMouse.y = event->position().y();

        mUpdateCamera = true;
    }

    if (mMouse.pressedButton == Qt::MiddleButton)
    {
        mMouse.dz += mMouse.z - event->position().y();
        mMouse.z = event->position().y();

        mUpdateCamera = true;
    }

    if (mMouse.pressedButton == Qt::RightButton)
    {
        mMouse.dw += mMouse.w - event->position().y();
        mMouse.w = event->position().y();
        mUpdateCamera = true;
    }

    mRendererManager->mouseMoved(event);
}

void Controller::keyPressed(QKeyEvent *event)
{
    if (!mSuccess)
        return;

    if (mImGuiWantsKeyboardCapture)
        return;
}

void Controller::keyReleased(QKeyEvent *event)
{
    if (!mSuccess)
        return;
}

void Controller::resize(int w, int h)
{
    if (!mSuccess)
        return;

    mWindow->makeCurrent();
    mRendererManager->resize(w, h);
    mCamera->resize(w, h);
    mWindow->doneCurrent();
}

void Controller::mouseDoubleClicked(QMouseEvent *)
{
    if (!mSuccess)
        return;
}

void Controller::setWindow(Window *newWindow)
{
    mWindow = newWindow;
}
