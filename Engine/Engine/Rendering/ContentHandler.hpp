#ifndef _RENDERING_CONTENT_HANDLER_HPP
#define _RENDERING_CONTENT_HANDLER_HPP
#include "../General/Types.hpp"
#include "ContentTypes.hpp"
#include "../Objects/FileIndex.hpp"

class ContentHandler {

	// textures and shaders with file index
	map<string, Texture> textures;
	map<string, Shader> shaders;
	FileIndex textureIndex;
	FileIndex shaderIndex;

public:

	ContentHandler(const string& textureIndexPath_, const string& shaderIndexPath_);
	~ContentHandler();

	// events
	void CleanUp();

	// functions
	Texture LoadTexture(const string& textureName);
	Shader LoadShader(const string& shaderName);

};

#endif // !_RENDERING_CONTENT_HANDLER_HPP