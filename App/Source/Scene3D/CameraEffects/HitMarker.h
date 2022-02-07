/**
 CBloodScreen
 By: Toh Da Jun
 Date: Apr 2020
 */
#pragma once

// Include CCameraEffects
#include "CameraEffects.h"

class CHitMarker : public CCameraEffects
{
public:
	// Constructor
	CHitMarker(void);
	// Destructor
	virtual ~CHitMarker(void);

	// Initialise this class instance
	bool Init(void);

	bool Update(const double dElapsedTime);

	float angle;
};
