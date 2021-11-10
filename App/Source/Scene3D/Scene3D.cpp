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
	, cProjectileManager(NULL)
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

	// Destroy the projectile manager
	if (cProjectileManager)
	{
		cProjectileManager->Destroy();
		cProjectileManager = NULL;
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

	// Initialise a CStructure3D
	float fCheckHeight = cTerrain->GetHeight(2.0f, -2.0f);
	CStructure3D* cStructure3D = new CStructure3D(glm::vec3(2.0f, fCheckHeight + .3f, -2.0f));
	cStructure3D->SetShader("Shader3D");
	cStructure3D->SetScale(glm::vec3(5, 1, 1));
	cStructure3D->SetFront(glm::vec3(-1, 0, 0));
	cStructure3D->Init();
	cStructure3D->InitCollider("Shader3D_Line", glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), glm::vec3(-2.5f, -0.5f, -0.5f), glm::vec3(2.5f, 0.5f, 0.5f));

	// Initialise a CEnemy3D
	fCheckHeight = cTerrain->GetHeight(0.0f, -10.0f);
	CEnemy3D* cEnemy3D = new CEnemy3D(glm::vec3(0.0f, fCheckHeight, -10.0f));
	cEnemy3D->SetShader("Shader3D");
	cEnemy3D->Init();
	cEnemy3D->InitCollider("Shader3D_Line", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	// Assign a cPistol to the cEnemy3D
	CPistol* cEnemyPistol = new CPistol();
	// Set the scale of this weapon
	cEnemyPistol->SetScale(glm::vec3(0.75f, 0.75f, 0.75f));
	//Initialise the instance
	cEnemyPistol->Init();
	cEnemyPistol->SetShader("Shader3D_Model");
	cEnemy3D->SetWeapon(0, cEnemyPistol);

	// Add the cStructure3D to the cSolidObjectManager
	cSolidObjectManager->Add(cStructure3D);
	cSolidObjectManager->Add(cEnemy3D);

	// Initialise the projectile manager
	cProjectileManager = CProjectileManager::GetInstance();
	cProjectileManager->Init();
	cProjectileManager->SetShader("Shader3D");

	//// Assign a cPistol to the cPlayer3D
	//CPistol* cPistol = new CPistol();
	//// Set the pos, rot, scale of this weapon
	//cPistol->SetPosition(glm::vec3(0.05f, -0.075f, -0.3f));
	//cPistol->SetRotation(3.14159f, glm::vec3(0.0f, 1.0f, 0.0f));
	//cPistol->SetScale(glm::vec3(0.75f, 0.75f, 0.75f));
	////Initialise the instance
	//cPistol->Init();
	//cPistol->SetShader("Shader3D_Model");

	//// Assign a cAssaultRifle to the cPlayer3D
	//CAssaultRifle* cAssaultRifle = new CAssaultRifle();
	//// Set the pos, rot, scale of this weapon
	//cAssaultRifle->SetPosition(glm::vec3(0.05f, -0.075f, -0.3f));
	//cAssaultRifle->SetRotation(3.14159f, glm::vec3(0.0f, 1.0f, 0.0f));
	//cAssaultRifle->SetScale(glm::vec3(0.75f, 0.75f, 0.75f));
	////Initialise the instance
	//cAssaultRifle->Init();
	//cAssaultRifle->SetShader("Shader3D_Model");

	// Assign a cAssaultRifle to the cPlayer3D
	CBurstAssaultRifle* cBurstAssaultRifle = new CBurstAssaultRifle();
	// Set the pos, rot, scale of this weapon
	cBurstAssaultRifle->SetPosition(glm::vec3(0.05f, -0.075f, -0.3f));
	cBurstAssaultRifle->SetRotation(3.14159f, glm::vec3(0.0f, 1.0f, 0.0f));
	cBurstAssaultRifle->SetScale(glm::vec3(0.75f, 0.75f, 0.75f));
	//Initialise the instance
	cBurstAssaultRifle->Init();
	cBurstAssaultRifle->SetShader("Shader3D_Model");

	// Assign a cSubmachineGun to the cPlayer3D
	CSubmachineGun* cSubmachineGun = new CSubmachineGun();
	// Set the pos, rot, scale of this weapon
	cSubmachineGun->SetPosition(glm::vec3(0.05f, -0.075f, -0.3f));
	cSubmachineGun->SetRotation(3.14159f, glm::vec3(0.0f, 1.0f, 0.0f));
	cSubmachineGun->SetScale(glm::vec3(0.75f, 0.75f, 0.75f));
	//Initialise the instance
	cSubmachineGun->Init();
	cSubmachineGun->SetShader("Shader3D_Model");

	cPlayer3D->SetWeapon(0, cBurstAssaultRifle);
	cPlayer3D->SetWeapon(1, cSubmachineGun);
	cPlayer3D->SetCurrentWeapon(0);

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

	// Update the projectiles
	cProjectileManager->Update(dElapsedTime);

	// Get keyboard updates for cPlayer3D

	//Player Movement
	cPlayer3D->activeState = CPlayer3D::PLAYER_STATE::REST;

	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_C))
		cPlayer3D->activeState = CPlayer3D::PLAYER_STATE::CROUCH;

	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_A))
	{
		cPlayer3D->activeState = CPlayer3D::PLAYER_STATE::WALK;
		cPlayer3D->ProcessMovement(CPlayer3D::PLAYERMOVEMENT::LEFT, (float)dElapsedTime);
	}
	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_S))
	{
		cPlayer3D->activeState = CPlayer3D::PLAYER_STATE::WALK;
		cPlayer3D->ProcessMovement(CPlayer3D::PLAYERMOVEMENT::BACKWARD, (float)dElapsedTime);
	}
	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_D))
	{
		cPlayer3D->activeState = CPlayer3D::PLAYER_STATE::WALK;
		cPlayer3D->ProcessMovement(CPlayer3D::PLAYERMOVEMENT::RIGHT, (float)dElapsedTime);
	}
	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_W))
	{
		if (cPlayer3D->activeState != CPlayer3D::PLAYER_STATE::CROUCH)
			cPlayer3D->activeState = CPlayer3D::PLAYER_STATE::WALK;
		if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_LEFT_SHIFT))
		{
			cPlayer3D->activeState = CPlayer3D::PLAYER_STATE::SPRINT;
			if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_C))
				cPlayer3D->activeState = CPlayer3D::PLAYER_STATE::CROUCH;
		}
		cPlayer3D->ProcessMovement(CPlayer3D::PLAYERMOVEMENT::FORWARD, (float)dElapsedTime);
	}
	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_SPACE))
		cPlayer3D->SetToJump();

	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_G))
		cPlayer3D->GetWeapon()->AnimateEquip(dElapsedTime);

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
	//Update for weapons
	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_1))
	{
		cPlayer3D->SetCurrentWeapon(0);
	}
	else if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_2))
	{
		cPlayer3D->SetCurrentWeapon(1);
	}
	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_R))
	{
		cPlayer3D->GetWeapon()->Reload();
	}
	if (cPlayer3D->GetWeapon()->GetAutoFire())
	{
		if (cMouseController->IsButtonDown(CMouseController::BUTTON_TYPE::LMB))
		{
			cPlayer3D->DischargeWeapon();
		}
	}
	else
	{
		if (cMouseController->IsButtonPressed(CMouseController::BUTTON_TYPE::LMB))
		{
			cPlayer3D->DischargeWeapon();
		}
	}

	// Post Update the mouse controller
	cMouseController->PostUpdate();

	// Update the Solid Objects
	cSolidObjectManager->Update(dElapsedTime);

	cSolidObjectManager->CheckForCollision();

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

	//Render the projectiles
	cProjectileManager->SetView(view);
	cProjectileManager->SetProjection(projection);
	cProjectileManager->PreRender();
	cProjectileManager->Render();
	cProjectileManager->PostRender();

	cPlayer3D->GetWeapon()->SetProjection(projection);
	cPlayer3D->GetWeapon()->PreRender();
	cPlayer3D->GetWeapon()->Render();
	cPlayer3D->GetWeapon()->PostRender();

	glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.

	return;
}

/**
 @brief PostRender Set up the OpenGL display environment after rendering.
 */
void CScene3D::PostRender(void)
{
}