#pragma once
#include <QOpenGLFunctions_4_1_Core>
#include <QMatrix4x4>
#include <matrix4x4.h>
#include <vector>
#include <entt/core/type_traits.hpp>
#include "vertex.h"
#include <iostream>
#include <sstream>

struct TransformComponent
{
    gsl::Matrix4x4 mPosition;
    gsl::Matrix4x4 mRotation;
    gsl::Matrix4x4 mScale;
    gsl::Vector3D mVelocity{gsl::Vector3D{0}};
    TransformComponent() = default;
    TransformComponent(TransformComponent&) = default;
    TransformComponent(const gsl::Matrix4x4& transform)
        :mPosition(transform) {}
    operator const gsl::Matrix4x4& () { return mPosition; }
    void move(float x, float y, float z) { ; };
    void move(float dt);
    void move(gsl::Vector3D vec);
    void Rotate(gsl::Vector3D);
    void Scale(float scale);


};

struct  MeshComponent
{
    std::vector<Vertex> mVertices;
    std::vector<GLuint>mIndices;
    GLuint mVAO{ 0 };
    GLuint mVBO{ 0 };
    GLuint mEAB{ 0 };//holds indices
    GLint mMatrixUniform{ 0 };
    bool bDraw{ true };

    MeshComponent() = default;
    MeshComponent(std::string fileName)
    {
        ReadFile("../EngineArc/Assets/untitled.obj");
        
	    
    };
    void ReadFile(std::string filnavn)
    {
        std::ifstream inn;
        inn.open(filnavn);
        qDebug() << "startet reading";
        if (!inn)
            qDebug() << "Could not open file for reading" << QString::fromStdString(filnavn);
        if (inn.is_open())
        {
            //qDebug()<<"Helo";

            std::string OneLine;
            std::string OneWord;

            std::vector<gsl::Vector3D> tempVertices;
            std::vector<gsl::Vector3D> tempNormals;
            std::vector<gsl::Vector2D> tempUVs;

            unsigned int temp_index = 0;

            while (std::getline(inn, OneLine))
            {
                //qDebug()<<"inn while loop";
                std::stringstream sStream;

                sStream << OneLine;
                OneWord = "";
                sStream >> OneWord;

                if (OneWord == "#")
                    continue;
                if (OneWord == "")
                    continue;
                if (OneWord == "v")
                {
                    //qDebug()<<"OneWord = v";
                    gsl::Vector3D tempVertex;
                    sStream >> OneWord;
                    tempVertex.x = std::stof(OneWord);
                    sStream >> OneWord;
                    tempVertex.y = std::stof(OneWord);
                    sStream >> OneWord;
                    tempVertex.z = std::stof(OneWord);
                    //qDebug()<<tempVertex;
                    tempVertices.push_back(tempVertex);

                    continue;

                }
                if (OneWord == "vt")
                {
                    //qDebug()<<"OneWord = vt";
                    gsl::Vector2D tempUV;
                    sStream >> OneWord;
                    tempUV.x = std::stof(OneWord);
                    sStream >> OneWord;
                    tempUV.y = std::stof(OneWord);

                    tempUVs.push_back(tempUV);

                    continue;
                }
                if (OneWord == "vn")
                {
                    //qDebug()<<"OneWord = vn";
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
                if (OneWord == "f")
                {
                    //qDebug()<<"OneWord = f";
                    int index, normal, uv;
                    for (int i = 0; i < 3; i++)
                    {
                        sStream >> OneWord;
                        std::stringstream tempWord(OneWord);
                        std::string segment;
                        std::vector<std::string> segmentArray;
                        while (std::getline(tempWord, segment, '/'))
                        {
                            segmentArray.push_back(segment);
                        }
                        index = std::stoi(segmentArray[0]);
                        if (segmentArray[1] != "")
                            uv = std::stoi(segmentArray[1]);
                        else
                            uv = 0;

                        normal = std::stoi(segmentArray[2]);

                        --index;
                        --uv;
                        --normal;

                        if (uv > -1)
                        {
                            Vertex tempVert(tempVertices[index], tempNormals[normal], tempUVs[uv]);
                            mVertices.push_back(tempVert);
                        }
                        else {

                            Vertex tempVert(tempVertices[index], tempNormals[normal], gsl::Vector2D(0, 0));
                            mVertices.push_back(tempVert);
                        }
                        mIndices.push_back(temp_index++);
                    }
                    continue;
                }
            }
        }
        qDebug() << "Closing file";
        inn.close();
    }



};

struct TagComponent
{
    std::string Tag;

    TagComponent() = default;
    TagComponent(const TagComponent&) = default;
    TagComponent(const std::string& tag)
	    :Tag(tag){}

};

struct ObjLoaderComponent
{
    ObjLoaderComponent() = default;
    ObjLoaderComponent(std::string fileName);

};
