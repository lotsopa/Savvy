# Savvy
A Generic and Flexible Shader Cross Compiler Library/Tool.

# What is Savvy?
Savvy is a library/tool which lets you convert shaders from and to different languages. Currently supported out of the box are the HLSL 5.0 (DirectX 11) and GLSL 4.5 (OpenGL) shader languages. However, the architecture is generic and easily extendable, which means anyone can write their own converter for their favourite shading language.

# How to compile?
Savvy uses Premake 5 (You can find it here: https://premake.github.io/) to generate the solution/project files needed to compile the project. Here's a step by step guide on how to generate the solution/project files for your favourite IDE:

1. Download the latest Premake 5 version from the link above.

2. Extract and place the Premake 5 executable in the root of Savvy (The Premake 5 executable should be in the same folder as the premake5.lua file)

3. Use Command Prompt (or Terminal) to navigate to the root Savvy folder on your computer.

4. Execute the command, without the quotes: "premake5 Your_IDE_Name" (For example: "premake5 vs2013" for Visual Studio 2013 IDE)

N.B: You can check if your IDE is supported by Premake 5 using the link above.

5. Open the Solution in your favourite IDE and press the compile button.

# Project Breakdown
After you've opened the solution, you should see three projects: Sample, Savvy and Savvy Standalone.

## Sample
The Sample is a project which shows different uses of of the Savvy library and demonstrates how to extend it.

## Savvy
This is the bread and butter of the tool. This project contains all the Savvy functionality and implementation of the converters.

## Savvy Standalone
This project is the standalone version of the tool. It is a simple console application which provides an out of the box functionality for anyone to convert shaders between GLSL and HLSL.
