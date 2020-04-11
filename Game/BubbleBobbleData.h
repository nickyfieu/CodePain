#pragma once

namespace Game
{
	enum EnemyType : unsigned char
	{
		//0b0000 0(000)
		ZenChan		= 000,
		//0b0000 0(110)
		Mighta		= 110,
		//0b0000 0(100)
		Monsta		= 100,
		//0b0000 0(011)
		Pulpul		= 011,
		//0b0000 0(010)
		Banebou		= 010,
		//0b0000 0(001)
		Hidegons	= 001,
		//0b0000 0(101)
		Drunk		= 101,
		//0b0000 0(111)
		Invader		= 111,
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