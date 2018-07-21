#pragma once
#include <string>
#include <cstdint>

class KTXImage
{
public:
	~KTXImage();

	KTXImage(const std::string& path);

	uint8_t* pixelData;

	uint32_t width = -1;
	uint32_t height = -1;
	uint32_t depth = -1;
	uint32_t mips = -1;
	uint32_t internalFormat = -1;
	uint32_t baseFormat = -1;
	uint8_t channels = 4;

private:
	KTXImage();
	bool isValid(const uint8_t* startHeader);

	void processHeader(const uint8_t* headerData);

	enum Endianness
	{
		BIG_ENDIAN,
		LITTLE_ENDIAN
	};

	uint32_t imageSize;

	uint32_t glType;
	uint32_t glTypeSize;
	uint32_t glFormat;
	uint32_t glInternalFormat;
	uint32_t glBaseInternalFormat;
	uint32_t pixelWidth;
	uint32_t pixelHeight;
	uint32_t pixelDepth;
	uint32_t nArrayElements;
	uint32_t nFaces;
	uint32_t nMips;
	uint32_t nKeyValueDataBytes;
	uint32_t keyAndValueByteSize;
	
	uint8_t* keys;
	uint8_t* values;

	Endianness endianness;
};

