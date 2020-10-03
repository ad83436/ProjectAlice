#include "Projectile.hpp"

RTTR_REGISTRATION {
	registration::class_<Projectile>("Projectile")
		.public_object_constructor;
}

Projectile::Projectile()
	: timer(0), sprite(nullptr), color(1.0f), destroyTime(60) { }

void Projectile::Start() {
	// create sprite
	sprite = AddComponent<Sprite>();
	sprite->LoadTexture("Flashing Grey Orb");
	sprite->SetTilingSize(vec2(16.0f));
	sprite->SetPivot(vec2(8.0f));
}

void Projectile::Update() { 
	const static float delta = (1.0f / 60.0f);
	SetPosition(GetPosition() + GetVelocity() * delta);
}

void Projectile::LateUpdate() {
	// optimisation
	static const float frame_per_second = 1.0f / 12.0f;

	timer++;
	sprite->SetTilingIndex(static_cast<unsigned int>(timer * frame_per_second) % 5);

	if (timer > destroyTime) { Destroy(this); }
}
