#ifndef _CORE_TIMER_HPP
#define _CORE_TIMER_HPP

#include <chrono>
#include "../General.hpp"

namespace ALC {

	// wrapper around a delta value
	struct Timestep final {
		explicit Timestep(const double delta)
			: m_delta(delta) { }

		operator float() const { return static_cast<float>(m_delta); }
		float Get() const { return static_cast<float>(m_delta); }
		double GetDouble() const { return m_delta; }
		float GetFPS() const { return static_cast<float>(1.0 / m_delta); }

	private:
		double m_delta;
	};

	class Timer {
		using steady_clock = std::chrono::steady_clock;
		using duration = std::chrono::duration<double>;
		using time_point = std::chrono::time_point<steady_clock, duration>;

		time_point lastTime;
		time_point currentTime;

		uint32 targetFPS;
		double secondsPerFrame;
		double lastDelta;

	public:

		Timer();
		~Timer();

		// returns a timestep struct
		Timestep GetTimestep() const;

		// functions
		void BeginFrame();
		void WaitForEndOfFrame();
		bool CheckIfFrameComplete();

		// getters and setters
		uint32 GetTargetFPS() const { return targetFPS; }
		void SetTargetFPS(const uint32& targetFPS_) {
			targetFPS = targetFPS_;
			secondsPerFrame = 1.0 / static_cast<double>(targetFPS);
		}
		float GetFPS() const;
		float GetDelta() const;

	};

}

#endif // !_CORE_TIMER_HPP