#pragma once
#include "BaseComponent.h"

namespace cp
{
	class Transform final : public BaseComponent
	{
	public:
		Transform(float x = 0.f, float y = 0.f, float z = 0.f);
		virtual ~Transform();

		virtual void Update(float elapsedSec) override;
		virtual void FixedUpdate(float elapsedSec) override;
		virtual void Draw() const override;
		virtual void DebugDraw() const override;

		Transform(const Transform&) = delete;
		Transform(Transform&&) = delete;
		Transform& operator= (const Transform&) = delete;
		Transform& operator= (const Transform&&) = delete;

		void SetPosition(float x, float y, float z);
		void Translate(float x, float y, float z);
		const glm::vec3& GetPosition() const { return m_Position; }

	private:
		glm::vec3 m_Position;
	};
}
