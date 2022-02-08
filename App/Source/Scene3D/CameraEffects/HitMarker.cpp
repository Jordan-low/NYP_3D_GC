/**
 CBloodScreen
 By: Toh Da Jun
 Date: Apr 2020
 */
#include "HitMarker.h"

// Include GLEW
#ifndef GLEW_STATIC
#define GLEW_STATIC
#include <GL/glew.h>
#endif

// Include ImageLoader
#include "System\ImageLoader.h"
#include "Inputs\KeyboardController.h"
// Include MeshBuilder
#include "Primitives/MeshBuilder.h"

#include <iostream>
using namespace std;

/**
 @brief Default Constructor
 */
CHitMarker::CHitMarker(void)
{
}

/**
 @brief Destructor
 */
CHitMarker::~CHitMarker(void)
{
}

/**
 @brief Initialise this class instance
 @return true is successfully initialised this class instance, else false
 */
bool CHitMarker::Init(void)
{
	displayTime = 0.0;
	maxDisplayTime = 2.0;
	angle = 0;
	bStatus = false;

	// Call the parent's Init()
	CCameraEffects::Init();

	// Set the type
	SetType(CEntity3D::TYPE::OTHERS);

	transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first

	// Generate and bind the VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Generate the mesh
	mesh = CMeshBuilder::GenerateQuad(glm::vec4(1, 1, 1, 1), 0.5f, 0.5f);

	// load and create a texture 
	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Image/GUI/HitMarker.png", false);
	if (iTextureID == 0)
	{
		cout << "Unable to load Image/GUI/CameraEffects_Heal.png" << endl;
		return false;
	}
	return true;
}

bool CHitMarker::Update(const double dElapsedTime)
{
	model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	model = glm::translate(model, glm::vec3(0,0,0));
	// reduce the angle  
	angle = (int)angle % 360;

	// force it to be the positive remainder, so that 0 <= angle < 360  
	angle = (int)(angle + 360) % 360;

	// force into the minimum absolute value residue class, so that -180 < angle <= 180  
	if (angle > 180)
		angle -= 360;

	if (angle < 90 && angle > 0)
		angle -= 90;

	model = glm::rotate(model, glm::radians(angle), glm::vec3(0,0,1));

	if (bStatus)
	{
		displayTime -= dElapsedTime;
		if (displayTime <= 0)
		{
			bStatus = false;
			displayTime = maxDisplayTime;
		}
	}

	return true;
}
