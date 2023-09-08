#include<windows.h>
#include"common.h"
#include"window.h"

static float Width;
static float Height;
static int CursorCounter = 0;

//���b�Z�[�W����
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wp, LPARAM lp)
{
    switch (uMsg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hWnd, uMsg, wp, lp);
    }
}
//���b�Z�[�W���o�i���B���֐��j
bool quit()
{
    MSG msg;
    while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
        DispatchMessage(&msg); //���̊֐����i�n�r�o�R�Łj WndProc()���Ăяo��
    }
    return msg.message == WM_QUIT;
}
//�E�B���h�E����郁�b�Z�[�W���o��
void closeWindow()
{
    HWND hWnd = FindWindow("GameWindow", NULL);
    PostMessage(hWnd, WM_CLOSE, 0, 0);
}

float getWidth()
{
    return Width;
}

float getHeight()
{
    return Height;
}
//�G�X�P�[�v�L�[��������Ă��邩
bool escKeyPressed()
{
    return GetAsyncKeyState(VK_ESCAPE) & 0x8000;
}
//�}�E�X�J�[�\�����B��
void hideCursor()
{
    if(CursorCounter>=0)
        CursorCounter = ShowCursor(FALSE);
}
//�}�E�X�J�[�\����\������
void showCursor()
{
    if (CursorCounter < 0)
        CursorCounter = ShowCursor(TRUE);
}
//�E�B���h�E������
void createWindow(const char* appName, int windowWidth, int windowHeight)
{
    //�E�B���h�E�̑傫��������Ă���
    Width = (float)windowWidth;
    Height = (float)windowHeight;

    //�u�E�B���h�E�N���X�\���́v�̒l��ݒ肵�āA�E�B���h�E�N���X��o�^����
    HINSTANCE hInstance = GetModuleHandle(NULL);
    HBRUSH hBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
    WNDCLASSA wc{};
    wc.style = CS_BYTEALIGNCLIENT;              //�������̒萔��g�ݍ��킹�ăN���X�X�^�C�����w��
    wc.lpfnWndProc = WndProc;                   //�R�[���o�b�N�֐��������|�C���^���w��
    wc.hInstance = hInstance;                   //�C���X�^���X�n���h�����w��
    wc.hIcon = LoadIcon(hInstance, "MYICON");   //�A�C�R���n���h�����w��
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);   //�J�[�\���n���h�����w��
    wc.hbrBackground = hBrush;                  //�w�i�p�̃u���V�n���h�����w��
    wc.lpszClassName = "GameWindow";            //�E�C���h�E�N���X�����w��
    ATOM rt = RegisterClass(&wc);//�����œo�^�I�I�I
    WARNING(rt == 0, "�E�B���h�E�N���X���o�^�ł��܂���ł���", "");

    //�E�B���h�E�X�^�C�������肷��
    unsigned windowStyle = WS_SYSMENU;//�g����
    if (windowWidth == 1920 && windowHeight == 1080) {
        windowStyle = WS_POPUP;//�g��
    }

    //�E�B���h�E�N���X�����ƂɃE�B���h�E�����
    HWND hWnd = CreateWindow(
        "GameWindow",               //��œo�^�����E�B���h�E�N���X�����w��
        appName,                    //�E�B���h�E�^�C�g����
        windowStyle,                //�E�B���h�E�X�^�C��
        0,                          //�E�B���h�E�\���ʒux
        0,                          //�E�B���h�E�\���ʒuy
        windowWidth,                //�E�B���h�E�̕��i�E�B���h�E�g�̕����܂ށj
        windowHeight,               //�E�B���h�E�̍����i�^�C�g���o�[�ƃE�B���h�E�g�̍������܂ށj
        NULL,                       //�e�E�B���h�E�܂��̓I�[�i�[�E�B���h�E�̃n���h��
        NULL,                       //���j���[�n���h���܂��͎q�E�B���h�EID
        hInstance,                  //�A�v���P�[�V�����C���X�^���X�̃n���h��
        NULL                        //�E�B���h�E�Ɋ֘A�t���郂�W���[���n���h��
    );
    WARNING(hWnd == 0, "�E�B���h�E������܂���ł���", "");

    //�E�B���h�E�̑傫���ƈʒu���Čv�Z����B
    RECT rect;
    GetClientRect(hWnd, &rect);    //���O�ō�����N���C�A���g�̈�̑傫�����擾
    windowWidth += (windowWidth - rect.right);   //�E�B���h�E�g�̕������Z
    windowHeight += (windowHeight - rect.bottom); //�^�C�g���o�[����уE�B���h�E�g�̍��������Z
    //�E�B���h�E�������ɕ\�������悤�ȕ\���ʒu���v�Z����B
    int windowPosX = (GetSystemMetrics(SM_CXSCREEN) - windowWidth) / 2;
    int windowPosY = (GetSystemMetrics(SM_CYSCREEN) - windowHeight) / 2;
    //�E�B���h�E�̈ʒu�Ƒ傫�����Đݒ�
    SetWindowPos(hWnd, 0, windowPosX, windowPosY, windowWidth, windowHeight, 0);

    //�E�B���h�E��\������
    ShowWindow(hWnd, SW_SHOW);
}
//lovelib.cpp��gmain()�I����ɌĂяo���B
void destroyWindow()
{
    showCursor();
}
