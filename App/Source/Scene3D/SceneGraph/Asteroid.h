/**
 COrbitPlanet
 By: Jordan Low
 Date: Jan 2022
 */
#pragma once

// Include CSceneNode
#include "SceneNode.h"

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

#include <string>
using namespace std;

// A tower which spins
class CAsteroid
{
public:
	// Create the Spin Tower using this static method
	static bool Create(void);

	// PrintSelf
	void PrintSelf(void);

protected:
	// Constructor
	CAsteroid(void);

	// Destructor
	virtual ~CAsteroid(void);
};
