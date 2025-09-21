REM Description: This script adds the current directory's 'dll' folder to the user's PATH environment variable permanently.
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
pause