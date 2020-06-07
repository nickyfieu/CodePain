#pragma once
#include <iostream>
#include <fstream>
#include <vector>

class BinaryReaderWriter final
{
public:
	BinaryReaderWriter() = default;
	~BinaryReaderWriter() = default;

	BinaryReaderWriter(const BinaryReaderWriter& other) = delete;
	BinaryReaderWriter(BinaryReaderWriter&& other) = delete;
	BinaryReaderWriter& operator=(const BinaryReaderWriter& other) = delete;
	BinaryReaderWriter& operator=(BinaryReaderWriter&& other) = delete;

	BinaryReaderWriter(const std::string& filePath)
		:m_FilePath{ filePath }
	{

	}

	inline bool OpenFileToWrite() noexcept
	{
		m_Write.open(m_FilePath, std::ios::out | std::ios::binary);
		return m_Write.is_open();
	}

	template<class T>
	inline void BinaryWriting(const std::vector<T>& data) noexcept
	{
		if (m_Write.is_open()) //to be safe check if it can open the file
		{
			size_t size = data.size();
			m_Write.write((char*)&size, sizeof(size));
			for (size_t i{}; i < size; i++)
			{
				BinaryWriting(data[i]);
			}
		}
	}

	template<class T>
	inline void BinaryWriting(const T& data) noexcept
	{
		if (m_Write.is_open()) //to be safe check if it can open the file
		{
			m_Write.write((const char*)&data, sizeof(T));
		}
	}

	template<>
	inline void BinaryWriting(const std::string& data) noexcept
	{
		if (m_Write.is_open()) //to be safe check if it can open the file
		{
			size_t size{ data.size() }; //in the instance of strings we need to store the size and use the size to store the string wich is a essentialy an array of chars
			m_Write.write((char*)&size, sizeof(size));
			m_Write.write((char*)&data[0], size);
		}
	}

	inline void CloseFileToWrite() noexcept
	{
		m_Write.close();
	}

	inline bool OpenFileToRead() noexcept
	{
		m_Read.open(m_FilePath, std::ios::in | std::ios::binary);
		return m_Read.is_open();
	}

	template<class T>
	inline void BinaryReading(T& data) noexcept
	{
		if (m_Read.is_open()) //to be safe check if it can open the file
		{
			m_Read.read((char*)&data, sizeof(T));
		}
	}

	template<class T>
	inline void BinaryReading(std::vector<T>& data) noexcept
	{
		if (m_Read.is_open()) //to be safe check if it can open the file
		{
			size_t size;
			m_Read.read((char*)&size, sizeof(size));
			for (size_t i{}; i < size; i++)
			{
				T temp;
				BinaryReading(temp);
				data.push_back(temp);
			}
		}
	}

	template<>
	inline void BinaryReading(std::string& data) noexcept
	{
		if (m_Read.is_open()) //to be safe check if it can open the file
		{
			size_t size{}; //in the instance of strings we need to store the size and use the size to store the string wich is a essentialy an array of chars
			m_Read.read((char*)&size, sizeof(size));
			data.resize(size);
			m_Read.read((char*)&data[0], size);
		}
	}

	inline void CloseFileToRead() noexcept
	{
		m_Read.close();
	}

	inline void ChangeFilePath(const std::string& filePath) noexcept
	{
		m_FilePath = filePath;
	}

	// returns negative 1 when file is not open
	inline int AmountCharsInFile() noexcept
	{
		if (!m_Read.is_open())
			return -1;

		int amountChars{};
		char character{};

		// tellg works here as we are reading a binary file!
		int currentPos = (int)m_Read.tellg();
		ResetReadPos();
		while (!m_Read.eof())
		{
			m_Read.read(&character, 1);
			amountChars++;
		}
		ResetReadPos();
		m_Read.seekg(currentPos, std::ios::beg);

		return amountChars - 1;
	}

	inline void ResetReadPos() noexcept
	{
		m_Read.clear();
		m_Read.seekg(0, std::ios::beg);
	}

	std::ofstream m_Write;
	std::ifstream m_Read;
private:
	std::string m_FilePath;
};