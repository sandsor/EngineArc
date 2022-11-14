#ifndef GENERATE_H
#define GENERATE_H

#include "visualobject.h"
#include <QVector3D>

class Generate : public VisualObject
{
public:
   Generate();
   Generate(std::string filnavn);
   ~Generate() override;
   void readFile(std::string filnavn);
   void writeFile(std::string filnavn);
   float FindAverage(const std::vector<float>& a);
//   void construct();

   void init(GLint shader) override;
   void draw() override;
private:
   float n= 0;
   Vertex vertex;

   float CordX;
   float CordY;
   float CordZ;

   float MaxCordX = 0;
   float MaxCordZ = 0;

   float MinCordX = 0;
   float MinCordZ = 0;

   float MaxX = 6743400;
   float MaxZ = 613600;

   float MinX = 6742200;
   float MinZ = 612000;

   float MaxY = 237;
   float MinY = 103;

   float rangeX = MaxX-MinX;
   float rangeZ = MaxZ-MinZ;
       float rangeY = MaxY-MinY;

   float LevelCounter = 0;
};
#endif
