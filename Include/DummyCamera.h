#pragma once

#include "PerspectiveCamera.h"

#include <QObject>

namespace Earth
{
    class DummyCamera : public PerspectiveCamera
    {
    public:
        explicit DummyCamera(QObject* parent = nullptr);
        virtual ~DummyCamera();
    };
}
