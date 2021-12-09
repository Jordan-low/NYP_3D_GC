/**
 CTurret
 By: Jordan Low
 Date: Nov 2021
 */
#pragma once

// Include CWeaponInfo
#include "WeaponInfo.h"

class CTurret : public CWeaponInfo
{
public:
	// Default Constructor
	CTurret(void);
	// Destructor
	virtual ~CTurret(void);

	// Initialise this instance to default values
	bool Init(void);
};

