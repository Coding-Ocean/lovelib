#include<windows.h>
#include"common.h"
#include"window.h"

static float Width;
static float Height;
static int CursorCounter = 0;

//メッセージ処理
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
//メッセージ送出（を隠す関数）
bool quit()
{
    MSG msg;
    while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
        DispatchMessage(&msg); //この関数が（ＯＳ経由で） WndProc()を呼び出す
    }
    return msg.message == WM_QUIT;
}
//ウィンドウを閉じるメッセージを出す
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
//エスケープキーが押されているか
bool escKeyPressed()
{
    return GetAsyncKeyState(VK_ESCAPE) & 0x8000;
}
//マウスカーソルを隠す
void hideCursor()
{
    if(CursorCounter>=0)
        CursorCounter = ShowCursor(FALSE);
}
//マウスカーソルを表示する
void showCursor()
{
    if (CursorCounter < 0)
        CursorCounter = ShowCursor(TRUE);
}
//ウィンドウをつくる
void createWindow(const char* appName, int windowWidth, int windowHeight)
{
    //ウィンドウの大きさを取っておく
    Width = (float)windowWidth;
    Height = (float)windowHeight;

    //「ウィンドウクラス構造体」の値を設定して、ウィンドウクラスを登録する
    HINSTANCE hInstance = GetModuleHandle(NULL);
    HBRUSH hBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
    WNDCLASSA wc{};
    wc.style = CS_BYTEALIGNCLIENT;              //いくつかの定数を組み合わせてクラススタイルを指定
    wc.lpfnWndProc = WndProc;                   //コールバック関数を示すポインタを指定
    wc.hInstance = hInstance;                   //インスタンスハンドルを指定
    wc.hIcon = LoadIcon(hInstance, "MYICON");   //アイコンハンドルを指定
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);   //カーソルハンドルを指定
    wc.hbrBackground = hBrush;                  //背景用のブラシハンドルを指定
    wc.lpszClassName = "GameWindow";            //ウインドウクラス名を指定
    ATOM rt = RegisterClass(&wc);//ここで登録！！！
    WARNING(rt == 0, "ウィンドウクラスが登録できませんでした", "");

    //ウィンドウスタイルを決定する
    unsigned windowStyle = WS_SYSMENU;//枠あり
    if (windowWidth == 1920 && windowHeight == 1080) {
        windowStyle = WS_POPUP;//枠無
    }

    //ウィンドウクラスをもとにウィンドウを作る
    HWND hWnd = CreateWindow(
        "GameWindow",               //上で登録したウィンドウクラス名を指定
        appName,                    //ウィンドウタイトル名
        windowStyle,                //ウィンドウスタイル
        0,                          //ウィンドウ表示位置x
        0,                          //ウィンドウ表示位置y
        windowWidth,                //ウィンドウの幅（ウィンドウ枠の幅を含む）
        windowHeight,               //ウィンドウの高さ（タイトルバーとウィンドウ枠の高さを含む）
        NULL,                       //親ウィンドウまたはオーナーウィンドウのハンドル
        NULL,                       //メニューハンドルまたは子ウィンドウID
        hInstance,                  //アプリケーションインスタンスのハンドル
        NULL                        //ウィンドウに関連付けるモジュールハンドル
    );
    WARNING(hWnd == 0, "ウィンドウがつくれませんでした", "");

    //ウィンドウの大きさと位置を再計算する。
    RECT rect;
    GetClientRect(hWnd, &rect);    //直前で作ったクライアント領域の大きさを取得
    windowWidth += (windowWidth - rect.right);   //ウィンドウ枠の幅を加算
    windowHeight += (windowHeight - rect.bottom); //タイトルバーおよびウィンドウ枠の高さを加算
    //ウィンドウが中央に表示されるような表示位置を計算する。
    int windowPosX = (GetSystemMetrics(SM_CXSCREEN) - windowWidth) / 2;
    int windowPosY = (GetSystemMetrics(SM_CYSCREEN) - windowHeight) / 2;
    //ウィンドウの位置と大きさを再設定
    SetWindowPos(hWnd, 0, windowPosX, windowPosY, windowWidth, windowHeight, 0);

    //ウィンドウを表示する
    ShowWindow(hWnd, SW_SHOW);
}
//lovelib.cppでgmain()終了後に呼び出す。
void destroyWindow()
{
    showCursor();
}
