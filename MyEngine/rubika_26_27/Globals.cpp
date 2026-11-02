#include "Globals.h"

#include <DebugMgr.h>
#include <Logger.h>
#include <SceneMgr.h>
#include <TextureMgr.h>
#include <InputMgr.h>

Globals gData;

Globals::Globals() : FrameCount(0)
{
	DebugMgr = new ::DebugMgr();
	Logger = new ::Logger();
	SceneMgr = new ::SceneMgr();
	TextureMgr = new ::TextureMgr();
	InputMgr = new ::InputMgr();
}

Globals::~Globals()
{
	Destroy();
}

void Globals::Init()
{
	Logger->Init();
	SceneMgr->Init();
	TextureMgr->Init();
	InputMgr->Init();
}

void Globals::Shut()
{
	Logger->Shut();
	SceneMgr->Shut();
	TextureMgr->Shut();
	InputMgr->Shut();
}

void Globals::Destroy()
{
	delete DebugMgr;
	DebugMgr = nullptr;

	delete Logger;
	Logger = nullptr;

	delete SceneMgr;
	SceneMgr = nullptr;

	delete TextureMgr;
	TextureMgr = nullptr;

	delete InputMgr;
	InputMgr = nullptr;
}

void Globals::IncrementFrameCount()
{
	++FrameCount;
}
