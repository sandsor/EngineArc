#include "renderwindow.h"
#include <QTimer>
#include <QMatrix4x4>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLDebugLogger>
#include <QKeyEvent>
#include <QStatusBar>
#include <QDebug>

#include <string>

#include "shader.h"
#include "mainwindow.h"
#include "logger.h"
//#include "visualobject.h"
#include "texture.h"

RenderWindow::RenderWindow(const QSurfaceFormat &format, MainWindow *mainWindow)
    : mContext(nullptr), mInitialized(false), mMainWindow(mainWindow)

{
    //This is sent to QWindow:
    setSurfaceType(QWindow::OpenGLSurface);
    setFormat(format);
    //Make the OpenGL context
    mContext = new QOpenGLContext(this);
    //Give the context the wanted OpenGL format (v4.1 Core)
    mContext->setFormat(requestedFormat());
    if (!mContext->create()) {
        delete mContext;
        mContext = nullptr;
        qDebug() << "Context could not be made - quitting this application";
    }

 //     mQuadTre.insert({"map", new QuadTre()})
        // **************************************
      Point2D a{-8, -8}, b{8, -8}, c{8, 8}, d{-8, 8}; // må gjøres ordentlig
      mQuadTre = new QuadTree(a,b,c,d);
      mQuadTre->subDivide(5);
      // **************************************

    //Make the gameloop timer:
    mRenderTimer = new QTimer(this);


}

RenderWindow::~RenderWindow()
{
    //cleans up the GPU memory
    //glDeleteVertexArrays( 1, &mVAO );
    //glDeleteBuffers( 1, &mVBO );

}


// Sets up the general OpenGL stuff and the buffers needed to render a triangle
void RenderWindow::init() {

    mLogger = Logger::getInstance();

    //Connect the gameloop timer to the render function:
    //This makes our render loop
    connect(mRenderTimer, SIGNAL(timeout()), this, SLOT(render()));
    //********************** General OpenGL stuff **********************

    //The OpenGL context has to be set.
    //The context belongs to the instanse of this class!
    if (!mContext->makeCurrent(this)) {
        mLogger->logText("makeCurrent() failed", LogType::REALERROR);
        return;
    }

    //just to make sure we don't init several times
    //used in exposeEvent()
    if (!mInitialized)
        mInitialized = true;

    //must call this to use OpenGL functions
    initializeOpenGLFunctions();

    //Print render version info (what GPU is used):
    //Nice to see if you use the Intel GPU or the dedicated GPU on your laptop
    // - can be deleted
    mLogger->logText("The active GPU and API:", LogType::HIGHLIGHT);
    std::string tempString;
    tempString += std::string("  Vendor: ") + std::string((char*)glGetString(GL_VENDOR)) + "\n" +
            std::string("  Renderer: ") + std::string((char*)glGetString(GL_RENDERER)) + "\n" +
            std::string("  Version: ") + std::string((char*)glGetString(GL_VERSION));

    //Print info about opengl texture limits on this GPU:
        int textureUnits;
        glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &textureUnits);
        tempString += std::string("  This GPU as ") + std::to_string(textureUnits) + std::string(" texture units / slots in total, \n");

        glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &textureUnits);
        tempString += std::string("  and supports ") + std::to_string(textureUnits) + std::string(" texture units pr shader");
    mLogger->logText(tempString);

    //Start the Qt OpenGL debugger
    //Really helpfull when doing OpenGL
    //Supported on most Windows machines - at least with NVidia GPUs
    //reverts to plain glGetError() on Mac and other unsupported PCs
    // - can be deleted
    startOpenGLDebugger();

    //********************** General OpenGL stuff: **********************
    glEnable(GL_DEPTH_TEST);            //enables depth sorting - must then use GL_DEPTH_BUFFER_BIT in glClear
    //    glEnable(GL_CULL_FACE);       //draws only front side of models - usually what you want - test it out!
    glClearColor(0.4f, 0.4f, 0.4f, 1.0f);    //gray color used in glClear GL_COLOR_BUFFER_BIT

    //set up alpha blending for textures
        glEnable(GL_BLEND);// you enable blending function
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //********************** Compile shaders: **********************
    //NB: hardcoded path to files! You have to change this if you change directories for the project.
    //Qt makes a build-folder besides the project folder. That is why we go down one directory
    // (out of the build-folder) and then up into the project folder.
        mShaderProgram[0] = new Shader("../EngineArc/plainshader.vert", "../EngineArc/plainshader.frag");
        mLogger->logText("Plain shader program id: " + std::to_string(mShaderProgram[0]->getProgram()) );
        mShaderProgram[1]= new Shader("../EngineArc/textureshader.vert", "../EngineArc/textureshader.frag");
        mLogger->logText("Texture shader program id: " + std::to_string(mShaderProgram[1]->getProgram()) );
        mShaderProgram[2]= new Shader("../EngineArc/phongshader.vert", "../EngineArc/phongshader.frag");
        mLogger->logText("Texture shader program id: " + std::to_string(mShaderProgram[2]->getProgram()) );


        setupPlainShader(0);
        setupTextureShader(1);
        setupPhongShader(2);

        //********************** Texture stuff: **********************
        //Returns a pointer to the Texture class. This reads and sets up the texture for OpenGL
        //and returns the Texture ID that OpenGL uses from Texture::id()
        mTexture[0] = new Texture();
        mTexture[1] = new Texture("../EngineArc/Assets/snail.bmp");
        mTexture[2] = new Texture("../EngineArc/Assets/heightmap.bmp");

        //Set the textures loaded to a texture unit (also called a texture slot)
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mTexture[0]->id());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, mTexture[1]->id());
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, mTexture[2]->id());

        //********************** Making the object to be drawn **********************





        //Light:
//        mLight = new Light();
//        mLight->init(mMatrixUniform0);
//        mLight->mMatrix.translate(-1.f, 1.5f, 1.f);
//        mVisualObjects.push_back(mLight);

//        mLight = new Light(); //[1]
//        mLight->init(mMatrixUniform0);
//        mLight->mMatrix.scale(1.f);
//        mLight->mMatrix.translate(3.0f, 130.f, 0.5f);
//        mVisualObjects.push_back(mLight);

        Vsflate = new VSflate();
        Vsflate->init(mMatrixUniform0);
        //Vsflate->mMatrix.scale(0.5);
        mVisualObjects.push_back(Vsflate);

        Triangle = new TriangleSurface("../EngineArc/CompressedFile.txt");
        Triangle->init(mMatrixUniform0);
        mVisualObjects.push_back(Triangle);

        RB = new RollingBall(3, Triangle);
        RB->init(mMatrixUniform0);
        //RB->mMatrix.scale(0.5);
        mVisualObjects.push_back(RB);

        Obj = new ObjLoader();
        Obj->init(mMatrixUniform0);
        Obj->mMatrix.translate(200.f,200.f,200.f);
        Obj->mMatrix.scale(10.f);
        mVisualObjects.push_back(Obj);







//        temp = new Door(); //[3]
//        temp->init(mMatrixUniform0);
//        //temp->mMatrix.translate(-0.2f, -0.2f, 0.5f);
//        mVisualObjects.push_back(temp);

        //**********Set up camera************
            mCurrentCamera = new Camera();
             mCurrentCamera->setPosition(gsl::Vector3D( -99.4843 , 660 , -29.8457 ));  //map oversikt
             //mCurrentCamera->setPosition(gsl::Vector3D(0.7 , 20 , 34.1666));
            mCurrentCamera->yaw(-230.f);
            mCurrentCamera->pitch(36.f);
            //mCurrentCamera->yaw(-5.f);
            checkForGLerrors();

//            temp= new OctahedronBall(3);
//            temp->init(mMatrixUniform0);
//            mVisualObjects.push_back(temp);

//            mio = new InteractiveObject(); //[1]
//             mio->init(mMatrixUniform0);
//             mio->mMatrix.scale(1);
//             mVisualObjects.push_back(mio);
//             mMap.insert(std::pair<std::string, VisualObject*>{"mio", mio});

//       terrain = new Terrain();  //[2]
//       terrain->init(mMatrixUniform0);
//      // terrain->mMatrix.scale(0.5f);
//       mVisualObjects.push_back(terrain);

       //map= new Generate();

//       trof1 = new Trofee("pickup", 0.3, -0.1, -0.34);
//       trof1->init(mMatrixUniform0);
//       trof1->mMatrix.translate(-3.4f, .3f, -3.5f);
//       mVisualObjects.push_back(trof1);
//       mQuadTre->insert(trof1->getPosition2D(), trof1);

//       trof2 = new Trofee("pickup", 0.8, -0.1, -0.34);
//       trof2->init(mMatrixUniform0);
//       trof2->mMatrix.translate(-3.2f, .2f, -0.5f);
//       mVisualObjects.push_back(trof2);
//       mQuadTre->insert(trof2->getPosition2D(), trof2);

//       trof3 = new Trofee("enemy", 1, -0.1, 1.5);
//       trof3->init(mMatrixUniform0);
//       trof3->mMatrix.translate(-3.2f, .3f, 0.9f);
//       //trof3->mMatrix.scale(3);
//       mVisualObjects.push_back(trof3);
//       mQuadTre->insert(trof3->getPosition2D(), trof3);

       //        trof1 = new Trofee("pickup", 0.3, -0.1, -0.34);
       //        trof2 = new Trofee("pickup", 0.8, -0.1, -0.34);
       //        trof3 = new Trofee("pickup", 1, -0.1, 1.5);

//    for (auto it=mMap.begin(); it!=mMap.end(); it++)
//            (*it).second->init(mMmatrixUniform);
//    for (auto it=mQuadTre.begin(); it!=mQuadTre.end(); it++)
//            (*it)->init(mMmatrixUniform);

//    mQuadTre->init(mMmatrixUniform);
      mQuadTre->init(mMatrixUniform0);
      mVisualObjects.push_back(mQuadTre);

    glBindVertexArray(0);       //unbinds any VertexArray - good practice
    checkForGLerrors();
}


// Called each frame - doing the rendering!!!
void RenderWindow::render()
{
    // Flere matriser her!
//    mPmatrix->setToIdentity();
//    mVmatrix->setToIdentity();
    //mPmatrix->perspective(60, 4.0/3.0, 0.1, 10.0);
//    mCamera.init(mPmatrixUniform, mVmatrixUniform);
//    mCamera.perspective(60, 4.0/3.0, 0.1, 10.0);

    mCurrentCamera->update();

    mTimeStart.restart(); //restart FPS clock
    mContext->makeCurrent(this);

    initializeOpenGLFunctions();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glUseProgram(mShaderProgram->getProgram() );

    //Draws the objects
        //This should be in a loop!
        {
            glUseProgram(mShaderProgram[0]->getProgram() );


            for(int x=0; x<=3; x++)
            {

                //send data to shader
                glUniformMatrix4fv( vMatrixUniform0, 1, GL_TRUE, mCurrentCamera->mViewMatrix.constData());
                glUniformMatrix4fv( pMatrixUniform0, 1, GL_TRUE, mCurrentCamera->mProjectionMatrix.constData());
                glUniformMatrix4fv( mMatrixUniform0, 1, GL_TRUE, mVisualObjects[x]->mMatrix.constData());
                //draw the object
                mVisualObjects[x]->draw();

            }

            for(int i=0; i<Rain.size(); i++)
            {
                Rain[i]->draw();
                Rain[i]->move(1);
            }

        RB->move(1);


        //mQuadTre->draw();

        }


   calculateFramerate();

   checkForGLerrors();

   mContext->swapBuffers(this);

//   float y = terrain->getPositionOnTerrain(mMap["mio"]->mMatrix.getPosition().getX(), mMap["mio"]->mMatrix.getPosition().getZ());
//       mMap["mio"]->mMatrix.setPosition(mMap["mio"]->mMatrix.getPosition().getX(),y, mMap["mio"]->mMatrix.getPosition().getZ());
//       qDebug() << "   y  = " << mMap["mio"]->mMatrix.getPosition().getY();


}

void RenderWindow::setupPlainShader(int shaderIndex)
{
    mMatrixUniform0 = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "mMatrix" );
    vMatrixUniform0 = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "vMatrix" );
    pMatrixUniform0 = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "pMatrix" );
}

void RenderWindow::setupTextureShader(int shaderIndex)
{
    mMatrixUniform1 = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "mMatrix" );
    vMatrixUniform1 = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "vMatrix" );
    pMatrixUniform1 = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "pMatrix" );
    mTextureUniform = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "textureSampler");
}

void RenderWindow::setupPhongShader(int shaderIndex)
{
    mMatrixUniform2 = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "mMatrix" );
    vMatrixUniform2 = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "vMatrix" );
    pMatrixUniform2 = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "pMatrix" );

    mLightColorUniform = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "lightColor" );
    mObjectColorUniform = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "objectColor" );
    mAmbientLightStrengthUniform = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "ambientStrengt" );
    mLightPositionUniform = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "lightPosition" );
    mSpecularStrengthUniform = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "specularStrength" );
    mSpecularExponentUniform = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "specularExponent" );
    mLightPowerUniform = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "lightPower" );
    mCameraPositionUniform = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "cameraPosition" );
    mTextureUniform2 = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "textureSampler");
}


//This function is called from Qt when window is exposed (shown)
// and when it is resized
//exposeEvent is a overridden function from QWindow that we inherit from
void RenderWindow::exposeEvent(QExposeEvent *)
{
    //if not already initialized - run init() function - happens on program start up
    if (!mInitialized)
        init();

    //This is just to support modern screens with "double" pixels (Macs and some 4k Windows laptops)
    const qreal retinaScale = devicePixelRatio();

    //Set viewport width and height to the size of the QWindow we have set up for OpenGL
    glViewport(0, 0, static_cast<GLint>(width() * retinaScale), static_cast<GLint>(height() * retinaScale));

    //If the window actually is exposed to the screen we start the main loop
    //isExposed() is a function in QWindow
    if (isExposed())
    {
        //This timer runs the actual MainLoop == the render()-function
        //16 means 16ms = 60 Frames pr second (should be 16.6666666 to be exact...)
        mRenderTimer->start(16);
        mTimeStart.start();
    }
    //calculate aspect ration and set projection matrix
        mAspectratio = static_cast<float>(width()) / height();
        mCurrentCamera->mProjectionMatrix.perspective(45.f, mAspectratio, 0.1f, 1000.f);
}

//The way this function is set up is that we start the clock before doing the draw call,
// and check the time right after it is finished (done in the render function)
//This will approximate what framerate we COULD have.
//The actual frame rate on your monitor is limited by the vsync and is probably 60Hz
void RenderWindow::calculateFramerate()
{
    long nsecElapsed = mTimeStart.nsecsElapsed();
    static int frameCount{0};                       //counting actual frames for a quick "timer" for the statusbar

    if (mMainWindow)            //if no mainWindow, something is really wrong...
    {
        ++frameCount;
        if (frameCount > 30)    //once pr 30 frames = update the message == twice pr second (on a 60Hz monitor)
        {
            //showing some statistics in status bar
            mMainWindow->statusBar()->showMessage(" Time pr FrameDraw: " +
                                                  QString::number(nsecElapsed/1000000.f, 'g', 4) + " ms  |  " +
                                                  "FPS (approximated): " + QString::number(1E9 / nsecElapsed, 'g', 7));
            frameCount = 0;     //reset to show a new message in 30 frames
        }
    }
}

//Uses QOpenGLDebugLogger if this is present
//Reverts to glGetError() if not
void RenderWindow::checkForGLerrors()
{
    if(mOpenGLDebugLogger)  //if our machine got this class to work
    {
        const QList<QOpenGLDebugMessage> messages = mOpenGLDebugLogger->loggedMessages();
        for (const QOpenGLDebugMessage &message : messages)
        {
            if (!(message.type() == message.OtherType)) // get rid of uninteresting "object ...
                                                        // will use VIDEO memory as the source for
                                                        // buffer object operations"
                // valid error message:
                mLogger->logText(message.message().toStdString(), LogType::REALERROR);
        }
    }
    else
    {
        GLenum err = GL_NO_ERROR;
        while((err = glGetError()) != GL_NO_ERROR)
        {
            mLogger->logText("glGetError returns " + std::to_string(err), LogType::REALERROR);
            switch (err) {
            case 1280:
                mLogger->logText("GL_INVALID_ENUM - Given when an enumeration parameter is not a "
                                "legal enumeration for that function.");
                break;
            case 1281:
                mLogger->logText("GL_INVALID_VALUE - Given when a value parameter is not a legal "
                                "value for that function.");
                break;
            case 1282:
                mLogger->logText("GL_INVALID_OPERATION - Given when the set of state for a command "
                                "is not legal for the parameters given to that command. "
                                "It is also given for commands where combinations of parameters "
                                "define what the legal parameters are.");
                break;
            }
        }
    }
}

//Tries to start the extended OpenGL debugger that comes with Qt
//Usually works on Windows machines, but not on Mac...
void RenderWindow::startOpenGLDebugger()
{
    QOpenGLContext * temp = this->context();
    if (temp)
    {
        QSurfaceFormat format = temp->format();
        if (! format.testOption(QSurfaceFormat::DebugContext))
            mLogger->logText("This system can not use QOpenGLDebugLogger, so we revert to glGetError()",
                             LogType::HIGHLIGHT);

        if(temp->hasExtension(QByteArrayLiteral("GL_KHR_debug")))
        {
            mLogger->logText("This system can log extended OpenGL errors", LogType::HIGHLIGHT);
            mOpenGLDebugLogger = new QOpenGLDebugLogger(this);
            if (mOpenGLDebugLogger->initialize()) // initializes in the current context
                mLogger->logText("Started Qt OpenGL debug logger");
        }
    }
}

//Event sent from Qt when program receives a keyPress
// NB - see renderwindow.h for signatures on keyRelease and mouse input
void RenderWindow::keyPressEvent(QKeyEvent *event)
{
    int checked = 0;
    if (event->key() == Qt::Key_Escape)
    {
        mMainWindow->close();       //Shuts down the whole program
    }
    //You get the keyboard input like this
        if(event->key() == Qt::Key_A)
        {
           // mMap["mio"]->move(-0.1f, 0.0f, 0.0f);
            //mMap["mio"]->mMatrix.translateX(-0.1);
            mCurrentCamera->moveRight(-1);
        }
        if(event->key() == Qt::Key_S)
        {
           // mMap["mio"]->move(0.0f,0.0f,0.1f);
            //mMap["mio"]->mMatrix.translateZ(0.1);
            mCurrentCamera->moveForward(-1);

        }
        if(event->key() == Qt::Key_W)
        {
           // mMap["mio"]->move(0.0f,0.0f,-0.1f);
           // mMap["mio"]->mMatrix.translateZ(-0.1);
            mCurrentCamera->moveForward(1);
        }
        if(event->key() == Qt::Key_D)
        {
           // mMap["mio"]->move(0.1f,0.0f,0.0f);
           // mMap["mio"]->mMatrix.translateX(0.1);
            mCurrentCamera->moveRight(1);
        }
        if(event->key() == Qt::Key_E)
            {
               // mCamera.switchView();
              //  mMap["door"]->open();

            mCurrentCamera->updateHeigth(1.f);
            }
        if(event->key() == Qt::Key_Q)
            {
                //npc->switchGraph();

            mCurrentCamera->updateHeigth(-1.f);
            }
        if(event->key() == Qt::Key_X)
        {
//            if (checked==0){
//            mCurrentCamera->setPosition(gsl::Vector3D(0.7 , 20 , 34.1666));
//            mCurrentCamera->yaw(230.f);
//            mCurrentCamera->pitch(40.f);
//            checked ++;}
//            else {
//            mCurrentCamera->setPosition(gsl::Vector3D(35 , 580 , 83));
//            mCurrentCamera->yaw(-230.f);
//            mCurrentCamera->pitch(36.f);
//            checked--;
//            }
                    qDebug() << "P =" << (mCurrentCamera->position());

        }

        if(event->key() == Qt::Key_1)
        {

            mySpeaker.Play(sound1);
          

        }

        if(event->key() == Qt::Key_2)
        {

            mySpeaker.Play(sound2);

        }

    //You get the keyboard input like this
//    if(event->key() == Qt::Key_A)
//    {
//        mMainWindow->statusBar()->showMessage(" AAAA");
//    }
//    if(event->key() == Qt::Key_S)
//    {
//        mMainWindow->statusBar()->showMessage(" SSSS");
//    }


//        auto posisjon = mMap["mio"]->getPosition2D();
//        auto subtre = mQuadTre->find(posisjon);
//        for (auto it=subtre->m_sub_objects.begin();it!=subtre->m_sub_objects.end();it++)
//            if((*it)->m_name == "pickup" || (*it)->m_name == "enemy" )
//                mio->collision(*it);

}

void RenderWindow::mousePressEvent(QMouseEvent *event)
{


        if(event->button() == Qt::RightButton)
        {
            mCurrentCamera->yaw(1.f);
        }
        if(event->button() == Qt::LeftButton)
        {    //using left and right click for now to move yaw
            mCurrentCamera->yaw(-1.f);
        }

}

void RenderWindow::spawnrain(){
    for (int i=0; i<=10 ;i++ ) {


    RollingBall* temp = new RollingBall(3, Triangle);
            temp->init(mMatrixUniform0);
            temp->mMatrix.scale(5);
            Rain.push_back(temp);
    }
}

void RenderWindow::spawnrainDrop(float a, float b, float c)
{
    RollingBall* temp = new RollingBall(3, Triangle);
            temp->init(mMatrixUniform0);
            temp->mMatrix.scale(5);
            temp->mMatrix.setPosition(a, b, c);
            Rain.push_back(temp);
}

void RenderWindow::ToggleWireframe(bool checked)
{
    if(checked)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


    }
}

void RenderWindow::ResetCamera()
{
    mCurrentCamera->setPosition(gsl::Vector3D( -99.4843 , 660 , -29.8457 ));
}
