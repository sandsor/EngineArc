#include "plan.h"

Plan::Plan()
{

    mVertices.push_back(Vertex{0.f, 0.f, 0.f,       1.f,1.f,1.f,     1.f,0.f}); //  A
    mVertices.push_back(Vertex{0.f, 1.f, 0.f,       1.f,1.f,1.f,     1.f,1.f}); //  B
    mVertices.push_back(Vertex{-1.f, 0.f, 0.f,       1.f,1.f,1.f,     0.f,0.f}); //  C


    mVertices.push_back(Vertex{-1.f, 0.f, 0.f,       1.f,1.f,1.f,     0.f,0.f}); //  D
    mVertices.push_back(Vertex{0.f, 1.f, 0.f,       1.f,1.f,1.f,     1.f,1.f}); //  E
    mVertices.push_back(Vertex{-1.f, 1.f, 0.f,       1.f,1.f,1.f,     0.f,1.f}); //  F

    mMatrix.setToIdentity();
}

Plan::~Plan(){}

void Plan::init(GLint matrixUniform)
{
    mMatrixUniform = matrixUniform;

    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );

    glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof( Vertex ), mVertices.data(), GL_STATIC_DRAW );

    // 1rst attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof( Vertex ),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : uvs
    glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)( 6 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(2);

    //enable the matrixUniform
    // mMatrixUniform = glGetUniformLocation( matrixUniform, "matrix" );

    glBindVertexArray(0);
}

void Plan::draw()
{
    glBindVertexArray( mVAO );
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
}
