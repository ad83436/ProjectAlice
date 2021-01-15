#ifndef ALICE_SCENES_BATTLELEVEL_HPP
#define ALICE_SCENES_BATTLELEVEL_HPP
#include <ALC\SceneManager.hpp>
#include <ALC\Entities.hpp>
#include <ALC\Bullets.hpp>
#include <ALC\Rendering.hpp>
#include "../BattleManager.hpp"

class BattleLevel : public ALC::IScene {
public:

	BattleLevel();
	virtual ~BattleLevel() = 0;

	ALC::Registry& GetReg() { return m_reg; }
	ALC::ContentStorage& GetStorage() { return m_storage; }
	ALC::Camera& GetCamera() { return m_camera; }
	ALC::SpriteBatch& GetSpr() { return m_batch; }
	ALC::UIBatch& GetUI() { return m_ui; }

	float GetTimescale() const { return m_timescale; }
	void SetTimescale(const float timescale) { m_timescale = timescale; }

	Character* GetCharacter() const { return m_character; }

protected:

	virtual void GameStep(ALC::Timestep t) = 0;
	virtual void Init();
	virtual void Exit();
	virtual void Draw();
	virtual void PostDraw() { }
	virtual void PreDraw() { }

private: 

	void Step(ALC::Timestep t) override;

	ALC::ContentStorage m_storage;
	ALC::Registry m_reg;
	ALC::EntityCreationHandler m_ech;
	ALC::SpriteBatch m_batch;
	ALC::UIBatch m_ui;
	ALC::Camera m_camera;
	ALC::BulletPhysicsHandler m_bPhysics;
	Character* m_character;
	float m_timescale;

};

#endif // !ALICE_SCENES_BATTLELEVEL_HPP