#include <DreamSky/dream_string.h>
#include <cstring>
#if __cplusplus >= 201103L
#include <regex>
#endif

DREAM_NAMESPACE_BEGIN

void left_delete_c(char* str, char ch)
{
	if (!str || str[0] == '\0')
		return;

	size_t len = 0;
	if (str == nullptr)
		return;

	len = strlen(str);
	while (len > 0 && str[len - 1] == ch)   //位置换一下   
		str[--len] = 0;
}

void right_delete_c(char* str, char ch)
{
	if (!str || str[0] == '\0')
		return;

	size_t len = 0;
	len = strlen(str);
	char* p_tmp = str;

	/* 如果不是直接以ch开头的，返回即可 */
	if ((*p_tmp) != ch)
		return;

	/* 找到第一个不是ch的位置 */
	while ((*p_tmp) != '\0')
	{
		if ((*p_tmp) == ch)
			p_tmp++;
		else
			break;
	}

	/* 如果字符串全部都是ch字符，字符串清空返回 */
	if ((*p_tmp) == '\0')
	{
		*str = '\0';
		return;
	}

	char* p_tmp2 = str;
	while ((*p_tmp) != '\0')
	{
		(*p_tmp2) = (*p_tmp);
		p_tmp++;
		p_tmp2++;
	}
	(*p_tmp2) = '\0';
}

void left_delete(std::string& str, char ch)
{
	if (str.empty())
		return;
	
	/* 判断是否为匹配的字符 */
	auto IsMatched = [ch](char ucs4) ->bool {
		/**
		 * 0x20	space 空格
		 * 0x09 HT - horizontal tab 水平制表符
		 * 0x0A LF - line feed 换行键
		 * 0x0B VT - vertical tab 垂直制表符
		 * 0x0C FF - form feed 换页键
		 * 0x0D CR - carriage return 回车键
		 */
		//return ucs4 == 0x20 || (ucs4 <= 0x0d && ucs4 >= 0x09);
		return ucs4 == ch;
	};

	auto begin = str.begin();
	auto end = str.end();

	while ((begin < end) && IsMatched(*begin))
		begin++;
	str = std::string(begin, end);
}

void right_delete(std::string& str, char ch)
{
	if (str.empty())
		return;

	/* 判断是否为匹配的字符 */
	auto IsMatched = [ch](char ucs4) ->bool {
		/**
		 * 0x20	space 空格
		 * 0x09 HT - horizontal tab 水平制表符
		 * 0x0A LF - line feed 换行键
		 * 0x0B VT - vertical tab 垂直制表符
		 * 0x0C FF - form feed 换页键
		 * 0x0D CR - carriage return 回车键
		 */
		 //return ucs4 == 0x20 || (ucs4 <= 0x0d && ucs4 >= 0x09);
		return ucs4 == ch;
	};

	auto begin = str.begin();
	auto end = str.end();
	while ((begin < end) && IsMatched(end[-1]))
		--end;

	str = std::string(begin, end);
}

void split_string(const std::string& str, const std::string& sp, std::vector<std::string>& res)
{
#ifdef USE_C_IMPLEMENT
	size_t len = str.size();
	char* buf = new char[len + 1];
	memcpy(buf, str.c_str(), len + 1);
	char* p = strtok(buf, sp.c_str());
	while (p)
	{
		res.push_back(p);
		p = strtok(nullptr, sp.c_str());
	}
	delete[] buf;
#else
#if __cplusplus < 201103L
	size_t pre_pos = 0;
	size_t pos = str.find(sp, pre_pos);

	while (pos != str.npos)
	{
		res.push_back(str.substr(pre_pos, pos - pre_pos));
		pre_pos = pos + 1;
		pos = str.find(sp, pre_pos);
	}

	if (pre_pos < str.size())
	{
		res.push_back(str.substr(pre_pos, str.size() - pre_pos));
	}
#else
	std::regex re(sp);
	std::sregex_token_iterator begin{ str.begin(), str.end(), re, -1 }, end;
	res.assign(begin, end);
#endif
#endif
}

DREAM_NAMESPACE_END