/**
 CBloodScreen
 By: Toh Da Jun
 Date: Apr 2020
 */
#pragma once

// Include CCameraEffects
#include "CameraEffects.h"

class CHealScreen : public CCameraEffects
{
public:
	// Constructor
	CHealScreen(void);
	// Destructor
	virtual ~CHealScreen(void);

	// Initialise this class instance
	bool Init(void);
};
