#include "SavvyPCH.h"
#include "SavvyFuncConverterGLSLToHLSL.h"
#include "SavvyConstructor.h"

#define FUNC_MAP_INSERT(a, b) m_FuncConvertMap.insert(std::make_pair(a, std::bind(&b, this, std::placeholders::_1, std::placeholders::_2)))
Savvy::Internal::FuncConverterGLSLToHLSL::FuncConverterGLSLToHLSL() : m_AtomicCtr(0), m_BitfieldExtractCtr(0), m_BitfieldInsertCtr(0), m_TextureSizeCtr(0)
{
	FUNC_MAP_INSERT(Key("acosh"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Acosh_Asinh_Atanh);
	FUNC_MAP_INSERT(Key("asinh"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Acosh_Asinh_Atanh);
	FUNC_MAP_INSERT(Key("atanh"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Acosh_Asinh_Atanh);
	FUNC_MAP_INSERT(Key("atomicAdd"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Atomic_Operations);
	FUNC_MAP_INSERT(Key("atomicAnd"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Atomic_Operations);
	FUNC_MAP_INSERT(Key("atomicExchange"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Atomic_Operations);
	FUNC_MAP_INSERT(Key("atomicMin"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Atomic_Operations);
	FUNC_MAP_INSERT(Key("atomicMax"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Atomic_Operations);
	FUNC_MAP_INSERT(Key("atomicOr"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Atomic_Operations);
	FUNC_MAP_INSERT(Key("atomicXor"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Atomic_Operations);
	FUNC_MAP_INSERT(Key("atomicCompSwap"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Atomic_CompSwap);
	FUNC_MAP_INSERT(Key("atomicCounter"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("atomicCounterDecrement"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("atomicCounterIncrement"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("atan"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Atan);
	FUNC_MAP_INSERT(Key("barrier"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("bitfieldExtract"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_BitfieldExtract);
	FUNC_MAP_INSERT(Key("bitfieldInsert"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_BitfieldInsert);
	FUNC_MAP_INSERT(Key("EmitStreamVertex"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("EmitVertex"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("EndPrimitive"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("EndStreamPrimitive"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("equal"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Vec_Comparisons);
	FUNC_MAP_INSERT(Key("mod"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Mod);
	FUNC_MAP_INSERT(Key("fwidthCoarse"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("fwidthFine"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("greaterThanEqual"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Vec_Comparisons);
	FUNC_MAP_INSERT(Key("greaterThan"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Vec_Comparisons);
	FUNC_MAP_INSERT(Key("imageAtomicAdd"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("imageAtomicAnd"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("imageAtomicCompSwap"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("imageAtomicExchange"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("imageAtomicMax"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("imageAtomicMin"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("imageAtomicOr"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("imageAtomicXor"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("imageLoad"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("imageSamples"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("imageSize"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("imageStore"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("imulExtended"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("inverse"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("lessThan"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Vec_Comparisons);
	FUNC_MAP_INSERT(Key("lessThanEqual"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Vec_Comparisons);
	FUNC_MAP_INSERT(Key("matrixCompMult"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_MatrixCompMult);
	FUNC_MAP_INSERT(Key("memoryBarrierAtomicCounter"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("memoryBarrierBuffer"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("memoryBarrierImage"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("memoryBarrierShared"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("memoryBarrierAtomicCounter"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("notEqual"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Vec_Comparisons);
	FUNC_MAP_INSERT(Key("not"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Not);
	FUNC_MAP_INSERT(Key("packDouble2x32"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("packHalf2x16"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("packSnorm2x16"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("packSnorm4x8"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("packUnorm"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("packUnorm2x16"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("packUnorm4x8"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("texelFetch"), Savvy::Internal::FuncConverterGLSLToHLSL::ConvertTexturing);
	FUNC_MAP_INSERT(Key("texelFetchOffset"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("textureGather"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("textureGatherOffset"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("textureGatherOffsets"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("textureGrad"), Savvy::Internal::FuncConverterGLSLToHLSL::ConvertTexturing);
	FUNC_MAP_INSERT(Key("textureGradOffset"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("textureLod"), Savvy::Internal::FuncConverterGLSLToHLSL::ConvertTexturing);
	FUNC_MAP_INSERT(Key("textureLodOffset"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("textureOffset"), Savvy::Internal::FuncConverterGLSLToHLSL::ConvertTexturing);
	FUNC_MAP_INSERT(Key("textureProj"), Savvy::Internal::FuncConverterGLSLToHLSL::ConvertTexturing);
	FUNC_MAP_INSERT(Key("texture"), Savvy::Internal::FuncConverterGLSLToHLSL::ConvertTexturing);
	FUNC_MAP_INSERT(Key("textureProjGrad"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("textureProjGradOffset"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("textureProjLod"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("textureProjLodOffset"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("textureProjOffset"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("textureQueryLevels"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("textureQueryLod"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("textureSamples"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("textureSize"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_TextureSize);
	FUNC_MAP_INSERT(Key("uaddCarry"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("umulExtended"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("unpackDouble2x32"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("unpackHalf2x16"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("unpackSnorm4x8"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("unpackUnorm"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("unpackUnorm2x16"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("unpackUnorm4x8"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("usubBorrow"), Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Unsupported);
	FUNC_MAP_INSERT(Key("vec2"), Savvy::Internal::FuncConverterGLSLToHLSL::ConvertVectors);
	FUNC_MAP_INSERT(Key("vec3"), Savvy::Internal::FuncConverterGLSLToHLSL::ConvertVectors);
	FUNC_MAP_INSERT(Key("vec4"), Savvy::Internal::FuncConverterGLSLToHLSL::ConvertVectors);
	FUNC_MAP_INSERT(Key("ivec2"), Savvy::Internal::FuncConverterGLSLToHLSL::ConvertVectors);
	FUNC_MAP_INSERT(Key("ivec3"), Savvy::Internal::FuncConverterGLSLToHLSL::ConvertVectors);
	FUNC_MAP_INSERT(Key("ivec4"), Savvy::Internal::FuncConverterGLSLToHLSL::ConvertVectors);
	FUNC_MAP_INSERT(Key("uvec2"), Savvy::Internal::FuncConverterGLSLToHLSL::ConvertVectors);
	FUNC_MAP_INSERT(Key("uvec3"), Savvy::Internal::FuncConverterGLSLToHLSL::ConvertVectors);
	FUNC_MAP_INSERT(Key("uvec4"), Savvy::Internal::FuncConverterGLSLToHLSL::ConvertVectors);
	FUNC_MAP_INSERT(Key("bvec2"), Savvy::Internal::FuncConverterGLSLToHLSL::ConvertVectors);
	FUNC_MAP_INSERT(Key("bvec3"), Savvy::Internal::FuncConverterGLSLToHLSL::ConvertVectors);
	FUNC_MAP_INSERT(Key("bvec4"), Savvy::Internal::FuncConverterGLSLToHLSL::ConvertVectors);
}

Savvy::Internal::FuncConverterGLSLToHLSL::~FuncConverterGLSLToHLSL()
{

}

Savvy::ResultCode Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Acosh_Asinh_Atanh(Database::WordMultiMap::iterator& it1, std::string& currentLine)
{
	Key keyword = it1->second.m_Name;

	StringList argList;
	ResultCode argRes = GetFuncArgumentList(it1, argList, currentLine);

	if (argRes != SAVVY_OK)
		return argRes;

	uint32 numArgs = argList.size();

	if (numArgs != 1)
		return SAVVY_SHADER_SYNTAX_ERROR;

	std::string varName = argList[0];
	std::string str;
	if (keyword == "asinh")
	{
		str = "(log(" + varName + " + sqrt(" + varName + " * " + varName + " + 1.0)))";
	}
	else if (keyword == "acosh")
	{
		str = "(log(" + varName + " + sqrt(" + varName + " * " + varName + " - 1.0)))";
	}
	else if (keyword == "atanh")
	{
		str = "(log((1.0 + " + varName + ") / (1.0 - " + varName + ")) / 2.0)";
	}
	else
	{
		return SAVVY_LOGICAL_ERROR;
	}
	currentLine.append(str);

	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Atomic_Operations(Database::WordMultiMap::iterator& it1, std::string& currentLine)
{
	Database::WordMultiMap& instructionMap = m_Database->GetInstructionMap();
	Key keyword = it1->second.m_Name;
	Key name;
	Key extraLine;
	bool nameDeclaredOnthisLine = false;
	it1--;

	if (it1->second.m_Name == "=")
	{
		it1--;
		name = it1->second.m_Name;
		currentLine.clear();

		it1--;

		if (it1->second.m_Name == "uint" || it1->second.m_Name == "int")
		{
			currentLine.append(it1->second.m_Name.GetString() + " " + name.GetString() + ";\n");
			nameDeclaredOnthisLine = true;
		}
		it1++;

		it1++;
	}
	else
	{
		// What if we're doing an operation on it?
		uint32 equalsPos = currentLine.rfind("=");

		if (equalsPos != std::string::npos)
		{
			uint32 ctr = 0;
			while (it1->second.m_Name != "=")
			{
				ctr++;
				it1--;
			}

			it1--;
			name = it1->second.m_Name;
			extraLine = currentLine.substr(equalsPos + 1, currentLine.size());
			currentLine = currentLine.substr(0, equalsPos);
			currentLine.append(";\n");
			it1++;

			// return
			for (uint32 i = 0; i < ctr; i++)
			{
				it1++;
			}
		}
	}

	it1++;

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
				it1->second.m_Name.Clear();
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

	if (!name.IsEmpty())
	{
		varName.append(", " + name.GetString());

		if (nameDeclaredOnthisLine)
			currentLine.append("\t");
	}

	if (keyword == "atomicAdd")
		currentLine.append("InterlockedAdd(" + varName + ")");
	else if (keyword == "atomicAnd")
		currentLine.append("InterlockedAnd(" + varName + ")");
	else if (keyword == "atomicExchange")
		currentLine.append("InterlockedExchange(" + varName + ")");
	else if (keyword == "atomicMax")
		currentLine.append("InterlockedMax(" + varName + ")");
	else if (keyword == "atomicMin")
		currentLine.append("InterlockedMin(" + varName + ")");
	else if (keyword == "atomicOr")
		currentLine.append("InterlockedOr(" + varName + ")");
	else
		currentLine.append("InterlockedXor(" + varName + ")");

	if (!extraLine.IsEmpty())
		currentLine.append("\t" + name.GetString() + " =" + extraLine.GetString() + name.GetString());

	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Atomic_CompSwap(Database::WordMultiMap::iterator& it1, std::string& currentLine)
{
	Database::WordMultiMap& instructionMap = m_Database->GetInstructionMap();
	Key keyword = it1->second.m_Name;
	Key name;
	Key extraLine;
	bool nameDeclaredOnthisLine = false;
	bool equalFound = false;
	it1--;

	if (it1->second.m_Name == "=")
	{
		equalFound = true;
		it1--;
		name = it1->second.m_Name;
		currentLine.clear();

		it1--;

		if (it1->second.m_Name == "uint" || it1->second.m_Name == "int")
		{
			currentLine.append(it1->second.m_Name.GetString() + " " + name.GetString() + ";\n");
			nameDeclaredOnthisLine = true;
		}
		it1++;

		it1++;
	}
	else
	{
		// What if we're doing an operation on it?
		uint32 equalsPos = currentLine.rfind("=");

		if (equalsPos != std::string::npos)
		{
			uint32 ctr = 0;
			while (it1->second.m_Name != "=")
			{
				ctr++;
				it1--;
			}

			it1--;
			name = it1->second.m_Name;
			extraLine = currentLine.substr(equalsPos + 1, currentLine.size());
			currentLine = currentLine.substr(0, equalsPos);
			currentLine.append(";\n");
			it1++;

			// return
			for (uint32 i = 0; i < ctr; i++)
			{
				it1++;
			}
		}
	}

	it1++;

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
				it1->second.m_Name.Clear();
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

	if (!name.IsEmpty())
	{
		varName.append(", " + name.GetString());

		if (nameDeclaredOnthisLine)
			currentLine.append("\t");
	}

	if (!equalFound)
	{
		currentLine.append("InterlockedCompareStore(" + varName + ")");

		if (!extraLine.IsEmpty())
			currentLine.append("\t" + name.GetString() + " =" + extraLine.GetString() + name.GetString());
	}
	else
	{
		currentLine.append("InterlockedCompareExchange(" + varName + ")");

		if (!extraLine.IsEmpty())
			currentLine.append("\t" + name.GetString() + " =" + extraLine.GetString() + name.GetString());
	}

	m_AtomicCtr++;
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Unsupported(Database::WordMultiMap::iterator& it1, std::string& currentLine)
{
	return SAVVY_UNSUPPORTED_FUNCTION;
}

Savvy::ResultCode Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Atan(Database::WordMultiMap::iterator& it1, std::string& currentLine)
{
	Key keyword = it1->second.m_Name;

	StringList argList;
	ResultCode argRes = GetFuncArgumentList(it1, argList, currentLine);

	if (argRes != SAVVY_OK)
		return argRes;

	uint32 numArgs = argList.size();

	if (numArgs < 1 || numArgs > 2)
		return SAVVY_SHADER_SYNTAX_ERROR;

	if (numArgs == 2)
	{
		currentLine.append("atan2(" + argList[1] + ", " + argList[0] + ")");
	}
	else
	{
		currentLine.append("atan(" + argList[0] + ")");
	}
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::FuncConverterGLSLToHLSL::Convert_BitfieldExtract(Database::WordMultiMap::iterator& it1, std::string& currentLine)
{
	Key keyword = it1->second.m_Name;

	// Get the previous stuff in the line and save it for later
	std::string oldLine = currentLine;
	currentLine.clear();

	StringList argList;
	ResultCode argRes = GetFuncArgumentList(it1, argList, currentLine);

	if (argRes != SAVVY_OK)
		return argRes;

	uint32 numArgs = argList.size();

	if (numArgs != 3)
		return SAVVY_SHADER_SYNTAX_ERROR;

	std::string firstParam = argList[0];
	std::string secondParam = argList[1];
	std::string thirdParam = argList[2];

	// Construct
	currentLine.append("int BitfieldInsertMask" + std::to_string(m_BitfieldExtractCtr) + " = ");
	currentLine.append("~(0xffffffff << " + thirdParam + ");\n");
	currentLine.append("\tif (" + secondParam + " > 0)\n");
	currentLine.append("\t\t" + firstParam + " = (" + firstParam + " >> " + "(" + secondParam + " - 1)) & " + "BitfieldInsertMask" + std::to_string(m_BitfieldExtractCtr) + ";\n");
	currentLine.append("\telse\n");
	currentLine.append("\t\t" + firstParam + " = " + firstParam + " & " + "BitfieldInsertMask" + std::to_string(m_BitfieldExtractCtr));

	if (!oldLine.empty())
	{
		currentLine.append(";\n\t" + oldLine + " " + firstParam);
	}
	

	m_BitfieldExtractCtr++;
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::FuncConverterGLSLToHLSL::Convert_BitfieldInsert(Database::WordMultiMap::iterator& it1, std::string& currentLine)
{
	Key keyword = it1->second.m_Name;

	// Get the previous stuff in the line and save it for later
	std::string oldLine = currentLine;
	currentLine.clear();

	StringList argList;
	ResultCode argRes = GetFuncArgumentList(it1, argList, currentLine);

	if (argRes != SAVVY_OK)
		return argRes;

	uint32 numArgs = argList.size();

	if (numArgs != 4)
		return SAVVY_SHADER_SYNTAX_ERROR;

	std::string firstParam = argList[0];
	std::string secondParam = argList[1];
	std::string thirdParam = argList[2];
	std::string fourthParam = argList[3];

	// Construct
	currentLine.append("uint BitfieldInsertMask" + std::to_string(m_BitfieldInsertCtr) + " = ");
	currentLine.append("~(0xffffffff << " + fourthParam + ")  << " + thirdParam + ";\n");
	currentLine.append("\tBitfieldInsertMask" + std::to_string(m_BitfieldInsertCtr) + " = " + "~BitfieldInsertMask" + std::to_string(m_BitfieldInsertCtr) + ";\n");
	currentLine.append("\t" + firstParam + " &= " + "BitfieldInsertMask" + std::to_string(m_BitfieldInsertCtr) + ";\n");
	currentLine.append("\t" + firstParam + " = " + firstParam + " | (" + secondParam + " + " + thirdParam + ")");

	if (!oldLine.empty())
	{
		currentLine.append(";\n\t" + oldLine + " " + firstParam);
	}

	m_BitfieldInsertCtr++;
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Vec_Comparisons(Database::WordMultiMap::iterator& it1, std::string& currentLine)
{
	Key keyword = it1->second.m_Name;

	StringList argList;
	ResultCode argRes = GetFuncArgumentList(it1, argList, currentLine);

	if (argRes != SAVVY_OK)
		return argRes;

	uint32 numArgs = argList.size();

	if (numArgs != 2)
		return SAVVY_SHADER_SYNTAX_ERROR;

	std::string firstArg = argList[0];
	std::string secondArg = argList[1];

	// Check the types
	bool cond = false;
	for (uint32 i = 2; i < 5; i++)
	{
		std::string numString = std::to_string(i);

		Database::KeyList list1;
		list1.push_back("vec" + numString);
		list1.push_back("ivec" + numString);
		list1.push_back("uvec" + numString);
		bool cond1 = firstArg.find("vec" + numString) != std::string::npos ||
			firstArg.find("uvec" + numString) != std::string::npos ||
			firstArg.find("ivec" + numString) != std::string::npos ||
			m_Database->FindVariableWithType(Key(firstArg), m_CurrFunction, list1);

		bool cond2 = secondArg.find("vec" + numString) != std::string::npos ||
			secondArg.find("uvec" + numString) != std::string::npos ||
			secondArg.find("ivec" + numString) != std::string::npos ||
			m_Database->FindVariableWithType(Key(firstArg), m_CurrFunction, list1);

		cond = cond1 && cond2;

		// Construct the comparison
		if (cond)
		{
			if (keyword == "equal")
			{
				switch (i)
				{
				case 2:
					currentLine.append("(" + firstArg + ".x == " + secondArg + ".x &&");
					currentLine.append(firstArg + ".y == " + secondArg + ".y)");
					break;

				case 3:
					currentLine.append("(" + firstArg + ".x == " + secondArg + ".x && ");
					currentLine.append(firstArg + ".y == " + secondArg + ".y && ");
					currentLine.append(firstArg + ".z == " + secondArg + ".z)");
					break;

				case 4:
					currentLine.append("(" + firstArg + ".x == " + secondArg + ".x && ");
					currentLine.append(firstArg + ".y == " + secondArg + ".y && ");
					currentLine.append(firstArg + ".z == " + secondArg + ".z && ");
					currentLine.append(firstArg + ".w == " + secondArg + ".w)");
					break;
				}
			}
			else if (keyword == "greaterThan")
			{
				switch (i)
				{
				case 2:
					currentLine.append("(" + firstArg + ".x > " + secondArg + ".x &&");
					currentLine.append(firstArg + ".y > " + secondArg + ".y)");
					break;

				case 3:
					currentLine.append("(" + firstArg + ".x > " + secondArg + ".x && ");
					currentLine.append(firstArg + ".y > " + secondArg + ".y && ");
					currentLine.append(firstArg + ".z > " + secondArg + ".z)");
					break;

				case 4:
					currentLine.append("(" + firstArg + ".x > " + secondArg + ".x && ");
					currentLine.append(firstArg + ".y > " + secondArg + ".y && ");
					currentLine.append(firstArg + ".z > " + secondArg + ".z && ");
					currentLine.append(firstArg + ".w > " + secondArg + ".w)");
					break;
				}
			}
			else if (keyword == "greaterThanEqual")
			{
				switch (i)
				{
				case 2:
					currentLine.append("(" + firstArg + ".x >= " + secondArg + ".x &&");
					currentLine.append(firstArg + ".y >= " + secondArg + ".y)");
					break;

				case 3:
					currentLine.append("(" + firstArg + ".x >= " + secondArg + ".x && ");
					currentLine.append(firstArg + ".y >= " + secondArg + ".y && ");
					currentLine.append(firstArg + ".z >= " + secondArg + ".z)");
					break;

				case 4:
					currentLine.append("(" + firstArg + ".x >= " + secondArg + ".x && ");
					currentLine.append(firstArg + ".y >= " + secondArg + ".y && ");
					currentLine.append(firstArg + ".z >= " + secondArg + ".z && ");
					currentLine.append(firstArg + ".w >= " + secondArg + ".w)");
					break;
				}
			}
			else if (keyword == "lessThanEqual")
			{
				switch (i)
				{
				case 2:
					currentLine.append("(" + firstArg + ".x <= " + secondArg + ".x &&");
					currentLine.append(firstArg + ".y <= " + secondArg + ".y)");
					break;

				case 3:
					currentLine.append("(" + firstArg + ".x <= " + secondArg + ".x && ");
					currentLine.append(firstArg + ".y <= " + secondArg + ".y && ");
					currentLine.append(firstArg + ".z <= " + secondArg + ".z)");
					break;

				case 4:
					currentLine.append("(" + firstArg + ".x <= " + secondArg + ".x && ");
					currentLine.append(firstArg + ".y <= " + secondArg + ".y && ");
					currentLine.append(firstArg + ".z <= " + secondArg + ".z && ");
					currentLine.append(firstArg + ".w <= " + secondArg + ".w)");
					break;
				}
			}
			else if (keyword == "lessThan")
			{
				switch (i)
				{
				case 2:
					currentLine.append("(" + firstArg + ".x < " + secondArg + ".x &&");
					currentLine.append(firstArg + ".y < " + secondArg + ".y)");
					break;

				case 3:
					currentLine.append("(" + firstArg + ".x < " + secondArg + ".x && ");
					currentLine.append(firstArg + ".y < " + secondArg + ".y && ");
					currentLine.append(firstArg + ".z < " + secondArg + ".z)");
					break;

				case 4:
					currentLine.append("(" + firstArg + ".x < " + secondArg + ".x && ");
					currentLine.append(firstArg + ".y < " + secondArg + ".y && ");
					currentLine.append(firstArg + ".z < " + secondArg + ".z && ");
					currentLine.append(firstArg + ".w < " + secondArg + ".w)");
					break;
				}
			}
			else if (keyword == "notEqual")
			{
				switch (i)
				{
				case 2:
					currentLine.append("(" + firstArg + ".x != " + secondArg + ".x &&");
					currentLine.append(firstArg + ".y != " + secondArg + ".y)");
					break;

				case 3:
					currentLine.append("(" + firstArg + ".x != " + secondArg + ".x && ");
					currentLine.append(firstArg + ".y != " + secondArg + ".y && ");
					currentLine.append(firstArg + ".z != " + secondArg + ".z)");
					break;

				case 4:
					currentLine.append("(" + firstArg + ".x != " + secondArg + ".x && ");
					currentLine.append(firstArg + ".y != " + secondArg + ".y && ");
					currentLine.append(firstArg + ".z != " + secondArg + ".z && ");
					currentLine.append(firstArg + ".w != " + secondArg + ".w)");
					break;
				}
			}
			else
			{
				return SAVVY_SHADER_SYNTAX_ERROR;
			}

			break;
		}
	}

	if (!cond)
		return SAVVY_SHADER_SYNTAX_ERROR;

	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Mod(Database::WordMultiMap::iterator& it1, std::string& currentLine)
{
	StringList argList;
	ResultCode argRes = GetFuncArgumentList(it1, argList, currentLine);

	if (argRes != SAVVY_OK)
		return argRes;

	uint32 numArgs = argList.size();

	if (numArgs != 2)
		return SAVVY_SHADER_SYNTAX_ERROR;

	std::string firstArg = argList[0];
	std::string secondArg = argList[1];

	currentLine.append("(" + firstArg + " - " + secondArg + " * floor(" + firstArg + " / " + secondArg + "))");

	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::FuncConverterGLSLToHLSL::Convert_MatrixCompMult(Database::WordMultiMap::iterator& it1, std::string& currentLine)
{
	StringList argList;
	ResultCode argRes = GetFuncArgumentList(it1, argList, currentLine);

	if (argRes != SAVVY_OK)
		return argRes;

	uint32 numArgs = argList.size();

	if (numArgs != 2)
		return SAVVY_SHADER_SYNTAX_ERROR;

	std::string firstArg = argList[0];
	std::string secondArg = argList[1];

	currentLine.append("(" + firstArg + " * " + secondArg + ")");
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::FuncConverterGLSLToHLSL::Convert_Not(Database::WordMultiMap::iterator& it1, std::string& currentLine)
{
	StringList argList;
	ResultCode argRes = GetFuncArgumentList(it1, argList, currentLine);

	if (argRes != SAVVY_OK)
		return argRes;

	uint32 numArgs = argList.size();

	if (numArgs != 2)
		return SAVVY_SHADER_SYNTAX_ERROR;

	std::string firstArg = argList[0];
	std::string secondArg = argList[1];

	std::string oldLine = currentLine;
	currentLine.clear();
	currentLine.append(firstArg + " = !" + firstArg + ";\n\t");
	currentLine.append(oldLine + firstArg);

	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::FuncConverterGLSLToHLSL::ConvertTexturing(Database::WordMultiMap::iterator& it1, std::string& currentLine)
{
	Key keyword = it1->second.m_Name;

	StringList argList;
	ResultCode argRes = GetFuncArgumentList(it1, argList, currentLine);

	if (argRes != SAVVY_OK)
		return argRes;

	uint32 numArgs = argList.size();

	if (numArgs < 2 || numArgs > 4)
		return SAVVY_SHADER_SYNTAX_ERROR;

	std::string firstArg = argList[0];
	std::string secondArg = argList[1];
	std::string thirdArg;
	std::string fourthArg;
	if (numArgs >= 3)
	{
		thirdArg = argList[2];

		uint32 pos = thirdArg.find(".");

		if (pos != std::string::npos)
		{
			thirdArg = thirdArg.substr(0, pos);
		}
	}
	
	if (numArgs >= 4)
	{
		fourthArg = argList[3];
	}

	std::string wholeLine;
	// Check the types
	bool cond = false;
	for (uint32 i = 1; i < 4; i++)
	{
		std::string numString = std::to_string(i);

		Database::KeyList list1;
		list1.push_back("sampler" + numString + "D");
		cond = firstArg.find("sampler" + numString + "D") != std::string::npos ||
			m_Database->FindVariableWithType(Key(firstArg), m_CurrFunction, list1);

		if (cond)
		{
			std::string oldNameFirstArg = firstArg;

			if (!m_Database->SamplerExists(oldNameFirstArg))
			{
				if (m_Database->GetSamplerList().empty())
					return SAVVY_LOGICAL_ERROR;

				firstArg = m_Database->GetSamplerWithIndex(0).GetString() + "Sampler";
			}
			else
			{
				firstArg.append("Sampler");
			}

			wholeLine = firstArg + ", " + secondArg;

			if (numArgs >= 3)
			{
				wholeLine.append(", " + thirdArg);
			}

			if (numArgs >= 4)
			{
				wholeLine.append(", " + fourthArg);
			}

			if (keyword == "textureOffset")
			{
				currentLine.append(oldNameFirstArg + ".SampleBias(" + wholeLine + ")");
			}
			else if (keyword == "textureLod")
			{
				currentLine.append("tex" + numString + "DLod(" + wholeLine + ")");
			}
			else if (keyword == "textureGrad" || keyword == "textureGradOffset")
			{
				currentLine.append(oldNameFirstArg + ".SampleGrad(" + wholeLine + ")");
			}
			else if (keyword == "textureProj")
			{
				currentLine.append("tex" + numString + "DProj(" + wholeLine + ")");
			}
			else if (keyword == "texelFetch")
			{
				currentLine.append(oldNameFirstArg + ".Load(" + secondArg + ")");
			}
			else if (keyword == "texture")
			{
				if (m_CurrShader == FRAGMENT_SHADER)
				{
					if (numArgs == 3)
					{
						switch (i)
						{
						case 1:
							currentLine.append(oldNameFirstArg + ".Sample(" + firstArg + "," + secondArg + "," + thirdArg + ")");
							break;

						case 2:
							currentLine.append(oldNameFirstArg + ".Sample(" + firstArg + "," + secondArg + "," + "int2(" + thirdArg + "," + thirdArg + ")" + ")");
							break;

						case 3:
							currentLine.append(oldNameFirstArg + ".Sample(" + firstArg + "," + secondArg + "," + "int3(" + thirdArg + "," + thirdArg + "," + thirdArg + ")" + ")");
							break;
						}
					}
					else
					{
						currentLine.append(oldNameFirstArg + ".Sample(" + wholeLine + ")");
					}
				}
				else
				{
					if (numArgs == 3)
					{
						switch (i)
						{
						case 1:
							currentLine.append(oldNameFirstArg + ".SampleLevel(" + firstArg + "," + secondArg + ",0," + thirdArg + ")");
							break;

						case 2:
							currentLine.append(oldNameFirstArg + ".SampleLevel(" + firstArg + "," + secondArg + ",0," + "int2(" + thirdArg + "," + thirdArg + ")" + ")");
							break;

						case 3:
							currentLine.append(oldNameFirstArg + ".SampleLevel(" + firstArg + "," + secondArg + ",0," + "int3(" + thirdArg + "," + thirdArg + "," + thirdArg + ")" + ")");
							break;
						}
					}
					else
					{
						currentLine.append(oldNameFirstArg + ".SampleLevel(" + wholeLine + ",0)");
					}
				}
			}
			break;
		}
	}

	if (!cond)
		return SAVVY_SHADER_SYNTAX_ERROR;

	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::FuncConverterGLSLToHLSL::ConvertFunction(Database::WordMultiMap::iterator& it1, std::string& currentLine, Key& a_FuncName, uint32 a_Shader, Key& a_InputStructName /*= Key()*/, Key& a_OutputStructName /*= Key()*/)
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

Savvy::ResultCode Savvy::Internal::FuncConverterGLSLToHLSL::ConvertVectors(Database::WordMultiMap::iterator& it1, std::string& currentLine)
{
	Key keyword = it1->second.m_Name;
	Database::WordMultiMap::iterator origIt = it1;
	StringList argList;
	ResultCode argRes = GetFuncArgumentList(it1, argList, currentLine);

	if (argRes != SAVVY_OK)
		return argRes;

	uint32 numArgs = argList.size();

	if (numArgs < 1 || numArgs > 4)
		return SAVVY_SHADER_SYNTAX_ERROR;

	std::string wholeLine;

	for (uint32 i = 0; i < numArgs; i++)
	{
		wholeLine.append(argList[i]);

		if (i < numArgs - 1)
			wholeLine.append(",");
	}

	StringList argVars;
	argRes = GetArgumentVars(origIt, argVars);

	if (argRes != SAVVY_OK)
		return argRes;

	// Remove any unnecessary info, in order to query properly

	for (uint32 i = 0; i < argVars.size(); i++)
	{
		std::string& queryString = argVars[i];
		uint32 structPos = queryString.find("IN.");
		if (structPos != std::string::npos)
		{
			std::string prev = queryString.substr(0, structPos);
			queryString = queryString.substr(structPos + 3);
			queryString = prev + queryString;
		}
		else
		{
			structPos = queryString.find("OUT.");
			if (structPos != std::string::npos)
			{
				std::string prev = queryString.substr(0, structPos);
				queryString = queryString.substr(structPos + 4);
				queryString = prev + queryString;
			}
		}
	}

	for (uint32 i = 2; i < 5; i++)
	{
		std::string ctrString = std::to_string(i);
		if (keyword == "vec" + ctrString)
		{
			bool hasVecDecl = false;
			for (uint32 j = 2; j < 5; j++)
			{
				uint32 vecPos = argList[0].find("float" + std::to_string(j));
				uint32 mulPos = argList[0].find("mul");
				if (vecPos != std::string::npos || mulPos != std::string::npos)
				{
					hasVecDecl = true;
					break;
				}
			}

			// Check if it's a function and what it returns
			bool isFuncTranslate = false;
			if (m_Database->FunctionExists(argVars[0]))
			{
				Key& retType = m_Database->GetFunction(argVars[0]).m_ReturnType;

				if (!retType.Contains("vec"))
				{
					isFuncTranslate = true;
				}
			}

			// Check for the type as well
			bool isVar = false;
			for (uint32 j = 0; j < argVars.size(); j++)
			{
				bool res = m_Database->FindVariableWithType(argVars[j], m_CurrFunction, "vec" + ctrString);
				if (res)
				{
					isVar = true;
					break;
				}
			}

			if (isFuncTranslate || (numArgs == 1 && !hasVecDecl && !isVar))
			{
				switch (i)
				{
				case 2:
					if (isFuncTranslate)
					{
						currentLine.append("float" + ctrString + "(" + wholeLine + "), " + wholeLine + ")");
					}
					else
					{
						currentLine.append("float" + ctrString + "(" + argList[0] + "," + argList[0] + ")");
					}
					break;

				case 3:
					if (isFuncTranslate)
					{
						currentLine.append("float" + ctrString + "(" + wholeLine + "), " + wholeLine + "), " + wholeLine + ")");
					}
					else
					{
						currentLine.append("float" + ctrString + "(" + argList[0] + "," + argList[0] + "," + argList[0] + ")");
					}
					break;

				case 4:
					if (isFuncTranslate)
					{
						currentLine.append("float" + ctrString + "(" + wholeLine + "), " + wholeLine + "), " + wholeLine + "), " + wholeLine + ")");
					}
					else
					{
						currentLine.append("float" + ctrString + "(" + argList[0] + "," + argList[0] + "," + argList[0] + "," + argList[0] + ")");
					}
					break;

				default:
					break;
				}
			}
			else
			{
				currentLine.append("float" + ctrString + "(" + wholeLine + ")");
			}
		}
		else if (keyword == "ivec" + ctrString)
		{
			bool hasVecDecl = false;

			for (uint32 j = 2; j < 5; j++)
			{
				uint32 vecPos = argList[0].find("int" + std::to_string(j));

				if (vecPos != std::string::npos)
				{
					hasVecDecl = true;
					break;
				}
			}

			// Check if it's a function and what it returns
			bool isFuncTranslate = false;
			if (m_Database->FunctionExists(argVars[0]))
			{
				Key& retType = m_Database->GetFunction(argVars[0]).m_ReturnType;

				if (!retType.Contains("vec"))
				{
					isFuncTranslate = true;
				}
			}

			// Check for the type as well
			bool isVar = false;
			for (uint32 j = 0; j < argVars.size(); j++)
			{
				bool res = m_Database->FindVariableWithType(argVars[j], m_CurrFunction, "ivec" + ctrString);
				if (res)
				{
					isVar = true;
					break;
				}
			}

			if (isFuncTranslate || (numArgs == 1 && !hasVecDecl && !isVar))
			{
				switch (i)
				{
				case 2:
					if (isFuncTranslate)
					{
						currentLine.append("int" + ctrString + "(" + wholeLine + "), " + wholeLine + ")");
					}
					else
					{
						currentLine.append("int" + ctrString + "(" + argList[0] + "," + argList[0] + ")");
					}
					break;

				case 3:
					if (isFuncTranslate)
					{
						currentLine.append("int" + ctrString + "(" + wholeLine + "), " + wholeLine + "), " + wholeLine + ")");
					}
					else
					{
						currentLine.append("int" + ctrString + "(" + argList[0] + "," + argList[0] + "," + argList[0] + ")");
					}
					break;

				case 4:
					if (isFuncTranslate)
					{
						currentLine.append("int" + ctrString + "(" + wholeLine + "), " + wholeLine + "), " + wholeLine + "), " + wholeLine + ")");
					}
					else
					{
						currentLine.append("int" + ctrString + "(" + argList[0] + "," + argList[0] + "," + argList[0] + "," + argList[0] + ")");
					}
					break;

				default:
					break;
				}
			}
			else
			{
				currentLine.append("int" + ctrString + "(" + wholeLine + ")");
			}
		}
		else if (keyword == "uvec" + ctrString)
		{
			bool hasVecDecl = false;

			for (uint32 j = 2; j < 5; j++)
			{
				uint32 vecPos = argList[0].find("uint" + std::to_string(j));

				if (vecPos != std::string::npos)
				{
					hasVecDecl = true;
					break;
				}
			}

			// Check if it's a function and what it returns
			bool isFuncTranslate = false;
			if (m_Database->FunctionExists(argVars[0]))
			{
				Key& retType = m_Database->GetFunction(argVars[0]).m_ReturnType;

				if (!retType.Contains("vec"))
				{
					isFuncTranslate = true;
				}
			}

			// Check for the type as well
			bool isVar = false;
			for (uint32 j = 0; j < argVars.size(); j++)
			{
				bool res = m_Database->FindVariableWithType(argVars[j], m_CurrFunction, "uvec" + ctrString);
				if (res)
				{
					isVar = true;
					break;
				}
			}

			if (isFuncTranslate || (numArgs == 1 && !hasVecDecl && !isVar))
			{
				switch (i)
				{
				case 2:
					if (isFuncTranslate)
					{
						currentLine.append("uint" + ctrString + "(" + wholeLine + "), " + wholeLine + ")");
					}
					else
					{
						currentLine.append("uint" + ctrString + "(" + argList[0] + "," + argList[0] + ")");
					}
					break;

				case 3:
					if (isFuncTranslate)
					{
						currentLine.append("uint" + ctrString + "(" + wholeLine + "), " + wholeLine + "), " + wholeLine + ")");
					}
					else
					{
						currentLine.append("uint" + ctrString + "(" + argList[0] + "," + argList[0] + "," + argList[0] + ")");
					}
					break;

				case 4:
					if (isFuncTranslate)
					{
						currentLine.append("uint" + ctrString + "(" + wholeLine + "), " + wholeLine + "), " + wholeLine + "), " + wholeLine + ")");
					}
					else
					{
						currentLine.append("uint" + ctrString + "(" + argList[0] + "," + argList[0] + "," + argList[0] + "," + argList[0] + ")");
					}
					break;

				default:
					break;
				}
			}
			else
			{
				currentLine.append("uint" + ctrString + "(" + wholeLine + ")");
			}
		}
		else if (keyword == "bvec" + ctrString)
		{
			bool hasVecDecl = false;

			for (uint32 j = 2; j < 5; j++)
			{
				uint32 vecPos = argList[0].find("bool" + std::to_string(j));

				if (vecPos != std::string::npos)
				{
					hasVecDecl = true;
					break;
				}
			}

			// Check if it's a function and what it returns
			bool isFuncTranslate = false;
			if (m_Database->FunctionExists(argVars[0]))
			{
				Key& retType = m_Database->GetFunction(argVars[0]).m_ReturnType;

				if (!retType.Contains("vec"))
				{
					isFuncTranslate = true;
				}
			}

			// Check for the type as well
			bool isVar = false;
			for (uint32 j = 0; j < argVars.size(); j++)
			{
				bool res = m_Database->FindVariableWithType(argVars[j], m_CurrFunction, "bvec" + ctrString);
				if (res)
				{
					isVar = true;
					break;
				}
			}

			if (isFuncTranslate || (numArgs == 1 && !hasVecDecl && !isVar))
			{
				switch (i)
				{
				case 2:
					if (isFuncTranslate)
					{
						currentLine.append("bool" + ctrString + "(" + wholeLine + "), " + wholeLine + ")");
					}
					else
					{
						currentLine.append("bool" + ctrString + "(" + argList[0] + "," + argList[0] + ")");
					}
					break;

				case 3:
					if (isFuncTranslate)
					{
						currentLine.append("bool" + ctrString + "(" + wholeLine + "), " + wholeLine + "), " + wholeLine + ")");
					}
					else
					{
						currentLine.append("bool" + ctrString + "(" + argList[0] + "," + argList[0] + "," + argList[0] + ")");
					}
					break;

				case 4:
					if (isFuncTranslate)
					{
						currentLine.append("bool" + ctrString + "(" + wholeLine + "), " + wholeLine + "), " + wholeLine + "), " + wholeLine + ")");
					}
					else
					{
						currentLine.append("bool" + ctrString + "(" + argList[0] + "," + argList[0] + "," + argList[0] + "," + argList[0] + ")");
					}
					break;

				default:
					break;
				}
			}
			else
			{
				currentLine.append("bool" + ctrString + "(" + wholeLine + ")");
			}
		}
		else if (keyword == "dvec" + ctrString)
		{
			bool hasVecDecl = false;

			for (uint32 j = 2; j < 5; j++)
			{
				uint32 vecPos = argList[0].find("double" + std::to_string(j));

				if (vecPos != std::string::npos)
				{
					hasVecDecl = true;
					break;
				}
			}

			// Check if it's a function and what it returns
			bool isFuncTranslate = false;
			if (m_Database->FunctionExists(argVars[0]))
			{
				Key& retType = m_Database->GetFunction(argVars[0]).m_ReturnType;

				if (!retType.Contains("vec"))
				{
					isFuncTranslate = true;
				}
			}

			// Check for the type as well
			bool isVar = false;
			for (uint32 j = 0; j < argVars.size(); j++)
			{
				bool res = m_Database->FindVariableWithType(argVars[j], m_CurrFunction, "dvec" + ctrString);
				if (res)
				{
					isVar = true;
					break;
				}
			}

			if (isFuncTranslate || (numArgs == 1 && !hasVecDecl && !isVar))
			{
				switch (i)
				{
				case 2:
					if (isFuncTranslate)
					{
						currentLine.append("double" + ctrString + "(" + wholeLine + "), " + wholeLine + ")");
					}
					else
					{
						currentLine.append("double" + ctrString + "(" + argList[0] + "," + argList[0] + ")");
					}
					break;

				case 3:
					if (isFuncTranslate)
					{
						currentLine.append("double" + ctrString + "(" + wholeLine + "), " + wholeLine + "), " + wholeLine + ")");
					}
					else
					{
						currentLine.append("double" + ctrString + "(" + argList[0] + "," + argList[0] + "," + argList[0] + ")");
					}
					break;

				case 4:
					if (isFuncTranslate)
					{
						currentLine.append("double" + ctrString + "(" + wholeLine + "), " + wholeLine + "), " + wholeLine + "), " + wholeLine + ")");
					}
					else
					{
						currentLine.append("double" + ctrString + "(" + argList[0] + "," + argList[0] + "," + argList[0] + "," + argList[0] + ")");
					}
					break;

				default:
					break;
				}
			}
			else
			{
				currentLine.append("double" + ctrString + "(" + wholeLine + ")");
			}
		}
	}

	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::FuncConverterGLSLToHLSL::GetFuncArgumentList(Database::WordMultiMap::iterator& it1, StringList& a_OutputList, std::string& currentLine)
{
	Database::WordMultiMap& instructionMap = m_Database->GetInstructionMap();
	Database::WordMultiMap::iterator endIt = instructionMap.upper_bound(m_CurrFunction);

	Key keyName = it1->second.m_Name;

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
	bool transformConstructorToCast = false;
	while (it1 != endIt)
	{
		transformConstructorToCast = false;
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
					// Check for casting down matrices
					transformConstructorToCast = IsMatrixDowncast(retList.back(), m_CurrFunction, it1);

					if (m_Database->OutputLangEquivalentExists(currWord.m_Name))
					{
						Key k;
						// Handle vectors
						if (it1->second.m_Name.Contains("vec"))
						{
							it1++;
							if (it1->second.m_Name == "(")
							{
								it1--;
								std::string tempLine = currentLine + keyName.GetString() + "(" + retList.back();
								ResultCode convRes = ConvertFunction(it1, tempLine, m_CurrFunction, m_CurrShader, m_InputStructName, m_OutputStructName);

								if (convRes != SAVVY_OK)
									return convRes;

								uint32 keyLength = std::string(keyName.GetString() + "(").size();
								uint32 realLinePos = tempLine.find(keyName.GetString() + "(");

								if (realLinePos == std::string::npos)
									return SAVVY_LOGICAL_ERROR;

								currentLine = tempLine.substr(0, realLinePos);
								retList.back() = tempLine.substr(realLinePos + keyLength);

							}
							else
							{
								k = m_Database->GetOutputLangEquivalent(currWord.m_Name);
								add = k.GetString();
							}
						}
						else
						{
							k = m_Database->GetOutputLangEquivalent(currWord.m_Name);
							add = k.GetString();
						}
					}
					else
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
							std::string tempLine = currentLine + keyName.GetString() + "(" + retList.back();
							ResultCode convRes = ConvertFunction(it1, tempLine, m_CurrFunction, m_CurrShader, m_InputStructName, m_OutputStructName);

							if (convRes != SAVVY_OK)
								return convRes;

							uint32 keyLength = std::string(keyName.GetString() + "(").size();
							uint32 realLinePos = tempLine.find(keyName.GetString() + "(");

							if (realLinePos == std::string::npos)
								return SAVVY_LOGICAL_ERROR;

							currentLine = tempLine.substr(0, realLinePos);
							retList.back() = tempLine.substr(realLinePos + keyLength);
						}
					}
				}
				else
				{
					// Determine whether the current instruction is an input or output variable
					if (m_Database->InputVariableExists(currWord.m_Name))
					{
						currWord.m_Name.SetString("IN." + currWord.m_Name.GetString());
					}
					else if (m_Database->OutputVariableExists(currWord.m_Name))
					{
						if (m_CurrShader == VERTEX_SHADER)
						{
							currWord.m_Name.SetString("OUT." + currWord.m_Name.GetString());
						}
					}

					// Matrix multiplication case
					bool matMultHandled = false;
					if (currWord.m_Name == "*")
					{
						if (it1 == instructionMap.lower_bound(m_CurrFunction))
							return SAVVY_SHADER_SYNTAX_ERROR;

						ResultCode resMatMult = HandleMatrixMultiplication(retList.back(), it1, matMultHandled);

						if (resMatMult != SAVVY_OK)
							return resMatMult;

						if (matMultHandled)
						{
							if (it1->second.m_Name == ",")
								retList.back().append(it1->second.m_Name.GetString());

							continue;
						}
					}

					if (!matMultHandled)
						add = currWord.m_Name.GetString();
				}
				if (transformConstructorToCast)
				{
					retList.back().append("(" + add + ")");
					it1++;
					it1->second.m_Name.Clear();
					it1++; it1++;
					it1->second.m_Name.Clear();
					it1--; it1--; it1--;
				}
				else
				{
					retList.back().append(add);
				}
			}
		}
		else
		{
			std::string add;
			if (currWord.m_Translate)
			{
				// Check for casting down matrices
				transformConstructorToCast = IsMatrixDowncast(retList.back(), m_CurrFunction, it1);

				if (m_Database->OutputLangEquivalentExists(currWord.m_Name))
				{
					Key k;
					// Handle vectors
					if (it1->second.m_Name.Contains("vec"))
					{
						it1++;
						if (it1->second.m_Name == "(")
						{
							it1--;
							std::string tempLine = currentLine + keyName.GetString() + "(" + retList.back();
							ResultCode convRes = ConvertFunction(it1, tempLine, m_CurrFunction, m_CurrShader, m_InputStructName, m_OutputStructName);

							if (convRes != SAVVY_OK)
								return convRes;

							uint32 keyLength = std::string(keyName.GetString() + "(").size();
							uint32 realLinePos = tempLine.find(keyName.GetString() + "(");

							if (realLinePos == std::string::npos)
								return SAVVY_LOGICAL_ERROR;

							currentLine = tempLine.substr(0, realLinePos);
							retList.back() = tempLine.substr(realLinePos + keyLength);
						}
						else
						{
							k = m_Database->GetOutputLangEquivalent(currWord.m_Name);
							add = k.GetString();
						}
					}
					else
					{
						k = m_Database->GetOutputLangEquivalent(currWord.m_Name);
						add = k.GetString();
					}
				}
				else
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
						std::string tempLine = currentLine + keyName.GetString() + "(" + retList.back();
						ResultCode convRes = ConvertFunction(it1, tempLine, m_CurrFunction, m_CurrShader, m_InputStructName, m_OutputStructName);

						if (convRes != SAVVY_OK)
							return convRes;

						uint32 keyLength = std::string(keyName.GetString() + "(").size();
						uint32 realLinePos = tempLine.find(keyName.GetString() + "(");

						if (realLinePos == std::string::npos)
							return SAVVY_LOGICAL_ERROR;

						currentLine = tempLine.substr(0, realLinePos);
						retList.back() = tempLine.substr(realLinePos + keyLength);
					}
				}
			}
			else
			{
				// Determine whether the current instruction is an input or output variable
				if (m_Database->InputVariableExists(currWord.m_Name))
				{
					currWord.m_Name.SetString("IN." + currWord.m_Name.GetString());
				}
				else if (m_Database->OutputVariableExists(currWord.m_Name))
				{
					if (m_CurrShader == VERTEX_SHADER)
					{
						currWord.m_Name.SetString("OUT." + currWord.m_Name.GetString());
					}
				}

				// Matrix multiplication case
				bool matMultHandled = false;
				if (currWord.m_Name == "*")
				{
					if (it1 == instructionMap.lower_bound(m_CurrFunction))
						return SAVVY_SHADER_SYNTAX_ERROR;

					ResultCode resMatMult = HandleMatrixMultiplication(retList.back(), it1, matMultHandled);

					if (resMatMult != SAVVY_OK)
						return resMatMult;

					if (matMultHandled)
					{
						if (it1->second.m_Name == ",")
							retList.back().append(it1->second.m_Name.GetString());

						continue;
					}
				}

				if (!matMultHandled)
					add = currWord.m_Name.GetString();
			}
			if (transformConstructorToCast)
			{
				retList.back().append("(" + add + ")");
				it1++;
				it1->second.m_Name.Clear();
				it1++; it1++;
				it1->second.m_Name.Clear();
				it1--; it1--; it1--;
			}
			else
			{
				retList.back().append(add);
			}
		}
	}

	if (it1 == endIt)
		return SAVVY_SHADER_SYNTAX_ERROR;

	a_OutputList.swap(retList);

	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::FuncConverterGLSLToHLSL::HandleMatrixMultiplication(std::string& currentLine, Database::WordMultiMap::iterator& it1, bool& a_Handled)
{
	Database::WordMultiMap& instructionMap = m_Database->GetInstructionMap();
	Database::WordMultiMap::iterator beginIt = instructionMap.lower_bound(m_CurrFunction);
	Database::WordMultiMap::iterator endIt = instructionMap.upper_bound(m_CurrFunction);
	Database::WordMultiMap::iterator originalIterator = it1;
	Database::WordMultiMap::iterator funcEndIterator = it1;
	Database::WordMultiMap::iterator funcBeginlIterator = it1;
	a_Handled = false;
	// First we need to extract the first operand
	uint32 ctrOffset = 0;
	uint32 numCloseBrackets = 0;
	uint32 numOpenBrackets = 0;
	it1--;
	while (it1 != beginIt)
	{
		Database::Word word = it1->second;

		if (word.m_Name == ")")
		{
			numCloseBrackets++;
		}
		else if (word.m_Name == "(")
		{
			if (numCloseBrackets == numOpenBrackets)
			{
				funcBeginlIterator = it1;
				break;
			}

			numOpenBrackets++;
		}

		if (numCloseBrackets == numOpenBrackets)
		{
			if (word.m_Name == "*" || word.m_Name == "/" ||
				word.m_Name == "+" || word.m_Name == "-" ||
				word.m_Name == "," || word.m_Name == "=" ||
				word.m_Name == "==" || word.m_Name == "!=" ||
				word.m_Name == ">" || word.m_Name == "<" ||
				word.m_Name == ">=" || word.m_Name == "<=" ||
				word.m_Name == "(")
			{
				funcBeginlIterator = it1;
				break;
			}
		}

		it1--;
		ctrOffset++;
	}

	if (it1 == beginIt)
	{
		return SAVVY_SHADER_SYNTAX_ERROR;
	}

	std::string firstArg;
	for (uint32 i = 0; i < ctrOffset; i++)
	{
		it1++;
		Database::Word word = it1->second;

		if (word.m_Translate)
		{
			bool matrixCast = IsMatrixDowncast(currentLine, m_CurrFunction, it1);
			if (!m_Database->OutputLangEquivalentExists(word.m_Name))
			{
				return SAVVY_SHADER_SYNTAX_ERROR;
			}

			const Key& equiv = m_Database->GetOutputLangEquivalent(word.m_Name);

			if (matrixCast)
			{
				firstArg.append("(" + equiv.GetString() + ")");
				it1++;
				it1->second.m_Name.Clear();
				it1++; it1++;
				it1->second.m_Name.Clear();
				it1--; it1--; it1--;
			}
			else
			{
				firstArg.append(equiv.GetString());
			}
		}
		else
		{
			// Determine whether the current instruction is an input or output variable
			if (m_Database->InputVariableExists(word.m_Name))
			{
				word.m_Name.SetString("IN." + word.m_Name.GetString());
			}
			else if (m_Database->OutputVariableExists(word.m_Name))
			{
				if (m_CurrShader == VERTEX_SHADER)
				{
					word.m_Name.SetString("OUT." + word.m_Name.GetString());
				}
			}

			firstArg.append(word.m_Name.GetString());
		}
	}

	// Return iterator to original place
	it1 = originalIterator;

	// Check if the first argument is a vector or matrix

	// Remove any unnecessary info, in order to query properly
	std::string queryArg1 = firstArg;
	uint32 structPos = queryArg1.find("IN.");
	if (structPos != std::string::npos)
	{
		std::string prev = queryArg1.substr(0, structPos);
		queryArg1 = queryArg1.substr(structPos + 3);
		queryArg1 = prev + queryArg1;
	}
	else
	{
		structPos = queryArg1.find("OUT.");
		if (structPos != std::string::npos)
		{
			std::string prev = queryArg1.substr(0, structPos);
			queryArg1 = queryArg1.substr(structPos + 4);
			queryArg1 = prev + queryArg1;
		}
	}

	bool isArg1Vec = IsVarVector(queryArg1);
	bool isArg1Mat = IsVarMatrix(queryArg1);

	// Early out
	if (!isArg1Vec && !isArg1Mat)
		return SAVVY_OK;


	// It could already be a mul
	uint32 mulPos = currentLine.find("mul");
	if (mulPos != std::string::npos)
	{
		firstArg = currentLine.substr(mulPos); // Overwrite
	}

	// Extract second argument
	it1++;
	ctrOffset = 0;
	numCloseBrackets = 0;
	numOpenBrackets = 0;

	while (it1 != endIt)
	{
		Database::Word word = it1->second;

		if (word.m_Name == ")")
		{
			if (numCloseBrackets == numOpenBrackets)
			{
				funcEndIterator = it1;
				break;
			}

			numCloseBrackets++;
		}
		else if (word.m_Name == "(")
		{
			numOpenBrackets++;
		}

		if (numCloseBrackets == numOpenBrackets)
		{
			if (word.m_Name == "*" || word.m_Name == "/" ||
				word.m_Name == "+" || word.m_Name == "-" ||
				word.m_Name == "," || word.m_Name == "=" ||
				word.m_Name == "==" || word.m_Name == "!=" ||
				word.m_Name == ">" || word.m_Name == "<" ||
				word.m_Name == ">=" || word.m_Name == "<=" ||
				word.m_Name == ";")
			{
				funcEndIterator = it1;
				break;
			}
		}

		it1++;
		ctrOffset++;
	}

	if (it1 == endIt)
	{
		return SAVVY_SHADER_SYNTAX_ERROR;
	}

	// Assemble the second argument
	std::string secondArg;
	it1 = originalIterator;
	for (uint32 i = 0; i < ctrOffset; i++)
	{
		it1++;
		Database::Word word = it1->second;

		if (word.m_Translate)
		{
			bool matrixCast = IsMatrixDowncast(currentLine, m_CurrFunction, it1);

			if (!m_Database->OutputLangEquivalentExists(word.m_Name))
				return SAVVY_SHADER_SYNTAX_ERROR;

			const Key& equiv = m_Database->GetOutputLangEquivalent(word.m_Name);
			if (matrixCast)
			{
				secondArg.append("(" + equiv.GetString() + ")");
				it1++;
				it1->second.m_Name.Clear();
				it1++; it1++;
				it1->second.m_Name.Clear();
				it1--; it1--; it1--;
			}
			else
			{
				secondArg.append(equiv.GetString());
			}
		}
		else
		{
			// Determine whether the current instruction is an input or output variable
			if (m_Database->InputVariableExists(word.m_Name))
			{
				word.m_Name.SetString("IN." + word.m_Name.GetString());
			}
			else if (m_Database->OutputVariableExists(word.m_Name))
			{
				if (m_CurrShader == VERTEX_SHADER)
				{
					word.m_Name.SetString("OUT." + word.m_Name.GetString());
				}
			}

			secondArg.append(word.m_Name.GetString());
		}
	}

	// Remove any unnecessary info, in order to query properly
	std::string queryArg2 = secondArg;
	structPos = queryArg2.find("IN.");
	if (structPos != std::string::npos)
	{
		std::string prev = queryArg2.substr(0, structPos);
		queryArg2 = queryArg2.substr(structPos + 3);
		queryArg2 = prev + queryArg2;
	}
	else
	{
		structPos = queryArg2.find("OUT.");
		if (structPos != std::string::npos)
		{
			std::string prev = queryArg2.substr(0, structPos);
			queryArg2 = queryArg2.substr(structPos + 4);
			queryArg2 = prev + queryArg2;
		}
	}

	// Check if the second argument is a vector or matrix
	bool isArg2Vec = IsVarVector(queryArg2);
	bool isArg2Mat = IsVarMatrix(queryArg2);
	// Early out

	it1 = originalIterator;


	if (!isArg1Mat)
	{
		if ((!isArg2Vec && !isArg2Mat) || (isArg2Vec && isArg1Vec))
			return SAVVY_OK;
	}


	// Both arguments are legit, turn this into a mul() function
	//it1->second.m_Name.Clear();
	it1 = funcEndIterator;
	// Erase the unneeded first argument
	uint32 erasePos = currentLine.find(firstArg);

	if (erasePos == std::string::npos)
	{
		// Normally this should never happen
		return SAVVY_UNKNOWN_ERROR;
	}

	currentLine.erase(erasePos);

	currentLine.append("mul(" + firstArg + ", " + secondArg + ")");
	a_Handled = true;
	return SAVVY_OK;
}

bool Savvy::Internal::FuncConverterGLSLToHLSL::IsVarVector(std::string a_VarName)
{
	Database::KeyList typeList;
	typeList.push_back("vec");
	bool cond1 = m_Database->FindVariableWithType(Key(a_VarName), m_CurrFunction, typeList);

	if (cond1)
		return cond1;

	for (uint32 i = 2; i < 5; i++)
	{
		uint32 pos = a_VarName.find("float" + std::to_string(i));

		if (pos != std::string::npos)
		{
			pos = a_VarName.find("float" + std::to_string(i) + "x");
			if (pos == std::string::npos)
			{
				return true;
			}
		}
	}

	return false;
}

bool Savvy::Internal::FuncConverterGLSLToHLSL::IsVarMatrix(std::string a_VarName)
{
	Database::KeyList typeList;
	typeList.push_back("mat");
	bool cond1 = m_Database->FindVariableWithType(Key(a_VarName), m_CurrFunction, typeList);

	if (cond1)
		return cond1;

	if (a_VarName.find("float2x") != std::string::npos ||
		a_VarName.find("float3x") != std::string::npos ||
		a_VarName.find("float4x") != std::string::npos ||
		a_VarName.find("matrix") != std::string::npos)
	{
		return true;
	}

	return false;
}

const bool Savvy::Internal::FuncConverterGLSLToHLSL::IsMatrixDowncast(std::string& a_CurrLine, const Key& a_FuncName, Database::WordMultiMap::iterator& a_Iter)
{
	bool ret = false;
	if (a_Iter->second.m_Name.Contains("mat3"))
	{
		a_Iter++;

		// Check if it's a constructor
		if (a_Iter->second.m_Name.GetString() == "(")
		{
			a_Iter++;

			Database::KeyList testList;
			testList.push_back("mat4");
			testList.push_back("mat4x4");
			testList.push_back("mat4x3");

			Key name = a_Iter->second.m_Name;

			// Check if the actual identifier is a type that can be casted down to a 3x3 matrix
			ret = m_Database->FindVariableWithType(name, a_FuncName, testList);

			a_Iter--;
		}
		a_Iter--;
	}
	else if (a_Iter->second.m_Name.Contains("mat2"))
	{
		a_Iter++;

		// Check if it's a constructor
		if (a_Iter->second.m_Name.GetString() == "(")
		{
			a_Iter++;

			Database::KeyList testList;
			testList.push_back("mat4");
			testList.push_back("mat4x4");
			testList.push_back("mat4x3");
			testList.push_back("mat3x3");
			testList.push_back("mat3");

			Key name = a_Iter->second.m_Name;

			// Check if the actual identifier is a type that can be casted down to a 3x3 matrix
			ret = m_Database->FindVariableWithType(name, a_FuncName, testList);

			a_Iter--;
		}
		a_Iter--;
	}
	return ret;
}

Savvy::ResultCode Savvy::Internal::FuncConverterGLSLToHLSL::Convert_TextureSize(Database::WordMultiMap::iterator& it1, std::string& currentLine)
{
	Key keyword = it1->second.m_Name;

	StringList argList;
	ResultCode argRes = GetFuncArgumentList(it1, argList, currentLine);

	if (argRes != SAVVY_OK)
		return argRes;

	uint32 numArgs = argList.size();

	if (numArgs != 2)
		return SAVVY_SHADER_SYNTAX_ERROR;

	std::string firstArg = argList[0];
	std::string secondArg = argList[1];

	// Make sure all variables have unique names and have the "Savvy" prefix
	std::string ctrStr = std::to_string(m_TextureSizeCtr);
	std::string widthStr = SAVVY_TEX_WIDTH + ctrStr;
	std::string heightStr = SAVVY_TEX_HEIGHT + ctrStr;

	std::string prevLine = "float " + widthStr + ";" + "float " + heightStr + ";";
	prevLine.append(firstArg + ".GetDimensions(" + widthStr + "," + heightStr + ");\n\t");
	currentLine = prevLine + currentLine;

	currentLine.append("float2(" + widthStr + "," + heightStr + ")");

	m_TextureSizeCtr++;

	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::FuncConverterGLSLToHLSL::GetArgumentVars(Database::WordMultiMap::iterator& it1, StringList& a_Variables)
{
	Database::WordMultiMap& instructionMap = m_Database->GetInstructionMap();
	Database::WordMultiMap::iterator endIt = instructionMap.upper_bound(m_CurrFunction);

	Key keyName = it1->second.m_Name;

	if (it1 == endIt)
		return SAVVY_SHADER_SYNTAX_ERROR;

	it1++;

	if (it1->second.m_Name != "(")
	{
		return SAVVY_SHADER_SYNTAX_ERROR;
	}

	StringList retList;
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

			if (!currWord.m_Translate)
			{
				if ( currWord.m_Name != "," && currWord.m_Name != "+" && 
					currWord.m_Name != "-" && currWord.m_Name != "*" && 
					currWord.m_Name != "/" && !currWord.m_Name.IsEmpty())
				retList.push_back(currWord.m_Name.GetString());
			}
		
	}
	a_Variables = retList;


	return SAVVY_OK;
}

void Savvy::Internal::FuncConverterGLSLToHLSL::Reset()
{
	m_AtomicCtr = 0;
	m_BitfieldExtractCtr = 0;
	m_BitfieldInsertCtr = 0;
	m_TextureSizeCtr = 0;
}
