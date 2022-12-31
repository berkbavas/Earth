#pragma once

#include "Camera.h"

namespace Earth
{
    class PerspectiveCamera : public Camera
    {
        Q_OBJECT

    protected:
        explicit PerspectiveCamera(QObject* parent = nullptr);
        virtual ~PerspectiveCamera();

    public:
        virtual QMatrix4x4 GetProjectionMatrix() override;

        void SetVerticalFov(float newVerticalFov);
        void SetHorizontalFov(float newHorizontalFov);

        float GetVerticalFov() const;
        float GetHorizontalFov() const;

    protected:
        float mVerticalFov;
        float mHorizontalFov;
    };
}
