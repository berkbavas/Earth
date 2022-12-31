#pragma once

#include "Node.h"
#include "Macros.h"

#include <QMatrix4x4>
#include <QMouseEvent>
#include <QTimer>

namespace Earth
{
    class Camera : public Node
    {
        Q_OBJECT
    protected:
        Camera(QObject* parent = nullptr);

    public:

        virtual QMatrix4x4 GetViewProjectionMatrix();
        virtual QMatrix4x4 GetViewMatrix();
        virtual QMatrix4x4 GetRotationMatrix();
        virtual QVector3D GetViewDirection();

        virtual QMatrix4x4 GetProjectionMatrix() = 0;

        virtual void MouseDoubleClicked(QMouseEvent*);
        virtual void MousePressed(QMouseEvent*);
        virtual void MouseReleased(QMouseEvent*);
        virtual void MouseMoved(QMouseEvent*);
        virtual void WheelMoved(QWheelEvent*);
        virtual void KeyPressed(QKeyEvent*);
        virtual void KeyReleased(QKeyEvent*);
        virtual void Update(float);
        virtual void Reset();
        virtual void Resize(int width, int height);

        bool Active() const;
        void SetActive(bool newActive);

    signals:
        void ActiveChanged();

    protected:
        bool mActive;

        DEFINE_MEMBER(int, Width);
        DEFINE_MEMBER(int, Height);
        DEFINE_MEMBER(float, ZNear);
        DEFINE_MEMBER(float, ZFar);
    };
}
