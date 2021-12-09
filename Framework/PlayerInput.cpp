#include "StdAfx.h"
#include "PlayerInput.h"

void CPlayerInput::RegisterActionMaps(IEntity* inputListener)
{
	m_pInputComponent = inputListener->GetOrCreateComponent<Cry::DefaultComponents::CInputComponent>();

	m_pInputComponent->RegisterAction("input", "enter", [this](int activationMode, float value) {ObserverManager::notify(&IInputEvents::OnEnter, activationMode, value); });
	m_pInputComponent->BindAction("input", "enter", eAID_KeyboardMouse, EKeyId::eKI_Enter);

	m_pInputComponent->RegisterAction("input", "left", [this](int activationMode, float value) {ObserverManager::notify(&IInputEvents::OnLeft, activationMode, value); });
	m_pInputComponent->BindAction("input", "left", eAID_KeyboardMouse, EKeyId::eKI_A);
	m_pInputComponent->BindAction("input", "left", eAID_KeyboardMouse, EKeyId::eKI_Left);

	m_pInputComponent->RegisterAction("input", "right", [this](int activationMode, float value) {ObserverManager::notify(&IInputEvents::OnRight, activationMode, value); });
	m_pInputComponent->BindAction("input", "right", eAID_KeyboardMouse, EKeyId::eKI_D);
	m_pInputComponent->BindAction("input", "right", eAID_KeyboardMouse, EKeyId::eKI_Right);

	m_pInputComponent->RegisterAction("input", "xi_thumblx", [this](int activationMode, float value) {ObserverManager::notify(&IInputEvents::OnYawDeltaXILeft, activationMode, value); });
	m_pInputComponent->BindAction("input", "xi_thumblx", eAID_XboxPad, EKeyId::eKI_XI_ThumbLX);

	m_pInputComponent->RegisterAction("input", "xi_thumbly", [this](int activationMode, float value) {ObserverManager::notify(&IInputEvents::OnPitchDeltaXILeft, activationMode, value); });
	m_pInputComponent->BindAction("input", "xi_thumbly", eAID_XboxPad, EKeyId::eKI_XI_ThumbLY);

	m_pInputComponent->RegisterAction("input", "xi_thumbrx", [this](int activationMode, float value) {ObserverManager::notify(&IInputEvents::OnYawDeltaXIRight, activationMode, value); });
	m_pInputComponent->BindAction("input", "xi_thumbrx", eAID_XboxPad, EKeyId::eKI_XI_ThumbRX);

	m_pInputComponent->RegisterAction("input", "xi_thumbry", [this](int activationMode, float value) {ObserverManager::notify(&IInputEvents::OnPitchDeltaXIRight, activationMode, value); });
	m_pInputComponent->BindAction("input", "xi_thumbry", eAID_XboxPad, EKeyId::eKI_XI_ThumbRY);

	m_pInputComponent->RegisterAction("input", "forward", [this](int activationMode, float value) {ObserverManager::notify(&IInputEvents::OnForward, activationMode, value); });
	m_pInputComponent->BindAction("input", "forward", eAID_KeyboardMouse, EKeyId::eKI_W);
	m_pInputComponent->BindAction("input", "forward", eAID_KeyboardMouse, EKeyId::eKI_Up);

	m_pInputComponent->RegisterAction("input", "backward", [this](int activationMode, float value) {ObserverManager::notify(&IInputEvents::OnBackward, activationMode, value); });
	m_pInputComponent->BindAction("input", "backward", eAID_KeyboardMouse, EKeyId::eKI_S);
	m_pInputComponent->BindAction("input", "backward", eAID_KeyboardMouse, EKeyId::eKI_Down);

	m_pInputComponent->RegisterAction("input", "xi_trigger_r", [this](int activationMode, float value) {ObserverManager::notify(&IInputEvents::OnTriggerXIRight, activationMode, value); });
	m_pInputComponent->BindAction("input", "xi_trigger_r", eAID_XboxPad, EKeyId::eKI_XI_TriggerR);

	m_pInputComponent->RegisterAction("input", "xi_b", [this](int activationMode, float value) {ObserverManager::notify(&IInputEvents::OnXI_B, activationMode, value); });
	m_pInputComponent->BindAction("input", "xi_b", eAID_XboxPad, EKeyId::eKI_XI_B);

	m_pInputComponent->RegisterAction("input", "mouse_x", [this](int activationMode, float value) { ObserverManager::notify(&IInputEvents::OnMouseX, activationMode, value); });
	m_pInputComponent->BindAction("input", "mouse_x", eAID_KeyboardMouse, EKeyId::eKI_MouseX);

	m_pInputComponent->RegisterAction("input", "mouse_y", [this](int activationMode, float value) { ObserverManager::notify(&IInputEvents::OnMouseY, activationMode, value); });
	m_pInputComponent->BindAction("input", "mouse_y", eAID_KeyboardMouse, EKeyId::eKI_MouseY);

	m_pInputComponent->RegisterAction("input", "x", [this](int activationMode, float value) {ObserverManager::notify(&IInputEvents::OnX, activationMode, value); });
	m_pInputComponent->BindAction("input", "x", eAID_KeyboardMouse, EKeyId::eKI_X);

	m_pInputComponent->RegisterAction("input", "shift", [this](int activationMode, float value) {ObserverManager::notify(&IInputEvents::OnShift, activationMode, value); });
	m_pInputComponent->BindAction("input", "shift", eAID_KeyboardMouse, EKeyId::eKI_LShift);

	m_pInputComponent->RegisterAction("input", "mouse_wheel", [this](int activationMode, float value) {ObserverManager::notify(&IInputEvents::OnMouseWheel, activationMode, value); });
	m_pInputComponent->BindAction("input", "mouse_wheel", eAID_KeyboardMouse, EKeyId::eKI_MouseWheelUp);
	m_pInputComponent->BindAction("input", "mouse_wheel", eAID_KeyboardMouse, EKeyId::eKI_MouseWheelDown);

	m_pInputComponent->RegisterAction("input", "mouse_1", [this](int activationMode, float value) {ObserverManager::notify(&IInputEvents::OnMouseButtonLeft, activationMode, value); });
	m_pInputComponent->BindAction("input", "mouse_1", eAID_KeyboardMouse, EKeyId::eKI_Mouse1);

	m_pInputComponent->RegisterAction("input", "mouse_2", [this](int activationMode, float value) {ObserverManager::notify(&IInputEvents::OnMouseButtonRight, activationMode, value); });
	m_pInputComponent->BindAction("input", "mouse_2", eAID_KeyboardMouse, EKeyId::eKI_Mouse2);

	m_pInputComponent->RegisterAction("input", "left_ctrl", [this](int activationMode, float value) {ObserverManager::notify(&IInputEvents::OnCtrlLeft, activationMode, value); });
	m_pInputComponent->BindAction("input", "left_ctrl", eAID_KeyboardMouse, EKeyId::eKI_LCtrl);
}
