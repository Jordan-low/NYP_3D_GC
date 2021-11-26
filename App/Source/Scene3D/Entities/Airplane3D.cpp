/**
 CAirplane3D
 By: Jordan Low
 Date: Nov 2021
 */
#include "Airplane3D.h"

 // Include ShaderManager
#include "RenderControl/ShaderManager.h"

// Include ImageLoader
#include "System/ImageLoader.h"

//For allowing creating of Mesh 
#include "Primitives/MeshBuilder.h"

#include "../../MyMath.h"

#include <iostream>
using namespace std;

/**
 @brief Default Constructor
 */
CAirplane3D::CAirplane3D(void)
	: vec3Up(glm::vec3(0.0f, 1.0f, 0.0f))
	, vec3Right(glm::vec3(1.0f, 1.0f, 0.0f))
	, vec3WorldUp(glm::vec3(0.0f, 1.0f, 0.0f))
	, fYaw(-90.0f)
	, fPitch(0.0f)
	, cCamera(NULL)
	, cTerrain(NULL)
	, cPlayer3D(NULL)
	, velocity(0)
	, maxSpeed(4)
	, accel(0)
	, speed(0)
{
	// Set the default position so it is above the ground
	vec3Position = glm::vec3(0.0f, 0.5f, 0.0f);
}

/**
 @brief Constructor with vectors
 @param position A const glm::vec3 variable which contains the position of the camera
 @param up A const glm::vec3 variable which contains the up direction of the camera
 @param yaw A const float variable which contains the yaw of the camera
 @param pitch A const float variable which contains the pitch of the camera
 */
CAirplane3D::CAirplane3D(	const glm::vec3 vec3Position,
						const glm::vec3 vec3Front,
						const float fYaw,
						const float fPitch)
	: vec3Up(glm::vec3(0.0f, 1.0f, 0.0f))
	, vec3Right(glm::vec3(1.0f, 1.0f, 0.0f))
	, vec3WorldUp(glm::vec3(0.0f, 1.0f, 0.0f))
	, fYaw(fYaw)
	, fPitch(fPitch)
	, cCamera(NULL)
	, cTerrain(NULL)
	, cPlayer3D(NULL)
	, velocity(0)
	, maxSpeed(4)
	, accel(0)
	, speed(0)
{
	mesh = NULL;

	this->vec3Position = vec3Position;
	this->vec3Front = vec3Front;
	this->fYaw = fYaw;
	this->fPitch = fPitch;
}

/**
 @brief Destructor
 */
CAirplane3D::~CAirplane3D(void)
{
	if (cTerrain)
	{
		// We set it to NULL only since it was declared somewhere else
		cTerrain = NULL;
	}
	
	if (cCamera)
	{
		// We set it to NULL only since it was declared somewhere else
		cCamera = NULL;
	}
}

/**
 @brief Initialise this class instance
 @return true is successfully initialised this class instance, else false
 */
bool CAirplane3D::Init(void)
{
	// Call the parent's Init()
	CSolidObject::Init();

	// Set the type
	SetType(CEntity3D::TYPE::AIRPLANE);

	// Generate and bind the VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//CS: Create the Quad Mesh using the mesh builder
	mesh = CMeshBuilder::GenerateBox(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		1.0f, 1.0f, 1.0f);

	// load and create a texture 
	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Image/Scene3D_Player.tga", false);
	if (iTextureID == 0)
	{
		cout << "Unable to load Image/Scene3D_Player.tga" << endl;
		return false;
	}

	// Store the handler to the CTerrain
	cTerrain = CTerrain::GetInstance();
	// Update the y-axis position of the player
	vec3Position.y = cTerrain->GetHeight(vec3Position.x, vec3Position.z);

	// Set the Physics to fall status by default
	cPhysics3D.SetStatus(CPhysics3D::STATUS::FALL);
	cPhysics3D.SetGravity(glm::vec3(0.f));

	fMovementSpeed = 100.f;
	return true;
}

/**
 @brief Set model
 @param model A const glm::mat4 variable containing the model for this class instance
 */
void CAirplane3D::SetModel(const glm::mat4 model)
{
	this->model = model;
}

/**
 @brief Set view
 @param view A const glm::mat4 variable containing the model for this class instance
 */
void CAirplane3D::SetView(const glm::mat4 view)
{
	this->view = view;
}

/**
 @brief Set projection
 @param projection A const glm::mat4 variable containing the model for this class instance
 */
void CAirplane3D::SetProjection(const glm::mat4 projection)
{
	this->projection = projection;
}

/**
@brief Returns the view matrix calculated using Euler Angles and the LookAt Matrix
@return A glm::mat4 variable which contains the view matrix
*/
glm::mat4 CAirplane3D::GetViewMatrix(void) const
{
	return glm::lookAt(vec3Position, vec3Position + vec3Front, vec3Up);
}

CPhysics3D CAirplane3D::GetPhysics()
{
	return cPhysics3D;
}

/**
 @brief Processes input received from any keyboard-like input system as player movements. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
 @param direction A const Player_Movement variable which contains the movement direction of the camera
 @param deltaTime A const float variable which contains the delta time for the realtime loop
 */
void CAirplane3D::ProcessMovement(const AIRPLANE_MOVEMENT direction, const float deltaTime)
{
	if (cPlayer3D == NULL)
		return;

	float accel = fMovementSpeed * deltaTime;
	velocity += accel * deltaTime;
	//velocity = Math::Clamp(velocity, -maxSpeed, maxSpeed);

	if (direction == AIRPLANE_MOVEMENT::FORWARD)
		vec3Position += vec3Front * velocity;
	if (direction == AIRPLANE_MOVEMENT::BACKWARD)
		vec3Position -= vec3Front * velocity;
	if (direction == AIRPLANE_MOVEMENT::LEFT)
		vec3Position -= vec3Right * velocity;
	if (direction == AIRPLANE_MOVEMENT::RIGHT)
		vec3Position += vec3Right * velocity;

	cPlayer3D->SetPosition(vec3Position);
}

CPlayer3D* CAirplane3D::GetPlayer3D()
{
	return cPlayer3D;
}

/**
 @brief Update the elapsed time
 @param dt A const double variable containing the elapsed time since the last frame
 @return A bool variable
 */
bool CAirplane3D::Update(const double dElapsedTime)
{
	if (!cPlayer3D)
	{
		if (glm::distance(vec3Position, CPlayer3D::GetInstance()->GetPosition()) < 5)
		{
			if (CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_E))
			{
				cPlayer3D = CPlayer3D::GetInstance();
				cCamera = CCamera::GetInstance();
				cPlayer3D->attachedAirplane = true;
			}
		}
	}
	else
	{
		accel = 0;
		float friction = speed * -1.f;
		speed += friction * dElapsedTime;
		vec3Front = cPlayer3D->GetFront();
		vec3Up = cPlayer3D->vec3Up;
		vec3Right = cPlayer3D->vec3Right;
		fYaw = cPlayer3D->fYaw;
		fPitch = cPlayer3D->fPitch;

		ProcessAirplaneInputs(dElapsedTime);

		speed += accel * dElapsedTime;
		velocity = vec3Front * speed * (float)dElapsedTime;

		if (glm::length(velocity) > 0.5f)
			velocity = glm::normalize(velocity) * 0.5f;

		std::cout << "ACCEL: " << accel << " SPEED: " << speed << " VEL: " << glm::length(velocity) << std::endl;
		vec3Position += velocity;
	}

	// If the camera is attached to this player, then update the camera
	if (cCamera)
	{
		cCamera->vec3Position = vec3Position + vec3Up * 2.f + vec3Front * -7.f;
		cCamera->vec3Front = vec3Front;
		cCamera->vec3Up = vec3Up;
		cCamera->vec3Right = vec3Right;
		cCamera->fYaw = fYaw;
		cCamera->fPitch = fPitch;
	}

	// Constraint the player's position
	Constraint();

	CSolidObject::Update(dElapsedTime);

	return true;
}

/**
 @brief PreRender Set up the OpenGL display environment before rendering
 */
void CAirplane3D::PreRender(void)
{
	CSolidObject::PreRender();
}

/**
 @brief Render Render this instance
 */
void CAirplane3D::Render(void)
{
	CSolidObject::Render();
}

/**
 @brief PostRender Set up the OpenGL display environment after rendering.
 */
void CAirplane3D::PostRender(void)
{
	CSolidObject::PostRender();
}

/**
 @brief Constraint the player's position
 */
void CAirplane3D::Constraint(void)
{
	// If the player is not jumping nor falling, then we snap his position to the terrain
	if (cPhysics3D.GetStatus() == CPhysics3D::STATUS::IDLE)
	{
		vec3Position.y = cTerrain->GetHeight(vec3Position.x, vec3Position.z) + fHeightOffset;
	}
	else
	{
		// If the player is jumping nor falling, then we only snap his position 
		// if the player's y-coordinate is below the cTerrain 
		float fCheckHeight = cTerrain->GetHeight(vec3Position.x, vec3Position.z) + fHeightOffset;

		// If the player is below the terrain, then snap to the terrain height
		if (fCheckHeight > vec3Position.y)
			vec3Position.y = fCheckHeight;
	}
}

void CAirplane3D::ProcessAirplaneInputs(double dElapsedTime)
{
	if (CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_E))
	{
		cPlayer3D->attachedAirplane = false;
		cPlayer3D = NULL;
		cCamera = NULL;
		return;
	}
	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_A))
	{
		//ProcessMovement(CAirplane3D::AIRPLANE_MOVEMENT::LEFT, (float)dElapsedTime);
	}
	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_S))
	{
		accel = -fMovementSpeed;
		//ProcessMovement(CAirplane3D::AIRPLANE_MOVEMENT::BACKWARD, (float)dElapsedTime);
	}
	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_D))
	{
		//ProcessMovement(CAirplane3D::AIRPLANE_MOVEMENT::RIGHT, (float)dElapsedTime);
	}
	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_W))
	{
		accel = fMovementSpeed;
		//ProcessMovement(CAirplane3D::AIRPLANE_MOVEMENT::FORWARD, (float)dElapsedTime);
	}
}

/**
 @brief Print Self
 */
void CAirplane3D::PrintSelf(void)
{
	cout << "CPlayer3D::PrintSelf()" << endl;
	cout << "========================" << endl;
}
