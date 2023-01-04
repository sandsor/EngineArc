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
#include "texture.h"

#include "Entity.h"


RenderWindow::RenderWindow(const QSurfaceFormat& format, MainWindow* mainWindow)
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

	//// ***************** QuadTree *********************
		  //Point2D a{-8, -8}, b{8, -8}, c{8, 8}, d{-8, 8}; // må gjøres ordentlig
		  //mQuadTre = new QuadTree(a,b,c,d);
		  //mQuadTre->subDivide(5);
	//// **************************************

  //Make the gameloop timer:
	mRenderTimer = new QTimer(this);


}

RenderWindow::~RenderWindow()
{

}

Entity RenderWindow::CreateEntity(const std::string& name)
{
	Entity entity = { mRegistry.create(),this };
	entity.AddComponent<TransformComponent>();
	auto& tag = entity.AddComponent<TagComponent>();
	tag.Tag = name.empty() ? "Entity" : name;
	return entity;
}


// Sets up the general OpenGL stuff and the buffers needed to render a triangle
void RenderWindow::init() {

	mLogger = Logger::getInstance();
	//Entt system Petter start********************************************

	auto square = CreateEntity("Square");

	//  mRegistry.emplace<TransformComponent>(entt::entity);
	//  if (mRegistry.all_of<TransformComponent>(entt::entity))
		  //TransformComponent& transform=mRegistry.get<TransformComponent>(entity);
	  /*auto view = mRegistry.view<TransformComponent>();
	  for (auto entity : view)
	  {
		  TransformComponent& transform = view.get<TransformComponent>(entity);


	  }
	  auto group = mRegistry.group<TransformComponent>(entt::get<MeshComponent>);
	  for (auto entity : group)
	  {
		  auto[Transform, mesh]= group.get<TransformComponent, MeshComponent>(entity);


	  }*/

    //################## Bullet start ######################

    //collisionConfig = new btDefaultCollisionConfiguration();
    //dispathcher = new btCollisionDispatcher(collisionConfig);
    //broadphase = new btDbvtBroadphase();
    //solver = new btSequentialImpulseConstraintSolver();
    //world = new btDiscreteDynamicsWorld(dispathcher, broadphase, solver, collisionConfig);
    //world->setGravity(btVector3(0, -10, 0));

    //################## Bullet end ########################



	  //Entt system Petter end********************************************
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
	mLogger->logText("Plain shader program id: " + std::to_string(mShaderProgram[0]->getProgram()));
	mShaderProgram[1] = new Shader("../EngineArc/textureshader.vert", "../EngineArc/textureshader.frag");
	mLogger->logText("Texture shader program id: " + std::to_string(mShaderProgram[1]->getProgram()));
	mShaderProgram[2] = new Shader("../EngineArc/phongshader.vert", "../EngineArc/phongshader.frag");
	mLogger->logText("Texture shader program id: " + std::to_string(mShaderProgram[2]->getProgram()));


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

	Perlin = new PerlinGenerator();
    Perlin->init(mMatrixUniform0);
    Perlin->mMatrix.scale(20);
    mVisualObjects.push_back(Perlin);

	Vsflate = new VSflate();
	Vsflate->init(mMatrixUniform0);
	//Vsflate->mMatrix.scale(0.5);
	mVisualObjects.push_back(Vsflate);

	Triangle = new TriangleSurface("../EngineArc/CompressedFile.txt");
	Triangle->init(mMatrixUniform0);
	mVisualObjects.push_back(Triangle);

	Obj = new ObjLoader();
	Obj->init(mMatrixUniform0);
	Obj->mMatrix.translate(200.f, 200.f, 200.f);
	Obj->mMatrix.scale(10.f);
	mVisualObjects.push_back(Obj);
	TestParticle.ColorBegin = glm::vec3{ 1,0,0 };
	TestParticle.ColorEnd = glm::vec3{ 0,0,1 };
	TestParticle.LifeTime = 2.f;
	TestParticle.Position = glm::uvec3{ 0,0,0 };
	TestParticle.SizeBegin = 50;
	TestParticle.SizeEnd = 1;
	TestParticle.Velocity = glm::vec3{ 0.5 ,0,0};
	TestParticle.SizeVariation = 0;
	TestParticle.VelocityVariation = glm::vec3{ 0 };

	    //mParticleComponent = new ParticleComponent();
		// 
	//mParticleComponent->Emit(TestParticle);
	//mVisualObjects.push_back(mParticleComponent);
	//mCube = new Cube();
	////mCube->ChangeColor(1, 0, 0);
	//mCube->move(0, 10, 0);
	//mCube->Scale(50);
	//
	//
	//mCube->init(mMatrixUniform0);
	
	//mVisualObjects.push_back(mCube);

//**********Set up camera************
	mCurrentCamera = new Camera();
	mCurrentCamera->setPosition(gsl::Vector3D(0,0,0));  //map oversikt
	//mCurrentCamera->setPosition(gsl::Vector3D(0.7 , 20 , 34.1666)); // gammel lokasjon
	mCurrentCamera->yaw(-230.f);
	mCurrentCamera->pitch(36.f);

	//********************** Making Audio Objects **********************

	BackgroundMusic = new AudioObject{"../EngineArc/Assets/the-rhythm-of-the-africa.wav"};
	SoundsToPlay.push_back(BackgroundMusic);
	BackgroundMusic->SetVolume(0.7);
	BackgroundMusic->Play();
	
	Monke = new AudioObject{"../EngineArc/Assets/mono_spell.ogg" };
	SoundsToPlay.push_back(Monke);
	Monke->SetAttenuation(5.f, 20.f);
	Monke->LoopSound(true);
	Monke->Play();



	checkForGLerrors();

	//mQuadTre->init(mMatrixUniform0);
	//mVisualObjects.push_back(mQuadTre);

	glBindVertexArray(0);       //unbinds any VertexArray - good practice
	checkForGLerrors();
}


// Called each frame - doing the rendering!!!
void RenderWindow::render()
{

    /*world->stepSimulation(1/60);*/
    //updates camera
    mCurrentCamera->update();
    mTimeStart.restart(); //restart FPS clock
    mContext->makeCurrent(this);
    initializeOpenGLFunctions();
    timer1 = clock();
    float deltaTime = (float)(timer1 - timer2) / 1000.f;
    handleInput();


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



         //******************************* Draws the objects ***********************************
        {
            glUseProgram(mShaderProgram[0]->getProgram() );

            //qDebug() << mVisualObjects.size();
            for(int x=0; x < mVisualObjects.size(); x++)
            {

                //send data to shader
                glUniformMatrix4fv( vMatrixUniform0, 1, GL_TRUE, mCurrentCamera->mViewMatrix.constData());
                glUniformMatrix4fv( pMatrixUniform0, 1, GL_TRUE, mCurrentCamera->mProjectionMatrix.constData());
                glUniformMatrix4fv( mMatrixUniform0, 1, GL_TRUE, mVisualObjects[x]->mMatrix.constData());
                //draw the object
                mVisualObjects[x]->draw();
            }
        }
        if (Perlin->RandomPoints == true) {
            Perlin->init(mMatrixUniform0);
            Perlin->RandomMove();
        }
        if (Perlin->PerlinActive == true) {
            Perlin->init(mMatrixUniform0);
            Perlin->PerlinMove(deltaTime);
        }

	mysounddevice->SetLocation(mCurrentCamera->position().getX(), mCurrentCamera->position().getY(), mCurrentCamera->position().getZ());
	for (AudioObject* s : SoundsToPlay)
		s->UpdateBufferStream();



	calculateFramerate();

	checkForGLerrors();

	mContext->swapBuffers(this);
	timer2 = timer1;
}

void RenderWindow::setupPlainShader(int shaderIndex)
{
	mMatrixUniform0 = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "mMatrix");
	vMatrixUniform0 = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "vMatrix");
	pMatrixUniform0 = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "pMatrix");
}

void RenderWindow::setupTextureShader(int shaderIndex)
{
	mMatrixUniform1 = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "mMatrix");
	vMatrixUniform1 = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "vMatrix");
	pMatrixUniform1 = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "pMatrix");
	mTextureUniform = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "textureSampler");
}

void RenderWindow::setupPhongShader(int shaderIndex)
{
	mMatrixUniform2 = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "mMatrix");
	vMatrixUniform2 = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "vMatrix");
	pMatrixUniform2 = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "pMatrix");

	mLightColorUniform = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "lightColor");
	mObjectColorUniform = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "objectColor");
	mAmbientLightStrengthUniform = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "ambientStrengt");
	mLightPositionUniform = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "lightPosition");
	mSpecularStrengthUniform = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "specularStrength");
	mSpecularExponentUniform = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "specularExponent");
	mLightPowerUniform = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "lightPower");
	mCameraPositionUniform = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "cameraPosition");
	mTextureUniform2 = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "textureSampler");
}


//This function is called from Qt when window is exposed (shown)
// and when it is resized
//exposeEvent is a overridden function from QWindow that we inherit from
void RenderWindow::exposeEvent(QExposeEvent*)
{
	//if not already initialized - run init() function - happens on program start up
	if (!mInitialized)
	{
		init();
	}


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
	static int frameCount{ 0 };                       //counting actual frames for a quick "timer" for the statusbar

	if (mMainWindow)            //if no mainWindow, something is really wrong...
	{
		++frameCount;
		if (frameCount > 30)    //once pr 30 frames = update the message == twice pr second (on a 60Hz monitor)
		{
			//showing some statistics in status bar
			mMainWindow->statusBar()->showMessage(" Time pr FrameDraw: " +
				QString::number(nsecElapsed / 1000000.f, 'g', 4) + " ms  |  " +
				"FPS (approximated): " + QString::number(1E9 / nsecElapsed, 'g', 7));
			frameCount = 0;     //reset to show a new message in 30 frames
		}
	}
}

//Uses QOpenGLDebugLogger if this is present
//Reverts to glGetError() if not
void RenderWindow::checkForGLerrors()
{
	if (mOpenGLDebugLogger)  //if our machine got this class to work
	{
		const QList<QOpenGLDebugMessage> messages = mOpenGLDebugLogger->loggedMessages();
		for (const QOpenGLDebugMessage& message : messages)
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
		while ((err = glGetError()) != GL_NO_ERROR)
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
	QOpenGLContext* temp = this->context();
	if (temp)
	{
		QSurfaceFormat format = temp->format();
		if (!format.testOption(QSurfaceFormat::DebugContext))
			mLogger->logText("This system can not use QOpenGLDebugLogger, so we revert to glGetError()",
				LogType::HIGHLIGHT);

		if (temp->hasExtension(QByteArrayLiteral("GL_KHR_debug")))
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
void RenderWindow::keyPressEvent(QKeyEvent* event)
{
	int checked = 0;
	if (event->key() == Qt::Key_Escape)
	{
		mMainWindow->close();       //Shuts down the whole program
	}
	if (event->key() == Qt::Key_A)
		mInput.A = true;
	if (event->key() == Qt::Key_S)
		mInput.S = true;
	if (event->key() == Qt::Key_W)
		mInput.W = true;
	if (event->key() == Qt::Key_D)
		mInput.D = true;
	if (event->key() == Qt::Key_E)
		mInput.E = true;
	if (event->key() == Qt::Key_Q)
		mInput.Q = true;


        if (event->key() == Qt::Key_Shift)
            mInput.LShift = true;
        if (event->key() == Qt::Key_Control)
            mInput.LCtrl = true;
        if(event->key() == Qt::Key_X)
        {
            //qDebug() << "P =" << (mCurrentCamera->position());
            if (Perlin->RandomPoints == false) 
            {
                qDebug() << "true";
                Perlin->RandomPoints = true;
            }
            else  
            {
                qDebug()<<"false";
                Perlin->RandomPoints = false;
            }
        }
        if (event->key() == Qt::Key_Space)
        {
            //qDebug() << "P =" << (mCurrentCamera->position());
            if (Perlin->PerlinActive == false)
            {
                qDebug() << "true";
                Perlin->PerlinActive = true;
            }
            else
            {
                qDebug() << "false";
                Perlin->PerlinActive = false;
            }
        }
        if(event->key() == Qt::Key_1)
        {
            //mySpeaker.Play(sound1);
			BackgroundMusic->Pause();
        }
        if(event->key() == Qt::Key_2)
        {
            //mySpeaker.Play(sound2);
			BackgroundMusic->Play();
        }
		if (event->key() == Qt::Key_3)
		{
			//BackgroundMusic->Stop();
		}
        if (event->key() == Qt::Key_Right)
        {
            Perlin->maxheight++;
            Perlin->init(mMatrixUniform0);
        }
        if (event->key() == Qt::Key_Left)
        {
            Perlin->maxheight--;
            Perlin->init(mMatrixUniform0);
        }

        if (event->key() == Qt::Key_Up)
        {
            Perlin->perlinrange += 0.1;
        }
        if (event->key() == Qt::Key_Down)
        {
            if (Perlin->perlinrange > 0.1) {
                Perlin->perlinrange -= 0.1;
            }
        }

}

void RenderWindow::keyReleaseEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_W)
		mInput.W = false;
	if (event->key() == Qt::Key_A)
		mInput.A = false;
	if (event->key() == Qt::Key_S)
		mInput.S = false;
	if (event->key() == Qt::Key_D)
		mInput.D = false;

	if (event->key() == Qt::Key_Shift)
		mInput.LShift = false;
	if (event->key() == Qt::Key_Control)
		mInput.LCtrl = false;

	if (event->key() == Qt::Key_E)
		mInput.E = false;
	if (event->key() == Qt::Key_Q)
		mInput.Q = false;
}

void RenderWindow::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::RightButton)
	{
		mInput.RMB = true;
	}
}
void RenderWindow::mouseMoveEvent(QMouseEvent* event)
{
	if (mInput.RMB)
	{
		mouseX = event->pos().x() - mouseX;
		mouseY = event->pos().y() - mouseY;
		//        qDebug() << mouseX;
		//        qDebug() << mouseY;
		mCurrentCamera->yaw(mouseX * 0.1);
		mCurrentCamera->pitch(mouseY * 0.1);
	}
	mouseX = event->pos().x();
	mouseY = event->pos().y();
}
void RenderWindow::handleInput()
{
	float speed = 10;
	if (mInput.LShift)
		speed = 30;
	else if (mInput.LCtrl)
		speed = 0.5;

	if (mInput.W)
		mCurrentCamera->moveForward(speed);
	if (mInput.A)
		mCurrentCamera->moveRight(-speed);
	if (mInput.S)
		mCurrentCamera->moveForward(-speed);
	if (mInput.D)
		mCurrentCamera->moveRight(speed);
	if (mInput.E)
		mCurrentCamera->updateHeigth(speed);
	if (mInput.Q)
		mCurrentCamera->updateHeigth(-speed);

}
void RenderWindow::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::RightButton)
	{
		mInput.RMB = false;
	}
}

void RenderWindow::spawnrain()
{

}

void RenderWindow::spawnrainDrop(float a, float b, float c)
{

}

void RenderWindow::ToggleWireframe(bool checked)
{
	if (checked)
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
	mCurrentCamera->setPosition(gsl::Vector3D(-99.4843, 660, -29.8457));
}

void RenderWindow::EntInit(GLint matrixUniform, std::vector<Vertex>Vertices)
{

	initializeOpenGLFunctions();

	//Vertex Array Object - VAO
	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);

	//Vertex Buffer Object to hold vertices - VBO
	glGenBuffers(1, &mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);

	glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), Vertices.data(), GL_STATIC_DRAW);

	// 1rst attribute buffer : vertices
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// 2nd attribute buffer : colors
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// 3rd attribute buffer : uvs
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	//enable the matrixUniform
	// mMatrixUniform = glGetUniformLocation( matrixUniform, "matrix" );

	glBindVertexArray(0);

}

void RenderWindow::EntDraw(GLuint VAO, std::vector<Vertex> Indices)
{
	glBindVertexArray(VAO);
	//glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
	glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, nullptr);

}
