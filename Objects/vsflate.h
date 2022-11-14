#ifndef VSFLATE_H
#define VSFLATE_H


//#include <QOpenGLFunctions_4_1_Core>
//#include <QMatrix4x4>
//#include <vector>
//#include "vertex.h"
#include "visualobject.h"

class VSflate : public VisualObject
{
//private:
//   std::vector<Vertex> mVertices;
//   //QOpenGLContext *mContext;

public:
    VSflate();
    ~VSflate() override;
    void init(GLint matrixUniform) override;
    void draw() override;
    std::vector<Vertex>& getVertices() {return mVertices;}
};

#endif // VSFLATE_H
