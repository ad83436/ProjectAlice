#include "Camera.hpp"
#include "../Core/Level.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "../Game.hpp"
#undef GetObject

void Camera::Start() {
	rendScene = GetObject()->GetLevel()->GetRenderScene();
	rendScene->AddCamera(this);
	SetCameraSize(GameContext::game->GetWindow()->GetScreenSize());
}

void Camera::OnDestroy() {
	rendScene->RemoveCamera(this);
}

Camera::Camera() : rendScene(nullptr) { }
Camera::~Camera() { }

void Camera::UpdateView() {
	// update the view
	view = glm::translate(mat4(), vec3(GetObject()->GetPosition(), 0.0f));

	// update the ortho
	vec2 halfsize = size * 0.5f;
	proj = glm::ortho(-halfsize.x, halfsize.x, -halfsize.y, halfsize.y);
}