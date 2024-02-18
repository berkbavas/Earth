#pragma once

#include "Macros.h"

#include <QMatrix4x4>
#include <QObject>
#include <QQuaternion>
#include <QVector3D>

#include <imgui.h>
#include <QtImGui.h>

namespace Earth
{
    class Node : public QObject
    {
    protected:
        friend class OpenGLWidget;

        explicit Node(QObject* parent = nullptr);
        virtual ~Node();

    public:
        const QMatrix4x4& Transformation() const;
        void SetTransformation(const QMatrix4x4& newTransformation);

        const QQuaternion& Rotation() const;
        void SetRotation(const QQuaternion& newRotation);

        const QVector3D& Position() const;
        void SetPosition(const QVector3D& newPosition);

        const QVector3D& Scale() const;
        void SetScale(const QVector3D& newScale);

        void SetPosition(float x, float y, float z) { SetPosition(QVector3D(x, y, z)); }
        void SetScale(float x, float y, float z) { SetScale(QVector3D(x, y, z)); }

    private:
        void UpdateTransformation();

    private:
        QMatrix4x4 mTransformation;
        QQuaternion mRotation;
        QVector3D mPosition;
        QVector3D mScale;

        DEFINE_MEMBER(bool, Visible)
    };
}
