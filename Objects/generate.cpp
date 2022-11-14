#include "generate.h"
#include <iostream>
#include <fstream>

Generate::Generate(): VisualObject()
{
//readFile("../3Dprog22/Coords.txt");
    writeFile("../3Dprog22/Coords.txt");
qDebug()<<n;

}

Generate::Generate(std::string filnavn) : VisualObject()
{
readFile(filnavn);
}

Generate::~Generate()
{

}

void Generate::readFile(std::string filnavn)
{
    std::ifstream inn;
     inn.open(filnavn);
         qDebug()<<"startet reading";
     if (inn.is_open())
     {
         qDebug()<<"Helo";

//         mVertices.reserve(n); // oppdaterer maks grense
        while (inn >> CordX >> CordZ >> CordY)
        {

//              if(inn.eof())break;
                LevelCounter++;
                //qDebug()<<CordX<<CordY<<CordZ;
                vertex.m_xyz[0]=CordX;
                vertex.m_xyz[1]=CordZ;
                vertex.m_xyz[2]=CordY;
                //mVertices.push_back(vertex);
                //qDebug()<<vertex.m_xyz[0]<<vertex.m_xyz[1]<<vertex.m_xyz[2];

                if(MinCordX==0)
                {
                    MinCordX=CordX;
                }
                else if(CordX<MinCordX)//litt problemer her pga start verdi er 0, har ikke endret siden jeg har all informasjon jeg trenger. Skal være enkel fiks
                {
                    MinCordX=CordX;
                }
                 if(CordX>MaxCordX)
                {
                    MaxCordX=CordX;
                }
                 if(MinCordZ==0)
                 {
                     MinCordZ=CordZ;
                 }
                else if(CordZ<MinCordZ)
                {
                    MinCordZ=CordZ;
                }
                if(CordZ>MaxCordZ)
                {
                    MaxCordZ=CordZ;  //midpoint ==
                }


//              mVertices.push_back(vertex);
         }
              qDebug()<<"Number of lines ="<<LevelCounter
                      <<"Max X ="<<MaxCordX
                      <<"Min X ="<< MinCordX
                      <<"Max Z ="<< MaxCordZ
                     <<"Min Z ="<< MinCordZ;
     }
     else  qDebug()<<"Unable to open file";
     inn.close();
}

void Generate::writeFile(std::string filnavn){

    float MaxX = 613600;
    float MinX = 612000;

    float MaxZ = 6743400;
    float MinZ = 6742200;

    float MaxY = 237;
    float MinY = 103;


    float rangeX = MaxX-MinX;
    float rangeZ = MaxZ-MinZ;

    int divide = 40;
    float CellLenght = rangeX/divide;
    float CellHeight = rangeZ/divide;

    qDebug()<<rangeX;
    qDebug()<<rangeZ;
        qDebug()<<CellHeight;
        qDebug()<<CellLenght;

    std::vector<gsl::Vector2D> cells; // deler arealet inn i mindre ruter
    for(int i = 0; i < divide-1; i++)// = z
    {
        for(int j = 0; j < divide-1; j++)// = x
        {
            cells.push_back({CellLenght*j+MinX, CellHeight*i+MinZ});
                qDebug()<< "cell x and y"<<CellLenght*j+MinX << CellHeight*i+MinZ;
        }
    }

// finding midpoints
    std::vector<std::pair<gsl::Vector2D, std::vector<float>>> CoordList;

    for(int i = 0; i < cells.size(); i++)
    {
        std::pair<gsl::Vector2D, std::vector<float>> temp;
        temp.first = cells[i];
        temp.first.x += CellLenght/2;
        temp.first.y += CellHeight/2;
        CoordList.push_back(temp);
        qDebug()<<"midpoint x and y"<< temp.first.x << temp.first.y;
    }

    float x;
    float y;
    float z;

    std::ifstream inn;
     inn.open(filnavn);
         qDebug()<<"startet reading";
     if (inn.is_open())
     {
         qDebug()<<"found file :D";

//         mVertices.reserve(n); // oppdaterer maks grense
        while (inn >> x >> z >> y)
        {
            for(int i = 0;i< cells.size(); i++)
            {

                if(x >= cells[i].x && z>=cells[i].y && x <= cells[i].x + CellLenght && z <= cells[i].y + CellHeight)
                {

                    CoordList[i].second.push_back(y);
                    break;
                }
            }
        }
     }
        inn.close();

    std::fstream out;
    out.open("../3Dprog22/CompressedFile.txt", std::ios::out);
             qDebug()<<"started Writing";
    if(out.is_open()){
    divide --;
        for(int mz = 0; mz < divide - 1; mz++)
        {
            for(int mx = 0; mx < divide - 1; mx++)
            {
                int it = mz * divide + mx;
                out << "(" << CoordList[it].first.x-MinX << ", " << FindAverage(CoordList[it].second) << ", " << CoordList[it].first.y-MinZ << ")"
                                      << "(" << (FindAverage(CoordList[it].second)-MinY)/rangeY << ", " << 1-(FindAverage(CoordList[it].second)-MinY)/rangeY << ", " << (FindAverage(CoordList[it].second)-MinY)/rangeY << ") "
                                      << "(" << 0 << ", " << 0 << ") " << "\n";
                out << "(" << CoordList[it+1].first.x-MinX << ", " << FindAverage(CoordList[it+1].second) << ", " << CoordList[it+1].first.y-MinZ << ")"
                                      << "(" << (FindAverage(CoordList[it].second)-MinY)/rangeY << ", " << 1-(FindAverage(CoordList[it].second)-MinY)/rangeY << ", " << (FindAverage(CoordList[it].second)-MinY)/rangeY << ") "
                                      << "(" << 0 << ", " << 0 << ") " << "\n";
                out << "(" << CoordList[it+divide+1].first.x-MinX << ", " << FindAverage(CoordList[it+divide+1].second) << ", " << CoordList[it+divide+1].first.y-MinZ << ")"
                                      << "(" << (FindAverage(CoordList[it].second)-MinY)/rangeY << ", " << 1-(FindAverage(CoordList[it].second)-MinY)/rangeY << ", " << (FindAverage(CoordList[it].second)-MinY)/rangeY << ") "
                                      << "(" << 0 << ", " << 0 << ") " << "\n";
                out << "(" << CoordList[it+divide+1].first.x-MinX << ", " << FindAverage(CoordList[it+divide+1].second) << ", " << CoordList[it+divide+1].first.y-MinZ << ")"
                                      << "(" << (FindAverage(CoordList[it].second)-MinY)/rangeY << ", " << 1-(FindAverage(CoordList[it].second)-MinY)/rangeY << ", " << (FindAverage(CoordList[it].second)-MinY)/rangeY << ") "
                                      << "(" << 0 << ", " << 0 << ") " << "\n";
                out << "(" << CoordList[it+divide].first.x-MinX << ", " << FindAverage(CoordList[it+divide].second) << ", " << CoordList[it+divide].first.y-MinZ << ")"
                                      << "(" << (FindAverage(CoordList[it].second)-MinY)/rangeY << ", " << 1-(FindAverage(CoordList[it].second)-MinY)/rangeY << ", " << (FindAverage(CoordList[it].second)-MinY)/rangeY << ") "
                                      << "(" << 0 << ", " << 0 << ") " << "\n";
                out << "(" << CoordList[it].first.x-MinX << ", " << FindAverage(CoordList[it].second) << ", " << CoordList[it].first.y-MinZ << ")"
                                      << "(" << (FindAverage(CoordList[it].second)-MinY)/rangeY << ", " << 1-(FindAverage(CoordList[it].second)-MinY)/rangeY << ", " << (FindAverage(CoordList[it].second)-MinY)/rangeY << ") "
                                      << "(" << 0 << ", " << 0 << ") " << "\n";
            }
        }
    }
    out.close();
}

float Generate::FindAverage(const std::vector<float>& a)
{
    //if(a.empty()) return 0;

    float average = 0;
    for(int j = 0; j < a.size(); j++)
        average += a[j];
    average = average/a.size();
    return average;
}

void Generate::init(GLint matrixUniform) {
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

void Generate::draw()
{
   glBindVertexArray( mVAO );
   glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
   glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
}
