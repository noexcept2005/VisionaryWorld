/* 	   VisionaryWorld   *
 *      �� �� ֮ ��      *
 *    SURVIVAL-GAME     *
 *   Author: Ricochesta *
 * StartDate: 2021/08/26*
 *    QQ:2399347979     *
 */
#ifdef __GNUC__		//gcc
#define DEVCPP 
#endif
#define NO_UIEFFECT	//��UI���뵭��Ч�� 
//�汾
#define CURRENT_VERSION "v1.0.4.1 Alpha"
#define CHINESE_PRODUCT_NAME "�� �� ֮ ��"
//-lgdi32 -luser32 -mwindows -lwininet -lws2_32 -lgraphics64 -luuid -lmsimg32 -limm32 -lole32 -loleaut32 -lgdiplus

#include <winSock2.h>	//-lws2_32
#include <windows.h>
#include <TlHelp32.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <io.h>
#include <string>
#include <fstream>
#include <sstream> 
#include <stdexcept>
#include <cmath>
#include <vector>
#include "graphics.h"	//Using EGE 
#include "ege\sys_edit.h"
//��������Dev-C++����
#pragma comment(lib,"graphics17.lib")
#pragma comment(lib,"ws2_32.lib")
using namespace std;
#define LOG_NAME "prevPlaying.log"
#define PLAYERDATA_NAME "PlayerData.txt"
#define WORLDDATA_NAME "WorldData.txt"
#define var auto

//ȫ��globals
int uix, uiy;
float g_scale = 1.0f;
int g_offset = 0;
int tag_value=0;
HWND hwnd;
string g_assets_path = "";
string g_release_path = "";
string g_vw_path = "";

void Useless(...){}

#ifdef DEVCPP
#define __noop() Useless()
#endif

#define CJZAPI __stdcall
void CJZAPI DebugLog(string text, bool bTime = true);
void CJZAPI ErrorLogTip(string strText, string strModule="");
template <typename _T>
string CJZAPI ToString(_T value);
template <typename _T>
int CJZAPI ToInt(_T value);

void GetAssetsPath()
{
	//DebugLog("Getting path..");
	string tmp;
	CHAR buff[MAX_PATH] { 0 };
	GetModuleFileNameA(NULL, buff, MAX_PATH);
	tmp = buff;
	//DebugLog("tmp = " + tmp);
	//e.g.   D:\1\VisionaryWorld\Release\VisionaryWorld.exe
	//       012345678901234567890123456789012345  ;size=36
	//                            22
	bool yes = false;
	int i;
	string res = "";
	for (i = tmp.size()-1; i >= 0; i--)
	{
		//DebugLog(ToString(tmp.at(i)));
		if (tmp.at(i) == '\\')
		{
			res = tmp.substr(0, i+1);	//including '\\'
			yes = true;
			break;
		}
	}
	if (!yes)
	{
		ErrorLogTip("�޷�Ѱ�ٲƲ�·��������סַ�Ƿ�Ϸ�\n������Ϣ��û���ҵ�һ����б�� '\\\\'��", "GetAssetsDir");
		exit(-1);
		return;
	}
	#ifdef DEVCPP
	res += "Release\\";
	#endif

	DebugLog("ReleaseĿ¼Ϊ��" + res);	//Release
		g_release_path = res;
		res += "Assets\\";
		g_assets_path = res;
		DebugLog("AssetsĿ¼Ϊ��" + res);
		//��������VWĿ¼·��
		int _cnt = 0;
		yes = false;
		for (i = res.size() - 1; i >= 0; i--)
		{
			//DebugLog(ToString(tmp.at(i)));
			if (res.at(i) == '\\')
			{
				if (_cnt < 2)
				{
					_cnt++;
					continue;
				}
				g_vw_path = tmp.substr(0, i + 1);	//including '\\'
				yes = true;
				break;
			}
		}
		if (!yes)
		{
			ErrorLogTip("�޷�Ѱ��VisionaryWorld·��������סַ�Ƿ�Ϸ�\n������Ϣ��û���ҵ�һ����б�� '\\\\'��", "GetAssetsDir");
			exit(-1);
			return;
		}
}

//��дͷ�ļ�
#include "strings.h"
#include "times.h"
#include "system.h"
#include "egeExtend.h"
#include "print.h"
#include "control.h"

namespace vis {	//��ȥ�����ĺ�������,NICE!
#define the ::vis::
#define own this->
#define self (the pls.at(0))

	void SavePlayingData(void);

	DWORD g_deltaTime;	//֡��

	#include "Physics.h"	//��ƻ���

	//bool g_mpos_runnning = false;
	LONG g_mx, g_my;	//�������������꣨��ʱˢ�£�

	#define MOUSEPOS_CD 0
	void FreshMousePos()
	{
		if (GetForegroundWindow() != hwnd)
			return;		//����Ը�
		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(hwnd, &pt);
		g_mx = pt.x;
		g_my = pt.y;
	}
	bool g_game_playing = false;
	int g_precur = -1;		//���������
	int g_precur_x = -1;	//��Ҫʹ��
	int g_precur_y = -1;	//��Ҫʹ��

	DWORD ThrMousePos(LPVOID none)
	{
		//g_mpos_runnning = true;
		//DebugLog("<�߳�> �����߳� ThrMousePos!");
		while (g_game_playing)
		{
			FreshMousePos();
			//Sleep(MOUSEPOS_CD);
		}
		//DebugLog("<�߳�> �ر��߳� ThrMousePos!");
		return 0;
	}

	struct UIImages {
		PIMAGE item_slot;
		PIMAGE item_slot_cur;
	}uiImages;

	void CheckFolders()
	{
		if (!ExistDir(g_vw_path + "Save"))
		{
			CreateDirectoryA((g_vw_path + "Save").c_str(), NULL);
		}
	}

	#define DEF_SLOT_PX 50
	UINT g_slot_px = DEF_SLOT_PX;	//���ӱ߳������أ�

#define ASSETS_PATH g_assets_path
	void ReadUIImages()
	{
		uiImages.item_slot = newimage();
		uiImages.item_slot_cur = newimage();
		DebugLog("����ȡ����ȡ�����ظ���UIͼƬ...");
		string path = ASSETS_PATH;
		path += "Images\\";
		string tmp = path;
		tmp += "ItemSlot.png";
		_getimage_withzoom(uiImages.item_slot, tmp.c_str(), g_slot_px, g_slot_px);
		tmp = path;
		tmp += "ItemSlotCur.png";
		_getimage_withzoom(uiImages.item_slot_cur, tmp.c_str(), g_slot_px, g_slot_px);
	}
		typedef UINT ITEM_ID;
		typedef UINT BLOCK_ID, BIOME_ID;
	
	ITEM_ID BlockToItem(BLOCK_ID id);
	float CJZAPI GetPlayerX(int i);
	float CJZAPI GetPlayerY(int i);
	float CJZAPI GetPlayerHp(int i);
	bool CJZAPI GetItem(int i,ITEM_ID id,int cnt=1,string tag="~");
	bool OuttaWorld(float x, float y);

	float g_block_px;	//ÿ��ı߳�����λ�����أ�

	//����ߴ�
	#define WORLD_W_MIN 640
	#define WORLD_W_MAX 820
	#define WORLD_H_MIN 730
	#define WORLD_H_MAX 880
	UINT g_world_w;
	UINT g_world_h;

#ifndef NO_UIEFFECT
	#include "StageEffects.h"	//��̨Ч��
#endif
	#include "Entity.h"
	#include "World.h"
	#include "Item.h"
	#include "Player.h"
	

	void DrawSceneBlocks()
	{
		EXCEPTION_L

		for(float y=0;y<(float)g_world_h;y+=1.0f)
			for (float x = 0; x < (float)g_world_w; x+=1.0f)
			{
				if (OuttaWorld(x, y))
				{
					//DebugLog("x=" + ToString(x) + " y=" + ToString(y) + "Out of world");
					continue;
				}
				/*if (x == GetPlayerX(0) && y == GetPlayerY(0))
				{
					int _x, _y;
					_x = uix / 2 - the g_block_px * (the GetPlayerX(0) - x);
					_y = uiy / 2 - the g_block_px * (y - the GetPlayerY(0));
					setfont(20, 0, "������");
					setcolor(AQUA);
					xyprintf(_x + 2, _y + 2, "��");
				}*/
				if (self.InView(x, y) && bk(x, y, "DrawSceneBlocks").id != 0)
					bk(x, y,"DrawSceneBlocks").Draw(x, y);	//��֮
				/*else {
					if (!self.InView(x, y))
					{
						DebugLog("x=" + ToString(x) + " y=" + ToString(y));
					}
				}*/
			}
		EXCEPTION_R
	}
	void DrawEntities()
	{
		EXCEPTION_L
		if (entities.empty())	return;
		for (int i = 0; i < entities.size(); ++i)
		{
			float x, y;
			x = entities.at(i).x, y = entities.at(i).y;
			if (self.InView(x, y) && entities.at(i).id != 0)
			{
				entities.at(i).Draw();
			}
		}
		EXCEPTION_R
	}
	clock_t lastSelect;
#define INTIP_FONT "΢���ź�Light"
#define INTIP_FS 30
#define INTIP_BEGCLR WHITE
#define INTIP_PERIOD_1 3000
#define INTIP_PERIOD_2 500
	void DrawItemNameTip()
	{
		if (self.handitem().id == 0)
			return;
		clock_t cur = clock();
		color_t clr = WHITE;
		if (cur - lastSelect <= INTIP_PERIOD_1)
			clr = INTIP_BEGCLR;
		else if(cur - lastSelect <= INTIP_PERIOD_1 + INTIP_PERIOD_2){
			//MsgSndTip();
			color_t _end = getpixel(uix / 2, uiy - g_slot_px - INTIP_FS / 2 - 24);
			double ratio = double(cur - lastSelect - INTIP_PERIOD_1) / (INTIP_PERIOD_2);
			clr = EgeColor(StepColor(WinColor(INTIP_BEGCLR), WinColor(_end), ratio));
		}
		else {
			return;
		}
		string name = self.handitem().GetName();
		setcolor(clr);
		setfont(INTIP_FS, 0, INTIP_FONT);
		midprinty(name, uiy - g_slot_px - INTIP_FS - 24);
	}
#define SLOT_CNT_FONT "Bahnschrift Semibold"
#define SLOT_CNT_COLOR CORNSILK
#define SLOT_CNT_FS 18
	void DrawSlot(int x, int y, int i,bool cur = false)	//��һ�����Ӽ��������Ʒ
	{
		EXCEPTION_L
		the Item item = self.item(i);
		PIMAGE img = uiImages.item_slot;
		PIMAGE img_cur = uiImages.item_slot_cur;

		putimage_withalpha(NULL, (cur ? img_cur : img), x, y);	//����
		if (item.id == 0 || item.cnt == 0)
			return;

		PIMAGE itemimg = GetItemImage(item.id);
		int ix, iy;
		int imgx = getwidth(itemimg), imgy = getheight(itemimg);
		ix = x + g_slot_px / 2 - imgx / 2;
		iy = y + g_slot_px / 2 - imgy / 2;
		putimage_withalpha(NULL, itemimg, ix, iy);

		string scnt = ToString(item.cnt);
		if (item.cnt == 1)
			scnt = "";

		//setbkmode(TRANSPARENT);
		setfont(SLOT_CNT_FS, 0, SLOT_CNT_FONT);
		setcolor(SLOT_CNT_COLOR);
		int cx = x + g_slot_px - textwidth(scnt.c_str()) - 4;
		int cy = y + g_slot_px - textheight(scnt.c_str()) - 4;
		xyprintf(cx, cy, "%s", scnt.c_str());

		EXCEPTION_R
	}
	void DrawHotbar()	//�����֮�滭
	{
		int _left = uix / 2 - g_slot_px * 10.0f / 2;
		int _top = uiy - g_slot_px - 25 + g_offset;
		//tag_value = uiy;
		
		int cur = self.curItem;
		for (int i = 0; i < 10; i++)
		{
			int _x = _left + g_slot_px * i;
			int _y = _top;
			DrawSlot(_x, _y, i, cur == i);
		}
		if(!self.bagOpen)
			DrawItemNameTip();
	}
	int CJZAPI GetParagraphWidth(string para, int maxWidth = INT_MAX - 1)
	{//ege	//����Ϊ��λ
		int _max = 0;
		int beg = 0;
		for (int i = 0; i < para.size(); i++)
		{
			if (para.at(i) == '\n' || para.at(i) == '\r')
			{
				_max = max(_max, int(para.substr(beg, i - beg).length()));
				beg = i + 1;
			}
		}
		if (_max > maxWidth)
			_max = maxWidth;
		_max *= textwidth('1');	//����Ϊ��λ
		return _max;
	}
	int CJZAPI GetParagraphHeight(string para, int maxHeight = INT_MAX - 1)
	{//ege	//����Ϊ��λ
		int j = 0;
		for (int i = 0; i < para.size(); i++)
		{
			if (para.at(i) == '\n' || para.at(i) == '\r')
				j++;
		}
		if (j == 0)
			j = 1;	//���п��Բ��ӻ���
		j *= textheight('1');	//����Ϊ��λ
		return j;
	}
#define ITAG_MAX_W 650
#define ITAG_MAX_H 65535
#define ITAG_FONT "΢���ź�"
#define ITAG_FS 22
#define ITAG_COLOR EGERGB(255,250,250)	//TEXTCOLOR
#define ITAG_LINE_COLOR EGERGB(255,165,0)
#define ITAG_FILL_COLOR EGERGB(139,62,47)
	void DrawItemTag()
	{
		if (g_precur <= -1 || g_precur >= PLAYER_ITEM_CNT)
			return;
		Item it = self.item(g_precur);	//Ԥѡ��Ʒ
		if (it.id == 0)
			return;
		string desc = it.GetRealDescription();
		int w = GetParagraphWidth(desc, ITAG_MAX_W);	//pixel
		int h = GetParagraphHeight(desc, ITAG_MAX_H);	//pixel
		int x, y;
		x = g_mx + 2;
		y = g_my + 2 + g_offset;
		//��ֹ������
		if (x+6+w >= uix)
			x -= (x+6+w - uix);
		if (y+h+3 >= uiy - 30)
			y -= (y+h+3 - uiy + 30);

		setlinestyle(PS_SOLID, 0, 3);
		setcolor(YELLOW);
		rectangle(x, y, x + 6 + w, y + 5 + h);
		setfillcolor(ITAG_FILL_COLOR);
		bar(x + 2, y + 2, x + 4 + w, y + 3 + h);
		setcolor(ITAG_COLOR);
		setfont(ITAG_FS, 0, ITAG_FONT);
		rectprintf(x + 4, y + 4, ITAG_MAX_W, ITAG_MAX_H, "%s", desc.c_str());
	}
	void DrawTmpItem()
	{
		if (self.tmpItem.IsEmpty())
			return;
		PIMAGE img = GetItemImage(self.tmpItem.id);
		if (img == nullptr)
			return;
		putimage_withalpha(NULL, img, g_mx + 3, g_my + 3);
		if (self.tmpItem.cnt == 1)
			return;
		int imgx = getwidth(img), imgy = getheight(img);
		string scnt = ToString(self.tmpItem.cnt);
		int x, y;
		int w = textwidth(scnt.c_str());
		int h = textheight('H');
		x = g_mx + 5 + imgx - w - 2;
		y = g_my + 5 + imgy - h - 2 + g_offset;
		setfont(SLOT_CNT_FS, 0, SLOT_CNT_FONT);
		setcolor(SLOT_CNT_COLOR);
		xyprintf(x, y, "%d", self.tmpItem.cnt);
	}
	#define HPBAR_COLOR EGERGB(255,69,0)
	#define FPBAR_COLOR EGERGB(238,154,0)
	#define MPBAR_COLOR EGERGB(67,110,238)
	void DrawPlayerBars()
	{
		const int _left = 60;
		const int _top = uiy - 180 + g_offset;
		const int _bar_width = 300;
		const int _bar_height = 30;
		const int _gap = 8;
		const int _offset_left = 44;
		const int _addition = _bar_height + _gap;
		short tw,th;
		string text;
		self.Clamps();
		setfont(30,0,"΢���ź�");
		//HP
		if (self.max_hp > 0)
		{
			setfillcolor(HPBAR_COLOR);
			bar(_left, _top, _left + _bar_width * ((float)self.hp / self.max_hp), _top + _bar_height);
			setfillcolor(EGERGB(238,59,59));
			bar(_left, _top + _bar_height - 8, _left + _bar_width * ((float)self.hp / self.max_hp), _top + _bar_height);
			text = ToString(self.hp) + " / " + ToString(self.max_hp);
			tw = textwidth(text.c_str());
			th = textheight(text.c_str());
			setcolor(YELLOW);
			//setbkmode(TRANSPARENT);
			xyprintf(_left - _offset_left, _top + _bar_height / 2 - th / 2, "����");
			xyprintf(_left + _bar_width / 2 - tw / 2, _top + _bar_height / 2 - th / 2, text.c_str());
		}
		//FP
		if (self.max_fp > 0)
		{
			setfillcolor(FPBAR_COLOR);
			bar(_left, _top + _addition, _left + _bar_width * ((float)self.hp / self.max_hp), _top + _bar_height + _addition);
			setfillcolor(EGERGB(205,102,0));
			bar(_left, _top + _bar_height - 8 + _addition, _left + _bar_width * ((float)self.hp / self.max_hp), _top + _bar_height + _addition);
			text = ToString(self.fp) + " / " + ToString(self.max_fp);
			tw = textwidth(text.c_str());
			th = textheight(text.c_str());
			setcolor(LIGHTGREEN);
			//setbkmode(TRANSPARENT);
			xyprintf(_left - _offset_left, _top + _bar_height / 2 - th / 2 + _addition, "��ʳ");
			xyprintf(_left + _bar_width / 2 - tw / 2, _top + _bar_height / 2 - th / 2 + _addition, text.c_str());
		}
		//MP
		if (self.max_mp > 0)
		{
			setfillcolor(MPBAR_COLOR);
			bar(_left, _top + _addition * 2, _left + _bar_width * ((float)self.hp / self.max_hp), _top + _bar_height + _addition * 2);
			setfillcolor(EGERGB(58,95,205));
			bar(_left, _top + _bar_height - 8 + _addition * 2, _left + _bar_width * ((float)self.hp / self.max_hp), _top + _bar_height + _addition * 2);
			text = ToString(self.mp) + " / " + ToString(self.max_mp);
			tw = textwidth(text.c_str());
			th = textheight(text.c_str());
			setcolor(AQUA);
			//setbkmode(TRANSPARENT);
			xyprintf(_left - _offset_left, _top + _bar_height / 2 - th / 2 + _addition * 2, "ħ��");
			xyprintf(_left + _bar_width / 2 - tw / 2, _top + _bar_height / 2 - th / 2 + _addition * 2, text.c_str());
		}
	}
	void DrawInventory()
	{
		EXCEPTION_L

		if (!self.bagOpen)
			return;

		const int _left = uix / 2 - g_slot_px * 10.0 / 2;
		const int _top = uiy - 25 - g_slot_px * 4 - 5 + g_offset;

		for(int y = 0; y < 3; y ++)
			for (int x = 0; x < 10; x++)
			{
				int _x = _left + g_slot_px * x;
				int _y = _top + g_slot_px * y;
				int i = (y + 1) * 10 + x;	//��Ʒ��������

				DrawSlot(_x, _y, i);
			}
		DrawHotbar();
		if(self.tmpItem.IsEmpty())
			DrawItemTag();

		EXCEPTION_R
	}
	void DrawScene()
	{
		EXCEPTION_L
		//swappage();
		//delay_fps(60);

		DrawSceneBlocks();

		/*setfont(g_block_px, 0, "������");
		setcolor(LIGHTGREEN);
		xyprintf(uix / 2 - 12, uiy / 2 - 12, "��");*/
		self.Draw();
		//delay_fps(30);

		EXCEPTION_R
	}
	void DrawUI()	//����Ϸ�еĽ���
	{
		if (!self.bagOpen)
			DrawHotbar();
		else
			DrawInventory();	//����Hotbar

		DrawTmpItem();
		DrawPlayerBars();
		/*setfont(20, 0, "����");
		setcolor(WHITE);*/
		setfont(20, 0, "Tahoma");
		setcolor(WHITE);
		xyprintf(20, 20 + g_offset, "x=%.1f y=%.1f  vx=%.1f  vy=%.1f", self.x, self.y, self.v.x, self.v.y);
		xyprintf(20, 42 + g_offset, "mx=%d,my=%d; precur=%d", g_mx, g_my, g_precur);
//		xyprintf(20, 64 + g_offset, "bid=%d", bk(self.x, self.y, "DrawUI"));	//WARNING: no checking!!
		//xyprintf(20, 42 + g_offset, "curBlock = %d", bk(self.x, self.y,"curBlock").id);
		//xyprintf(20, 64 + g_offset, "delta = %d", (clock() - self.lastStep));
		xyprintf(20,64 + g_offset,"uix=%d uiy=%d",uix,uiy);
		tag_value = g_offset;
		xyprintf(20, 86 + g_offset, "tag=%d", tag_value);
	}

	void KeyboardControl()
	{
		clock_t cur = clock();
		while (1)
		{
			self.CountStepTime();
			for (int i = 0; i < 10; i++)	//ѡ������ϵ���Ʒ
			{
				if (K('0' + i))
				{
					if (i == 0)
						self.curItem = 9;
					else
						self.curItem = i - 1;
					lastSelect = clock();
				}
			}
			if (K('A'))
			{
				if (self.x <= 0 || self.SideTouch(Vector(-1.0f, 0.0f)))
				{
					self.WallStop();
					break;
				}
				self.v.x = -self.speed();
				self.v.ClampMagnitudeA(self.speed());
			}
			else if (K('D'))
			{
				if (self.x >= g_world_w || self.SideTouch(Vector(1.0f, 0.0f)))
				{
					self.WallStop();
					break;
				}
				self.v.x = self.speed();
				self.v.ClampMagnitudeA(self.speed());
			}
			if (K('W'))	//��y-
			{
				if (self.y >= g_world_h || self.SideTouch(Vector(0.0f, -1.0f)))
				{
					self.WallStop();
					break;
				}
				self.v.y = -self.speed();
				self.v.ClampMagnitudeA(self.speed());
			}
			else if (K('S'))	//��y+
			{
				if (self.y <= 0 || self.SideTouch(Vector(0.0f, 1.0f)))
				{
					self.WallStop();
					break;
				}
				self.v.y = self.speed();
				self.v.ClampMagnitudeA(self.speed());
			}
			else if (K('Q'))	//˲��ֹͣ�ƶ�
			{
				self.v.Clear();
			}
			else if(K(VK_F1))
			{
//				DebugLog(ToString(the itemdata.size()));
				//DebugLog("������Ʒ��"+GetItemName(self.handitem().id));
				g_offset = 40;
				Sleep(400);
			}
			
#define BAGOPEN_CD 260
			if (K('E'))
			{
				if (cur - self.lastBag > BAGOPEN_CD)
				{
					self.bagOpen = !self.bagOpen;
					self.lastBag = clock();
				}
			}		
			if (K(27))	//Esc exitting
			{
				DebugLog("Esc�˳�...");
				setfont(30, 0,"΢���ź�");
				setcolor(YELLOW);
				//setbkmode(OPAQUE);
				midprinty("  ������  ", 100);
				g_game_playing = false;
				the SavePlayingData();
				Sleep(200);
				DebugLog("�ر�EGE����...", false);
				closegraph();
				exit(0);
			}
			break;
		}
		
	}
	void LeftButton()
	{
		clock_t cur = clock();
		Item& item = self.handitem();
		BYTE itype = GetItemType(item.id);
		if (item.id == 0)
			return;
		if (IsWeaponItem(item.id))
		{	//��������˺����ֳֳ�����Ʒ
			//DebugLog("��������");
			if (cur - self.lastAttack <= GetItemCD(item.id))
				return;				//OUT: ��ȴʱ����
			//...
			//self.lastAttack = clock(); down exists
		}
		if (the IsPickaxeItem(item.id) || the IsAxeItem(item.id))
		{	//ר�������ƻ��������Ʒ
			//DebugLog("�丫����");
			if (cur - self.lastAttack <= GetItemCD(item.id))
				return;				//OUT: ��ȴʱ����
			int x, y;
			the GetMouseBlockPos(x, y);
			//DebugLog("�ھ�λ�� x=" + ToString(x) + "y=" + ToString(y));
			if (!InDistance(self.x,self.y,x,y,DEF_REACH_DISTANCE) || OuttaWorld(x, y))	//OUT: ����/����Χ
				return;							//<!>Ĭ��Reach����
			Block& b = the bk(x, y, "LeftButton");
			if (b.GetBrr() == BRR_AIR || b.GetBrr() == BRR_FLUID)
				return;				//OUT: �������ڲ���
			int factpower = the GetItemPower(item.id);	//powerָ�ھ�ĵ���
			if(IsWoodenBlock(b.id) && the IsAxeItem(item.id))
			{
				//MsgSndTip();
				factpower++;	//��ͷ���ʺϿ�ľͷ 
			}
			else if(IsStoneBlock(b.id) && the IsAxeItem(item.id))
			{
				MsgSndTip();
				factpower--;	//��ͷ��������ʯͷ 
			}
				
			ClampA(factpower,1,b.dur+1);
			b.dur -= factpower;
			//DebugLog("�ھ�ɹ�,b.dur=" + ToString(b.dur));
			b.RandomOtherId2();	//��̩������һ�����任ID2������ȥ�����ڱ��ݲ�
			b.Normalize();	//ˢ�� ����
			item.dur--;
			item.Normalize();	//ˢ�� ��Ʒ
			self.lastAttack = clock();
		}else if(the IsBlockItem(item.id))	//���÷��� 
		{
			if (cur - self.lastAttack <= GetItemCD(item.id))
				return;				//OUT: ��ȴʱ����
			int x, y;
			the GetMouseBlockPos(x, y);
			//DebugLog("����λ�� x=" + ToString(x) + "y=" + ToString(y) + "p.x="+ToString(self.x)+" p.y="+ToString(self.y));
			if (!InDistance(self.x,self.y,x,y,DEF_REACH_DISTANCE) || OuttaWorld(x, y)
			|| (int(self.x) == int(x) && int(self.y) == int(y)))	//OUT: ����/����Χ/̫�� 
				return;							//<!>Ĭ��Reach����
			Block& b = the bk(x, y, "LeftButton");
			if (b.GetBrr() != BRR_AIR && !IsTinyBlock(b.id))
				return;				//OUT: �������ڲ���
			SetBlock(x,y,GetItemBlock(item.id));
			item.cnt--;
			item.Normalize();	//ˢ�� ��Ʒ
			self.lastAttack = clock();
		}
		else {
			return;
		}
		self.lastAttack = clock();
	}
	void PhysicsUpdate()
	{
		for (int i = 0; i < pls.size(); i++)
		{
			pls.at(i).Physics::Move();
			pls.at(i).Physics::GroundFrict();
		}
	}

	void RefreshPrecur()
	{
		if (!self.bagOpen)
		{
			g_precur = -1;
			return;
		}
			
		int _left = uix / 2 - g_slot_px * 10.0 / 2;
		int _top = uiy - 45 - g_slot_px * 4 - 5;
		//DebugLog("_left=" + ToString(_left) + " _top=" + ToString(_top));

		/*if (g_mx < _left)
		{
			g_precur = -1; return;
		}
		else if (g_mx > _left + 10.0 * g_slot_px)
		{
			g_precur = -1; return;
		}
		else if (g_my < _top)
		{
			g_precur = -1; return;
		}*/

		g_precur_x = (g_mx - _left) / g_slot_px;
		g_precur_y = (g_my - _top) / g_slot_px;

		short gap = 5;

		if (g_precur_y > 2)
		{
			if (g_my > _top + g_slot_px * 3 + gap && _top + g_slot_px * 4 + gap)
				g_precur_y = -1;	//�Ե�
			else
			{
				//DebugLog("down");
				g_precur = -1;
				return;
			}
		}
		else if (g_precur_y < 0 || g_precur_x < 0 || g_precur_x > 9)
		{
			//DebugLog("up");
			//g_precur_x = 0;
			g_precur = -1;
			return;
		}
		else {
			//DebugLog("normal");
		}
		//DebugLog("1");
		g_precur = (g_precur_y + 1) * 10 + g_precur_x;	//refresh
		if (g_precur < 0 || g_precur >= PLAYER_ITEM_CNT)
			g_precur = -1;	//���ı��� �������
		return;
	}
#define TMPITEM_CHANGING_CD 200
	DWORD ThrMouseControl(LPVOID none)
	{
		//DebugLog("[�߳�] �����̣߳�ThrMouseControl");
		while (g_game_playing)
		{
			RefreshPrecur();
			//if (! self.tmpItem.IsEmpty())	//�ŵ�DrawUI��
			if (g_precur < 0 || g_precur >= PLAYER_ITEM_CNT)
				g_precur = -1;	//���ı��� �������
			if (g_precur != -1)
			{
				if (LEFT_CLICK_FOCUSED(hwnd)
					)	//��� 
				{
					if (clock() - self.lastTmpItemChanged >= TMPITEM_CHANGING_CD	//��ֹˢ�� 
						&& (self.tmpItem.id != 0 || self.preitem().id != 0))
					{
						if (!SAME_ITEM(self.tmpItem, self.preitem()))
						{	//�����Ժϲ��� �ͽ���
							SWAP_ITEM(self.tmpItem, self.preitem());
							self.preitem().Normalize();
						}
						else {	//���Ժϲ��� �ͺϲ�
							var _cnt = self.preitem().cnt + self.tmpItem.cnt;	//original
							var _maxheap = GetItemMaxHeap(self.preitem().id);
							if (_cnt > _maxheap)
							{
								self.tmpItem.cnt = _cnt - _maxheap;
								_cnt = _maxheap;
							}
							else {
								self.tmpItem.Clear();	//��� 
							}
							self.preitem().cnt = _cnt;	//Merge
						}
						//					Sleep(60);
						self.lastTmpItemChanged = clock();
					}//left click
					if (RIGHT_CLICK_FOCUSED(hwnd))
					{	//�Ҽ����ţ�����һ�� 
						if (clock() - self.lastTmpItemChanged >= TMPITEM_CHANGING_CD	//��ֹˢ�� 
							&& !self.tmpItem.IsEmpty()	//src���� 
							&& (SAME_ITEM(self.tmpItem, self.preitem())	//�ɺϲ� 
								|| self.preitem().IsEmpty())					//�����dstΪ�� 
							&& !self.preitem().IsFull())	//���벻�� 
						{
							if (self.preitem().IsEmpty())
							{
								self.preitem() = self.tmpItem;	//ASS,��֤��������ת�� 
								self.preitem().cnt = 1;
							}
							else {
								self.preitem().Add(1);
							}
							self.tmpItem.Minus(1);
							self.lastTmpItemChanged = clock();
						}
					}//right click
				}
			}
			else {
				if (LEFT_CLICK_FOCUSED(hwnd)
					)	//��� 
				{
					LeftButton();
				}
			}
		}
		//DebugLog("[�߳�] ֹͣ�̣߳�ThrMouseControl");
		return 0;
	}
	clock_t lastGrassAni,lastLiquidAni;
#define ANI_GRASS_T 500
#define ANI_LIQUID_T 800
	void ApproachEnvAnimation()
	{	//������Χ��������
		clock_t cur = clock();
		/*if (cur - lastGrassAni >= ANI_GRASS_T)
		{
			for(int y = self.y - uiy / 2.0f / g_block_px - VIEW_EXTRA_BLOCKS;
				y<= self.y + uiy / 2.0f / g_block_px + VIEW_EXTRA_BLOCKS; ++y)
				for (int x = self.x - uix / 2.0f / g_block_px - VIEW_EXTRA_BLOCKS;
					x <= self.x + uix / 2.0f / g_block_px + VIEW_EXTRA_BLOCKS; ++x)
			{
				Block& b = bk(x, y, "ApproachEnvAnimation");
				if (b.id == 11 || b.id == 12)
				{
					b.RandomOtherId2();
				}
			}
			lastGrassAni = clock();
		}*/
		if (cur - lastLiquidAni >= ANI_LIQUID_T)
		{
			for (int y = self.y - uiy / 2.0f / g_block_px - VIEW_EXTRA_BLOCKS;
				y <= self.y + uiy / 2.0f / g_block_px + VIEW_EXTRA_BLOCKS; ++y)
				for (int x = self.x - uix / 2.0f / g_block_px - VIEW_EXTRA_BLOCKS;
					x <= self.x + uix / 2.0f / g_block_px + VIEW_EXTRA_BLOCKS; ++x)
			{
				Block& b = bk(x, y, "ApproachEnvAnimation");
				if (b.GetBrr() == BRR_FLUID)
				{
					b.RandomOtherId2();
				}
			}
			lastLiquidAni = clock();
		}
	}
	bool ExistSaves()	//�������浵
	{
		if (ExistFile(g_vw_path + "Save\\" + PLAYERDATA_NAME) && ExistFile(g_vw_path + "Save\\" + WORLDDATA_NAME))
			return true;
		return false;
	}
	void ReadPlayingData(void)	//��ȡ�������� ��������������
	{
		string path = "";
		DebugLog("��ȡ�������...");
		path = g_vw_path + "Save\\" + PLAYERDATA_NAME;
		fstream fin(path, ios::in);
		if (fin.fail())
		{
			ErrorLogTip("�޷��ҵ���������ļ�", "ReadPlayingData");
			exit(-1);
		}
		fin >> self.name;
		fin >> self.x >> self.y;
		fin >> self.fig_id;
		fin >> self.hp >> self.max_hp;
		fin >> self.m;
		for (int t = 0; t < PLAYER_ITEM_CNT; ++t)
			fin >> self.item(t).id;
		for (int t = 0; t < PLAYER_ITEM_CNT; ++t)
			fin >> self.item(t).cnt;
		for (int t = 0; t < PLAYER_ITEM_CNT; ++t)
			fin >> self.item(t).dur;
		for (int t = 0; t < PLAYER_ITEM_CNT; ++t)
			fin >> self.item(t).tag;
		for (int t = 0; t < PLAYER_ITEM_CNT; ++t)
			self.item(t).Normalize();

		for (int t = 0; t < PLAYER_EQUIP_CNT; ++t)
			fin >> self.equip(t).id;
		for (int t = 0; t < PLAYER_EQUIP_CNT; ++t)
			fin >> self.equip(t).cnt;
		for (int t = 0; t < PLAYER_EQUIP_CNT; ++t)
			fin >> self.equip(t).dur;
		for (int t = 0; t < PLAYER_EQUIP_CNT; ++t)
			fin >> self.equip(t).tag;

		for (int t = 0; t < PLAYER_ARMOR_CNT; ++t)
			fin >> self.armor(t).id;
		for (int t = 0; t < PLAYER_ARMOR_CNT; ++t)
			fin >> self.armor(t).cnt;
		for (int t = 0; t < PLAYER_ARMOR_CNT; ++t)
			fin >> self.armor(t).dur;
		for (int t = 0; t < PLAYER_ARMOR_CNT; ++t)
			fin >> self.armor(t).tag;

		if (fin.fail())
		{
			ErrorLogTip("��������ļ����𻵡� :(", "ReadPlayingData");
			exit(-1);
		}
		fin.close();

		DebugLog("��ȡ��������...");
		path = g_vw_path + "Save\\" + WORLDDATA_NAME;
		fin.open(path, ios::in);
		if (fin.fail())
		{
			ErrorLogTip("�޷��ҵ����������ļ�", "ReadPlayingData");
			exit(-1);
		}
		fin >> g_world_w >> g_world_h;
		EmptyWorld();
		for (int y = 0; y < g_world_h; ++y)
		{
			for (int x = 0; x < g_world_w; ++x)
			{
				Block b;
				fin >> b.id;
				fin >> b.id2;
				fin >> b.dur;
				fin >> b.biome;
				b.Normalize();	//good��
				bk(x, y, "ReadWorld").id = b.id;
				bk(x, y, "ReadWorld").id2 = b.id2;
				bk(x, y, "ReadWorld").dur = b.dur;
				bk(x, y, "ReadWorld").biome = b.biome;
				//blocks.push_back(b);	//WRONG!
			}
		}
		DebugLog("�����С("+ToString(g_world_w)+","+ToString(g_world_h)+"); ���緽��������" + ToString(blocks.size()));
		if (fin.fail())
		{
			ErrorLogTip("���������ļ����𻵡� :(", "ReadPlayingData");
			exit(-1);
		}
		if (!fin.eof() && !fin.fail())
		{	//����ʵ������
			while (!fin.eof() && !fin.fail())
			{
				Entity e;
				fin >> e.id >> e.m >> e.x >> e.y
					>> e.hp >> e.max_hp;
				if (e.id <= 0)	continue;
				entities.push_back(e);
			}
		}
		fin.close();
	}
	void SavePlayingData(void)
	{
		string path = "";
		DebugLog("�����������...");
		path = g_vw_path + "Save\\" + PLAYERDATA_NAME;
		DebugLog(path);
		fstream fout(path, ios::out);
		fout << self.name << "\n";
		fout << self.x << " " << self.y<<"\n";
		fout << self.fig_id<<"\n";
		fout << self.hp << " " << self.max_hp << "\n";
		fout << self.m << "\n";
		fout << "\n";
		for (int t = 0; t < PLAYER_ITEM_CNT; ++t)
			fout << self.item(t).id << " ";
		fout << endl;
		for (int t = 0; t < PLAYER_ITEM_CNT; ++t)
			fout << self.item(t).cnt << " ";
		fout << endl;
		for (int t = 0; t < PLAYER_ITEM_CNT; ++t)
			fout << self.item(t).dur << " ";
		fout << endl;
		for (int t = 0; t < PLAYER_ITEM_CNT; ++t)
			fout << self.item(t).tag << " ";
		fout << endl;
		fout << endl;

		for (int t = 0; t < PLAYER_EQUIP_CNT; ++t)
			fout << self.equip(t).id << " ";
		fout << endl;
		for (int t = 0; t < PLAYER_EQUIP_CNT; ++t)
			fout << self.equip(t).cnt << " ";
		fout << endl;
		for (int t = 0; t < PLAYER_EQUIP_CNT; ++t)
			fout << self.equip(t).dur << " ";
		fout << endl;
		for (int t = 0; t < PLAYER_EQUIP_CNT; ++t)
			fout << self.equip(t).tag << " ";
		fout << endl;
		fout << endl;

		for (int t = 0; t < PLAYER_ARMOR_CNT; ++t)
			fout << self.armor(t).id << " ";
		fout << endl;
		for (int t = 0; t < PLAYER_ARMOR_CNT; ++t)
			fout << self.armor(t).cnt << " ";
		fout << endl;
		for (int t = 0; t < PLAYER_ARMOR_CNT; ++t)
			fout << self.armor(t).dur << " ";
		fout << endl;
		for (int t = 0; t < PLAYER_ARMOR_CNT; ++t)
			fout << self.armor(t).tag << " ";
		fout << endl;

		fout.close();

		DebugLog("������������...");
		path = g_vw_path + "Save\\" + WORLDDATA_NAME;
		DebugLog(path);
		fout.open(path, ios::out);
		fout << g_world_w << "  " << g_world_h << "\n";	//World Size
		fout << "\n";
		for (int y = 0; y < g_world_h; ++y)
		{
			for (int x = 0; x < g_world_w; ++x)
			{
				Block b = bk(x, y, "SaveWorld");
				fout << b.id << " " << b.id2 << " " << b.dur << " " << b.biome << " ";
			}
			fout << endl;
		}
		
		//ʵ�����ݰ�������������WorldData֮��
		if (!entities.empty())
		{
			fout << "\n\n";
			for (int c = 0; c < entities.size(); c++)
			{
				fout << entities.at(c).id << " " << entities.at(c).m << " " << entities.at(c).x << " " << entities.at(c).y << " "
					<< entities.at(c).hp << " " << entities.at(c).max_hp;
				fout << endl;
			}
		}
		fout.close();
		DebugLog("��Ϸ���ݱ������");
	}

#define SCENE_DRAW_T 0			//�����滭����
#define UPDATE_T 0				//����ϵͳ��������
	void Movement()	//��Ϸ����
	{
		EXCEPTION_L
			//g_offset = 40;
		//g_offset = 50;

		DebugLog("������Ϸ����Movement!");
		
		CreateThread((LPTHREAD_START_ROUTINE)ThrMousePos);
		CreateThread((LPTHREAD_START_ROUTINE)ThrMouseControl);
		clock_t lastDraw = clock() + SCENE_DRAW_T;
		clock_t lastUpdate = clock();
		g_game_playing = true;
		while (g_game_playing)
		{
			if (ToMilliSecond(clock() - lastDraw) >= SCENE_DRAW_T)
			{
				//setbkmode(TRANSPARENT);
				cls();
				DrawScene();
				DrawEntities();
				ApproachEnvAnimation();
				DrawUI();
				lastDraw = clock();

				//������������
				if (hwnd == GetForegroundWindow() && g_scale > 1.0f)
				{
					g_offset = 40;
				}
				else
					g_offset = 0;

				delay_fps(30);
			}
			if (ToMilliSecond(clock() - lastUpdate) >= UPDATE_T)
			{
				g_deltaTime = ToMilliSecond(clock() - lastUpdate);	//ms
				PhysicsUpdate();
				lastUpdate = clock();
			}
			KeyboardControl();
			self.LimitCoord();
		}
		g_game_playing = false;

		EXCEPTION_R
	}

	void Loading()
	{
		the g_block_px = BLOCK_DEF_PX;	//��һ���BUG�˷ѵ�ʱ�䣺25����

		cls();
		setfont(30, 0, "΢���ź�");
		setcolor(WHITE);
		setbkmode(OPAQUE);	//��������

		midprint("������", 9);
		setfont(25, 0, "΢���ź�");
		setcolor(YELLOW);

		midprint("��ȡ��������..", 10);
		delay_fps(10);
		ReadFigures();

		midprint("��ȡUIͼƬ...", 10);
		delay_fps(10);
		ReadUIImages();

		midprint("��ȡ�������� BlockData ..",10);
		delay_fps(30);
		ReadBlockData();

		midprint("��ȡʵ������ EntityData ...", 10);
		delay_fps(40);
		ReadEntityData();
		
		midprint("��ȡȺ������ BiomeData ...", 10);
		delay_fps(30);
		ReadBiomeData();

		midprint("��ȡ��Ʒ���� ItemData ....", 10);
		delay_fps(80);
		ReadItemData();

		Sleep(50);
		setbkmode(TRANSPARENT);
	}

	void WannaPlay()
	{
		srand(time(0));
		
		if (!pls.empty())	pls.clear();	//clear

		Player me;
		me.Init();			//init

		if (ExistSaves())
		{
			DebugLog("��ǰ���ڴ浵!");
			pls.push_back(me);	//in
			//self = pls.front();	//ref-transport
			ReadPlayingData();
		}
		else {	//New World and Player
			DebugLog("�����´浵...");
			cls();
			setcolor(YELLOW);
			setfont(25, 0, "΢���ź�");
			midprinty("$ ���������� $", 45);
			InitWorld();		//The World
			delay_fps(70);
			cls();
			setfont(30, 0, "΢���ź�");
			midprint("% ��������� %", 42);
			me.Birth();
			delay_fps(50);
			pls.push_back(me);	//in
			//self = pls.front();	//ref-transport

			cls();
			setcolor(AQUA);
			setfont(35, 0, "΢���ź�");
			midprinty("# ���������� #", 40);
			SavePlayingData();	//����һ����˵
			delay_fps(60);
		}

		Movement();	//ENTER GAME
	}
	void Settings()
	{
		//empty
	}

	void DrawMainUI(RECT& rt1, RECT& rt2, RECT& rt3)
	{
		//delay_fps(60);
		cls();
		setfont(75, 0, "Calibri");
		setcolor(PINK);
		midprinty("VisionaryWorld", 100);
		setfont(60, 0, "΢���ź�");
		setcolor(PLUM);
		//xyprintf(uix/2-150,185,CHINESE_PRODUCT_NAME);
		midprint(CHINESE_PRODUCT_NAME, 3);
		setcolor(AQUA);
		setfont(40, 0, "΢���ź�");
		rt1 = midprint("         ��ʼ��Ϸ         ", 10);
		setcolor(LIGHTBLUE1);
		setlinestyle(PS_SOLID, 0, 1);
		rectangle(rt1.left, rt1.top-5, rt1.right+5, rt1.bottom);
		rt2 = midprint("         ѡ    ��         ", 12);
		setcolor(LIGHTBLUE1);
		rectangle(rt2.left, rt2.top-5, rt2.right+5, rt2.bottom);
		rt3 = midprint("         ��    ��          ", 14);
		setcolor(LIGHTBLUE1);
		rectangle(rt3.left, rt3.top-5, rt3.right+5, rt3.bottom);

		/*setfont(20, 0, "Consolas");
		setcolor(WHITE);
		xyprintf(20, 20, "x=%d,y=%d", g_mx, g_my);*/

		setfont(25, 0, "Century Gothic");
		setcolor(DARKGRAY);
		xyprintf(0, uiy - 101, CURRENT_VERSION);
		xyprintf(0, uiy - 76, "Made by Ricochesta");
		xyprintf(0, uiy - 51, "qq-> 2399347979");
#ifndef NO_UIEFFECT
		DrawStageEffects();
#endif
		delay_fps(60);
		//delay_jfps(60);
	}

#define MAINUI_DRAW_T 40
	RECT g_rt1, g_rt2, g_rt3;	//��ť����
	bool g_muidraw_running;

	DWORD ThrDrawingMainUI(LPVOID none)
	{
		clock_t lastDraw=clock()+MAINUI_DRAW_T;
		g_muidraw_running = true;

		DrawMainUI(g_rt1, g_rt2, g_rt3);
		//DebugLog("<�߳�> �����̣߳�ThrDrawingMainUI");
		while (g_muidraw_running)
		{
			clock_t cur = clock();
			if (ToMilliSecond(cur - lastDraw) >= MAINUI_DRAW_T)
			{
				//DebugLog("draw");
				DrawMainUI(g_rt1, g_rt2, g_rt3);
				//DebugLog(ToString(rt1.left) + " " + ToString(rt1.top));
				lastDraw = clock();
			}
			//Sleep(1);
		}
		//DebugLog("<�߳�> ֹͣ�̣߳�ThrDrawingMainUI");
		return 0;
	}

	void EnterMainUI()
	{
		DebugLog("����������.");
#ifndef NO_UIEFFECT
		if (!vis::stageEffects.empty())	vis::stageEffects.clear();
#endif
		mouse_msg mmsg;
		#ifndef NO_UIEFFECT
		AddStageEffect(STAGE_FADEIN, BLACK, 1100);	//����Ч��
		#endif

		Sleep(120);
		DrawMainUI(g_rt1, g_rt2, g_rt3);
		UINT gonna = 0;		//ѡ���ĸ���ť������
		CreateThread((LPTHREAD_START_ROUTINE)ThrDrawingMainUI);

		while (true)
		{
			FreshMousePos();
			//DebugLog(ToString(ToMilliSecond(clock() - lastDraw)));
#ifndef NO_UIEFFECT
			CheckStageEffects();	//���
#endif

			if (mousemsg())
			{
				mmsg = getmouse();
				//MsgSndTip();
				if (mmsg.is_up())
				{
					//MsgSndTip();

					if (InRect(g_rt1, g_mx, g_my))
					{
						#ifndef NO_UIEFFECT
						AddStageEffect(STAGE_FADEOUT, BLACK, 700);	//����Ч��
						#endif
						gonna = 1;
						//Sleep(60);
					}
					else if (InRect(g_rt2, g_mx, g_my))
					{
						ErrorTip(NULL, "���û�û���أ���", "VisionaryWorld ERROR");
						//Sleep(60);
						//gonna = 2;
					}
					else if (InRect(g_rt3, g_mx, g_my))
					{
						#ifndef NO_UIEFFECT
						AddStageEffect(STAGE_FADEOUT, BLACK, 300);	//����Ч��
						#endif
						gonna = 3;
					}
					else {
						gonna = 0;
						//Sleep(30);
					}
				}
			}
#ifndef NO_UIEFFECT
			//if (gonna != 0 && NoStageEffectsAlive())
			if (gonna != 0 && stageEffects.empty())
#else
			if(gonna != 0)
#endif
			{
				g_muidraw_running = false;
				break;
			}
		}

		if (gonna == 1)
		{
			WannaPlay();
			EnterMainUI();	//�����Լ�
		}
		else if (gonna == 2)
		{
			Settings();
			EnterMainUI();	//�����Լ�
		}
		else if (gonna == 3)
		{
			DebugLog("�˳�...");
			g_game_playing = false;	//�˳��߳�
			Sleep(200);
			DebugLog("�ر�EGE����...", false);
			closegraph();
			exit(0);
		}
		else {
			ErrorTip(NULL, "��Ч������", "VisionaryWorld ERROR");
			EnterMainUI();	//�����Լ�
		}
	}

}//end of namespace

using namespace vis;

int main(int argc,char *argv[])
{
	if (IsDebuggerPresent() || HaveProcess("ollydbg.exe"))	//�򵥵ķ�����
	{
		ErrorTip(NULL, "�Ҳ���ָ����ģ�� msvcrt.dll:\n��װ��ϷҲ������޸���һ���⡣", "VisionaryWorld init ERROR");
		exit(-1);
	}

	g_game_playing = false;
	CreateFile3(LOG_NAME, "*********************************");
	DebugLog("       VisionaryWorld ��Ϸ������־   ", false);
	DebugLog((string)"����ʱ��:" + ToString(GetYear()) + "/" + ToString(GetMonth()) + "/" + ToString(GetDay()) + " " + ToString(GetHour()) + ":" + ToString(GetMinute()) + ":" + ToString(GetSecond()), false);
	DebugLog("\nע��EGE����...", false);

	CheckFolders();
	GetAssetsPath();
	//g_scale = GetScreenScale();
	g_scale = 1.25f;
	g_offset = 0;
	DebugLog("��ʾ�����ű��� = " + ToString(g_scale));

	uix = GetFullUIX();
	uiy = GetFullUIY();
	DebugLog("GetScreenHeight = " + ToString(GetScreenHeight() * g_scale));
	DebugLog("GetTitleHeight = " + ToString(GetTitleHeight() * g_scale));
	DebugLog("GetTaskbarHeight = " + ToString(GetTaskbarHeight() * g_scale));
	DebugLog("GetFullUIY = " + ToString(uiy));
	//uiy -= GetTitleHeight();
	initgraph(uix, uiy, INIT_RENDERMANUAL);
	//DebugLog("GetClientTop = " + ToString(GetClientTop(hwnd)));
	
	delay_fps(60);
	hwnd = getHWnd();
	initprint();
	ShowWindow(hwnd, SW_MAXIMIZE);
	
	//uix = GetClientWidth(hwnd);
	//uiy = GetClientHeight(hwnd);
	//DebugLog("GetClientHeight = " + ToString(uiy));
	//uiy -= GetTitleHeight();
	//uiy /= g_scale;
	
	setrendermode(RENDER_MANUAL);
	setcaption((string("VisionaryWorld ")+CURRENT_VERSION).c_str());
	setbkcolor(BLACK);
	setbkmode(TRANSPARENT);

	Loading();

	//CreateThread(LPTHREAD_START_ROUTINE(vis::ThrMousePos));	//���߳�
	EnterMainUI();

	//getch();
	DebugLog("�ر�EGE����...", false);
	closegraph();
}