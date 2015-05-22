/*
The MIT License (MIT)

Copyright (c) 2015 Apostol Dadachev

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#ifndef SAVVY_FUNC_CONVERTER_HLSL_TO_GLSL_H
#define SAVVY_FUNC_CONVERTER_HLSL_TO_GLSL_H

#include "SavvyFuncConverter.h"

namespace Savvy
{
	namespace Internal
	{
		class FuncConverterHLSLToGLSL : public FuncConverter
		{
		public:
			FuncConverterHLSLToGLSL();
			virtual ~FuncConverterHLSLToGLSL();
			virtual ResultCode ConvertFunction(Database::WordMultiMap::iterator& it1, std::string& currentLine, Key& a_FuncName, uint32 a_Shader, Key& a_InputStructName = Key(), Key& a_OutputStructName = Key());
			virtual ResultCode GetFuncArgumentList(Database::WordMultiMap::iterator& it1, StringList& a_OutputList);
			virtual void Reset();
		private:
			typedef std::map<Key, std::function<ResultCode(Database::WordMultiMap::iterator&, std::string&)>> FuncConvertMap;
			FuncConvertMap m_FuncConvertMap;

			ResultCode HandleNoEquivalentFunc(Database::WordMultiMap::iterator& it1, std::string& currentLine);
			ResultCode HandleMatrixMultiplication(Database::WordMultiMap::iterator& it1, std::string& currentLine);
			ResultCode Convert_Interlocked_CompSwap(Database::WordMultiMap::iterator& it1, std::string& currentLine);
			ResultCode Convert_Interlocked_Operations(Database::WordMultiMap::iterator& it1, std::string& currentLine);
			ResultCode Convert_Saturate(Database::WordMultiMap::iterator& it1, std::string& currentLine);
			ResultCode Convert_Atan2(Database::WordMultiMap::iterator& it1, std::string& currentLine);
			ResultCode Convert_Unsupported(Database::WordMultiMap::iterator& it1, std::string& currentLine);
			ResultCode Convert_Fmod(Database::WordMultiMap::iterator& it1, std::string& currentLine);
			ResultCode Convert_Log10(Database::WordMultiMap::iterator& it1, std::string& currentLine);
			ResultCode Convert_Mad(Database::WordMultiMap::iterator& it1, std::string& currentLine);
			ResultCode Convert_Noise(Database::WordMultiMap::iterator& it1, std::string& currentLine);
			ResultCode Convert_Texturing(Database::WordMultiMap::iterator& it1, std::string& currentLine);
			ResultCode Convert_TexGather(Database::WordMultiMap::iterator& it1, std::string& currentLine);
		protected:
			const bool IsBuiltInGLSLVariable(Key& a_Name);
		};
	}
}

#endif