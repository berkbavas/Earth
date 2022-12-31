#include "Helper.h"

#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QQuaternion>
#include <QTextStream>
#include <QtMath>

Earth::Helper::Helper() {}

QByteArray Earth::Helper::GetBytes(QString path)
{
    QFile file(path);
    if (file.open(QFile::ReadOnly))
    {
        return file.readAll();
    } else
    {
        qWarning() << QString("Could not open '%1'").arg(path);
        return QByteArray();
    }
}

QQuaternion Earth::Helper::RotateX(float angleRadians)
{
    return QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0), qRadiansToDegrees(angleRadians));
}

QQuaternion Earth::Helper::RotateY(float angleRadians)
{
    return QQuaternion::fromAxisAndAngle(QVector3D(0, 1, 0), qRadiansToDegrees(angleRadians));
}

QQuaternion Earth::Helper::RotateZ(float angleRadians)
{
    return QQuaternion::fromAxisAndAngle(QVector3D(0, 0, 1), qRadiansToDegrees(angleRadians));
}

QQuaternion Earth::Helper::Invert(const QQuaternion &rotation)
{
    float yaw, pitch, roll;
    GetEulerDegrees(rotation, yaw, pitch, roll);

    QQuaternion r = QQuaternion::fromAxisAndAngle(QVector3D(0, 1, 0), yaw);
    r = r * QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0), -pitch);
    r = r * QQuaternion::fromAxisAndAngle(QVector3D(0, 0, 1), roll);
    return r;
}

float Earth::Helper::AngleBetween(const QVector3D &v1, const QVector3D &v2, const QVector3D &left)
{
    QVector3D u1 = v1.normalized();
    QVector3D u2 = v2.normalized();

    float dot = QVector3D::dotProduct(u1, u2);

    if (qFuzzyCompare(dot, 1.0f))
    {
        return 0.0f;

    } else if (qFuzzyCompare(dot, -1.0f))
    {
        return 180.0f;

    } else
    {
        float angle = qRadiansToDegrees(acos(dot));
        QVector3D u1xu2 = QVector3D::crossProduct(u1, u2);

        if (QVector3D::dotProduct(u1xu2, left) < 0.0001f)
            return angle;
        else
            return -angle;
    }
}

float Earth::Helper::AngleBetween(const QVector3D &v1, const QVector3D &v2)
{
    QVector3D u1 = v1.normalized();
    QVector3D u2 = v2.normalized();

    float dot = QVector3D::dotProduct(u1, u2);

    if (qFuzzyCompare(dot, 1.0f))
    {
        return 0.0f;

    } else if (qFuzzyCompare(dot, -1.0f))
    {
        return 180.0f;

    } else
    {
        return qRadiansToDegrees(acos(dot));
    }
}

void Earth::Helper::GetEulerDegrees(const QQuaternion &rotation, float &yaw, float &pitch, float &roll)
{
    QVector3D zAxis = rotation * QVector3D(0, 0, -1);
    float x = zAxis.x();
    float y = zAxis.y();
    float z = zAxis.z();
    yaw = qRadiansToDegrees(atan2(-z, x)) - 90;
    pitch = qRadiansToDegrees(atan2(y, sqrt(z * z + x * x)));

    QVector3D xAxis = rotation * QVector3D(1, 0, 0);

    QVector3D xAxisProj = QQuaternion::fromAxisAndAngle(QVector3D(0, 1, 0), yaw) *   //
                          QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0), pitch) * //
                          QVector3D(1, 0, 0);

    QVector3D left = QQuaternion::fromAxisAndAngle(QVector3D(0, 1, 0), yaw) *   //
                     QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0), pitch) * //
                     QVector3D(0, 0, -1);

    roll = Earth::Helper::AngleBetween(xAxis, xAxisProj, left);

    if (yaw < 0.0f)
        yaw += 360.0f;
}

QQuaternion Earth::Helper::ConstructFromEulerDegrees(float yaw, float pitch, float roll)
{
    return QQuaternion::fromAxisAndAngle(QVector3D(0, 1, 0), yaw) *   //
           QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0), pitch) * //
           QQuaternion::fromAxisAndAngle(QVector3D(0, 0, -1), roll);
}

QVector3D Earth::Helper::ConstructFromLatLon(float lat, float lon)
{
    return QQuaternion::fromAxisAndAngle(QVector3D(0, 1, 0), lon) * //
           QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0), lat) * //
           QVector3D(0, 0, 1);
}

Earth::ModelData *Earth::Helper::LoadModelData(const QString &path)
{
    qInfo() << "Loading model at" << path;

    ModelData *modelData = nullptr;
    QVector<QVector3D> vertices;
    QVector<QVector3D> normals;
    QVector<QVector2D> textureCoords;

    QFile file(path);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        modelData = new ModelData;

        QTextStream fileText(&file);
        while (!fileText.atEnd())
        {
            QString fileLine = fileText.readLine();
            if (fileLine.startsWith("vn "))
            {
                QStringList lineList = fileLine.split(" ");
                normals << QVector3D(lineList[1].toFloat(), lineList[2].toFloat(), lineList[3].toFloat());

            } else if (fileLine.startsWith("vt "))
            {
                QStringList lineList = fileLine.split(" ");
                textureCoords << QVector2D(lineList[1].toFloat(), lineList[2].toFloat());

            } else if (fileLine.startsWith("v "))
            {
                QStringList lineList = fileLine.split(" ");
                vertices << QVector3D(lineList[1].toFloat(), lineList[2].toFloat(), lineList[3].toFloat());

            } else if (fileLine.startsWith("f "))
            {
                QStringList lineList = fileLine.split(" ");
                for (int i = 1; i <= 3; i++)
                {
                    QStringList arg = lineList[i].split("/");
                    ModelData::Vertex vertex;

                    if (arg.size() == 2)
                    {
                        if (arg[0].toInt() - 1 < vertices.size())
                            vertex.position = QVector4D(vertices[arg[0].toInt() - 1], 1);

                        if (arg[1].toInt() - 1 < normals.size())
                            vertex.normal = normals[arg[1].toInt() - 1];
                    } else if (arg.size() == 3)
                    {
                        if (arg[0].toInt() - 1 < vertices.size())
                            vertex.position = QVector4D(vertices[arg[0].toInt() - 1], 1);

                        if (arg[1].toInt() - 1 < textureCoords.size())
                            vertex.textureCoords = textureCoords[arg[1].toInt() - 1];

                        if (arg[2].toInt() - 1 < normals.size())
                            vertex.normal = normals[arg[2].toInt() - 1];
                    }

                    modelData->AddVertex(vertex);
                }
            }
        }

        file.close();
    } else
        qWarning() << QString("Could not open file '%1'.").arg(file.fileName());

    return modelData;
}
