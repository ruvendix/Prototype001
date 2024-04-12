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