#pragma once
#include "BinaryReaderWriter.h"
#include "Scene.h"

namespace Game
{
	class BubbleBobbleLevelReader final
	{
	public:
		BubbleBobbleLevelReader() = default;
		~BubbleBobbleLevelReader() = default;

		BubbleBobbleLevelReader(const BubbleBobbleLevelReader& other) = delete;
		BubbleBobbleLevelReader(BubbleBobbleLevelReader&& other) = delete;
		BubbleBobbleLevelReader& operator=(const BubbleBobbleLevelReader& other) = delete;
		BubbleBobbleLevelReader& operator=(BubbleBobbleLevelReader&& other) = delete;

		void ReadLevelData(cp::Scene* pScene);

		void ReadEnemyData() const;

	private:
		BinaryReaderWriter m_ReadWrite{};
	};
}