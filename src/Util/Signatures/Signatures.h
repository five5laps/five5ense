#pragma once
#include <cstdint>
#include <vector>

class CSignature
{
private:
	std::uintptr_t m_dwVal = 0x0;
	const char* m_pszDLLName = {};
	const char* m_pszSignature = {};
	int m_nOffset = 0;
	const char* m_pszName = {};

public:
	CSignature(const char* sDLLName, const char* sSignature, int nOffset, const char* sName);

	void Initialize();

	inline std::uintptr_t operator()()
	{
		return m_dwVal;
	}

	template <typename T> T As() { return reinterpret_cast<T>(this->operator()()); }
};

#define MAKE_SIGNATURE(name, dll, sig, offset) namespace S { inline CSignature name(dll, sig, offset, #name); }
MAKE_SIGNATURE(CEntitySphereQuery, "client.dll", "40 53 48 83 EC ? 48 8B D9 C7 44 24 ? ? ? ? ? 33 C9", 0x0);
class CSignatures
{
private:
	std::vector<CSignature*> m_vecSignatures = {};

public:
	void Initialize();

	inline void AddSignature(CSignature* pSignature)
	{
		m_vecSignatures.push_back(pSignature);
	}
};

namespace U { inline CSignatures Signatures; }
