#ifndef PLAYER_OWPLAYER_HPP
#define PLAYER_OWPLAYER_HPP
#include <Engine/Common.hpp>
#include <Engine/Rendering/UIRenderer.hpp>

class OWPlayer : public Object {

	enum class Facing : char {
		Right,
		Up,
		Down,
		Left
	};

	Sprite* sprite;
	Camera* cam;
	CircleCollider* coll;
	UIRenderer* ui;

	float colorMix;
	bool increaseMix;
	unsigned int animTimer;
	Facing facing;

	// projectile settings
	vec4 shootColor;
	float shootSpeed;
	unsigned int shootDestroyTimer;

public:

	OWPlayer();
	~OWPlayer();

	void Start() override;
	void Update() override;
	void LateUpdate() override;
	void OnCollisionEnter(const CollisionData& data) override;
	void OnCollisionExit(const CollisionData& data) override;

	RTTR_ENABLE(Object) RTTR_REGISTRATION_FRIEND
};

#endif // !PLAYER_OWPLAYER_HPP