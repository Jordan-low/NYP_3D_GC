/**
 CPistol
 By: Toh Da Jun
 Date: Sep 2020
 */
#pragma once

// Include CWeaponInfo
#include "WeaponInfo.h"

class CKnife : public CWeaponInfo
{
public:
	// Default Constructor
	CKnife(void);
	// Destructor
	virtual ~CKnife(void);

	// Initialise this instance to default values
	bool Init(void);
};

