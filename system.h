//VisionaryWorld ר��ͷ�ļ�
//�κ��˲��õ��á�
//Written by Rico
#pragma once
#define HWND_CONSOLE GetConsoleWindow()
INT CJZAPI GetScreenHeight(void) //��ȡ��Ļ�߶� 
{
	return GetSystemMetrics(SM_CYSCREEN);
}
INT CJZAPI GetScreenWidth(void) //��ȡ��Ļ��� 
{
	return GetSystemMetrics(SM_CXSCREEN);
}
RECT CJZAPI GetWindowRect(HWND hwnd=HWND_CONSOLE){
	RECT rt;
	GetWindowRect(hwnd,&rt);
	return rt;
}
RECT CJZAPI GetClientRect(HWND hwnd=HWND_CONSOLE){
	RECT rt;
	GetClientRect(hwnd,&rt);
	return rt;
}
LONG CJZAPI GetWindowWidth(HWND hwnd=HWND_CONSOLE){
	RECT rect=GetWindowRect(hwnd);
	return (rect.right - rect.left);
}
LONG CJZAPI GetWindowHeight(HWND hwnd=HWND_CONSOLE){
	RECT rect=GetWindowRect(hwnd);
	return (rect.bottom - rect.top);
}
LONG CJZAPI GetClientWidth(HWND hwnd=HWND_CONSOLE){
	RECT rect=GetClientRect(hwnd);
	return (rect.right - rect.left);
}
LONG CJZAPI GetClientHeight(HWND hwnd=HWND_CONSOLE){
	RECT rect=GetClientRect(hwnd);
	return (rect.bottom - rect.top);
}
LONG CJZAPI GetWindowLeft(HWND hwnd=HWND_CONSOLE){
	RECT rect=GetWindowRect(hwnd);
	return rect.left;
}
LONG CJZAPI GetWindowRight(HWND hwnd=HWND_CONSOLE){
	RECT rect=GetWindowRect(hwnd);
	return rect.right;
}
LONG CJZAPI GetWindowTop(HWND hwnd=HWND_CONSOLE){
	RECT rect=GetWindowRect(hwnd);
	return rect.top;
}
LONG CJZAPI GetWindowBottom(HWND hwnd=HWND_CONSOLE){
	RECT rect=GetWindowRect(hwnd);
	return rect.bottom;
}
LONG CJZAPI GetClientLeft(HWND hwnd = HWND_CONSOLE) {
	RECT rect = GetClientRect(hwnd);
	return rect.left;
}
LONG CJZAPI GetClientRight(HWND hwnd = HWND_CONSOLE) {
	RECT rect = GetClientRect(hwnd);
	return rect.right;
}
LONG CJZAPI GetClientTop(HWND hwnd = HWND_CONSOLE) {
	RECT rect = GetClientRect(hwnd);
	return rect.top;
}
LONG CJZAPI GetClientBottom(HWND hwnd = HWND_CONSOLE) {
	RECT rect = GetClientRect(hwnd);
	return rect.bottom;
}
RECT CJZAPI GetSystemWorkAreaRect(void) //��ȡ���������� 
{
	RECT rt;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rt, 0);    // ��ù�������С
	return rt;
}
int CJZAPI GetFullUIX(void)
{
	RECT rt = GetSystemWorkAreaRect();
	return (rt.right - rt.left);
}
LONG CJZAPI GetTitleHeight()
{
	return 30 / g_scale;
	//return GetSystemMetrics(SM_CYCAPTION);
}
float CJZAPI GetScreenScale()
{
	HDC hdtdc = GetDC(0);
	int ret = GetDeviceCaps(hdtdc, LOGPIXELSY);
	if (ret == 96)
		return 1.0f;
	else if (ret == 120)
		return 1.25f;
	else if (ret == 144)
		return 1.50f;
	else if (ret == 192)
		return 2.0f;
	else
	{
		DebugLog("����δ�������ĻDPI=" + ToString(ret));
		return 1.0f;
	}
}
LONG CJZAPI GetTaskbarHeight()
{
	HWND htbwnd = FindWindowA("Shell_TrayWnd", NULL);
	if (!htbwnd)
	{
		DebugLog("�����޷���ȡ���������ھ��");
		return 50;
	}
	return GetWindowHeight(htbwnd);
}
int CJZAPI GetFullUIY(void)
{
	//RECT rt = GetSystemWorkAreaRect();
	//return (rt.bottom - rt.top) * g_scale;
	return (GetScreenHeight() * g_scale - GetTitleHeight() * g_scale - GetTaskbarHeight() * g_scale) / g_scale;
	//return GetScreenHeight() - 50 - GetTitleHeight();
	//return 900;
}


HANDLE CJZAPI GetProcessHandle(DWORD pid)	//ͨ������ID��ȡ���̾��
{
	return OpenProcess(PROCESS_TERMINATE, FALSE, pid);
}
HANDLE CJZAPI GetProcessHandle(LPCSTR lpName)	//ͨ����������ȡ���̾��
{	//******���棡���ִ�Сд��������******// 
	//*****���棡�������չ����������*****// 
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hSnapshot)
	{
		return NULL;
	}
	PROCESSENTRY32 pe = { sizeof(pe) };
	BOOL fOk;
	for (fOk = Process32First(hSnapshot, &pe); fOk; fOk = Process32Next(hSnapshot, &pe))
	{
		if (!_stricmp(pe.szExeFile, lpName))
		{
			CloseHandle(hSnapshot);
			return GetProcessHandle(pe.th32ProcessID);
		}
	}
	return NULL;
}
//from  https://blog.csdn.net/fzuim/article/details/60954959
BOOL CJZAPI HaveProcess(DWORD pid)
{
	return (GetProcessHandle(pid) != NULL ? true : false);
}
BOOL CJZAPI HaveProcess(LPCSTR lpName)
{	//******���棡���ִ�Сд��������******// 
	//*****���棡�������չ����������*****// 
	return (GetProcessHandle(lpName) != NULL ? true : false);
}

HANDLE CJZAPI CreateThread(LPTHREAD_START_ROUTINE func_name)
{ //###���棡��������ʽ����Ϊ DWORD FUNC(LPVOID XXX) ###
	return ::CreateThread(NULL, 0, func_name, NULL, 0, NULL);
}


BOOL CJZAPI ExistFile(string strFile)
{	//�ļ����ļ��ж����� 
	return !_access(strFile.c_str(), S_OK);	//S_OK��ʾֻ����Ƿ���� 
}
BOOL CJZAPI ExistDir(string strDir)
{
	return !_access(strDir.c_str(), S_OK);
}
