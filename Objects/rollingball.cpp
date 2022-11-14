#include "rollingball.h"
#include <QDebug>


RollingBall::RollingBall(int n, TriangleSurface* ts) : OctahedronBall(n), ts(ts)
{
    MinMax = Vertex::findMinMaxXZ(vertices); // finner min og maks punkter ifra Triangle Surface

     float MinX = MinMax[0]; //avsetter punkter inn i floats ifra en std::vektor
     float MaxX = MinMax[1];
     float MinZ = MinMax[2];
     float MaxZ = MinMax[3];

 //    float rangeX = MinX + static_cast<float>(std::rand() / static_cast<float>(MaxX - MinX + 1));
     float rangeX = MinX + (int)(std::rand() % (int)(MaxX - MinX + 1)); //lager random x og z coordinater
     float rangeZ = MinZ + (int)(std::rand() % (int)(MaxZ - MinZ + 1));
     float rangeY = 200 + (std::rand() % (15 - 5 + 1)); // Y plaseres over bakken men får en tilfeldig ekstra høydebonus

     mPos = {rangeX,rangeY,rangeZ};
     v = {0,0,0};

     mMatrix.setPosition(mPos.getX(),mPos.getY(),mPos.getZ());
     mScale.scale(allScale,allScale,allScale);
    //mVelocity = gsml::Vector3d{1.0f, 1.0f, -0.05f};
//    mPosition.translate(mPos);
    mMatrix.setPosition(mPos.getX(),mPos.getY(),mPos.getZ()); // finner posisjonen til Ballen vår / erklært i header

    mScale.scale(5,5,5); // setter scale. // må ha denne verdien // pga koden min //// har prøvd å fjerne, endte i error så lar stå
}
RollingBall::~RollingBall()
{

}
void RollingBall::move(float dt)
{
   // std::vector<Vertex>& vertices = dynamic_cast<VSflate*>(Vsflate)->getVertices(); //henter vertecies ifra renderwindow(vsflate object)
    // Finne trekant vertecies med Indeks

    // Finn ballens posisjon i xy−planet
    gsl::Vector2D Pos2D = {mMatrix.getPosition().getX(), mMatrix.getPosition().getZ()}; // posisjonen til ballen blir lagret i 2d Vector

    for (int i = 0; i < vertices.size(); i += 3) // trenger bare å finne hver 3 vertecies vil ikke at den skal ta de samme punktene flere ganger
    {
        // Finn trekantens vertices v0, v1, v2
        Vertex v0 = vertices[i];
        Vertex v1 = vertices[i+1];
        Vertex v2 = vertices[i+2];

        // Søk etter triangel som ballen er på nå med barysentriske koordinater
        gsl::Vector3D bary = BarycCoordinates({v0.m_xyz[0], v0.m_xyz[2]}, {v1.m_xyz[0], v1.m_xyz[2]}, {v2.m_xyz[0], v2.m_xyz[2]}, Pos2D);
        bary = {bary.getX(), bary.getY() , bary.getZ() };

        //ser at ballen er på en trekant ved å sjekke om kordinatene er mellom 0 og 1
        if(bary.getX() >= 0 && bary.getX() <= 1 && bary.getY() >= 0 && bary.getY() <= 1 && bary.getZ() >= 0 && bary.getZ() <= 1)
        {
            float yCoord = 0; //dersom vi fant trekanten, så finner vi høyden som tilsvarer coordinatet til ballen
            yCoord += v0.m_xyz[1] * bary.getX();
            yCoord += v1.m_xyz[1] * bary.getY();
            yCoord += v2.m_xyz[1] * bary.getZ();

            if(yCoord>=(mMatrix.getPosition().getY()-0.35)) //ser om ballen er på samme høyde som terrenget // tar minus en summ for å ta ifra hvor mye ballen går igjenom flaten
              {

                int thisIndex = i;
                // beregne normal
                // gjør v0, v1 og v2 til vektorer og regner funskjon ((v1-v0)(v2-v0)) Krysspunkt pga scalar
                gsl::Vector3D thisNormalVector = gsl::Vector3D::cross(v1.to3DVec()-v0.to3DVec(), v2.to3DVec()-v0.to3DVec());
                thisNormalVector.normalize();

                // beregn akselerasjonsvektor−ligning( 7 )
                gsl::Vector3D a = FinnAkselerasjonsvektor(thisNormalVector); // går til akelerasjon og finner a


                // Oppdaterer hastighet og posisjon
                v += a;
                mMatrix.translate(v);
                ////kollisjons detektering
                if (thisIndex != oldIndex)
                  {
//                    // Ballen har rullet over pa nytt triangel
//                    // Beregner normalen til kollisjonsplanet, se ligning ( 9 )
//                    //x = (m+n)/(|m+n|)
                    gsl::Vector3D m = gsl::Vector3D::cross(vertices[oldIndex+1].to3DVec()-vertices[oldIndex].to3DVec(),vertices[oldIndex+2].to3DVec()-vertices[oldIndex+0].to3DVec());
                    m.normalize();
                    gsl::Vector3D normalvector = {(m+thisNormalVector)/(m+thisNormalVector).length()};
//                    // Korrigere posisjon oppover i normalens retning
//                    // Oppdater hastighetsvektoren, se ligning( 8 )
//                    gsl::Vector3D a = FinnAkselerasjonsvektor(normalvector);
                    float vn = gsl::Vector3D::dot(v,normalvector) * 2;
                    gsl::Vector3D temp = {vn * normalvector.getX(), vn * normalvector.getY(), vn * normalvector.getZ()};

                    v=v-temp;
                    mMatrix.translate(v);
                    //qDebug()<<mMatrix.getPosition();
//                  // Oppdatere posisjon i retning den nye hastighetsvektoren
                    ////MANGLER
                  }
                // Oppdater gammel normal og indeks
                oldIndex = thisIndex;
                // normalvector = thisNormalVector;
              }else if(yCoord!=mMatrix.getPosition().getY()){
//        else{  //Her vil eg regne tyngdekraft
           float g = -0.1;
////                mPos.setY(mPos.getY() + g);
                mMatrix.translateY(g);
        }
       }
    }
}




gsl::Vector3D RollingBall::BarycCoordinates(const gsl::Vector2D& p1, const gsl::Vector2D& p2, const gsl::Vector2D& p3, const gsl::Vector2D& player)
{
    gsl::Vector2D p12 = p2-p1;
    gsl::Vector2D p13 = p3-p1;
    gsl::Vector3D n = n.cross(gsl::Vector3D{p13.x, 0, p13.y}, gsl::Vector3D{p12.x, 0, p12.y});

    float areal_123 = n.length(); // dobbelt areal
    gsl::Vector3D baryc; // til retur. Husk
    // u
    gsl::Vector2D p = p2 - player;
    gsl::Vector2D q = p3 - player;
    n = n.cross(gsl::Vector3D{q.x, 0, q.y}, gsl::Vector3D{p.x, 0, p.y});

    baryc.x = n.y/areal_123;
    // v
    p = p3 - player;
    q = p1 - player;

    n = n.cross(gsl::Vector3D{q.x, 0, q.y}, gsl::Vector3D{p.x, 0, p.y});

    baryc.y = n.y/areal_123;

    // w
    p = p1 - player;
    q = p2 - player;
    n = n.cross(gsl::Vector3D{q.x, 0, q.y}, gsl::Vector3D{p.x, 0, p.y});

    baryc.z = n.y/areal_123;

    return baryc;
}

gsl::Vector3D RollingBall::FinnAkselerasjonsvektor(const gsl::Vector3D& normalvector)
{

//    g[nx · nz, ny · nz, n^2z - 1]
    gsl::Vector3D output = {normalvector.getX() * normalvector.getY(), (normalvector.getY() * normalvector.getY()) - 1, normalvector.getZ() * normalvector.getY()};
    return {output.getX()*gr, output.getY()*gr,output.getZ()*gr};
}


void RollingBall::init(GLint matrixUniform)
{
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

void RollingBall::draw()
{
   glBindVertexArray( mVAO );
   glUniformMatrix4fv( mMatrixUniform, 1, GL_TRUE, mMatrix.constData());
   glDrawArrays(GL_TRIANGLES, 0, mVertices.size());//mVertices.size());
}
