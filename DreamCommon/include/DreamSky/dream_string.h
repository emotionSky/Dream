#ifndef __DREAM_STRING_H__
#define __DREAM_STRING_H__

#include "string/dstring.h"
#include <vector>

DREAM_NAMESPACE_BEGIN

/**
 * @brief 删除字符串左边的特定字符
 * @param[in] str 需要操作的字符串
 * @param[in] ch  需要删除的特定字符
 * @return void
 */
DREAMSKY_API void left_delete_c(char* str, char ch);

/**
 * @brief 删除字符串右边的特定字符
 * @param[in] str 需要操作的字符串
 * @param[in] ch  需要删除的特定字符
 * @return void
 */
DREAMSKY_API void right_delete_c(char* str, char ch);

#define left_trim_c(str) left_delete_c(str, ' ')
#define right_trim_c(str) right_delete_c(str, ' ')
#define both_trim_c(str)       do  \
{                                  \
	left_delete_c(str, ' ');         \
	right_delete_c(str, ' ');      \
} while (false);


DREAMSKY_API void left_delete(std::string& str, char ch);

DREAMSKY_API void right_delete(std::string& str, char ch);

#define left_trim(str) left_delete(str, ' ')
#define right_trim(str) right_delete(str, ' ')
#define both_trim(str)      do     \
{                                  \
	left_delete(str, ' ');         \
	right_delete(str, ' ');        \
} while (false);

DREAMSKY_API void split_string(const std::string & str, const std::string & sp, std::vector<std::string>&res);

#define split_trim(str, res) do    \
{                                  \
	split_string(str, " ", res);   \
} while (false);

DREAM_NAMESPACE_END

#endif //!__DREAM_STRING_H__