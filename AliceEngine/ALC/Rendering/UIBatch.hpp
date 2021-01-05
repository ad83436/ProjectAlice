#ifndef ALC_RENDERING_UIBATCH_HPP
#define ALC_RENDERING_UIBATCH_HPP
#include "../General.hpp"
#include "../Content/Font.hpp"
#include "../Content/Texture.hpp"
#include "../Content/Shader.hpp"

namespace ALC {

	// used to draw ui to the screen
	// draws in pixel coordinates
	class UIBatch final {
	public:

		UIBatch();
		~UIBatch();

		// drawing functions

		// starts drawing the UI
		void Begin(Shader shader = nullptr);

		// draws a quad to screen
		void DrawQuad(rect position, vec4 color = ALC_COLOR_WHITE, rect target = rect(0.0f), Texture texture = nullptr);

		// draws text to the screen
		void DrawText(string text, rect position, Font font, vec4 color = ALC_COLOR_WHITE);

		// stops drawing and outputs to the screen
		void End();

	private:

		struct vertex {
			vec2 position;
			vec2 uvcoords;
			vec4 color;
			int32 textureIndex = -1;
		};

		vector<vertex> m_verticies;
		vector<uint32> m_textures;
		uint32 m_vao, m_vbo;
		uint32 m_TextureCountLoc;
		uint32 m_bufferSize;
		Shader m_defaultShader;
		Shader m_currentShader;

		uint32 TryAddTexture(const Texture& texture);
		void DrawCurrent();
	};

}

#endif // !ALC_RENDERING_UIBATCH_HPP