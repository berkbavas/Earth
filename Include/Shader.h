#pragma once

#include "ShaderManager.h"

#include <QObject>
#include <QOpenGLFunctions>
#include <QOpenGLShader>

namespace Earth
{
    class Shader : protected QOpenGLFunctions
    {
    public:
        Shader(ShaderManager::ShaderType type);
        ~Shader();

        bool Init();
        bool Bind();
        void Release();

        void AddPath(QOpenGLShader::ShaderTypeBit type, const QString& path);

        void SetUniformValue(const QString& name, int value);
        void SetUniformValue(const QString& name, unsigned int value);
        void SetUniformValue(const QString& name, float value);
        void SetUniformValue(const QString& name, const QVector3D& value);
        void SetUniformValue(const QString& name, const QVector4D& value);
        void SetUniformValue(const QString& name, const QMatrix4x4& value);
        void SetUniformValue(const QString& name, const QMatrix3x3& value);
        void SetUniformValueArray(const QString& name, const QVector<QVector3D>& values);
        void SetSampler(const QString& name, unsigned int unit, unsigned int id, GLenum target = GL_TEXTURE_2D);

        ShaderManager::ShaderType GetType() const;

        static QByteArray GetBytes(const QString& path);

    private:
        QString GetShaderTypeString(QOpenGLShader::ShaderTypeBit type);

    private:
        QOpenGLShaderProgram* mProgram;
        QMap<QOpenGLShader::ShaderTypeBit, QString> mPaths;

        ShaderManager::ShaderType mType;
    };
}
