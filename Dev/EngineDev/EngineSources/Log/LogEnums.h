// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

// ������ ������ �α� Ÿ�Ե� (�� Ÿ���� ������)
enum class ELogType
{
	Trace = 0, // ���â�� ���
	Popup, // �޽��� �ڽ��� ���
	File, // ���Ͽ� ��
	Screen, // �� ���
};

// ���� ���Ǵ� ����
enum class ELogVerbosity
{
	// �α� Ÿ�� �ܵ����� ����� ��
	OnlyTrace = 0,
	OnlyPopup,
	OnlyFile,
	OnlyScreen,

	// ���⼭���ʹ� ����
	Assert, // Trace + Popup (���� �ܰ迡���� ������ �뵵, ���� ���Ͽ� �� �ʿ�� ����)
	Notice, // Trace + File
	Error, // Trace + PopUp + File
	Display, // Trace + File + Screen
};

// �α׿� ������ �߰� ���� (�ϳ��� ���� ���� ����)
enum class ELogAdditionalOption
{
	Time = 0, // ���� �ð�
	FilePath, // �׻� ���� ���
	FileLine, // �ҽ� ������ �� ��ȣ (FilePath�� Ȱ��ȭ�Ǿ�� �����)
	Function, // ���� ���� �Լ� �̸�
	Count,
};