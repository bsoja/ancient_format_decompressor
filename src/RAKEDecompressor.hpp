/* Copyright (C) Teemu Suutari */

#ifndef RAKEDECOMPRESSOR_HPP
#define RAKEDECOMPRESSOR_HPP

#include "XPKDecompressor.hpp"

class RAKEDecompressor : public XPKDecompressor
{
public:
	RAKEDecompressor(uint32_t hdr,uint32_t recursionLevel,const Buffer &packedData,std::unique_ptr<XPKDecompressor::State> &state);

	virtual ~RAKEDecompressor();

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
	bool		_isRAKE=false;
	size_t		_midStreamOffset=0;

	static XPKDecompressor::Registry<RAKEDecompressor> _XPKregistration;
};

#endif
