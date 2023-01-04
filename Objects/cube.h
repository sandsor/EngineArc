#ifndef CUBE_H
#define CUBE_H

#include <QOpenGLFunctions_4_1_Core>
#include <QMatrix4x4>
#include <vector>
#include "vertex.h"
#include "visualobject.h"

class Cube : public VisualObject
{
//private:
//   std::vector<Vertex> mVertices;
//   //QOpenGLContext *mContext;

public:
    Cube();
    ~Cube() override;
    void construct();
    void init(GLint matrixUniform) override;
    void draw() override;
    void ChangeColor(float r, float g, float b);
    void move(float x, float y, float z)override;
    void Scale(float scale);
    void RotateX(float x);
    void RotateY(float y);
    void RotateZ(float z);
    void RotateXYZ(float x, float y, float z);
};

#endif // CUBE_H
