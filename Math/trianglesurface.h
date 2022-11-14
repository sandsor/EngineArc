#ifndef TRIANGLESURFACE_H
#define TRIANGLESURFACE_H

#include "visualobject.h"
//#include <QVector3D>

class TriangleSurface : public VisualObject
{
public:
   TriangleSurface();
   TriangleSurface(std::string filnavn);
   ~TriangleSurface() override;
   void readFile(std::string filnavn);
   void writeFile(std::string filnavn);
   void init(GLint shader) override;
   void draw() override;
   void construct();
   float function(float x, float y);
   void volum();
   std::vector<Vertex>& getVertices() {return mVertices;}


   //void rotate(float angle, float x, float y, float z = 0.0f);
};
#endif
