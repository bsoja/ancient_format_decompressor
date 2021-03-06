/* Copyright (C) Teemu Suutari */

#include "TPWMDecompressor.hpp"

bool TPWMDecompressor::detectHeader(uint32_t hdr)
{
	return hdr==FourCC('TPWM');
}

std::unique_ptr<Decompressor> TPWMDecompressor::create(const Buffer &packedData,bool exactSizeKnown)
{
	return std::make_unique<TPWMDecompressor>(packedData);
}

TPWMDecompressor::TPWMDecompressor(const Buffer &packedData) :
	_packedData(packedData)
{
	if (packedData.size()<12) return;
	uint32_t hdr;
	if (!packedData.readBE(0,hdr)) return;
	if (!detectHeader(hdr)) return;

	if (!packedData.readBE(4,_rawSize)) return;
	if (!_rawSize) return;
	if (_rawSize>getMaxRawSize()) return;
	_isValid=true;
}

TPWMDecompressor::~TPWMDecompressor()
{
	// nothing needed
}

bool TPWMDecompressor::isValid() const
{
	return _isValid;
}

bool TPWMDecompressor::verifyPacked() const
{
	// no checksum whatsoever
	return _isValid;
}

bool TPWMDecompressor::verifyRaw(const Buffer &rawData) const
{
	// no CRC
	return _isValid;
}

const std::string &TPWMDecompressor::getName() const
{
	if (!_isValid) return Decompressor::getName();
	static std::string name="TPWM: Turbo Packer";
	return name;
}

size_t TPWMDecompressor::getPackedSize() const
{
	// No packed size in the stream :(
	// After decompression, we can tell how many bytes were actually used
	if (!_isValid) return 0;
	return _decompressedPackedSize;
}

size_t TPWMDecompressor::getRawSize() const
{
	if (!_isValid) return 0;
	return _rawSize;
}

bool TPWMDecompressor::decompress(Buffer &rawData)
{
	if (!_isValid || rawData.size()<_rawSize) return false;

	// Stream reading
	bool streamStatus=true;
	const uint8_t *bufPtr=_packedData.data();
	size_t bufOffset=8;
	size_t packedSize=_packedData.size();
	uint8_t bufBitsContent=0;
	uint8_t bufBitsLength=0;

	auto readBit=[&]()->uint8_t
	{
		uint8_t ret=0;
		if (!bufBitsLength)
		{
			if (bufOffset>=packedSize)
			{
				streamStatus=false;
				return 0;
			}
			bufBitsContent=bufPtr[bufOffset++];
			bufBitsLength=8;
		}
		ret=bufBitsContent>>7;
		bufBitsContent<<=1;
		bufBitsLength--;
		return ret;
	};

	auto readByte=[&]()->uint8_t
	{
		if (!streamStatus || bufOffset>=packedSize)
		{
			streamStatus=false;
			return 0;
		}
		return bufPtr[bufOffset++];
	};

	uint8_t *dest=rawData.data();
	size_t destOffset=0;

	while (streamStatus)
	{
		if (destOffset==_rawSize) break;
		if (readBit())
		{
			uint8_t byte1=readByte();
			uint8_t byte2=readByte();
			uint32_t distance=(uint32_t(byte1&0xf0)<<4)|byte2;
			uint32_t count=uint32_t(byte1&0xf)+3;
			if (streamStatus && distance && distance<=destOffset)
			{
				for (uint32_t i=0;i<count;i++)
				{
					dest[destOffset]=dest[destOffset-distance];
					destOffset++;
					if (destOffset==_rawSize) break;
				}
			} else {
				streamStatus=false;
			}
		} else {
			dest[destOffset++]=readByte();
		}
	}

	bool ret=(streamStatus && destOffset==_rawSize);
	if (ret) _decompressedPackedSize=bufOffset;
	return ret;
}

Decompressor::Registry<TPWMDecompressor> TPWMDecompressor::_registration;
