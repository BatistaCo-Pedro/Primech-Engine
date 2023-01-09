#include "ppch.h"
#include "OrthographicCameraController.h"

#include "Primech/Core/Input.h"
#include "Primech/Core/Codes/KeyCodes.h"


namespace PriMech {
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotationEnabled) : 
		aspectRatio_(aspectRatio), rotationEnabled_(rotationEnabled),
		bounds_({ -aspectRatio_ * zoomLevel_, aspectRatio_ * zoomLevel_, -zoomLevel_, zoomLevel_ }), 
		camera_(bounds_.left, bounds_.right, bounds_.bottom, bounds_.top) {

	}

	OrthographicCameraController::~OrthographicCameraController() {

	}

	void OrthographicCameraController::OnUpdate(Timestep timestep) {
		PM_PROFILE_FUNCTION();
		if (Input::IsKeyPressed(PM_KEY_D)) {
			cameraPosition_.x += cos(glm::radians(cameraRotation_)) * cameraPositionChangeSpeed_ * timestep;
			cameraPosition_.y += sin(glm::radians(cameraRotation_)) * cameraPositionChangeSpeed_ * timestep;
		}
		else if (Input::IsKeyPressed(PM_KEY_A)) {
			cameraPosition_.x -= cos(glm::radians(cameraRotation_)) * cameraPositionChangeSpeed_ * timestep;
			cameraPosition_.y -= sin(glm::radians(cameraRotation_)) * cameraPositionChangeSpeed_ * timestep;
		}

		if (Input::IsKeyPressed(PM_KEY_W)) {
			cameraPosition_.x += sin(glm::radians(cameraRotation_)) * cameraPositionChangeSpeed_ * timestep;
			cameraPosition_.y += cos(glm::radians(cameraRotation_)) * cameraPositionChangeSpeed_ * timestep;
		}
		else if (Input::IsKeyPressed(PM_KEY_S)) {
			cameraPosition_.x -= sin(glm::radians(cameraRotation_)) * cameraPositionChangeSpeed_ * timestep;
			cameraPosition_.y -= cos(glm::radians(cameraRotation_)) * cameraPositionChangeSpeed_ * timestep;
		}

		if (rotationEnabled_) {
			if (Input::IsKeyPressed(PM_KEY_Q))
				cameraRotation_ += cameraRotationSpeed_ * timestep;
			if (Input::IsKeyPressed(PM_KEY_E))
				cameraRotation_ -= cameraRotationSpeed_ * timestep;

			if (cameraRotation_ > 180.0f)
				cameraRotation_ -= 360.0f;
			else if (cameraRotation_ <= -180.0f)
				cameraRotation_ += 360.0f;
			
			camera_.SetRotation(cameraRotation_);
		}		

		camera_.SetPosition(cameraPosition_);
		cameraPositionChangeSpeed_ = zoomLevel_ * 2; //adapt camera speed to zoomLevel (linear)
	}

	void OrthographicCameraController::OnEvent(Event& event) {
		PM_PROFILE_FUNCTION();
		EventDispatcher eventDispatcher(event);
		eventDispatcher.Dispatch<MouseScrolledEvent>(PM_BIND_EVENT_FUNCTION(OrthographicCameraController::OnMouseScrolled));
		eventDispatcher.Dispatch<WindowResizeEvent>(PM_BIND_EVENT_FUNCTION(OrthographicCameraController::OnWindowResize));
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& event) {
		PM_PROFILE_FUNCTION();
		zoomLevel_ -= event.GetOffsetY() * 0.25;
		zoomLevel_ = std::max(zoomLevel_, 0.25f);
		zoomLevel_ = std::min(zoomLevel_, 15.0f);
		bounds_ = { -aspectRatio_ * zoomLevel_, aspectRatio_ * zoomLevel_, -zoomLevel_, zoomLevel_ };
		camera_.SetProjection(bounds_.left, bounds_.right, bounds_.bottom, bounds_.top);
		return false;
	}

	bool OrthographicCameraController::OnWindowResize(WindowResizeEvent& event) {
		PM_PROFILE_FUNCTION();
		aspectRatio_ = (float)event.GetWidth() / (float)event.GetHeight(); 
		bounds_ = { -aspectRatio_ * zoomLevel_, aspectRatio_ * zoomLevel_, -zoomLevel_, zoomLevel_ };
		camera_.SetProjection(bounds_.left, bounds_.right, bounds_.bottom, bounds_.top);
		return false;
	}
}