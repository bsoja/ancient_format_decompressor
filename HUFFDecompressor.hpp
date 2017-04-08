/* Copyright (C) Teemu Suutari */

#ifndef HUFFDECOMPRESSOR_HPP
#define HUFFDECOMPRESSOR_HPP

#include "XPKDecompressor.hpp"

class HUFFDecompressor : public XPKDecompressor
{
public:
	HUFFDecompressor(uint32_t hdr,const Buffer &packedData,std::unique_ptr<XPKDecompressor::State> &state);

	virtual ~HUFFDecompressor();

	virtual bool isValid() const override final;
	virtual bool verifyPacked() const override final;
	virtual bool verifyRaw(const Buffer &rawData) const override final;

	virtual const std::string &getSubName() const override final;

	virtual bool decompress(Buffer &rawData,const Buffer &previousData) override final;

	static bool detectHeaderXPK(uint32_t hdr);

private:
	const Buffer &_packedData;
	bool		_isValid=false;
};

#endif
