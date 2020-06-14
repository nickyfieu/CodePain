#pragma once
#include "BaseComponent.h"

namespace cp
{
	class Target final : public BaseComponent
	{
	public:
		Target();
		virtual ~Target() = default;

		virtual void Update(float elapsedSec) override;
		virtual void FixedUpdate(float elapsedSec) override;
		virtual void Draw() const override;
		virtual void DebugDraw() const override;

		Target(const Target&) = delete;
		Target(Target&&) = delete;
		Target& operator= (const Target&) = delete;
		Target& operator= (const Target&&) = delete;

		void AddTarget(GameObject* ref);
		const std::vector<GameObject*>& GetTargets() const { return m_Targets; }

	private:
		std::vector<GameObject*> m_Targets;

	};
}