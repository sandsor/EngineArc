#ifndef DOOR_H
#define DOOR_H

#include "visualobject.h"

class Door : public VisualObject
{
public:
   Door();
   ~Door() override;
   void init(GLint matrixUniform) override;
   void draw() override;
   void construct();
   //void move(float dt) override;
   void open() override;
   bool bOpen {false};
protected:
   std::vector<GLuint> mIndices;   	// Til indeksering
   GLuint mIBO{0};                 		// for glDrawElements()

   float mRadius{0.5};			//
   //QVector4D mRotation;		// her eller i VisualObjects???? Bruk en av dem
};

#endif // DOOR_H
