#pragma once

#include <string>

class GestureEngine
{
private:
	double cooldown;
	int gesturseInRow;
	std::string lastGesture;
	int inRowThreshold;
public:
	GestureEngine();
	~GestureEngine();

	void processGesture(const std::string & gesture);
private:
	void executeGesture(const std::string & gesture);
};

