#include "disc.h"

Disc::Disc()
{
    construct();
//    writeFile("D:/3Dprog22/disc_test.txt");
//    mMatrix.setToIdentity();
}

Disc::Disc(std::string filnavn)
{
    construct();
    writeFile(filnavn);
    readFile(filnavn);
    mPosition.setToIdentity();
    mRotation.setToIdentity();
    mScale.setToIdentity();
       mMatrix.setToIdentity();
}

Disc::~Disc()
{

}

void Disc::readFile(std::string filnavn)
{
    std::ifstream inn; // ("../3Dprog22/test.txt")
    //inn.open(filnavn.c_str());

    if (inn.is_open()) {
        int n;
        Vertex vertex;
        inn >> n;
        mVertices.reserve(n);
        for (int i=0; i<n; i++) {
             inn >> vertex;
             mVertices.push_back(vertex);
        }

    }else{
        std::cerr << "ERROR: File not open";
    }
    inn.close();
}

void Disc::writeFile(std::string filnavn)
{
    std::ofstream ut;
    ut.open(filnavn.c_str());

    ut << mVertices.size();
    for (auto it=mVertices.begin(); it!=mVertices.end(); it++)
        ut << *it << std::endl;
    ut.close();
}

void Disc::init(GLint matrixUniform)
{
   ///
   mMatrixUniform = matrixUniform;
   initializeOpenGLFunctions();

   //Vertex Array Object - VAO
   glGenVertexArrays( 1, &mVAO );
   glBindVertexArray( mVAO );

   //Vertex Buffer Object to hold vertices - VBO
   glGenBuffers( 1, &mVBO );
   glBindBuffer( GL_ARRAY_BUFFER, mVBO );
   glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof(Vertex), mVertices.data(), GL_STATIC_DRAW );

   // 1rst attribute buffer : vertices
   glBindBuffer(GL_ARRAY_BUFFER, mVBO);
   glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE,sizeof(Vertex), reinterpret_cast<const void*>(0));
   glEnableVertexAttribArray(0);

   // 2nd attribute buffer : colors
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  reinterpret_cast<const void*>(3 * sizeof(GLfloat)) );
   glEnableVertexAttribArray(1);

   //enable the matrixUniform
   // mMatrixUniform = glGetUniformLocation( matrixUniform, "matrix" );
   glGenBuffers(1, &mIBO);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size()*sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);

   glBindVertexArray(0);

}


void Disc::draw()
{
   initializeOpenGLFunctions();
   glBindVertexArray( mVAO );
   // GL_FALSE for QMatrix4x4
   glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
   //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
   glDrawElements(GL_TRIANGLE_FAN, mVertices.size(), GL_UNSIGNED_INT, reinterpret_cast<const void*>(0));//mVertices.size());
}


void Disc::construct()
{
   mVertices.push_back(Vertex{0,0,0,1,1,1});
   for (int i=0; i<13; i++)
   {
       double angle = 30 * i * M_PI / 180;
       qDebug() << "angle = " << angle;
       float x = cos(angle);
       float y = sin(angle);
       float z = 0.0f;
       float r = i%2;
       float g = 0.f;
       float b = 0.5f;
       mVertices.push_back(Vertex{x,y,z,r,g,b});
   }
   for (GLuint i=0; i<14; i++) mIndices.push_back(i);
}


//void Disc::move(float dt)
//{
//   float degrees = (180 * dt) / M_PI;
//   mRotation.rotate(degrees, 0, 0, 1);
//   mMatrix = mRotation;
//}

void Disc::move(float dt)
{
  QVector3D ds=mVelocity*dt;

   //mPosition = mPosition + ds;		// hvis mPosisjon er Vector3d
  mPosition.translate(ds.x(), ds.y(), ds.z());	// hvis mPosisjon er Matrix4x4

  // normalen kan generelt være en parameter inn
  QVector3D normal = QVector3D{0.0f, 1.0f, 0.0f};

  // bruker kryssprodukt for å finne rotasjonsvektor
  QVector3D rotation = QVector3D::crossProduct(normal, mVelocity);
  rotation.normalize();

  // bruk formelen ds = r dt ==> dt = ds/r
  // for å finne ut hvor mye hjulet har rotert
  // og oppdater rotasjonsmatrisen
  // husk å starte med mRotation som identitetsmatrise

  float degrees = (180 * dt) / M_PI;
    mRotation.rotateZ(degrees);

  mMatrix = mPosition*mRotation;		// hvis mPosition og mRotation er Matrix4x4
}


