#include "Node.h"
#include "Helper.h"

#include <QtMath>

Earth::Node::Node(QObject* parent)
    : QObject(parent)
    , mPosition(0, 0, 0)
    , mScale(1, 1, 1)
    , mVisible(true)
{}

Earth::Node::~Node() {}

const QQuaternion& Earth::Node::Rotation() const
{
    return mRotation;
}

void Earth::Node::SetRotation(const QQuaternion& newRotation)
{
    mRotation = newRotation;

    UpdateTransformation();
}

const QVector3D& Earth::Node::Position() const
{
    return mPosition;
}

void Earth::Node::SetPosition(const QVector3D& newPosition)
{
    mPosition = newPosition;

    UpdateTransformation();
}

const QVector3D& Earth::Node::Scale() const
{
    return mScale;
}

void Earth::Node::SetScale(const QVector3D& newScale)
{
    mScale = newScale;

    UpdateTransformation();
}

const QMatrix4x4& Earth::Node::Transformation() const
{
    return mTransformation;
}

void Earth::Node::SetTransformation(const QMatrix4x4& newTransformation)
{
    mTransformation = newTransformation;

    mPosition = mTransformation.column(3).toVector3D();
    mRotation = QQuaternion::fromRotationMatrix(mTransformation.normalMatrix());
}

void Earth::Node::UpdateTransformation()
{
    mTransformation.setToIdentity();
    mTransformation.scale(mScale);
    mTransformation.rotate(mRotation);
    mTransformation.setColumn(3, QVector4D(mPosition, 1.0f));
}
