//VisionaryWorld 专属头文件
//任何人不得盗用。
//Written by Rico
#pragma once

#ifndef NOEXCEPT
#define EXCEPTION_L try{
#define EXCEPTION_R }catch(exception& e)	\
					{	\
						MessageBoxA(NULL, e.what(), ((string)"VisionaryWorld EXCEPTION in "+__func__).c_str(), MB_ICONERROR);	\
					}
#else
#define EXCEPTION_L 
#define EXCEPTION_R 
#endif

void CJZAPI CreateFile3(LPCSTR wholePath,LPCSTR text)
{
	FILE *fp;
	fp = fopen(wholePath, "w");
	fputs((LPCSTR)text, fp);
	fclose(fp);
	return;
}
void CJZAPI DebugLog(string text, bool bTime/* = true*/)
{	//日志输出 
	FILE *fp = fopen(LOG_NAME, "a+");
	//fprintf(fp,"\n%s%s",bTime?(("["+string(ToTimeStr(time(0)))+"] ").c_str()):"",text.c_str());
	fprintf(fp, "\n%s%s", bTime ? (("[" + ToString(GetHour()) + ":" + ToString(GetMinute()) + ":" + ToString(GetSecond()) + "] ").c_str()) : "", text.c_str());
	fclose(fp);
}

VOID CJZAPI InfoTip(HWND hwnd = nullptr, string strText = "Information", string strTitle = "InfoTip")
{
	MessageBox(hwnd, strText.c_str(), strTitle.c_str(), MB_ICONINFORMATION | MB_OK | MB_SYSTEMMODAL);
}
VOID CJZAPI ExclaTip(HWND hwnd = nullptr, string strText = "Exclamation", string strTitle = "ExclaTip")
{
	MessageBox(hwnd, strText.c_str(), strTitle.c_str(), MB_ICONEXCLAMATION | MB_OK | MB_SYSTEMMODAL);
}
VOID CJZAPI ErrorTip(HWND hwnd = nullptr, string strText = "Error", string strTitle = "ErrorTip")
{
	MessageBox(hwnd, strText.c_str(), strTitle.c_str(), MB_ICONERROR | MB_OK | MB_SYSTEMMODAL);
}

void CJZAPI ErrorLogTip(string strText, string strModule/* = ""有的*/)
{
	stringstream ss;
	ss << "【错误】(" << strModule << "): " << strText;
	DebugLog(ss.str());
	ErrorTip(NULL, strText, "VisionaryWorld ERROR [" + strModule + "]");
}

void CJZAPI MsgSndTip()
{
	MessageBeep(MB_ICONEXCLAMATION);
}

int g_lineNum;	//行数 , 从零开始 

void CJZAPI initprint(void)
{
	g_lineNum = 0;
}
void CJZAPI cls(bool bZeroLineNum = true)
{	//清屏 
	cleardevice();
	if (bZeroLineNum)
		g_lineNum = 0;
}

#define CUR_LINE_NUM (g_lineNum)	//当前行 
#define NEXT_LINE_NUM (++g_lineNum)	//下一行 

#define _OFFSET_X 0

RECT CJZAPI midprint(string strText, int lineNum = NEXT_LINE_NUM)
{	//居中显示句子 
	int fy = textheight('1');	//font height
	int fx = textwidth('1');	//font width
	//int sx = fx * strText.length() ;	//string width
	int sx = textwidth(strText.c_str());
	int x = uix / 2 - sx / 2 + _OFFSET_X;

	int y = fy * lineNum;

	xyprintf(x, y, "%s", strText.c_str());

	RECT rt;
	rt.left = x;
	rt.right = x + sx;
	rt.top = y;
	rt.bottom = y + fy;
	return rt;	//返回矩形
}
RECT CJZAPI midprinty(string strText, int y)
{	//自定义 y
	int fy = textheight('1');	//font height
	int fx = textwidth('A');	//font width
	//int sx = fx * strText.length() ;	//string width
	int sx = textwidth(strText.c_str());
	int x = uix / 2 - sx / 2 + _OFFSET_X;
	xyprintf(x, y, "%s", strText.c_str());

	RECT rt;
	rt.left = x;
	rt.right = x + sx;
	rt.top = y;
	rt.bottom = y + fy;
	return rt;	//返回矩形
}
