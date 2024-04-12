/*
초기화와 정리만 있는 기본 인터페이스
이걸 상속받는 베이스 클래스는 빈 구현으로 둠
*/

#pragma once

class IGameDefault
{
public:
	virtual void Startup() = 0;
	virtual bool Update()  = 0;
	virtual bool PostUpdate() = 0;
	virtual void Render()  = 0;
	virtual void Cleanup() = 0;
};