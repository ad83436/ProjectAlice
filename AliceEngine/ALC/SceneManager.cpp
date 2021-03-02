#include "SceneManager.hpp"
#include "Input\detail\SystemEvents.hpp"
#include "Jobs.hpp"

namespace ALC {

	EngineSettings SceneManager::s_settings;
	uint32 SceneManager::s_levelToLoad = -1;
	IScene* SceneManager::s_activeScene = nullptr;
	uint32 SceneManager::s_currentSceneIndex = -1;
	Game* SceneManager::s_activeGame = nullptr;
	Window* SceneManager::s_window = nullptr;
	FT_Library SceneManager::s_fontLib = nullptr;
	bool SceneManager::s_isRunning = false;
	bool SceneManager::s_shouldQuit = false;

	void SceneManager::StartGame(Game* game_) {
		if (s_settings.scenes.bindings.size() == 0) {
			ALC_DEBUG_FATAL_ERROR("No scenes were bound");
			return;
		}

		// setup jobs
		if (s_settings.jobsystem.enable) JobQueue::__Init(s_settings.jobsystem.threadcount);

		// setup variables
		s_levelToLoad = -1;
		s_activeGame = game_;
		s_shouldQuit = false;
		Timer timer;
		s_activeGame->__Initialize(s_window, &timer);
		s_currentSceneIndex = s_settings.scenes.startScene;
		s_activeScene = s_settings.scenes.bindings[s_currentSceneIndex]();
		if (FT_Init_FreeType(&s_fontLib)) {
			ALC_DEBUG_ERROR("Font library did not initialized.");
			return;
		}

		// init
		try {
			s_activeGame->Init();
			s_activeScene->Init();
		}
		// catch any errors and quit
		catch (const std::exception& e) {
			ALC_DEBUG_FATAL_ERROR(e.what());
			delete s_activeScene;
			return;
		}

		while (s_isRunning && !s_shouldQuit) {
			timer.BeginFrame();

			if (s_levelToLoad != -1) {
				if (s_levelToLoad < s_settings.scenes.bindings.size()) {
					s_activeScene->Exit();
					delete s_activeScene;
					s_currentSceneIndex = s_levelToLoad;
					s_activeScene = s_settings.scenes.bindings[s_levelToLoad]();
					s_activeScene->Init();
				}
				s_levelToLoad = -1;
			}

			detail::SystemEvents::PollEvents();

			Timestep t = timer.GetTimestep();
			s_activeGame->Step(t);
			s_activeScene->Step(t);

			s_activeGame->PreDraw();
			s_activeScene->PreDraw();

			s_activeGame->Draw();
			s_activeScene->Draw();

			s_activeGame->PostDraw();
			s_activeScene->PostDraw();

		}

		// exit
		try {
			s_activeGame->Exit();
			s_activeScene->Exit();
		} catch (const std::exception& e) {
			ALC_DEBUG_FATAL_ERROR(e.what());
		}

		// cleanup
		if (s_settings.jobsystem.enable) JobQueue::__Exit();
		FT_Done_FreeType(s_fontLib);
		delete s_activeScene, s_activeScene = nullptr;
	}

}