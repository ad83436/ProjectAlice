#include "AliceGame.hpp"
#include "Scripts/Overworld/SpellInventory.hpp"
#include <Engine/Input/Keyboard.hpp>
#include <Engine/Battle/BattleLevel.hpp>
#include <Engine/Battle/BattleManager.hpp>
#include <Engine/Physics/PhysicsScene.hpp>
#include<Engine/Cutscene/CutsceneManager.hpp>
#include <random>
#include <chrono>
#include <iostream>

AliceGame::AliceGame()
	: Game()
	, quitTimer(0)
	, max_quit_time(15) { }

AliceGame::~AliceGame() { }

bool AliceGame::Init() {
	// read the level
	string levelToLoad;
	std::cout << "Enter level that you would like to load:" << std::endl;
	std::getline(std::cin, levelToLoad);

	if (!Game::Init()) return false;

	// initialize the singletons
	ContentHandler::Init("Resources/Textures/Textures.index", "Resources/Shaders/Shaders.index", "Resources/Fonts/Fonts.index");
	RenderScene::Init();
	ObjectFactory::Init("Resources/Objects/Objects.index");
	PhysicsScene::Init();
	BattleManager::Init("Resources/EnemyPhases/EnemyPhases.index");
	LevelManager::Init("Resources/Levels/Levels.index", levelToLoad);
	CutsceneManager::Init("Resources/Cutscenes/Cutscene.index");


	// FOR TESTING SPELL INVENTORY //

	SpellList::InitSpellData("Resources/Spells/Spells.json");
	SpellInventory::InitData("");

	SpellInventory::AddSpell("Boomerang");
	SpellInventory::AddSpell("Wind Slash");
	SpellInventory::AddSpell("Homing Lasers");
	SpellInventory::AddSpell("Absolute Barrier");
	SpellInventory::AddSpell("Absolute Reflection");
	SpellInventory::AddSpell("Healing");

	DEBUG_LOG("No. of Attack Spells: " + VTOS(SpellInventory::GetSpellCount(SpellType::Attack)));
	DEBUG_LOG("No. of Defence Spells: " + VTOS(SpellInventory::GetSpellCount(SpellType::Defence)));

	SpellInventory::EquipAtkSpell("Boomerang", 0);
	SpellInventory::EquipAtkSpell("Wind Slash", 1);
	SpellInventory::EquipAtkSpell("Wind Slash", 2);

	SpellInventory::EquipDefSpell("Absolute Barrier");

	for (uint32 i = 0; i < MAX_EQUIPPED_SPELLS; i++) {
		DEBUG_LOG("Atk Skill Slot " + VTOS(i + 1) + ": " + VTOS(SpellInventory::GetEquippedAtkSpell(i)));
	}
	DEBUG_LOG("Def Skill: " + VTOS(SpellInventory::GetEquippedDefSpell()));

	// FOR TESTING SPELL INVENTORY //

	return true;
}

void AliceGame::Update() {

	PollEvents();

	if (Keyboard::GetKey(KeyCode::Escape)) {
		++quitTimer;
		if (quitTimer > max_quit_time) Quit();
	} else {
		quitTimer = 0;
	}

	// update 
	LevelManager::Update();
	ObjectFactory::Update();

	// do physics
	PhysicsScene::Step();

}

void AliceGame::Draw() {

	// late update
	LevelManager::LateUpdate();
	ObjectFactory::LateUpdate();


	// draw 
	GetWindow()->ClearScreen(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	RenderScene::Draw();

	// cleanup
	ObjectFactory::Cleanup();
	LevelManager::Cleanup();
	ContentHandler::Clean();

	// wait for end of frame
	GetTimer()->WaitForEndOfFrame();
	GetWindow()->SwapBuffers();
}

bool AliceGame::Exit() {
	if (!Game::Exit()) return false;

	// Exit the singletons
	LevelManager::Exit();
	ObjectFactory::Exit();
	PhysicsScene::Exit();
	RenderScene::Exit();
	ContentHandler::Exit();

	return true;
}

void LoadObjects(const json& data) {
	for (auto it = data.begin(); it != data.end(); ++it) {
		if (!it->is_object()) {
			DEBUG_ERROR("Non object in object array! Skipped");
			continue;
		}
		const json& objData = (*it);

		// construct using index
		if (objData.contains("objectName") && objData["objectName"].is_string()) {
			string objectName = objData["objectName"].get<string>();
			Object* o = nullptr;

			// construct with instance data
			if (objData.contains("data") && objData.is_object())
				o = ObjectFactory::Make(objectName, objData["data"]);

			// construct without instance data
			else o = ObjectFactory::Make<Object>(objectName);

			// check
			if (o == nullptr) DEBUG_ERROR("Could not make object of name " + objectName);
			else DEBUG_LOG("Successfully made object of name " + objectName);
		}
		// construct using type
		else if (objData.contains("type") && objData["type"].is_string()) {
			string typName = objData["type"].get<string>();
			type typ = type::get_by_name(typName.c_str());
			Object* o = nullptr;

			// construct with instance data
			if (objData.contains("data") && objData.is_object())
				o = ObjectFactory::Make(typ, objData["data"]);

			// construct without instance data
			else o = ObjectFactory::Make(typ);

			// check
			if (o == nullptr) DEBUG_ERROR("Could not make object of type " + typName);
			else DEBUG_LOG("Successfully made object of type " + typName);
		}
		// if nothing print error
		else {
			DEBUG_ERROR("No Type or ObjectName found");
		}

	}
}

void AliceGame::LevelLoad(Level* level, const json& data) {
	// destroy all objects
	ObjectFactory::Clear();

	// load the level data
	if (data.contains("objects"))
		LoadObjects(data["objects"]);
}

int main(int argc, char* argv[]) {

	// set random value using chrono
	srand(std::chrono::high_resolution_clock::now().time_since_epoch().count());
	
	Game* game = new AliceGame();
	game->Run();
	delete game;

	return 0;
}