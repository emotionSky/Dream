#ifndef __DREAM_JSON_H__
#define __DREAM_JSON_H__

/**
 * 这种封装处理主要是为了避免在外部还需要导入rapidjson的库文件。
 * 并且，这样处理可以在Windows仍然适用。
 */

#include <DreamSky/dream_define.h>
#include <cstdint>

DREAM_NAMESPACE_BEGIN

enum JsonType
{
	JsonTypeNull = 0,
	JsonTypeFalse,
	JsonTypeTrue,
	JsonTypeObject,
	JsonTypeArray,
	JsonTypeString,
	JsonTypeNumber,
};

class JsonDocument;
class DREAMSKY_API JsonAllocator
{
private:
	class _JsonAllocator;

public:
	explicit JsonAllocator(JsonDocument& doc);
	~JsonAllocator();

	_JsonAllocator* GetAllocator();

private:
	_JsonAllocator* m_pAlloc;
};

class JsonObject;
// 这个类的设计可能有一些不太易用，暂时没有想到更优的处理办法
class DREAMSKY_API PackJsonObject
{
public:
	explicit PackJsonObject();
	~PackJsonObject();
	PackJsonObject& operator=(PackJsonObject& sub) noexcept;

	JsonObject* GetObject();
	void SetObject(JsonObject* pObj);
private:
	JsonObject* m_pObj;
};

class DREAMSKY_API JsonObject
{
public:
	class _JsonObject;

public:
	explicit JsonObject(JsonType type);
	explicit JsonObject(_JsonObject* pObj);
	JsonObject& operator=(const JsonObject&) = delete;
	bool operator==(const JsonObject&) = delete;

	~JsonObject();
	_JsonObject* GetObject();

	/* get */
	// 需要确保这是个 JsonTypeObject/JsonTypeArray，否则接口挂掉
	bool IsObjectEmpty() const;
	// 需要确保这是个 JsonTypeObject/JsonTypeArray，否则接口挂掉
	uint32_t GetMemberCount() const;
	// 需要确保这是个 JsonTypeArray，否则接口挂掉
	uint32_t GetSize() const;
	// 需要确保这是个 JsonTypeArray，否则接口挂掉
	PackJsonObject operator[](uint32_t index);
	int32_t GetInt(const char* key, int32_t default_val) const;
	int32_t GetInt(int32_t default_val) const;
	double GetDouble(const char* key, double default_val) const;
	const char* GetString(const char* key, const char* default_val) const;
	const char* GetString(const char* default_val) const;
	PackJsonObject GetObject(const char* key) const;
	
	/* set */
	void SetNull(const char* key, JsonAllocator& alloc);
	bool SetBool(const char* key, bool val, JsonAllocator& alloc);
	bool SetInt(const char* key, int32_t val, JsonAllocator& alloc);
	bool SetUint(const char* key, uint32_t val, JsonAllocator& alloc);
	bool SetInt64(const char* key, int64_t val, JsonAllocator& alloc);
	bool SetUint64(const char* key, uint64_t val, JsonAllocator& alloc);
	bool SetDouble(const char* key, double val, JsonAllocator& alloc);
	bool SetString(const char* key, const char* val, JsonAllocator& alloc);
	bool SetObject(const char* key, JsonObject& obj, JsonAllocator& alloc);
	bool SetArrayInt(int32_t val, JsonAllocator& alloc);
	bool SetArrayString(const char* val, JsonAllocator& alloc);
	bool SetArrayObject(JsonObject& obj, JsonAllocator& alloc);

	/* remove */
	void RemoveChild(const char* key);

private:
	_JsonObject* m_pObj;
};

class DREAMSKY_API JsonDocument
{
private:
	class _JsonDocument;

public:
	explicit JsonDocument();
	~JsonDocument();

	_JsonDocument* GetDocument();
	JsonAllocator& GetAllocator();

	/* parse */
	bool ParseFromString(const char* json);
	const char* ParseToString(bool bEasy2Read = true);

	/* get */
	bool GetBool(const char* key, bool default_val) const;
	int32_t GetInt(const char* key, int32_t default_val) const;
	double GetDouble(const char* key, double default_val) const;
	const char* GetString(const char* key, const char* default_val) const;
	// 获取不到，则里面的 m_pObj 为空
	// 数组和 object 都通过这个接口获取
	PackJsonObject GetObject(const char* key) const;

	/* set */
	void SetObject();
	void SetNull(const char* key);
	bool SetBool(const char* key, bool val);
	bool SetInt(const char* key, int32_t val);
	bool SetUint(const char* key, uint32_t val);
	bool SetInt64(const char* key, int64_t val);
	bool SetUint64(const char* key, uint64_t val);
	bool SetDouble(const char* key, double val);
	bool SetString(const char* key, const char* val);
	bool SetObject(const char* key, JsonObject& obj);

private:
	char*          m_pBuf;
	_JsonDocument* m_pDoc;
	JsonAllocator  m_alloc;
};

//JsonDocument jsdoc;

DREAM_NAMESPACE_END

#endif //!__DREAM_JSON_H__