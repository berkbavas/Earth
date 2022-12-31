#include "ShaderManager.h"
#include "Shader.h"

Earth::ShaderManager::ShaderManager(QObject* parent)
    : QObject(parent)
    , mActiveShader(ShaderType::None)
{}

Earth::ShaderManager::~ShaderManager()
{
    for (auto& shader : mShaders.values())
        delete shader;

    mShaders.clear();
}

Earth::Shader* Earth::ShaderManager::GetShader(ShaderType shader)
{
    return mShaders.value(shader);
}

bool Earth::ShaderManager::Init()
{
    {
        Shader* shader = new Shader(ShaderType::EarthShader);
        mShaders.insert(shader->GetType(), shader);

        shader->AddPath(QOpenGLShader::Vertex, ":/Resources/Shaders/Earth.vert");
        shader->AddPath(QOpenGLShader::Fragment, ":/Resources/Shaders/Earth.frag");

        if (!shader->Init())
            return false;
    }

    {
        Shader* shader = new Shader(ShaderType::EarthMousePositionShader);
        mShaders.insert(shader->GetType(), shader);

        shader->AddPath(QOpenGLShader::Vertex, ":/Resources/Shaders/EarthMousePosition.vert");
        shader->AddPath(QOpenGLShader::Fragment, ":/Resources/Shaders/EarthMousePosition.frag");

        if (!shader->Init())
            return false;
    }

    return true;
}

bool Earth::ShaderManager::Bind(ShaderType shader)
{
    mActiveShader = shader;
    return mShaders.value(mActiveShader)->Bind();
}

void Earth::ShaderManager::Release()
{
    mShaders.value(mActiveShader)->Release();
}

void Earth::ShaderManager::SetUniformValue(const QString& name, int value)
{
    mShaders.value(mActiveShader)->SetUniformValue(name, value);
}

void Earth::ShaderManager::SetUniformValue(const QString& name, unsigned int value)
{
    mShaders.value(mActiveShader)->SetUniformValue(name, value);
}

void Earth::ShaderManager::SetUniformValue(const QString& name, float value)
{
    mShaders.value(mActiveShader)->SetUniformValue(name, value);
}

void Earth::ShaderManager::SetUniformValue(const QString& name, const QVector3D& value)
{
    mShaders.value(mActiveShader)->SetUniformValue(name, value);
}

void Earth::ShaderManager::SetUniformValue(const QString& name, const QVector4D& value)
{
    mShaders.value(mActiveShader)->SetUniformValue(name, value);
}

void Earth::ShaderManager::SetUniformValue(const QString& name, const QMatrix4x4& value)
{
    mShaders.value(mActiveShader)->SetUniformValue(name, value);
}

void Earth::ShaderManager::SetUniformValue(const QString& name, const QMatrix3x3& value)
{
    mShaders.value(mActiveShader)->SetUniformValue(name, value);
}

void Earth::ShaderManager::SetUniformValueArray(const QString& name, const QVector<QVector3D>& values)
{
    mShaders.value(mActiveShader)->SetUniformValueArray(name, values);
}

void Earth::ShaderManager::SetSampler(const QString& name, unsigned int unit, unsigned int id, GLenum target)
{
    mShaders.value(mActiveShader)->SetSampler(name, unit, id, target);
}