#include "ColliderComponent.hpp"
#include "PhysicsScene.hpp" // TODO: add the collider component and remove it from the physics scene

RTTR_REGISTRATION {
	registration::class_<ColliderComponent>("ColliderComponent");
}

ColliderComponent::ColliderComponent() { }

ColliderComponent::~ColliderComponent() { }

void ColliderComponent::Start() { PhysicsScene::AddComponent(this); }

void ColliderComponent::OnDestroy() { PhysicsScene::RemoveComponent(this); }
