#pragma once

#include <SDL_events.h>
#include <SDL_scancode.h>

#include <array>
#include <vector>
#include <map>

#include "Utils/Base.h"

class CWorld;

enum class EAction : uint16
{
	A_MOVE_FORWARD,
	A_MOVE_BACKWARD,
	A_MOVE_LEFT,
	A_MOVE_RIGHT,
	NUM_ACTIONS
};

enum class EKeyState : int8
{
	KEY_STATE_NONE = -1,
	KEY_STATE_PRESSED,
	KEY_STATE_RELEASED
};

enum class EMouseButton : int8
{
	MOUSE_BUTTON_LEFT,
	MOUSE_BUTTON_RIGHT,
	MOUSE_BUTTON_MIDDLE,
	MOUSE_BUTTON_X1,
	MOUSE_BUTTON_X2,
	NUM_MOUSE_BUTTONS
};

constexpr uint8 HISTORY_BUFFER_SIZE = 2;

class CPlayerInput
{
public:
	CPlayerInput(CWorld* World);
	~CPlayerInput();

	void Update(const std::vector<SDL_Event>& InputEvents);

	const CVector2f& GetMouseWorldPosition() const;
	const CVector2i& GetMousePosition() const;

	bool IsPressed(const EAction Action) const;
	bool IsPressed(const EMouseButton Action) const;
	bool IsHeld(const EAction Action) const;
	bool IsHeld(const EMouseButton Action) const;
	bool IsReleased(const EAction Action) const;
	bool IsReleased(const EMouseButton Action) const;

private:

	typedef std::array<EKeyState, HISTORY_BUFFER_SIZE> SInputs;

	void HandleKeyEvent(const SDL_Event& InputEvent);
	void HandleMouseButton(const SDL_Event& InputEvent);
	void HandleMouseMotion(const SDL_Event& InputEvent);

	bool IsHeld(const SInputs& KeyInputs) const;
	bool IsReleased(const SInputs& KeyInputs) const;
	bool IsPressed(const SInputs& KeyInputs) const;

	std::array<SInputs, static_cast<size_t>(EAction::NUM_ACTIONS)> ActionKeyMap;
	std::array<SInputs, static_cast<size_t>(EMouseButton::NUM_MOUSE_BUTTONS)> MouseKeyMap;

	CVector2i MousePosition[HISTORY_BUFFER_SIZE];
	CVector2f CameraRelativeMousePosition[HISTORY_BUFFER_SIZE];

	std::map<SDL_Keycode, EAction> SDLKeyboardToActionMap;
	std::map<uint8, EMouseButton> SDLMouseToMouseButtonEnum;

	uint32 CurrentFrameBufferIndex = 0;

	CWorld* World;
};

