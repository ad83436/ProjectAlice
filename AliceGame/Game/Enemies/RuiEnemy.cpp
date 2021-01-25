#include "RuiEnemy.hpp"
#include "../Systems/BulletDeleterSystem.hpp"
#include "../BTA.hpp"
#include "../Characters/Character.hpp"

RuiEnemy::RuiEnemy()
	: m_phases(this, Phase::PreBattle), m_state(State::None) {
	// bind phases
	m_phases.Bind(Phase::PreBattle, &RuiEnemy::PreBattleStep, &RuiEnemy::PreBattleBegin);
	m_phases.Bind(Phase::Phase0, &RuiEnemy::Phase0Step, &RuiEnemy::Phase0Begin);
	m_phases.Bind(Phase::Phase1, &RuiEnemy::Phase1Step, &RuiEnemy::Phase1Begin);
	m_phases.Bind(Phase::Phase2, &RuiEnemy::Phase2Step, &RuiEnemy::Phase2Begin);
	m_phases.Bind(Phase::Phase3, &RuiEnemy::Phase3Step, &RuiEnemy::Phase3Begin);
	m_phases.Bind(Phase::Phase4, &RuiEnemy::Phase4Step, &RuiEnemy::Phase4Begin);
	m_phases.Bind(Phase::PostBattle, &RuiEnemy::PostBattleStep, &RuiEnemy::PostBattleBegin);
}

RuiEnemy::~RuiEnemy() { }

void RuiEnemy::Start(ALC::Entity self) {
	Enemy::Start(self); // must be called to initialize

	auto& spr = self.GetComponent<ALC::SpriteComponent>();
	spr.texture = ALC::ContentManager::LoadTexture("Resources/Textures/Enemies/Rui-Facing-Forward.png");
	spr.textureBounds = spr.texture.GetBounds();

	// match the ratio 
	float ratio = spr.textureBounds.Height() / spr.textureBounds.Width();
	float newHeight = spr.bounds.Width() * ratio;
	spr.bounds.top = newHeight * 0.5f;
	spr.bounds.bottom = -newHeight * 0.5f;


}

void RuiEnemy::Update(ALC::Entity self, ALC::Timestep ts) {
	// update the phase
	m_phases(self, ts);
}

void RuiEnemy::LateUpdate(ALC::Entity self, ALC::Timestep ts) {
	// only do this stuff when the battle actually starts
	if (m_phases.GetState() != Phase::PreBattle ||
		m_phases.GetState() != Phase::PostBattle) {
		// takes damage from all bullets, calls OnDeath when it reaches 0
		UpdateCollisions(self, ts);

		// updates the lifetime, calls OnDeath when it reaches 0
		UpdateLifetime(self, ts);
	}
}

void RuiEnemy::OnDeath(ALC::Entity self) {
	switch (m_phases.GetState()) {
		case Phase::Phase0: m_phases.ChangeState(Phase::Phase1); break;
		case Phase::Phase1: m_phases.ChangeState(Phase::Phase2); break;
		case Phase::Phase2: m_phases.ChangeState(Phase::Phase3); break;
		case Phase::Phase3: m_phases.ChangeState(Phase::Phase4); break;
		case Phase::Phase4: m_phases.ChangeState(Phase::PostBattle); break;
		default: break;
	}
}

void RuiEnemy::BattleBegin() {
	// we start by changing phases
	m_phases.ChangeState(Phase::Phase0);
}

void RuiEnemy::PreBattleBegin(const Phase lastphase, ALC::Entity self, ALC::Timestep ts) { }
void RuiEnemy::PreBattleStep(ALC::Entity self, ALC::Timestep ts) { }

void RuiEnemy::Phase0Begin(const Phase lastphase, ALC::Entity self, ALC::Timestep ts) {
	ResetHealth(100.0f);
	SetLifetime(30.0f);
	m_state = State::Shoot3;
	// make sure our bullets spawn with this component
	ShooterBehavior::SetBulletTypes<BulletDeleterComponent>();
}
void RuiEnemy::Phase0Step(ALC::Entity self, ALC::Timestep ts) {
	// get our components
	auto [transform, cbody] = self.GetComponent<ALC::Transform2D, ALC::CharacterBody>();

	ALC::vec2 pos = transform.position - 8.0f;
	float speed = 10.0f;

	auto* playerb = BattleManager::GetCurrentCharacter();
	auto& playerTransform = playerb->GetEntity().GetComponent<ALC::Transform2D>();

	// target the players position by using this in velocity
	ALC::vec2 targetdir = playerTransform.position - transform.position;
	if (glm::length2(targetdir) > 0.0f) targetdir = glm::normalize(targetdir);

	if (m_state == State::Shoot3) {
		// set value
		ShooterBehavior::SetDefaultPosition(transform.position);
		ShooterBehavior::SetDefaultVelocity(targetdir * 300.0f);

		// uses default velocity to shoot a range
		// if default is [0, 1] then it will shoot in a range from (-45.0f / 2) to (+45.0f / 2)
		ShooterBehavior::ShootRange(self, 3, 45.0f, [](ALC::Entity bullet) {
			// update body collision
			auto& body = bullet.GetComponent<ALC::BulletBody>();
			body.radius = 4.0f;

			auto& sprite = bullet.GetComponent<ALC::SpriteComponent>();
			sprite.bounds = ALC::rect(14.0f);
			// this should be loaded ahead of time
			sprite.texture = ALC::ContentManager::LoadTexture("Resources/Textures/Grey Orb Flashing.png");
			sprite.textureBounds = ALC::rect(0.0f, 0.0f, 15.0f, 15.0f);
		});

		// change state
		m_state = State::Move;
	} else if (m_state == State::Move) {

		// move
		auto result = BTA::MoveTo(&cbody.velocity, transform.position, ALC::vec2(-100, 100), 400.0f, speed * 20.0f, ts);

		if (result == BTA::Result::Arrived) {
			// set value
			ShooterBehavior::SetDefaultPosition(transform.position);
			ShooterBehavior::SetDefaultVelocity(targetdir * 300.0f);

			ShooterBehavior::ShootRange(self, 3, 45.0f, [](ALC::Entity bullet) {
				// update body collision
				auto& body = bullet.GetComponent<ALC::BulletBody>();
				body.radius = 4.0f;

				auto& sprite = bullet.GetComponent<ALC::SpriteComponent>();
				sprite.bounds = ALC::rect(14.0f);
				// this should be loaded ahead of time
				sprite.texture = ALC::ContentManager::LoadTexture("Resources/Textures/Grey Orb Flashing.png");
				sprite.textureBounds = ALC::rect(0.0f, 0.0f, 15.0f, 15.0f);
			});

			// change state
			m_state = State::Center;
		}
	}

	else if (m_state == State::Center) {

		BTA::Result result = BTA::MoveTo(&cbody.velocity, transform.position, ALC::vec2(0, 0), 400.0f, speed * 20, ts);

		if (result == BTA::Result::Arrived) {
			m_state = State::Shoot3;
		}
	}
}

void RuiEnemy::Phase1Begin(const Phase lastphase, ALC::Entity self, ALC::Timestep ts) { }
void RuiEnemy::Phase1Step(ALC::Entity self, ALC::Timestep ts) { }

void RuiEnemy::Phase2Begin(const Phase lastphase, ALC::Entity self, ALC::Timestep ts) { }
void RuiEnemy::Phase2Step(ALC::Entity self, ALC::Timestep ts) { }

void RuiEnemy::Phase3Begin(const Phase lastphase, ALC::Entity self, ALC::Timestep ts) { }
void RuiEnemy::Phase3Step(ALC::Entity self, ALC::Timestep ts) { }

void RuiEnemy::Phase4Begin(const Phase lastphase, ALC::Entity self, ALC::Timestep ts) { }
void RuiEnemy::Phase4Step(ALC::Entity self, ALC::Timestep ts) { }

void RuiEnemy::PostBattleBegin(const Phase lastphase, ALC::Entity self, ALC::Timestep ts) {
	// enemy is done fighting
	MarkDone();
}
void RuiEnemy::PostBattleStep(ALC::Entity self, ALC::Timestep ts) { }