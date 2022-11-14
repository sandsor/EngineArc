#include "vsflate.h"

VSflate::VSflate()
{
    mVertices.push_back(Vertex{-20.2,0,20.2,1,0,0   ,0,1});
    mVertices.push_back(Vertex{0,0,0,       1,0,0   ,0,1});
    mVertices.push_back(Vertex{-20.2,5,0,   1,0,0   ,0,1}); //Triangle 1 , Side 1

    mVertices.push_back(Vertex{0,2.325,20.2,  1,1,0   ,0,1});
    mVertices.push_back(Vertex{0,0,0,       1,1,0   ,0,1});
    mVertices.push_back(Vertex{-20.2,0,20.2,1,1,0   ,0,1}); //Triangle 2

    mVertices.push_back(Vertex{20.2,0,20.2, 0,1,0   ,0,1});
    mVertices.push_back(Vertex{0,0,0,       0,1,0   ,0,1});
    mVertices.push_back(Vertex{0,2.325,20.2,  0,1,0   ,0,1}); //Triangle 3 , Side 2

    mVertices.push_back(Vertex{20.2,3.75,0,  0,1,1   ,0,1});
    mVertices.push_back(Vertex{0,0,0,       0,1,1   ,0,1});
    mVertices.push_back(Vertex{20.2,0,20.2, 0,1,1   ,0,1}); //Triangle 4
    mMatrix.setToIdentity();

}

VSflate::~VSflate()
{

}

void VSflate::init(GLint matrixUniform) {
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
void VSflate::draw()
{
    glBindVertexArray( mVAO );
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
}
