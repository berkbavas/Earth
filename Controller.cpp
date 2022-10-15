#include "Controller.h"
#include "Window.h"

#include <QApplication>
#include <QDebug>
#include <QDir>

Controller::Controller(QObject *parent)
    : QObject(parent)
    , mImGuiWantsMouseCapture(false)
    , mSuccess(true)
    , mZoomLevel(100)
    , mUpdate(false)
    , mDistance(40.0f)
    , mTilt(0.0f)
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
    mCamera->setZNear(0.01f);
    mCamera->setZFar(100000.0f);

    mRendererManager->setCamera(mCamera);

    mEarth->setPosition(QVector3D(0, 0, 0));
}

void Controller::render(float ifps)
{
    if (!mSuccess)
        return;

    if (mUpdate)
    {
        // Earth
        {
            auto rotation = mEarth->rotation();
            rotation = QQuaternion::fromAxisAndAngle(QVector3D(0, 0, 1), -mMouse.mDz * ifps * qMax(2.0f, mDistance - 10)) * rotation;
            rotation = QQuaternion::fromAxisAndAngle(QVector3D(0, 1, 0), -mMouse.mDx * ifps * (mDistance - 10)) * rotation;
            rotation = QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0), -mMouse.mDy * ifps * (mDistance - 10)) * rotation;
            mEarth->setRotation(rotation);
        }

        // Camera
        {
            mTilt -= mMouse.mDw * ifps * 30;

            mTilt = qBound(0.0f, mTilt, 89.0f);

            auto rotation = QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0), mTilt);
            auto position = QVector3D(0, 0, 10) + rotation * QVector3D(0, 0, mDistance - 10);

            mCamera->setRotation(rotation);
            mCamera->setPosition(position);
        }

        mMouse.mDx = 0.0f;
        mMouse.mDy = 0.0f;
        mMouse.mDz = 0.0f;
        mMouse.mDw = 0.0f;
        mUpdate = false;
    }

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
        mDistance = mDistance / 0.95;

    if (event->angleDelta().y() > 0)
        mDistance = mDistance * 0.95;

    mDistance = qBound(10.0f + 2 * mCamera->zNear(), mDistance, 1000.0f);

    mUpdate = true;
}

void Controller::mousePressed(QMouseEvent *event)
{
    if (!mSuccess)
        return;

    if (mImGuiWantsMouseCapture)
        return;

    if (event->button() == Qt::LeftButton)
    {
        mMouse.mX = event->position().x();
        mMouse.mY = event->position().y();
        mMouse.mPressedButton = Qt::LeftButton;
    }

    if (event->button() == Qt::MiddleButton)
    {
        mMouse.mZ = event->position().y();
        mMouse.mPressedButton = Qt::MiddleButton;
    }

    if (event->button() == Qt::RightButton)
    {
        mMouse.mW = event->position().y();
        mMouse.mPressedButton = Qt::RightButton;
    }

    mRendererManager->mousePressed(event);
}

void Controller::mouseReleased(QMouseEvent *event)
{
    if (!mSuccess)
        return;

    mMouse.mPressedButton = Qt::NoButton;

    mRendererManager->mouseReleased(event);
}

void Controller::mouseMoved(QMouseEvent *event)
{
    if (!mSuccess)
        return;

    if (mImGuiWantsMouseCapture)
        return;

    if (mMouse.mPressedButton == Qt::LeftButton)
    {
        mMouse.mDx += mMouse.mX - event->position().x();
        mMouse.mDy += mMouse.mY - event->position().y();

        mMouse.mX = event->position().x();
        mMouse.mY = event->position().y();

        mUpdate = true;
    }

    if (mMouse.mPressedButton == Qt::MiddleButton)
    {
        mMouse.mDz += mMouse.mZ - event->position().y();
        mMouse.mZ = event->position().y();

        mUpdate = true;
    }

    if (mMouse.mPressedButton == Qt::RightButton)
    {
        mMouse.mDw += mMouse.mW - event->position().y();
        mMouse.mW = event->position().y();
        mUpdate = true;
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
