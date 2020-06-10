#pragma once
#include "BaseComponent.h"
namespace cp
{
	class FrameRate final : public BaseComponent
	{
	public:
		explicit FrameRate();
		virtual ~FrameRate() = default;

		virtual void Update(float elapsedSec) override;
		virtual void FixedUpdate(float elapsedSec) override;
		virtual void Draw() const override;
		virtual void DebugDraw() const override;

		FrameRate(const FrameRate& other) = delete;
		FrameRate(FrameRate&& other) = delete;
		FrameRate& operator=(const FrameRate& other) = delete;
		FrameRate& operator=(FrameRate&& other) = delete;

		bool GetFrameRate(float& container);
		bool GetFixedFrameRate(float& container);
	private:
		float m_Time;
		float m_FixedTimer;
		int m_FrameCounter;
		int m_FixedFrameCounter;
	};
}