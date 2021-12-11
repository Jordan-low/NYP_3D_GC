/**
 CCrossHair
 By: Toh Da Jun
 Date: Sep 2021
 */
#include "CrossHair.h"

 // Include GLEW
#ifndef GLEW_STATIC
#define GLEW_STATIC
#include <GL/glew.h>
#endif

// Include ShaderManager
#include "RenderControl/ShaderManager.h"

// Include ImageLoader
#include "System\ImageLoader.h"

// Include MeshBuilder
#include "Primitives/MeshBuilder.h"

#include <iostream>
using namespace std;

/**
 @brief Default Constructor
 */
CCrossHair::CCrossHair(void)
{
}

/**
 @brief Destructor
 */
CCrossHair::~CCrossHair(void)
{
}

/**
 @brief Initialise this class instance
 @return true is successfully initialised this class instance, else false
 */
bool CCrossHair::Init(void)
{
	// Call the parent's Init()
	CCameraEffects::Init();

	// Set the type
	SetType(CEntity3D::TYPE::OTHERS);

	transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first

	float fAspectRatio = (float)cSettings->iWindowWidth / (float)cSettings->iWindowHeight;

	// Generate and bind the VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	bStatus = true;

	// Generate the mesh
	mesh = CMeshBuilder::GenerateQuad(glm::vec4(1, 1, 1, 1), .05, .05f);

	// load and create textures for all types of crosshairs
	iTextureIDArray[CROSSHAIR_DOT] = CImageLoader::GetInstance()->LoadTextureGetID("Image/Crosshair/dot.png", true);
	iTextureIDArray[CROSSHAIR_SPREAD] = CImageLoader::GetInstance()->LoadTextureGetID("Image/Crosshair/spread.png", true);

	//// load and create a texture 
	//iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Image/Crosshair/dot.png", true);
	if (iTextureIDArray[CROSSHAIR_DOT] == 0)
	{
		cout << "Unable to load Image/Crosshair/dot.png" << endl;
		return false;
	}
	else if (iTextureIDArray[CROSSHAIR_SPREAD] == 0)
	{
		cout << "Unable to load Image/Crosshair/spread.png" << endl;
		return false;
	}

	//default crosshair type to dot
	activeCrosshair = CROSSHAIR_DOT;
	iTextureID = iTextureIDArray[activeCrosshair];

	return true;
}

/**
 @brief Update the elapsed time
 @param dt A const double variable containing the elapsed time since the last frame
 @return A bool variable
 */
bool CCrossHair::Update(const double dElapsedTime)
{
	iTextureID = iTextureIDArray[activeCrosshair];
	return true;
}

void CCrossHair::SetCrossHairType(CROSSHAIR_TYPE type)
{
	activeCrosshair = type;
}
