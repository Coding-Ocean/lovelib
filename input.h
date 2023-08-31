#pragma once
enum INPUT_CODE {
	KEY_A,KEY_D,KEY_W,KEY_S,
	NUM_CODE
};
void getInputState();
int isPress(INPUT_CODE i);
int isTrigger(INPUT_CODE i);
int isRelease(INPUT_CODE i);

