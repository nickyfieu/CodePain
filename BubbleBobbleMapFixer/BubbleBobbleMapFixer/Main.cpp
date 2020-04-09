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

			if (byte1 != 0b00000000)
			{
				readerWriter.BinaryReading(byte2);
				readerWriter.BinaryReading(byte3);

				readerWriter.BinaryWriting(byte1);
				readerWriter.BinaryWriting(byte2);
				readerWriter.BinaryWriting(byte3);

				byte1 = byte1 << 3;
				byte2 = byte2 << 3;

				bool IsMovingLeft = byte3 & 0b10000000;
				bool IsLookingLeft = byte3 & 0b01000000;

				printf("Level(%i) Enemy(%i)" \
					"\n{"					 \
					"\n\tCollumn["			 \
					PRINTFBYTEPATERN		 \
					"]"						 \
					"\n\tRow["				 \
					PRINTFBYTEPATERN		 \
					"]"						 \
					"\n\tExtra Data["		 \
					PRINTFBYTEPATERN		 \
					"]"						 \
					"\n}"					 \
					"\nMoving Left = %s"	 \
					"\nLooking left = %s"	 \
					"\n", amountRead, enemy, PRINTFBYTE(byte1), PRINTFBYTE(byte2), PRINTFBYTE(byte3), IsMovingLeft ? "true" : "false", IsLookingLeft ? "true" : "false");
				enemy++;
			}
			else
			{
				printf("-------------- EndOfLevel(%i) --------------\n", amountRead);
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
				if (byte & mask) std::cout << char(219) << char(219);
				else std::cout << ' ' << ' ';

				mask = mask >> 1;
			}
			amountRead++;
			if ((amountRead % (4 * 25)) == 0)
			{
				printf("\n------------------------- End of Level %i -------------------------\n", level);
				level++;
			}
			if ((amountRead % 4) == 0) std::cout << '\n';
			
			
		}
	}
	readerWriter.CloseFileToRead();



	std::cin.clear();
	std::cin.ignore();
	std::cin.get();
	return 0;
}