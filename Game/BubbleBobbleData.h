#pragma once

namespace Game
{
	enum class EnemyType : unsigned char
	{
		//0b0000 0(000)
		ZenChan		= 0b00000000,
		//0b0000 0(110)
		Mighta		= 0b00000110,
		//0b0000 0(100)
		Monsta		= 0b00000100,
		//0b0000 0(011)
		Pulpul		= 0b00000011,
		//0b0000 0(010)
		Banebou		= 0b00000010,
		//0b0000 0(001)
		Hidegons	= 0b00000001,
		//0b0000 0(101)
		Drunk		= 0b00000101,
		//0b0000 0(111)
		Invader		= 0b00000111,
	};

	struct BubbleBobbleEnemyData
	{
		EnemyType enemyType;
		unsigned char collumn;
		unsigned char row;
		bool unknown1;
		bool unknown2;
		bool unknown3;
		bool unknown4;
		bool isMovingLeft;
		float delay;
	};
}