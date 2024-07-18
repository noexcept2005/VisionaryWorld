//VisionaryWorld 专属头文件
//任何人不得盗用。
//Written by Rico
#pragma once

//from stdcjz.h
//键盘
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME)& 0x8000 ? 1:0))
#define KEY_DOWN_FOREMOST(hWnd,vk) (KEY_DOWN(vk) && GetForegroundWindow()==hWnd)	//最前面 
#		define KEY_DOWN_FOCUSED(hWnd,vk) KEY_DOWN_FOREMOST(hWnd,vk)	//带焦点 
#define K(vk) KEY_DOWN_FOREMOST(hwnd,vk)	//注意这个hwnd是全局变量
//鼠标
#define MOUSE_LEFT_DOWN KEY_DOWN(VK_LBUTTON)
#define MOUSE_LEFT_DOWN_FOCUSED(hWnd) KEY_DOWN_FOCUSED(hWnd,VK_LBUTTON)
#define MOUSE_RIGHT_DOWN KEY_DOWN(VK_RBUTTON)
#define MOUSE_RIGHT_DOWN_FOCUSED(hWnd) KEY_DOWN_FOCUSED(hWnd,VK_RBUTTON)
#define LEFT_CLICK MOUSE_LEFT_DOWN
#define LEFT_CLICK_FOCUSED(hWnd) MOUSE_LEFT_DOWN_FOCUSED(hWnd)
#define RIGHT_CLICK MOUSE_RIGHT_DOWN
#define RIGHT_CLICK_FOCUSED(hWnd) MOUSE_RIGHT_DOWN_FOCUSED(hWnd)
#define CLICK LEFT_CLICK  //默认了，可以#undef掉。 
#define KBHIT kbhit()
#define LEFT_CLICK_OR_KBHIT (LEFT_CLICK || KBHIT)
#define CLICK_OR_KBHIT (LEFT_CLICK || KBHIT)