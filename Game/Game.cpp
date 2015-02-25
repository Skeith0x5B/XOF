#include "Game.hpp"

#include <iostream>
#include <memory>
#include <SFML/Graphics.hpp>

#include "../Platform/XOF_Platform.hpp"
#include "../Platform/XOF_Timer.hpp"
#include "../Rendering/XOF_Mesh.hpp"
#include "../Rendering/XOF_Shader.hpp"
#include "../Rendering/XOF_Transform.hpp"
#include "../Rendering/XOF_Texture.hpp"
#include "../Rendering/XOF_StaticCamera.hpp"
#include "../Rendering/XOF_FirstPersonCamera.hpp"
#include "../Rendering/XOF_Lights.hpp"
#include "../Rendering/XOF_GeoPrimitiveGenerator.hpp"


// editor testing
#include "../Editor/XOF-ED_CubeBrush.hpp"
CubeBrush brush; 


Mesh *testMesh, testMesh2;
Shader *testShader = nullptr;
Transform testTransform( glm::vec3( 0.f, 0.f, 0.f ), glm::vec3( 0.f, 0.f, 0.f ) );
float counter = 0.f;
Texture *testTexture = nullptr;
StaticCamera *testStaticCamera = nullptr;
FirstPersonCamera testFirstPersonCamera;
DirectionalLight testDirectionalLight;

std::unique_ptr<Mesh> meshPtr;


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

	if( testMesh ) {
		delete testMesh;
		testMesh = nullptr;
	}

	if( testShader ) {
		delete testShader;
		testShader = nullptr;
	}

	if( testTexture ) {
		delete testTexture;
		testTexture = nullptr;
	}

	if( testStaticCamera ) {
		delete testStaticCamera;
		testStaticCamera = nullptr;
	}
}

// NOTE: Move context set-up to graphics device? want all open-gl/directX stuff to stay in the rendering
// folder, not spread out
bool Game::Initialise( U16 width, U16 height, CHAR* caption ) {
	// Inspect OpenGL context/settings
	sf::ContextSettings context;
	context.majorVersion		= 4;
	context.minorVersion		= 4;
	context.antialiasingLevel	= 4;
	context.depthBits			= 24;
	context.stencilBits			= 8;

	mWindow = new (std::nothrow)sf::Window( sf::VideoMode( width, height ), caption, 
												sf::Style::Default, context );

	if( !mWindow ) {
		return false;
	} else {
		context = mWindow->getSettings();
		std::cout << "OpenGL version: " << context.majorVersion << "." << context.minorVersion << std::endl;
		std::cout << "Anti-Aliasing: " << context.antialiasingLevel << std::endl;
		std::cout << "Depth: " << context.depthBits << ", Stencil: " << context.stencilBits << "\n" << std::endl;

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
	Vertex vertices[] = { Vertex( glm::vec3( -0.5f, -0.5f, 0.f ), glm::vec2( 1.f,  1.f ) ), 
						  Vertex( glm::vec3(   0.f,  0.5f, 0.f ), glm::vec2( 0.5f, 0.f ) ), 
						  Vertex( glm::vec3(  0.5f, -0.5f, 0.f ), glm::vec2( 0.f,  1.f ) ) };

	U32 indices[] = { 0, 1, 2 };

	//testMesh2.Load( "./Resources/Booker_DeWitt.obj" );

	// TEMP PLATFORM MADE USING GEOPRIMGEN
	meshPtr = GeoPrimitiveGenerator::GenerateCube( 10, 10, 10 );

	testShader = new Shader( "./Resources/BasicTextureAmbientDiffuseSpecularLight" );
	//testShader->AddAttribute( "position", 0 );
	//testShader->AddAttribute( "normal", 1 );
	//testShader->AddAttribute( "texCoord", 2 );
	testShader->AddUniform( "transform" );
	testShader->AddUniform( "world" );
	//testShader->AddUniform( "testUni" );
	testShader->AddUniform( "directionalLight.direction" );
	testShader->AddUniform( "directionalLight.color" );
	testShader->AddUniform( "directionalLight.ambientIntensity" );
	testShader->AddUniform( "directionalLight.diffuseIntensity" );
	testShader->AddUniform( "eyeWorldPos" );
	testShader->AddUniform( "matSpecularIntensity" );
	testShader->AddUniform( "specularPower" );
	testShader->Bind();

	//testTexture = new Texture( "./Resources/Booker_body_D.jpg" );
	//testTexture->Bind( 0 );
	testStaticCamera = new StaticCamera( glm::vec3( 0.f, 2.f, 4.5f ), glm::vec3( 0.f, 0.f, -1.f ), 70.f, 
		(float)mWindow->getSize().x / (float)mWindow->getSize().y, 0.01f, 1000.f );

	testFirstPersonCamera.Setup( glm::vec3( 3.f, 2.f, 5.f ), glm::vec3( 0.f, 0.f, -1.f ), 70.f, 
		(float)mWindow->getSize().x / (float)mWindow->getSize().y, 0.01f, 1000.f );

	testDirectionalLight.direction = glm::vec3( 0.f, 0.f, -1.f );
	testDirectionalLight.color = glm::vec3( 1.f, 1.f, 1.f );
	testDirectionalLight.ambientIntensity = 0.5f;
	testDirectionalLight.diffuseIntensity = 1.f;
}

void Game::HandleEvents() {
	sf::Event event;
	while( mWindow->pollEvent( event ) ) {
		if( event.type == sf::Event::Closed ) {
			TerminateProcess( GetCurrentProcess(), EXIT_SUCCESS ); // Temp fix for crash when closing on Laptop
			mWindow->close();
		}

		HandleInput();

		if( mKeyboard.IsKeyDown( XOF_KEYBOARD::ESCAPE ) ) {
			TerminateProcess( GetCurrentProcess(), EXIT_SUCCESS ); // Temp fix for crash when closing on Laptop
			mWindow->close();
		}
	}
}

void Game::HandleInput() {
	if( mKeyboard.IsKeyDown( XOF_KEYBOARD::A ) ) {
		testFirstPersonCamera.Translate( -0.05f, 0.f, 0.f );
	}

	if( mKeyboard.IsKeyDown( XOF_KEYBOARD::D ) ) {
		testFirstPersonCamera.Translate( 0.05f, 0.f, 0.f );
	}

	if( mKeyboard.IsKeyDown( XOF_KEYBOARD::W ) ) {
		testFirstPersonCamera.Translate( 0.f, 0.f, -0.05f );
	}

	if( mKeyboard.IsKeyDown( XOF_KEYBOARD::S ) ) {
		testFirstPersonCamera.Translate( 0.f, 0.f, 0.05f );
	}
	
	if( mKeyboard.IsKeyDown( XOF_KEYBOARD::Q ) ) {
		testFirstPersonCamera.Translate( 0.f, 0.05f, 0.f );
	}

	if( mKeyboard.IsKeyDown( XOF_KEYBOARD::E ) ) {
		testFirstPersonCamera.Translate( 0.f, -0.05f, 0.f );
	}

	if( mKeyboard.IsKeyDown( XOF_KEYBOARD::ARROW_LEFT ) ) {
		testFirstPersonCamera.Yaw( 0.05f );
	}

	if( mKeyboard.IsKeyDown( XOF_KEYBOARD::ARROW_RIGHT ) ) {
		testFirstPersonCamera.Yaw( -0.05f );
	}

	if( mKeyboard.IsKeyDown( XOF_KEYBOARD::ARROW_UP ) ) {
		testFirstPersonCamera.Pitch( 0.05f );
	}

	if( mKeyboard.IsKeyDown( XOF_KEYBOARD::ARROW_DOWN ) ) {
		testFirstPersonCamera.Pitch( -0.05f );
	}
}

void Game::Render() {
	mRenderer.ClearScreen();
	
	testTransform.mPos.x = 3.f;
	//testTransform.mPos.x = sinf( counter );
	//testTransform.mRot.y = cosf( counter );
	//testTransform.mRot.z = counter * 25.f;
	//testTransform.mScale = glm::vec3( cosf( counter ), cosf( counter ), cosf( counter ) );
	//testTransform.mScale.y = 0.5f;
	//testTransform.mScale.x = testTransform.mScale.z = 10.f;
	counter += 0.001f;

	// modify the transform value for each mesh before drawing it
	testShader->SetUniform( "transform", testFirstPersonCamera.GetViewProjection() * testTransform.GetModelMatrix() );
	testShader->SetUniform( "world", testTransform.GetModelMatrix() );
	testShader->SetUniform( "directionalLight.direction", glm::normalize( testDirectionalLight.direction ) );
	testShader->SetUniform( "directionalLight.color", testDirectionalLight.color );
	testShader->SetUniform( "directionalLight.ambientIntensity", testDirectionalLight.ambientIntensity );
	testShader->SetUniform( "directionalLight.diffuseIntensity", testDirectionalLight.diffuseIntensity );
	testShader->SetUniform( "eyeWorldPos", const_cast<glm::vec3&>( testFirstPersonCamera.GetPosition() ) );
	testShader->SetUniform( "matSpecularIntensity", 1.f );
	testShader->SetUniform( "specularPower", 64.f );
	//testMesh2.Draw();
	meshPtr->Draw();

	// Display will swap the OpenGL buffers for us
	mWindow->display();
}
