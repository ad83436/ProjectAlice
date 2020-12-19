#include "DemoChara.hpp"

DemoChara::DemoChara()
	: timer(0.0f), circleshootoffset(0.0f), clockwise(true), spinspeedmult(1.0f) {
	textures = {
		ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/1B Ninja.png"),
		ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/AAEEEIOU.png"),
		ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/AmazedFace.png"),
		ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/Ayyad.jpg"),
		ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/ayyad.png"),
		ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/Ayyyad.jpg"),
		ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/Dwane_Face.png"),
		ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/HACK.png"),
		ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/IMG_2312.JPG"),
		ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/King_Sean.png"),
		ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/mohd.png"),
		ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/MrV.png"),
		ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/NIGHT.png"),
		ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/P.E.E.P.png"),
		ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/PizzaTime.png"),
		ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/Random.png"),
		ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/Random_Purple.png"),
		ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/Random_Yellow.png"),
		ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/Scott.png"),
		ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/SeanToxic.png"),
		ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/Slick Shades Devil.png"),
		ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/unknown.png")
	};
}

DemoChara::~DemoChara() { }

void DemoChara::Start(ALC::Entity self) {
	if (!self.HasComponent<ALC::Transform2D>()) self.AddComponent<ALC::Transform2D>();
	if (!self.HasComponent<ALC::Rigidbody2D>()) self.AddComponent<ALC::Rigidbody2D>();
	if (!self.HasComponent<ALC::SpriteComponent>()) self.AddComponent<ALC::SpriteComponent>();

	auto& spr = self.GetComponent<ALC::SpriteComponent>();
	//spr.texture = ALC::ContentManager::LoadTexture("Resources/Textures/Grey Orb Flashing.png");
	spr.bounds = ALC::rect(-8, -8, 8, 8);
	//spr.textureBounds = ALC::rect(ALC::vec2(0.0f), spr.texture.GetSize());

}

void DemoChara::OnDestroy(ALC::Entity self) { }

void DemoChara::Shoot(ALC::Entity self, const float angle, const float speed, const ALC::vec2 position) {
	if (!self.HasComponent<ALC::EntityCreatorComponent>())
		self.AddComponent<ALC::EntityCreatorComponent>();
	auto& ecc = self.GetComponent<ALC::EntityCreatorComponent>();

	ecc.Create([angle, speed, position](ALC::Entity e) {
		auto& transform = e.AddComponent<ALC::Transform2D>();
		auto& rigidbody = e.AddComponent<ALC::Rigidbody2D>();
		e.AddComponent<DemoBulletComponent>();
		auto& sprite = e.AddComponent<ALC::SpriteComponent>();
		//sprite.texture = textures[rand() % textures.size()];
		//sprite.textureBounds = ALC::rect(ALC::vec2(0.0f), sprite.texture.GetSize());
		sprite.bounds = ALC::rect(-3, -3, 3, 3);
		transform.position = position;
		ALC::vec4 vel = glm::rotateZ(ALC::vec4(0.0f, 1.0f, 0.0f, 1.0f), ALC_TO_RADIANS(angle));
		rigidbody.velocity = ALC::vec2(vel.x, vel.y) * speed;
	});
}

void DemoChara::Update(ALC::Entity self, ALC::Timestep t) {
	if (self.HasComponent<ALC::Transform2D, ALC::Rigidbody2D>()) {
		// get components
		ALC::Transform2D& transform = self.GetComponent<ALC::Transform2D>();
		ALC::Rigidbody2D& rigidbody = self.GetComponent<ALC::Rigidbody2D>();

		// get input
		const auto key_up = ALC::Keyboard::GetKey(ALC::KeyCode::ArrowUp);
		const auto key_down = ALC::Keyboard::GetKey(ALC::KeyCode::ArrowDown);
		const auto key_left = ALC::Keyboard::GetKey(ALC::KeyCode::ArrowLeft);
		const auto key_right = ALC::Keyboard::GetKey(ALC::KeyCode::ArrowRight);
		const auto key_shoot = ALC::Keyboard::GetKey(ALC::KeyCode::KeyC);

		// convert input into velocity
		glm::vec2 input = glm::vec2(key_right.IsHeld() - key_left.IsHeld(), key_up.IsHeld() - key_down.IsHeld());
		if (glm::length2(input) > 0.0f) {
			//ALC_DEBUG_LOG("Moving at " + VTOS(input));
		}
		rigidbody.velocity = input * 60.0f;

		// calculate shoot related variables
		constexpr float shoottime = 0.07f;
		if (clockwise)
			circleshootoffset += 50.0f * t * spinspeedmult;
		else
			circleshootoffset -= 50.0f * t * spinspeedmult;
		spinspeedmult += t * 1.3f;

		// shoot
		if (key_shoot) {
			timer += t;
			if (timer > shoottime || key_shoot.Pressed()) {
				if (!key_shoot.Pressed())
					timer -= shoottime;

				// shoots 'shootcount' bullets in circular formation
				constexpr ALC::uint32 shootcount = 9;
				for (ALC::uint32 i = 0; i < shootcount; i++) {
					// (360.0f / float(shootcount)) the difference in angle if you want to shoot 'shootcount' bullets
					// * float(i) multiplies to get a specific angle
					Shoot(self, (360.0f / float(shootcount)) * float(i) + circleshootoffset, 80.0f, transform.position);
				}
			}
		} 
		// not shooting
		else {
			circleshootoffset = 0.0f;
			timer = 0.0f;
			spinspeedmult = 1.0f;
			// flip shoot rotation dir
			if (key_shoot.Released()) clockwise = !clockwise;
		}
	}
}

void DemoChara::LateUpdate(ALC::Entity self, ALC::Timestep t) { }
