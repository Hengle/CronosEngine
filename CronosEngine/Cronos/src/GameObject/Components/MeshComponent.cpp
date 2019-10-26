#include "Providers/cnpch.h"
#include "MeshComponent.h"

#include "Application.h"

namespace Cronos {

	MeshComponent::MeshComponent(std::vector<CronosVertex> vertices, std::vector<uint> indices, std::vector<Texture*>& textures) :
		Component(ComponentType::MESH , nullptr)
	{
		m_VertexVector = vertices;
		m_IndicesVector = indices;
		m_TexturesVector = textures;

		SetupMesh();
	}

	MeshComponent::~MeshComponent()
	{
		for (uint i = 0; i < m_TexturesVector.size(); i++)
		{
			RELEASE(m_TexturesVector[i]);
			m_TexturesVector.erase(m_TexturesVector.begin() + i);
		}
		for (uint i = 0; i < m_VertexVector.size(); i++)
			m_VertexVector.erase(m_VertexVector.begin() + i);
		for (uint i = 0; i < m_IndicesVector.size(); i++)
			m_IndicesVector.erase(m_IndicesVector.begin() + i);

		m_TexturesVector.clear();
		m_IndicesVector.clear();
		m_VertexVector.clear();

		RELEASE(m_MeshVAO);
		RELEASE(m_MeshIBO);
		RELEASE(m_MeshVBO);
	}

	void MeshComponent::SetupMesh()
	{
		m_MeshVAO = new VertexArray();
		m_MeshVBO = new VertexBuffer(&m_VertexVector[0], m_VertexVector.size() * sizeof(CronosVertex));

		m_MeshVBO->SetLayout({
			{Cronos::VertexDataType::VEC3F, "a_Position"},
			{Cronos::VertexDataType::VEC3F, "a_Normal"},
			{Cronos::VertexDataType::VEC2F, "a_TexCoords"}
			});
		m_MeshVAO->AddVertexBuffer(*m_MeshVBO);

		m_MeshIBO = new IndexBuffer(&m_IndicesVector[0], m_IndicesVector.size());
		m_MeshVAO->AddIndexBuffer(*m_MeshIBO);
	}

	void MeshComponent::SetTextures(std::vector<Texture*>& newTexture, TextureType textureType)
	{
		std::vector<Texture*>::iterator item = m_TexturesVector.begin();
		for (; item != m_TexturesVector.end() && (*item) != NULL; item++)
		{
			if ((*item)->GetTextureType() == textureType)
			{
				(*item)->~Texture();
				delete (*item);
				m_TexturesVector.erase(item);
			}
		}

		m_TexturesVector.insert(m_TexturesVector.end(), newTexture.begin(), newTexture.end());
	}

	void MeshComponent::Draw(Shader* shader, bool bindShader)
	{
		if (!isEnabled())
			return;

		m_MeshVAO->Bind();
		if (bindShader)
		{
			shader->Bind();
			shader->SetUniformMat4f("u_Proj", App->engineCamera->GetProjectionMatrixMAT4());
			shader->SetUniformMat4f("u_View", App->engineCamera->GetViewMatrixMAT4());
			shader->SetUniformMat4f("u_Model", glm::mat4(1.0f)/*GetTransformation()*/);
		}

		std::vector< Texture*>::iterator item = m_TexturesVector.begin();
		for (; item != m_TexturesVector.end() && (*item) != NULL; item++)
		{
			(*item)->Bind((*item)->GetTextureID());

			if ((*item)->GetTextureType() == TextureType::AMBIENT)
				shader->SetUniform1i("u_AmbientTexture", (*item)->GetTextureID());
			if ((*item)->GetTextureType() == TextureType::DIFFUSE)
				shader->SetUniform1i("u_DiffuseTexture", (*item)->GetTextureID());
			if ((*item)->GetTextureType() == TextureType::SPECULAR)
				shader->SetUniform1i("u_SpecularTexture", (*item)->GetTextureID());
			if ((*item)->GetTextureType() == TextureType::NORMALMAP)
				shader->SetUniform1i("u_NormalMap", (*item)->GetTextureID());
			if ((*item)->GetTextureType() == TextureType::HEIGHTMAP)
				shader->SetUniform1i("u_HeightMap", (*item)->GetTextureID());
			if ((*item)->GetTextureType() == TextureType::LIGHTMAP)
				shader->SetUniform1i("u_LightMap", (*item)->GetTextureID());
			//else
			//	App->scene->BasicTestShader->SetUniform1i("u_Texture", (*item)->GetTextureID());
		}


		glDrawElements(GL_TRIANGLES, m_MeshVAO->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, 0);
		for (; item != m_TexturesVector.end() && (*item) != NULL; item++)
			(*item)->Unbind();

		if (m_DebugDraw)
		{
			DrawVerticesNormals();
			DrawPlanesNormals();
		}

		shader->Unbind();
	}

	void MeshComponent::DrawVerticesNormals()
	{
		float linelength = 0.03f;
		glLineWidth(2.0f);
		glColor4f(0.1f, 0.5f, 0.8f, 1.0f);

		std::vector<CronosVertex>::iterator item = m_VertexVector.begin();
		for (; item != m_VertexVector.end(); item++)
		{
			glm::vec3 pos = (*item).Position;
			glm::vec3 norm = (*item).Position + (*item).Normal;

			glBegin(GL_LINES);
				glVertex3f(pos.x, pos.y, pos.z);
				glVertex3f(norm.x + linelength, norm.y, norm.z);
				glVertex3f(pos.x, pos.y, pos.z);
				glVertex3f(norm.x, norm.y + linelength, norm.z);
				glVertex3f(pos.x, pos.y, pos.z);
				glVertex3f(norm.x, norm.y, norm.z + linelength);
			glEnd();
		}
	}

	void MeshComponent::DrawPlanesNormals()
	{
		float linelength = 0.2f;
		glColor4f(0.0f, 1.0f, 1.0f, 1.0f);

		static std::vector<glm::vec3> Normals;
		static  std::vector<glm::vec3> Positions;

		if (!m_NormalsCalculated)
		{
			CalculateNormals(Normals, Positions);
			m_NormalsCalculated = true;
		}

		for (uint i = 0; i < Normals.size(); i++)
		{
			glBegin(GL_LINES);
				glVertex3f(Positions[i].x, Positions[i].y, Positions[i].z);
				glVertex3f(Normals[i].x + linelength, Normals[i].y, Normals[i].z);
				glVertex3f(Positions[i].x, Positions[i].y, Positions[i].z);
				glVertex3f(Normals[i].x, Normals[i].y + linelength, Normals[i].z);
				glVertex3f(Positions[i].x, Positions[i].y, Positions[i].z);
				glVertex3f(Normals[i].x, Normals[i].y, Normals[i].z + linelength);
			glEnd();
		}

		//for (uint i = 0; i < m_IndicesVector.size(); i += 3)
		//{
		//	glm::vec3 p1 = m_VertexVector[m_IndicesVector[i]].Position;
		//	glm::vec3 p2 = m_VertexVector[m_IndicesVector[i + 1]].Position;
		//	glm::vec3 p3 = m_VertexVector[m_IndicesVector[i + 2]].Position;

		//	glm::vec3 PlaneNormal = glm::cross(p2 - p1, p3 - p1);
		//	glm::normalize(PlaneNormal);
		//	//PlaneNormal *= linelength;

		//	glm::vec3 TriCenter = { 0, 0, 0 };
		//	TriCenter.x = (p1.x + p2.x + p3.x) / 3;
		//	TriCenter.y = (p1.y + p2.y + p3.y) / 3;
		//	TriCenter.z = (p1.z + p2.z + p3.z) / 3;

		//	glBegin(GL_LINES);
		//	//glVertex3f(TriCenter.x, TriCenter.y, TriCenter.z);
		//	//glVertex3f(TriCenter.x + PlaneNormal.x, TriCenter.y + PlaneNormal.y, TriCenter.z + PlaneNormal.z);

		//		glVertex3f(TriCenter.x, TriCenter.y, TriCenter.z);
		//		glVertex3f(PlaneNormal.x + linelength, PlaneNormal.y, PlaneNormal.z);
		//		glVertex3f(TriCenter.x, TriCenter.y, TriCenter.z);
		//		glVertex3f(PlaneNormal.x, PlaneNormal.y + linelength, PlaneNormal.z);
		//		glVertex3f(TriCenter.x, TriCenter.y, TriCenter.z);
		//		glVertex3f(PlaneNormal.x, PlaneNormal.y, PlaneNormal.z + linelength);
		//	glEnd();
		//}
	}

	void MeshComponent::CalculateNormals(std::vector<glm::vec3>& normals, std::vector<glm::vec3>& positions)
	{
		//Clear Vectors passed
		if (normals.size() > 0 || positions.size() > 0)
		{
			std::vector<glm::vec3>::iterator item = normals.begin();
			for (; item != normals.end(); item++)
				normals.erase(item);
			item = positions.begin();
			for (; item != positions.end(); item++)
				positions.erase(item);

			positions.clear();
			normals.clear();
		}

		//Calculate normals and add them to the vectors passed
		for (uint i = 0; i < m_IndicesVector.size(); i += 3)
		{
			glm::vec3 p1 = m_VertexVector[m_IndicesVector[i]].Position;
			glm::vec3 p2 = m_VertexVector[m_IndicesVector[i + 1]].Position;
			glm::vec3 p3 = m_VertexVector[m_IndicesVector[i + 2]].Position;

			glm::vec3 PlaneNormal = glm::cross(p2 - p1, p3 - p1);
			glm::normalize(PlaneNormal);

			glm::vec3 TriCenter = { 0, 0, 0 };
			TriCenter.x = (p1.x + p2.x + p3.x) / 3;
			TriCenter.y = (p1.y + p2.y + p3.y) / 3;
			TriCenter.z = (p1.z + p2.z + p3.z) / 3;

			positions.push_back(TriCenter);
			normals.push_back(PlaneNormal);
		}
	}
}