#include "Globals.h"

#include <DebugMgr.h>
#include <Logger.h>
#include <SceneMgr.h>

Globals gData;

Globals::Globals() : FrameCount(0)
{
	DebugMgr = new ::DebugMgr();
	Logger = new ::Logger();
	SceneMgr = new ::SceneMgr();
}

Globals::~Globals()
{
	Destroy();
}

void Globals::Init()
{
	Logger->Init();
	SceneMgr->Init();
}

void Globals::Shut()
{
	Logger->Shut();
	SceneMgr->Shut();
}

void Globals::Destroy()
{
	delete DebugMgr;
	DebugMgr = nullptr;

	delete Logger;
	Logger = nullptr;
}

void Globals::IncrementFrameCount()
{
	++FrameCount;
}
