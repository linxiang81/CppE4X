#include "E4X.h"
#include <sstream>
#include <iomanip>

namespace E4X
{

	E4XCell::E4XCell(E4X_TYPE nType)
		: m_nType( nType)
		,m_pParent( 0)
	{
	}

	E4XCell::~E4XCell(void)
	{
		removeAllChild();
	}

	void E4XCell::removeAllChild()
	{
		std::list< E4XCell*>::iterator it;
		for( it = m_lstCell.begin(); it != m_lstCell.end(); it++)
		{
			delete *it;

		}
		m_lstCell.clear();
	}

	void E4XCell::removeAllChildElement()
	{
		std::list< E4XCell*>::iterator it = m_lstCell.begin();
		std::list< E4XCell*>::iterator itTemp = it;
		while( it != m_lstCell.end())
		{
			itTemp = it;
			itTemp++;
			if( (*it)->m_nType == E4X_ELEMENT)
			{
				delete *it;
				m_lstCell.remove( *it);
			}
			it = itTemp;
		}
	}

	const char* E4XCell::skipWhiteSpace(const char* pszIn)
	{
		while( *pszIn != 0)
		{
			if( *pszIn ==' ' || *pszIn=='\t' || *pszIn=='\r' || *pszIn=='\n')
			{
				pszIn++;
				continue;
			}
			else
			{
				return pszIn;
			}
		}

		return 0;
	}

	const char* E4XCell::skipValueChars(const char* pszIn)
	{
		while( *pszIn != 0)
		{
			if( *pszIn ==' ' || *pszIn=='\t' || *pszIn=='\r' || *pszIn=='\n' || *pszIn=='>' || *pszIn=='/' || *pszIn=='=' || *pszIn=='\"' || *pszIn=='\'')
			{
				return pszIn;
			}
			else
			{
				pszIn++;
				continue;
			}
		}

		return 0;
	}

	int E4XCell::GetType()
	{
		return m_nType;
	}

	void E4XCell::appendChild( E4XCell* pCell)
	{
		assert( pCell->m_pParent == 0);
		m_lstCell.push_back( pCell);
		pCell->m_pParent = this;
	}

	void E4XCell::prependChild( E4XCell* pCell)
	{
		assert( pCell->m_pParent == 0);
		m_lstCell.push_front( pCell);
		pCell->m_pParent = this;
	}

	void E4XCell::insertChildAfter( E4XCell* exist, E4XCell* insert)
	{
		std::list<E4XCell*>::iterator it;
		it = std::find( m_lstCell.begin(), m_lstCell.end(),exist);
		assert( it != m_lstCell.end());
		it++;
		m_lstCell.insert( it, insert);	
	}

	void E4XCell::insertChildBefore( E4XCell* exist, E4XCell* insert)
	{
		std::list<E4XCell*>::iterator it;
		it = std::find( m_lstCell.begin(), m_lstCell.end(),exist);
		assert( it != m_lstCell.end());
		m_lstCell.insert( it, insert);	
	}

	bool E4XCell::removeChild( E4XCell* pCell)
	{
		std::list< E4XCell*>::iterator it = std::find(m_lstCell.begin(), m_lstCell.end(), pCell);
		if( it != m_lstCell.end())
		{
			delete pCell;
			m_lstCell.erase( it);
			return true;
		}

		return false;
	}
	std::string& E4XCell::writeIndent( std::string& strXml, int nIndent)
	{
		if( nIndent != -1)
		{
			strXml += "\n";
			for( int i = 0; i < nIndent; i++)
				strXml += "\t";
		}
		return strXml;
	}

	std::string E4XCell::toXmlString( )
	{
		std::string strXml;
		return toXmlStringInternal( strXml, -1);
	}

	const std::string& E4XCell::getName()
	{
		return m_strName;
	}

	const std::string& E4XCell::getValue()
	{
		return m_strValue;
	}

	void E4XCell::setName(const std::string& name)
	{
		m_strName = name;
	}

	void E4XCell::setValue(const std::string& value)
	{
		m_strValue = value;
	}

	bool E4XCell::NotEnd(const char* pszBuffer, int nMinSize)
	{
		for( int i = 0; i< nMinSize; i++)
		{
			if( pszBuffer[i] != 0) continue;

			pszBuffer = 0;
			break;
		}

		return pszBuffer == 0? false: true;
	}

	int E4XCell::ToNumber()
	{
		return atoi( getValue().c_str());
	}

	double E4XCell::ToFloat()
	{
		return atof( getValue().c_str());
	}

	std::string E4XCell::ToString()
	{
		return getValue().c_str();
	}

	bool E4XCell::ToBoolean()
	{
		return getValue()=="true" || getValue()=="yes";
	}

	E4XCell& E4XCell::operator = ( const E4XCell& cell)
	{
		return const_cast<E4XCell&>(cell);
	}

	//E4XCell& E4XCell::operator = ( const char* strName)
	//{
	//	std::string str( strName);
	//	return operator = ( const_cast<std::string&>(str));
	//}

	//E4XCell& E4XCell::operator = ( char* strName)
	//{
	//	std::string str( strName);
	//	return operator = ( const_cast<std::string&>(str));
	//}

	E4XCell& E4XCell::operator = ( bool bFlag)
	{
		m_strValue = bFlag? "true":"false";

		return *this;
	}

	E4XIterator E4XCell::operator/(const std::string& strChildName)
	{
		return E4XIterator( *this, strChildName);
	}

	E4XIterator E4XCell::operator[](const std::string& strChildName)
	{
		return E4XIterator( *this, strChildName);
	}

	E4XCell& E4XCell::GetParent()
	{
		return *m_pParent;
	}

	std::string E4XCell::getRealString(const char* src)
	{
		/*
		&lt; <
		&gt; >
		&amp; &
		&apos; '
		&quot; "
		*/

		std::string out;
		out.reserve( strlen( src) + 1);

		const char* pos = src;
		const char* tmp;
		const char* end;
		while( *pos != 0)
		{
			if( *pos != '&')
			{
				out.push_back( *pos);
				++pos;
				continue;
			}

			end = strchr( pos, ';');
			if( end == 0)
				throw( E4XException( E4XException::ERROR_INVALID_TEXT));

			tmp = pos + 1;
			pos = end + 1;
			int len = end - tmp;

			if( *tmp == '#')
			{
				++tmp;
				if( *tmp == 'x')	// hex
				{
					if( len <=2  || (len&1)==1)
						throw(E4XException( E4XException::ERROR_INVALID_TEXT));
					char data = 0;
					int char_count = 0;
					++tmp;
					while( tmp != end)
					{
						data <<= 4;			// * 16
						char num = *tmp++;
						if( num >= '0' && num <='9')
						{
							num -= '0';
						}
						else if( num >='a' && num <='f')
						{
							num=num-'a'+10;
						}
						else if( num >='A' && num <='F')
						{
							num=num-'A'+10;
						}
						else
						{
							throw (E4XException( E4XException::ERROR_INVALID_TEXT));
						}

						data += num;

						char_count++;
						if( (char_count & 1) == 0)
						{
							out.push_back( data);
							data = 0;
						}
					}

				}
				else				// dec
				{
					if( len <=1)
						throw( E4XException( E4XException::ERROR_INVALID_TEXT));
					char data = 0;
					while( tmp != end)
					{
						char num = *tmp++;
						if( num>='0' && num<='9')
							num-='0';
						else
							throw( E4XException( E4XException::ERROR_INVALID_TEXT));
						data *= 10;
						data += num;
					}
					out.push_back( data);
				}
			}
			else
			{
				switch( len)
				{
				case 2:
					if(*tmp=='l' && *(tmp+1)=='t')	//  lt = '<'
					{
						out.push_back( '<');
					}
					else if( *tmp=='g' && *(tmp+1)=='t') // gt = '>'
					{
						out.push_back('>');
					}
					else
					{
						throw( E4XException( E4XException::ERROR_INVALID_TEXT));
					}
					break;

				case 3:
					if( *tmp=='a' && *(tmp+1)=='m' && *(tmp+2)=='p')		// amp = '&'
						out.push_back('&');
					else
						throw( E4XException( E4XException::ERROR_INVALID_TEXT));
					break;

				case 4:
					if( *tmp=='a' && *(tmp+1)=='p' && *(tmp+2)=='o' && *(tmp+3)=='s')	// apos = '\''
					{
						out.push_back('\'');
					}
					else if( *tmp=='q' && *(tmp+1)=='u' && *(tmp+2)=='o' && *(tmp+3)=='t')	// apos = '\"'
					{
						out.push_back('\"');
					} 
					else
					{
						throw( E4XException( E4XException::ERROR_INVALID_TEXT));
					}
					break;
				default:
					throw( E4XException( E4XException::ERROR_INVALID_TEXT));

				}
			}
		}
		return out;
	}

	std::string E4XCell::getXmlString(const char* src)
	{
		std::string out;
		while( *src != 0)
		{
			if( *src == '&')
			{
				out += "&amp;";
			} else if( *src == '<')
			{
				out += "&lt;";
			} else if( *src == '>')
			{
				out += "&gt;";
			} else if( *src == '\'')
			{
				out += "&apos;";
			} else if( *src == '\"')
			{
				out += "&quot;";
			} else if( *src == ' ')
			{
				out += "&#x20;";
			} else if( *src >= '\0' && *src <='\26')
			{
				std::stringstream ss;
				ss <<"&#x" << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(*src) << ";";
				out += ss.str();

				//char buf[10];
				//sprintf( buf, "%02x", *src);
				//out += "&#x";
				//out += buf;
				//out += ";";
			}
			else
			{
				out.push_back(*src);
			}
			// TODO need filter more ...

			src++;
		}

		return out;
	}

	E4XCell* E4XCell::parent()
	{
		return m_pParent;
	}

	void E4XCell::destroy()
	{
		removeAllChild();
		delete this;
	}

}  // namespace
