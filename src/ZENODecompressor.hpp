/* Copyright (C) Teemu Suutari */

#ifndef ZENODECOMPRESSOR_HPP
#define ZENODECOMPRESSOR_HPP

#include "XPKDecompressor.hpp"

class ZENODecompressor : public XPKDecompressor
{
public:
	ZENODecompressor(uint32_t hdr,uint32_t recursionLevel,const Buffer &packedData,std::unique_ptr<XPKDecompressor::State> &state);

	virtual ~ZENODecompressor();

	virtual bool isValid() const override final;
	virtual bool verifyPacked() const override final;
	virtual bool verifyRaw(const Buffer &rawData) const override final;

	virtual const std::string &getSubName() const override final;

	virtual bool decompress(Buffer &rawData,const Buffer &previousData) override final;

	static bool detectHeaderXPK(uint32_t hdr);
	static std::unique_ptr<XPKDecompressor> create(uint32_t hdr,uint32_t recursionLevel,const Buffer &packedData,std::unique_ptr<XPKDecompressor::State> &state);

private:
	const Buffer	&_packedData;

	bool		_isValid=false;
	uint32_t	_maxBits=0;
	size_t		_startOffset=0;

	static XPKDecompressor::Registry<ZENODecompressor> _XPKregistration;
};

#endif
