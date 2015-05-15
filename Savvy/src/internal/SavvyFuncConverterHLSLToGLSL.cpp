#include "SavvyPCH.h"
#include "SavvyFuncConverterHLSLToGLSL.h"
#include "SavvyConstructor.h"

#define FUNC_MAP_INSERT(a, b) m_FuncConvertMap.insert(std::make_pair(a, std::bind(&b, this, std::placeholders::_1, std::placeholders::_2)))

Savvy::Internal::FuncConverterHLSLToGLSL::FuncConverterHLSLToGLSL()
{
	FUNC_MAP_INSERT(Key("InterlockedAdd"), Savvy::Internal::FuncConverterHLSLToGLSL::Convert_Interlocked_Operations);
	FUNC_MAP_INSERT(Key("InterlockedAnd"), Savvy::Internal::FuncConverterHLSLToGLSL::Convert_Interlocked_Operations);
	FUNC_MAP_INSERT(Key("InterlockedMax"), Savvy::Internal::FuncConverterHLSLToGLSL::Convert_Interlocked_Operations);
	FUNC_MAP_INSERT(Key("InterlockedMin"), Savvy::Internal::FuncConverterHLSLToGLSL::Convert_Interlocked_Operations);
	FUNC_MAP_INSERT(Key("InterlockedOr"), Savvy::Internal::FuncConverterHLSLToGLSL::Convert_Interlocked_Operations);
	FUNC_MAP_INSERT(Key("InterlockedXor"), Savvy::Internal::FuncConverterHLSLToGLSL::Convert_Interlocked_Operations);
	FUNC_MAP_INSERT(Key("InterlockedExchange"), Savvy::Internal::FuncConverterHLSLToGLSL::Convert_Interlocked_Operations);
	FUNC_MAP_INSERT(Key("InterlockedCompareExchange"), Savvy::Internal::FuncConverterHLSLToGLSL::Convert_Interlocked_CompSwap);
	FUNC_MAP_INSERT(Key("InterlockedCompareStore"), Savvy::Internal::FuncConverterHLSLToGLSL::Convert_Interlocked_CompSwap);
	FUNC_MAP_INSERT(Key("atan2"), Savvy::Internal::FuncConverterHLSLToGLSL::Convert_Atan2);
	FUNC_MAP_INSERT(Key("saturate"), Savvy::Internal::FuncConverterHLSLToGLSL::Convert_Saturate);
	FUNC_MAP_INSERT(Key("CheckAccessFullyMapped"), Savvy::Internal::FuncConverterHLSLToGLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("D3DCOLORtoUBYTE4"), Savvy::Internal::FuncConverterHLSLToGLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("DeviceMemoryBarrier"), Savvy::Internal::FuncConverterHLSLToGLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("DeviceMemoryBarrierWithGroupSync"), Savvy::Internal::FuncConverterHLSLToGLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("errorf"), Savvy::Internal::FuncConverterHLSLToGLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("f16tof32"), Savvy::Internal::FuncConverterHLSLToGLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("f32tof16"), Savvy::Internal::FuncConverterHLSLToGLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("fmod"), Savvy::Internal::FuncConverterHLSLToGLSL::Convert_Fmod);
	FUNC_MAP_INSERT(Key("GetRenderTargetSampleCount"), Savvy::Internal::FuncConverterHLSLToGLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("GetRenderTargetSamplePosition"), Savvy::Internal::FuncConverterHLSLToGLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("lit"), Savvy::Internal::FuncConverterHLSLToGLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("log10"), Savvy::Internal::FuncConverterHLSLToGLSL::Convert_Log10);
	FUNC_MAP_INSERT(Key("mad"), Savvy::Internal::FuncConverterHLSLToGLSL::Convert_Mad);
	FUNC_MAP_INSERT(Key("msad4"), Savvy::Internal::FuncConverterHLSLToGLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("noise"), Savvy::Internal::FuncConverterHLSLToGLSL::Convert_Noise);
	FUNC_MAP_INSERT(Key("Process2DQuadTessFactorsAvg"), Savvy::Internal::FuncConverterHLSLToGLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("Process2DQuadTessFactorsMax"), Savvy::Internal::FuncConverterHLSLToGLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("Process2DQuadTessFactorsMin"), Savvy::Internal::FuncConverterHLSLToGLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("ProcessIsolineTessFactors"), Savvy::Internal::FuncConverterHLSLToGLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("ProcessQuadTessFactorsAvg"), Savvy::Internal::FuncConverterHLSLToGLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("ProcessQuadTessFactorsMax"), Savvy::Internal::FuncConverterHLSLToGLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("ProcessQuadTessFactorsMin"), Savvy::Internal::FuncConverterHLSLToGLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("ProcessTriTessFactorsAvg"), Savvy::Internal::FuncConverterHLSLToGLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("ProcessTriTessFactorsMax"), Savvy::Internal::FuncConverterHLSLToGLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("ProcessTriTessFactorsMin"), Savvy::Internal::FuncConverterHLSLToGLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("rcp"), Savvy::Internal::FuncConverterHLSLToGLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("tex1D"), Savvy::Internal::FuncConverterHLSLToGLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("tex1Dbias"), Savvy::Internal::FuncConverterHLSLToGLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("texCUBE"), Savvy::Internal::FuncConverterHLSLToGLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("texCUBEbias"), Savvy::Internal::FuncConverterHLSLToGLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("texCUBEgrad"), Savvy::Internal::FuncConverterHLSLToGLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("texCUBElod"), Savvy::Internal::FuncConverterHLSLToGLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("texCUBEproj"), Savvy::Internal::FuncConverterHLSLToGLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("tex1D"), Savvy::Internal::FuncConverterHLSLToGLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("tex1D"), Savvy::Internal::FuncConverterHLSLToGLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("abort"), Savvy::Internal::FuncConverterHLSLToGLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("Sample"), Savvy::Internal::FuncConverterHLSLToGLSL::Convert_Texturing);
	FUNC_MAP_INSERT(Key("GetDimensions"), Savvy::Internal::FuncConverterHLSLToGLSL::Convert_Texturing);
	FUNC_MAP_INSERT(Key("SampleLevel"), Savvy::Internal::FuncConverterHLSLToGLSL::Convert_Texturing);
	FUNC_MAP_INSERT(Key("SampleGrad"), Savvy::Internal::FuncConverterHLSLToGLSL::Convert_Texturing);
	FUNC_MAP_INSERT(Key("SampleBias"), Savvy::Internal::FuncConverterHLSLToGLSL::Convert_Texturing);
	FUNC_MAP_INSERT(Key("Load"), Savvy::Internal::FuncConverterHLSLToGLSL::Convert_Texturing);
	FUNC_MAP_INSERT(Key("SampleCmp"), Savvy::Internal::FuncConverterHLSLToGLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("SampleCmpLevelZero"), Savvy::Internal::FuncConverterHLSLToGLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("GetDimensions"), Savvy::Internal::FuncConverterHLSLToGLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("GetSamplePosition"), Savvy::Internal::FuncConverterHLSLToGLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("CalculateLevelOfDetail"), Savvy::Internal::FuncConverterHLSLToGLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("Gather"), Savvy::Internal::FuncConverterHLSLToGLSL::Convert_TexGather);
}

Savvy::Internal::FuncConverterHLSLToGLSL::~FuncConverterHLSLToGLSL()
{

}

Savvy::ResultCode Savvy::Internal::FuncConverterHLSLToGLSL::Convert_Interlocked_Operations(Database::WordMultiMap::iterator& it1, std::string& currentLine)
{
	Key keyword = it1->second.m_Name;

	StringList argList;
	ResultCode argRes = GetFuncArgumentList(it1, argList);

	if (argRes != SAVVY_OK)
		return argRes;

	uint32 numArgs = argList.size();

	if (numArgs > 3 || numArgs < 2)
		return SAVVY_SHADER_SYNTAX_ERROR;

	if (numArgs == 3)
	{
		std::string outputName = argList[2];
		std::string varName = argList[0] + argList[1];

		if (keyword == "InterlockedAdd")
			currentLine.append(outputName + " = atomicAdd(" + varName + ")");
		else if (keyword == "InterlockedAnd")
			currentLine.append(outputName + " = atomicAnd(" + varName + ")");
		else if (keyword == "InterlockedMax")
			currentLine.append(outputName + " = atomicMax(" + varName + ")");
		else if (keyword == "InterlockedMin")
			currentLine.append(outputName + " = atomicMin(" + varName + ")");
		else if (keyword == "InterlockedOr")
			currentLine.append(outputName + " = atomicOr(" + varName + ")");
		else if (keyword == "InterlockedExchange")
			currentLine.append(outputName + " = atomicExchange(" + varName + ")");
		else
			currentLine.append(outputName + " = atomicXor(" + varName + ")");
	}
	else if (numArgs == 2)
	{
		std::string varName = argList[0] + argList[1];
		if (keyword == "InterlockedAdd")
			currentLine.append("atomicAdd(" + varName + ")");
		else if (keyword == "InterlockedAnd")
			currentLine.append("atomicAnd(" + varName + ")");
		else if (keyword == "InterlockedMin")
			currentLine.append("atomicMin(" + varName + ")");
		else if (keyword == "InterlockedMax")
			currentLine.append("atomicMax(" + varName + ")");
		else if (keyword == "InterlockedOr")
			currentLine.append("atomicOr(" + varName + ")");
		else if (keyword == "InterlockedExchange")
			currentLine.append("atomicExchange(" + varName + ")");
		else
			currentLine.append("atomicXor(" + varName + ")");
	}

	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::FuncConverterHLSLToGLSL::Convert_Saturate(Database::WordMultiMap::iterator& it1, std::string& currentLine)
{
	Key keyword = it1->second.m_Name;

	StringList argList;
	ResultCode argRes = GetFuncArgumentList(it1, argList);

	if (argRes != SAVVY_OK)
		return argRes;

	uint32 numArgs = argList.size();

	if (numArgs != 1)
		return SAVVY_SHADER_SYNTAX_ERROR;

	currentLine.append("clamp("+ argList[0] + ", 0.0, 1.0)");
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::FuncConverterHLSLToGLSL::Convert_Interlocked_CompSwap(Database::WordMultiMap::iterator& it1, std::string& currentLine)
{
	Key keyword = it1->second.m_Name;
	it1++;

	if (it1->second.m_Name != "(")
	{
		return SAVVY_SHADER_SYNTAX_ERROR;
	}
	uint32 openBrackets = 1;
	uint32 closeBrackets = 0;
	uint32 ctrAhead = 0;
	bool stop = false;
	std::string varName;
	while (!stop)
	{
		ctrAhead++;
		it1++;

		if (it1->second.m_Name == ";")
		{
			return SAVVY_SHADER_SYNTAX_ERROR;
		}
		else if (it1->second.m_Name == ")")
		{
			closeBrackets++;

			if (closeBrackets == openBrackets)
			{
				stop = true;
			}
		}
		else if (it1->second.m_Name == "(")
		{
			openBrackets++;
		}

		if (it1->second.m_Name != ")" || (it1->second.m_Name == ")" && closeBrackets != openBrackets))
		{
			std::string add;
			if (it1->second.m_Translate)
			{
				if (m_Database->OutputLangEquivalentExists(it1->second.m_Name))
				{
					const Key& k = m_Database->GetOutputLangEquivalent(it1->second.m_Name);
					add = k.GetString();
				}
				else
				{
					return SAVVY_LOGICAL_ERROR;
				}
			}
			else
			{
				add = it1->second.m_Name.GetString();
			}
			varName.append(add);
		}
	}

	uint32 timesComma = StringContainsCharTimes(varName, ',');
	if (timesComma == 3)
	{
		uint32 lastComma = varName.rfind(",");
		std::string outputName = varName.substr(lastComma + 1);
		varName = varName.substr(0, lastComma);

		currentLine.append(outputName + " = atomicCompSwap(" + varName + ")");
	}
	else if (timesComma == 2)
	{
		currentLine.append("atomicCompSwap(" + varName + ")");
	}
	else
	{
		return SAVVY_SHADER_SYNTAX_ERROR;
	}

	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::FuncConverterHLSLToGLSL::Convert_Atan2(Database::WordMultiMap::iterator& it1, std::string& currentLine)
{
	StringList argList;
	ResultCode argRes = GetFuncArgumentList(it1, argList);

	if (argRes != SAVVY_OK)
		return argRes;

	uint32 numArgs = argList.size();

	if (numArgs != 2)
		return SAVVY_SHADER_SYNTAX_ERROR;

	std::string firstArg = argList[0];
	std::string secondArg = argList[1];

	std::string varName = firstArg + secondArg;
	if (!secondArg.empty())
	{
		currentLine.append("atan(" + secondArg + ", " + firstArg + ")"); // Swap them for GLSL
	}
	else
	{
		return SAVVY_SHADER_SYNTAX_ERROR;
	}

	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::FuncConverterHLSLToGLSL::Convert_Unsupported(Database::WordMultiMap::iterator& it1, std::string& currentLine)
{
	return SAVVY_UNSUPPORTED_FUNCTION;
}

Savvy::ResultCode Savvy::Internal::FuncConverterHLSLToGLSL::Convert_Fmod(Database::WordMultiMap::iterator& it1, std::string& currentLine)
{
	StringList argList;
	ResultCode argRes = GetFuncArgumentList(it1, argList);

	if (argRes != SAVVY_OK)
		return argRes;

	uint32 numArgs = argList.size();

	if (numArgs != 2)
		return SAVVY_SHADER_SYNTAX_ERROR;

	std::string firstArg = argList[0];
	std::string secondArg = argList[1];

	currentLine.append("(" + firstArg + " - " + secondArg + " * trunc(" + firstArg + " / " + secondArg + ")");

	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::FuncConverterHLSLToGLSL::Convert_Log10(Database::WordMultiMap::iterator& it1, std::string& currentLine)
{
	StringList argList;
	ResultCode argRes = GetFuncArgumentList(it1, argList);

	if (argRes != SAVVY_OK)
		return argRes;

	uint32 numArgs = argList.size();

	if (numArgs != 1)
		return SAVVY_SHADER_SYNTAX_ERROR;

	std::string varName = argList[0];
	currentLine.append("(log(" + varName + ") / log(10))");
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::FuncConverterHLSLToGLSL::Convert_Mad(Database::WordMultiMap::iterator& it1, std::string& currentLine)
{
	StringList argList;
	ResultCode argRes = GetFuncArgumentList(it1, argList);

	if (argRes != SAVVY_OK)
		return argRes;

	uint32 numArgs = argList.size();

	if (numArgs != 3)
		return SAVVY_SHADER_SYNTAX_ERROR;

	std::string firstArg = argList[0];
	std::string secondArg = argList[1];
	std::string thirdArg = argList[2];
	currentLine.append("(" + firstArg + " * " + secondArg + " + " + thirdArg + ")");

	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::FuncConverterHLSLToGLSL::Convert_Noise(Database::WordMultiMap::iterator& it1, std::string& currentLine)
{
	
	StringList argList;
	ResultCode argRes = GetFuncArgumentList(it1, argList);

	if (argRes != SAVVY_OK)
		return argRes;

	uint32 numArgs = argList.size();

	if (numArgs != 1)
		return SAVVY_SHADER_SYNTAX_ERROR;

	std::string varName = argList[0];

	bool cond = false;
	for (uint32 i = 2; i < 5; i++)
	{
		std::string numString = std::to_string(i);

		Database::KeyList list1;
		list1.push_back("float" + numString);
		list1.push_back("double" + numString);
		list1.push_back("int" + numString);
		list1.push_back("uint" + numString);
		cond = varName.find("float" + numString) != std::string::npos ||
			varName.find("uint" + numString) != std::string::npos ||
			varName.find("int" + numString) != std::string::npos ||
			varName.find("double" + numString) != std::string::npos ||
			m_Database->FindVariableWithType(Key(varName), m_CurrFunction, list1);

		if (cond)
		{
			currentLine.append("noise" + numString + "(" + varName + ")");
			break;
		}
	}

	if (!cond)
	{
		currentLine.append("noise(" + varName + ")");
	}

	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::FuncConverterHLSLToGLSL::ConvertFunction(Database::WordMultiMap::iterator& it1, std::string& currentLine, Key& a_FuncName, uint32 a_Shader, Key& a_InputStructName /*= Key()*/, Key& a_OutputStructName /*= Key()*/)
{
	Key keyword = it1->second.m_Name;
	uint32 count = m_FuncConvertMap.count(keyword);
	m_CurrFunction = a_FuncName;
	m_InputStructName = a_InputStructName;
	m_OutputStructName = a_OutputStructName;
	m_CurrShader = a_Shader;
	if (count < 1)
	{
		return SAVVY_SHADER_SYNTAX_ERROR;
	}
	ResultCode res = m_FuncConvertMap.at(keyword)(it1, currentLine);

	return res;
}

Savvy::ResultCode Savvy::Internal::FuncConverterHLSLToGLSL::GetFuncArgumentList(Database::WordMultiMap::iterator& it1, StringList& a_OutputList)
{
	Database::WordMultiMap& instructionMap = m_Database->GetInstructionMap();
	Database::WordMultiMap::iterator endIt = instructionMap.upper_bound(m_CurrFunction);

	if (it1 == endIt)
		return SAVVY_SHADER_SYNTAX_ERROR;

	it1++;

	if (it1->second.m_Name != "(")
	{
		return SAVVY_SHADER_SYNTAX_ERROR;
	}

	StringList retList;
	retList.push_back(std::string());
	uint32 openBrackets = 1;
	uint32 closeBrackets = 0;
	uint32 ctrAhead = 0;
	while (it1 != endIt)
	{
		ctrAhead++;
		it1++;

		Database::Word& currWord = it1->second;

		if (currWord.m_Name == ";")
		{
			return SAVVY_SHADER_SYNTAX_ERROR;
		}
		else if (currWord.m_Name == ")")
		{
			closeBrackets++;

			if (closeBrackets == openBrackets)
			{
				break;
			}
		}
		else if (currWord.m_Name == "(")
		{
			openBrackets++;
		}

		if (currWord.m_Name == ",")
		{
			if (closeBrackets == openBrackets - 1)
			{
				retList.push_back(std::string()); // Insert new argument
			}
			else
			{
				std::string add;
				if (currWord.m_Translate)
				{
					// Check if it's a cast
					if (it1 != instructionMap.lower_bound(m_CurrFunction))
					{
						it1--;
						if (it1->second.m_Name == "(")
						{
							it1++; it1++;

							if (it1->second.m_Name == ")")
							{
								uint32 pos = retList.back().rfind("(");
								retList.back().erase(pos, pos + 1);
								// C-style casts not allowed in GLSL - turn them into constructors
								it1->second.m_Name.SetString("(");
								it1++;
								it1->second.m_Name.SetString(it1->second.m_Name.GetString() + ") ");
								it1--;
							}
							it1--; it1--;
						}
						it1++;
					}

					if (m_Database->OutputLangEquivalentExists(currWord.m_Name))
					{
						const Key& k = m_Database->GetOutputLangEquivalent(currWord.m_Name);
						add = k.GetString();
					}
					else
					{
						if (currWord.m_Name != "mul")
						{
							// Handle variables named the same as built in functions
							it1++;
							if (m_Database->VariableWithNameExists(currWord.m_Name, m_CurrFunction) && it1->second.m_Name != "(")
							{
								add.append(currWord.m_Name.GetString() + " ");
								it1--;
							}
							else
							{
								it1--;
								ResultCode convRes = ConvertFunction(it1, retList.back(), m_CurrFunction, m_CurrShader, m_InputStructName, m_OutputStructName);

								if (convRes != SAVVY_OK)
									return convRes;
							}
						}
						else
						{
							ResultCode convRes = HandleMatrixMultiplication(it1, retList.back());

							if (convRes != SAVVY_OK)
								return convRes;
						}
					}
				}
				else
				{
					// Get rid of struct initialization
					if (currWord.m_Name == m_InputStructName || currWord.m_Name == m_OutputStructName)
					{
						currWord.m_Name.Clear();
						it1++;
						if (it1->second.m_Name == ".")
						{
							it1->second.m_Name.Clear();

							if (currWord.m_Name == m_InputStructName)
							{
								it1++;
								// Check if we renamed this one
								while (!m_Database->StructVariableExists(m_InputStructName, it1->second.m_Name))
								{
									it1->second.m_Name.SetString("in_" + it1->second.m_Name.GetString());
								}
								it1--;
							}
							else if (!m_OutputStructName.IsEmpty())
							{
								it1++;
								// Check if we renamed this one
								while (!m_Database->StructVariableExists(m_OutputStructName, it1->second.m_Name))
								{
									it1->second.m_Name.SetString("out_" + it1->second.m_Name.GetString());
								}

								it1--;
							}
							
						}
						it1--;
					}
					add = currWord.m_Name.GetString();
				}
				retList.back().append(add);
			}
		}
		else
		{
			std::string add;
			if (currWord.m_Translate)
			{
				// Check if it's a cast
				if (it1 != instructionMap.lower_bound(m_CurrFunction))
				{
					it1--;
					if (it1->second.m_Name == "(")
					{
						it1++; it1++;

						if (it1->second.m_Name == ")")
						{
							uint32 pos = retList.back().rfind("(");
							retList.back().erase(pos, pos + 1);
							// C-style casts not allowed in GLSL - turn them into constructors
							it1->second.m_Name.SetString("(");
							it1++;
							it1->second.m_Name.SetString(it1->second.m_Name.GetString() + ") ");
							it1--;
						}
						it1--; it1--;
					}
					it1++;
				}

				if (m_Database->OutputLangEquivalentExists(currWord.m_Name))
				{
					const Key& k = m_Database->GetOutputLangEquivalent(currWord.m_Name);
					add = k.GetString();
				}
				else
				{
					if (currWord.m_Name != "mul")
					{
						// Handle variables named the same as built in functions
						it1++;
						if (m_Database->VariableWithNameExists(currWord.m_Name, m_CurrFunction) && it1->second.m_Name != "(")
						{
							add.append(currWord.m_Name.GetString() + " ");
							it1--;
						}
						else
						{
							it1--;
							ResultCode convRes = ConvertFunction(it1, retList.back(), m_CurrFunction, m_CurrShader, m_InputStructName, m_OutputStructName);

							if (convRes != SAVVY_OK)
								return convRes;
						}					
					}
					else
					{
						ResultCode convRes = HandleMatrixMultiplication(it1, retList.back());

						if (convRes != SAVVY_OK)
							return convRes;
					}
				}
			}
			else
			{
				// Get rid of struct initialization
				if ( (currWord.m_Name == m_InputStructName && !m_InputStructName.IsEmpty()) || 
					(currWord.m_Name == m_OutputStructName && !m_OutputStructName.IsEmpty()))
				{
					currWord.m_Name.Clear();
					it1++;
					if (it1->second.m_Name == ".")
					{
						it1->second.m_Name.Clear();

						if (currWord.m_Name == m_InputStructName)
						{
							it1++;
							// Check if we renamed this one
							while (!m_Database->StructVariableExists(m_InputStructName, it1->second.m_Name))
							{
								it1->second.m_Name.SetString("in_" + it1->second.m_Name.GetString());
							}
							
							it1--;
						}
					}
					else
					{
						it1++;
						// Check if we renamed this one
						while (!m_Database->StructVariableExists(m_OutputStructName, it1->second.m_Name))
						{
							it1->second.m_Name.SetString("out_" + it1->second.m_Name.GetString());
						}

						it1--;
					}
					it1--;
				}
				add = currWord.m_Name.GetString();
			}
			retList.back().append(add);
		}
	}

	if (it1 == endIt)
		return SAVVY_SHADER_SYNTAX_ERROR;

	a_OutputList.swap(retList);

	return SAVVY_OK;
}

const bool Savvy::Internal::FuncConverterHLSLToGLSL::IsBuiltInGLSLVariable(Key& a_Name)
{
	if (a_Name == "gl_Position" || a_Name == "gl_FragDepth" ||
		a_Name == "gl_PointSize" || a_Name.Contains("gl_FragColor") ||
		a_Name == "gl_FrontFacing" || a_Name.Contains("gl_CullDistance") ||
		a_Name.Contains("gl_ClipDistance"))
	{

		return true;
	}
	return false;
}

Savvy::ResultCode Savvy::Internal::FuncConverterHLSLToGLSL::Convert_Texturing(Database::WordMultiMap::iterator& it1, std::string& currentLine)
{
	Key keyword = it1->second.m_Name;

	it1--; it1--;
	Key texObjName = it1->second.m_Name;
	it1++; it1++;

	StringList argList;
	ResultCode argRes = GetFuncArgumentList(it1, argList);

	if (argRes != SAVVY_OK)
		return argRes;

	uint32 numArgs = argList.size();

	if (numArgs < 1 || numArgs > 5)
		return SAVVY_SHADER_SYNTAX_ERROR;

	std::string firstArg = argList[0];
	std::string secondArg;

	if (numArgs > 1)
		secondArg = argList[1];

	std::string thirdArg;
	std::string fourthArg;
	std::string fifthArg;

	// Remove the object
	uint32 pos = currentLine.rfind(texObjName.GetString());

	if (pos != std::string::npos)
	{
		std::string oldLine = currentLine.substr(0, pos);
		currentLine = oldLine;
	}

	if (numArgs >= 3)
	{
		thirdArg = argList[2];
	}
	if (numArgs >= 4)
	{
		fourthArg = argList[3];
		uint32 pos = fourthArg.find("(");
		if ( pos!= std::string::npos)
		{
			fourthArg = fourthArg.substr(pos);
			fourthArg = Constructor::ReplaceAll(fourthArg, "(", "");
			fourthArg = Constructor::ReplaceAll(fourthArg, ")", "");
			fourthArg = fourthArg.substr(0, fourthArg.find(","));
		}
	}
	if (numArgs == 5)
	{
		fifthArg = argList[4];

		uint32 pos = fifthArg.find("(");
		if (pos != std::string::npos)
		{
			fifthArg = fifthArg.substr(pos);
			fifthArg = Constructor::ReplaceAll(fifthArg, "(", "");
			fifthArg = Constructor::ReplaceAll(fifthArg, ")", "");
			fifthArg = fifthArg.substr(0, fifthArg.find(","));
		}
	}

	if (keyword == "Sample")
	{
		if (numArgs == 2)
		{
			currentLine.append("texture(" + texObjName.GetString() + "," + secondArg + ")");
		}
		else if (numArgs == 3)
		{
			uint32 pos = thirdArg.find("(");
			if (pos != std::string::npos)
			{
				thirdArg = thirdArg.substr(pos);
				thirdArg = Constructor::ReplaceAll(thirdArg, "(", "");
				thirdArg = Constructor::ReplaceAll(thirdArg, ")", "");
				thirdArg = thirdArg.substr(0, thirdArg.find(","));
			}
			currentLine.append("texture(" + texObjName.GetString() + "," + secondArg + "," + thirdArg + ")");
		}
	}
	else if (keyword == "SampleLevel")
	{
		if (numArgs == 4)
			currentLine.append("texture(" + texObjName.GetString() + "," + secondArg + "," + fourthArg + ")");
		else
		{
			currentLine.append("texture(" + texObjName.GetString() + "," + secondArg + ")");
		}
	}
	else if (keyword == "SampleGrad")
	{
		if (numArgs == 4)
		{
			currentLine.append("textureGrad(" + texObjName.GetString() + "," + secondArg + "," + thirdArg + "," + fourthArg + ")");
		}
		else if (numArgs == 5)
		{
			currentLine.append("textureGradOffset(" + texObjName.GetString() + "," + secondArg + "," + thirdArg + "," + fourthArg + "," + fifthArg + ")");
		}
	}
	else if (keyword == "Load")
	{
		if (numArgs == 1)
		{
			currentLine.append("texelFetch(" + texObjName.GetString() + "," + firstArg + ",0)");
		}
	}
	else if (keyword == "SampleBias")
	{
		if (numArgs == 3)
		{
			currentLine.append("textureOffset(" + texObjName.GetString() + "," + secondArg + "," + thirdArg + ")");
		}
		else if (numArgs == 4)
		{
			currentLine.append("textureOffset(" + texObjName.GetString() + "," + secondArg + "," + thirdArg + "," + fourthArg + ")");
		}
	}
	else if (keyword == "GetDimensions")
	{
		it1++;
		it1->second.m_Name.Clear(); // Get rid of semicolon
		Database::WordMultiMap::iterator returnIt = it1;
		Database::WordMultiMap::iterator endIt = m_Database->GetInstructionMap().upper_bound(m_CurrFunction);

		// Go through all the instructions and replace the arguments with the function call "textureSize"
		while (it1 != endIt)
		{
			if (it1->second.m_Name == firstArg)
			{
				it1->second.m_Name.Clear();
				Database::WordMultiMap::iterator retIT = it1;
				it1--;
				bool vecBlock = false;
				if (it1->second.m_Name == "(")
				{
					it1--;
					if (it1->second.m_Name == "float2")
					{
						it1->second.m_Name.Clear();
						it1++;
						it1->second.m_Name.Clear();
						vecBlock = true;
					}
				}
				it1 = retIT;

				it1++;
				if (it1->second.m_Name == ",")
				{
					it1->second.m_Name.Clear();
					it1++;
					if (it1->second.m_Name == secondArg)
					{
						it1->second.m_Name = "textureSize(" + texObjName.GetString() + ", 0)";

						if (vecBlock)
						{
							it1++;
							it1->second.m_Name.Clear();
						}
					}
				}
				else
				{
					it1--;
					it1->second.m_Name = "textureSize(" + texObjName.GetString() + ", 0).x";
				}
			}
			else if (it1->second.m_Name == secondArg)
			{
				it1->second.m_Name = "textureSize(" + texObjName.GetString() + ", 0).y";
			}

			it1++;
		}
		it1 = returnIt;
	}

	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::FuncConverterHLSLToGLSL::Convert_TexGather(Database::WordMultiMap::iterator& it1, std::string& currentLine)
{
	Key keyword = it1->second.m_Name;

	it1--; it1--;
	Key texObjName = it1->second.m_Name;
	it1++; it1++;

	StringList argList;
	ResultCode argRes = GetFuncArgumentList(it1, argList);

	if (argRes != SAVVY_OK)
		return argRes;

	uint32 numArgs = argList.size();

	if (numArgs < 2 || numArgs > 3)
		return SAVVY_SHADER_SYNTAX_ERROR;

	std::string firstArg = argList[0];
	std::string secondArg = argList[1];
	std::string thirdArg;

	// Remove the object
	uint32 pos = currentLine.find(texObjName.GetString());

	if (pos != std::string::npos)
	{
		std::string oldLine = currentLine.substr(0, pos);
		currentLine = oldLine;
	}

	if (numArgs >= 3)
	{
		thirdArg = argList[2];

		uint32 pos = thirdArg.find("(");
		if (pos != std::string::npos)
		{
			thirdArg = thirdArg.substr(pos);
			thirdArg = Constructor::ReplaceAll(thirdArg, "(", "");
			thirdArg = Constructor::ReplaceAll(thirdArg, ")", "");
			thirdArg = thirdArg.substr(0, thirdArg.find(","));
		}
	}

	if (keyword == "Gather")
	{
		if (numArgs == 2)
		{
			currentLine.append("textureGather(" + texObjName.GetString() + "," + secondArg + ")");
		}
		else if (numArgs == 3)
		{
			currentLine.append("textureGather(" + texObjName.GetString() + "," + secondArg + "," + thirdArg + ")");
		}
	}

	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::FuncConverterHLSLToGLSL::HandleMatrixMultiplication(Database::WordMultiMap::iterator& it1, std::string& currentLine)
{
	Database::Word word = it1->second;
	if (word.m_Name == "mul")
	{
		uint32 ctrAhead = 0;
		uint32 openBrackets = 0;
		uint32 closeBrackets = 0;
		bool stop = false;
		while (!stop)
		{
			it1++;
			ctrAhead++;
			Key& currKey = it1->second.m_Name;

			if (currKey == ";")
			{
				return SAVVY_SHADER_SYNTAX_ERROR;
			}
			else if (currKey == "(")
			{
				openBrackets++;
			}
			else if (currKey == ")")
			{
				closeBrackets++;

				if (closeBrackets == openBrackets)
				{
					currKey.Clear(); // Get rid of closing bracket
					stop = true;
				}
			}
			else if (currKey == ",")
			{
				// Check scope
				if (closeBrackets == openBrackets - 1)
				{
					// Replace comma with multiplication
					currKey.SetString("*");
				}
			}

			if (ctrAhead == 1)
			{
				// Compulsory open bracket here
				if (currKey != "(")
				{
					return SAVVY_SHADER_SYNTAX_ERROR;
				}

				currKey.Clear();
			}
		}

		// Return back to proper place
		for (uint32 i = 0; i < ctrAhead; ++i)
		{
			it1--;
		}
	}
	return SAVVY_OK;
}

void Savvy::Internal::FuncConverterHLSLToGLSL::Reset()
{

}
