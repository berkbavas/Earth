#pragma once

#include <QVector3D>
#include <QVector4D>
#include <QVector2D>

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QVector>

namespace Earth
{

    class ModelData : protected QOpenGLFunctions
    {
    public:
        ModelData();

        struct Vertex {
            QVector4D position;
            QVector3D normal;
            QVector2D textureCoords;
        };

        void AddVertex(const Vertex& vertex);
        void Create();
        void Render();

    private:
        QVector<Vertex> mVertices;
        QOpenGLVertexArrayObject mVAO;
        QOpenGLBuffer mVBO;
    };
}


