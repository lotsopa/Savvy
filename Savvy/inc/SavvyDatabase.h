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
#ifndef SAVVY_DATABASE_H
#define SAVVY_DATABASE_H

#include "SavvyKey.h"
#include "SavvyFrameworkDefines.h"
#include "SavvyDefines.h"

namespace Savvy
{
	/*
	The Database class keeps all the necessary information needed to convert the shader from one language to another.
	Stored data includes: data types, uniforms, functions and variables.
	*/
	class Database
	{
	public:

		typedef std::vector<Key> KeyList;

		struct Variable
		{
			Key m_Name;
			KeyList m_QualifierList;
			KeyList m_TemplateArgList; // This list contains the template arguments, if there are any
		};

		typedef std::vector<Variable> VarList;
		struct Function
		{
			Key m_Name;
			Key m_ReturnType;
			bool m_TranslateReturnType;
			VarList m_Arguments;
		};

		struct Word
		{
			Key m_Name;
			bool m_Translate; // Indicates if the word needs translation or not
		};

		struct VarWithSemantic
		{
			Key m_VarName;
			Key m_Semantic;
		};

		typedef std::map<Key, Key> KeyMap;
		typedef std::multimap<Key, VarWithSemantic> SemanticMap;
		typedef std::map<Key, KeyList> KeyListMap;
		typedef std::multimap<Key, Word> WordMultiMap;
		typedef std::map<uint32, Key> IndexMap;
		typedef std::multimap<Key, Key> KeyMultiMap;
		typedef std::multimap<Key, Variable> KeyVarMultiMap;
		typedef std::map<Key, Variable> KeyVarMap;
		typedef std::vector<Function> FunctionList;

		/*
		Default Constructor
		*/
		Database();

		/*
		Destructor
		*/
		virtual ~Database();

		/*
		Adds an input variable to the shader description.
		*/
		ResultCode AddInputVariable(Key& a_Name, KeyList& a_TypeQualifierList, KeyList& a_TemplateArgList = KeyList());

		/*
		Adds an input variable to the shader description with a specified index.
		*/
		ResultCode AddInputVariable(Key& a_Name, KeyList& a_TypeQualifierList, uint32 a_Idx, KeyList& a_TemplateArgList = KeyList());


		/*
		Adds an output variable to the shader description.
		*/
		ResultCode AddOutputVariable(Key& a_Name, KeyList& a_TypeQualifierList, KeyList& a_TemplateArgList = KeyList());

		/*
		Adds an output variable to the shader description with a specified index
		*/
		ResultCode AddOutputVariable(Key& a_Name, KeyList& a_TypeQualifierList, uint32 a_Idx, KeyList& a_TemplateArgList = KeyList());

		/*
		Adds a uniform variable to the shader description.
		*/
		ResultCode AddUniformVariable(Key& a_Name, KeyList& a_TypeQualifierList, Key& a_BufferName, KeyList& a_TemplateArgList = KeyList());

		/*
		Adds a generic buffer variable variable to the shader description.
		*/
		ResultCode AddGenericBufferVariable(Key& a_Name, KeyList& a_TypeQualifierList, Key& a_BufferName, KeyList& a_TemplateArgList = KeyList());

		/*
		Adds a local function variable to the database.
		*/
		ResultCode AddLocalVariable(Key& a_Name, KeyList& a_TypeQualifierList, Key& a_FuncName, KeyList& a_TemplateArgList = KeyList());

		/*
		Adds an instruction to a function.
		*/
		ResultCode AddLocalInstruction(Key& a_Instruction, bool a_Translate, Key& a_FuncName);

		/*
		Adds a new function.
		*/
		ResultCode AddFunction(Key& a_FuncName, Key& a_FuncReturnType, VarList& a_Arguments, bool a_TranslateReturnType = true);

		/*
		Adds a new sampler to the shader description.
		*/
		ResultCode AddSampler(Key& a_Name, Key& a_Type, KeyList& a_TemplateArgList = KeyList());

		/*
		Adds a new sampler to the shader description with a specified index.
		*/
		ResultCode AddSampler(Key& a_Name, Key& a_Type, uint32 a_Idx, KeyList& a_TemplateArgList = KeyList());

		/*
		Checks if an input variable exists. Returns true if it does, otherwise returns false.
		*/
		const bool InputVariableExists(const Key& a_Name);

		/*
		Checks if an output variable exists. Returns true if it does, otherwise returns false.
		*/
		const bool OutputVariableExists(const Key& a_Name);

		/*
		Checks if an input index exists. Returns true if it does, otherwise returns false.
		*/
		const bool InputIndexExists(uint32 a_Idx);

		/*
		Checks if an output index exists. Returns true if it does, otherwise returns false.
		*/
		const bool OutputIndexExists(uint32 a_Idx);

		/*
		Checks if a uniform variable for a particular buffer exists. Returns true if it does, otherwise returns false.
		*/
		const bool UniformVariableExists(const Key& a_BufferName, const Key& a_Name);

		/*
		Checks if a buffer variable for a particular buffer exists. Returns true if it does, otherwise returns false.
		*/
		const bool GenericBufferVariableExists(const Key& a_BufferName, const Key& a_Name);

		/*
		Checks if a local variable in a function exists. Returns true if it does, otherwise returns false.
		*/
		const bool LocalVariableExists(const Key& a_FuncName, const Key& a_Name);

		/*
		Checks if a uniform buffer with the specified name exists. Returns true if it does, otherwise returns false.
		*/
		const bool UniformBufferExists(const Key& a_Name);

		/*
		Checks if a generic buffer with the specified name exists. Returns true if it does, otherwise returns false.
		*/
		const bool GenericBufferExists(const Key& a_Name);

		/*
		Checks if a function with the specified name exists. Returns true if it does, otherwise returns false.
		*/
		const bool FunctionExists(const Key& a_Name, Key& a_FuncReturnType, VarList& a_Arguments);

		const bool FunctionExists(const Key& a_Name);

		/*
		Checks if a sampler with the specified name exists. Returns true if it does, otherwise returns false.
		*/
		const bool SamplerExists(const Key& a_Name);

		/*
		Checks if a sampler with the specified index exists. Returns true if it does, otherwise returns false.
		*/
		const bool SamplerIndexExists(uint32 a_Idx);

		/*
		Checks if a registered variable with the name exists and checks if it contains a keyword in its type.
		*/
		const bool FindVariableWithType(const Key& a_Name, const Key& a_FuncName, const Key& a_Type);

		/*
		Checks if a registered variable with the name exists and checks if it contains any of the list of keywords in its type.
		*/
		const bool FindVariableWithType(const Key& a_Name, const Key& a_FuncName, KeyList& a_TypeList);

		/*
		Adds a new uniform buffer to the database.
		*/
		ResultCode AddUniformBuffer(const Key& a_Name);

		/*
		Adds a new generic buffer to the database.
		*/
		ResultCode AddGenericBuffer(const Key& a_Name);

		/*
		Adds a new generic buffer with a specified index to the database.
		*/
		ResultCode AddGenericBuffer(const Key& a_Name, uint32 a_Idx);

		/*
		Adds a new global variable to the database.
		*/
		ResultCode AddGlobalVariable(const Key& a_Name, KeyList& a_TypeQualifiers, KeyList& a_TemplateArgList = KeyList());

		/*
		Checks if a global variable exists. Returns true if it does, false otherwise.
		*/
		const bool GlobalVariableExists(const Key& a_Name);

		/*
		Adds a new uniform buffer with a specified index to the database.
		*/
		ResultCode AddUniformBuffer(const Key& a_Name, uint32 a_Idx);

		/*
		Checks if a uniform buffer with the specified index exists. Returns true if it does, otherwise returns false.
		*/
		const bool UniformBufferIndexExists(uint32 a_Idx);

		/*
		Checks if a generic buffer with the specified index exists. Returns true if it does, otherwise returns false.
		*/
		const bool GenericBufferIndexExists(uint32 a_Idx);

		/*
		Checks if a struct with the specified name exists. Returns true if it does, otherwise returns false.
		*/
		const bool StructExists(const Key& a_Name);

		/*
		Checks if a variable for a particular struct exists. Returns true if it does, otherwise returns false.
		*/
		const bool StructVariableExists(const Key& a_BufferName, const Key& a_Name);

		/*
		Adds a new structure to the database.
		*/
		ResultCode AddStruct(const Key& a_Name);

		/*
		Adds a new variable to a structure in the database.
		*/
		ResultCode AddStructVariable(const Key& a_StructName, const Key& a_Name, KeyList& a_TypeQualifierList, KeyList& a_TemplateArgList = KeyList());

		/*
		Returns the Uniform Variables map.
		*/
		SAVVY_INLINE KeyVarMultiMap& GetUniformMap() { return m_UniformVariables; }

		/*
		Returns the Generic Buffer Variables map.
		*/
		SAVVY_INLINE KeyVarMultiMap& GetGenericBufferVarMap() { return m_GenericBufferVariables; }

		/*
		Returns the Input Variables map.
		*/
		SAVVY_INLINE KeyVarMap& GetInputMap() { return m_InputVariables; }

		/*
		Returns the Output Variables map.
		*/
		SAVVY_INLINE KeyVarMap& GetOutputMap() { return m_OutputVariables; }

		/*
		Returns the Uniform Buffer list.
		*/
		SAVVY_INLINE KeyList& GetUniformBufferList() { return m_UniformBuffers; }

		/*
		Returns the Generic Buffer list.
		*/
		SAVVY_INLINE KeyList& GetGenericBufferList() { return m_GenericBuffers; }

		/*
		Returns the sampler list.
		*/
		SAVVY_INLINE KeyVarMap& GetSamplerList() { return m_Samplers; }

		/*
		Returns an Input variable with the specified index.
		*/
		SAVVY_INLINE const Key& GetInputVariableWithIndex(uint32 a_Idx) { return m_InputIndexMap.at(a_Idx); }

		/*
		Returns an Output variable with the specified index.
		*/
		SAVVY_INLINE const Key& GetOutputVariableWithIndex(uint32 a_Idx) { return m_OutputIndexMap.at(a_Idx); }

		/*
		Returns a sampler variable with the specified index.
		*/
		SAVVY_INLINE const Key& GetSamplerWithIndex(uint32 a_Idx) { return m_SamplerIndexMap.at(a_Idx); }

		/*
		Returns a uniform buffer with the specified index.
		*/
		SAVVY_INLINE const Key& GetUniformBufferWithIndex(uint32 a_Idx) { return m_UniformBufferIndexMap.at(a_Idx); }

		/*
		Returns a generic buffer with the specified index.
		*/
		SAVVY_INLINE const Key& GetGenericBufferWithIndex(uint32 a_Idx) { return m_GenericBufferIndexMap.at(a_Idx); }

		/*
		Returns the list of functions.
		*/
		SAVVY_INLINE FunctionList& GetFunctionList() { return m_FunctionList; }

		/*
		Returns the map of instructions.
		*/
		SAVVY_INLINE WordMultiMap& GetInstructionMap() { return m_FuncInstructionMap; }

		/*
		Returns the map of variables declared inside functions.
		*/
		SAVVY_INLINE KeyVarMultiMap& GetFuncVariableMap() { return m_FunctionVariableMap; }

		/*
		Returns the index map of sampler objects.
		*/
		SAVVY_INLINE IndexMap& GetSamplerIndexMap() { return m_SamplerIndexMap; }

		/*
		Returns the index map of uniform buffers.
		*/
		SAVVY_INLINE IndexMap& GetUniformBufferIndexMap() { return m_UniformBufferIndexMap; }

		/*
		Returns the index map of generic buffers.
		*/
		SAVVY_INLINE IndexMap& GetGenericBufferIndexMap() { return m_GenericBufferIndexMap; }

		/*
		Returns the list of structures.
		*/
		SAVVY_INLINE KeyList& GetStructList() { return m_StructList; }

		/*
		Returns the list of structure variables.
		*/
		SAVVY_INLINE KeyVarMultiMap& GetStructVariableMap() { return m_StructVariables; }

		/*
		Returns the list of global variables.
		*/
		SAVVY_INLINE VarList& GetGlobalVariableList() { return m_GlobalVarList; }

		/*
		Returns the map of input indices of input variables.
		*/
		SAVVY_INLINE IndexMap& GetInputVariablesIndexMap() { return m_InputIndexMap; }

		/*
		Returns the map of output indices of output variables.
		*/
		SAVVY_INLINE IndexMap& GetOutputVariablesIndexMap() { return m_OutputIndexMap; }

		/*
		Returns a function with the specified name. If the structure does not exist, the last element is returned.
		*/
		Function& GetFunction(const Key& a_Name);

		/*
		Adds a semantic to a variable name.
		*/
		ResultCode AddSemantic(Key& a_StructName, Key& a_VarName, Key& a_SemanticName);

		/*
		Checks if a semantic for the variable name exists. Returns true if it does, false otherwise.
		*/
		const bool SemanticExists(const Key& a_StructName, const Key& a_VarName);

		/*
		Returns a Key with the semantic for a particular variable.
		*/
		const Key& GetSemantic(const Key& a_StructName, const Key& a_VarName);

		/*
		Checks if an equivalent for the input language keyword exists in the output language.
		Returns true if it exists, false otherwise.
		*/
		virtual const bool OutputLangEquivalentExists(const Key& a_Name) = 0;

		/*
		Returns a keyword with the same meaning from the output language.
		*/
		virtual const Key& GetOutputLangEquivalent(const Key& a_Name) = 0;

		/*
		Adds a preprocessor define statement to the defines list.
		*/
		ResultCode AddDefine(const Key& a_Def);

		/*
		Returns the list of preprocessor defines.
		*/
		SAVVY_INLINE KeyList& GetDefinesList() { return m_DefineList; }

		/*
		Attempts to find a variable with the specified name, declared anywhere. Returns true if the search succeeds, false otherwise.
		*/
		const bool VariableWithNameExists(const Key& a_Name, const Key& a_FuncName);

		/*
		Resets the state of the Database and clears any data it holds.
		*/
		virtual void Reset();

	protected:

		IndexMap m_InputIndexMap;
		IndexMap m_OutputIndexMap;
		IndexMap m_SamplerIndexMap;
		IndexMap m_UniformBufferIndexMap;
		IndexMap m_GenericBufferIndexMap;

		KeyVarMap m_InputVariables;
		KeyVarMap m_OutputVariables;
		KeyList m_UniformBuffers;
		KeyList m_GenericBuffers;
		KeyVarMultiMap m_GenericBufferVariables;
		KeyVarMultiMap m_UniformVariables;
		KeyVarMultiMap m_StructVariables;
		KeyList m_StructList;
		KeyVarMap m_Samplers;
		VarList m_GlobalVarList;
		KeyVarMultiMap m_FunctionVariableMap;
		WordMultiMap m_FuncInstructionMap;
		FunctionList m_FunctionList;
		KeyList m_DefineList;
		SemanticMap m_SemanticMap;
		uint32 m_GenericBufferIndex;
		uint32 m_UniformBufferIndex;
		uint32 m_InputsIndex;
		uint32 m_SamplerIndex;
		uint32 m_OutputsIndex;
	};
}

#endif // !SAVVY_DATABASE_H