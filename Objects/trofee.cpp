#include "trofee.h"
//#include <fstream>

Trofee::Trofee()
{
    Vertex k1(0,-0.3,-0.107,1,0,0);
    Vertex k2(0.15,-0.3,0.053,0,0,1);
    Vertex k3(-0.15,-0.3,0.053,0,1,0);
    Vertex k4(0,-0.153,0,1,1,0);

    std::vector<Vertex> corners{k1,k2,k3,k4};

    for(int i=1;i<3;i++)
        for(int j=(i+1);j<4;j++)
            if(i==2 && j==4)
                break;
            else
            {
                mVertices.push_back((corners[0]));
                mVertices.push_back((corners[i]));
                mVertices.push_back((corners[j]));
            }

    mVertices.push_back((corners[1]));
    mVertices.push_back((corners[2]));
    mVertices.push_back((corners[3]));
       mMatrix.setToIdentity();
}

Trofee::Trofee(std::string name, float mx, float my, float mz)
{

    m_name = name;
    Vertex k1(0,-0.3,-0.1732,1,0,0);
    Vertex k2(0.15,-0.3,0.0866,0,0,1);
    Vertex k3(-0.15,-0.3,0.0866,0,1,0);
    Vertex k4(0,-0.15,0,1,1,0);

    mMatrix.setToIdentity();
    mMatrix.translate(mx,my,mz);

    mPosition(0,3) = mx;
    mPosition(1,3) = my;
    mPosition(2,3) = mz;

    std::vector<Vertex> corners{k1,k2,k3,k4};

    for(int i=1;i<3;i++)
        for(int j=(i+1);j<4;j++)
            if(i==2 && j==4)
                break;
            else
            {
                mVertices.push_back((corners[0]));
                mVertices.push_back((corners[i]));
                mVertices.push_back((corners[j]));
            }

    mVertices.push_back((corners[1]));
    mVertices.push_back((corners[2]));
    mVertices.push_back((corners[3]));
     mMatrix.setToIdentity();
}

Trofee::~Trofee(){}

void Trofee::init(GLint matrixUniform)
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

void Trofee::draw()
{
    if(!bDraw)
            return;
        else{
    glBindVertexArray( mVAO );
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
    }
}

//void Trofee::readFile()
//{
//    std::string filnavn = "../3Dprog22/trofeModell.txt";
//std::ifstream inn;
// inn.open(filnavn.c_str());

// if (inn.is_open()) {
//     //int n;
//     Vertex vertex;
//     //inn >> n;
//     //mVertices.reserve(n);
//     while (true) {
//          inn >> vertex;
//          if(inn.eof())break;
//          mVertices.push_back(vertex);
//     }
//     inn.close();
// }
//}
