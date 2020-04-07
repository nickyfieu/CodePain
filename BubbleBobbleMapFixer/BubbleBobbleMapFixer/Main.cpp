#include "BinaryReaderWriter.h"
#include <iostream>
#include <string>
#include <fstream>

int main()
{
	BinaryReaderWriter readerWriter{"leveldata.dat"};

	readerWriter.OpenFileToRead();
	readerWriter.ChangeFilePath("FixedLevelData.dat");
	readerWriter.OpenFileToWrite();

	unsigned char byte = ' ';
	unsigned char mask = 0b00000011;
	int amountRead{0};
	int size = ((100 * 25 * 32) / 8);
	for (int i = 0 ; i < size; i ++)
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
	readerWriter.CloseFileToRead();
	readerWriter.OpenFileToRead();
	amountRead = 0;
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
	readerWriter.CloseFileToRead();
	std::cin.clear();
	std::cin.ignore();
	std::cin.get();
	return 0;
}