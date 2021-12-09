/**
 CCar3D
 By: Jordan Low
 Date: Nov 2021
 */
#pragma once

// Include SingletonTemplate
#include "DesignPatterns\SingletonTemplate.h"

// Include CSolidObject
#include "SolidObject.h"

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

// Include Camera
#include "../Camera.h"

// Include CTerrain
#include "../Terrain/Terrain.h"

// Include WeaponInfo
#include "../WeaponInfo/WeaponInfo.h"

// Include Physics3D
#include "../Physics3D.h"

#include "Player3D.h"

#include "Inputs/KeyboardController.h"

#include <string>
using namespace std;

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class CCar3D : public CSolidObject
{
public:
	// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
	enum class AIRPLANE_MOVEMENT : unsigned int 
	{
		FORWARD = 0,
		BACKWARD,
		LEFT,
		RIGHT,
		NUM_MOVEMENTS
	};

	// Player Attributes
	glm::vec3 vec3Up;
	glm::vec3 vec3Right;
	glm::vec3 vec3WorldUp;
	// Euler Angles
	float fYaw;
	float fPitch;

	// Default Constructor
	CCar3D(void);

	// Constructor with vectors
	CCar3D(const glm::vec3 vec3Position,
		const glm::vec3 vec3Front = glm::vec3(0.0f, 0.0f, -1.0f),
		const float fYaw = -90.0f,
		const float fPitch = 0.0f);

	// Initialise this class instance
	bool Init(void);

	// Set model
	virtual void SetModel(const glm::mat4 model);
	// Set view
	virtual void SetView(const glm::mat4 view);
	// Set projection
	virtual void SetProjection(const glm::mat4 projection);

	// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix(void) const;

	CPhysics3D GetPhysics();

	void ProcessMovement(double dt);

	CPlayer3D* GetPlayer3D();

	// Set Weapon to this class instance
	void SetWeapon(CWeaponInfo* cWeaponInfo = NULL);

	// Get current weapon of this class instance
	CWeaponInfo* GetWeapon(void) const;

	// Update this class instance
	virtual bool Update(const double dElapsedTime);

	// PreRender
	virtual void PreRender(void);
	// Render
	virtual void Render(void);
	// PostRender
	virtual void PostRender(void);

	// Print Self
	virtual void PrintSelf(void);

protected:
	// The handle to the CCamera class instance
	CCamera* cCamera;

	// The handle to the CTerrain class instance
	CTerrain* cTerrain;

	// Physics
	CPhysics3D cPhysics3D;

	// Player
	CPlayer3D* cPlayer3D;

	// Weapon
	CWeaponInfo* cWeapon;

	float accel;
	float currSpeed;
	float torque;
	float torqueSpeed;
	float tiltAngle;
	glm::vec3 velocity;
	float maxSpeed;
	bool thirdPersonView;

	// Destructor
	virtual ~CCar3D(void);

	// Constraint the player's position
	void Constraint(void);

	void ProcessCarInputs(double dElapsedTime);

	// Calculates the front vector from the Player's (updated) Euler Angles
	void UpdatePlayerVectors(void);
};
