/*
The main Savvy header file.
This is the ONLY header file you need to include in your application, if you wish to use the built-in converters.
If that is not the case, include the SavvyFramework.h file instead.
*/
#ifndef SAVVY_H
#define SAVVY_H

#include "SavvyPCH.h"
#include "SavvyFrameworkDefines.h"
#include "SavvyDefines.h"
#include "SavvyBlob.h"
#include "SavvyConstructor.h"
#include "SavvyParser.h"
#include "SavvyScanner.h"
#include "SavvyKey.h"
#include "SavvyDatabase.h"
#include "SavvyShaderConverter.h"
#include "SavvyException.h"
#include "internal/SavvyScannerGLSL.h"
#include "internal/SavvyScannerHLSL.h"
#include "internal/SavvyParserGLSL.h"
#include "internal/SavvyParserHLSL.h"
#include "internal/SavvyDatabaseGLSL.h"
#include "internal/SavvyDatabaseHLSL.h"
#include "internal/SavvyConstructorGLSLToHLSL.h"
#include "internal/SavvyConstructorHLSLToGLSL.h"
#include "internal/SavvyConstructorHLSLToGLSLNoUBO.h"
#include "internal/SavvyConstructorHLSLToGLSLNoLoc.h"
#include "internal/SavvyConstructorHLSLToGLSLNoUBONoLoc.h"

#endif // !SAVVY_H