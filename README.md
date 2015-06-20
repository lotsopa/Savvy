# Savvy
A Generic and Flexible Shader Cross Compiler Library/Tool with full preprocessor support, which also includes a text editor.

# What is Savvy?
Savvy is a library/tool which lets you convert shaders from and to different languages. Currently supported out of the box are the HLSL 5.0 (DirectX 11) and GLSL 4.5 (OpenGL) shader languages. However, the architecture is generic and easily extendable, which means anyone can write their own converter for their favourite shading language.

The tool is under active development and from version 2.0 onward, includes a GUI Editor in the style of Notepad++ which supports all standard text editor features as well as syntax highlighting and automatic conversion of the above mentioned languages.

# Dependencies
As of Version 2.0, Savvy depends on:
1. wxWidgets - A GUI library which is used by the Savvy Standalone project ONLY.
2. mcpp - A C/C++ Preprocessor used by the tool in order to translate preprocessor directives and includes.

## Important
All dependencies are included in the ext folder, however they are only compiled for Windows 32-bit and Visual Studio 2013. If you wish to compile Savvy on another OS or with a different compiler, you must replace the provided compiled libraries with a compiled version for your particular Compiler/OS.

## How to build the dependencies for a different Compiler/OS

### WxWidgtets
1. Go to https://www.wxwidgets.org/

2. Download the source package of the latest release (At the time of writing - version 3.0.2)

3. Follow the instruction on the wxWidgets Website to build the UNICODE(Very important) version of the library and link against it.

4. Copy and Paste the library you just built in the ext/wxWidgets/lib folder. (do the same for the include folder as well, if you used a newer version of wxWidgets)

### MCPP
1. Navigate to ext/mcpp/include

2. Open README.md

3. Follow the instructions

# How to compile
Savvy uses Premake 5 (You can find it here: https://premake.github.io/) to generate the solution/project files needed to compile the project. Here's a step by step guide on how to generate the solution/project files for your favourite IDE:

1. Download the latest Premake 5 version from the link above.

2. Extract and place the Premake 5 executable in the root of Savvy (The Premake 5 executable should be in the same folder as the premake5.lua file)

3. Use Command Prompt (or Terminal) to navigate to the root Savvy folder on your computer.

4. Execute the command, without the quotes: "premake5 Your_IDE_Name" (For example: "premake5 vs2013" for Visual Studio 2013 IDE)N.B: You can check if your IDE is supported by Premake 5 using the link above.

5. Open the Solution in your favourite IDE and press the compile button.

# Project Breakdown
After you've opened the solution, you should see three projects: Sample, Savvy and Savvy Standalone.

## Sample
The Sample is a project which shows different uses of of the Savvy library and demonstrates how to extend it. Treat it as sample code and nothing more.

## Savvy
This is the bread and butter of the tool. This project contains all the Savvy functionality and implementation of the converters.

## Savvy Standalone
This project is the standalone version of the tool, which is basically a GUI editor application. It includes the above mentioned Savvy project in order to provide the user with an interface for converting shaders.

# More Documentation and Tutorials Coming Soon
In the meantime, if you have any questions/suggestions you can always contact me at: 

apostol_dadachev[at]yahoo.com or Twitter: @ApostolDadachev
