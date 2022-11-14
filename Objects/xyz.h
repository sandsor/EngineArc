#ifndef XYZ_H
#define XYZ_H


//#include <QMatrix4x4>
//#include <vector>
//#include "vertex.h"
#include "visualobject.h"
#include <QOpenGLFunctions_4_1_Core>

class XYZ : public VisualObject
{

public:
   XYZ();
   ~XYZ() override;

   void init(GLint matrixUniform) override;
   void draw() override;
};
#endif // XYZ_H
