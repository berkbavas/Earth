#ifndef RENDERERMANAGER_H
#define RENDERERMANAGER_H

#include "Camera.h"
#include "Earth.h"
#include "Manager.h"
#include "ModelData.h"
#include "ShaderManager.h"
#include "Sun.h"

#include <QMap>
#include <QMatrix4x4>
#include <QObject>
#include <QOpenGLExtraFunctions>
#include <QOpenGLFramebufferObject>

class RendererManager : public Manager, protected QOpenGLExtraFunctions
{
    Q_OBJECT

private:
    explicit RendererManager(QObject *parent = nullptr);

public:
    static RendererManager *instance();

    bool init() override;
    void render(float ifps);
    void resize(int w, int h);
    void setCamera(Camera *camera);
    void drawGUI();

    void mousePressed(QMouseEvent *event);
    void mouseReleased(QMouseEvent *event);
    void mouseMoved(QMouseEvent *event);

    QVector4D pixelToLatLon(float x, float y);

private:
    QMap<QString, ModelData *> mModelsData;

    ShaderManager *mShaderManager;
    Camera *mCamera;
    Sun *mSun;
    Earth *mEarth;

    int mWidth;
    int mHeight;

    // Mouse
    QOpenGLFramebufferObjectFormat mMousePositionFBOFormat;
    QOpenGLFramebufferObject *mMousePositionFBO;
    QVector4D mMouseWorldPosition;

    Qt::MouseButton mPressedButton;
};

#endif // RENDERERMANAGER_H
