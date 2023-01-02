#pragma once
#include <glm/glm.hpp>
#include "Primech/Renderer/OrthographicCamera.h"
#include "Primech/Core/Timestep.h"

#include "Primech/Events/ApplicationEvent.h"
#include "Primech/Events/MouseEvent.h"

namespace PriMech {
	class OrthographicCameraController {
	public:
		OrthographicCameraController(float aspectRatio, bool rotationEnabled = false); // aspectratio * 2 units / 2 units
		~OrthographicCameraController();

		inline const OrthographicCamera& GetCamera() const { return camera_; }
		inline OrthographicCamera& GetCamera() { return camera_; }

		void OnUpdate(Timestep timestep);
		void OnEvent(Event& event);
	private:
		bool OnMouseScrolled(MouseScrolledEvent& event);
		bool OnWindowResize(WindowResizeEvent& event);

		float aspectRatio_;;
		float zoomLevel_ = 1.0f;
		OrthographicCamera camera_;

		bool rotationEnabled_;

		glm::vec3 cameraPosition_ = glm::vec3(0.0f);
		float cameraRotation_ = 0.0f;

		float cameraPositionChangeSpeed_ = 3.0f, cameraRotationSpeed_ = 200.0f;
	};
}