#include "objloader.h"
#include <iostream>
#include <sstream>      // std::stringstream, std::stringbuf


ObjLoader::ObjLoader()
{
    ReadFile("../3Dprog22/Assets/untitled.obj");
    mMatrix.setToIdentity();
}

ObjLoader::~ObjLoader()
{

}

void ObjLoader::ReadFile(std::string filnavn)
{
    std::ifstream inn;
     inn.open(filnavn);
         qDebug()<<"startet reading";
         if(!inn)
             qDebug() << "Could not open file for reading" << QString::fromStdString(filnavn);
     if (inn.is_open())
     {
         qDebug()<<"Helo";

         std::string OneLine;
         std::string OneWord;

         std::vector<gsl::Vector3D> tempVertices;
         std::vector<gsl::Vector3D> tempNormals;
         std::vector<gsl::Vector2D> tempUVs;

         unsigned int temp_index = 0;

         while(std::getline(inn, OneLine))
         {
             qDebug()<<"inn while loop";
             std::stringstream sStream;

             sStream << OneLine;
             OneWord = "";
             sStream >> OneWord;

             if(OneWord == "#")
                 continue;
             if(OneWord == "")
                 continue;
             if(OneWord == "v")
             {
                 qDebug()<<"OneWord = v";
                 gsl::Vector3D tempVertex;
                 sStream >> OneWord;
                 tempVertex.x = std::stof(OneWord);
                 sStream >> OneWord;
                 tempVertex.y = std::stof(OneWord);
                 sStream >> OneWord;
                 tempVertex.z = std::stof(OneWord);
                    qDebug()<<tempVertex;
                 tempVertices.push_back(tempVertex);

                 continue;

             }
             if(OneWord == "vt")
             {
                 qDebug()<<"OneWord = vt";
                 gsl::Vector2D tempUV;
                 sStream >> OneWord;
                 tempUV.x = std::stof(OneWord);
                 sStream >> OneWord;
                 tempUV.y = std::stof(OneWord);

                 tempUVs.push_back(tempUV);

                 continue;
             }
             if(OneWord == "vn")
             {
                 qDebug()<<"OneWord = vn";
                 gsl::Vector3D tempNormal;
                 sStream >> OneWord;
                 tempNormal.x = std::stof(OneWord);
                 sStream >> OneWord;
                 tempNormal.y = std::stof(OneWord);
                 sStream >> OneWord;
                 tempNormal.z = std::stof(OneWord);

                 tempNormals.push_back(tempNormal);
                 continue;
             }
             if(OneWord == "f")
             {
                 qDebug()<<"OneWord = f";
                 int index, normal, uv;
                 for(int i = 0; i < 3; i++)
                 {
                     sStream >> OneWord;
                     std::stringstream tempWord(OneWord);
                     std::string segment;
                     std::vector<std::string> segmentArray;
                     while(std::getline(tempWord, segment, '/'))
                     {
                         segmentArray.push_back(segment);
                     }
                     index = std::stoi(segmentArray[0]);
                     if(segmentArray[1] != "")
                         uv = std::stoi(segmentArray[1]);
                     else
                         uv = 0;

                     normal = std::stoi(segmentArray[2]);

                     --index;
                     --uv;
                     --normal;

                     if(uv > -1)
                     {
                         Vertex tempVert(tempVertices[index], tempNormals[normal], tempUVs[uv]);
                         mVertices.push_back(tempVert);
                     } else {

                         Vertex tempVert(tempVertices[index], tempNormals[normal], gsl::Vector2D(0,0));
                         mVertices.push_back(tempVert);
                     }
                     mIndices.push_back(temp_index++);
                 }
               continue;
             }
         }
     }
     qDebug()<<"Closing file";
     inn.close();
}

void ObjLoader::init(GLint matrixUniform)
{
    mMatrixUniform =matrixUniform;
    initializeOpenGLFunctions();

    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );
        glBufferData( GL_ARRAY_BUFFER,      //what buffer type
                      mVertices.size()*sizeof( Vertex ),   //how big buffer do we need
                      mVertices.data(),             //the actual vertices
                      GL_STATIC_DRAW        //should the buffer be updated on the GPU
                      );

        glBindBuffer(GL_ARRAY_BUFFER, mVBO);
        glVertexAttribPointer(
                    0,                  // attribute. No particular reason for 0, but must match layout(location = 0) in the vertex shader.
                    3,                  // size / number of elements of data type
                    GL_FLOAT,           // data type
                    GL_FALSE,           // normalize data
                    sizeof(Vertex),  // stride
                    reinterpret_cast<GLvoid*>(0)  );          // array buffer offset
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)) );
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  reinterpret_cast<GLvoid*>(6 * sizeof(GLfloat)) );
        glEnableVertexAttribArray(2);

        glGenBuffers ( 1, &mEAB );
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEAB);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size()*sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);

        glBindVertexArray(0);
}

void ObjLoader::draw()
{
    glBindVertexArray( mVAO );
    //glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, nullptr);
}
