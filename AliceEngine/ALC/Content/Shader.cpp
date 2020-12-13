#include "Shader.hpp"
#include <fstream>
#include <glew.h>

namespace ALC {

	Shader::Shader() : m_programID(-1) { }

	Shader::Shader(std::nullptr_t) : Shader() { }

	bool Shader::IsValid() const {
		return m_programID != -1;
	}

	Shader::operator bool() const {
		return m_programID != -1;
	}

	uint32 Shader::GetID() const {
		return m_programID;
	}

	bool Shader::operator==(const Shader& other) const {
		return m_programID == other.m_programID;
	}

	bool Shader::operator!=(const Shader& other) const {
		return m_programID != other.m_programID;
	}

	static uint32 GetShaderTypeFromString(const string& shadertype) {
		if (shadertype == "vertex") return GL_VERTEX_SHADER;
		if (shadertype == "fragment") return GL_FRAGMENT_SHADER;
		return -1;
	}

	Shader Shader::LoadSource(const string& source) {
		// via "the cherno" https://youtu.be/8wFEzIYRZXg?t=1221

		unordered_map<uint32, string> sources;

		// read the file for each shader
		constexpr const char* typeToken = "#type";
		constexpr size_t typeTokenLen = 6;
		size_t pos = source.find_first_of(typeToken, 0);
		while (pos != string::npos) {
			size_t eol = source.find_first_of("\n", pos);
			if (eol != string::npos) {
				ALC_DEBUG_ERROR("Syntax error");
				return nullptr;
			}
			size_t begin = pos + typeTokenLen + 1;
			uint32 shaderType = GetShaderTypeFromString(source.substr(eol, eol - begin));
			if (shaderType == -1) {
				ALC_DEBUG_ERROR("invalid shader type");
				continue;
			}

			size_t nextLinePos = source.find_first_not_of("\n", eol);
			size_t endPos = (nextLinePos == string::npos ? source.size() - 1 : nextLinePos);
			sources.emplace(shaderType, source.substr(nextLinePos, pos - endPos));
		}

		// end 'the cherno'

		if (sources.size() == 0) {
			ALC_DEBUG_ERROR("Invalid shader source");
			return nullptr;
		}

		vector<uint32> shaders;
		uint32 shaderProgram = glCreateProgram();

		for (auto& [type, source] : sources) {
			// create and load the shader
			uint32 shaderID = glCreateShader(type);
			const char* csource = source.c_str();
			glShaderSource(shaderID, 1, &csource, 0);
			glCompileShader(shaderID);

			// check for errors
			int32 success;
			glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
			if (!success) {
				// get error and print
				char infoLog[512];
				glGetShaderInfoLog(shaderID, 512, 0, infoLog);
				ALC_DEBUG_ERROR("Failed to compile shader: " + string(infoLog));

				// delete the shader and return
				glDeleteShader(shaderID);
				continue;
			}

			glAttachShader(shaderProgram, shaderID);
			shaders.push_back(shaderID);
		}

		// no shaders were attached
		if (shaders.size() == 0) {
			ALC_DEBUG_ERROR("Could not load any shaders, deleted shader program");
			glDeleteProgram(shaderProgram);
			return nullptr;
		}

		// link
		glLinkProgram(shaderProgram);

		// check linking errors
		GLint success;
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			// get error message and print
			GLchar infoLog[512];
			glGetProgramInfoLog(shaderProgram, 512, 0, infoLog);
			ALC_DEBUG_ERROR("Failed to link program: " + string(infoLog));
			// delete shaders/program
			for (GLuint id : shaders) glDeleteShader(id);
			glDeleteProgram(shaderProgram);
			return nullptr;
		}

		// delete shaders
		for (GLuint id : shaders) glDeleteShader(id);
		shaders.clear();

		// create shader and assign program ID
		Shader shader;
		shader.m_programID = shaderProgram;
		return shader;
	}

	Shader Shader::Load(const string& path) {
		std::ifstream file(path);
		if (!file.is_open()) {
			ALC_DEBUG_ERROR("Shader path was invalid: " + path);
			return nullptr;
		}

		// read file into string
		string source;
		file.seekg(0, std::ios::end);
		source.resize(file.tellg());
		file.seekg(0, std::ios::beg);
		file.read(source.data(), source.size());
		file.close();

		return LoadSource(source);
	}

	void Shader::Delete(const Shader& shader) { 
		glDeleteProgram(shader.m_programID);
	}

}