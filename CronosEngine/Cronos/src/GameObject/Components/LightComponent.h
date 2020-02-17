#ifndef _LIGHTCOMPONENT_H_
#define _LIGHTCOMPONENT_H_

#include "Component.h"
#include "Renderer/Shaders.h"
#include "MaterialComponent.h"

namespace Cronos
{
	enum class LightType { NONE = -1, DIRECTIONAL, POINTLIGHT, SPOTLIGHT };

	struct DirectionalLight
	{
		glm::vec3 m_LightDirection = glm::vec3(0.0f);
		glm::vec3 m_LightColor = glm::vec3(0.0f);
		float m_LightIntensity = 1.0f;
	};

	struct PointLight
	{
		glm::vec3 m_LightPosition = glm::vec3(0.0f);
		glm::vec3 m_LightColor = glm::vec3(0.0f);

		float m_LightIntensity = 1.0f;
		float LightAttenuationFactor_K = 1.0f;
		float LightAttenuationFactor_L = 0.0f;
		float LightAttenuationFactor_Q = 0.0f;
	};

	struct SpotLight
	{
		glm::vec3 m_LightPosition = glm::vec3(0.0f);
		glm::vec3 m_LightDirection = glm::vec3(0.0f);
		glm::vec3 m_LightColor = glm::vec3(0.0f);

		float m_LightIntensity = 1.0f;
		float LightAttenuationFactor_K = 1.0f;
		float LightAttenuationFactor_L = 0.0f;
		float LightAttenuationFactor_Q = 0.0f;

		float m_InnerCutoffAngle = 12.5f; //Cosinus value
		float m_OuterCutoffAngle = 45.0f; //Cosinus value
	};


	class LightComponent : public Component
	{
	public:

		LightComponent(GameObject* attachedGO);
		~LightComponent();
		
		static ComponentType GetType() { return ComponentType::LIGHT; }
		
	public:

		//Setters
		void SetLightType(LightType type);
		void SetLightDirection(const glm::vec3& direction) { m_LightDirection = direction; }
		void SetAttenuationFactors(const glm::vec3& attenuationFactorsKLQ) { m_LightAttK = attenuationFactorsKLQ.x; m_LightAttL = attenuationFactorsKLQ.y, m_LightAttQ = attenuationFactorsKLQ.z; }
		void SetLightColor(const glm::vec3& color);
		void SetLightIntensity(const float& intensity);
		void SetSpotlightInnerCutoff(const float& degreesAngle) { m_LightCutoffAngle = degreesAngle; }
		void SetSpotlightOuterCutoff(const float& degreesAngle) { m_LightOuterCutoffAngle = degreesAngle; }

		//Getters
		inline const LightType GetLightType()				const { return m_LightType; }
		inline const glm::vec3 GetLightColor()				const { return m_LightColor; }
		inline const float GetLightIntensity()				const { return m_LightIntensity; }
		inline const glm::vec3 GetLightDirection()			const { return m_LightDirection; }
		inline const glm::vec3 GetLightAttenuationFactors()	const { return { m_LightAttK, m_LightAttL, m_LightAttQ }; }
		inline const float GetSpotlightInnerCutoff()		const { return m_LightCutoffAngle; }
		inline const float GetSpotlightOuterCutoff()		const { return m_LightOuterCutoffAngle; }

	public:

		void SendUniformsLightData(Shader* shader, uint lightIndex);

	private:

		void SetLightToZero(Shader* shader, uint lightIndex, LightType lType);

	private:

		LightType m_LightType = LightType::NONE;

		Material* m_LightMaterial = nullptr;

		glm::vec3 m_LightColor = glm::vec3(1.0f);
		glm::vec3 m_LightDirection = glm::vec3(0.0f);
		float m_LightIntensity = 0.5f;
		float m_LightAttK = 1.0f, m_LightAttL = 0.09f, m_LightAttQ = 0.032f;
		float m_LightCutoffAngle = 12.5f, m_LightOuterCutoffAngle = 17.5f;

		bool m_ChangeLightType = false;
	};
}

#endif