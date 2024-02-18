#include "Shader.h"
#include "Helper.h"

#include <QDebug>
#include <QFile>

Earth::Shader::Shader(ShaderManager::ShaderType type)
    : mProgram(nullptr)
    , mType(type)
{}

Earth::Shader::~Shader()
{
    if (mProgram)
    {
        delete mProgram;
        mProgram = nullptr;
    }
}

QByteArray Earth::Shader::GetBytes(const QString& path)
{
    QFile file(path);
    if (file.open(QFile::ReadOnly))
    {
        return file.readAll();
    }
    else
    {
        qWarning() << QString("Could not open '%1'").arg(path);
        return QByteArray();
    }
}

bool Earth::Shader::Init()
{
    initializeOpenGLFunctions();

    qInfo().noquote() << mType << "is initializing... ";

    mProgram = new QOpenGLShaderProgram;

    auto types = mPaths.keys();

    for (auto type : qAsConst(types))
    {
        if (!mProgram->addShaderFromSourceCode(type, GetBytes(mPaths[type])))
        {
            qWarning() << "Could not load" << GetShaderTypeString(type);
            mProgram->deleteLater();
            return false;
        }
    }

    if (!mProgram->link())
    {
        qWarning() << "Could not link shader program.";
        mProgram->deleteLater();
        return false;
    }

    if (!mProgram->bind())
    {
        qWarning() << "Could not bind shader program.";
        mProgram->deleteLater();
        return false;
    }

    mProgram->release();

    qInfo().noquote() << mType << "is initialized.";

    return true;
}

bool Earth::Shader::Bind()
{
    return mProgram->bind();
}

void Earth::Shader::Release()
{
    mProgram->release();
}

void Earth::Shader::AddPath(QOpenGLShader::ShaderTypeBit type, const QString& path)
{
    mPaths.insert(type, path);
}

void Earth::Shader::SetUniformValue(const QString& name, int value)
{
    mProgram->setUniformValue(mProgram->uniformLocation(name), value);
}

void Earth::Shader::SetUniformValue(const QString& name, unsigned int value)
{
    mProgram->setUniformValue(mProgram->uniformLocation(name), value);
}

void Earth::Shader::SetUniformValue(const QString& name, float value)
{
    mProgram->setUniformValue(mProgram->uniformLocation(name), value);
}

void Earth::Shader::SetUniformValue(const QString& name, const QVector3D& value)
{
    mProgram->setUniformValue(mProgram->uniformLocation(name), value);
}

void Earth::Shader::SetUniformValue(const QString& name, const QVector4D& value)
{
    mProgram->setUniformValue(mProgram->uniformLocation(name), value);
}

void Earth::Shader::SetUniformValue(const QString& name, const QMatrix4x4& value)
{
    mProgram->setUniformValue(mProgram->uniformLocation(name), value);
}

void Earth::Shader::SetUniformValue(const QString& name, const QMatrix3x3& value)
{
    mProgram->setUniformValue(mProgram->uniformLocation(name), value);
}

void Earth::Shader::SetUniformValueArray(const QString& name, const QVector<QVector3D>& values)
{
    mProgram->setUniformValueArray(mProgram->uniformLocation(name), values.constData(), values.size());
}

void Earth::Shader::SetSampler(const QString& name, unsigned int unit, unsigned int id, GLenum target)
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(target, id);
    SetUniformValue(name, unit);
}

Earth::ShaderManager::ShaderType Earth::Shader::GetType() const
{
    return mType;
}

QString Earth::Shader::GetShaderTypeString(QOpenGLShader::ShaderTypeBit type)
{
    switch (type)
    {
    case QOpenGLShader::Vertex:
        return "Vertex Shader";
    case QOpenGLShader::Fragment:
        return "Fragment Shader";
    case QOpenGLShader::Geometry:
        return "Geometry Shader";
    case QOpenGLShader::TessellationControl:
        return "Tessellation Control Shader";
    case QOpenGLShader::TessellationEvaluation:
        return "Tesselation Evaluation Shader";
    case QOpenGLShader::Compute:
        return "Compute Shader";
    default:
        return "Unknown Shader";
    }
}