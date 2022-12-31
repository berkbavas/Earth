#include "PerspectiveCamera.h"

#include <QtMath>

Earth::PerspectiveCamera::PerspectiveCamera(QObject* parent)
    : Camera(parent)
{
    SetVerticalFov(60.0f);
}

Earth::PerspectiveCamera::~PerspectiveCamera() {}

void Earth::PerspectiveCamera::SetHorizontalFov(float newHorizontalFov)
{
    mHorizontalFov = newHorizontalFov;

    float halfHorizontalFovRadian = 0.5f * qDegreesToRadians(mHorizontalFov);
    float verticalFovRadian = 2 * atan(tan(halfHorizontalFovRadian) * (float(mWidth) / float(mHeight)));

    mVerticalFov = qRadiansToDegrees(verticalFovRadian);
}

void Earth::PerspectiveCamera::SetVerticalFov(float newVerticalFov)
{
    mVerticalFov = newVerticalFov;

    float halfVerticalFovRadian = 0.5f * qDegreesToRadians(mVerticalFov);
    float horizontalFovRadian = 2 * atan(tan(halfVerticalFovRadian) / (float(mWidth) / float(mHeight)));

    mHorizontalFov = qRadiansToDegrees(horizontalFovRadian);
}

float Earth::PerspectiveCamera::GetVerticalFov() const
{
    return mVerticalFov;
}

float Earth::PerspectiveCamera::GetHorizontalFov() const
{
    return mHorizontalFov;
}

QMatrix4x4 Earth::PerspectiveCamera::GetProjectionMatrix()
{
    QMatrix4x4 projection;
    projection.perspective(mVerticalFov, float(mWidth) / float(mHeight), mZNear, mZFar);
    return projection;
}