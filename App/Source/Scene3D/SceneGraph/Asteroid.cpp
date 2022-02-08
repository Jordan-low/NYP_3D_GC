/**
 CAsteroid
 By: Jordan Low
 Date: Jan 2022
 */
#include "Asteroid.h"

// Include CTerrain
#include "../Terrain/Terrain.h"
#include "../Entities/SolidObjectManager.h"
#include "../../MyMath.h"

#include <iostream>
using namespace std;

/**
 @brief Default Constructor
 */
CAsteroid::CAsteroid(void)
{
}

/**
 @brief Destructor
 */
CAsteroid::~CAsteroid(void)
{
}

/**
 @brief Create the Spin Tower using this static method
 @return true is successfully created the spin tower, else false
 */
bool CAsteroid::Create(void)
{
	// Initialise a CSceneNode
	CSceneNode* cSceneNode = new CSceneNode(6);
	cSceneNode->nodeType = CSceneNode::N_TOTAL;
	cSceneNode->SetShader("Shader3D");
	float fCheckHeight = CTerrain::GetInstance()->GetHeight(0.f, 0.f);
	cSceneNode->SetWorldTranslateMtx(glm::vec3(0.f, fCheckHeight + 50.f, 0.f));
	cSceneNode->Init();
	cSceneNode->bContinuousUpdate = true;
	cSceneNode->SetUpdateRotateMtx(glm::radians(0.1f), glm::vec3(0.0f, 1.0f, 0.0f));

	for (int i = 3; i < 8; i++)
	{
		// Initialise a child CSceneNode
		CSceneNode* cSceneNodeChild = new CSceneNode(7);
		cSceneNodeChild->nodeType = CSceneNode::N_ASTEROID;
		cSceneNodeChild->SetShader("Shader3D");
		cSceneNodeChild->Init();
		cSceneNodeChild->bContinuousUpdate = true;
		cSceneNodeChild->SetWorldTranslateMtx(glm::vec3(Math::RandFloatMinMax(-20, 20) * i, 0.f, Math::RandFloatMinMax(-20, 20) * i));
		cSceneNodeChild->SetUpdateRotateMtx(glm::radians(0.25f * (i * Math::RandFloatMinMax(0.1, 0.25))), glm::vec3(0.0f, 0.0f, 1.0f));
		cSceneNodeChild->SetLocalTranslateMtx(glm::vec3(Math::RandFloatMinMax(-10, 10) * i, 0, Math::RandFloatMinMax(-10, 10) * i));
		cSceneNode->AddChild(cSceneNodeChild);

		for (int j = 1; j < 3; j++)
		{
			// Initialise a child CSceneNode
			CSceneNode* cSceneNodeChild2 = new CSceneNode(8);
			cSceneNodeChild2->nodeType = CSceneNode::N_ASTEROID;
			cSceneNodeChild2->SetShader("Shader3D");
			cSceneNodeChild2->Init();
			cSceneNodeChild2->bContinuousUpdate = true;
			cSceneNodeChild2->SetUpdateRotateMtx(glm::radians(0.25f * (j * i * Math::RandFloatMinMax(0.1, 0.3))), glm::vec3(1.0f, 0.0f, 0.0f));
			cSceneNodeChild2->SetLocalTranslateMtx(glm::vec3(Math::RandFloatMinMax(-5, 5) * i * j, 0, Math::RandFloatMinMax(-10, 10) * i * j));
			cSceneNodeChild->AddChild(cSceneNodeChild2);
		}

	}
	CSolidObjectManager::GetInstance()->Add((CSolidObject*)cSceneNode);
	return true;
}

/**
 @brief PrintSelf
 */
void CAsteroid::PrintSelf(void)
{
	cout << "CAsteroid::PrintSelf()" << endl;
}