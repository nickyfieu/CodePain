#include "BinaryReaderWriter.h"
#include <iostream>
#include <string>
#include <fstream>

#define PRINTFBYTEPATERN "%c%c%c%c%c%c%c%c"
#define PRINTFBYTE(byte)	 \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 

enum EnemyType : unsigned char
{
	ZenChan		= 0b000,
	Mighta		= 0b110,
	Monsta		= 0b100,
	Pulpul		= 0b011,
	Banebou		= 0b010,
	Hidegons	= 0b001,
	Drunk		= 0b101,
	Invader		= 0b111,
};

std::string GetEnemyStringFromType(EnemyType type)
{
	switch (type)
	{
	case ZenChan:
		return "Zen-Chan";
		break;
	case Mighta:
		return "Mighta";
		break;
	case Monsta:
		return "Monsta";
		break;
	case Pulpul:
		return "Pulpul";
		break;
	case Banebou:
		return "Banebou";
		break;
	case Hidegons:
		return "Hidegons";
		break;
	case Drunk:
		return "Drunk";
		break;
	case Invader:
		return "Invader";
		break;
	}
	return "";
}

int main()
{
	BinaryReaderWriter readerWriter{ "leveldata.dat" };

	readerWriter.OpenFileToRead();
	readerWriter.ChangeFilePath("SeperatedLevelData.dat");
	readerWriter.OpenFileToWrite();

	{
		unsigned char byte = ' ';
		unsigned char mask = 0b00000011;
		int amountRead{ 0 };
		int size = ((100 * 25 * 32) / 8);
		for (int i = 0; i < size; i++)
		{
			readerWriter.BinaryReading(byte);
			if ((amountRead % 4) == 3)
			{
				byte |= mask;
			}
			readerWriter.BinaryWriting(byte);
			amountRead++;
		}
		readerWriter.CloseFileToWrite();
	}

	readerWriter.ChangeFilePath("SeperatedEnemyData.dat");
	readerWriter.OpenFileToWrite();

	{	// skip 10 bytes of data ??? currently unknown
		unsigned char byte = ' ';
		for (int i = 0; i < 10; i++) readerWriter.BinaryReading(byte);
	}

	{	// read 
		unsigned char byte1 = ' ';
		unsigned char byte2 = ' ';
		unsigned char byte3 = ' ';
		int amountRead = 1;
		int enemy = 1;
		printf("------------- BeginOfLevel(%i) -------------\n", amountRead);
		while (amountRead != 101)
		{
			readerWriter.BinaryReading(byte1);

			readerWriter.BinaryWriting(byte1);
			if (byte1 != 0b00000000)
			{
				readerWriter.BinaryReading(byte2);
				readerWriter.BinaryReading(byte3);

				readerWriter.BinaryWriting(byte2);
				readerWriter.BinaryWriting(byte3);

				EnemyType enemyType = EnemyType(byte1 & 0b00000111);
				const char* enemyTypeStr = GetEnemyStringFromType(enemyType).c_str();

				unsigned char col = byte1 >> 3;
				unsigned char row = byte2 >> 3;

				bool bit1 = byte2 & 0b00000100;
				bool bit2 = byte2 & 0b00000010;
				bool bit3 = byte2 & 0b00000001;

				bool bit4 = byte3 & 0b10000000;
				bool bit5 = byte3 & 0b01000000;

				float delay = unsigned int((byte3 & 0b00111111) << 1) * 0.017f;

				printf("\nLevel(%i) Enemy(%i) Type(%s)"			\
					"\n\tByte1["								\
					PRINTFBYTEPATERN							\
					"]"											\
					"\n\tByte2["								\
					PRINTFBYTEPATERN							\
					"]"											\
					"\n\tByte3["								\
					PRINTFBYTEPATERN							\
					"]"											\
					"\n\tCol/Row[%i / %i]"						\
					"\n\tUnknown bool bits [%s ,%s ,%s | %s ]"	\
					"\n\tMoving dir = %s"						\
					"\n\tDelay[%f]"								\
					"\n\n", amountRead, enemy, enemyTypeStr, PRINTFBYTE(byte1), PRINTFBYTE(byte2), PRINTFBYTE(byte3), col, row, (bit1) ? "True" : "False", (bit2) ? "True" : "False", (bit3) ? "True" : "False", (bit4) ? "True" : "False", (bit5) ? "Left" : "Right", delay);
				enemy++;
			}
			else
			{
				printf("-------------- EndOfLevel(%i) --------------\n\n", amountRead);
				amountRead++;
				enemy = 1;
				if (amountRead != 101)
				{
					printf("------------- BeginOfLevel(%i) -------------\n", amountRead);
				}
			}
		}
	}

	readerWriter.CloseFileToWrite();
	readerWriter.CloseFileToRead();


	readerWriter.ChangeFilePath("SeperatedLevelData.dat");
	readerWriter.OpenFileToRead();
	{	// reading in the level and displaying it
		unsigned char byte = ' ';
		int amountRead = 0;
		int size = ((100 * 25 * 32) / 8);
		int level = 1;
		std::string levelStr{};
		for (int i = 0; i < size; i++)
		{
			if ((amountRead % (4 * 25)) == 0)
			{
				printf("\n------------------------ Begin of Level %i ------------------------\n", level);
			}
			readerWriter.BinaryReading(byte);
			unsigned char mask = 0b10000000;
			for (unsigned char i = 0; i < 8; i++)
			{
				(byte & mask) ? levelStr += std::string{ char(219), char(219) } : levelStr += std::string{ "  " };

				mask = mask >> 1;
			}
			amountRead++;
			if ((amountRead % (4 * 25)) == 0)
			{
				std::cout << levelStr;
				levelStr.clear();
				printf("\n------------------------- End of Level %i -------------------------\n", level);
				level++;
			}
			if ((amountRead % 4) == 0) levelStr += "\n";
		}
	}
	readerWriter.CloseFileToRead();



	std::cin.clear();
	std::cin.ignore();
	std::cin.get();
	return 0;
}