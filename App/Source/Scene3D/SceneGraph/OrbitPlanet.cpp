/**
 COrbitPlanet
 By: Jordan Low
 Date: Jan 2022
 */
#include "OrbitPlanet.h"

// Include CTerrain
#include "../Terrain/Terrain.h"
#include "../Entities/SolidObjectManager.h"
#include <iostream>
using namespace std;

/**
 @brief Default Constructor
 */
COrbitPlanet::COrbitPlanet(void)
{
}

/**
 @brief Destructor
 */
COrbitPlanet::~COrbitPlanet(void)
{
}

/**
 @brief Create the Spin Tower using this static method
 @return true is successfully created the spin tower, else false
 */
bool COrbitPlanet::Create(void)
{
	// Initialise a CSceneNode
	CSceneNode* cSceneNode = new CSceneNode(0);
	cSceneNode->nodeType = CSceneNode::N_TOTAL;
	cSceneNode->SetShader("Shader3D");
	float fCheckHeight = CTerrain::GetInstance()->GetHeight(0.f, 0.f);
	cSceneNode->SetWorldTranslateMtx(glm::vec3(0.f, fCheckHeight + 50.f, 0.f));
	cSceneNode->Init();
	cSceneNode->bContinuousUpdate = true;
	cSceneNode->SetUpdateRotateMtx(glm::radians(0.1f), glm::vec3(0.0f, 1.0f, 0.0f));

	// Initialise a child CSceneNode
	CSceneNode* cSceneNodeChild = new CSceneNode(1);
	cSceneNodeChild->nodeType = CSceneNode::N_ORBIT;
	cSceneNodeChild->SetShader("Shader3D");
	cSceneNodeChild->Init();
	cSceneNodeChild->SetWorldTranslateMtx(glm::vec3(200, 15, 200));
	cSceneNodeChild->bContinuousUpdate = true;
	cSceneNodeChild->SetUpdateRotateMtx(glm::radians(0.25f), glm::vec3(0.0f, 1.0f, 0.0f));
	cSceneNodeChild->SetLocalTranslateMtx(glm::vec3(400, 0, 400));
	cSceneNode->AddChild(cSceneNodeChild);

	// Initialise a grandchild CSceneNode
	CSceneNode* cSceneNodeGrandChild = new CSceneNode(2);
	cSceneNodeGrandChild->nodeType = CSceneNode::N_ORBIT;
	cSceneNodeGrandChild->SetShader("Shader3D");
	cSceneNodeGrandChild->Init();
	cSceneNodeGrandChild->SetWorldTranslateMtx(glm::vec3(150, 20, 150));
	cSceneNodeGrandChild->bContinuousUpdate = true;
	cSceneNodeGrandChild->SetUpdateRotateMtx(glm::radians(0.1f), glm::vec3(0.0f, 1.0f, 0.0f));
	cSceneNodeGrandChild->SetLocalTranslateMtx(glm::vec3(300, 10, 300));
	cSceneNode->AddChild(cSceneNodeGrandChild);
	CSolidObjectManager::GetInstance()->Add((CSolidObject*)cSceneNode);

	return true;
}

/**
 @brief PrintSelf
 */
void COrbitPlanet::PrintSelf(void)
{
	cout << "COrbitPlanet::PrintSelf()" << endl;
}