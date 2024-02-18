#include "ModelData.h"

Earth::ModelData::ModelData() {}

void Earth::ModelData::AddVertex(const Vertex& vertex)
{
    mVertices << vertex;
}

void Earth::ModelData::Create()
{
    initializeOpenGLFunctions();
    mVAO.create();
    mVAO.bind();

    mVBO.create();
    mVBO.bind();
    mVBO.setUsagePattern(QOpenGLBuffer::UsagePattern::StaticDraw);
    mVBO.allocate(mVertices.constData(), sizeof(Vertex) * mVertices.size());

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureCoords));
    glEnableVertexAttribArray(2);

    mVBO.release();
    mVAO.release();
}

void Earth::ModelData::Render()
{
    if (mVAO.isCreated())
    {
        mVAO.bind();
        glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
        mVAO.release();
    }
}