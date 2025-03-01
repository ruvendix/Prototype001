@echo off

IF ERRORLEVEL 1 PAUSE

echo #####################################################################################################
echo ※ 프로토 버퍼 컴파일 ※

REM 배치 파일이 저장된 경로 알아내기 (protoc.exe와 같은 폴더에 있어야 함)
set PROTO_COMPILER_PATH=%~dp0
REM echo %PROTO_COMPILER_PATH%

REM 목표 경로를 절대 경로로 변환 (반복문을 이용해서 상대 경로를 절대 경로로 변환함)
set ABSOULTE_PROTO_DIRECTORY_PATH=%PROTO_COMPILER_PATH%..\..\Dev\Common\ProtoBuffer\ProtoFiles\
for %%I in ("%ABSOULTE_PROTO_DIRECTORY_PATH%") do set "ABSOULTE_PROTO_DIRECTORY_PATH=%%~fI"
echo 프로토 파일의 디렉토리: %ABSOULTE_PROTO_DIRECTORY_PATH%
REM pause

REM proto 파일 목록
echo -----------------------------------------------------------------------------------------------------
echo proto 파일 목록
for /f "delims=" %%f in ('dir /b /a-d "%ABSOULTE_PROTO_DIRECTORY_PATH%\*.proto"') do (
    echo %%f
)
echo -----------------------------------------------------------------------------------------------------
echo.

REM proto 파일 컴파일
echo -----------------------------------------------------------------------------------------------------
echo 프로토 파일 컴파일 시작
call %PROTO_COMPILER_PATH%protoc.exe --proto_path=%ABSOULTE_PROTO_DIRECTORY_PATH% --cpp_out=%ABSOULTE_PROTO_DIRECTORY_PATH% "%ABSOULTE_PROTO_DIRECTORY_PATH%\*.proto"

if %ERRORLEVEL% equ 0 (
    echo 프로토 파일 컴파일 완료
) else (
    echo 프로토 파일 컴파일 실패
	pause
	exit
)

echo -----------------------------------------------------------------------------------------------------
echo #####################################################################################################

pause