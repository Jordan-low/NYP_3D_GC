/**
 CStructure3D
 By: Toh Da Jun
 Date: Apr 2020
 */
#include "Structure3D.h"

// Include ShaderManager
#include "RenderControl/ShaderManager.h"

// Include MeshBuilder
#include "Primitives/MeshBuilder.h"

// Include ImageLoader
#include "System\ImageLoader.h"
#include "System/LoadOBJ.h"

#include <iostream>
using namespace std;

/**
 @brief Default Constructor
 */
CStructure3D::CStructure3D(void)
{
	// Set the default position to the origin
	vec3Position = glm::vec3(0.0f, fHeightOffset, 0.0f);
}

/**
 @brief Constructor with vectors
 @param vec3Position A const glm::vec3 variable which contains the position of the camera
 @param vec3Front A const glm::vec3 variable which contains the up direction of the camera
 @param yaw A const float variable which contains the yaw of the camera
 @param pitch A const float variable which contains the pitch of the camera
 */
CStructure3D::CStructure3D(	const glm::vec3 vec3Position,
							const glm::vec3 vec3Front)
{
	this->vec3Position = glm::vec3(vec3Position.x, vec3Position.y + fHeightOffset, vec3Position.z);
	this->vec3Front = vec3Front;
}

/**
 @brief Destructor
 */
CStructure3D::~CStructure3D(void)
{
}

/**
 @brief Initialise this class instance
 @return true is successfully initialised this class instance, else false
 */
bool CStructure3D::Init(CEntity3D::TYPE type)
{
	// Call the parent's Init()
	CSolidObject::Init();

	// Set the type
	SetType(type);

	if (type == CEntity3D::TYPE::AMMO)
	{
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec2> uvs;
		std::vector<glm::vec3> normals;
		std::vector<ModelVertex> vertex_buffer_data;
		std::vector<GLuint> index_buffer_data;

		std::string file_path = "Models/Weapons/AmmoBox.obj";
		bool success = CLoadOBJ::LoadOBJ(file_path.c_str(), vertices, uvs, normals, true);
		if (!success)
		{
			cout << "Unable to load Models/Weapons/gun_type64_01.obj" << endl;
			return false;
		}

		CLoadOBJ::IndexVBO(vertices, uvs, normals, index_buffer_data, vertex_buffer_data);

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &IBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(ModelVertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
		iIndicesSize = index_buffer_data.size();
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (void*)(sizeof(glm::vec3) + sizeof(glm::vec3)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// load and create a texture 
		iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Models/Weapons/AmmoBox.jpg", false);
		if (iTextureID == 0)
		{
			cout << "Unable to load Models/Weapons/map_gunType64_01_AO.png" << endl;
			return false;
		}
	}
	else
	{
		// Generate and bind the VAO
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		mesh = CMeshBuilder::GenerateBox(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

		// load and create a texture 
		iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Image/Scene3D_Structure_01.tga", false);
		if (iTextureID == 0)
		{
			cout << "Unable to load Image/Scene3D_Structure_01.tga" << endl;
			return false;
		}
	}


	return true;
}


/**
 @brief Set model
 @param model A const glm::mat4 variable containing the model for this class instance
 */
void CStructure3D::SetModel(const glm::mat4 model)
{
	this->model = model;
}

/**
 @brief Set view
 @param view A const glm::mat4 variable containing the model for this class instance
 */
void CStructure3D::SetView(const glm::mat4 view)
{
	this->view = view;
}

/**
 @brief Set projection
 @param projection A const glm::mat4 variable containing the model for this class instance
 */
void CStructure3D::SetProjection(const glm::mat4 projection)
{
	this->projection = projection;
}

/**
 @brief Update the elapsed time
 @param dt A const double variable containing the elapsed time since the last frame
 @return A bool variable
 */
bool CStructure3D::Update(const double dElapsedTime)
{
	if (!bStatus)
		return false;
	CSolidObject::Update(dElapsedTime);

	return true;
}

/**
@brief PreRender Set up the OpenGL display environment before rendering
*/
void CStructure3D::PreRender(void)
{
	if (!bStatus)
		return;
	CSolidObject::PreRender();
}

/**
@brief Render Render this instance
@param cShader A Shader* variable which contains the Shader to use in this class instance
*/
void CStructure3D::Render(void)
{
	if (!bStatus)
		return;
	CSolidObject::Render();
}

/**
@brief PostRender Set up the OpenGL display environment after rendering.
*/
void CStructure3D::PostRender(void)
{
	if (!bStatus)
		return;
	CSolidObject::PostRender();
}
