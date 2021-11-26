/**
 CAirplane3D
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
class CAirplane3D : public CSolidObject
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
	CAirplane3D(void);

	// Constructor with vectors
	CAirplane3D(const glm::vec3 vec3Position,
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

    // Processes input received from any keyboard-like input system as player movements. 
	// Accepts input parameter in the form of Player defined ENUM (to abstract it from windowing systems)
	void ProcessMovement(const AIRPLANE_MOVEMENT direction, const float fDeltaTime);

	CPlayer3D* GetPlayer3D();

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

	float accel;
	float speed;
	glm::vec3 velocity;
	float maxSpeed;

	// Destructor
	virtual ~CAirplane3D(void);

	// Constraint the player's position
	void Constraint(void);

	void ProcessAirplaneInputs(double dElapsedTime);
};
