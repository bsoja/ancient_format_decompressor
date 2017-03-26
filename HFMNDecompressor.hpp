/* Copyright (C) Teemu Suutari */

#ifndef HFMNDECOMPRESSOR_HPP
#define HFMNDECOMPRESSOR_HPP

#include "XPKDecompressor.hpp"

class HFMNDecompressor : public XPKDecompressor
{
public:
	HFMNDecompressor(uint32_t hdr,const Buffer &packedData,std::unique_ptr<XPKDecompressor::State> &state);

	virtual ~HFMNDecompressor();

	virtual bool isValid() const override final;
	virtual bool verifyPacked() const override final;
	virtual bool verifyRaw(const Buffer &rawData) const override final;

	virtual const std::string &getSubName() const override final;

	virtual bool decompress(Buffer &rawData) override final;

	static bool detectHeaderXPK(uint32_t hdr);

private:
	const Buffer &_packedData;

	bool		_isValid=false;

	size_t		_headerSize;
	size_t		_rawSize;
};

#endif