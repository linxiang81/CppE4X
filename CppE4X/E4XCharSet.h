

#ifdef WIN32

#include <windows.h>
#include <string>

namespace E4X
{
	inline std::string _w2a( const std::wstring& pwcsSrc, int codepage)
	{
		char* _lin_dst = 0;
		int _lin_dst_size = 0;
		_lin_dst_size = (pwcsSrc.length()+1)*3*2;
		_lin_dst = new (std::nothrow) char[ _lin_dst_size];
		if( _lin_dst == 0) return std::string();

		_lin_dst_size = WideCharToMultiByte( codepage, 0, pwcsSrc.c_str(), pwcsSrc.length(), _lin_dst, _lin_dst_size, 0, 0);

		if( _lin_dst_size == 0) 
		{
			delete[] _lin_dst;
			return std::string();
		}

		_lin_dst[_lin_dst_size] = 0;
		std::string result = _lin_dst;
		delete[] _lin_dst;
		return result;
	}

	inline std::wstring _a2w( const std::string& pszSrc, int codepage)
	{
		wchar_t* _lin_wdst = 0;
		int _lin_dst_size = 0;
		_lin_dst_size = (pszSrc.length()+1)*2;
		_lin_wdst = new (std::nothrow) wchar_t [ _lin_dst_size];
		if( _lin_wdst == 0) return std::wstring();

		_lin_dst_size = MultiByteToWideChar( codepage, 0, pszSrc.c_str(), pszSrc.length(), _lin_wdst,_lin_dst_size);

		if( _lin_dst_size == 0)
		{
			delete[] _lin_wdst;
			return std::wstring();
		}

		_lin_wdst[_lin_dst_size] = 0;
		std::wstring result = _lin_wdst;
		delete[] _lin_wdst;
		return result;
	}
};
//#define _utf82w( pszSrc) E4X::_a2w( pszSrc, CP_UTF8)
//#define _w2utf8( pszSrc) E4X::_w2a( pszSrc, CP_UTF8)
//
//#define _utf82w( pszSrc) E4X::_a2w( pszSrc, CP_UTF8)
//#define _w2utf8( pszSrc) E4X::_w2a( pszSrc, CP_UTF8)
//
//
//	// �����ĺ�
//#define a2w(pszSrc) _a2w(pszSrc, CP_ACP)
//#define w2a(tcsSrc) _w2a(tcsSrc, CP_ACP)
//
//#define w2utf8(wcsSrc) _w2utf8(wcsSrc)
//#define utf82w(pszSrc) _utf82w(pszSrc)

#define e4x_a2utf8(pszSrc)  E4X::_w2a( E4X::_a2w(pszSrc, CP_ACP), CP_UTF8)
#define e4x_utf82a(pszSrc)  E4X::_w2a( E4X::_a2w(pszSrc, CP_UTF8), CP_ACP)

#else
#define e4x_a2utf8( pszSrc) pszSrc
#define e4x_utf82a( pszSrc) pszSrc
#endif