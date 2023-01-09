#pragma once
#include <glm/glm.hpp>
#include "Primech/Renderer/camera/OrthographicCamera.h"
#include "Primech/Core/Timestep.h"

#include "Primech/Events/ApplicationEvent.h"
#include "Primech/Events/MouseEvent.h"

namespace PriMech {
	struct OrthoGraphicCameraBounds {
		float left, right;
		float bottom, top;

		float GetWidth() { return right - left; }
		float GetHeight() { return top - bottom; }
	};

	class OrthographicCameraController {
	public:
		OrthographicCameraController(float aspectRatio, bool rotationEnabled = false);
		~OrthographicCameraController();

		inline OrthographicCamera& GetCamera() { return camera_; }
		inline const OrthographicCamera& GetCamera() const { return camera_; }

		inline float GetZoomLevel() { return zoomLevel_; }
		inline void SetZoomLevel(float zoomLevel) { zoomLevel_ = zoomLevel; }
		inline const OrthoGraphicCameraBounds& GetBounds() const { return bounds_; }

		void OnUpdate(Timestep timestep);
		void OnEvent(Event& event);
	private:
		bool OnMouseScrolled(MouseScrolledEvent& event);
		bool OnWindowResize(WindowResizeEvent& event);

		float aspectRatio_;;
		float zoomLevel_ = 1.0f;
		OrthoGraphicCameraBounds bounds_;
		OrthographicCamera camera_;

		bool rotationEnabled_;

		glm::vec3 cameraPosition_ = glm::vec3(0.0f);
		float cameraRotation_ = 0.0f; //In degrees, in the anti-clockwise direction

		float cameraPositionChangeSpeed_ = 3.0f, cameraRotationSpeed_ = 200.0f;
	};
}