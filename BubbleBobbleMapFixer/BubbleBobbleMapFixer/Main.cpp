#include "BinaryReaderWriter.h"
#include <iostream>
#include <string>
#include <fstream>

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
		std::cout << amountRead << '\n';
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
				byte3 = byte3 << 3;

				printf("Level(%i) Enemy(%i) Collumn[%u] Row[%u] ???[%u]\n", amountRead, enemy, byte1, byte2, byte3);
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
		for (int i = 0; i < size; i++)
		{
			readerWriter.BinaryReading(byte);
			unsigned char mask = 0b10000000;
			for (unsigned char i = 0; i < 8; i++)
			{
				if (byte & mask) std::cout << "1";
				else std::cout << "0";

				mask = mask >> 1;
			}
			amountRead++;
			if ((amountRead % 4) == 0) std::cout << '\n';
		}
	}
	readerWriter.CloseFileToRead();



	std::cin.clear();
	std::cin.ignore();
	std::cin.get();
	return 0;
}