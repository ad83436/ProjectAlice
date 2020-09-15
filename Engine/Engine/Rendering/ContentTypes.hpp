#ifndef _RENDERING_CONTENT_TYPES_HPP
#define _RENDERING_CONTENT_TYPES_HPP
#include <glm/vec2.hpp>
using glm::uvec2;

class Texture {

	struct Data {
		unsigned int id;
		uvec2 size;
		unsigned int refcount;
	};
	Data* data;

public:

	// constructors/operators & destructor
	Texture();
	Texture(unsigned int id_, const uvec2& size_);

	Texture(const Texture& other);
	Texture& operator=(const Texture& other);
	Texture(Texture&& other);
	Texture& operator=(Texture&& other);

	~Texture();

	// functions
	unsigned int GetID() const { if (data) return data->id; else return -1; }
	operator unsigned int() const { if (data) return data->id; else return -1; }
	unsigned int GetRefCount() const { if (data) return data->refcount; else return -1; }
	uvec2 GetSize() const { if (data) return data->size; else return uvec2(); }

private:

	// helper
	void FreeThis();

};

class Shader {

	struct Data {
		unsigned int id;
		unsigned int refcount;
	};
	Data* data;

public:

	// constructors/operators & destructor
	Shader();
	Shader(unsigned int id_);

	Shader(const Shader& other);
	Shader& operator=(const Shader& other);
	Shader(Shader&& other);
	Shader& operator=(Shader&& other);

	~Shader();

	// functions
	unsigned int GetID() const { if (data) return data->id; else return -1; }
	operator unsigned int() { if (data) return data->id; else return -1; }
	unsigned int GetRefCount() const { if (data) return data->refcount; else return -1; }

private:

	// helper
	void FreeThis();

};

#endif // !_RENDERING_CONTENT_TYPES_HPP