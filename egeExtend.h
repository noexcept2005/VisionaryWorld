//VisionaryWorld 专属头文件
//任何人不得盗用。
//Written by Rico
#pragma once


//颜色 
//Reference from https://tool.oschina.net/commons?type=3

//橙黄系列 
#define IVORY EGERGB(255,255,240)	//极淡黄 
#define CORNSILK EGERGB(255,248,220)	//淡黄 
#define GOLD EGERGB(255,215,0)
#define ORANGE EGERGB(255,165,0)

//绿色系列
#define DARKGREEN EGERGB(0,100,0) 

//蓝系列 
#define POWDERBLUE EGERGB(176,224,230)
#define AQUA EGERGB(0,0xFC,0xFC)
#define ROYALBLUE EGERGB(65,105,225)
#define STEELBLUE EGERGB(70,130,180)
#define CADETBLUE EGERGB(95,158,160)
#define LIGHTBLUE1 EGERGB(191,239,255)
#define LIGHTCYAN1 EGERGB(224,255,255)
#define TURQUOISE EGERGB(64,224,208)	//青色 

//粉紫系列 
#define PINK EGERGB(255,105,180) 
#define LIGHTPINK EGERGB(255,182,193)
#define MAROON EGERGB(176,48,96)
#define VIOLET EGERGB(238,130,238)
#define PLUM EGERGB(221,160,221)
#define PURPLE EGERGB(160,32,240)

//黑白系列
#define GRAY_11 EGERGB(28,28,28)
#define DIMGRAY EGERGB(105,105,105)
#define SLATEGRAY EGERGB(112,128,144)	//略带蓝色

COLORREF CJZAPI StepColor(COLORREF startColor, COLORREF endColor, double rate)
{	//颜色的渐变 
	int r = (GetRValue(endColor) - GetRValue(startColor));
	int g = (GetGValue(endColor) - GetGValue(startColor));
	int b = (GetBValue(endColor) - GetBValue(startColor));

	int nSteps = max(abs(r), max(abs(g), abs(b)));
	if (nSteps < 1) nSteps = 1;

	// Calculate the step size for each color
	float rStep = r / (float)nSteps;
	float gStep = g / (float)nSteps;
	float bStep = b / (float)nSteps;

	// Reset the colors to the starting position
	float fr = GetRValue(startColor);
	float fg = GetGValue(startColor);
	float fb = GetBValue(startColor);

	COLORREF color;
	for (int i = 0; i < int(nSteps * rate); i++)
	{
		fr += rStep;
		fg += gStep;
		fb += bStep;
		color = RGB((int)(fr + 0.5), (int)(fg + 0.5), (int)(fb + 0.5));
		//color 即为重建颜色
	}
	return color;
}//from https://bbs.csdn.net/topics/240006897 , owner: zgl7903

COLORREF CJZAPI WinColor(color_t clr)
{	//EGERGB -> RGB
	return RGB(EGEGET_R(clr), EGEGET_G(clr), EGEGET_B(clr));
}
color_t CJZAPI EgeColor(COLORREF clr)
{	//RGB -> EGERGB
	return EGERGB(GetRValue(clr), GetGValue(clr), GetBValue(clr));
}

//↓带缩放的getimage()函数（原来的zoomWidth/zoomHeight不管用，是个bug） 
VOID EGEAPI getimage_withzoom(PIMAGE pDstImg,
	LPCSTR lpFile,
	UINT zoomWidth,
	UINT zoomHeight)
{
	getimage(pDstImg, lpFile);
	//Resize image to pwidth*pheight
	PIMAGE save = gettarget();
	//Get image size
	settarget(pDstImg);
	int width, height;
	width = getwidth();
	height = getheight();
	settarget(save);
	PIMAGE backproc = newimage(zoomWidth, zoomHeight);
	//Resize
	putimage(backproc, 0, 0, zoomWidth, zoomHeight, pDstImg, 0, 0, width, height);
	getimage(pDstImg, backproc, 0, 0, zoomWidth, zoomHeight);
	delimage(backproc);
}

VOID EGEAPI _getimage_withzoom(PIMAGE pDstImg,
	LPCSTR lpFile,
	UINT zoomWidth,
	UINT zoomHeight)
{
	//先检查，更保险！
	if (!ExistFile(lpFile))
	{
		ErrorLogTip("找不到图像：" + (string)lpFile, "_getimage_withzoom");
		return;
	}
	getimage_withzoom(pDstImg, lpFile, zoomWidth, zoomHeight);
}

void EGEAPI _getimage(PIMAGE pDstImg, LPCSTR lpFile)
{
	if (!ExistFile(lpFile))
	{
		string info = (string)"找不到指定图像:" + lpFile;
		DebugLog(info);
		ErrorLogTip( info.c_str(), "_getimage");
		return;
	}
	//DebugLog(lpFile);
	getimage(pDstImg, lpFile);
}
