// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

/*
���� �����ӿ����� ������ �ʾ����� ���� �����ӿ����� ���� ����
���� �����ӿ��� �����µ� ���� �����ӿ����� ���� ����� ��
�ֻ��� ��Ʈ�� 1�� �ǹǷ� 0x8000���� Ȯ�� ����
*/
#define IS_EXIST_LOCAL_PLAYER_INPUT(virtualKeyCode) ((::GetAsyncKeyState(virtualKeyCode) & 0x8000) != 0)