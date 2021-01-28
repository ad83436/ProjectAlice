#ifndef ALC_SCENEMANAGER_HPP
#define ALC_SCENEMANAGER_HPP
#include "General.hpp"
#include "Core\Game.hpp"
#include "Content\ContentManager.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H
#include "Core\Timer.hpp"

namespace ALC {

	class IScene {
	public:
		virtual ~IScene() = 0 { }
		virtual void Init() = 0;
		virtual void Exit() = 0;
		virtual void Step(ALC::Timestep ts) = 0;
		virtual void PreDraw() = 0;
		virtual void Draw() = 0;
		virtual void PostDraw() = 0;
	};

	using SceneBinding = IScene * (*)();
	template<class SceneType>
	inline SceneBinding __BindScene() {
		return []()->::ALC::IScene* { return new SceneType(); };
	}
	#define ALC_BIND_SCENE(SceneType) ::ALC::__BindScene<SceneType>()

	class SceneManager {
		ALC_NON_CONSTRUCTABLE(SceneManager);
	public:

		template<typename GameTy, typename Bindings, typename = std::enable_if_t<std::is_default_constructible_v<GameTy>>>
		static void Start(const string& title_, const uvec2& windowSize_, Bindings&& bindings);

		static void LoadLevel(const uint32 index) { s_levelToLoad = index; }
		static uint32 GetActiveSceneBuildIndex() { return s_currentSceneIndex; }
		static bool IsRunning() { return s_isRunning; }
		static void Quit() { s_shouldQuit = true; }
		static IScene* GetActiveScene() { return s_activeScene; }
		static Game* GetActiveGame() { return s_activeGame; }
		static Window* GetWindow() { return s_window; }

	private:

		static void StartGame(Game* game_, const std::vector<SceneBinding> bindings);
		static uint32 s_levelToLoad;
		static IScene* s_activeScene;
		static uint32 s_currentSceneIndex;
		static Game* s_activeGame;
		static Window* s_window;
		static FT_Library s_fontLib;
		static bool s_isRunning;
		static bool s_shouldQuit;

	public:
		static FT_Library __GetFTLibrary() { return s_fontLib; }
	};

	template<typename GameTy, typename Bindings, typename>
	inline void SceneManager::Start(const string& title_, const uvec2& windowSize_, Bindings&& bindings) {
		if (s_isRunning) {
			ALC_DEBUG_ERROR("Game is already running, cannot start another instance");
			return;
		}
		s_isRunning = true;
		s_window = new Window(title_, windowSize_);
		Scope<Game> game(new GameTy());
		StartGame(game.get(), bindings);
		ContentManager::Clear();
		s_isRunning = false;
	}

}

#endif // !ALC_SCENEMANAGER_HPP