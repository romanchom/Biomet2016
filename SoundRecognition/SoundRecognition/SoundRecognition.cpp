// SoundRecognition.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "AppController.h"

int main()
{
	AppController ac;
	ac.Initialize();
	ac.Run();
	ac.Shutdown();
    return 0;
}

