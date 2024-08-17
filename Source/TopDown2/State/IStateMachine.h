#pragma once

class IStateMachine {
public:
	virtual ~IStateMachine() {};
	virtual void Tick(double Delta);
};
