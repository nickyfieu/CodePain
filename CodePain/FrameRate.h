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

		FrameRate(const FrameRate& other) = delete;
		FrameRate(FrameRate&& other) = delete;
		FrameRate& operator=(const FrameRate& other) = delete;
		FrameRate& operator=(FrameRate&& other) = delete;

		bool GetFrameRate(float& container);
	private:
		float m_Time;
		int m_FrameCounter;
	};
}