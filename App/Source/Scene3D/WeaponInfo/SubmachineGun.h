/**
 CPistol
 By: Toh Da Jun
 Date: Sep 2020
 */
#pragma once

// Include CWeaponInfo
#include "WeaponInfo.h"

class CSubmachineGun : public CWeaponInfo
{
public:
	// Default Constructor
	CSubmachineGun(void);
	// Destructor
	virtual ~CSubmachineGun(void);

	// Initialise this instance to default values
	bool Init(void);
};

