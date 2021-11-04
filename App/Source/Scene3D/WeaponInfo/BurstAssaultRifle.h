/**
 CBurstAssaultRifle
 By: Jordan Low
 Date: Nov 2021
 */
#pragma once

// Include CWeaponInfo
#include "WeaponInfo.h"

class CBurstAssaultRifle : public CWeaponInfo
{
public:
	// Default Constructor
	CBurstAssaultRifle(void);
	// Destructor
	virtual ~CBurstAssaultRifle(void);

	// Initialise this instance to default values
	bool Init(void);
};

