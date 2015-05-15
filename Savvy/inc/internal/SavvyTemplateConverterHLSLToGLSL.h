#ifndef SAVVY_TEMPLATE_CONVERTER_HLSL_TO_GLSL_H
#define SAVVY_TEMPLATE_CONVERTER_HLSL_TO_GLSL_H
#include "SavvyTemplateConverter.h"

namespace Savvy
{
	namespace Internal
	{
		class TemplateConverterHLSLToGLSL : public TemplateConverter
		{
		public:
			TemplateConverterHLSLToGLSL();
			~TemplateConverterHLSLToGLSL();
			virtual ResultCode ConvertTemplatedVariable(Database::Variable& a_Var, Database::Variable& a_RetVar);
			virtual ResultCode ConvertTemplatedInstruction(Database::WordMultiMap::iterator& it1, std::string& currentLine, Key& a_FuncName, uint32 a_Shader, Key& a_InputStructName = Key(), Key& a_OutputStructName = Key());
			virtual ResultCode GetTemplateArguments(Database::WordMultiMap::iterator& it1, StringList& a_OutputList, std::string& currentLine);

		protected:
			typedef std::vector<std::function<ResultCode(Database::Variable&, Database::Variable&, bool&)>> TemplateConvertList;
			TemplateConvertList m_TemplateConvertList;
			ResultCode ConvertTextureTemplates(Database::Variable& a_Var, Database::Variable& a_RetVar, bool& a_Handled);
		};
	}
}

#endif // !SAVVY_TEMPLATE_CONVERTER_HLSL_TO_GLSL_H