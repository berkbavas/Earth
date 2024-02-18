#pragma once

#include "ModelData.h"

#include <QMatrix4x4>
#include <QQuaternion>
#include <QString>

namespace Earth
{
    class Helper
    {
    private:
        Helper();

    public:
        static QByteArray GetBytes(QString path);
        static QQuaternion RotateX(float angleRadians);
        static QQuaternion RotateY(float angleRadians);
        static QQuaternion RotateZ(float angleRadians);
        static QQuaternion Invert(const QQuaternion& rotation);
        static float AngleBetween(const QVector3D& v1, const QVector3D& v2, const QVector3D& left);
        static float AngleBetween(const QVector3D& v1, const QVector3D& v2);
        static void GetEulerDegrees(const QQuaternion& rotation, float& yaw, float& pitch, float& roll);
        static QQuaternion ConstructFromEulerDegrees(float yaw, float pitch, float roll);
        static QVector3D ConstructFromLatLon(float lat, float lon);
        static ModelData* LoadModelData(const QString& path);
    };
}
