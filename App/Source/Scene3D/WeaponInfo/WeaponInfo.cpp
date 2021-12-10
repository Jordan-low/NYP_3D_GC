/**
 CWeaponInfo
 By: Toh Da Jun
 Date: Sep 2020
 */
#include "WeaponInfo.h"

// Include CShaderManager
#include "RenderControl/ShaderManager.h"

// Include CProjectile
#include "Projectile.h"

// Include CProjectileManager
#include "ProjectileManager.h"
#include "../Camera.h"

#include <iostream>
#include "../App/Source/MyMath.h"
using namespace std;

/**
@brief Default Constructor
*/
CWeaponInfo::CWeaponInfo()
	: iMagRounds(1)
	, iMaxMagRounds(1)
	, iTotalRounds(8)
	, iMaxTotalRounds(8)
	, iBulletsPerClick(1)
	, dTimeBetweenShots(0.5)
	, dElapsedTime(0.0)
	, dReloadTime(0.0f)
	, dEquipTime(0.0f)
	, dMaxEquipTime(0.0f)
	, dMaxReloadTime(5.0f)
	, animateMeleeAttackZ(0.f)
	, animateMeleeAttackDir(-1)
	, animateReloadAngle(0.f)
	, animateReloadPosY(0.f)
	, animateReloadPosZ(0.f)
	, animateEquipAngle(90.f)
	, animateADSPosX(0.f)
	, bFire(true)
	, isMeleeAttacking(false)
	, bAuto(true)
	, bulletSpread(0.0f)
	, minRecoil(glm::vec2(0.f))
	, maxRecoil(glm::vec2(0.f))
	, gunRecoilPos(glm::vec3(0.f))
	, crossHairType(CCrossHair::CROSSHAIR_DOT)
{
}

/**
 @brief Default Destructor
 */
CWeaponInfo::~CWeaponInfo()
{
}

string CWeaponInfo::GetName(void) const
{
	return name;
}

/**
 @brief Set the number of ammunition in the magazine for this player
 @param iMagRounds A const int variable containing the new number of ammunition
 */
void CWeaponInfo::SetMagRound(const int iMagRounds)
{
	this->iMagRounds = iMagRounds;
}

/**
 @brief Set the maximum number of ammunition in the magazine for this weapon
 @param iMagRounds A const int variable containing the new maximum number of ammunition
 */
void CWeaponInfo::SetMaxMagRound(const int iMaxMagRounds)
{
	this->iMaxMagRounds = iMaxMagRounds;
}

/**
 @brief The current total number of rounds currently carried by this player
 @param iMagRounds A const int variable containing the new total number of ammunition
 */
void CWeaponInfo::SetTotalRound(const int iTotalRounds)
{
	this->iTotalRounds = iTotalRounds;
}

/**
 @brief The max total number of rounds currently carried by this player
 @param iMagRounds A const int variable containing the new maximum total number of ammunition
 */
void CWeaponInfo::SetMaxTotalRound(const int iMaxTotalRounds)
{
	this->iMaxTotalRounds = iMaxTotalRounds;
}

void CWeaponInfo::SetBulletsPerClick(const int iBulletsPerClick)
{
	this->iBulletsPerClick = iBulletsPerClick;
}


/**
 @brief Get the number of ammunition in the magazine for this player
 @return A int variable
 */
int CWeaponInfo::GetMagRound(void) const
{
	return iMagRounds;
}

/**
 @brief Get the maximum number of ammunition in the magazine for this weapon
 @return A int variable
 */
int CWeaponInfo::GetMaxMagRound(void) const
{
	return iMaxMagRounds;
}

/**
 @brief Get the current total number of rounds currently carried by this player
 @return A int variable
 */
int CWeaponInfo::GetTotalRound(void) const
{
	return iTotalRounds;
}

/**
 @brief Get the max total number of rounds currently carried by this player
 @return A int variable
 */
int CWeaponInfo::GetMaxTotalRound(void) const
{
	return iMaxTotalRounds;
}

int CWeaponInfo::GetBulletsPerClick(void) const
{
	return iBulletsPerClick;
}

/**
 @brief Set the time between shots
 @param dTimeBetweenShots A const double variable containing the time between shots
 */
void CWeaponInfo::SetTimeBetweenShots(const double dTimeBetweenShots)
{
	this->dTimeBetweenShots = dTimeBetweenShots;
}

/**
 @brief Set the firing rate in rounds per min
 @param firingRate A const int variable containing the firing rate for this weapon
 */
void CWeaponInfo::SetFiringRate(const int firingRate)
{
	dTimeBetweenShots = 60.0 / (double)firingRate;	// 60 seconds divided by firing rate
}

/**
 @brief Set the firing flag
 @param bFire A const bool variable describing if this weapon can fire
 */
void CWeaponInfo::SetCanFire(const bool bFire)
{
	this->bFire = bFire;
}

void CWeaponInfo::SetGunRecoilPos(glm::vec3 _gunRecoilPos)
{
	gunRecoilPos = _gunRecoilPos;
}

/**
 @brief Get the time between shots
 @return A double variable
 */
double CWeaponInfo::GetTimeBetweenShots(void) const
{
	return dTimeBetweenShots;
}

/**
 @brief Get the firing rate
 @return An int variable
 */
int CWeaponInfo::GetFiringRate(void) const
{
	return (int)(60.0 / dTimeBetweenShots);	// 60 seconds divided by dTimeBetweenShots
}

/**
 @brief Get the firing flag
 @return A bool variable
 */
bool CWeaponInfo::GetCanFire(void) const
{
	return bFire;
}

/**
 @brief Get the auto flag
 @return A bool variable
 */
bool CWeaponInfo::GetAutoFire(void) const
{
	return bAuto;
}

glm::vec2 CWeaponInfo::GetMinRecoil(void) const
{
	return minRecoil;
}

glm::vec2 CWeaponInfo::GetMaxRecoil(void) const
{
	return maxRecoil;
}

glm::vec3 CWeaponInfo::GetGunRecoilPos(void) const
{
	return gunRecoilPos;
}

float CWeaponInfo::GetBulletSpread(void) const
{
	return bulletSpread;
}

/**
 @brief Initialise this instance to default values
 @return A bool variable
 */
bool CWeaponInfo::Init(void)
{
	// The number of ammunition in a magazine for this weapon
	iMagRounds = 1;
	// The maximum number of ammunition for this magazine for this weapon
	iMaxMagRounds = 1;
	// The current total number of rounds currently carried by this player
	iTotalRounds = 8;
	// The max total number of rounds currently carried by this player
	iMaxTotalRounds = 8;

	// The time between shots
	dTimeBetweenShots = 0.5f;
	// The elapsed time (between shots)
	dElapsedTime = dTimeBetweenShots;
	// The elapsed time for reloading of a magazine in milliseconds
	dReloadTime = -1.0f;
	// The maximum elapsed time for reloading of a magazine in milliseconds
	dMaxReloadTime = 5.0f;
	// Boolean flag to indicate if weapon can fire now
	bFire = true;

	cSettings = CSettings::GetInstance();
	ADSzoom = cSettings->FOV;

	// Update the model matrix
	initialZPos = vec3Position.z;
	initialXPos = vec3Position.x;
	vec3Position.z += 0.3f;
	finalZPos = vec3Position.z;
	model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	model = glm::translate(model, glm::vec3(vec3Position.x, vec3Position.y, vec3Position.z));
	model = glm::rotate(model, fRotationAngle, vec3RotationAxis);
	model = glm::scale(model, vec3Scale);
	//fRotationAngle += 0.01f;

	return true;
}

/**
 @brief Update the elapsed time
 @param dt A const double variable containing the elapsed time since the last frame
 @return A bool variable
 */
bool CWeaponInfo::Update(const double dt)
{
	//apply ads
	ADS(dt, isADS);

	//lerp gun recoil pos back to normal
	gunRecoilPos.y = Math::Lerp(gunRecoilPos.y, 0.f, (float)dt);
	gunRecoilPos.z = Math::Lerp(gunRecoilPos.z, 0.f, (float)dt);

	// Update the model matrix
	model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	model = glm::translate(model, glm::vec3(vec3Position.x, vec3Position.y, vec3Position.z) + gunRecoilPos + glm::vec3(animateADSPosX, animateReloadPosY, animateReloadPosZ + animateMeleeAttackZ));
	model = glm::scale(model, vec3Scale);
	model = glm::rotate(model, fRotationAngle, vec3RotationAxis);
	model = glm::rotate(model, glm::radians(animateReloadAngle), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(animateEquipAngle), glm::vec3(1, 0, 0));

	if (isMeleeAttacking)
		AnimateMeleeAttack(dt);

	// If the weapon can fire, then just fire and return
	if (bFire)
		return false;


	// Update the dReloadTime
	if (dReloadTime >= 0.0f)
	{
		AnimateReload(dt);
		// Reduce the dReloadTime
		dReloadTime -= dt;
		// Return true since we have already updated the dReloadTime
		return true;
	}
	// Update the elapsed time if there is no reload to countdown
	else if (dElapsedTime > 0.0f)
	{
		dElapsedTime -= dt;
		// Return true since we have already updated the dReloadTime
		return true;
	}
	else if (dEquipTime > 0.0f)
	{
		AnimateEquip(dt);
		dEquipTime -= dt;
		return true;
	}
	else
	{
		// Set the weapon to fire ready since reloading is completed
		bFire = true;
	}

	return false;
}

/**
 @brief Discharge this weapon
 @param vec3Position A const glm::vec3 variable containing the source position of the projectile
 @param vec3Front A const glm::vec3 variable containing the direction of the projectile
 @param pSource A const CSolidObject* variable containing the source of this projectile
 @return A bool variable
 */
bool CWeaponInfo::Discharge(glm::vec3 vec3Position, 
							glm::vec3 vec3Front,
							CSolidObject* pSource)
{
	if (bFire)
	{
		bool firstBulletFired = false;
		for (int i = 0; i < iBulletsPerClick; i++)
		{
			// If there is still ammo in the magazine, then fire
			if (iMagRounds > 0)
			{
				if (i == 0)
					firstBulletFired = true;
				// Get Bullet Spread Amount
				float randomSpreadX = Math::RandFloatMinMax(-bulletSpread, bulletSpread);
				float randomSpreadY = Math::RandFloatMinMax(-bulletSpread, bulletSpread);

				// Create a projectile. 
				// Its position is slightly in front of the player to prevent collision
				// Its direction is same as the player.
				// It will last for 2.0 seconds and travel at 20 units per frame
				CProjectileManager::GetInstance()->Activate(vec3Position + vec3Front * 0.75f,
					vec3Front + glm::vec3(randomSpreadX, randomSpreadY, 0),
					2.0f, 20.0f, pSource);

				// Lock the weapon after this discharge
				bFire = false;
				// Reset the dElapsedTime to dTimeBetweenShots for the next shot
				dElapsedTime = dTimeBetweenShots;
				// Reduce the rounds by 1
				iMagRounds--;
			}
		}
		if (firstBulletFired)
			return true;
	}

	//cout << "Unable to discharge weapon." << endl;
	return false;
}

/**
 @brief Reload this weapon
 */
void CWeaponInfo::Reload(void)
{
	// If the weapon is already reloading, then don't reload again
	if (dReloadTime > 0.0f)
		return;

	// Check if there is enough bullets
	if (iMagRounds < iMaxMagRounds)
	{
		if (iMaxMagRounds - iMagRounds <= iTotalRounds)
		{
			iTotalRounds -= iMaxMagRounds - iMagRounds;
			iMagRounds = iMaxMagRounds;
		}
		else
		{
			iMagRounds += iTotalRounds;
			iTotalRounds = 0;
		}
		// Set the elapsed time for reloading of a magazine to dMaxReloadTime
		dReloadTime = dMaxReloadTime;
		// Disable the weapon's ability to discharge
		bFire = false;
		// No need to have countdown for between shots since we are reloading
		dElapsedTime = dTimeBetweenShots;
	}
}

/**
 @brief Animate Reload
 @param dt A const double variable containing the elapsed time since the last frame
 */
void CWeaponInfo::AnimateReload(const double dt)
{
	//animate weapon reloading
	if (dReloadTime > dMaxReloadTime * 0.5f)
	{
		animateReloadPosY = Math::Lerp(animateReloadPosY, -0.2f, (float)dt * 5);
		animateReloadPosZ = Math::Lerp(animateReloadPosZ, 0.2f, (float)dt * 10);
	}
	else if (dReloadTime < dMaxReloadTime * 0.5f)
	{
		animateReloadPosY = Math::Lerp(animateReloadPosY, 0.f, (float)dt * 5);
		animateReloadPosZ = Math::Lerp(animateReloadPosZ, 0.f, (float)dt * 10);
	}
}

void CWeaponInfo::AnimateEquip(const double dt)
{
	//Rotate 90 degree upwards during its equip time
	animateEquipAngle -= 90.f / dMaxEquipTime * dt;

	if (animateEquipAngle <= 0.f)
		animateEquipAngle = 0.f;

	//Pull out the weapon slightly from the back to the front
	vec3Position.z -= 0.3f / dMaxEquipTime * dt;

	if (vec3Position.z <= initialZPos)
		vec3Position.z = initialZPos;
}

void CWeaponInfo::SetEquip()
{
	bFire = false;
	dEquipTime = dMaxEquipTime;
}

void CWeaponInfo::SetUnequip()
{
	animateEquipAngle = 90;
	vec3Position.z = finalZPos;
}

void CWeaponInfo::ADS(double dt, bool enable)
{
	if (enable) //if ads, lerp towards the zoom
	{
		animateADSPosX = Math::Lerp(animateADSPosX, -initialXPos, (float)dt * 10);
		CCamera::GetInstance()->fZoom = Math::Lerp(CCamera::GetInstance()->fZoom, ADSzoom, (float)dt * 5);
	}
	else //else lerp back to normal pos
	{
		animateADSPosX = Math::Lerp(animateADSPosX, 0.f, (float)dt * 10);
		CCamera::GetInstance()->fZoom = Math::Lerp(CCamera::GetInstance()->fZoom, cSettings->FOV, (float)dt * 5);
	}
}

/**
/**
 @brief Add rounds
 @param newRounds A const int variable containing the number of new ammunition to add to this weapon
 */
void CWeaponInfo::AddRounds(const int newRounds)
{
	if (iTotalRounds + newRounds > iMaxTotalRounds)
		iTotalRounds = iMaxTotalRounds;
	else
		iTotalRounds += newRounds;
}

/**
 @brief Set model
 @param model A glm::mat4 variable to be assigned to this class instance
 */
void CWeaponInfo::SetModel(glm::mat4 model)
{
	this->model = model;
}

/**
 @brief Set view
 @param view A glm::mat4 variable to be assigned to this class instance
 */
void CWeaponInfo::SetView(glm::mat4 view)
{
	this->view = view;
}

/**
 @brief Set projection
 @param projection A glm::mat4 variable to be assigned to this class instance
 */
void CWeaponInfo::SetProjection(glm::mat4 projection)
{
	this->projection = projection;
}

/**
 @brief Set up the OpenGL display environment before 1ing
 */
void CWeaponInfo::PreRender(void)
{
	// Change depth function so depth test passes when values are equal to depth buffer's content
	glDepthFunc(GL_LEQUAL);

	// Activate normal 3D shader
	CShaderManager::GetInstance()->Use(sShaderName);
}

/**
 @brief Render this instance
 */
void CWeaponInfo::Render(void)
{	
	// note: currently we set the projection matrix each frame, but since the projection 
	// matrix rarely changes it's often best practice to set it outside the main loop only once.
	CShaderManager::GetInstance()->activeShader->setMat4("projection", projection);
	CShaderManager::GetInstance()->activeShader->setMat4("view", view);
	CShaderManager::GetInstance()->activeShader->setMat4("model", model);

	// bind textures on corresponding texture units
	glBindTexture(GL_TEXTURE_2D, iTextureID);
	glActiveTexture(GL_TEXTURE0);
		// Render the mesh
		glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, iIndicesSize, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

/**
 @brief PostRender Set up the OpenGL display environment after rendering.
 */
void CWeaponInfo::PostRender(void)
{
	glDepthFunc(GL_LESS); // set depth function back to default
}

/**
 @brief Print Self
 */
void CWeaponInfo::PrintSelf(void)
{
	cout << "CWeaponInfo::PrintSelf()" << endl;
	cout << "========================" << endl;
	cout << "iMagRounds\t\t:\t" << iMagRounds << endl;
	cout << "iMaxMagRounds\t\t:\t" << iMaxMagRounds << endl;
	cout << "iTotalRounds\t\t:\t" << iTotalRounds << endl;
	cout << "iMaxTotalRounds\t\t:\t" << iMaxTotalRounds << endl;
	cout << "dReloadTime\t\t:\t" << dReloadTime << endl;
	cout << "dEquipTime\t\t:\t" << dEquipTime << endl;
	cout << "dTimeBetweenShots\t:\t" << dTimeBetweenShots << endl;
	cout << "dElapsedTime\t\t:\t" << dElapsedTime << endl;
	cout << "bFire\t\t:\t" << bFire << endl;
}

void CWeaponInfo::AnimateMeleeAttack(const double dElapsedTime)
{
	if (animateMeleeAttackDir == -1 && animateMeleeAttackZ < -0.25f)
		animateMeleeAttackDir = 1;
	else if (animateMeleeAttackDir == 1 && animateMeleeAttackZ >= 0)
	{
		std::cout << "DASD" << std::endl;
		animateMeleeAttackZ = 0;
		isMeleeAttacking = false;
		bFire = true;
		animateMeleeAttackDir = -1;
		return;
	}
	animateMeleeAttackZ += dElapsedTime * animateMeleeAttackDir;
}
