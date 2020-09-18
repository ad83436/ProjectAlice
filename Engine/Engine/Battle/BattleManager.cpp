#include "BattleManager.hpp"

BattleManager::BattleManager() {
	// just some initial values
	players.reserve(2);
	enemies.reserve(5);
}

BattleManager::~BattleManager() {
	if (players.size() > 0)
		DEBUG_ERROR("The player list in the BattleManager was not empty. There were " + VTOS(players.size()) + " players left!");
	players.clear();
	if (enemies.size() > 0)
		DEBUG_ERROR("The enemy list in the BattleManager was not empty. There were " + VTOS(enemies.size()) + " enemies left!");
	enemies.clear();
}

#pragma region Adders & Removers

void BattleManager::AddPlayer(BTPlayer* player) {
	// temp check
	auto& players = Get()->players;
	for (auto it = players.begin(); it != players.end(); ++it) {
		if ((*it) == player) {
			DEBUG_LOG("Player was already in the battle!");
			return;
		}
	}

	Get()->players.push_back(player);
	DEBUG_LOG("Added player!");
}

void BattleManager::RemovePlayer(BTPlayer* player) {
	// find and remove
	auto& players = Get()->players;
	for (auto it = players.begin(); it != players.end(); ++it) {
		if ((*it) == player) {
			players.erase(it);
			DEBUG_LOG("Successfully removed player from battle!");
			return;
		}
	}
	DEBUG_ERROR("Could not remove player from battle. It was not found!");
}

void BattleManager::AddEnemy(BTEnemy* enemy) {
	// temp check
	auto& enemies = Get()->enemies;
	for (auto it = enemies.begin(); it != enemies.end(); ++it) {
		if ((*it) == enemy) {
			DEBUG_LOG("Enemy was already in the battle!");
			return;
		}
	}

	Get()->enemies.push_back(enemy);
	DEBUG_LOG("Added enemy!");
}

void BattleManager::RemoveEnemy(BTEnemy* enemy) {
	// find and remove
	auto& enemies = Get()->enemies;
	for (auto it = enemies.begin(); it != enemies.end(); ++it) {
		if ((*it) == enemy) {
			enemies.erase(it);
			DEBUG_LOG("Successfully removed player from battle!");
			return;
		}
	}
	DEBUG_ERROR("Could not remove player from battle. It was not found!");
}

#pragma endregion

#pragma region Getters

size_t BattleManager::GetPlayerCount() {
	return Get()->players.size();
}

BTPlayer* BattleManager::GetPlayer(size_t index) {
	return Get()->players[index];
}

size_t BattleManager::GetEnemyCount() {
	return Get()->enemies.size();
}

BTEnemy* BattleManager::GetEnemy(size_t index) {
	return Get()->enemies[index];
}

#pragma endregion
