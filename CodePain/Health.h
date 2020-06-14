#pragma once
#include "BaseComponent.h"

namespace cp
{
	class Health final : public BaseComponent
	{
	public:
		Health(bool deleteObjOnDeath, int maxHealth);
		virtual ~Health() = default;

		virtual void Update(float elapsedSec) override;
		virtual void FixedUpdate(float elapsedSec) override;
		virtual void Draw() const override;
		virtual void DebugDraw() const override;

		Health(const Health&) = delete;
		Health(Health&&) = delete;
		Health& operator= (const Health&) = delete;
		Health& operator= (const Health&&) = delete;

		bool IsDead() const;
		inline int GetHealth() const { return m_CurrentHealth; }
		
		// can be positive or negative
		void AddHealth(int amount);

	private:
		const bool m_DeleteOnDeath;
		const int m_MaxHealth;

		int m_CurrentHealth;

		const float m_ImunityTime{ 2.f };
		float m_ImunityTimer = 3.f;
		bool m_GotHit{};
	};
}