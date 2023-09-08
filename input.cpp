#include <windows.h>
#include "input.h"

//���O�ƌ��݂́u�L�[��{�^���̏�ԁv���L������B
static short InputState[2][NUM_CODE]{};
//��̂Q�����z����w���Y�����i�O�܂��͂P�̂ǂ��炩�j
static short Now{};
//�E�B���h�E�n���h���BScreenToClient�AClientToScreen�ɕK�v
static HWND HWnd{};
//�}�E�X�J�[�\���\���E��\���ɂ���
static int CursorCounter{};

//���݂̓��͏�Ԃ𒲂ׂ�
void getInputState()
{
	//���O�ƌ��݂��w���Y������؂�ւ���
	Now = 1 - Now;
	//���͏�Ԃ��L�����Ă���
	InputState[Now][KEY_A] = GetAsyncKeyState('A') < 0;
	InputState[Now][KEY_D] = GetAsyncKeyState('D') < 0;
	InputState[Now][KEY_W] = GetAsyncKeyState('W') < 0;
	InputState[Now][KEY_S] = GetAsyncKeyState('S') < 0;
	InputState[Now][KEY_ESC] = GetAsyncKeyState(VK_ESCAPE) < 0;
	InputState[Now][MOUSE_LBUTTON] = GetAsyncKeyState(VK_LBUTTON) < 0;
	InputState[Now][MOUSE_RBUTTON] = GetAsyncKeyState(VK_RBUTTON) < 0;
	//�}�E�X���W���L�����Ă���
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(HWnd, &pt);
	InputState[Now][MOUSE_X] = (short)pt.x;
	InputState[Now][MOUSE_Y] = (short)pt.y;
}
//�w�肵���L�[��{�^����������Ă��邩
int isPress(INPUT_CODE i)
{
	return InputState[Now][i];
}
//�w�肵���L�[��{�^���������ꂽ�u�Ԃ�
int isTrigger(INPUT_CODE i)
{
	if (InputState[!Now][i] == 0 &&	InputState[Now][i] == 1)
		return 1;
	return 0;
}
//�w�肵���L�[��{�^���������ꂽ�u�Ԃ�
int isRelease(INPUT_CODE i)
{
	if (InputState[!Now][i] == 1 &&	InputState[Now][i] == 0)
		return 1;
	return 0;
}
//�}�E�X�J�[�\�����B��
void hideCursor()
{
	if (CursorCounter >= 0)
		CursorCounter = ShowCursor(FALSE);
}
//�}�E�X�J�[�\����\������
void showCursor()
{
	if (CursorCounter < 0)
		CursorCounter = ShowCursor(TRUE);
}
//�}�E�X���WX��Ԃ�
float getMouseX()
{
	return (float)InputState[Now][MOUSE_X];
}
//�}�E�X���WY��Ԃ�
float getMouseY()
{
	return (float)InputState[Now][MOUSE_Y];
}
//�}�E�X�ړ��x�N�g����Ԃ�
float getMouseVx()
{
	return float( -InputState[!Now][MOUSE_X] + InputState[Now][MOUSE_X]);
}
//�}�E�X�ړ��x�N�g����Ԃ�
float getMouseVy()
{
	return float(InputState[!Now][MOUSE_Y] - InputState[Now][MOUSE_Y]);
}
//�}�E�X�ʒu���Z�b�g
void setMousePos(float x, float y)
{
	InputState[Now][MOUSE_X] = (short)x;
	InputState[Now][MOUSE_Y] = (short)y;

	POINT pt{ (short)x, (short)y };
	ClientToScreen(HWnd, &pt);
	SetCursorPos(pt.x, pt.y);
}

//�C���v�b�g������
void createInput()
{
	HWnd = FindWindow("GameWindow", NULL);
	//�E�B���h�E�����Ƀ}�E�X���Z�b�g
	RECT rect;
	GetClientRect(HWnd, &rect);
	setMousePos(rect.right / 2.0f, rect.bottom / 2.0f);
}
void destroyInput()
{
	showCursor();
}

