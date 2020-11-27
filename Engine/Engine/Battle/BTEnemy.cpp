#include "BTEnemy.hpp"
#include "../Game.hpp"
#include "BTPhase.hpp"

RTTR_REGISTRATION {
	registration::class_<BTEnemy>("BTEnemy")
		.public_object_constructor
		.property("phases",&BTEnemy::phases)
		.property("currentPhase",&BTEnemy::currentPhase)
		.property("maxHealth",&BTEnemy::maxHealth);
}

BTEnemy::BTEnemy() : currentHealth(0.0f), currentPhase(-1), maxHealth(0.0f) {
	BattleManager::AddEnemy(this);

}

BTEnemy::~BTEnemy() {
	OnDestroy();
	BattleManager::RemoveEnemy(this);
}

void BTEnemy::Start() {

}

void BTEnemy::Update() {

	if (phases.empty()) {
		DEBUG_ERROR("There are no phases in the phases Vector");
	}

	//right now only goes through the phases 0,1,2 etc....
	//when current phase is greater than the size go back to the begining of the vector 

	else {

		if (currentPhase == -1) {
			currentPhase = 0;
			phases[currentPhase]->StartPhase();
		}

		if (!phases[currentPhase]->IsComplete()) {

			phases[currentPhase]->UpdatePhase();
		}

		if (phases[currentPhase]->IsComplete()) {
			currentPhase += 1;

			if (currentPhase > phases.size() - 1) {
				currentPhase = 0;
				phases[currentPhase]->StartPhase();
				return;
			}

			phases[currentPhase]->StartPhase();
		}
	}
}

void BTEnemy::LateUpdate() { }

void BTEnemy::OnDestroy() {
	for (auto phase : phases) {
		phase = nullptr;
	}
	phases.clear();

}

BTPhase* BTEnemy::AddPhase(const type type_) {
	if (!type_.is_derived_from(type::get<BTPhase>())) {
		DEBUG_ERROR("Type : " + type_.get_name() + "is not of type BTPhase");
		return nullptr;
	}

	variant phaseObj = type_.get_constructor().invoke();
	if (!phaseObj) {
		DEBUG_ERROR("Could not construct phase of type : " + type_.get_name());
		return nullptr;
	}

	BTPhase* phase_ = phaseObj.get_value<BTPhase*>();
	if (!phase_) {
		DEBUG_ERROR("could not instatiate phase : " + type_.get_name());
		return nullptr;
	}

	phases.push_back(phase_);
	phase_->SetEnemy(this);
	phase_->Init();
	return phase_;

}

BTPhase* BTEnemy::GetPhase(const string& phaseName_) {
	for (BTPhase* phase : phases) {

		if (phase->GetPhaseName() == phaseName_) {
			return phase;
		}
	}

	DEBUG_ERROR("No phase found with name " + phaseName_);
	return nullptr;
}







