#include "../Game/Game.hpp"

#include <iostream>
#include <memory>
#include <SFML/Graphics.hpp>

#include "../Platform/XOF_Platform.hpp"
#include "../Platform/XOF_PlatformInfo.hpp"
#include "../Platform/XOF_Timer.hpp"
#include "../Rendering/XOF_Mesh.hpp"
#include "../Rendering/XOF_Shader.hpp"
#include "../Rendering/XOF_Transform.hpp"
#include "../Rendering/XOF_Texture.hpp"
#include "../Rendering/XOF_StaticCamera.hpp"
#include "../Rendering/XOF_FirstPersonCamera.hpp"
#include "../Rendering/XOF_Lights.hpp"
#include "../Rendering/XOF_GeoPrimitiveGenerator.hpp"


// Editor testing
#include "../GameplayFoundations/XOF_GameObject.hpp"
#include "../Editor/XOF-ED_TransformGizmo.hpp"
TransformGizmo	transformGizmo;
GameObject		boxObject;

glm::vec3 brushHighlight( 1.f, 1.f, 1.f );
bool isChunkSelected = false;

bool TestCollision( const glm::vec2& v );


std::unique_ptr<Mesh> testMeshPtr( new Mesh() );
Shader gizmoShader, chunkShader;
Transform testTransform( glm::vec3( 0.f, 0.f, 0.f ), glm::vec3( 0.f, 0.f, 0.f ) );
FirstPersonCamera camera;
DirectionalLight testDirectionalLight;

Mesh gizmoMesh;
U32 screenWidth = 800, screenHeight = 600;


float counter = 0.f;


Game::Game() 
: mWindow( nullptr )
, mIsInitialised( false ) {
	// ...
}

Game::~Game() {
	if( mWindow ) {
		delete mWindow;
		mWindow = nullptr;
	}
}

// NOTE: Move context set-up to graphics device? Want all open-gl/directX stuff to stay in the rendering folder, not spread out
bool Game::Initialise( U16 width, U16 height, CHAR* caption ) {
	// Inspect OpenGL context/settings
	sf::ContextSettings context;
	context.majorVersion		= 4;
	context.minorVersion		= 4;
	context.antialiasingLevel	= 4;
	context.depthBits			= 24;
	context.stencilBits			= 8;

	mWindow = new (std::nothrow)sf::Window( sf::VideoMode( width, height ), "XOF Editor (TESTING)", 
												sf::Style::Default, context );

	if( !mWindow ) {
		return false;
	} else {
		GetPlatformInfo();
		std::cout << "\n";

		context = mWindow->getSettings();
		std::cout << "OpenGL version: " << context.majorVersion << "." << context.minorVersion << std::endl;
		std::cout << "Anti-Aliasing: " << context.antialiasingLevel << std::endl;
		std::cout << "Depth: " << context.depthBits << std::endl;
		std::cout << "Stencil: " << context.stencilBits << "\n" << std::endl;

		mMouse.SetWindow( mWindow );
	}

	mRenderer.Initialise();

	SetupScene();

	mIsInitialised = true;
	return mIsInitialised;
}

// frame-limit
static double FRAME_TIME = 1.f / Game::mFrameCap;
Timer gTimer;

void Game::Run() {
	double unprocessedTime = 0.f;
	double elapsedTime = 0.f;
	U32 frameCount = 0;
	gTimer.Reset();
	double lastTime = gTimer.GetMilliseconds();

	if( mIsInitialised ) {
		while( mWindow->isOpen() ) {
			double currentTime = gTimer.GetMilliseconds();
			double deltaTime = currentTime - lastTime;
			lastTime = currentTime;

			unprocessedTime += deltaTime;

			// TEMP --------------------------------------------------------
			if( unprocessedTime >= FRAME_TIME ) {
				// Get user input?
				HandleEvents();
				Render();

				++frameCount;
				unprocessedTime -= FRAME_TIME;

				elapsedTime += deltaTime;
				if( elapsedTime >= 1000 ) {
					std::cout << "FPS: " << frameCount << std::endl;
					frameCount = 0;
					elapsedTime = 0.f;
				}
			}
			// TEMP --------------------------------------------------------
		} // while( mWindow->isOpen() ) 
	} // if( mIsInitialised )
}

void Game::SetupScene() {
	boxObject.SetMesh( GeoPrimitiveGenerator::GenerateCube( 5, 5, 5 ), true );

	// Transform gizmo(s)
	transformGizmo.Setup( "./Resources/TranslateGizmo.obj" );
	//gizmoMesh.Load( "./Resources/TranslateGizmo.obj" );

	gizmoShader.Load( "./Resources/EditorGizmo" );
	gizmoShader.AddUniform( "transform" );
	gizmoShader.Bind();



	chunkShader.Load( "./Resources/BasicTexture_EDITOR" );
	chunkShader.AddUniform( "transform" );
	chunkShader.AddUniform( "highlightColor" );

	// Camera
	camera.Setup( glm::vec3( 0.f, 5.f, 10.f ), glm::vec3( 0.f, 0.f, -1.f ),
		(float)mWindow->getSize().x, (float)mWindow->getSize().y, 
		(float)mWindow->getSize().x / (float)mWindow->getSize().y, 0.01f, 1000.f );
}

void Game::HandleEvents() {
	sf::Event event;
	while( mWindow->pollEvent( event ) ) {
		if( event.type == sf::Event::Closed ) {
			TerminateProcess( GetCurrentProcess(), EXIT_SUCCESS ); // Temp fix for crash when closing on Laptop
			mWindow->close();
		}

		if( event.type == sf::Event::Resized ) {
			mRenderer.Resize( event.size.width, event.size.height );
			camera.Resize( static_cast<float>( event.size.width ), static_cast<float>( event.size.height ) );
		}

		HandleInput();

		if( mKeyboard.IsKeyDown( XOF_KEYBOARD::ESCAPE ) ) {
			TerminateProcess( GetCurrentProcess(), EXIT_SUCCESS ); // Temp fix for crash when closing on Laptop
			mWindow->close();
		}
	}
}

void Game::HandleInput() {
	// Interaction with mouse

	// Has world geometry from brush been hit?
	if( mMouse.IsButtonDown( XOF_MOUSE::LEFT ) ) {
		if( TestCollision(mMouse.GetMousePosition()) ) {
			brushHighlight = glm::vec3( 1.f, 0.55f, 0.f );
			isChunkSelected = true;
		} else {
			brushHighlight = glm::vec3( 1.f, 1.f, 1.f );
			isChunkSelected = false;
		}
	}

	// Has transform gizmo been hit?
	// ...

	// Movement
	if( mKeyboard.IsKeyDown( XOF_KEYBOARD::A ) ) {
		camera.Translate( -0.05f, 0.f, 0.f );
	}

	if( mKeyboard.IsKeyDown( XOF_KEYBOARD::D ) ) {
		camera.Translate( 0.05f, 0.f, 0.f );
	}

	if( mKeyboard.IsKeyDown( XOF_KEYBOARD::W ) ) {
		camera.Translate( 0.f, 0.f, -0.05f );
	}

	if( mKeyboard.IsKeyDown( XOF_KEYBOARD::S ) ) {
		camera.Translate( 0.f, 0.f, 0.05f );
	}
	
	if( mKeyboard.IsKeyDown( XOF_KEYBOARD::Q ) ) {
		camera.Translate( 0.f, 0.05f, 0.f );
	}

	if( mKeyboard.IsKeyDown( XOF_KEYBOARD::E ) ) {
		camera.Translate( 0.f, -0.05f, 0.f );
	}

	// Looking
	if( mKeyboard.IsKeyDown( XOF_KEYBOARD::ARROW_LEFT ) ) {
		camera.Yaw( 0.05f );
	}

	if( mKeyboard.IsKeyDown( XOF_KEYBOARD::ARROW_RIGHT ) ) {
		camera.Yaw( -0.05f );
	}

	if( mKeyboard.IsKeyDown( XOF_KEYBOARD::ARROW_UP ) ) {
		camera.Pitch( 0.05f );
	}

	if( mKeyboard.IsKeyDown( XOF_KEYBOARD::ARROW_DOWN ) ) {
		camera.Pitch( -0.05f );
	}
}

void Game::Render() {
	screenWidth  = mWindow->getSize().x;
	screenHeight = mWindow->getSize().y;

	mRenderer.ClearScreen();

	// Modify the transform value for each mesh before drawing it
	chunkShader.Bind();
	chunkShader.SetUniform( "highlightColor", brushHighlight );
	// NOTE: Is the correct order not view->projection, not projection->view?
	chunkShader.SetUniform( "transform", (camera.GetProjectionMatrix() * camera.GetViewMatrix()) * boxObject.GetTransform().GetModelMatrix() );
	mRenderer.Draw( boxObject );

	if( isChunkSelected ) {
		gizmoShader.Bind();
		// USE GIZMO'S TRANSFORM? (1/distance^2)?
		gizmoShader.SetUniform( "transform", (camera.GetProjectionMatrix() * camera.GetViewMatrix()) * transformGizmo.GetTransform().GetModelMatrix() );	
		mRenderer.Draw( transformGizmo );
	}

	// Display will swap the OpenGL buffers for us
	mWindow->display();
}


bool TestCollision( const glm::vec2& v ) {
	Ray r;

	// Calculate a ray from the mouse position (http://antongerdelan.net/opengl/raycasting.html)
	// 0) Start with 2d, viewport-space mouse coordinates

	// 1) Transform into 3d NDC, NOTE: Add window size
	float x = ( 2.f * v.x ) / screenWidth - 1.f;
	float y = 1.f - ( 2.f * v.y ) / screenHeight;
	float z = 1.f;
	glm::vec3 rayNDS( x, y, z );

	// 2) 4d Homogeneous Clip Coordinates
	glm::vec4 rayClip( rayNDS.x, rayNDS.y, -1.f, 1.f ); // NOTE: Reverse perspective divide?

	// 3) 4d Eye (Camera) Coordinates
	glm::vec4 rayEye = glm::inverse( camera.GetProjectionMatrix() ) * rayClip;
	rayEye.z = -1.f;
	rayEye.w = 0.f;

	// 4) 4d World Coordinates
	glm::vec4 temp = glm::inverse( camera.GetViewMatrix() ) * rayEye;
	glm::vec3 rayWorld( temp.x, temp.y, temp.z );
	rayWorld = glm::normalize( rayWorld );

	r.origin = camera.GetPosition();
	r.direction = rayWorld;
	float cameraZ = camera.GetPosition().z;
	r.start = cameraZ < 0 ? -cameraZ : cameraZ;
	r.end = r.start + 10.f;

	return boxObject.GetCollisionVolume().IsHit( r );
}