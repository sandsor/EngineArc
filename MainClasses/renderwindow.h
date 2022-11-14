#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H

#include <QWindow>
#include <QOpenGLFunctions_4_1_Core>
#include <QTimer>
#include <QElapsedTimer>
#include <unordered_map>

#include <vector>
//#include "quadtre.h"
#include "camera.h"

#include "quadtree.h"

#include "vertex.h"
#include "xyz.h"
#include "cube.h"
#include "trianglesurface.h"
#include "visualobject.h"
#include "interactiveobject.h"
#include "octahedronball.h"
#include "disc.h"
#include "house.h"
#include "trofee.h"
#include "door.h"
#include "npc.h"
#include "visualcurve.h"
#include "plan.h"
#include "light.h"
#include "terrain.h"
#include "vsflate.h"
#include "rollingball.h"
#include "generate.h"
#include "objloader.h"

#include "SoundDevice.h"
#include "SoundBuffer.h"
#include "SoundSource.h"


class QOpenGLContext;
class Shader;
class MainWindow;
class Texture;

/// This inherits from QWindow to get access to the Qt functionality and
// OpenGL surface.
// We also inherit from QOpenGLFunctions, to get access to the OpenGL functions
// This is the same as using "glad" and "glw" from general OpenGL tutorials
class RenderWindow : public QWindow, protected QOpenGLFunctions_4_1_Core
{
    Q_OBJECT
public:
    RenderWindow(const QSurfaceFormat &format, MainWindow *mainWindow);
    ~RenderWindow() override;

    QOpenGLContext *context() { return mContext; }

    void exposeEvent(QExposeEvent *) override;  //gets called when app is shown and resized

    bool mRotate{true};     //Check if triangle should rotate

    bool mMove{true};     //Check if triangle should move

    class Light *mLight{nullptr};

    void spawnrain();
    void spawnrainDrop(float a, float b, float c);

    void ToggleWireframe(bool checked);
    void ResetCamera();

private slots:
    void render();          //the actual render - function

private:

    std::vector<VisualObject*> mObjects;
    std::unordered_map<std::string, VisualObject*> mMap;    // alternativ container
    QuadTree* mQuadTre;

   // gsml::QuadTre<std::string, VisualObject*> mQuadTre;
    //VisualObject* mio;
    Camera mCamera;
    InteractiveObject* mio;
    Disc* disc;
    Trofee* trof1;
    Trofee* trof2;
    Trofee* trof3;
    Trofee* trof4;
    Trofee* trof5;
    Trofee* trof6;
    Trofee* trof7;
    House* house;
    VSflate* Vsflate;
    NPC* npc;
    VisualCurve* curve;
    VisualCurve* curve1;
    RollingBall* RB;
    Terrain* terrain{nullptr};
    std::vector<RollingBall*> Rain;
    Generate* map;
    TriangleSurface* Triangle;
    ObjLoader* Obj;

    //**************************************************

    SoundDevice* mysounddevice = SoundDevice::get(); 

    uint32_t /*ALuint*/ sound1 = SoundBuffer::get()->addSoundEffect("D:\\diverse\\spell.ogg");
    uint32_t /*ALuint*/ sound2 = SoundBuffer::get()->addSoundEffect("D:\\diverse\\magicfail.ogg");

    SoundSource mySpeaker;

    //**************************************************


    void init();            //initialize things we need before rendering

    //std::vector<Vertex> mVertices;  //sjekk om denne er nødvendig************

    //*************************************************************************

    QOpenGLContext *mContext{nullptr};  //Our OpenGL context
    bool mInitialized{false};

    //Shader *mShaderProgram{nullptr};    //holds pointer the GLSL shader program
    GLint  mPmatrixUniform;
    GLint  mVmatrixUniform;
    GLint  mMmatrixUniform;

    GLuint mVAO;                        //OpenGL reference to our VAO
    GLuint mVBO;                        //OpenGL reference to our VBO

    QMatrix4x4 *mMVPmatrix{nullptr};         //The matrix with the transform for the object we draw
    QMatrix4x4 *mPmatrix{nullptr};         // denne,
    QMatrix4x4 *mVmatrix{nullptr};         // og denne

    QTimer *mRenderTimer{nullptr};           //timer that drives the gameloop
    QElapsedTimer mTimeStart;               //time variable that reads the calculated FPS

    MainWindow *mMainWindow{nullptr};        //points back to MainWindow to be able to put info in StatusBar

    class QOpenGLDebugLogger *mOpenGLDebugLogger{nullptr};  //helper class to get some clean debug info from OpenGL
    class Logger *mLogger{nullptr};         //logger - Output Log in the application

    //*************************************************************************

    ///Helper function that uses QOpenGLDebugLogger or plain glGetError()
    void checkForGLerrors();

    void calculateFramerate();          //as name says

    ///Starts QOpenGLDebugLogger if possible
    void startOpenGLDebugger();

    void setupPlainShader(int shaderIndex);
    GLint mMatrixUniform0{-1};
    GLint vMatrixUniform0{-1};
    GLint pMatrixUniform0{-1};

    void setupTextureShader(int shaderIndex);
    GLint mMatrixUniform1{-1};
    GLint vMatrixUniform1{-1};
    GLint pMatrixUniform1{-1};
    GLint mTextureUniform{-1};

    void setupPhongShader(int shaderIndex);
    GLint mMatrixUniform2{-1};
    GLint vMatrixUniform2{-1};
    GLint pMatrixUniform2{-1};

    //other light shader variables
    GLint mLightColorUniform{-1};
    GLint mObjectColorUniform{-1};
    GLint mAmbientLightStrengthUniform{-1};
    GLint mLightPositionUniform{-1};
    GLint mCameraPositionUniform{-1};
    GLint mSpecularStrengthUniform{-1};
    GLint mSpecularExponentUniform{-1};
    GLint mLightPowerUniform{-1};
    GLint mTextureUniform2{-1};

    Texture *mTexture[4]{nullptr};
    Shader *mShaderProgram[4]{nullptr};

    Camera *mCurrentCamera;
    float mAspectratio{1.f};

    std::vector<VisualObject*> mVisualObjects;

protected:
    //The QWindow that we inherit from have these functions to capture
    // - mouse and keyboard.
    // Uncomment to use (you also have to make the definitions of
    // these functions in the cpp-file to use them of course!)
    //
    void mousePressEvent(QMouseEvent *event) override;
    //    void mouseMoveEvent(QMouseEvent *event) override{}
    void keyPressEvent(QKeyEvent *event) override;              //the only one we use now
    //    void keyReleaseEvent(QKeyEvent *event) override{}
    //    void wheelEvent(QWheelEvent *event) override{}
};

#endif // RENDERWINDOW_H
