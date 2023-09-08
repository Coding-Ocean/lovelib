#include <windows.h>
#include "input.h"

//���O�ƌ��݂́u�L�[��{�^���̏�ԁv���L������B
static short InputState[2][NUM_CODE]{};
//��̂Q�����z����w���Y�����i�O�܂��͂P�̂ǂ��炩�j
static short Now{};

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