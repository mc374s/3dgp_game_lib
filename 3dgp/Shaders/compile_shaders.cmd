@echo off
rem Copyright (c) Microsoft Corporation. All rights reserved.
rem Licensed under the MIT License.

setlocal
set error=0

set FXCOPTS=/Zi /Od /nologo 
set FileDir=./3dgp/Shaders/
set OutDir=./Data/Shaders/

if %1.==. goto ContinuePC
echo usage: CompileShaders [pc]
exit /b

:ContinuePC
echo.
set PCFXC="%DXSDK_DIR%Utilities\bin\x86\fxc.exe"
echo Using %PCFXC%
if exist %PCFXC% goto Continue
set PCFXC="%DXSDK_DIR%Utilities\bin\x64\fxc.exe"
echo Using %PCFXC%
if exist %PCFXC% goto Continue
if not exist %PCFXC% goto needsdk

:Continue
@if not exist %OutDir% mkdir %OutDir%

call :CompileShader%1 common_vs vs diffuse
call :CompileShader%1 common_vs vs diffuse_tex
call :CompileShader%1 common_vs vs diffuse_normal
call :CompileShader%1 common_vs vs diffuse_tex_normal
call :CompileShader%1 common_vs vs diffuse_tex_normal_bone

call :CompileShader%1 common_ps ps diffuse
call :CompileShader%1 common_ps ps diffuse_tex

goto ErrorCheck


:ErrorCheck
echo.
if %error% == 0 (
    echo Shaders compiled ok
) else (
    echo There were shader compilation errors!
)
echo.
endlocal
exit /b

:CompileShader
set fxc=%PCFXC% "%FileDir%%1.hlsl" %FXCOPTS% /T%2_5_0 /E%3 /Fo"%OutDir%\%1_%3.cso"
rem echo.
rem echo %fxc%
%fxc% || set error=1
exit /b

:CompileShaderSM4
set fxc=%PCFXC% "%FileDir%%1.hlsl" %FXCOPTS% /T%2_4_0_level_9_3 /E%3 /Fo"%OutDir%\%1.cso"
rem echo.
rem echo %fxc%
%fxc% || set error=1
exit /b

:needsdk
echo ERROR: CompileShaders requires the Microsoft DirectX SDK
echo        (try checking the environment variable %DXSDK_DIR%)