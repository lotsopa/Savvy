%option noyywrap
%option nounistd
%option yylineno
%option C++
%option yyclass="glslFlexLexer"
%option prefix="glsl"
%option nodefault

/* GLSL scanner */
%{
#include"SavvyScannerGLSL.h"
%}

DIGIT [0-9]
ID [A-Za-z_][A-Za-z0-9_]*

MAT2X2 "mat2"|"mat2x2"
MAT3X3 "mat3"|"mat3x3"
MAT4X4 "mat4"|"mat4x4"

DMAT2X2 "dmat2"|"dmat2x2"
DMAT3X3 "dmat3"|"dmat3x3"
DMAT4X4 "dmat4"|"dmat4x4"


%%

"//".+

"/* Original Semantic: " return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;

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
"bool" return Savvy::SAVVY_DATA_TYPE;
"int" return Savvy::SAVVY_DATA_TYPE;
"uint" return Savvy::SAVVY_DATA_TYPE;
"float" return Savvy::SAVVY_DATA_TYPE;
"double" return Savvy::SAVVY_DATA_TYPE;

"bvec2" return Savvy::SAVVY_DATA_TYPE;
"bvec3" return Savvy::SAVVY_DATA_TYPE;
"bvec4" return Savvy::SAVVY_DATA_TYPE;

"ivec2" return Savvy::SAVVY_DATA_TYPE;
"ivec3" return Savvy::SAVVY_DATA_TYPE;
"ivec4" return Savvy::SAVVY_DATA_TYPE;

"uvec2" return Savvy::SAVVY_DATA_TYPE;
"uvec3" return Savvy::SAVVY_DATA_TYPE;
"uvec4" return Savvy::SAVVY_DATA_TYPE;

"vec2" return Savvy::SAVVY_DATA_TYPE;
"vec3" return Savvy::SAVVY_DATA_TYPE;
"vec4" return Savvy::SAVVY_DATA_TYPE;

"dvec2" return Savvy::SAVVY_DATA_TYPE;
"dvec3" return Savvy::SAVVY_DATA_TYPE;
"dvec4" return Savvy::SAVVY_DATA_TYPE;

{MAT2X2} return Savvy::SAVVY_DATA_TYPE;
"mat2x3" return Savvy::SAVVY_DATA_TYPE;
"mat2x4" return Savvy::SAVVY_DATA_TYPE;

{DMAT2X2} return Savvy::SAVVY_DATA_TYPE;
"dmat2x3" return Savvy::SAVVY_DATA_TYPE;
"dmat2x4" return Savvy::SAVVY_DATA_TYPE;

{MAT3X3} return Savvy::SAVVY_DATA_TYPE;
"mat3x2" return Savvy::SAVVY_DATA_TYPE;
"mat3x4" return Savvy::SAVVY_DATA_TYPE;

{DMAT3X3} return Savvy::SAVVY_DATA_TYPE;
"dmat3x2" return Savvy::SAVVY_DATA_TYPE;
"dmat3x4" return Savvy::SAVVY_DATA_TYPE;

{MAT4X4} return Savvy::SAVVY_DATA_TYPE;
"mat4x2" return Savvy::SAVVY_DATA_TYPE;
"mat4x3" return Savvy::SAVVY_DATA_TYPE;

{DMAT4X4} return Savvy::SAVVY_DATA_TYPE;
"dmat4x2" return Savvy::SAVVY_DATA_TYPE;
"dmat4x3" return Savvy::SAVVY_DATA_TYPE;

"struct" return Savvy::SAVVY_DATA_TYPE;
"uniform" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;

"image1D" return Savvy::SAVVY_DATA_TYPE;
"image2D" return Savvy::SAVVY_DATA_TYPE;
"image3D" return Savvy::SAVVY_DATA_TYPE;
"uimage1D" return Savvy::SAVVY_DATA_TYPE;
"uimage2D" return Savvy::SAVVY_DATA_TYPE;
"uimage3D" return Savvy::SAVVY_DATA_TYPE;
"iimage1D" return Savvy::SAVVY_DATA_TYPE;
"iimage2D" return Savvy::SAVVY_DATA_TYPE;
"iimage3D" return Savvy::SAVVY_DATA_TYPE;
"imageCube" return Savvy::SAVVY_DATA_TYPE;
"iimageCube" return Savvy::SAVVY_DATA_TYPE;
"uimageCube" return Savvy::SAVVY_DATA_TYPE;
"image2DRect" return Savvy::SAVVY_DATA_TYPE;
"uimage2DRect" return Savvy::SAVVY_DATA_TYPE;
"iimage2DRect" return Savvy::SAVVY_DATA_TYPE;
"image1DArray" return Savvy::SAVVY_DATA_TYPE;
"uimage1DArray" return Savvy::SAVVY_DATA_TYPE;
"iimage1DArray" return Savvy::SAVVY_DATA_TYPE;
"image2DArray" return Savvy::SAVVY_DATA_TYPE;
"uimage2DArray" return Savvy::SAVVY_DATA_TYPE;
"iimage2DArray" return Savvy::SAVVY_DATA_TYPE;
"imageCubeArray" return Savvy::SAVVY_DATA_TYPE;
"uimageCubeArray" return Savvy::SAVVY_DATA_TYPE;
"iimageCubeArray" return Savvy::SAVVY_DATA_TYPE;
"imageBuffer" return Savvy::SAVVY_DATA_TYPE;
"uimageBuffer" return Savvy::SAVVY_DATA_TYPE;
"iimageBuffer" return Savvy::SAVVY_DATA_TYPE;
"image2DMS" return Savvy::SAVVY_DATA_TYPE;
"uimage2DMS" return Savvy::SAVVY_DATA_TYPE;
"iimage2DMS" return Savvy::SAVVY_DATA_TYPE;
"image2DMSArray" return Savvy::SAVVY_DATA_TYPE;
"uimage2DMSArray" return Savvy::SAVVY_DATA_TYPE;
"iimage2DMSArray" return Savvy::SAVVY_DATA_TYPE;

"sampler1D" return Savvy::SAVVY_DATA_TYPE;
"sampler2D" return Savvy::SAVVY_DATA_TYPE;
"sampler3D" return Savvy::SAVVY_DATA_TYPE;
"isampler1D" return Savvy::SAVVY_DATA_TYPE;
"isampler2D" return Savvy::SAVVY_DATA_TYPE;
"isampler3D" return Savvy::SAVVY_DATA_TYPE;
"usampler1D" return Savvy::SAVVY_DATA_TYPE;
"usampler2D" return Savvy::SAVVY_DATA_TYPE;
"usampler3D" return Savvy::SAVVY_DATA_TYPE;
"samplerCube" return Savvy::SAVVY_DATA_TYPE;
"isamplerCube" return Savvy::SAVVY_DATA_TYPE;
"usamplerCube" return Savvy::SAVVY_DATA_TYPE;
"sampler2DRect" return Savvy::SAVVY_DATA_TYPE;
"usampler2DRect" return Savvy::SAVVY_DATA_TYPE;
"isampler2DRect" return Savvy::SAVVY_DATA_TYPE;
"sampler1DArray" return Savvy::SAVVY_DATA_TYPE;
"usampler1DArray" return Savvy::SAVVY_DATA_TYPE;
"isampler1DArray" return Savvy::SAVVY_DATA_TYPE;
"sampler2DArray" return Savvy::SAVVY_DATA_TYPE;
"usampler2DArray" return Savvy::SAVVY_DATA_TYPE;
"isampler2DArray" return Savvy::SAVVY_DATA_TYPE;
"samplerCubeArray" return Savvy::SAVVY_DATA_TYPE;
"usamplerCubeArray" return Savvy::SAVVY_DATA_TYPE;
"isamplerCubeArray" return Savvy::SAVVY_DATA_TYPE;
"samplerBuffer" return Savvy::SAVVY_DATA_TYPE;
"usamplerBuffer" return Savvy::SAVVY_DATA_TYPE;
"isamplerBuffer" return Savvy::SAVVY_DATA_TYPE;
"sampler2DMS" return Savvy::SAVVY_DATA_TYPE;
"usampler2DMS" return Savvy::SAVVY_DATA_TYPE;
"isampler2DMS" return Savvy::SAVVY_DATA_TYPE;
"sampler2DMSArray" return Savvy::SAVVY_DATA_TYPE;
"usampler2DMSArray" return Savvy::SAVVY_DATA_TYPE;
"isampler2DMSArray" return Savvy::SAVVY_DATA_TYPE;
"sampler1DShadow" return Savvy::SAVVY_DATA_TYPE;
"sampler2DShadow" return Savvy::SAVVY_DATA_TYPE;
"samplerCubeShadow" return Savvy::SAVVY_DATA_TYPE;
"sampler2DRectShadow" return Savvy::SAVVY_DATA_TYPE;
"sampler1DArrayShadow" return Savvy::SAVVY_DATA_TYPE;
"sampler2DArrayShadow" return Savvy::SAVVY_DATA_TYPE;
"samplerCubeArrayShadow" return Savvy::SAVVY_DATA_TYPE;
"atomic_uint" return Savvy::SAVVY_DATA_TYPE;
"const" return Savvy::SAVVY_DATA_TYPE;
"invariant" return Savvy::SAVVY_DATA_TYPE;
"writeonly" return Savvy::SAVVY_DATA_TYPE;
"readonly" return Savvy::SAVVY_DATA_TYPE;
"restrict" return Savvy::SAVVY_DATA_TYPE;
"volatile" return Savvy::SAVVY_DATA_TYPE;
"coherent" return Savvy::SAVVY_DATA_TYPE;
"highp" return Savvy::SAVVY_DATA_TYPE;
"mediump" return Savvy::SAVVY_DATA_TYPE;
"lowp" return Savvy::SAVVY_DATA_TYPE;
"flat" return Savvy::SAVVY_DATA_TYPE;
"smooth" return Savvy::SAVVY_DATA_TYPE;
"noperspective" return Savvy::SAVVY_DATA_TYPE;

"void" return Savvy::SAVVY_DATA_TYPE;
"!" return Savvy::SAVVY_OPERATOR;
"==" return Savvy::SAVVY_OPERATOR;
"!=" return Savvy::SAVVY_OPERATOR;
"++" return Savvy::SAVVY_OPERATOR;
"--" return Savvy::SAVVY_OPERATOR;
"+=" return Savvy::SAVVY_OPERATOR;
"-=" return Savvy::SAVVY_OPERATOR;
"%=" return Savvy::SAVVY_OPERATOR;
"*=" return Savvy::SAVVY_OPERATOR;
"/=" return Savvy::SAVVY_OPERATOR;
"&&" return Savvy::SAVVY_OPERATOR;
">=" return Savvy::SAVVY_OPERATOR;
"<=" return Savvy::SAVVY_OPERATOR;
"&=" return Savvy::SAVVY_OPERATOR;
"|=" return Savvy::SAVVY_OPERATOR;
">>" return Savvy::SAVVY_OPERATOR;
"<<" return Savvy::SAVVY_OPERATOR;
"&" return Savvy::SAVVY_OPERATOR;

"~" return Savvy::SAVVY_OPERATOR;
"|" return Savvy::SAVVY_OPERATOR;
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

"abs" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"acos" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"acosh" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"all" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"any" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"asin" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"asinh" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"atan" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"atanh" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"atomicAdd" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"atomicAnd" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"atomicCompSwap" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"atomicCounter" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"atomicCounterDecrement" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"atomicCounterIncrement" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"atomicExchange" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"atomicMax" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"atomicMin" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"atomicOr" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"atomicXor" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"barrier" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"bitCount" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"bitfieldExtract" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"bitfieldInsert" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"bitfieldReverse" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"ceil" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"clamp" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"cos" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"cosh" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"cross" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"degrees" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"determinant" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"dFdx" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"dFdxCoarse" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"dFdxFine" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"dFdy" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"dFdyCoarse" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"dFdyFine" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"distance" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"dot" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"EmitStreamVertex" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"EmitVertex" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"EndPrimitive" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"EndStreamPrimitive" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"equal" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"exp" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"exp2" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"faceforward" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"findLSB" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"findMSB" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"floatBitsToInt" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"floatBitsToUint" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"floor" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"fma" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"fract" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"frexp" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"fwidth" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"fwidthCoarse" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"fwidthFine" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"greaterThan" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"greaterThanEqual" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"groupMemoryBarrier" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"imageAtomicAdd" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"imageAtomicAnd" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"imageAtomicCompSwap" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"imageAtomicExchange" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"imageAtomicMax" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"imageAtomicMin" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"imageAtomicOr" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"imageAtomicXor" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"imageLoad" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"imageSamples" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"imageSize" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"imageStore" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"imulExtended" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"intBitsToFloat" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"interpolateAtCentroid" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"interpolateAtOffset" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"interpolateAtSample" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"inverse" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"inversesqrt" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"isinf" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"isnan" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"ldexp" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"length" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"lessThan" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"lessThanEqual" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"log" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"log2" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"matrixCompMult" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"max" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"memoryBarrier" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"memoryBarrierAtomicCounter" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"memoryBarrierBuffer" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"memoryBarrierImage" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"memoryBarrierShared" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"min" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"mix" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"mod" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"modf" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"noise" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"noise1" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"noise2" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"noise3" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"noise4" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"normalize" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"not" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"notEqual" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"outerProduct" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"packDouble2x32" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"packHalf2x16" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"packSnorm2x16" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"packSnorm4x8" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"packUnorm" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"packUnorm2x16" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"packUnorm4x8" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"pow" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"radians" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"reflect" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"refract" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"round" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"roundEven" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"sign" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"sin" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"sinh" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"smoothstep" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"sqrt" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"step" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"tan" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"tanh" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"texelFetch" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"texelFetchOffset" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"texture" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"textureGather" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"textureGatherOffset" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"textureGatherOffsets" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"textureGrad" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"textureGradOffset" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"textureLod" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"textureLodOffset" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"textureOffset" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"textureProj" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"textureProjGrad" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"textureProjGradOffset" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"textureProjLod" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"textureProjLodOffset" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"textureProjOffset" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"textureQueryLevels" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"textureQueryLod" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"textureSamples" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"textureSize" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"transpose" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"trunc" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"uaddCarry" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"uintBitsToFloat" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"umulExtended" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"unpackDouble2x32" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"unpackHalf2x16" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"unpackSnorm2x16" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"unpackSnorm4x8" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"unpackUnorm" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"unpackUnorm2x16" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"unpackUnorm4x8" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"usubBorrow" return Savvy::SAVVY_BUILT_IN_FUNCTION;

"out" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"in" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"layout" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"gl_ClipDistance["{DIGIT}*"]" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"gl_CullDistance["{DIGIT}*"]" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"gl_FragCoord" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"gl_FragDepth" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"gl_FrontFacing" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"gl_GlobalInvocationID" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"gl_HelperInvocation" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"gl_InstanceID" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"gl_InvocationID" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"gl_Layer" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"gl_LocalInvocationID" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"gl_LocalInvocationIndex" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"gl_NumSamples" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"gl_NumWorkGroups" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"gl_PatchVerticesIn" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"gl_PointCoord" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"gl_PointSize" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"gl_Position" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"gl_PrimitiveID" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"gl_PrimitiveIDIn" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"gl_SampleID" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"gl_SampleMask" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"gl_SampleMaskIn" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"gl_SamplePosition" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"gl_TessCoord" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"gl_TessLevelInner" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"gl_TessLevelOuter" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"gl_VertexID" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"gl_ViewportIndex" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"gl_WorkGroupID" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"gl_WorkGroupSize" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"location" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"binding" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"std140" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"packed" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"shared" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"std340" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"std430" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"core" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"#" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"version" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"discard" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"buffer" return Savvy::SAVVY_LANGUAGE_SPECIFIC_KEYWORD;
"texture" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"mix" return Savvy::SAVVY_BUILT_IN_FUNCTION;
"return" return Savvy::SAVVY_RETURN_KEYWORD;

{ID} return Savvy::SAVVY_IDENTIFIER;
{DIGIT}+ return Savvy::SAVVY_INTEGER_VALUE;
{DIGIT}+"."{DIGIT}* return Savvy::SAVVY_FLOAT_VALUE;
{DIGIT}+"."{DIGIT}*"f" return Savvy::SAVVY_FLOAT_VALUE;
{DIGIT}+"."{DIGIT}*"e-"{DIGIT}+ return Savvy::SAVVY_FLOAT_VALUE;
{DIGIT}+"."{DIGIT}*"e+"{DIGIT}+ return Savvy::SAVVY_FLOAT_VALUE;
{DIGIT}+"."{DIGIT}*"e"{DIGIT}+ return Savvy::SAVVY_FLOAT_VALUE;
"0x"{DIGIT}* return Savvy::SAVVY_INTEGER_VALUE;
"0x"{ID} return Savvy::SAVVY_INTEGER_VALUE;
[\n] return Savvy::SAVVY_NEW_LINE;
[ \t]+
. return Savvy::SAVVY_UNKNOWN_TOKEN;

%%