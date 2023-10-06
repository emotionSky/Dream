#include "DreamSky/string/dstring.h"
#include "DreamSky/dream_mem.h"
#include <cstring>
#include <cassert>

DREAM_NAMESPACE_BEGIN

dstring::dstring() noexcept : dstring("")
{}

dstring::dstring(const char* s) : dstring(s, strlen(s))
{}

dstring::dstring(const char* s, size_t len) : 
	m_size(len), m_capacity(len)
{
	if (m_capacity > 0)
	{
		m_ch = (char*)dreamsky::alloc(m_capacity);
		memcpy(m_ch, s, m_size);
	}
	else
	{
		m_capacity = 10;
		m_ch = (char*)dreamsky::alloc(m_capacity);
		m_ch[0] = 0;
	}
}

dstring::dstring(const std::string& str) : dstring(str.data(), str.size())
{}

dstring::dstring(const dstring& str) : dstring(str.data(), str.size())
{}

dstring::dstring(dstring&& str) noexcept :
	m_ch(str.m_ch), m_size(str.m_size), m_capacity(str.m_capacity)
{
	str.m_ch = nullptr;
	str.m_size = str.m_capacity = 0;
}

dstring::~dstring()
{
	this->_reset();
}

dstring& dstring::operator=(dstring&& str) noexcept
{
	if (this != &str)
	{
		if (m_ch)
			dreamsky::free(m_ch, m_size);
		new (this) dstring(std::move(str));
	}
	return *this;
}

dstring& dstring::operator=(const dstring& str)
{
	if (this != &str)
	{
		this->_assign(str.data(), str.size());
	}
	return *this;
}

dstring& dstring::operator=(const std::string& str)
{
	return this->_assign(str.data(), str.size());
}

dstring& dstring::operator=(const char* s)
{
	return this->_assign(s, strlen(s));
}

void dstring::reserve(size_t size)
{
	if (m_capacity < size)
	{
		m_ch = (char*)dreamsky::realloc(m_ch, m_size, size);
		assert(m_ch);
		m_capacity = size;
	}
}

void dstring::clear()
{
	m_size = 0;
	m_ch[0] = 0;
}

void dstring::_reset()
{
	if (m_ch)
	{
		free(m_ch, m_size);
		m_ch = nullptr;
	}
	m_size = 0;
	m_capacity = 0;
}

dstring& dstring::_assign(const void* p, size_t len)
{
	m_size = len;
	if (len > 0)
	{
		this->reserve(len);
		memcpy(m_ch, p, len);
	}
	return *this;
}

DREAM_NAMESPACE_END