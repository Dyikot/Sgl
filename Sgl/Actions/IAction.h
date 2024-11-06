#pragma once

class IAction
{
public:
	virtual ~IAction() = default;
	
	virtual void Start() = 0;
	virtual void Pause() = 0;
	virtual void Stop() = 0;
	virtual void Handle() = 0;
};