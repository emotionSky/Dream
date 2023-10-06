#ifndef __DREAM_ERROR_H__
#define __DREAM_ERROR_H__

#include "string/dstring.h"

DREAM_NAMESPACE_BEGIN

class DREAMSKY_API DreamError
{
public:
	explicit DreamError();
	~DreamError();

	int GetSocketError() const;
	const char* GetErrorString();

#if __cplusplus >= 201103L
	DreamError& operator=(const DreamError&) = delete;
#else
private:
	DreamError& operator=(const DreamError&) { return *this; }
#endif

private:
	int   m_err;
	char* m_pStr;
};

DREAM_NAMESPACE_END

#endif //!__DREAM_ERROR_H__