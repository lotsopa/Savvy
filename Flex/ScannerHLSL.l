%option noyywrap
%option nounistd
%option yylineno
%option C++ 
%option yyclass="hlslFlexLexer"
%option prefix="hlsl"
%option nodefault

/* HLSL scanner */
%{
#include"SavvyScannerHLSL.h"
%}

DIGIT [0-9]
ID [A-Za-z_][A-Za-z0-9_]*

MAT2X2 "float2x2"|"matrix"
MAT3X3 "float3x3"|"matrix"
MAT4X4 "float4x4"|"matrix"

DMAT2X2 "double2x2"|"matrix"
DMAT3X3 "double3x3"|"matrix"
DMAT4X4 "double4x4"|"matrix"
STRING \"(\\.|[^"])*\"
%%

"//".+ 

"/*" { int c1 = 0, c2 = yyinput();
       for(;;) {
         if(c2 == EOF) break;
         if(c1 == '*' && c2 == '/')
           break;
         c1 = c2;
         c2 = yyinput();
       }
     }

"#".+

"true" return Savvy::SAVVY_BOOL_VALUE;
"false" return Savvy::SAVVY_BOOL_VALUE;
"string" return Savvy::SAVVY_DATA_TYPE;
"bool" return Savvy::SAVVY_DATA_TYPE;
"int" return Savvy::SAVVY_DATA_TYPE;
"uint" return Savvy::SAVVY_DATA_TYPE;
"float" return Savvy::SAVVY_DATA_TYPE;
"double" return Savvy::SAVVY_DATA_TYPE;
"dword" return Savvy::SAVVY_DATA_TYPE;
"half" return Savvy::SAVVY_DATA_TYPE;
"min16float" return Savvy::SAVVY_DATA_TYPE;
"min10float" return Savvy::SAVVY_DATA_TYPE;
"min32float" return Savvy::SAVVY_DATA_TYPE;
"min16int" return Savvy::SAVVY_DATA_TYPE;
"min32int" return Savvy::SAVVY_DATA_TYPE;
"const" return Savvy::SAVVY_DATA_TYPE;
"static" return Savvy::SAVVY_DATA_TYPE;
"min10" return Savvy::SAVVY_DATA_TYPE;
"min16" return Savvy::SAVVY_DATA_TYPE;
"min32" return Savvy::SAVVY_DATA_TYPE;
"in" return Savvy::SAVVY_DATA_TYPE;
"out" return Savvy::SAVVY_DATA_TYPE;
"inout" return Savvy::SAVVY_DATA_TYPE;
"uniform" return Savvy::SAVVY_DATA_TYPE;

"float2" return Savvy::SAVVY_DATA_TYPE;
"float3" return Savvy::SAVVY_DATA_TYPE;
"float4" return Savvy::SAVVY_DATA_TYPE;
"vector" return Savvy::SAVVY_DATA_TYPE;

"int2" return Savvy::SAVVY_DATA_TYPE;
"int3" return Savvy::SAVVY_DATA_TYPE;
"int4" return Savvy::SAVVY_DATA_TYPE;

"uint2" return Savvy::SAVVY_DATA_TYPE;
"uint3" return Savvy::SAVVY_DATA_TYPE;
"uint4" return Savvy::SAVVY_DATA_TYPE;

"bool2" return Savvy::SAVVY_DATA_TYPE;
"bool3" return Savvy::SAVVY_DATA_TYPE;
"bool4" return Savvy::SAVVY_DATA_TYPE;

"double2" return Savvy::SAVVY_DATA_TYPE;
"double3" return Savvy::SAVVY_DATA_TYPE;
"double4" return Savvy::SAVVY_DATA_TYPE;

{MAT2X2} return Savvy::SAVVY_DATA_TYPE;
"float2x3" return Savvy::SAVVY_DATA_TYPE;
"float2x4" return Savvy::SAVVY_DATA_TYPE;

{DMAT2X2} return Savvy::SAVVY_DATA_TYPE;
"double2x3" return Savvy::SAVVY_DATA_TYPE;
"double2x4" return Savvy::SAVVY_DATA_TYPE;

{MAT3X3} return Savvy::SAVVY_DATA_TYPE;
"float3x2" return Savvy::SAVVY_DATA_TYPE;
"float3x4" return Savvy::SAVVY_DATA_TYPE;

{DMAT3X3} return Savvy::SAVVY_DATA_TYPE;
"double3x2" return Savvy::SAVVY_DATA_TYPE;
"double3x4" return Savvy::SAVVY_DATA_TYPE;

{MAT4X4} return Savvy::SAVVY_DATA_TYPE;
"float4x2" return Savvy::SAVVY_DATA_TYPE;
"floatx3" return Savvy::SAVVY_DATA_TYPE;

{DMAT4X4} return Savvy::SAVVY_DATA_TYPE;
"double4x2" return Savvy::SAVVY_DATA_TYPE;
"double4x3" return Savvy::SAVVY_DATA_TYPE;

"struct" return Savvy::SAVVY_DATA_TYPE;

"texture" return Savvy::SAVVY_DATA_TYPE;
"Texture1D" return Savvy::SAVVY_DATA_TYPE;
"Texture2D" return Savvy::SAVVY_DATA_TYPE;
"Texture2DArray" return Savvy::SAVVY_DATA_TYPE;
"Texture3D" return Savvy::SAVVY_DATA_TYPE;
"TextureCube" return Savvy::SAVVY_DATA_TYPE;

"sampler1D" return Savvy::SAVVY_DATA_TYPE;
"sampler2D" return Savvy::SAVVY_DATA_TYPE;
"sampler3D" return Savvy::SAVVY_DATA_TYPE;
"SamplerState" return Savvy::SAVVY_DATA_TYPE;
"cbuffer" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"StructuredBuffer" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"Buffer" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;

"void" return Savvy::SAVVY_DATA_TYPE;
"!" return Savvy::SAVVY_OPERATOR;
"==" return Savvy::SAVVY_OPERATOR;
"!=" return Savvy::SAVVY_OPERATOR;
"++" return Savvy::SAVVY_OPERATOR;
"--" return Savvy::SAVVY_OPERATOR;
"+=" return Savvy::SAVVY_OPERATOR;
"-=" return Savvy::SAVVY_OPERATOR;
"%=" return Savvy::SAVVY_OPERATOR;
"&=" return Savvy::SAVVY_OPERATOR;
"|=" return Savvy::SAVVY_OPERATOR;
"->" return Savvy::SAVVY_OPERATOR;
"##" return Savvy::SAVVY_OPERATOR;
"#@" return Savvy::SAVVY_OPERATOR;
"::" return Savvy::SAVVY_OPERATOR;
"*=" return Savvy::SAVVY_OPERATOR;
"/=" return Savvy::SAVVY_OPERATOR;
"&&" return Savvy::SAVVY_OPERATOR;
">=" return Savvy::SAVVY_OPERATOR;
"<=" return Savvy::SAVVY_OPERATOR;
">>=" return Savvy::SAVVY_OPERATOR;
"<<=" return Savvy::SAVVY_OPERATOR;
"^=" return Savvy::SAVVY_OPERATOR;
"~" return Savvy::SAVVY_OPERATOR;
"|" return Savvy::SAVVY_OPERATOR;
">>" return Savvy::SAVVY_OPERATOR;
"<<" return Savvy::SAVVY_OPERATOR;
"&" return Savvy::SAVVY_OPERATOR;

">" return Savvy::SAVVY_OPERATOR;
"<" return Savvy::SAVVY_OPERATOR;
"%" return Savvy::SAVVY_OPERATOR;
"." return Savvy::SAVVY_OPERATOR;
"," return Savvy::SAVVY_OPERATOR;
":" return Savvy::SAVVY_OPERATOR;
";" return Savvy::SAVVY_INSTRUCTION_END;
"+" return Savvy::SAVVY_OPERATOR;
"-" return Savvy::SAVVY_OPERATOR;
"=" return Savvy::SAVVY_OPERATOR;
"*" return Savvy::SAVVY_OPERATOR;
"/" return Savvy::SAVVY_OPERATOR;
"(" return Savvy::SAVVY_OPEN_BRACKET;
")" return Savvy::SAVVY_CLOSE_BRACKET;
"{" return Savvy::SAVVY_OPEN_BRACE;
"}" return Savvy::SAVVY_CLOSE_BRACE;
"[" return Savvy::SAVVY_OPEN_SQUARE_BRACKET;
"]" return Savvy::SAVVY_CLOSE_SQUARE_BRACKET;
"if" return Savvy::SAVVY_IF_STATEMENT;
"else" return Savvy::SAVVY_ELSE_STATEMENT;
"switch" return Savvy::SAVVY_SWITCH_STATEMENT;
"break" return Savvy::SAVVY_BREAK_STATEMENT;
"case" return Savvy::SAVVY_CASE_STATEMENT;
"for" return Savvy::SAVVY_FOR_STATEMENT;
"do" return Savvy::SAVVY_DO_STATEMENT;
"while" return Savvy::SAVVY_WHILE_STATEMENT;
"continue" return Savvy::SAVVY_CONTINUE_STATEMENT;

"Load" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"abort" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"abs" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"acos" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"all" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"AllMemoryBarrier" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"AllMemoryBarrierWithGroupSync" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"any" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"asdouble" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"asfloat" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"asin" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"asint" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"asuint" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"atan" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"atan2" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"ceil" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"CheckAccessFullyMapped" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"clamp" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"clip" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"cos" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"cosh" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"countbits" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"cross" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"D3DCOLORtoUBYTE4" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"ddx" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"ddx_coarse" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"ddx_fine" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"ddy" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"ddy_coarse" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"ddy_fine" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"degrees" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"determinant" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"DeviceMemoryBarrier" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"DeviceMemoryBarrierWithGroupSync" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"distance" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"dot" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"dst" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"errorf" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"EvaluateAttributeAtCentroid" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"EvaluateAttributeAtSample" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"EvaluateAttributeSnapped" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"exp" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"exp2" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"f16tof32" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"f32tof16" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"faceforward" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"firstbithigh" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"firstbitlow" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"floor" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"fma" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"fmod" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"frac" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"frexp" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"fwidth" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"GetRenderTargetSampleCount" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"GetRenderTargetSamplePosition" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"GroupMemoryBarrier" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"GroupMemoryBarrierWithGroupSync" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"InterlockedAdd" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"InterlockedAnd" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"InterlockedCompareExchange" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"InterlockedCompareStore" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"InterlockedExchange" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"InterlockedMax" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"InterlockedMin" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"InterlockedOr" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"InterlockedXor" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"isfinite" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"isinf" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"isnan" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"ldexp" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"length" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"lerp" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"lit" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"log" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"log10" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"log2" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"mad" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"max" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"min" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"modf" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"msad4" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"mul" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"noise" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"normalize" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"pow" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"printf" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"Process2DQuadTessFactorsAvg" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"Process2DQuadTessFactorsMax" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"Process2DQuadTessFactorsMin" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"ProcessIsolineTessFactors" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"ProcessQuadTessFactorsAvg" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"ProcessQuadTessFactorsMax" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"ProcessQuadTessFactorsMin" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"ProcessTriTessFactorsAvg" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"ProcessTriTessFactorsMax" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"ProcessTriTessFactorsMin" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"radians" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"rcp" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"reflect" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"refract" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"reversebits" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"round" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"rsqrt" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"saturate" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"sign" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"sin" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"sincos" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"sinh" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"smoothstep" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"sqrt" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"step" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"tan" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"tanh" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"tex1D" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"tex1Dbias" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"tex1Dgrad" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"tex1Dlod" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"tex1Dproj" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"tex2D" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"tex2Dbias" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"tex2Dgrad" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"tex2Dlod" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"tex2Dproj" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"tex3D" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"tex3Dproj" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"texCUBE" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"texCUBEbias" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"texCUBEgrad" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"texCUBElod" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"texCUBEproj" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"transpose" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"trunc" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"GetDimensions" return Savvy::SAVVY_BUILT_IN_FUNCTION;

"return" return Savvy::SAVVY_RETURN_KEYWORD;

"snorm" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"unorm" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"typedef" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"linear" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"centroid" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"nointerpolation" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"noperspective" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"Sample" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"SampleLevel" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"SV_Position" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"SV_ClipDistance["{DIGIT}*"]" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"SV_CullDistance["{DIGIT}*"]" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"SV_Coverage" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"SV_Depth" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"SV_DispatchThreadID" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"SV_DomainLocation" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"SV_GroupID" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"SV_GroupIndex" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"SV_GroupThreadID" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"SV_GSInstanceID" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"SV_InsideTessFactor" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"SV_IsFrontFace" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"SV_OutputControlPointID" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"SV_RenderTargetArrayIndex" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"SV_SampleIndex" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"SV_TessFactor" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"SV_ViewportArrayIndex" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"SV_PrimitiveID" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"SV_VertexID" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"SV_POSITION" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"SV_CLIPDISTANCE["{DIGIT}*"]" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"SV_CULLDISTANCE["{DIGIT}*"]" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"SV_COVERAGE" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"SV_DEPTH" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"SV_DISPATCHTHREADID" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"SV_DOMAINLOCATION" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"SV_GROUPID" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"SV_GROUPINDEX" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"SV_GROUPTHREADID" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"SV_GSINSTANCEID" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"SV_INSIDETESSFACTOR" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"SV_ISFRONTFACE" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"SV_OUTPUTCONTROLPOINTID" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"SV_RENDERTARGETARRAYINDEX" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"SV_SAMPLEINDEX" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"SV_TESSFACTOR" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"SV_VIEWPORTARRAYINDEX" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"SV_PRIMITIVEID" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"SV_VERTEXID" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"SV_TARGET" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"SV_Target" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"SV_Target["{DIGIT}*"]" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"SV_TARGET["{DIGIT}*"]" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"register" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"discard" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;

{ID} return Savvy::SAVVY_IDENTIFIER;
{DIGIT}+ return Savvy::SAVVY_INTEGER_VALUE;
{DIGIT}+"."{DIGIT}*"f" return Savvy::SAVVY_FLOAT_VALUE;
{DIGIT}+"."{DIGIT}* return Savvy::SAVVY_FLOAT_VALUE;
{DIGIT}+"."{DIGIT}*"e-"{DIGIT}+ return Savvy::SAVVY_FLOAT_VALUE;
{DIGIT}+"."{DIGIT}*"e+"{DIGIT}+ return Savvy::SAVVY_FLOAT_VALUE;
{DIGIT}+"."{DIGIT}*"e"{DIGIT}+ return Savvy::SAVVY_FLOAT_VALUE;
"0x"{DIGIT}* return Savvy::SAVVY_INTEGER_VALUE;
"0x"{ID} return Savvy::SAVVY_INTEGER_VALUE;
{STRING} return Savvy::SAVVY_STRING;
[\n] return Savvy::SAVVY_NEW_LINE;
[ \t]+
. return Savvy::SAVVY_UNKNOWN_TOKEN;
%%