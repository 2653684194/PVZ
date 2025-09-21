REM Description: This script adds a specific directory to the user's PATH environment variable and starts a program if it exists.
@echo off
:: 获取当前目录的绝对路径
for %%I in ("%cd%") do set CURRENT_DIR=%%~fI
:: 拼接 dll 文件夹路径
set DLL_PATH=%CURRENT_DIR%\dll

:: 检查路径是否已存在
echo %PATH% | find "%DLL_PATH%" >nul
if %errorlevel% equ 0 (
    echo Path already exists, no need to add again.
) else (
    :: 永久添加到用户 PATH
    setx PATH "%PATH%;%DLL_PATH%"
    echo Path permently added: %DLL_PATH%
)
@echo off
:: 获取脚本所在目录的绝对路径
set "SCRIPT_DIR=%~dp0"
:: 拼接bin目录路径
set "BIN_DIR=%SCRIPT_DIR%bin"

:: 检查PVZ.exe是否存在
if exist "%BIN_DIR%\PVZ.exe" (
    echo starting PVZ.exe...
    cd /d "%BIN_DIR%"
    start "" "PVZ.exe"
) else (
    echo 错误：未找到 %BIN_DIR%\PVZ.exe
)
@REM pause


