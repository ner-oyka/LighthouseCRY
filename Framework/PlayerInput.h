#pragma once

#include <DefaultComponents/Input/InputComponent.h>
#include "External/ObserverManager/ObserverManager.h"

struct IInputEvents : public BaseObserverProtocol
{
	//Keyboard
	virtual void OnEnter(int activationMode, float value) {}
	virtual void OnLeft(int activationMode, float value) {}
	virtual void OnRight(int activationMode, float value) {}
	virtual void OnForward(int activationMode, float value) {}
	virtual void OnBackward(int activationMode, float value) {}
	virtual void OnSpace(int activationMode, float value) {}
	virtual void OnShift(int activationMode, float value) {}
	virtual void OnCtrlLeft(int activationMode, float value) {}
	virtual void OnTab(int activationMode, float value) {}
	virtual void OnF(int activationMode, float value) {}
	virtual void OnE(int activationMode, float value) {}
	virtual void OnR(int activationMode, float value) {}
	virtual void OnQ(int activationMode, float value) {}
	virtual void OnOne(int activationMode, float value) {}
	virtual void OnTwo(int activationMode, float value) {}
	virtual void OnThree(int activationMode, float value) {}
	virtual void OnFour(int activationMode, float value) {}
	virtual void OnFive(int activationMode, float value) {}
	virtual void OnI(int activationMode, float value) {}
	virtual void OnX(int activationMode, float value) {}

	//Mouse
	virtual void OnMouseX(int activationMode, float value) {}
	virtual void OnMouseY(int activationMode, float value) {}
	virtual void OnMouseButtonLeft(int activationMode, float value) {}
	virtual void OnMouseButtonRight(int activationMode, float value) {}
	virtual void OnMouseWheel(int activationMode, float value) {}

	//Xbox
	virtual void OnYawDeltaXILeft(int activationMode, float value) {}
	virtual void OnPitchDeltaXILeft(int activationMode, float value) {}
	virtual void OnThumbClickXILeft(int activationMode, float value) {}
	virtual void OnYawDeltaXIRight(int activationMode, float value) {}
	virtual void OnPitchDeltaXIRight(int activationMode, float value) {}
	virtual void OnThumbClickXIRight(int activationMode, float value) {}
	virtual void OnXI_A(int activationMode, float value) {}
	virtual void OnXI_B(int activationMode, float value) {}
	virtual void OnXI_X(int activationMode, float value) {}
	virtual void OnXI_Y(int activationMode, float value) {}
	virtual void OnTriggerXILeft(int activationMode, float value) {}
	virtual void OnTriggerXIRight(int activationMode, float value) {}
	virtual void OnShoulderXILeft(int activationMode, float value) {}
	virtual void OnShoulderXIRight(int activationMode, float value) {}
	virtual void OnD_PadXILeft(int activationMode, float value) {}
	virtual void OnD_PadXIRight(int activationMode, float value) {}
	virtual void OnD_PadXIUp(int activationMode, float value) {}
	virtual void OnD_PadXIDown(int activationMode, float value) {}
	virtual void OnBackXI(int activationMode, float value) {}
	virtual void OnStartXI(int activationMode, float value) {}

	//PS4
};

enum class EInputFlagType
{
	Hold = 0,
	Toggle
};

typedef uint8 TInputFlags;

enum class EInputFlag
	: TInputFlags
{
	None = 0,
	Left = 1 << 0,
	Right = 1 << 1,
	Forward = 1 << 2,
	Backward = 1 << 3
};

class CPlayerInput final
{
public:
	CPlayerInput() = default;
	~CPlayerInput() {};

	static CPlayerInput* GetInput()
	{
		static CPlayerInput instance;
		return &instance;
	}

	void RegisterActionMaps(IEntity* inputListener);

private:
	Cry::DefaultComponents::CInputComponent* m_pInputComponent = nullptr;
};