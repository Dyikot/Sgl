#pragma once

class IProcessed
{
public:
	virtual ~IProcessed() = 0;

	virtual void Process(float elapsedMs) = 0;
};