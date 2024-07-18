//VisionaryWorld 专属头文件
//任何人不得盗用。
//Written by Rico
#pragma once
#define HWND_CONSOLE GetConsoleWindow()
INT CJZAPI GetScreenHeight(void) //获取屏幕高度 
{
	return GetSystemMetrics(SM_CYSCREEN);
}
INT CJZAPI GetScreenWidth(void) //获取屏幕宽度 
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
RECT CJZAPI GetSystemWorkAreaRect(void) //获取工作区矩形 
{
	RECT rt;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rt, 0);    // 获得工作区大小
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
		DebugLog("错误：未定义的屏幕DPI=" + ToString(ret));
		return 1.0f;
	}
}
LONG CJZAPI GetTaskbarHeight()
{
	HWND htbwnd = FindWindowA("Shell_TrayWnd", NULL);
	if (!htbwnd)
	{
		DebugLog("错误：无法获取任务栏窗口句柄");
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


HANDLE CJZAPI GetProcessHandle(DWORD pid)	//通过进程ID获取进程句柄
{
	return OpenProcess(PROCESS_TERMINATE, FALSE, pid);
}
HANDLE CJZAPI GetProcessHandle(LPCSTR lpName)	//通过进程名获取进程句柄
{	//******警告！区分大小写！！！！******// 
	//*****警告！必须加扩展名！！！！*****// 
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
{	//******警告！区分大小写！！！！******// 
	//*****警告！必须加扩展名！！！！*****// 
	return (GetProcessHandle(lpName) != NULL ? true : false);
}

HANDLE CJZAPI CreateThread(LPTHREAD_START_ROUTINE func_name)
{ //###警告！！函数格式必须为 DWORD FUNC(LPVOID XXX) ###
	return ::CreateThread(NULL, 0, func_name, NULL, 0, NULL);
}


BOOL CJZAPI ExistFile(string strFile)
{	//文件或文件夹都可以 
	return !_access(strFile.c_str(), S_OK);	//S_OK表示只检查是否存在 
}
BOOL CJZAPI ExistDir(string strDir)
{
	return !_access(strDir.c_str(), S_OK);
}
