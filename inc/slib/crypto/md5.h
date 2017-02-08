#ifndef CHECKHEADER_SLIB_CRYPTO_MD5
#define CHECKHEADER_SLIB_CRYPTO_MD5

#include "definition.h"

#include "hash.h"

/*
	MD5 - Message Digest algorithm

	Output: 128bits (16 bytes)
*/

SLIB_CRYPTO_NAMESPACE_BEGIN

class SLIB_EXPORT MD5 : public CryptoHash
{
public:
	MD5();
	
	~MD5();

public:
	// override
	void start();
	
	// override
	void update(const void* input, sl_size n);
	
	// override
	void finish(void* output);
	
public: /* common functions for CryptoHash */
	static void hash(const void* input, sl_size n, void* output);
	
	static sl_uint32 getHashSize();
	
	static void hash(const String& s, void* output);
	
	static void hash(const Memory& data, void* output);
	
	static Memory hash(const void* input, sl_size n);
	
	static Memory hash(const String& s);
	
	static Memory hash(const Memory& data);
	
	sl_uint32 getSize() const;

private:
	void _updateSection(const sl_uint8* input);

private:
	sl_size sizeTotalInput;
	sl_uint32 rdata_len;
	sl_uint8 rdata[64];
	sl_uint32 A[4];
};

SLIB_CRYPTO_NAMESPACE_END

#endif
