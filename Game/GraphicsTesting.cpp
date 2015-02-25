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


// Game-object model / Scene-graphing
#include "../GameplayFoundations/XOF_PlayerObject.hpp"
GameObject platformObject;
PlayerObject barrelObject;

// Directional light
#include "../Rendering/XOF_Lights.hpp"
DirectionalLight directionalLight;


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

	mWindow = new (std::nothrow)sf::Window( sf::VideoMode( width, height ), "XOF - Graphics Testing", 
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
	characterMesh.Load( "./Resources/barrel.obj" );
	barrelObject.SetMesh( &characterMesh, false );

	GeoPrimitiveGenerator::GenerateCube( 50, 2, 50, platformMesh );
	platformObject.SetMesh( &platformMesh, true );
	platformObject.Translate( 0.f, -1.f, 0.25f );

	shader.Load( "./Resources/BasicTexture_AmbientDiffuseSpecularLight_NormalMapping" );
	shader.AddUniform( "transform" );
	shader.AddUniform( "world" );
	std::cout << glGetError() << std::endl;
	shader.AddUniform( "directionalLight.direction" );
	shader.AddUniform( "directionalLight.color" );
	shader.AddUniform( "directionalLight.ambientIntensity" );
	shader.AddUniform( "directionalLight.diffuseIntensity" );
	shader.AddUniform( "eyeWorldPos" );
	shader.AddUniform( "matSpecularIntensity" );
	shader.AddUniform( "specularPower" );
	shader.Bind();

	// Camera
	camera.Setup( glm::vec3( 0.f, 2.f, 5.f ), glm::vec3( 0.f, 0.f, -1.f ),
		(float)mWindow->getSize().x, (float)mWindow->getSize().y, 
		(float)mWindow->getSize().x / (float)mWindow->getSize().y, 0.01f, 1000.f );

	// Light
	directionalLight.color = glm::vec3( 1.f, 1.f, 1.f );
	directionalLight.ambientIntensity = 0.1f;
	directionalLight.diffuseIntensity = 0.5f;
	directionalLight.direction = glm::vec3( 0.f, 0.5f, 1.f );
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
		camera.Translate( 0.f, 0.25f, 0.f );
	}

	if( mKeyboard.IsKeyDown( XOF_KEYBOARD::E ) ) {
		camera.Translate( 0.f, -0.25f, 0.f );
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

	counter += 0.00001f;
	//barrelObject.Rotate( 0.f, sinf( counter / 1000.f ), 0.f );

	shader.SetUniform( "directionalLight.direction", directionalLight.direction );
	shader.SetUniform( "directionalLight.color", directionalLight.color );
	shader.SetUniform( "directionalLight.ambientIntensity", directionalLight.ambientIntensity );
	shader.SetUniform( "directionalLight.diffuseIntensity", directionalLight.diffuseIntensity );
	shader.SetUniform( "eyeWorldPos", const_cast<glm::vec3&>( camera.GetPosition() ) );
	shader.SetUniform( "matSpecularIntensity", 0.1f );
	shader.SetUniform( "specularPower", 1.f );

	// NOTE: Is the correct order not view->projection, not projection->view?
	shader.SetUniform( "transform", ( camera.GetProjectionMatrix() * camera.GetViewMatrix() ) * platformObject.GetTransform().GetModelMatrix() );
	shader.SetUniform( "world", platformObject.GetTransform().GetModelMatrix() );
	mRenderer.Draw( platformObject );

	shader.SetUniform( "transform", ( camera.GetProjectionMatrix() * camera.GetViewMatrix() ) * barrelObject.GetTransform().GetModelMatrix() );
	shader.SetUniform( "world", barrelObject.GetTransform().GetModelMatrix() );
	mRenderer.Draw( barrelObject );

	mWindow->display();
}