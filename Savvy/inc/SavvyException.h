#ifndef SAVVY_EXCEPTION_H
#define SAVVY_EXCEPTION_H

namespace Savvy
{
	/*
	A generic exception class.
	*/
	class Exception: public std::exception
	{
	public:

		Exception(std::string a_Msg);
		virtual ~Exception();

		virtual const char* what() const throw ();

	protected:
		std::string m_Error;
	};
}

#endif