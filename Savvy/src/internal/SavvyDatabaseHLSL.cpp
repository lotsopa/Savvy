#include "SavvyPCH.h"
#include "SavvyDatabaseHLSL.h"

Savvy::Internal::DatabaseHLSL::KeyMap Savvy::Internal::DatabaseHLSL::m_DataTypeMapHLSL = Savvy::Internal::DatabaseHLSL::CreateKeywordMap();

Savvy::Internal::DatabaseHLSL::DatabaseHLSL() : Database()
{
	
}

Savvy::Internal::DatabaseHLSL::~DatabaseHLSL()
{

}

void Savvy::Internal::DatabaseHLSL::Reset()
{
	Database::Reset();
}

Savvy::Database::KeyMap Savvy::Internal::DatabaseHLSL::CreateKeywordMap()
{
	KeyMap DataTypeMapHLSL;
	DataTypeMapHLSL["int"] = Key("int");
	DataTypeMapHLSL["bool"] = Key("bool");
	DataTypeMapHLSL["uint"] = Key("uint");
	DataTypeMapHLSL["float"] = Key("float");
	DataTypeMapHLSL["double"] = Key("double");
	DataTypeMapHLSL["void"] = Key("void");

	DataTypeMapHLSL["float2"] = Key("vec2");
	DataTypeMapHLSL["float3"] = Key("vec3");
	DataTypeMapHLSL["float4"] = Key("vec4");

	DataTypeMapHLSL["bool2"] = Key("bvec2");
	DataTypeMapHLSL["bool3"] = Key("bvec3");
	DataTypeMapHLSL["bool4"] = Key("bvec4");

	DataTypeMapHLSL["uint2"] = Key("uvec2");
	DataTypeMapHLSL["uint3"] = Key("uvec3");
	DataTypeMapHLSL["uint4"] = Key("uvec4");

	DataTypeMapHLSL["int2"] = Key("ivec2");
	DataTypeMapHLSL["int3"] = Key("ivec3");
	DataTypeMapHLSL["int4"] = Key("ivec4");

	DataTypeMapHLSL["double2"] = Key("dvec2");
	DataTypeMapHLSL["double3"] = Key("dvec3");
	DataTypeMapHLSL["double4"] = Key("dvec4");

	DataTypeMapHLSL["matrix"] = Key("mat4");
	DataTypeMapHLSL["float4x4"] = Key("mat4");
	DataTypeMapHLSL["float3x3"] = Key("mat3");
	DataTypeMapHLSL["float2x2"] = Key("mat2");
	DataTypeMapHLSL["float2x3"] = Key("mat2x3");
	DataTypeMapHLSL["float2x4"] = Key("mat2x4");

	DataTypeMapHLSL["float3x2"] = Key("mat3x2");
	DataTypeMapHLSL["float3x4"] = Key("mat3x4");

	DataTypeMapHLSL["float4x2"] = Key("mat4x2");
	DataTypeMapHLSL["float4x3"] = Key("mat4x3");

	DataTypeMapHLSL["double4x4"] = Key("dmat4");
	DataTypeMapHLSL["double3x3"] = Key("dmat3x3");
	DataTypeMapHLSL["double2x2"] = Key("dmat2x2");
	DataTypeMapHLSL["double2x3"] = Key("dmat2x3");
	DataTypeMapHLSL["double2x4"] = Key("dmat2x4");

	DataTypeMapHLSL["double3x2"] = Key("dmat3x2");
	DataTypeMapHLSL["double3x4"] = Key("dmat3x4");

	DataTypeMapHLSL["double4x2"] = Key("dmat4x2");
	DataTypeMapHLSL["double4x3"] = Key("dmat4x3");

	DataTypeMapHLSL["Texture2D"] = Key("sampler2D");
	DataTypeMapHLSL["Texture3D"] = Key("sampler3D");
	DataTypeMapHLSL["Texture1D"] = Key("sampler1D");

	DataTypeMapHLSL["Texture2DArray"] = Key("sampler2DArray");
	DataTypeMapHLSL["Texture1DArray"] = Key("sampler1DArray");

	DataTypeMapHLSL["normalize"] = Key("normalize");
	DataTypeMapHLSL["clamp"] = Key("clamp");
	DataTypeMapHLSL["dot"] = Key("dot");
	DataTypeMapHLSL["const"] = Key("const");
	DataTypeMapHLSL["static"] = Key("");
	DataTypeMapHLSL["discard"] = Key("discard");
	DataTypeMapHLSL["abs"] = Key("abs");
	DataTypeMapHLSL["acos"] = Key("acos");
	DataTypeMapHLSL["memoryBarrier"] = Key("AllMemoryBarrier");
	DataTypeMapHLSL["groupMemoryBarrier"] = Key("AllMemoryBarrierWithGroupSync");
	DataTypeMapHLSL["all"] = Key("all");
	DataTypeMapHLSL["any"] = Key("any");
	DataTypeMapHLSL["asin"] = Key("asin");
	DataTypeMapHLSL["atan"] = Key("atan");
	DataTypeMapHLSL["asuint"] = Key("floatBitsToUint");
	DataTypeMapHLSL["asint"] = Key("floatBitsToInt");
	DataTypeMapHLSL["countbits"] = Key("bitCount");
	DataTypeMapHLSL["reversebits"] = Key("bitfieldReverse");
	DataTypeMapHLSL["ceil"] = Key("ceil");
	DataTypeMapHLSL["cross"] = Key("cross");
	DataTypeMapHLSL["cos"] = Key("cos");
	DataTypeMapHLSL["asfloat"] = Key("intBitsToFloat");
	DataTypeMapHLSL["determinant"] = Key("determinant");
	DataTypeMapHLSL["ddx"] = Key("dFdx");
	DataTypeMapHLSL["ddy"] = Key("dFdy");
	DataTypeMapHLSL["ddx_coarse"] = Key("dFdxCoarse");
	DataTypeMapHLSL["ddy_coarse"] = Key("dFdyCoarse");
	DataTypeMapHLSL["ddx_fine"] = Key("dFdxFine");
	DataTypeMapHLSL["ddy_fine"] = Key("dFdyFine");
	DataTypeMapHLSL["distance"] = Key("distance");
	DataTypeMapHLSL["dst"] = Key("distance");
	DataTypeMapHLSL["EvaluateAttributeAtCentroid"] = Key("interpolateAtCentroid");
	DataTypeMapHLSL["EvaluateAttributeSnapped"] = Key("interpolateAtOffset");
	DataTypeMapHLSL["EvaluateAttributeAtSample"] = Key("interpolateAtSample");
	DataTypeMapHLSL["exp"] = Key("exp");
	DataTypeMapHLSL["exp2"] = Key("exp2");
	DataTypeMapHLSL["faceforward"] = Key("faceforward");
	DataTypeMapHLSL["firstbitlow"] = Key("findLSB");
	DataTypeMapHLSL["firstbithigh"] = Key("findMSB");
	DataTypeMapHLSL["floor"] = Key("floor");
	DataTypeMapHLSL["fma"] = Key("fma");
	DataTypeMapHLSL["frac"] = Key("fract");
	DataTypeMapHLSL["frexp"] = Key("frexp");
	DataTypeMapHLSL["fwidth"] = Key("fwidth");
	DataTypeMapHLSL["modf"] = Key("modf");
	DataTypeMapHLSL["rsqrt"] = Key("inversesqrt");
	DataTypeMapHLSL["isinf"] = Key("isinf");
	DataTypeMapHLSL["isnan"] = Key("isnan");
	DataTypeMapHLSL["ldexp"] = Key("ldexp");
	DataTypeMapHLSL["length"] = Key("length");
	DataTypeMapHLSL["log"] = Key("log");
	DataTypeMapHLSL["log2"] = Key("log2");
	DataTypeMapHLSL["lerp"] = Key("mix");
	DataTypeMapHLSL["max"] = Key("max");
	DataTypeMapHLSL["pow"] = Key("pow");
	DataTypeMapHLSL["radians"] = Key("radians");
	DataTypeMapHLSL["reflect"] = Key("reflect");
	DataTypeMapHLSL["refract"] = Key("refract");
	DataTypeMapHLSL["round"] = Key("round");
	DataTypeMapHLSL["sign"] = Key("sign");
	DataTypeMapHLSL["sin"] = Key("sin");
	DataTypeMapHLSL["cos"] = Key("cos");
	DataTypeMapHLSL["smoothstep"] = Key("smoothstep");
	DataTypeMapHLSL["sqrt"] = Key("sqrt");
	DataTypeMapHLSL["step"] = Key("step");
	DataTypeMapHLSL["tan"] = Key("tan");
	DataTypeMapHLSL["tanh"] = Key("tanh");
	DataTypeMapHLSL["sinh"] = Key("sinh");
	DataTypeMapHLSL["cosh"] = Key("cosh");
	DataTypeMapHLSL["transpose"] = Key("transpose");
	DataTypeMapHLSL["trunc"] = Key("trunc");
	DataTypeMapHLSL["tex1D"] = Key("texture");
	DataTypeMapHLSL["tex2D"] = Key("texture");
	DataTypeMapHLSL["tex3D"] = Key("texture");
	DataTypeMapHLSL["tex1DBias"] = Key("textureOffset");
	DataTypeMapHLSL["tex2DBias"] = Key("textureOffset");
	DataTypeMapHLSL["tex3DBias"] = Key("textureOffset");
	DataTypeMapHLSL["tex1Dgrad"] = Key("textureGrad");
	DataTypeMapHLSL["tex2Dgrad"] = Key("textureGrad");
	DataTypeMapHLSL["tex3Dgrad"] = Key("textureGrad");
	DataTypeMapHLSL["tex1Dlod"] = Key("textureLod");
	DataTypeMapHLSL["tex2Dlod"] = Key("textureLod");
	DataTypeMapHLSL["tex3Dlod"] = Key("textureLod");
	DataTypeMapHLSL["tex1Dproj"] = Key("textureProj");
	DataTypeMapHLSL["min"] = Key("min");
	DataTypeMapHLSL["max"] = Key("max");
	DataTypeMapHLSL["min10float"] = Key("lowp float");
	DataTypeMapHLSL["min16float"] = Key("mediump float");
	DataTypeMapHLSL["min32float"] = Key("highp float");
	DataTypeMapHLSL["min10int"] = Key("lowp int");
	DataTypeMapHLSL["min16int"] = Key("mediump int");
	DataTypeMapHLSL["min32int"] = Key("highp int");
	DataTypeMapHLSL["min10uint"] = Key("lowp uint");
	DataTypeMapHLSL["min16uint"] = Key("mediump uint");
	DataTypeMapHLSL["min32uint"] = Key("highp uint");
	DataTypeMapHLSL["in"] = Key("in");
	DataTypeMapHLSL["out"] = Key("out");
	DataTypeMapHLSL["inout"] = Key("inout");
	DataTypeMapHLSL["uniform"] = Key("");
	DataTypeMapHLSL["noperspective"] = Key("noperspective");
	DataTypeMapHLSL["nointerpolation"] = Key("flat");
	DataTypeMapHLSL["linear"] = Key("smooth");
	DataTypeMapHLSL["centroid"] = Key("centroid");
	DataTypeMapHLSL["sample"] = Key("sample");
	return DataTypeMapHLSL;
}

const Savvy::Key& Savvy::Internal::DatabaseHLSL::GetOutputLangEquivalent(const Key& a_Name)
{
	if (OutputLangEquivalentExists(a_Name))
	{
		return m_DataTypeMapHLSL.at(a_Name);
	}
	return m_DataTypeMapHLSL.begin()->second;
}

const bool Savvy::Internal::DatabaseHLSL::OutputLangEquivalentExists(const Key& a_Name)
{
	uint32 count = m_DataTypeMapHLSL.count(a_Name);

	if (count > 0)
		return true;

	return false;
}
