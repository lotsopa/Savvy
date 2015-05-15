#ifndef SAVVY_CONSTRUCTOR_HLSL_TO_GLSL_NO_UBO_H
#define SAVVY_CONSTRUCTOR_HLSL_TO_GLSL_NO_UBO_H
#include "SavvyConstructorHLSLToGLSL.h"

namespace Savvy
{
	namespace Internal
	{
		// Forward Declarations
		class FuncConverterHLSLToGLSL;
		class ConstructorHLSLToGLSLNoUBO : public ConstructorHLSLToGLSL
		{
		public:

			/*
			Default constructor
			*/
			ConstructorHLSLToGLSLNoUBO();

			/*
			Destructor
			*/
			virtual ~ConstructorHLSLToGLSLNoUBO();

			/*
			The function which resets the Constructor to its initial state.
			*/
			virtual void Reset();

			/*
			The function which initializes the Constructor options.
			It MUST be called before the Construct() function for proper behaviour.
			*/
			virtual ResultCode Initialize(Key& a_InputEntry, Key& a_OutputEntry, uint32 a_Inputlang, uint32 a_ShaderType, Database* a_Database);

			/*
			The function which constructs the new shader to the specified output stream.
			*/
			virtual ResultCode Construct(std::ostream& a_OutputStream);

		protected:
			virtual ResultCode ConstructUniformBuffersGLSL(std::ostream& a_OutputStream);
		};
	}
}

#endif // !SAVVY_CONSTRUCTOR_HLSL_TO_GLSL_H