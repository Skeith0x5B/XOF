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

// Third-person camera testing
#include "../Rendering/XOF_ThirdPersonCamera.hpp"
ThirdPersonCamera tpCam;

// Game-object model / Scene-graphing
#include "../GameplayFoundations/XOF_PlayerObject.hpp"
GameObject platformObject;
PlayerObject playerObject;


Mesh characterMesh, platformMesh;
Transform characerTransform( glm::vec3( 0.f, 0.f, 0.f ), glm::vec3( 0.f, 0.f, 0.f ) );
Shader shader;
FirstPersonCamera camera;
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
	characterMesh.Load( "./Resources/Booker_DeWitt.obj" );
	playerObject.SetMesh( &characterMesh );

	GeoPrimitiveGenerator::GenerateCube( 50, 2, 50, platformMesh );
	platformObject.SetMesh( &platformMesh );
	platformObject.Move( 0.f, -1.5f, 0.f );

	shader.Load( "./Resources/BasicTexture" );
	shader.AddUniform( "transform" );
	shader.Bind();

	// Camera
	camera.Setup( glm::vec3( 0.f, 2.f, 5.f ), glm::vec3( 0.f, 0.f, -1.f ),
		(float)mWindow->getSize().x, (float)mWindow->getSize().y, 
		(float)mWindow->getSize().x / (float)mWindow->getSize().y, 0.01f, 1000.f );

	tpCam.Setup( glm::vec3( 0.f, 2.f, 5.f ), glm::vec3( 0.f, 0.f, -1.f ),
		(float)mWindow->getSize().x, (float)mWindow->getSize().y, 
		(float)mWindow->getSize().x / (float)mWindow->getSize().y, 0.01f, 1000.f, playerObject );
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
	// Movement

	// TODO: MOVE PLAYER OBJECT
	if( mKeyboard.IsKeyDown( XOF_KEYBOARD::A ) ) {
		playerObject.Move( -0.05f, 0.f, 0.f );
		tpCam.Translate( -0.05f, 0.f, 0.f );
	}

	if( mKeyboard.IsKeyDown( XOF_KEYBOARD::D ) ) {
		playerObject.Move( 0.05f, 0.f, 0.f );
		tpCam.Translate( 0.05f, 0.f, 0.f );
	}

	if( mKeyboard.IsKeyDown( XOF_KEYBOARD::W ) ) {
		playerObject.Move( 0.f, 0.f, -0.05f );
		tpCam.Translate( 0.f, 0.f, -0.05f );
	}

	if( mKeyboard.IsKeyDown( XOF_KEYBOARD::S ) ) {
		playerObject.Move( 0.f, 0.f, 0.05f );
		tpCam.Translate( 0.f, 0.f, 0.05f );
	}
	
	if( mKeyboard.IsKeyDown( XOF_KEYBOARD::Q ) ) {
		tpCam.ZoomIn( 0.5f );
	}

	if( mKeyboard.IsKeyDown( XOF_KEYBOARD::E ) ) {
		tpCam.ZoomIn( 0.5f );
	}

	// Looking
	if( mKeyboard.IsKeyDown( XOF_KEYBOARD::ARROW_LEFT ) ) {
		tpCam.Yaw( 0.05f );
	}

	if( mKeyboard.IsKeyDown( XOF_KEYBOARD::ARROW_RIGHT ) ) {
		tpCam.Yaw( -0.05f );
	}

	if( mKeyboard.IsKeyDown( XOF_KEYBOARD::ARROW_UP ) ) {
		tpCam.Pitch( 0.05f );
	}

	if( mKeyboard.IsKeyDown( XOF_KEYBOARD::ARROW_DOWN ) ) {
		tpCam.Pitch( -0.05f );
	}
}

void Game::Render() {
	screenWidth  = mWindow->getSize().x;
	screenHeight = mWindow->getSize().y;

	mRenderer.ClearScreen();

	// NOTE: Is the correct order not view->projection, not projection->view?
	shader.SetUniform( "transform", (tpCam.GetProjectionMatrix() * tpCam.GetViewMatrix()) * platformObject.GetTransform().GetModelMatrix() );
	mRenderer.Draw( platformObject );

	shader.SetUniform( "transform", (tpCam.GetProjectionMatrix() * tpCam.GetViewMatrix()) * playerObject.GetTransform().GetModelMatrix() );
	mRenderer.Draw( playerObject );

	mWindow->display();
}