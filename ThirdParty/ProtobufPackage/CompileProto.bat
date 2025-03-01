@echo off

IF ERRORLEVEL 1 PAUSE

echo #####################################################################################################
echo �� ������ ���� ������ ��

REM ��ġ ������ ����� ��� �˾Ƴ��� (protoc.exe�� ���� ������ �־�� ��)
set PROTO_COMPILER_PATH=%~dp0
REM echo %PROTO_COMPILER_PATH%

REM ��ǥ ��θ� ���� ��η� ��ȯ (�ݺ����� �̿��ؼ� ��� ��θ� ���� ��η� ��ȯ��)
set ABSOULTE_PROTO_DIRECTORY_PATH=%PROTO_COMPILER_PATH%..\..\Dev\Common\ProtoBuffer\ProtoFiles\
for %%I in ("%ABSOULTE_PROTO_DIRECTORY_PATH%") do set "ABSOULTE_PROTO_DIRECTORY_PATH=%%~fI"
echo ������ ������ ���丮: %ABSOULTE_PROTO_DIRECTORY_PATH%
REM pause

REM proto ���� ���
echo -----------------------------------------------------------------------------------------------------
echo proto ���� ���
for /f "delims=" %%f in ('dir /b /a-d "%ABSOULTE_PROTO_DIRECTORY_PATH%\*.proto"') do (
    echo %%f
)
echo -----------------------------------------------------------------------------------------------------
echo.

REM proto ���� ������
echo -----------------------------------------------------------------------------------------------------
echo ������ ���� ������ ����
call %PROTO_COMPILER_PATH%protoc.exe --proto_path=%ABSOULTE_PROTO_DIRECTORY_PATH% --cpp_out=%ABSOULTE_PROTO_DIRECTORY_PATH% "%ABSOULTE_PROTO_DIRECTORY_PATH%\*.proto"

if %ERRORLEVEL% equ 0 (
    echo ������ ���� ������ �Ϸ�
) else (
    echo ������ ���� ������ ����
	pause
	exit
)

echo -----------------------------------------------------------------------------------------------------
echo #####################################################################################################

pause