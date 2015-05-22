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

#ifndef SAVVY_H
#define SAVVY_H

/*
The main Savvy header file.
This is the ONLY header file you need to include in your application, if you wish to use the built-in converters.
If that is not the case, include the SavvyFramework.h file instead.
*/

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