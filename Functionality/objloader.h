#ifndef OBJLOADER_H
#define OBJLOADER_H

#include "visualobject.h"
#include "fstream"
#include <QVector3D>

class ObjLoader : public VisualObject
{
public:
   ObjLoader();
   ~ObjLoader();
   void ReadFile(std::string filnavn);
   void init(GLint matrixUniform) override;
   void draw() override;
private:
   Vertex vertex;

};

#endif // OBJLOADER_H
