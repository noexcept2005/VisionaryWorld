//VisionaryWorld ר��ͷ�ļ�
//�κ��˲��õ��á�
//Written by Rico
#pragma once

//from stdcjz.h
//����
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME)& 0x8000 ? 1:0))
#define KEY_DOWN_FOREMOST(hWnd,vk) (KEY_DOWN(vk) && GetForegroundWindow()==hWnd)	//��ǰ�� 
#		define KEY_DOWN_FOCUSED(hWnd,vk) KEY_DOWN_FOREMOST(hWnd,vk)	//������ 
#define K(vk) KEY_DOWN_FOREMOST(hwnd,vk)	//ע�����hwnd��ȫ�ֱ���
//���
#define MOUSE_LEFT_DOWN KEY_DOWN(VK_LBUTTON)
#define MOUSE_LEFT_DOWN_FOCUSED(hWnd) KEY_DOWN_FOCUSED(hWnd,VK_LBUTTON)
#define MOUSE_RIGHT_DOWN KEY_DOWN(VK_RBUTTON)
#define MOUSE_RIGHT_DOWN_FOCUSED(hWnd) KEY_DOWN_FOCUSED(hWnd,VK_RBUTTON)
#define LEFT_CLICK MOUSE_LEFT_DOWN
#define LEFT_CLICK_FOCUSED(hWnd) MOUSE_LEFT_DOWN_FOCUSED(hWnd)
#define RIGHT_CLICK MOUSE_RIGHT_DOWN
#define RIGHT_CLICK_FOCUSED(hWnd) MOUSE_RIGHT_DOWN_FOCUSED(hWnd)
#define CLICK LEFT_CLICK  //Ĭ���ˣ�����#undef���� 
#define KBHIT kbhit()
#define LEFT_CLICK_OR_KBHIT (LEFT_CLICK || KBHIT)
#define CLICK_OR_KBHIT (LEFT_CLICK || KBHIT)