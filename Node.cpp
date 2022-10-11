#include "Node.h"
#include "Helper.h"

#include <QtMath>

Node::Node(QObject *parent)
    : QObject{parent}
    , mPosition(0, 0, 0)
    , mScale(1, 1, 1)
    , mNodeType(NodeType::DummyNode)

{}

Node::~Node() {}

const QMatrix4x4 &Node::transformation() const
{
    return mTransformation;
}

void Node::setTransformation(const QMatrix4x4 &newTransformation)
{
    mTransformation = newTransformation;
    mPosition = mTransformation.column(3).toVector3D();
    mRotation = QQuaternion::fromRotationMatrix(mTransformation.normalMatrix());
}

const QQuaternion &Node::rotation() const
{
    return mRotation;
}

void Node::setRotation(const QQuaternion &newRotation)
{
    mRotation = newRotation;
    updateTransformation();
}

const QVector3D &Node::position() const
{
    return mPosition;
}

void Node::setPosition(const QVector3D &newPosition)
{
    mPosition = newPosition;
    updateTransformation();
}

const QVector3D &Node::scale() const
{
    return mScale;
}

void Node::setScale(const QVector3D &newScale)
{
    mScale = newScale;
    updateTransformation();
}

void Node::updateTransformation()
{
    mTransformation.setToIdentity();
    mTransformation.scale(mScale);
    mTransformation.rotate(mRotation);
    mTransformation.setColumn(3, QVector4D(mPosition, 1.0f));
}

const QString &Node::name() const
{
    return mName;
}

void Node::setName(const QString &newName)
{
    mName = newName;
}

Node::NodeType Node::nodeType() const
{
    return mNodeType;
}

void Node::drawGUI()
{
    // Position
    {
        ImGui::TextColored(ImVec4(1, 1, 0, 1), "Position:");

        float x = mPosition.x();
        float y = mPosition.y();
        float z = mPosition.z();

        if (ImGui::DragFloat("x##NodePosition", &x, 0.01f, -1000.0f, 1000.0f, "%.3f"))
            setPosition(QVector3D(x, y, z));
        if (ImGui::DragFloat("y##NodePosition", &y, 0.01f, -1000.0f, 1000.0f, "%.3f"))
            setPosition(QVector3D(x, y, z));
        if (ImGui::DragFloat("z##NodePosition", &z, 0.01f, -1000.0f, 1000.0f, "%.3f"))
            setPosition(QVector3D(x, y, z));
    }

    // Rotation
    {
        ImGui::TextColored(ImVec4(1, 1, 0, 1), "Rotation:");
        float yaw, pitch, roll;

        Helper::getEulerDegrees(mRotation, yaw, pitch, roll);

        if (ImGui::SliderFloat("Yaw##NodeRotation", &yaw, 0.0f, 359.999f, "%.3f"))
            setRotation(Helper::constructFromEulerDegrees(yaw, pitch, roll));
        if (ImGui::SliderFloat("Pitch##NodeRotation", &pitch, -89.999f, 89.999f, "%.3f"))
            setRotation(Helper::constructFromEulerDegrees(yaw, pitch, roll));
        if (ImGui::SliderFloat("Roll##NodeRotation", &roll, -179.999f, 179.999f, "%.3f"))
            setRotation(Helper::constructFromEulerDegrees(yaw, pitch, roll));
    }
}
