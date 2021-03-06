/* Copyright (C) Teemu Suutari */

#ifndef CYB2DECODER_HPP
#define CYB2DECODER_HPP

#include "XPKDecompressor.hpp"

class CYB2Decoder : public XPKDecompressor
{
public:
	CYB2Decoder(uint32_t hdr,uint32_t recursionLevel,const Buffer &packedData,std::unique_ptr<XPKDecompressor::State> &state);

	virtual ~CYB2Decoder();

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
	uint32_t	_blockHeader;

	static XPKDecompressor::Registry<CYB2Decoder> _XPKregistration;
};

#endif
