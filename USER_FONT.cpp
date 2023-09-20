#include<Windows.h>
#include"USER_FONT.h"
USER_FONT::USER_FONT(const char* filename)
	:Filename(filename)
{
	AddFontResourceEx(filename, FR_PRIVATE, 0);
}
USER_FONT::~USER_FONT() {
	RemoveFontResourceEx(Filename.c_str(), FR_PRIVATE, 0);
}
