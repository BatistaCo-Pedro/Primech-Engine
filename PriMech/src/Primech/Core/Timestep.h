#pragma once

namespace PriMech {
	class Timestep {
	public:
		Timestep(float time = 0.0f) : time_(time) {}

		operator float() const { return time_; }

		inline float GetSeconds() const { return time_; }
		inline float GetMiliSeconds() const { return time_ * 1000.0f; }
	private:
		float time_;
	};
}