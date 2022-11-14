#include "trianglesurface.h"
#include <iostream>
#include <fstream>



TriangleSurface::TriangleSurface() : VisualObject()
{

  construct();
  //writeFile("D:/3Dprog22/test.txt");
  mMatrix.setToIdentity();
}

TriangleSurface::TriangleSurface(std::string filnavn)
{
   //construct();
   //writeFile(filnavn);
   readFile(filnavn);
//   mMatrix.setToIdentity();
}


TriangleSurface::~TriangleSurface()
{

}

void TriangleSurface::readFile(std::string filnavn)
{
    std::ifstream inn;
     inn.open(filnavn.c_str());

     if (inn.is_open()) {
         Vertex vertex;
         while (inn >> vertex) {

              if(inn.eof())break;
              mVertices.push_back(vertex);
         }
         inn.close();
         mMatrix.setToIdentity();
         qDebug()<<vertex.to3DVec();
     }
     else
          qDebug()<<"no file found";
}

void TriangleSurface::writeFile(std::string filnavn)
{
    std::ofstream ut;
    ut.open(filnavn.c_str());

    ut << mVertices.size();
    for (auto it=mVertices.begin(); it!=mVertices.end(); it++)
        ut << *it << std::endl;
    ut.close();

}

void TriangleSurface::init(GLint matrixUniform) {
    mMatrixUniform = matrixUniform;
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );

    glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof(Vertex), mVertices.data(), GL_STATIC_DRAW );

    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE,sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void TriangleSurface::draw()
{
   glBindVertexArray( mVAO );
   glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
   glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
}



//void TriangleSurface::construct()
//{
//  float xmin=-5.0f, xmax=5.0f, ymin=-5.0f, ymax=5.0f, h=0.25f;
//  for (auto x=xmin; x<xmax; x+=h)
//      for (auto y=ymin; y<ymax; y+=h)
//      {
//          float z = function(x,y);
//          mVertices.push_back(Vertex{x,y,z,x,y,z});
//          z = function(x+h,y);
//          mVertices.push_back(Vertex{x+h,y,z,x,y,z});
//          z = function(x,y+h);
//          mVertices.push_back(Vertex{x,y+h,z,x,y,z});
//          mVertices.push_back(Vertex{x,y+h,z,x,y,z});
//          z = function(x+h,y);
//          mVertices.push_back(Vertex{x+h,y,z,x,y,z});
//          z = function(x+h,y+h);
//          mVertices.push_back(Vertex{x+h,y+h,z,x,y,z});
//      }
//}

void TriangleSurface::construct()
{
  float xmin=-5.5f, xmax=5.0f, ymin=-5.5f, ymax=5.0f, h=0.25f;
  for (auto x=xmin; x<xmax; x+=h)
      for (auto y=ymin; y<ymax; y+=h)
      {
          float z = function(x,y);
          mVertices.push_back(Vertex{x,z,y,x,y,z});
          z = function(x+h,y);
          mVertices.push_back(Vertex{x+h,z,y,x,y,z});
          z = function(x,y+h);
          mVertices.push_back(Vertex{x,z,y+h,x,y,z});
          mVertices.push_back(Vertex{x,z,y+h,x,y,z});
          z = function(x+h,y);
          mVertices.push_back(Vertex{x+h,z,y,x,y,z});
          z = function(x+h,y+h);
          mVertices.push_back(Vertex{x+h,z,y+h,x,y,z});


      }
}


float TriangleSurface::function(float x, float y)
{
    //return (x*x-y*y);
    //return sin(M_PI*x)*sin(M_PI*y);
    //return (x*x*y+1);
    //return (-0.0341* x * x + (0.7019* x) + 0.2915);
    //return 0.417*(x*x*x)-3.75*(x*x)+10.333*x-6;
    //return x+y+3*y+y*x+2.5;
    return sin(x * 0.8) * sin(y * 0.8) - 1;
}

//void TriangleSurface::rotate(float angle, float x, float y, float z)
//{
//    mMatrix.rotate( angle, x, y, z);
//}

void TriangleSurface::volum()
{
    std::fstream file;
    file.open("../3Dprog22/matte_oblig1.txt", std::ios::out);
    if(file.is_open())
    {
        float xmin=0.0f, xmax=0.5f, ymin=0.0f, ymax=0.5f, h=0.005f, volume = 0.0f;
            for (auto x=xmin; x<xmax; x+=h)
                for (auto y=ymin; y<ymax; y+=h)
                {
                    volume += h*h*(1-x-y);
                }
     file << "Volumet under funksjonen er " << volume;
     file.close();
    }
}


