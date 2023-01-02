#pragma once

#include "glm/glm.hpp"

namespace PriMech {
	class OrthographicCamera {
	public:
		OrthographicCamera(float left, float right, float bottom, float top);
		~OrthographicCamera() {}

		void SetProjection(float left, float right, float bottom, float top);

		inline const glm::vec3& GetPostion() const { return position_; }
		inline float GetRotation() const { return rotation_; }

		inline void SetPosition(const glm::vec3& pos) { position_ = pos; RecalculateViewMatrix(); }
		inline void SetRotation(float rot) { rotation_ = rot; RecalculateViewMatrix(); }

		inline const glm::mat4 GetViewProjectionMatrix() const { return viewProjectionMatrix_; }
		inline const glm::mat4 GetProjectionMatrix() const { return projectionMatrix_; }
		inline const glm::mat4 GetViewMatrix() const { return viewMatrix_; }
	private:
		void RecalculateViewMatrix();

		glm::mat4 projectionMatrix_ = glm::mat4(1.0f);
		glm::mat4 viewMatrix_ = glm::mat4(1.0f);
		glm::mat4 viewProjectionMatrix_ = glm::mat4(1.0f);

		glm::vec3 position_ = glm::vec3(0.0f);
		float rotation_ = 0.0f;
	};
} 