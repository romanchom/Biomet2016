#include "GestureEngine.h"


#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

GestureEngine::GestureEngine() :
	cooldown(0.0),
	gesturseInRow(0),
	lastGesture(""),
	inRowThreshold(60)
{
}


GestureEngine::~GestureEngine()
{
}

void GestureEngine::processGesture(const std::string & gesture)
{
	if (lastGesture == gesture) {
		++gesturseInRow;
	} else {
		lastGesture = gesture;
	}

	if (gesturseInRow >= inRowThreshold) {
		executeGesture(gesture);
		lastGesture = "";
		gesturseInRow = 0;
	}
}

void GestureEngine::executeGesture(const std::string & gesture)
{
	INPUT key;
	key.type = INPUT_KEYBOARD;
	key.ki.dwExtraInfo = 0;
	key.ki.dwFlags = 0;
	key.ki.wScan = 0;
	if (gesture == "play") {
		key.ki.wVk = VK_MEDIA_PLAY_PAUSE;
	} else if(gesture == "next") {
		key.ki.wVk = VK_MEDIA_NEXT_TRACK;
	} else if (gesture == "prev") {
		key.ki.wVk = VK_MEDIA_PREV_TRACK;
	} else if (gesture == "stop") {
		key.ki.wVk = VK_MEDIA_STOP;
	} else {
		return;
	}
	SendInput(1, &key, sizeof(INPUT));
	key.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &key, sizeof(INPUT));
}
