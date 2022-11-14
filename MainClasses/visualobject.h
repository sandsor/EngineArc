#ifndef VISUALOBJECT_H
#define VISUALOBJECT_H

#include <QOpenGLFunctions_4_1_Core>
#include <QMatrix4x4>
#include <matrix4x4.h>
#include <vector>
#include "vertex.h"
//#include <AL/al.h>

class VisualObject : public QOpenGLFunctions_4_1_Core {
public:
   VisualObject();
   VisualObject(std::string name);
   ~VisualObject();
   virtual void init(GLint matrixUniform)=0;
   virtual void draw()=0;
   virtual void move(float x, float y, float z){;};
   virtual void move(float dt) { }
   std::pair<float,float> getPosition2D();

   std::string m_name{""};

   virtual void open() {};

   gsl::Matrix4x4 mPosition;
   gsl::Matrix4x4 mRotation;
   gsl::Matrix4x4 mScale;

   bool bDraw{true};

    gsl::Matrix4x4 mMatrix; // kan ligge i protected
    std::vector<Vertex> getVertices();
protected:
    std::vector<Vertex> mVertices;
    std::vector<GLuint> mIndices;

   GLuint mVAO{0};
   GLuint mVBO{0};
   GLuint mEAB{0};//holds indices
   GLint mMatrixUniform{0};

   // Velger å lagre posisjon, rotasjon og translasjon
    // i hver sin 4x4 matrise

    // Legger til rette for simulering
    QVector3D mVelocity{1.f, 0.f, 0.f};
};
#endif // VISUALOBJECT_H
