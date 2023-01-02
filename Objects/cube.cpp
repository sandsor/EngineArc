#include "cube.h"
#include "vector3d.h"
Cube::Cube()
{

    construct();
    mMatrix.setToIdentity();


}

Cube::~Cube()
{

}

void Cube::construct()
{
    mVertices.push_back(Vertex{ -0.5, -0.5, -0.5,1,0,0}); // LSW 0
    mVertices.push_back(Vertex{ 0.5, -0.5, -0.5,0,1,0}); // LSE 1
    mVertices.push_back(Vertex{ 0.5, -0.5, 0.5,0,0,1}); // LNE 2
    mVertices.push_back(Vertex{ -0.5, -0.5, 0.5,1,1,0}); // LNW 3

    mVertices.push_back(Vertex{ -0.5, 0.5, -0.5,1,0,1}); // USW 4
    mVertices.push_back(Vertex{ 0.5, 0.5, -0.5,1,1,0}); // USE 5
    mVertices.push_back(Vertex{ 0.5, 0.5, 0.5,1,1,1,}); // UNE 6
    mVertices.push_back(Vertex{ -0.5, 0.5, 0.5,0,0,0 }); // UNW 7

    mIndices =
    {
        // Bottom
        0, 1, 2,
        0, 2, 3,

        // Sides
        0, 1, 5,
        0, 5, 4,

        1, 2, 6,
        1, 6, 5,

        2, 3, 7,
        2, 7, 6,

        3, 0, 4,
        3, 4, 7,

        // Top
        4, 5, 6,
        4, 6, 7
    };
}

void Cube::init(GLint matrixUniform) {
    mMatrixUniform = matrixUniform;
    initializeOpenGLFunctions();

    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER,      //what buffer type
        mVertices.size() * sizeof(Vertex),   //how big buffer do we need
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
        reinterpret_cast<GLvoid*>(0));          // array buffer offset
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glGenBuffers(1, &mEAB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEAB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}
void Cube::draw()
{
    glBindVertexArray(mVAO);
    //glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, nullptr);

}

void Cube::ChangeColor(float r, float g, float b)
{
    for (int i = 0; i < mVertices.size(); i++)
    {
        Vertex tempVertex = mVertices[i];
        tempVertex.m_normal[0] = r;
        tempVertex.m_normal[1] = g;
        tempVertex.m_normal[2] = b;
        mVertices[i] = tempVertex;

    }
}

void Cube::move(float x, float y, float z)
{

    mMatrix.translate(x, y, z);
    mPosition=mMatrix;
}

void Cube::Scale(float scale)
{
    mMatrix.scale(scale);
    mScale = scale;
    
}

void Cube::RotateX(float x)
{
    mMatrix.rotateX(x);
}

void Cube::RotateY(float y)
{
    mMatrix.rotateY(y);
}

void Cube::RotateZ(float z)
{
    mMatrix.rotateZ(z);
}

void Cube::RotateXYZ(float x, float y, float z)
{
    RotateX(x);
    RotateY(y);
    RotateZ(z);
}
