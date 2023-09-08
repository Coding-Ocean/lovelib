#include <windows.h>
#include "input.h"

//直前と現在の「キーやボタンの状態」を記憶する。
static short InputState[2][NUM_CODE]{};
//上の２次元配列を指す添え字（０または１のどちらか）
static short Now{};

//現在の入力状態を調べる
void getInputState()
{
	//直前と現在を指す添え字を切り替える
	Now = 1 - Now;
	//入力状態を記憶しておく
	InputState[Now][KEY_A] = GetAsyncKeyState('A') < 0;
	InputState[Now][KEY_D] = GetAsyncKeyState('D') < 0;
	InputState[Now][KEY_W] = GetAsyncKeyState('W') < 0;
	InputState[Now][KEY_S] = GetAsyncKeyState('S') < 0;
}
//指定したキーやボタンが押されているか
int isPress(INPUT_CODE i)
{
	return InputState[Now][i];
}
//指定したキーやボタンが押された瞬間か
int isTrigger(INPUT_CODE i)
{
	if (InputState[!Now][i] == 0 &&	InputState[Now][i] == 1)
		return 1;
	return 0;
}
//指定したキーやボタンが離された瞬間か
int isRelease(INPUT_CODE i)
{
	if (InputState[!Now][i] == 1 &&	InputState[Now][i] == 0)
		return 1;
	return 0;
}