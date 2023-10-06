#ifndef __PIPE_OPERATION_H__
#define __PIPE_OPERATION_H__

#include <DreamSky/dream_define.h>
#include <DreamSky/string/dstring.h>

DREAM_NAMESPACE_BEGIN

class DREAMSKY_API DreamPipe
{
public:
	DreamPipe();
	~DreamPipe();

	bool CreatePipe(bool is_named = false, const char* name = nullptr);
	int WriteData(const char* buf, int len);
	int ReadData(char* buf, int size);
	void CloseWritePipe();
	void CloseReadPipe();

#if defined(_WIN32) || defined(_WINDLL)
	static d_handle GetStdReadPipe();
	static void CloseStdReadPipe(d_handle hdl);
#endif

private:
#if defined(_WIN32) || defined(_WINDLL)
	d_handle m_writeHdl;
	d_handle m_readHdl;
#else
	int m_writeFd;
	int m_readFd;
#endif
};

DREAM_NAMESPACE_END

#endif //!__PIPE_OPERATION_H__