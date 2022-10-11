#include "Controller.h"
#include "Window.h"

#include <QApplication>
#include <QDebug>
#include <QDir>

Controller::Controller(QObject *parent)
    : QObject(parent)
    , mImGuiWantsMouseCapture(false)
    , mSuccess(true)
    , mPressedButton(Qt::NoButton)
    , mX(0)
    , mY(0)
    , mZ(0)
    , mW(0)
    , mDx(0)
    , mDy(0)
    , mDz(0)
    , mDw(0)
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
            rotation = QQuaternion::fromAxisAndAngle(QVector3D(0, 0, 1), -mDz * ifps * qMax(2.0f, mDistance - 10)) * rotation;
            rotation = QQuaternion::fromAxisAndAngle(QVector3D(0, 1, 0), -mDx * ifps * (mDistance - 10)) * rotation;
            rotation = QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0), -mDy * ifps * (mDistance - 10)) * rotation;
            mEarth->setRotation(rotation);
        }

        // Camera
        {
            mTilt -= mDw * ifps * 30;

            mTilt = qBound(0.0f, mTilt, 89.0f);

            auto rotation = QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0), mTilt);
            auto position = QVector3D(0, 0, 10) + rotation * QVector3D(0, 0, mDistance - 10);

            mCamera->setRotation(rotation);
            mCamera->setPosition(position);
        }

        mDx = 0.0f;
        mDy = 0.0f;
        mDz = 0.0f;
        mDw = 0.0f;
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
        mDistance = 1.015 * mDistance;

    if (event->angleDelta().y() > 0)
        mDistance = mDistance / 1.015;

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
        mX = event->position().x();
        mY = event->position().y();
        mPressedButton = Qt::LeftButton;
    }

    if (event->button() == Qt::MiddleButton)
    {
        mZ = event->position().y();
        mPressedButton = Qt::MiddleButton;
    }

    if (event->button() == Qt::RightButton)
    {
        mW = event->position().y();
        mPressedButton = Qt::RightButton;
    }

    mRendererManager->mousePressed(event);
}

void Controller::mouseReleased(QMouseEvent *event)
{
    if (!mSuccess)
        return;

    mPressedButton = Qt::NoButton;

    mRendererManager->mouseReleased(event);
}

void Controller::mouseMoved(QMouseEvent *event)
{
    if (!mSuccess)
        return;

    if (mImGuiWantsMouseCapture)
        return;

    if (mPressedButton == Qt::LeftButton)
    {
        mDx += mX - event->position().x();
        mDy += mY - event->position().y();

        mX = event->position().x();
        mY = event->position().y();

        mUpdate = true;
    }

    if (mPressedButton == Qt::MiddleButton)
    {
        mDz += mZ - event->position().y();
        mZ = event->position().y();

        mUpdate = true;
    }

    if (mPressedButton == Qt::RightButton)
    {
        mDw += mW - event->position().y();
        mW = event->position().y();
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
