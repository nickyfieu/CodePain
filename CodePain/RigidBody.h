#pragma once
#include "BaseComponent.h"

namespace cp
{
	class RigidBody final : public BaseComponent
	{
	public:
		RigidBody(bool applyGravity = true);
		virtual ~RigidBody();

		void PreFixedUpdate(float elapsedSec);
		virtual void Update(float elapsedSec) override;
		virtual void FixedUpdate(float elapsedSec) override;
		virtual void Draw() const override;
		virtual void DebugDraw() const override;

		RigidBody(const RigidBody&) = delete;
		RigidBody(RigidBody&&) = delete;
		RigidBody& operator= (const RigidBody&) = delete;
		RigidBody& operator= (const RigidBody&&) = delete;

		void SetForce(const glm::vec2& force);
		void AddForce(const glm::vec2& force, const BaseComponent* owner);

		glm::vec2 GetForce() const { return m_Force; }
		// note the actual velocity is velocity.x , -velocity.y ! ! ! ! ! 
		glm::vec2 GetVelocity() const { return m_Velocity; }
		bool GetColLeft() const { return m_IsColLeft; }
		bool GetColDown() const { return m_IsColDown; }
		bool GetColRight() const { return m_IsColRight; }
		bool GetColUp() const { return m_IsColUp; }
		bool GetOnGround() const { return m_IsOnGround; }

		void SetIsColLeft(bool col);
		void SetIsColRight(bool col);
		void SetIsColDown(bool col);
		void SetIsColUp(bool col);
		void SetIsOnGround(bool col);
		void SetUseGravit(bool use);
		void SetResetXForceEachFrame(bool use);

		void SetEntryUDTime(float time);
		void SetEntryLRTime(float time);

		void SetGravitationalForce(const glm::vec2& gravity);

	private:

		// reference to a component tha has added a force to the rigidbody in the time between the previous and the current fixed update cycle
		std::vector<const BaseComponent*> m_pAddedForce;

		bool m_IsColLeft = false;
		bool m_IsColDown = false;
		bool m_IsColRight = false;
		bool m_IsColUp = false;
		// same as is col down but only for static
		bool m_IsOnGround = false;

		glm::vec2 m_Force;
		glm::vec2 m_Gravity = { 0.f,-500.f};
		glm::vec2 m_Velocity;

		float m_EntryUDTime, m_EntryLRTime;

		bool m_ApplyGravity;

		bool m_ResetXForceEachFrame = true;
	};
}