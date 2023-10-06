#include "DreamSky/dream_json.h"
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>

DREAM_NAMESPACE_BEGIN

rapidjson::Type rapid_type[] =
{
	rapidjson::kNullType,
	rapidjson::kFalseType,
	rapidjson::kTrueType,
	rapidjson::kObjectType,
	rapidjson::kArrayType,
	rapidjson::kStringType,
	rapidjson::kNumberType,
};

bool IsValidChildType(rapidjson::Value& val, const char* key, rapidjson::Type type)
{
	if (/*val.IsObject() && */val.HasMember(key) && !val[key].IsNull())
	{
		rapidjson::Value& cur = val[key];
		switch (type)
		{
		case rapidjson::kFalseType:
			return cur.IsFalse();
			break;
		case rapidjson::kTrueType:
			return cur.IsTrue();
			break;
		case rapidjson::kObjectType:
			return cur.IsObject();
			break;
		case rapidjson::kArrayType:
			return cur.IsArray();
			break;
		case rapidjson::kStringType:
			return cur.IsString();
			break;
		case rapidjson::kNumberType:
			return (cur.IsInt() || cur.IsInt64() || cur.IsDouble() || cur.IsUint() || cur.IsUint64());
			break;
		default:
			break;
		}
	}
	return false;
}

class JsonAllocator::_JsonAllocator
{
public:
	explicit _JsonAllocator(rapidjson::Document::AllocatorType& alloc) :
		m_alloc(alloc)
	{}
	~_JsonAllocator() = default;

	rapidjson::Document::AllocatorType& m_alloc;
};

class JsonObject::_JsonObject
{
public:
	explicit _JsonObject(rapidjson::Type type) :
		m_bRelease(true),
		m_pVal(new rapidjson::Value(type))
	{
	}

	explicit _JsonObject(rapidjson::Value* pVal) :
		m_bRelease(false),
		m_pVal(pVal)
	{
	}

	~_JsonObject()
	{
		if (m_bRelease && m_pVal)
		{
			delete m_pVal;
		}
		m_pVal = nullptr;
	}

	bool              m_bRelease;
	rapidjson::Value* m_pVal;
};

class JsonDocument::_JsonDocument
{
public:
	explicit _JsonDocument() = default;
	~_JsonDocument() = default;

	rapidjson::Document m_doc;
};

JsonAllocator::JsonAllocator(JsonDocument& doc)
{
	m_pAlloc = new _JsonAllocator(doc.GetDocument()->m_doc.GetAllocator());
}

JsonAllocator::~JsonAllocator()
{
	if (m_pAlloc)
	{
		delete m_pAlloc;
		m_pAlloc = nullptr;
	}
}

JsonAllocator::_JsonAllocator* JsonAllocator::GetAllocator()
{
	return m_pAlloc;
}

PackJsonObject::PackJsonObject() :
	m_pObj(nullptr)
{
}

PackJsonObject::~PackJsonObject()
{
	if (m_pObj)
	{
		delete m_pObj;
		m_pObj = nullptr;
	}
}

PackJsonObject& PackJsonObject::operator=(PackJsonObject& sub) noexcept
{
	if (this != &sub)
	{
		if (m_pObj)
			delete m_pObj;
		m_pObj = sub.m_pObj;
		m_pObj = nullptr;
	}
	return *this;
}

JsonObject* PackJsonObject::GetObject()
{
	return m_pObj;
}

void PackJsonObject::SetObject(JsonObject* pObj)
{
	if (m_pObj)
		delete m_pObj;
	m_pObj = pObj;
}

JsonObject::JsonObject(JsonType type)
{
	m_pObj = new _JsonObject(rapid_type[type]);
}

JsonObject::JsonObject(_JsonObject* pObj) :
	m_pObj(pObj)
{
}

JsonObject::~JsonObject()
{
	if (m_pObj)
	{
		delete m_pObj;
		m_pObj = nullptr;
	}
}

JsonObject::_JsonObject* JsonObject::GetObject()
{
	return m_pObj;
}

bool JsonObject::IsObjectEmpty() const
{
	auto& my_val = *(m_pObj->m_pVal);
	return my_val.ObjectEmpty();
}

uint32_t JsonObject::GetMemberCount() const
{
	auto& my_val = *(m_pObj->m_pVal);
	return my_val.MemberCount();
}

uint32_t JsonObject::GetSize() const
{
	auto& my_val = *(m_pObj->m_pVal);
	return my_val.Size();
}

PackJsonObject JsonObject::operator[](uint32_t index)
{
	PackJsonObject subor;
	auto& my_val = *(m_pObj->m_pVal);
	_JsonObject* pObj = new _JsonObject(&my_val[index]);
	subor.SetObject(new JsonObject(pObj));
	return subor;
}

int32_t JsonObject::GetInt(const char* key, int32_t default_val) const
{
	auto& my_val = *(m_pObj->m_pVal);
	if (IsValidChildType(my_val, key, rapidjson::kNumberType))
	{
		return my_val[key].GetInt();
	}
	return default_val;
}

int32_t JsonObject::GetInt(int32_t default_val) const
{
	auto& my_val = *(m_pObj->m_pVal);
	if (my_val.IsInt())
	{
		return my_val.GetInt();
	}
	return default_val;
}

double JsonObject::GetDouble(const char* key, double default_val) const
{
	auto& my_val = *(m_pObj->m_pVal);
	if (IsValidChildType(my_val, key, rapidjson::kNumberType))
	{
		return my_val[key].GetDouble();
	}
	return default_val;
}

const char* JsonObject::GetString(const char* key, const char* default_val) const
{
	auto& my_val = *(m_pObj->m_pVal);
	if (IsValidChildType(my_val, key, rapidjson::kStringType))
	{
		return my_val[key].GetString();
	}
	return default_val;
}

const char* JsonObject::GetString(const char* default_val) const
{
	auto& my_val = *(m_pObj->m_pVal);
	if (my_val.IsString())
	{
		return my_val.GetString();
	}
	return default_val;
}

PackJsonObject JsonObject::GetObject(const char* key) const
{
	PackJsonObject obj;
	auto& my_val = *(m_pObj->m_pVal);
	if (IsValidChildType(my_val, key, rapidjson::kObjectType) ||
		IsValidChildType(my_val, key, rapidjson::kArrayType))
	{
		JsonObject* pObj = new JsonObject(new JsonObject::_JsonObject(&my_val[key]));
		obj.SetObject(pObj);
	}
	return obj;
}

void JsonObject::SetNull(const char* key, JsonAllocator& alloc)
{
	auto& my_val = *(m_pObj->m_pVal);
	auto& my_alloc = alloc.GetAllocator()->m_alloc;
	if (my_val.HasMember(key))
	{
		if (!my_val[key].IsNull())
		{
			my_val[key].SetNull();
		}
	}
	else
	{
		rapidjson::Value keyObj(rapidjson::kStringType);
		keyObj.SetString(key, my_alloc);
		rapidjson::Value val(rapidjson::kNullType);
		my_val.AddMember(keyObj, val, my_alloc);
	}
}

bool JsonObject::SetBool(const char* key, bool val, JsonAllocator& alloc)
{
	auto& my_val = *(m_pObj->m_pVal);
	auto& my_alloc = alloc.GetAllocator()->m_alloc;
	if (my_val.HasMember(key))
	{
		if (my_val[key].IsBool())
		{
			my_val.SetBool(val);
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		rapidjson::Value keyObj(rapidjson::kStringType);
		keyObj.SetString(key, my_alloc);
		my_val.AddMember(keyObj, val, my_alloc);
		return true;
	}
}

bool JsonObject::SetInt(const char* key, int32_t val, JsonAllocator& alloc)
{
	auto& my_val = *(m_pObj->m_pVal);
	auto& my_alloc = alloc.GetAllocator()->m_alloc;
	if (my_val.HasMember(key))
	{
		if (my_val[key].IsInt())
		{
			my_val.SetInt(val);
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		rapidjson::Value keyObj(rapidjson::kStringType);
		keyObj.SetString(key, my_alloc);
		my_val.AddMember(keyObj, val, my_alloc);
		return true;
	}
}

bool JsonObject::SetUint(const char* key, uint32_t val, JsonAllocator& alloc)
{
	auto& my_val = *(m_pObj->m_pVal);
	auto& my_alloc = alloc.GetAllocator()->m_alloc;
	if (my_val.HasMember(key))
	{
		if (my_val[key].IsUint())
		{
			my_val.SetUint(val);
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		rapidjson::Value keyObj(rapidjson::kStringType);
		keyObj.SetString(key, my_alloc);
		my_val.AddMember(keyObj, val, my_alloc);
		return true;
	}
}

bool JsonObject::SetInt64(const char* key, int64_t val, JsonAllocator& alloc)
{
	auto& my_val = *(m_pObj->m_pVal);
	auto& my_alloc = alloc.GetAllocator()->m_alloc;
	if (my_val.HasMember(key))
	{
		if (my_val[key].IsInt64())
		{
			my_val.SetInt64(val);
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		rapidjson::Value keyObj(rapidjson::kStringType);
		keyObj.SetString(key, my_alloc);
		my_val.AddMember(keyObj, val, my_alloc);
		return true;
	}
}

bool JsonObject::SetUint64(const char* key, uint64_t val, JsonAllocator& alloc)
{
	auto& my_val = *(m_pObj->m_pVal);
	auto& my_alloc = alloc.GetAllocator()->m_alloc;
	if (my_val.HasMember(key))
	{
		if (my_val[key].IsUint64())
		{
			my_val.SetUint64(val);
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		rapidjson::Value keyObj(rapidjson::kStringType);
		keyObj.SetString(key, my_alloc);
		my_val.AddMember(keyObj, val, my_alloc);
		return true;
	}
}

bool JsonObject::SetDouble(const char* key, double val, JsonAllocator& alloc)
{
	auto& my_val = *(m_pObj->m_pVal);
	auto& my_alloc = alloc.GetAllocator()->m_alloc;
	if (my_val.HasMember(key))
	{
		if (my_val[key].IsDouble())
		{
			my_val.SetDouble(val);
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		rapidjson::Value keyObj(rapidjson::kStringType);
		keyObj.SetString(key, my_alloc);
		my_val.AddMember(keyObj, val, my_alloc);
		return true;
	}
}

bool JsonObject::SetString(const char* key, const char* val, JsonAllocator& alloc)
{
	auto& my_val = *(m_pObj->m_pVal);
	auto& my_alloc = alloc.GetAllocator()->m_alloc;
	if (my_val.HasMember(key))
	{
		if (my_val[key].IsString())
		{
			my_val.SetString(val, my_alloc);
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		rapidjson::Value keyObj(rapidjson::kStringType);
		keyObj.SetString(key, my_alloc);
		rapidjson::Value tmpObj(rapidjson::kStringType);
		tmpObj.SetString(val, my_alloc);
		my_val.AddMember(keyObj, tmpObj, my_alloc);
		return true;
	}
}

bool JsonObject::SetObject(const char* key, JsonObject& obj, JsonAllocator& alloc)
{
	auto& my_val = *(m_pObj->m_pVal);
	auto& my_alloc = alloc.GetAllocator()->m_alloc;
	auto& add_val = *(obj.GetObject()->m_pVal);
	if (my_val.HasMember(key))
	{
		if (my_val[key].IsObject())
		{
			my_val.RemoveMember(key);
			rapidjson::Value keyObj(rapidjson::kStringType);
			keyObj.SetString(key, my_alloc);
			my_val.AddMember(keyObj, add_val, my_alloc);
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		rapidjson::Value keyObj(rapidjson::kStringType);
		keyObj.SetString(key, my_alloc);
		my_val.AddMember(keyObj, add_val, my_alloc);
		return true;
	}
}

bool JsonObject::SetArrayInt(int32_t val, JsonAllocator& alloc)
{
	auto& my_val = *(m_pObj->m_pVal);
	auto& my_alloc = alloc.GetAllocator()->m_alloc;
	if (my_val.IsArray())
	{
		my_val.PushBack(val, my_alloc);
		return true;
	}
	return false;
}

bool JsonObject::SetArrayString(const char* val, JsonAllocator& alloc)
{
	auto& my_val = *(m_pObj->m_pVal);
	auto& my_alloc = alloc.GetAllocator()->m_alloc;
	if (my_val.IsArray())
	{
		rapidjson::Value temp(rapidjson::kStringType);
		temp.SetString(val, my_alloc);
		my_val.PushBack(temp, my_alloc);
		return true;
	}
	return false;
}

bool JsonObject::SetArrayObject(JsonObject& obj, JsonAllocator& alloc)
{
	auto& my_val = *(m_pObj->m_pVal);
	auto& my_alloc = alloc.GetAllocator()->m_alloc;
	auto& add_val = *(obj.GetObject()->m_pVal);
	if (my_val.IsArray())
	{
		my_val.PushBack(add_val, my_alloc);
		return true;
	}
	return false;
}

void JsonObject::RemoveChild(const char* key)
{
	auto& my_val = *(m_pObj->m_pVal);
	if (my_val.HasMember(key))
	{
		my_val.RemoveMember(key);
	}
}

JsonDocument::JsonDocument() :
	m_pBuf(nullptr),
	m_pDoc(new _JsonDocument()),
	m_alloc(*this)
{
}

JsonDocument::~JsonDocument()
{
	if (m_pBuf)
	{
		delete[] m_pBuf;
		m_pBuf = nullptr;
	}

	if (m_pDoc)
	{
		delete m_pDoc;
		m_pDoc = nullptr;
	}
}

JsonDocument::_JsonDocument* JsonDocument::GetDocument()
{
	return m_pDoc;
}

JsonAllocator& JsonDocument::GetAllocator()
{
	return m_alloc;
}

bool JsonDocument::ParseFromString(const char* json)
{
	auto& my_doc = m_pDoc->m_doc;
	my_doc.Parse(json);
	if (my_doc.HasParseError()) 
	{
		rapidjson::ParseErrorCode err = my_doc.GetParseError();
		return false;
	}
	return true;
}

const char* JsonDocument::ParseToString(bool bEasy2Read)
{
	if (m_pBuf)
		delete[] m_pBuf;

	auto& my_doc = m_pDoc->m_doc;
	rapidjson::StringBuffer buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer, rapidjson::UTF8<>, rapidjson::UTF8<>, rapidjson::CrtAllocator, rapidjson::kWriteDefaultFlags> writer(buffer);
	my_doc.Accept(writer);

	const char* str = buffer.GetString();
	m_pBuf = new char[strlen(str) + 1];
	memcpy(m_pBuf, str, strlen(str));
	m_pBuf[strlen(str)] = 0;
	return m_pBuf;
}

bool JsonDocument::GetBool(const char* key, bool default_val) const
{
	auto& my_doc = m_pDoc->m_doc;
	if (IsValidChildType(my_doc, key, rapidjson::kFalseType))
	{
		return false;
	}
	else if (IsValidChildType(my_doc, key, rapidjson::kTrueType))
	{
		return true;
	}
	return default_val;
}

int32_t JsonDocument::GetInt(const char* key, int32_t default_val) const
{
	auto& my_doc = m_pDoc->m_doc;
	if (IsValidChildType(my_doc, key, rapidjson::kNumberType))
	{
		return my_doc[key].GetInt();
	}
	return default_val;
}

double JsonDocument::GetDouble(const char* key, double default_val) const
{
	auto& my_doc = m_pDoc->m_doc;
	if (IsValidChildType(my_doc, key, rapidjson::kNumberType))
	{
		return my_doc[key].GetDouble();
	}
	return default_val;
}

const char* JsonDocument::GetString(const char* key, const char* default_val) const
{
	auto& my_doc = m_pDoc->m_doc;
	if (IsValidChildType(my_doc, key, rapidjson::kStringType))
	{
		return my_doc[key].GetString();
	}
	return default_val;
}

PackJsonObject JsonDocument::GetObject(const char* key) const
{
	PackJsonObject obj;
	auto& my_doc = m_pDoc->m_doc;
	if (IsValidChildType(my_doc, key, rapidjson::kObjectType) ||
		IsValidChildType(my_doc, key, rapidjson::kArrayType))
	{
		JsonObject* pObj = new JsonObject(new JsonObject::_JsonObject(&my_doc[key]));
		obj.SetObject(pObj);
	}
	return obj;
}

void JsonDocument::SetObject()
{
	auto& my_doc = m_pDoc->m_doc;
	my_doc.SetObject();
}

void JsonDocument::SetNull(const char* key)
{
	auto& my_doc = m_pDoc->m_doc;
	auto& my_alloc = my_doc.GetAllocator();
	if (my_doc.HasMember(key))
	{
		if (!my_doc[key].IsNull())
		{
			my_doc[key].SetNull();
		}
	}
	else
	{
		rapidjson::Value keyObj(rapidjson::kStringType);
		keyObj.SetString(key, my_alloc);
		rapidjson::Value val(rapidjson::kNullType);
		my_doc.AddMember(keyObj, val, my_alloc);
	}
}

bool JsonDocument::SetBool(const char* key, bool val)
{
	auto& my_doc = m_pDoc->m_doc;
	auto& my_alloc = my_doc.GetAllocator();
	if (my_doc.HasMember(key))
	{
		if (my_doc[key].IsBool())
		{
			my_doc[key].SetBool(val);
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		rapidjson::Value keyObj(rapidjson::kStringType);
		keyObj.SetString(key, my_alloc);
		my_doc.AddMember(keyObj, val, my_alloc);
		return true;
	}
}

bool JsonDocument::SetInt(const char* key, int32_t val)
{
	auto& my_doc = m_pDoc->m_doc;
	auto& my_alloc = my_doc.GetAllocator();
	if (my_doc.HasMember(key))
	{
		if (my_doc[key].IsInt())
		{
			my_doc[key].SetInt(val);
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		rapidjson::Value keyObj(rapidjson::kStringType);
		keyObj.SetString(key, my_alloc);
		my_doc.AddMember(keyObj, val, my_alloc);
		return true;
	}
}

bool JsonDocument::SetUint(const char* key, uint32_t val)
{
	auto& my_doc = m_pDoc->m_doc;
	auto& my_alloc = my_doc.GetAllocator();
	if (my_doc.HasMember(key))
	{
		if (my_doc[key].IsUint())
		{
			my_doc[key].SetUint(val);
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		rapidjson::Value keyObj(rapidjson::kStringType);
		keyObj.SetString(key, my_alloc);
		my_doc.AddMember(keyObj, val, my_alloc);
		return true;
	}
}

bool JsonDocument::SetInt64(const char* key, int64_t val)
{
	auto& my_doc = m_pDoc->m_doc;
	auto& my_alloc = my_doc.GetAllocator();
	if (my_doc.HasMember(key))
	{
		if (my_doc[key].IsInt64())
		{
			my_doc[key].SetInt64(val);
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		rapidjson::Value keyObj(rapidjson::kStringType);
		keyObj.SetString(key, my_alloc);
		my_doc.AddMember(keyObj, val, my_alloc);
		return true;
	}
}

bool JsonDocument::SetUint64(const char* key, uint64_t val)
{
	auto& my_doc = m_pDoc->m_doc;
	auto& my_alloc = my_doc.GetAllocator();
	if (my_doc.HasMember(key))
	{
		if (my_doc[key].IsUint64())
		{
			my_doc[key].SetUint64(val);
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		rapidjson::Value keyObj(rapidjson::kStringType);
		keyObj.SetString(key, my_alloc);
		my_doc.AddMember(keyObj, val, my_alloc);
		return true;
	}
}

bool JsonDocument::SetDouble(const char* key, double val)
{
	auto& my_doc = m_pDoc->m_doc;
	auto& my_alloc = my_doc.GetAllocator();
	if (my_doc.HasMember(key))
	{
		if (my_doc[key].IsDouble())
		{
			my_doc[key].SetDouble(val);
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		rapidjson::Value keyObj(rapidjson::kStringType);
		keyObj.SetString(key, my_alloc);
		my_doc.AddMember(keyObj, val, my_alloc);
		return true;
	}
}

bool JsonDocument::SetString(const char* key, const char* val)
{
	auto& my_doc = m_pDoc->m_doc;
	auto& my_alloc = my_doc.GetAllocator();
	if (my_doc.HasMember(key))
	{
		if (my_doc[key].IsString())
		{
			my_doc[key].SetString(val, my_alloc);
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		rapidjson::Value keyObj(rapidjson::kStringType);
		keyObj.SetString(key, my_alloc);
		rapidjson::Value tmpObj(rapidjson::kStringType);
		tmpObj.SetString(val, my_alloc);
		my_doc.AddMember(keyObj, tmpObj, my_alloc);
		return true;
	}
}

bool JsonDocument::SetObject(const char* key, JsonObject& obj)
{
	auto& my_doc = m_pDoc->m_doc;
	auto& my_alloc = my_doc.GetAllocator();
	auto& add_val = *(obj.GetObject()->m_pVal);
	if (my_doc.HasMember(key))
	{
		if (my_doc[key].IsObject())
		{
			my_doc.RemoveMember(key);
			rapidjson::Value keyObj(rapidjson::kStringType);
			keyObj.SetString(key, my_alloc);
			my_doc.AddMember(keyObj, add_val, my_alloc);
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		rapidjson::Value keyObj(rapidjson::kStringType);
		keyObj.SetString(key, my_alloc);
		my_doc.AddMember(keyObj, add_val, my_alloc);
		return true;
	}
}

DREAM_NAMESPACE_END