/**
 CScene3D
 By: Toh Da Jun
 Date: Mar 2020
 */
#include "Scene3D.h"

// Include GLEW
#ifndef GLEW_STATIC
	#define GLEW_STATIC
	#include <GL/glew.h>
#endif

// Include this for glm::to_string() function
#define GLM_ENABLE_EXPERIMENTAL
#include <includes/gtx/string_cast.hpp>

// Include filesystem to read from a file
#include "System\filesystem.h"

// Include CShaderManager
#include "RenderControl/ShaderManager.h"

#include <iostream>
using namespace std;

/**
 @brief Constructor This constructor has protected access modifier as this class will be a Singleton
 */
CScene3D::CScene3D(void)
	: cSettings(NULL)
	, cKeyboardController(NULL)
	, cMouseController(NULL)
	, cFPSCounter(NULL)
	, cSoundController(NULL)
	, cCamera(NULL)
	, cSkybox(NULL)
	, cTerrain(NULL)
	, cSolidObjectManager(NULL)
	, cPlayer3D(NULL)
{
}

/**
 @brief Destructor
 */
CScene3D::~CScene3D(void)
{
	// Destroy the camera
	if (cCamera)
	{
		cCamera->Destroy();
		cCamera = NULL;
	}

	// Destroy the cSoundController
	if (cSoundController)
	{
		cSoundController->Destroy();
		cSoundController = NULL;
	}

	// Destroy the skybox
	if (cSkybox)
	{
		cSkybox->Destroy();
		cSkybox = NULL;
	}

	// Destroy the terrain
	if (cTerrain)
	{
		cTerrain->Destroy();
		cTerrain = NULL;
	}	

	// Destroy the terrain
	if (cPlayer3D)
	{
		cSolidObjectManager->Erase(cPlayer3D);
		cPlayer3D->Destroy();
		cPlayer3D = NULL;
	}

	// Destroy the terrain
	if (cSolidObjectManager)
	{
		cSolidObjectManager->Destroy();
		cSolidObjectManager = NULL;
	}

	// We won't delete this since it was created elsewhere
	cFPSCounter = NULL;

	// We won't delete this since it was created elsewhere
	cMouseController = NULL;

	// We won't delete this since it was created elsewhere
	cKeyboardController = NULL;
	
	// We won't delete this since it was created elsewhere
	cSettings = NULL;
}

/**
 @brief Init Initialise this instance
 @return true if the initialisation is successful, else false
 */ 
bool CScene3D::Init(void)
{
	cSettings = CSettings::GetInstance();

	// configure global opengl state
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Configure the camera
	cCamera = CCamera::GetInstance();
	cCamera->vec3Position = glm::vec3(0.0f, 0.5f, 3.0f);

	// Store the keyboard controller singleton instance here
	cKeyboardController = CKeyboardController::GetInstance();

	// Store the mouse controller singleton instance here
	cMouseController = CMouseController::GetInstance();

	// Store the CFPSCounter singleton instance here
	cFPSCounter = CFPSCounter::GetInstance();

	// Store the CSkyBox singleton instance here
	cSkybox = CSkyBox::GetInstance();
	cSkybox->SetShader("Shader3D_SkyBox");
	cSkybox->Init();

	// Store the CSkyBox singleton instance here
	cTerrain = CTerrain::GetInstance();
	cTerrain->SetShader("Shader3D_Terrain");
	cTerrain->Init();
	// Set the size of the terrain
	cTerrain->SetRenderSize(100.f, 5.0f, 100.f);

	// Load the movable Entities
	// Init the CSolidObjectManager
	cSolidObjectManager = CSolidObjectManager::GetInstance();
	cSolidObjectManager->Init();

	// Init the cPlayer3D
	cPlayer3D = CPlayer3D::GetInstance();
	cPlayer3D->SetPosition(glm::vec3(0.0f, 0.5f, 0.0f));
	cPlayer3D->SetShader("Shader3D");
	cPlayer3D->Init();
	cPlayer3D->InitCollider("Shader3D_Line", glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	cPlayer3D->AttachCamera(cCamera);

	// Add the cPlayer3D to the cSolidObjectManager
	cSolidObjectManager->Add(cPlayer3D);

	// Load the sounds into CSoundController
	cSoundController = CSoundController::GetInstance();
	cSoundController->Init();
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\Sound_Bell.ogg"), 1, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\Sound_Explosion.ogg"), 2, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\Sound_Jump.ogg"), 3, true);

	return true;
}

/**
 @brief Update Update this instance
 @param dElapsedTime A const double variable contains the time since the last frame
 @return A bool variable
*/
bool CScene3D::Update(const double dElapsedTime)
{
	// Store the current position, if rollback is needed
	cPlayer3D->StorePositionForRollback();

	// Get keyboard updates for cPlayer3D

	//Player Movement
	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_W))
	{
		if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_LEFT_SHIFT))
			cPlayer3D->ProcessMovement(CPlayer3D::PLAYERMOVEMENT::FORWARD, CPlayer3D::PLAYER_SPEED::SPRINT, (float)dElapsedTime);
		else
			cPlayer3D->ProcessMovement(CPlayer3D::PLAYERMOVEMENT::FORWARD, CPlayer3D::PLAYER_SPEED::WALK, (float)dElapsedTime);
	}
	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_A))
		cPlayer3D->ProcessMovement(CPlayer3D::PLAYERMOVEMENT::LEFT, CPlayer3D::PLAYER_SPEED::WALK, (float)dElapsedTime);
	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_S))
		cPlayer3D->ProcessMovement(CPlayer3D::PLAYERMOVEMENT::BACKWARD, CPlayer3D::PLAYER_SPEED::WALK, (float)dElapsedTime);
	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_D))
		cPlayer3D->ProcessMovement(CPlayer3D::PLAYERMOVEMENT::RIGHT, CPlayer3D::PLAYER_SPEED::WALK, (float)dElapsedTime);
	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_SPACE))
		cPlayer3D->SetToJump();
	
	// Get keyboard and mouse updates for camera
	if (CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_0))
	{
		if (cPlayer3D->IsCameraAttached())
			cPlayer3D->AttachCamera();
		else
			cPlayer3D->AttachCamera(cCamera);

		// Reset the key so that it will not repeat until the key is released and pressed again
		CKeyboardController::GetInstance()->ResetKey(GLFW_KEY_0);
	}

	if (!cPlayer3D->IsCameraAttached())
	{
		if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_I))
			cCamera->ProcessKeyboard(CCamera::CAMERAMOVEMENT::FORWARD, (float)dElapsedTime);
		if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_K))
			cCamera->ProcessKeyboard(CCamera::CAMERAMOVEMENT::BACKWARD, (float)dElapsedTime);
		if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_J))
			cCamera->ProcessKeyboard(CCamera::CAMERAMOVEMENT::LEFT, (float)dElapsedTime);
		if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_L))
			cCamera->ProcessKeyboard(CCamera::CAMERAMOVEMENT::RIGHT, (float)dElapsedTime);
		// Get mouse updates
		cCamera->ProcessMouseMovement((float)cMouseController->GetMouseDeltaX(),
			(float)cMouseController->GetMouseDeltaY());
		cCamera->ProcessMouseScroll((float)cMouseController->GetMouseScrollStatus(CMouseController
			::SCROLL_TYPE::SCROLL_TYPE_YOFFSET));
	}
	else
	{
		// Get mouse updates
		cPlayer3D->ProcessRotate((float)cMouseController->GetMouseDeltaX(),
			(float)cMouseController->GetMouseDeltaY());
		cCamera->ProcessMouseScroll((float)cMouseController->GetMouseScrollStatus(CMouseController
			::SCROLL_TYPE::SCROLL_TYPE_YOFFSET));
	}

	// Post Update the mouse controller
	cMouseController->PostUpdate();

	// Update the Solid Objects
	cSolidObjectManager->Update(dElapsedTime);
	return true;
}

/**
 @brief PreRender Set up the OpenGL display environment before rendering
 */
void CScene3D::PreRender(void)
{
	// Reset the OpenGL rendering environment
	glLoadIdentity();

	// Clear the screen and buffer
	glClearColor(0.0f, 0.1f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/**
 @brief Render Render this instance
 */
void CScene3D::Render(void)
{
	glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)

							 // Part 2: Render the entire scene as per normal
	// Get the camera view and projection

	glm::mat4 view = CCamera::GetInstance()->GetViewMatrix();;
	glm::mat4 projection = glm::perspective(	glm::radians(CCamera::GetInstance()->fZoom),
												(float)cSettings->iWindowWidth / (float)cSettings->iWindowHeight,
												0.1f, 1000.0f);
	glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Render Skybox
	cSkybox->SetView(view);
	cSkybox->SetProjection(projection);
	cSkybox->PreRender();
	cSkybox->Render();
	cSkybox->PostRender();

	//Render Terrain
	cTerrain->SetView(view);
	cTerrain->SetProjection(projection);
	cTerrain->PreRender();
	cTerrain->Render();
	cTerrain->PostRender();

	//Render the solid objects
	cSolidObjectManager->SetView(view);

	cSolidObjectManager->SetProjection(projection);
	cSolidObjectManager->Render();

	glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.

	return;
}

/**
 @brief PostRender Set up the OpenGL display environment after rendering.
 */
void CScene3D::PostRender(void)
{
}