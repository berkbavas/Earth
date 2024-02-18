#include "Camera.h"

Earth::Camera::Camera(QObject* parent)
    : Node(parent)
    , mActive(false)
    , mWidth(1600)
    , mHeight(900)
    , mZNear(0.1f)
    , mZFar(10000.0f)
{}

QMatrix4x4 Earth::Camera::GetViewProjectionMatrix()
{
    return GetProjectionMatrix() * GetViewMatrix();
}

QMatrix4x4 Earth::Camera::GetViewMatrix()
{
    QMatrix4x4 viewMatrix;
    viewMatrix.rotate(Rotation().conjugated());
    viewMatrix.translate(-Position());

    return viewMatrix;
}

QMatrix4x4 Earth::Camera::GetRotationMatrix()
{
    auto rotation = GetViewMatrix();
    rotation.setColumn(3, QVector4D(0, 0, 0, 1));
    return rotation;
}

QVector3D Earth::Camera::GetViewDirection()
{
    return Rotation() * QVector3D(0, 0, -1);
}

void Earth::Camera::Resize(int width, int height)
{
    mWidth = width;
    mHeight = height;
}

bool Earth::Camera::Active() const
{
    return mActive;
}

void Earth::Camera::SetActive(bool newActive)
{
    if (mActive == newActive)
        return;

    mActive = newActive;

    emit ActiveChanged();
}

void Earth::Camera::MouseDoubleClicked(QMouseEvent*) {}

void Earth::Camera::MousePressed(QMouseEvent*) {}

void Earth::Camera::MouseReleased(QMouseEvent*) {}

void Earth::Camera::MouseMoved(QMouseEvent*) {}

void Earth::Camera::WheelMoved(QWheelEvent*) {}

void Earth::Camera::KeyPressed(QKeyEvent*) {}

void Earth::Camera::KeyReleased(QKeyEvent*) {}

void Earth::Camera::Update(float) {}

void Earth::Camera::Reset() {}