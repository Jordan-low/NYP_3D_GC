/**
 CPistol
 By: Toh Da Jun
 Date: Sep 2020
 */
#pragma once

// Include CWeaponInfo
#include "WeaponInfo.h"

class CAssaultRifle : public CWeaponInfo
{
public:
	// Default Constructor
	CAssaultRifle(void);
	// Destructor
	virtual ~CAssaultRifle(void);

	// Initialise this instance to default values
	bool Init(void);
};

