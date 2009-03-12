#pragma once

namespace E4X
{

	class E4XElement :
		public E4XCell
	{
	public:
		E4XElement(void);
		E4XElement(const std::string& strName);
		virtual ~E4XElement(void);

		const char* parse( const char* xmldata);

		virtual void setValue(const std::string& value);
		virtual const std::string& getValue();
		virtual E4XElement& operator = ( const std::string& strName);

		virtual	E4XElement& copy();


	protected:
		std::string& toXmlStringInternal( std::string& strXml, int nIndent);
	};

}  // namespace
