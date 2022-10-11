#ifndef MODELDATA_H
#define MODELDATA_H

#include <QVector3D>

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QVector>

class ModelData : protected QOpenGLFunctions
{
public:
    ModelData();

    struct Vertex {
        QVector4D position;
        QVector3D normal;
        QVector2D textureCoords;
    };

    void addVertex(const Vertex &vertex);
    void create();
    void render();

private:
    QVector<Vertex> mVertices;
    QOpenGLVertexArrayObject mVAO;
    QOpenGLBuffer mVBO;
};

#endif // MODELDATA_H
