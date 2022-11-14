#ifndef PLAN_H
#define PLAN_H

#include <QOpenGLFunctions_4_1_Core>
#include <vector>
#include "vertex.h"
#include "visualobject.h"

class Plan : public VisualObject
{
private:
   std::vector<Vertex> mVertices;
   //QOpenGLContext *mContext;

public:
    Plan();
    ~Plan();
    //void setContext(QOpenGLContext *context);
    void init(GLint matrixUniform) override;
    void draw() override;
};

#endif // PLAN_H
