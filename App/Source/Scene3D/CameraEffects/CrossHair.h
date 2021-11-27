/**
 CCrossHair
 By: Toh Da Jun
 Date: Sep 2021
 */
#pragma once

// Include CCameraEffects
#include "CameraEffects.h"
#include <vector>

class CCrossHair : public CCameraEffects
{
public:

	enum CROSSHAIR_TYPE
	{
		CROSSHAIR_DOT,
		CROSSHAIR_SPREAD,
		CROSSHAIR_TOTAL
	};
	// Constructor
	CCrossHair(void);
	// Destructor
	virtual ~CCrossHair(void);

	// Initialise this class instance
	bool Init(void);

	// Update this class instance
	virtual bool Update(const double dElapsedTime);

	CROSSHAIR_TYPE activeCrosshair;

	void SetCrossHairType(CROSSHAIR_TYPE type);
protected:
	GLuint iTextureIDArray[CROSSHAIR_TOTAL];
};
