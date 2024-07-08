#include "Hooks.h"
#include <stdexcept>
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_win32.h"
#include "../imgui/imgui_impl_dx9.h"

using namespace GUIHooks;
using namespace Hooks;

void CGlobal_Hooks::Initialize()
{
	XASSERT(MH_Initialize() != MH_STATUS::MH_OK);

	if (MH_CreateHook(
		VirtualFunction(gui::device, 42),
		&EndScene,
		reinterpret_cast<void**>(&EndSceneOriginal)
	)) throw std::runtime_error("Unable to hook EndScene()");

	if (MH_CreateHook(
		VirtualFunction(gui::device, 16),
		&Reset,
		reinterpret_cast<void**>(&ResetOriginal)
	)) throw std::runtime_error("Unable to hook Reset()");

	ClientModeShared_CreateMove::Initialize();
	IEngineVGui_Paint::Initialize();
	IBaseClientDLL_LevelInitPostEntity::Initialize();
	IBaseClientDLL_LevelShutdown::Initialize();
	IBaseClientDLL_FrameStageNotify::Initialize();
	CL_CheckForPureServerWhitelist::Initialize();
	CTFPlayer_AvoidPlayers::Initialize();
	CPrediction_RunCommand::Initialize();
	ISurface_OnScreenSizeChanged::Initialize();

	XASSERT(MH_EnableHook(MH_ALL_HOOKS) != MH_STATUS::MH_OK);
	WINAPI_WndProc::Initialize();
	gui::DestroyDirectX();
}

void CGlobal_Hooks::UnInitialize()
{
	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);
	MH_Uninitialize();
	WINAPI_WndProc::UnInitialize();

	XASSERT(MH_Uninitialize() != MH_STATUS::MH_OK);
}

long __stdcall GUIHooks::EndScene(IDirect3DDevice9* device) noexcept
{
	const auto result = EndSceneOriginal(device, device);

	if (!gui::setup)
		gui::SetupMenu(device);

	if (gui::open)
		gui::Render();

	return result;
}


HRESULT __stdcall GUIHooks::Reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params) noexcept
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
	const auto result = ResetOriginal(device, device, params);
	ImGui_ImplDX9_CreateDeviceObjects();
	return result;
}
