// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

// 조합이 가능한 로그 타입들 (각 타입은 독립적)
enum class ELogType
{
	Trace = 0, // 출력창에 띄움
	Popup, // 메시지 박스로 띄움
	File, // 파일에 씀
	Screen, // 뷰어에 띄움
};

// 자주 사용되는 조합
enum class ELogVerbosity
{
	// 로그 타입 단독으로 사용할 때
	OnlyTrace = 0,
	OnlyPopup,
	OnlyFile,
	OnlyScreen,

	// 여기서부터는 조합
	Assert, // Trace + Popup (개발 단계에서만 보려는 용도, 따라서 파일에 쓸 필요는 없음)
	Notice, // Trace + File
	Error, // Trace + PopUp + File
	Display, // Trace + File + Screen
};

// 로그에 적용할 추가 정보 (하나도 없을 수도 있음)
enum class ELogAdditionalOption
{
	Time = 0, // 현재 시간
	FilePath, // 항상 절대 경로
	FileLine, // 소스 파일의 줄 번호 (FilePath가 활성화되어야 적용됨)
	Function, // 실행 중인 함수 이름
	Count,
};