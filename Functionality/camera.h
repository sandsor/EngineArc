#ifndef CAMERA_H
#define CAMERA_H

#include <QOpenGLFunctions_4_1_Core>
#include "vector3d.h"
#include "matrix4x4.h"

class Camera : public QOpenGLFunctions_4_1_Core
{
public:
    Camera();
    void pitch(float degrees);
    void yaw(float degrees);
    void updateRightVector();
    void updateForwardVector();
    void update();

    gsl::Matrix4x4 mViewMatrix;
    gsl::Matrix4x4 mProjectionMatrix;

    void setPosition(const gsl::Vector3D &position);

    void setSpeed(float speed);
    void updateHeigth(float deltaHeigth);
    void moveRight(float delta);
    void moveForward(float delta);

    gsl::Vector3D position() const;
    gsl::Vector3D up() const;

private:
    gsl::Vector3D mForward{0.f, 0.f, -1.f};
    gsl::Vector3D mRight{1.f, 0.f, 0.f};
    gsl::Vector3D mUp{0.f, 1.f, 0.f};

    gsl::Vector3D mPosition{0.f, 0.f, 0.f};
    float mPitch{0.f};
    float mYaw{0.f};

    gsl::Matrix4x4 mYawMatrix;
    gsl::Matrix4x4 mPitchMatrix;

    float mSpeed{0.f}; //camera will move by this speed along the mForward vector
};

#endif // CAMERA_H
