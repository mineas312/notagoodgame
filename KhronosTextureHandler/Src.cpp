#include "stdafx.h"
#include "KTXImage.hpp"

#ifndef WIN32
extern "C" uint32_t _SwapBytes32bit(uint32_t a);
extern "C" uint64_t _SwapBytes64bit(uint64_t a);
#else
uint32_t _SwapBytes32bit(uint32_t a)
{
	__asm {
		mov eax, a
		bswap eax
		mov a, eax
	}
	return a;
}
#endif

__declspec(align(16)) constexpr uint8_t _Identifier1[16]{ 0xAB, 0x4B, 0x54, 0x58, 0x20, 0x31, 0x31, 0xBB, 0x0D, 0x0A, 0x1A, 0x0A,
	0x01, 0x02, 0x03, 0x04 };
__declspec(align(16)) constexpr uint8_t _Identifier2[16]{ 0xAB, 0x4B, 0x54, 0x58, 0x20, 0x31, 0x31, 0xBB, 0x0D, 0x0A, 0x1A, 0x0A,
	0x04, 0x03, 0x02, 0x01};

KTXImage::KTXImage()
{}

KTXImage::~KTXImage()
{
	delete[] pixelData;
}

KTXImage::KTXImage(const std::string & path)
{
	std::ifstream ktxFile(path, std::ios::binary | std::ios::beg);

	if (!ktxFile.good())
		return;

	// Read header
	__declspec(align(16)) uint8_t* header = new uint8_t[64];
	ktxFile.read(reinterpret_cast<char*>(header), 64);

	// Verify and process header
	if (!isValid(header))
		return;
	processHeader(header);

	// Skip arbitrary KV data
	ktxFile.readsome(nullptr, nKeyValueDataBytes);

	// Read image size
	ktxFile.readsome((char*)&imageSize, 4);
	imageSize = endianness == BIG_ENDIAN ? _SwapBytes32bit(imageSize) : imageSize;

	// Read compressed data
	pixelData = new uint8_t[imageSize];
	ktxFile.readsome(reinterpret_cast<char*>(pixelData), imageSize);
}

bool KTXImage::isValid(const uint8_t* startHeader)
{
	const __m128i sHeader = _mm_load_si128(reinterpret_cast<const __m128i*>(startHeader));
	const __m128i IDP1 = _mm_load_si128(reinterpret_cast<const __m128i*>(_Identifier1));
	const __m128i IDP2 = _mm_load_si128(reinterpret_cast<const __m128i*>(_Identifier2));

	const __m128i ResultBE = _mm_cmpeq_epi8(sHeader, IDP1);
	const __m128i ResultLE = _mm_cmpeq_epi8(sHeader, IDP2);

	const int maskBE = uint32_t(_mm_movemask_epi8(ResultBE));
	const int maskLE = uint32_t(_mm_movemask_epi8(ResultLE));

	if (maskBE == 0x0000FFFF)
	{
		endianness = LITTLE_ENDIAN;
		return true;
	}
	else if (maskLE == 0x0000FFFF)
	{
		endianness = BIG_ENDIAN;
		return true;
	}
	else
	{
		return false;
	}
}

void KTXImage::processHeader(const uint8_t * headerData)
{
	if (endianness == LITTLE_ENDIAN)
	{
		std::memcpy(&glType, reinterpret_cast<const void*>((headerData + 16)), 4);
		std::memcpy(&glTypeSize, reinterpret_cast<const void*>(headerData + 20), 4);
		std::memcpy(&glFormat, reinterpret_cast<const void*>(headerData + 24), 4);
		std::memcpy(&glInternalFormat, reinterpret_cast<const void*>(headerData + 28), 4);
		std::memcpy(&glBaseInternalFormat, reinterpret_cast<const void*>(headerData + 32), 4);
		std::memcpy(&pixelWidth, reinterpret_cast<const void*>(headerData + 36), 4);
		std::memcpy(&pixelHeight, reinterpret_cast<const void*>(headerData + 40), 4);
		std::memcpy(&pixelDepth, reinterpret_cast<const void*>(headerData + 44), 4);
		std::memcpy(&nArrayElements, reinterpret_cast<const void*>(headerData + 48), 4);
		std::memcpy(&nFaces, reinterpret_cast<const void*>(headerData + 52), 4);
		std::memcpy(&nMips, reinterpret_cast<const void*>(headerData + 56), 4);
		std::memcpy(&nKeyValueDataBytes, reinterpret_cast<const void*>(headerData + 60), 4);

		internalFormat = glInternalFormat;
		baseFormat = glBaseInternalFormat;
		width = pixelWidth;
		height = pixelHeight;
		depth = pixelDepth;
		mips = nMips;
	}
	else
	{
		std::memcpy(&glType, reinterpret_cast<const void*>((headerData + 16)), 4);
		std::memcpy(&glTypeSize, reinterpret_cast<const void*>(headerData + 20), 4);
		std::memcpy(&glFormat, reinterpret_cast<const void*>(headerData + 24), 4);
		std::memcpy(&glInternalFormat, reinterpret_cast<const void*>(headerData + 28), 4);
		std::memcpy(&glBaseInternalFormat, reinterpret_cast<const void*>(headerData + 32), 4);
		std::memcpy(&pixelWidth, reinterpret_cast<const void*>(headerData + 36), 4);
		std::memcpy(&pixelHeight, reinterpret_cast<const void*>(headerData + 40), 4);
		std::memcpy(&pixelDepth, reinterpret_cast<const void*>(headerData + 44), 4);
		std::memcpy(&nArrayElements, reinterpret_cast<const void*>(headerData + 48), 4);
		std::memcpy(&nFaces, reinterpret_cast<const void*>(headerData + 52), 4);
		std::memcpy(&nMips, reinterpret_cast<const void*>(headerData + 56), 4);
		std::memcpy(&nKeyValueDataBytes, reinterpret_cast<const void*>(headerData + 60), 4);

		glType = _SwapBytes32bit(glType);
		glTypeSize = _SwapBytes32bit(glTypeSize);
		glFormat = _SwapBytes32bit(glFormat);
		glInternalFormat = _SwapBytes32bit(glInternalFormat);
		glBaseInternalFormat = _SwapBytes32bit(glBaseInternalFormat);
		pixelWidth = _SwapBytes32bit(pixelWidth);
		pixelHeight = _SwapBytes32bit(pixelHeight);
		pixelDepth = _SwapBytes32bit(pixelDepth);
		nArrayElements = _SwapBytes32bit(nArrayElements);
		nFaces = _SwapBytes32bit(nFaces);
		nMips = _SwapBytes32bit(nMips);
		nKeyValueDataBytes = _SwapBytes32bit(nKeyValueDataBytes);

		internalFormat = glInternalFormat;
		baseFormat = glBaseInternalFormat;
		width = pixelWidth;
		height = pixelHeight;
		depth = pixelDepth;
		mips = nMips;
	}
}
