#pragma once
#include "BaseComponent.h"

namespace cp
{
	class Score final : public BaseComponent
	{
	public:
		Score(int score);
		virtual ~Score() = default;

		virtual void Update(float elapsedSec) override;
		virtual void FixedUpdate(float elapsedSec) override;
		virtual void Draw() const override;
		virtual void DebugDraw() const override;

		Score(const Score&) = delete;
		Score(Score&&) = delete;
		Score& operator= (const Score&) = delete;
		Score& operator= (const Score&&) = delete;

		inline int GetScore() const { return m_CurrentScore; }
		// can be positive or negative
		void AddScore(int amount);

	private:
		int m_CurrentScore;

	};
}