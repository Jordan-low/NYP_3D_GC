/**
 CJupiterPlanet
 By: Jordan Low
 Date: Jan 2022
 */
#include "JupiterPlanet.h"

// Include CTerrain
#include "../Terrain/Terrain.h"
#include "../Entities/SolidObjectManager.h"
#include <iostream>
using namespace std;

/**
 @brief Default Constructor
 */
CJupiterPlanet::CJupiterPlanet(void)
{
}

/**
 @brief Destructor
 */
CJupiterPlanet::~CJupiterPlanet(void)
{
}

/**
 @brief Create the Spin Tower using this static method
 @return true is successfully created the spin tower, else false
 */
bool CJupiterPlanet::Create(void)
{
	// Initialise a CSceneNode
	CSceneNode* cSceneNode = new CSceneNode(3);
	cSceneNode->nodeType = CSceneNode::N_PLANET;
	cSceneNode->SetLocalScaleMtx(glm::vec3(5, 5, 5));
	cSceneNode->SetShader("Shader3D");
	float fCheckHeight = CTerrain::GetInstance()->GetHeight(400.f, 400.f);
	cSceneNode->SetWorldTranslateMtx(glm::vec3(400.f, fCheckHeight + 200.f, 400.f));
	cSceneNode->Init();
	cSceneNode->bContinuousUpdate = true;
	cSceneNode->SetUpdateRotateMtx(glm::radians(0.1f), glm::vec3(0.0f, 1.0f, 0.0f));

	// Initialise a child CSceneNode
	CSceneNode* cSceneNodeChild = new CSceneNode(4);
	cSceneNodeChild->nodeType = CSceneNode::N_RING;
	cSceneNodeChild->SetLocalScaleMtx(glm::vec3(1.5, 1.5, 1.5));
	cSceneNodeChild->SetShader("Shader3D");
	cSceneNodeChild->Init();
	cSceneNodeChild->bContinuousUpdate = true;
	cSceneNodeChild->SetUpdateRotateMtx(glm::radians(0.25f), glm::vec3(0.0f, 0.0f, 1.0f));
	cSceneNode->AddChild(cSceneNodeChild);

	// Initialise a grandchild CSceneNode
	CSceneNode* cSceneNodeGrandChild = new CSceneNode(5);
	cSceneNodeGrandChild->nodeType = CSceneNode::N_RING;
	cSceneNodeGrandChild->SetLocalScaleMtx(glm::vec3(2.5, 2, 2.5));
	cSceneNodeGrandChild->SetShader("Shader3D");
	cSceneNodeGrandChild->Init();
	cSceneNodeGrandChild->bContinuousUpdate = true;
	cSceneNodeGrandChild->SetUpdateRotateMtx(glm::radians(0.5f), glm::vec3(1.0f, 0.0f, 0.0f));
	cSceneNode->AddChild(cSceneNodeGrandChild);
	CSolidObjectManager::GetInstance()->Add((CSolidObject*)cSceneNode);

	return true;
}

/**
 @brief PrintSelf
 */
void CJupiterPlanet::PrintSelf(void)
{
	cout << "CJupiterPlanet::PrintSelf()" << endl;
}