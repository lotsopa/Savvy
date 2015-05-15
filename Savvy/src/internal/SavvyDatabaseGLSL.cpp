#include "SavvyPCH.h"
#include "SavvyDatabaseGLSL.h"

Savvy::Internal::DatabaseGLSL::KeyMap Savvy::Internal::DatabaseGLSL::m_DataTypeMapGLSL = Savvy::Internal::DatabaseGLSL::CreateKeywordMap();

Savvy::Internal::DatabaseGLSL::DatabaseGLSL() : Database()
{

}

Savvy::Internal::DatabaseGLSL::~DatabaseGLSL()
{

}

void Savvy::Internal::DatabaseGLSL::Reset()
{
	Database::Reset();
}

Savvy::Database::KeyMap Savvy::Internal::DatabaseGLSL::CreateKeywordMap()
{
	KeyMap DataTypeMapGLSL;
	DataTypeMapGLSL["int"] = Key("int");
	DataTypeMapGLSL["bool"] = Key("bool");
	DataTypeMapGLSL["uint"] = Key("uint");
	DataTypeMapGLSL["float"] = Key("float");
	DataTypeMapGLSL["double"] = Key("double");
	DataTypeMapGLSL["void"] = Key("void");

	DataTypeMapGLSL["vec2"] = Key("float2");
	DataTypeMapGLSL["vec3"] = Key("float3");
	DataTypeMapGLSL["vec4"] = Key("float4");

	DataTypeMapGLSL["bvec2"] = Key("bool2");
	DataTypeMapGLSL["bvec3"] = Key("bool3");
	DataTypeMapGLSL["bvec4"] = Key("bool4");

	DataTypeMapGLSL["uvec2"] = Key("uint2");
	DataTypeMapGLSL["uvec3"] = Key("uint3");
	DataTypeMapGLSL["uvec4"] = Key("uint4");

	DataTypeMapGLSL["ivec2"] = Key("int2");
	DataTypeMapGLSL["ivec3"] = Key("int3");
	DataTypeMapGLSL["ivec4"] = Key("int4");

	DataTypeMapGLSL["dvec2"] = Key("double2");
	DataTypeMapGLSL["dvec3"] = Key("double3");
	DataTypeMapGLSL["dvec4"] = Key("double4");

	DataTypeMapGLSL["mat4x4"] = Key("matrix");
	DataTypeMapGLSL["mat4"] = Key("matrix");
	DataTypeMapGLSL["mat3"] = Key("float3x3");
	DataTypeMapGLSL["mat3x3"] = Key("float3x3");
	DataTypeMapGLSL["mat2"] = Key("float2x2");
	DataTypeMapGLSL["mat2x2"] = Key("float2x2");
	DataTypeMapGLSL["mat2x3"] = Key("float2x3");
	DataTypeMapGLSL["mat2x4"] = Key("float2x4");

	DataTypeMapGLSL["mat3x2"] = Key("float3x2");
	DataTypeMapGLSL["mat3x4"] = Key("float3x4");

	DataTypeMapGLSL["mat4x2"] = Key("float4x2");
	DataTypeMapGLSL["mat4x3"] = Key("float4x3");

	DataTypeMapGLSL["dmat4x4"] = Key("double4x4");
	DataTypeMapGLSL["dmat4"] = Key("double4x4");
	DataTypeMapGLSL["dmat3"] = Key("double3x3");
	DataTypeMapGLSL["dmat3x3"] = Key("double3x3");
	DataTypeMapGLSL["dmat2"] = Key("double2x2");
	DataTypeMapGLSL["dmat2x2"] = Key("double2x2");
	DataTypeMapGLSL["dmat2x3"] = Key("double2x3");
	DataTypeMapGLSL["dmat2x4"] = Key("double2x4");

	DataTypeMapGLSL["dmat3x2"] = Key("double3x2");
	DataTypeMapGLSL["dmat3x4"] = Key("double3x4");

	DataTypeMapGLSL["dmat4x2"] = Key("double4x2");
	DataTypeMapGLSL["dmat4x3"] = Key("double4x3");

	DataTypeMapGLSL["sampler2D"] = Key("Texture2D");
	DataTypeMapGLSL["sampler3D"] = Key("Texture3D");
	DataTypeMapGLSL["sampler1D"] = Key("Texture1D");
	DataTypeMapGLSL["sampler2DArray"] = Key("Texture2DArray");
	DataTypeMapGLSL["sampler1DArray"] = Key("Texture1DArray");
	DataTypeMapGLSL["usampler3D"] = Key("Texture3D<uint>");
	DataTypeMapGLSL["usampler2D"] = Key("Texture2D<int>");
	DataTypeMapGLSL["usampler1D"] = Key("Texture1D<int>");
	DataTypeMapGLSL["isampler3D"] = Key("Texture3D<int>");
	DataTypeMapGLSL["isampler2D"] = Key("Texture2D<int>");
	DataTypeMapGLSL["isampler1D"] = Key("Texture1D<int>");

	DataTypeMapGLSL["normalize"] = Key("normalize");
	DataTypeMapGLSL["clamp"] = Key("clamp");
	DataTypeMapGLSL["dot"] = Key("dot");
	DataTypeMapGLSL["const"] = Key("const");
	DataTypeMapGLSL["discard"] = Key("discard");
	DataTypeMapGLSL["abs"] = Key("abs");
	DataTypeMapGLSL["acos"] = Key("acos");
	DataTypeMapGLSL["all"] = Key("all");
	DataTypeMapGLSL["any"] = Key("any");
	DataTypeMapGLSL["asin"] = Key("asin");
	DataTypeMapGLSL["memoryBarrier"] = Key("AllMemoryBarrier");
	DataTypeMapGLSL["groupMemoryBarrier"] = Key("AllMemoryBarrierWithGroupSync");
	DataTypeMapGLSL["floatBitsToUint"] = Key("asuint");
	DataTypeMapGLSL["uintBitsToFloat"] = Key("asfloat");
	DataTypeMapGLSL["intBitsToFloat"] = Key("asfloat");
	DataTypeMapGLSL["bitCount"] = Key("countbits");
	DataTypeMapGLSL["bitfieldReverse"] = Key("reversebits");
	DataTypeMapGLSL["ceil"] = Key("ceil");
	DataTypeMapGLSL["cross"] = Key("cross");
	DataTypeMapGLSL["cos"] = Key("cos");
	DataTypeMapGLSL["degrees"] = Key("degrees");
	DataTypeMapGLSL["determinant"] = Key("determinant");
	DataTypeMapGLSL["dFdx"] = Key("ddx");
	DataTypeMapGLSL["dFdy"] = Key("ddy");
	DataTypeMapGLSL["dFdxCoarse"] = Key("ddx_coarse");
	DataTypeMapGLSL["dFdyCoarse"] = Key("ddy_coarse");
	DataTypeMapGLSL["dFdxFine"] = Key("ddx_fine");
	DataTypeMapGLSL["dFdyFine"] = Key("ddy_fine");
	DataTypeMapGLSL["distance"] = Key("distance");
	DataTypeMapGLSL["interpolateAtCentroid"] = Key("EvaluateAttributeAtCentroid");
	DataTypeMapGLSL["interpolateAtOffset"] = Key("EvaluateAttributeSnapped");
	DataTypeMapGLSL["interpolateAtSample"] = Key("EvaluateAttributeAtSample");
	DataTypeMapGLSL["exp"] = Key("exp");
	DataTypeMapGLSL["exp2"] = Key("exp2");
	DataTypeMapGLSL["faceforward"] = Key("faceforward");
	DataTypeMapGLSL["findLSB"] = Key("firstbitlow");
	DataTypeMapGLSL["findMSB"] = Key("firstbithigh");
	DataTypeMapGLSL["floor"] = Key("floor");
	DataTypeMapGLSL["fma"] = Key("fma");
	DataTypeMapGLSL["fract"] = Key("frac");
	DataTypeMapGLSL["frexp"] = Key("frexp");
	DataTypeMapGLSL["fwidth"] = Key("fwidth");
	DataTypeMapGLSL["modf"] = Key("modf");
	DataTypeMapGLSL["inversesqrt"] = Key("rsqrt");
	DataTypeMapGLSL["isinf"] = Key("isinf");
	DataTypeMapGLSL["isnan"] = Key("isnan");
	DataTypeMapGLSL["ldexp"] = Key("ldexp");
	DataTypeMapGLSL["length"] = Key("length");
	DataTypeMapGLSL["log"] = Key("log");
	DataTypeMapGLSL["log2"] = Key("log2");
	DataTypeMapGLSL["mix"] = Key("lerp");
	DataTypeMapGLSL["min"] = Key("min");
	DataTypeMapGLSL["noise"] = Key("noise");
	DataTypeMapGLSL["noise1"] = Key("noise");
	DataTypeMapGLSL["noise2"] = Key("noise");
	DataTypeMapGLSL["noise3"] = Key("noise");
	DataTypeMapGLSL["noise4"] = Key("noise");
	DataTypeMapGLSL["pow"] = Key("pow");
	DataTypeMapGLSL["round"] = Key("round");
	DataTypeMapGLSL["roundEven"] = Key("round");
	DataTypeMapGLSL["reflect"] = Key("reflect");
	DataTypeMapGLSL["refract"] = Key("refract");
	DataTypeMapGLSL["radians"] = Key("radians");
	DataTypeMapGLSL["sign"] = Key("sign");
	DataTypeMapGLSL["sin"] = Key("sin");
	DataTypeMapGLSL["cos"] = Key("cos");
	DataTypeMapGLSL["smoothstep"] = Key("smoothstep");
	DataTypeMapGLSL["sqrt"] = Key("sqrt");
	DataTypeMapGLSL["step"] = Key("step");
	DataTypeMapGLSL["tan"] = Key("tan");
	DataTypeMapGLSL["tanh"] = Key("tanh");
	DataTypeMapGLSL["sinh"] = Key("sinh");
	DataTypeMapGLSL["cosh"] = Key("cosh");
	DataTypeMapGLSL["transpose"] = Key("transpose");
	DataTypeMapGLSL["trunc"] = Key("trunc");
	DataTypeMapGLSL["smooth"] = Key("");
	DataTypeMapGLSL["flat"] = Key("");
	DataTypeMapGLSL["max"] = Key("max");
	DataTypeMapGLSL["min"] = Key("min");
	DataTypeMapGLSL["lowp"] = Key("min10");
	DataTypeMapGLSL["mediump"] = Key("min16");
	DataTypeMapGLSL["highp"] = Key("min16");
	DataTypeMapGLSL["in"] = Key("in");
	DataTypeMapGLSL["out"] = Key("out");
	DataTypeMapGLSL["inout"] = Key("inout");
	DataTypeMapGLSL["noperspective"] = Key("noperspective");
	DataTypeMapGLSL["flat"] = Key("nointerpolation");
	DataTypeMapGLSL["smooth"] = Key("linear");
	DataTypeMapGLSL["centroid"] = Key("centroid");
	DataTypeMapGLSL["sample"] = Key("sample");
	return DataTypeMapGLSL;
}

const Savvy::Key& Savvy::Internal::DatabaseGLSL::GetOutputLangEquivalent(const Key& a_Name)
{
	return m_DataTypeMapGLSL.at(a_Name);
}

const bool Savvy::Internal::DatabaseGLSL::OutputLangEquivalentExists(const Key& a_Name)
{
	uint32 count = m_DataTypeMapGLSL.count(a_Name);

	if (count > 0)
		return true;

	return false;
}
