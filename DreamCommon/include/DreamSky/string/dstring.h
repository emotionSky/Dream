#ifndef __D_STRING_H__
#define __D_STRING_H__

#include "../dream_define.h"
#include <string>

DREAM_NAMESPACE_BEGIN

// api的命名上最大可能接近 std::string 的原生命名，但是，也改进了一些原生的不合理命名。
// 类设计参考了 coost 的 fastring ，感谢大佬的开源代码。
// https://github.com/idealvin/coost/blob/master/include/co/fastring.h

class DREAMSKY_API dstring
{
public:
	dstring() noexcept;
	dstring(const char* s);
	explicit dstring(const char* s, size_t len);
	dstring(const std::string& str);
	dstring(const dstring& str);
	dstring(dstring&& str) noexcept;
	~dstring();

	dstring& operator=(dstring&& str) noexcept;
	dstring& operator=(const dstring& str);
	dstring& operator=(const std::string& str);
	dstring& operator=(const char* s);

	bool is_empty() const { return m_size == 0; }
	size_t size() const { return m_size; }
	size_t len() const { return m_size; }
	char* data() const { return m_ch; }
	char* c_str() const { return m_ch; }
	void reserve(size_t size);
	void clear();

private:
	void _reset();
	dstring& _assign(const void* p, size_t len);

private:
	char*  m_ch;
	size_t m_size;
	size_t m_capacity;
};

DREAM_NAMESPACE_END

#endif //!__D_STRING_H__