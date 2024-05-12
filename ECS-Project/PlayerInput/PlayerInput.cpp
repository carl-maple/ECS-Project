#include "PlayerInput.h"
#include "World/World.h"
#include "System/SystemManager.h"
#include "Graphics/GraphicsSystem.h"
#include "SDL_mouse.h"

CPlayerInput::CPlayerInput(CWorld* World)
:World(World)
{
	SDLKeyboardToActionMap[SDLK_w] = EAction::A_MOVE_FORWARD;
	SDLKeyboardToActionMap[SDLK_s] = EAction::A_MOVE_BACKWARD;
	SDLKeyboardToActionMap[SDLK_a] = EAction::A_MOVE_LEFT;
	SDLKeyboardToActionMap[SDLK_d] = EAction::A_MOVE_RIGHT;

	SDLMouseToMouseButtonEnum[SDL_BUTTON_LEFT] = EMouseButton::MOUSE_BUTTON_LEFT;
	SDLMouseToMouseButtonEnum[SDL_BUTTON_MIDDLE] = EMouseButton::MOUSE_BUTTON_MIDDLE;
	SDLMouseToMouseButtonEnum[SDL_BUTTON_RIGHT] = EMouseButton::MOUSE_BUTTON_RIGHT;
	SDLMouseToMouseButtonEnum[SDL_BUTTON_X1] = EMouseButton::MOUSE_BUTTON_X1;
	SDLMouseToMouseButtonEnum[SDL_BUTTON_X2] = EMouseButton::MOUSE_BUTTON_X2;


	for (SInputs& KeyInputs : ActionKeyMap)
	{
		for (EKeyState& KeyState : KeyInputs)
		{
			KeyState = EKeyState::KEY_STATE_RELEASED;
		}
	}

	for (SInputs& mouse_inputs : MouseKeyMap)
	{
		for (EKeyState& key_state : mouse_inputs)
		{
			key_state = EKeyState::KEY_STATE_RELEASED;
		}
	}
}


CPlayerInput::~CPlayerInput()
{
}

void CPlayerInput::Update(const std::vector<SDL_Event>& InputEvents)
{
	CurrentFrameBufferIndex = !CurrentFrameBufferIndex;
	
	for (SInputs& KeyInputs : ActionKeyMap)
	{
		KeyInputs[CurrentFrameBufferIndex] = KeyInputs[!CurrentFrameBufferIndex];
	}

	for (SInputs& mouse_inputs : MouseKeyMap)
	{
		mouse_inputs[CurrentFrameBufferIndex] = mouse_inputs[!CurrentFrameBufferIndex];
	}

	MousePosition[CurrentFrameBufferIndex] = MousePosition[!CurrentFrameBufferIndex];

	for (const SDL_Event& InputEvent : InputEvents)
	{
		switch (InputEvent.type)
		{
		case SDL_KEYDOWN:
		case SDL_KEYUP:
		{
			HandleKeyEvent(InputEvent);
		}
		case SDL_MOUSEBUTTONUP:
		case SDL_MOUSEBUTTONDOWN:
		{
			HandleMouseButton(InputEvent);

		}
		case SDL_MOUSEMOTION:
		{
			HandleMouseMotion(InputEvent);

		}
		default:
			break;
		}
		
	}
}

const CVector2f& CPlayerInput::GetMouseWorldPosition() const
{
	return CameraRelativeMousePosition[CurrentFrameBufferIndex];
	
}

const CVector2i& CPlayerInput::GetMousePosition() const
{
	return MousePosition[CurrentFrameBufferIndex];
}

void CPlayerInput::HandleMouseMotion(const SDL_Event &InputEvent)
{
	CVector2i WindowSize;
	World->GetSystemManager()->GetSystem<CGraphicsSystem>()->GetRendererOutputSize(WindowSize);
	MousePosition[CurrentFrameBufferIndex] = CVector2i(InputEvent.motion.x, InputEvent.motion.y - WindowSize.y);

	const CVector2f camera_adjustment_values = World->GetSystemManager()->GetSystem<CGraphicsSystem>()->GetCameraAdjustmentValues();
	CameraRelativeMousePosition[CurrentFrameBufferIndex] = camera_adjustment_values + MousePosition[CurrentFrameBufferIndex];
}

void CPlayerInput::HandleMouseButton(const SDL_Event &InputEvent)
{
	const size_t Button = static_cast<size_t>(SDLMouseToMouseButtonEnum[InputEvent.button.button]);
	MouseKeyMap[Button][CurrentFrameBufferIndex] = 
		InputEvent.button.state == SDL_PRESSED ? EKeyState::KEY_STATE_PRESSED : EKeyState::KEY_STATE_RELEASED;
}

void CPlayerInput::HandleKeyEvent(const SDL_Event &InputEvent)
{
	const SDL_Keycode KeyCode = InputEvent.key.keysym.sym;
	const std::map<SDL_Keycode, EAction>::const_iterator it = SDLKeyboardToActionMap.find(KeyCode);
	if (it != std::end(SDLKeyboardToActionMap))
	{
		const uint32 Action = static_cast<size_t>(it->second);
		ActionKeyMap[Action][CurrentFrameBufferIndex] = 
			InputEvent.key.state == SDL_PRESSED ? EKeyState::KEY_STATE_PRESSED : EKeyState::KEY_STATE_RELEASED;
	}
}

bool CPlayerInput::IsPressed(const EAction Action) const
{
	const SInputs& KeyInputs = ActionKeyMap[static_cast<uint32>(Action)];
	return IsPressed(KeyInputs);
}

bool CPlayerInput::IsHeld(const EAction Action) const 
{
	const SInputs& KeyInputs = ActionKeyMap[static_cast<uint32>(Action)];
	return IsHeld(KeyInputs);
}

bool CPlayerInput::IsReleased(const EAction Action) const
{
	const SInputs& KeyInputs = ActionKeyMap[static_cast<uint32>(Action)];
	return IsReleased(KeyInputs);
}

bool CPlayerInput::IsReleased(const SInputs& KeyInputs) const
{
	if (KeyInputs[CurrentFrameBufferIndex] == EKeyState::KEY_STATE_RELEASED && 
		KeyInputs[!CurrentFrameBufferIndex] == EKeyState::KEY_STATE_PRESSED)
	{
		return true;
	}

	return false;
}

bool CPlayerInput::IsHeld(const SInputs& KeyInputs) const
{
	if (KeyInputs[CurrentFrameBufferIndex] == EKeyState::KEY_STATE_PRESSED && 
		KeyInputs[!CurrentFrameBufferIndex] == EKeyState::KEY_STATE_PRESSED)
	{
		return true;
	}

	return false;
}

bool CPlayerInput::IsPressed(const SInputs& KeyInputs) const
{
	if (KeyInputs[CurrentFrameBufferIndex] == EKeyState::KEY_STATE_PRESSED && 
		KeyInputs[!CurrentFrameBufferIndex] == EKeyState::KEY_STATE_RELEASED)
	{
		return true;
	}

	return false;
}

bool CPlayerInput::IsPressed(const EMouseButton Action) const
{
	const SInputs& KeyInputs = MouseKeyMap[static_cast<uint32>(Action)];
	return IsPressed(KeyInputs);
}

bool CPlayerInput::IsHeld(const EMouseButton Action) const
{
	const SInputs& KeyInputs = MouseKeyMap[static_cast<uint32>(Action)];
	return IsHeld(KeyInputs);
}

bool CPlayerInput::IsReleased(const EMouseButton Action) const
{
	const SInputs& KeyInputs = MouseKeyMap[static_cast<uint32>(Action)];
	return IsReleased(KeyInputs);
}