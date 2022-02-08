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
#include "../Application.h"
// Include filesystem to read from a file
#include "System\filesystem.h"
#include "../MyMath.h"

// Include CShaderManager
#include "RenderControl/ShaderManager.h"

#include "Entities/TreeKabak3D.h"
#include "Entities/Creature3D.h"

#include "Entities/Hut_Concrete.h"
#include "Entities/HangerA.h"
#include "Entities/Building.h"
#include "CameraEffects/CameraShake.h"
#include <iostream>

// Include CSpinTower
#include "SceneGraph/SpinTower.h"
#include "SceneGraph/OrbitPlanet.h"
#include "SceneGraph/JupiterPlanet.h"
#include "SceneGraph/Asteroid.h"

using namespace std;

/**
 @brief Constructor This constructor has protected access modifier as this class will be a Singleton
 */
CScene3D::CScene3D(void)
	: cSettings(NULL)
	, cKeyboardController(NULL)
	, cMouseController(NULL)
	, cFPSCounter(NULL)
	, cGUI_Scene3D(NULL)
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

	// Destroy the cSoundController
	if (cGUI_Scene3D)
	{
		cGUI_Scene3D->Destroy();
		cGUI_Scene3D = NULL;
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

void CScene3D::ProcessPlayerInputs(double dElapsedTime)
{
	// Get keyboard 1s for cPlayer3D
	//Player Movement
	cPlayer3D->activeState = CPlayer3D::PLAYER_STATE::REST;
	
	if (cPlayer3D->GetWeapon()->isADS)
	{
		if (CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_LEFT_SHIFT))
		{
			cPlayer3D->GetWeapon()->toggleADSCloseZoom = !cPlayer3D->GetWeapon()->toggleADSCloseZoom;
		}
	}


	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_C))
		cPlayer3D->activeState = CPlayer3D::PLAYER_STATE::CROUCH;

	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_A))
	{
		if (cPlayer3D->activeState != CPlayer3D::PLAYER_STATE::CROUCH)
			cPlayer3D->activeState = CPlayer3D::PLAYER_STATE::WALK;
		cPlayer3D->ProcessMovement(CPlayer3D::PLAYERMOVEMENT::LEFT, (float)dElapsedTime);
	}
	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_S))
	{
		((CCameraShake*)CCameraEffectsManager::GetInstance()->Get("CameraShake"))->bToBeUpdated = true;
		if (cPlayer3D->activeState != CPlayer3D::PLAYER_STATE::CROUCH)
			cPlayer3D->activeState = CPlayer3D::PLAYER_STATE::WALK;
		cPlayer3D->ProcessMovement(CPlayer3D::PLAYERMOVEMENT::BACKWARD, (float)dElapsedTime);
	}
	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_D))
	{
		if (cPlayer3D->activeState != CPlayer3D::PLAYER_STATE::CROUCH)
			cPlayer3D->activeState = CPlayer3D::PLAYER_STATE::WALK;
		cPlayer3D->ProcessMovement(CPlayer3D::PLAYERMOVEMENT::RIGHT, (float)dElapsedTime);
	}
	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_W))
	{
		((CCameraShake*)CCameraEffectsManager::GetInstance()->Get("CameraShake"))->bToBeUpdated = true;
		if (cPlayer3D->activeState != CPlayer3D::PLAYER_STATE::CROUCH)
			cPlayer3D->activeState = CPlayer3D::PLAYER_STATE::WALK;
		if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_LEFT_SHIFT) && !cPlayer3D->GetWeapon()->isADS)
		{
			cPlayer3D->activeState = CPlayer3D::PLAYER_STATE::SPRINT;
			if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_C))
				cPlayer3D->activeState = CPlayer3D::PLAYER_STATE::CROUCH;
		}
		cPlayer3D->ProcessMovement(CPlayer3D::PLAYERMOVEMENT::FORWARD, (float)dElapsedTime);
	}
	if (CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_SPACE))
	{
		cPlayer3D->SetToJump();
		if (cPlayer3D->GetPhysics().GetStatus() == CPhysics3D::STATUS::FALL)
		{
			cPlayer3D->SetToJetpack(dElapsedTime);
		}
	}

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
}

void CScene3D::SpawnEnemy(glm::vec3 pos)
{
	// Initialise a CEnemy3D
	float fCheckHeight = cTerrain->GetHeight(pos.x, pos.z);
	CEnemy3D* cEnemy3D = new CEnemy3D(glm::vec3(pos.x, fCheckHeight, pos.z));
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
	cSolidObjectManager->Add(cEnemy3D);

	//// Initialise a CEnemy3D
	//float fCheckHeight = cTerrain->GetHeight(pos.x, pos.z);
	//CCreature3D* cCreature3D = new CCreature3D(glm::vec3(pos.x, fCheckHeight, pos.z));
	//cCreature3D->SetShader("Shader3D");
	//cCreature3D->Init();
	//cCreature3D->InitCollider("Shader3D_Line", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

	//// Add the cStructure3D to the cSolidObjectManager
	//cSolidObjectManager->Add(cCreature3D);
}

void CScene3D::SpawnStructure(glm::vec3 pos)
{
	//get random values
	int random = Math::RandIntMinMax(2, 6);
	int j = Math::RandIntMinMax(0, 2);
	int k = Math::RandIntMinMax(0, 2);

	//random spawning of structures
	for (int i = 0; i < random; i++)
	{
		if (i % 2 == 0)
		{
			// Initialise a structure
			float fCheckHeight = cTerrain->GetHeight(pos.x + k * 1, pos.z + j * 1);
			CStructure3D* cStructure3D = new CStructure3D(glm::vec3(pos.x + k * 1, fCheckHeight, pos.z + j * 1));
			cStructure3D->SetShader("Shader3D");
			cStructure3D->Init();
			cStructure3D->InitCollider("Shader3D_Line", glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.5f, 0.5f, 0.5f));
			cSolidObjectManager->Add(cStructure3D);

			j += Math::RandIntMinMax(1, 2);
		}
		else
		{
			// Initialise a structure
			float fCheckHeight = cTerrain->GetHeight(pos.x + k * 1, pos.z + j * 1);
			CStructure3D* cStructure3D = new CStructure3D(glm::vec3(pos.x + k * 1, fCheckHeight, pos.z + j * 1));
			cStructure3D->SetShader("Shader3D");
			cStructure3D->Init();
			cStructure3D->InitCollider("Shader3D_Line", glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.5f, 0.5f, 0.5f));
			cSolidObjectManager->Add(cStructure3D);

			k += Math::RandIntMinMax(1, 2);
		}
	}
}

void CScene3D::SpawnAmmoBox(glm::vec3 pos)
{
	// Initialise a structure
	float fCheckHeight = cTerrain->GetHeight(pos.x, pos.z);
	CStructure3D* cStructure3D = new CStructure3D(glm::vec3(pos.x, fCheckHeight, pos.z));
	cStructure3D->SetShader("Shader3D");
	cStructure3D->Init(CEntity3D::TYPE::AMMO);
	cStructure3D->InitCollider("Shader3D_Line", glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.5f, 0.5f, 0.5f));
	cSolidObjectManager->Add(cStructure3D);
}

void CScene3D::SpawnEnemyWave(int waveCount)
{
	for (int i = 0; i < (3 * waveCount); i++)
	{
		float posX = Math::RandFloatMinMax(-125, 125);
		float posZ = Math::RandFloatMinMax(-125, 125);
		SpawnEnemy(glm::vec3(posX, 0, posZ));
	}
	for (int i = 0; i < 3; i++)
	{
		float posX = Math::RandFloatMinMax(-125, 125);
		float posZ = Math::RandFloatMinMax(-125, 125);
		SpawnAmmoBox(glm::vec3(posX, 0, posZ));
	}
}

/**
 @brief Init Initialise this instance
 @return true if the initialisation is successful, else false
 */ 
bool CScene3D::Init(void)
{
	srand(time(NULL));
	cSettings = CSettings::GetInstance();
	savedMouseScrollY = 30;

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
	cTerrain->SetRenderSize(300.f, 5.0f, 300.f);

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

	SpawnEnemy(glm::vec3(-50, 0, 50));
	// Initialise a CSpinTower & CJupiterPlanet
	COrbitPlanet::Create();
	CJupiterPlanet::Create();
	CAsteroid::Create();

	/*for (int i = 0; i < 1; i++)
	{
		float posX = Math::RandFloatMinMax(-125, 125);
		float posZ = Math::RandFloatMinMax(-125, 125);
		SpawnEnemy(glm::vec3(posX, 0, posZ));
	}
	
	for (int i = 0; i < 5; i++)
	{
		float posX = Math::RandFloatMinMax(-125, 125);
		float posZ = Math::RandFloatMinMax(-125, 125);
		SpawnStructure(glm::vec3(posX, 0, posZ));
	}
	
	for (int i = 0; i < 5; i++)
	{
		float posX = Math::RandFloatMinMax(-125, 125);
		float posZ = Math::RandFloatMinMax(-125, 125);
		SpawnAmmoBox(glm::vec3(posX, 0, posZ));
	}*/

	// Initialise the projectile manager
	cProjectileManager = CProjectileManager::GetInstance();
	cProjectileManager->Init();
	cProjectileManager->SetShader("Shader3D");

	//// Assign a cPistol to the cPlayer3D
	CPistol* cPistol = new CPistol();
	// Set the pos, rot, scale of this weapon
	cPistol->SetPosition(glm::vec3(0.05f, -0.075f, -0.3f));
	cPistol->SetRotation(3.14159f, glm::vec3(0.0f, 1.0f, 0.0f));
	cPistol->SetScale(glm::vec3(0.75f, 0.75f, 0.75f));
	//Initialise the instance
	cPistol->Init();
	cPistol->SetShader("Shader3D_Model");

	//// Assign a cAssaultRifle to the cPlayer3D
	CAssaultRifle* cAssaultRifle = new CAssaultRifle();
	// Set the pos, rot, scale of this weapon
	cAssaultRifle->SetPosition(glm::vec3(0.1f, -0.06f, -0.3f));
	cAssaultRifle->SetRotation(3.14159f, glm::vec3(0.0f, 1.0f, 0.0f));
	cAssaultRifle->SetScale(glm::vec3(0.75f, 0.75f, 0.75f));
	//Initialise the instance
	cAssaultRifle->Init();
	cAssaultRifle->SetShader("Shader3D_Model");

	// Assign a cAssaultRifle to the cPlayer3D
	CBurstAssaultRifle* cBurstAssaultRifle = new CBurstAssaultRifle();
	// Set the pos, rot, scale of this weapon
	cBurstAssaultRifle->SetPosition(glm::vec3(0.1f, -0.075f, -0.3f));
	cBurstAssaultRifle->SetRotation(3.14159f, glm::vec3(0.0f, 1.0f, 0.0f));
	cBurstAssaultRifle->SetScale(glm::vec3(0.75f, 0.75f, 0.75f));
	//Initialise the instance
	cBurstAssaultRifle->Init();
	cBurstAssaultRifle->SetShader("Shader3D_Model");

	//// Assign a cSubmachineGun to the cPlayer3D
	//CSubmachineGun* cSubmachineGun = new CSubmachineGun();
	//// Set the pos, rot, scale of this weapon
	//cSubmachineGun->SetPosition(glm::vec3(0.05f, -0.075f, -0.3f));
	//cSubmachineGun->SetRotation(3.14159f, glm::vec3(0.0f, 1.0f, 0.0f));
	//cSubmachineGun->SetScale(glm::vec3(0.75f, 0.75f, 0.75f));
	////Initialise the instance
	//cSubmachineGun->Init();
	//cSubmachineGun->SetShader("Shader3D_Model");

	// Assign a cSubmachineGun to the cPlayer3D
	CKnife* cKnife = new CKnife();
	// Set the pos, rot, scale of this weapon
	cKnife->SetPosition(glm::vec3(0.05f, -0.075f, -0.3f));
	cKnife->SetRotation(3.14159f, glm::vec3(0.0f, 1.0f, 0.0f));
	cKnife->SetScale(glm::vec3(0.75f, 0.75f, 0.75f));
	//Initialise the instance
	cKnife->Init();
	cKnife->SetShader("Shader3D_Model");

	cPlayer3D->SetWeapon(0, cKnife);
	cPlayer3D->SetWeapon(1, cAssaultRifle);
	cPlayer3D->SetWeapon(2, cBurstAssaultRifle);
	cPlayer3D->SetCurrentWeapon(0);

	// Assign a cSubmachineGun to the cPlayer3D
	CTurret* cTurret = new CTurret();
	cTurret->Init();

	CCar3D* car = new CCar3D(glm::vec3(-5.f, 0.5f, 0.0f));
	car->SetShader("Shader3D");
	car->Init();
	car->SetWeapon(cTurret);
	car->InitCollider("Shader3D_Line", glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), glm::vec3(-0.5f, -0.5f, -1.f), glm::vec3(0.5f, 0.5f, 1.f));

	// Add the airplane to the cSolidObjectManager
	cSolidObjectManager->Add(car);

	//// Initialise a CHut_Concrete
	//float fCheckHeight = cTerrain->GetHeight(-2.0f, 2.0f);
	//CHut_Concrete* cHut_Concrete = new CHut_Concrete(glm::vec3(-2.0f, fCheckHeight, 2.0f));
	//cHut_Concrete->SetShader("Shader3DNoColour");
	//cHut_Concrete->SetLODStatus(true);
	//cHut_Concrete->Init();
	//cHut_Concrete->InitCollider("Shader3D_Line", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

	//// Add the cHut_Concrete to the cSolidObjectManager
	//cSolidObjectManager->Add(cHut_Concrete);

	// Initialise a CHut_Concrete
	float fCheckHeight = cTerrain->GetHeight(-20.f, 20.f);
	CHangarA* cHangerA = new CHangarA(glm::vec3(-20.f, fCheckHeight, 20.f));
	cHangerA->SetShader("Shader3DNoColour");
	cHangerA->SetLODStatus(true);
	cHangerA->Init();
	cHangerA->InitCollider("Shader3D_Line", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

	// Add the cHut_Concrete to the cSolidObjectManager
	cSolidObjectManager->Add(cHangerA);

	for (int i = 1; i < 5; i++)
	{
		float fCheckHeight = cTerrain->GetHeight(20.f * i, 20.f);
		CBuilding* cBuilding = new CBuilding(glm::vec3(20.f * i, fCheckHeight, 20.f));
		cBuilding->buildingType = (CBuilding::BUILDING_TYPE)Math::RandIntMinMax(0, 1);
		cBuilding->SetShader("Shader3DNoColour");
		cBuilding->SetLODStatus(true);
		cBuilding->Init();
		cBuilding->InitCollider("Shader3D_Line", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

		// Add the cHut_Concrete to the cSolidObjectManager
		cSolidObjectManager->Add(cBuilding);
	}

	//// Initialise the CTreeKabak3D
	//CTreeKabak3D* cTreeKabak3D = new CTreeKabak3D(glm::vec3(0.0f, 0.0f, 0.0f));
	//cTreeKabak3D->SetInstancingMode(true);
	//if (cTreeKabak3D->IsInstancedRendering() == true)
	//{
	//	cTreeKabak3D->SetScale(glm::vec3(1.0f));
	//	cTreeKabak3D->SetNumOfInstance(100);
	//	cTreeKabak3D->SetSpreadDistance(100.0f);
	//	cTreeKabak3D->SetShader("Shader3D_Instancing"); // FOR INSTANCED RENDERING
	//}
	//if (cTreeKabak3D->Init() == true)
	//	cSolidObjectManager->Add(cTreeKabak3D);
	//else
	//	delete cTreeKabak3D;

	// Load the GUI Entities
	// Store the CGUI_Scene3D singleton instance here
	cGUI_Scene3D = CGUI_Scene3D::GetInstance();
	cGUI_Scene3D->Init();

	// Load the sounds into CSoundController
	cSoundController = CSoundController::GetInstance();
	cSoundController->Init();
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\Sound_Bell.ogg"), 1, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\Sound_Explosion.ogg"), 2, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\Sound_Jump.ogg"), 3, true);

	CCameraEffectsManager::GetInstance()->Get("HitMarker")->SetStatus(false);
	return true;
}

/**
 @brief Update Update this instance
 @param dElapsedTime A const double variable contains the time since the last frame
 @return A bool variable
*/
bool CScene3D::Update(const double dElapsedTime)
{
	//count wave timer
	cPlayer3D->timer += dElapsedTime;

	if (cSolidObjectManager->allEnemyDied)
	{
		//add loading wave timer
		loadingWave += dElapsedTime;

		//start loading new wave
		if (loadingWave > 1)
		{
			cPlayer3D->waveCount += 1;
			SpawnEnemyWave(cPlayer3D->waveCount);
			loadingWave = 0;
			cSolidObjectManager->allEnemyDied = false;
		}
	}
	if (!cPlayer3D->GetStatus())
	{
		//add loading player died timer
		loadingPlayerDied += dElapsedTime;

		//start loading player died screen
		if (loadingPlayerDied > 3)
		{
			loadingPlayerDied = 0;
			return false;
		}
	}
	
	// Store the current position, if rollback is needed
	cPlayer3D->StorePositionForRollback();

	// Update the projectiles
	cProjectileManager->Update(dElapsedTime);

	if (!cPlayer3D->isDriving)
		ProcessPlayerInputs(dElapsedTime);

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
	}

	if (!cPlayer3D->isDriving)
	{
		//Update for weapons
		if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_1))
		{
			cPlayer3D->SetCurrentWeapon(0);
		}
		if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_2))
		{
			cPlayer3D->SetCurrentWeapon(1);
		}
		else if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_3))
		{
			cPlayer3D->SetCurrentWeapon(2);
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
		if (cMouseController->IsButtonDown(CMouseController::BUTTON_TYPE::RMB))
		{
			// Switch on Scope mode and zoom in
			cPlayer3D->GetWeapon()->isADS = true;
			//CCameraEffectsManager::GetInstance()->Get("ScopeScreen")->SetStatus(true);
		}
		else if (cMouseController->IsButtonReleased(CMouseController::BUTTON_TYPE::RMB))
		{
			// Switch off Scope mode and zoom out
			//cCamera->fZoom = 45.0f;
			cPlayer3D->GetWeapon()->isADS = false;
			//CCameraEffectsManager::GetInstance()->Get("ScopeScreen")->SetStatus(false);
		}
	}

	if (CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_9))
	{
		bool bStatus = CCameraEffectsManager::GetInstance()->Get("CrossHair")->GetStatus();
		CCameraEffectsManager::GetInstance()->Get("CrossHair")->SetStatus(!bStatus);

		// Reset the key so that it will not repeat until the key is released and pressed again
		CKeyboardController::GetInstance()->ResetKey(GLFW_KEY_9);
	}
	if (CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_8))
	{
		bool bStatus = CCameraEffectsManager::GetInstance()->Get("CameraShake")->GetStatus();
		CCameraEffectsManager::GetInstance()->Get("CameraShake")->SetStatus(!bStatus);

		// Reset the key so that it will not repeat until the key is released and pressed again
		CKeyboardController::GetInstance()->ResetKey(GLFW_KEY_8);
	}

	//Handle Scroll Wheel Inputs for Camera FOV and Minimap Zoom
	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_LEFT_CONTROL))
		savedMouseScrollY -= (float)cMouseController->GetMouseScrollStatus(CMouseController::SCROLL_TYPE::SCROLL_TYPE_YOFFSET);
	else
		cCamera->ProcessMouseScroll((float)cMouseController->GetMouseScrollStatus(CMouseController::SCROLL_TYPE::SCROLL_TYPE_YOFFSET));

	//Calculate minimap zoom
	savedMouseScrollY = Math::Clamp(savedMouseScrollY, 10.f, 30.f);
	float zoom = 0.5 * savedMouseScrollY;
	CMinimap::GetInstance()->SetMinimapZoom(zoom);

	// Post Update the mouse controller
	cMouseController->PostUpdate();

	cPlayer3D->isHealing = false;

	// Update the Solid Objects
	cSolidObjectManager->Update(dElapsedTime);

	cSolidObjectManager->CheckForCollision();

	//update player's healing health and heal screen
	cPlayer3D->SetHealth(CPlayer3D::GetInstance()->GetHealth() + 0.1f * cPlayer3D->isHealing);
	CCameraEffectsManager::GetInstance()->Get("Healscreen")->SetStatus(cPlayer3D->isHealing);

	// Call the CGUI_Scene3D's update method
	cGUI_Scene3D->Update(dElapsedTime);

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
	// Part 1: Render for the minimap by binding to framebuffer and render to color texture
		//         But the camera is move to top-view of the scene

		// Backup some key settings for the camera and player
	glm::vec3 storePlayerPosition = cPlayer3D->GetPosition();
	float storeCameraYaw = cCamera->fYaw;
	float storeCameraPitch = cCamera->fPitch;
	glm::vec3 storeCameraPosition = cCamera->vec3Position;
	// Adjust camera yaw and pitch so that it is looking from a top-view of the terrain
	cCamera->fYaw += 180.0f;
	cCamera->fPitch = -90.0f;
	// We store the player's position into the camera as we want the minimap to focus on the player
	cCamera->vec3Position = glm::vec3(storePlayerPosition.x, CMinimap::GetInstance()->GetMinimapZoom(), storePlayerPosition.z);
	// Recalculate all the camera vectors. 
	// We disable pitch constrains for this specific case as we want the camera to look straight down
	cCamera->ProcessMouseMovement(0, 0, false);
	// Generate the view and projection
	glm::mat4 playerView = cCamera->GetViewMatrix();
	glm::mat4 playerProjection = glm::perspective(glm::radians(100.f),
		(float)cSettings->iWindowWidth / (float)cSettings->iWindowHeight,
		0.1f, 1000.0f);

	// Set the camera parameters back to the previous values
	cCamera->fYaw = storeCameraYaw;
	cCamera->fPitch = storeCameraPitch;
	cCamera->vec3Position = storeCameraPosition;
	cCamera->ProcessMouseMovement(0, 0, true); // call this to make sure it updates its camera vectors, note that we disable pitch constrains for this specific case (otherwise we can't reverse camera's pitch values)

	// Activate the minimap system
	CMinimap::GetInstance()->Activate();
	// Setup the rendering environment
	CMinimap::GetInstance()->PreRender();


	glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)

							 // Part 2: Render the entire scene as per normal
	// Render the Terrain
	cTerrain->SetView(playerView);
	cTerrain->SetProjection(playerProjection);
	cTerrain->PreRender();
	cTerrain->Render();
	cTerrain->PostRender();
	// Render the entities for the minimap
	cSolidObjectManager->SetView(playerView);
	cSolidObjectManager->SetProjection(playerProjection);
	cSolidObjectManager->Render();

	// Deactivate the cMinimap so that we can render as per normal
	CMinimap::GetInstance()->Deactivate();

	// Part 2: Render the entire scene as per normal
	// Get the camera view and projection
	glm::mat4 view = CCamera::GetInstance()->GetViewMatrix();
	glm::mat4 projection = glm::perspective(glm::radians(CCamera::GetInstance()->fZoom),
		(float)cSettings->iWindowWidth / (float)cSettings->iWindowHeight,
		0.1f, 1000.0f);


	glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	cSkybox->SetView(view);
	cSkybox->SetProjection(projection);
	cSkybox->PreRender();
	cSkybox->Render();
	cSkybox->PostRender();

	// Render the Terrain
	cTerrain->SetView(view);
	cTerrain->SetProjection(projection);
	cTerrain->PreRender();
	cTerrain->Render();
	cTerrain->PostRender();

	cSolidObjectManager->SetView(view);
	cSolidObjectManager->SetProjection(projection);
	cSolidObjectManager->Render();

	//Render the projectiles
	cProjectileManager->SetView(view);
	cProjectileManager->SetProjection(projection);
	cProjectileManager->PreRender();
	cProjectileManager->Render();
	cProjectileManager->PostRender();

	glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.

	cGUI_Scene3D->SetProjection(projection);
	// Call the CGUI_Scene3D's PreRender ()
	cGUI_Scene3D->PreRender();
	// Call the cGUI_Scene3D's Render()
	cGUI_Scene3D->Render();
	// Call the CGUI_Scene3D's PostRender()
	cGUI_Scene3D->PostRender();

	return;
}

/**
 @brief PostRender Set up the OpenGL display environment after rendering.
 */
void CScene3D::PostRender(void)
{
}