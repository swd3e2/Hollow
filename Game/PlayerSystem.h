#pragma once

#include <Hollow/ECS/System.h>
#include <Hollow/Input/InputManager.h>
#include "TransformComponent.h"
#include "GameObject.h"
#include <Hollow/ECS/EntityManager.h>
#include <Hollow/Common/Log.h>
#include "Bullet.h"
#include "BulletComponent.h"
#include "RenderComponent.h"
#include "PlayerComponent.h"
#include <Hollow/Graphics/Camera.h>

class PlayerSystem : public Hollow::System<PlayerSystem>
{
private:
	const Hollow::Vector3 constantForward = Hollow::Vector3(0.0f, 1.0f, 0.0f);
	Hollow::Vector4 cursorPosition;
	Hollow::Vector3 forward;

	Hollow::Vector4 temp;

	Hollow::Camera* m_Camera;

	const float BULLET_COOLDOWN_TIMEOUT = 0.1f;
	float bulletCooldown = 0.0f;
public:
	PlayerSystem(Hollow::Camera* camera) :
		m_Camera(camera)
	{}

	virtual void PreUpdate(double dt) override
	{
		cursorPosition.x = (Hollow::InputManager::mcx / 1920.0f * 2.0f - 1.0f);
		cursorPosition.y = -(Hollow::InputManager::mcy / 1080.0f * 2.0f - 1.0f);
		cursorPosition.z = 0.0f;
		cursorPosition.w = 1.0f;

		Hollow::Matrix4 tempmat = Hollow::Matrix4::transpose(m_Camera->getViewMatrix()) * m_Camera->getProjectionMatrix();
		Hollow::Matrix4 invtempmat = Hollow::Matrix4::inverse(tempmat);

		temp = cursorPosition * invtempmat;
		temp.w = 1.0 / temp.w;

		temp.x *= temp.w;
		temp.y *= temp.w;
		temp.z = 0.0f;
	}

	virtual void Update(double dt) override
	{
		for (auto& entity : Hollow::EntityManager::instance()->container<GameObject>()) {
			if (entity.hasComponent<TransformComponent>() && entity.hasComponent<PlayerComponent>()) {
				TransformComponent* transform = entity.getComponent<TransformComponent>();
				forward = constantForward * Hollow::Matrix4::rotation(0.0f, 0.0f, -transform->rotation.z);

				if (Hollow::InputManager::GetKeyboardKeyIsPressed(Hollow::eKeyCodes::KEY_A)) {
					transform->position.x -= 0.01f * dt;
				} else if (Hollow::InputManager::GetKeyboardKeyIsPressed(Hollow::eKeyCodes::KEY_D)) {
					transform->position.x += 0.01f * dt;
				}

				if (Hollow::InputManager::GetKeyboardKeyIsPressed(Hollow::eKeyCodes::KEY_W)) {
					transform->position.y += 0.01f * dt;
				} else if (Hollow::InputManager::GetKeyboardKeyIsPressed(Hollow::eKeyCodes::KEY_S)) {
					transform->position.y -= 0.01f * dt;
				}

				Hollow::Vector3 rotation = Hollow::Vector3::normalize(Hollow::Vector3(transform->position.x - temp.x, transform->position.y - temp.y, 0.0f));
				transform->rotation.z = atan2(rotation.x, -rotation.y);// -Hollow::Math::HALF_PI;

				if (bulletCooldown < BULLET_COOLDOWN_TIMEOUT) {
					bulletCooldown += dt / 1000.0;
				}

				bool canShoot = bulletCooldown >= BULLET_COOLDOWN_TIMEOUT;

				if (Hollow::InputManager::GetMouseButtonIsPressed(Hollow::eMouseKeyCodes::MOUSE_LEFT) && canShoot) {
					Hollow::DelayedTaskManager::instance()->add([&, transform, rotation]() {
						Bullet* bullet = Hollow::EntityManager::instance()->create<Bullet>();
						BulletComponent* bulletComponent = bullet->addComponent<BulletComponent>();
						TransformComponent* transformComponent = bullet->addComponent<TransformComponent>();
						RenderComponent* renderComponent = bullet->addComponent<RenderComponent>();

						Vertex vertices[] = {
							Vertex({  1.0f,  1.0f, 0.0f }, { 1.0f, 0.0f }),
							Vertex({  1.0f, -1.0f, 0.0f }, { 1.0f, 1.0f }),
							Vertex({ -1.0f,  1.0f, 0.0f }, { 0.0f, 0.0f }),
							Vertex({ -1.0f, -1.0f, 0.0f }, { 0.0f, 1.0f }),
						};
						renderComponent->vertexBuffer = Hollow::VertexBuffer::create({ vertices, 4, sizeof(Vertex) });

						unsigned int indices[] = { 0, 1, 2, 2, 1, 3 };
						renderComponent->indexBuffer = Hollow::IndexBuffer::create({ indices, 6, Hollow::IndexFormat::IFT_UINT });

						transformComponent->rotation = transform->rotation;
						transformComponent->position = transform->position;
						bulletComponent->velocity.x = -rotation.x * 20.0f;
						bulletComponent->velocity.y = -rotation.y * 20.0f;

						Hollow::Log::instance()->debug("velocity x {} y {}", bulletComponent->velocity.x, bulletComponent->velocity.y);

						bulletCooldown = 0.0f;
					});
				}
			}
		}
	}

	virtual void PostUpdate(double dt) override
	{}
};