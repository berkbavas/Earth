#ifndef NODE_H
#define NODE_H

#include <QMatrix4x4>
#include <QObject>
#include <QQuaternion>
#include <QVector3D>

#include <imgui.h>
#include <QtImGui.h>

class Node : public QObject
{
    Q_OBJECT
public:
    explicit Node(QObject *parent = nullptr);
    virtual ~Node();

    enum class NodeType { //
        DummyNode,
        Model,
        DummyCamera,
    };

    virtual const QMatrix4x4 &transformation() const;
    virtual void setTransformation(const QMatrix4x4 &newTransformation);

    virtual const QQuaternion &rotation() const;
    virtual void setRotation(const QQuaternion &newRotation);

    virtual const QVector3D &position() const;
    virtual void setPosition(const QVector3D &newPosition);

    virtual const QVector3D &scale() const;
    virtual void setScale(const QVector3D &newScale);

    const QString &name() const;
    void setName(const QString &newName);

    NodeType nodeType() const;

    virtual void drawGUI();

private:
    virtual void updateTransformation();

protected:
    QMatrix4x4 mTransformation;
    QQuaternion mRotation;
    QVector3D mPosition;
    QVector3D mScale;
    QString mName;
    NodeType mNodeType;
};

#endif // NODE_H
