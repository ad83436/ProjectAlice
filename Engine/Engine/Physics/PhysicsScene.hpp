#ifndef _PHYSICS_PHYSICS_SCENE_HPP
#define _PHYSICS_PHYSICS_SCENE_HPP
#include "ColliderComponent.hpp"
#include "../General/Macros.hpp"

class PhysicsScene {
	PRIVATE_SINGLETON(PhysicsScene);

	using CollDataPairIter = list<pair<CollisionData, unsigned int>>::iterator;
	using ObjectPairType = pair<Object*, unsigned int>;

	forward_list<ObjectPairType> objects;
	forward_list<ColliderComponent*> colliderComponents;
	//forward_list<TilemapCollider*> tilemapColliders;

	PhysicsScene();
	~PhysicsScene();

public:

	// adding/removing colliders
	static void AddComponent(ColliderComponent* colliderComponent);
	static void RemoveComponent(ColliderComponent* colliderComponent);

	// events
	static void Init();
	static void Exit();
	static void Step();

private:

	// helper functions
	static CollDataPairIter FindCollision(list<pair<CollisionData, unsigned int>>& collisions, Collider* c0, Collider* c1);
	static bool DoCollision(ColliderComponent* c0, Collider* c1);
	static bool DoOverlap(ColliderComponent* c0, Collider* c1);

};

#endif // !_PHYSICS_PHYSICS_SCENE_HPP