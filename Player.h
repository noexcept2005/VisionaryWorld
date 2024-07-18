//VisionaryWorld ר��ͷ�ļ�
//�κ��˲��õ��á�
//Written by Rico
#pragma once

#define PLAYER_DEF_NAME "Player"//Ĭ���������
#define PLAYER_DEF_MAX_HP 100	//Ĭ�����Ѫ��
#define PLAYER_DEF_MAX_FP 20	//Ĭ�����ʳ 
#define PLAYER_DEF_MAX_MP 0	//Ĭ�����ħ�� 
#define PLAYER_DEF_SPEED 4.0f	//Ĭ�ϱ�׼�ٶ�
//#define PLAYER_DEF_SPEED 40.0f	//Ĭ�ϱ�׼�ٶ�
#define PLAYER_DEF_MASS 50		//kg

//fig_id
#define PERSON_MAN 0

#define FAT_NONE 0
#define FAT_PERSON 1
#define FAT_ENTITY 2

class FigureAnimation {		//���󶯻�
public:
	UINT type;	//��������ʲô����

	ENTITY_ID id;
	PIMAGE figure;
	PIMAGE walk[4];	//0,1����   2,3ǰ��

	FigureAnimation()
	{
		type = FAT_NONE;
		figure = newimage();
		walk[0] = newimage();
		walk[1] = newimage();
		walk[2] = newimage();
		walk[3] = newimage();
	}
};

vector<the FigureAnimation> figures{};	//��������

void ReadFigures()
{
	if (!the figures.empty())	the figures.clear();

	string path = ASSETS_PATH;
	path += "Images\\";
	path += "Person_";
	string tmp;
	int i = 0;
	while (1)
	{
		PIMAGE img = newimage();
		the FigureAnimation anm;
		anm.id = 0;
		anm.type = FAT_PERSON;

		tmp = path;
		tmp += ToString(i);
		tmp += ".png";

		if (ExistFile(tmp))
		{
			DebugLog("��ȡ����ͼ���䶯ͼ��" + tmp);
			_getimage_withzoom(img, tmp.c_str(), the g_block_px, the g_block_px);
			anm.figure = img;
			for (int t = 0; t < 4; t++)
			{
				PIMAGE img2 = newimage();
				tmp = path;
				tmp += ToString(i);
				tmp += "_walk" + ToString(t);
				tmp += ".png";
				_getimage_withzoom(img2, tmp.c_str(), the g_block_px, the g_block_px);
				anm.walk[t] = img2;
			}
			//the figures.push_back(img);
			the figures.push_back(anm);
		}
		else {
			if (i == 0)
			{
				ErrorLogTip("��Ȼû��һ������ͼ��", "ReadFigures");
			}
			break;
		}
		i++;
	}
}
PIMAGE GetFigure(UINT id)
{
	if (the figures.empty())
	{
		ErrorLogTip("��ô������ͼ��", "GetFigure");
		return nullptr;
	}
	return the figures.at(int(id)).figure;
}

#define PLAYER_ITEM_CNT 40	//��Ʒ����
#define PLAYER_ARMOR_CNT 4	//װ������
#define PLAYER_EQUIP_CNT 5	//��Ʒ����

class Player : public the Physics{
public:
	//Basic Info
	string name;
	UINT fig_id;	//����ID

			/*//POSITION
			float x;
			float y;

			float v;	//˲ʱ�ٶ� m/s*/
	float _speed;	//��׼�ٶ�

protected:
	//Inventory
	the Item _items[PLAYER_ITEM_CNT];	//����
	the Item _armor[PLAYER_ARMOR_CNT];	//װ��
	the Item _equip[PLAYER_EQUIP_CNT];	//��Ʒ

public:

	the Item tmpItem;	//��ʱ��Ʒ
	short lastItemSrc;	//��һ����ȡ����Ʒ���±�
	clock_t lastTmpItemChanged;

	short curItem;
	bool bagOpen;
	clock_t lastBag;	//��һ�ο�/�ر���
	
	//Values
	int hp;		//Ѫ��
	int max_hp;	//Ѫ������
	int mp;		//ħ�� 
	int max_mp;	//ħ������
	int fp;		//��ʳ 
	int max_fp;	//��ʳ���� 

	clock_t lastAttack;
	clock_t lastStep;	//����һ�͵�ʱ��(ms)

	Player()
	{
		lastItemSrc = -1;
		tmpItem.Clear();
		lastBag = clock();
		bagOpen = false;
		curItem = 0;
		fig_id = PERSON_MAN;
		name = PLAYER_DEF_NAME;
		hp = max_hp = PLAYER_DEF_MAX_HP;
		fp = max_fp = PLAYER_DEF_MAX_FP;
		mp = max_mp = PLAYER_DEF_MAX_MP;
		x = 1, y = 1;
		_speed = PLAYER_DEF_SPEED;
	}
	bool IsAlive(void) const
	{
		bool ret=true;
		if(this->hp <= 0)
			ret = false;
		return ret;
	}
#define SPEED_FLUID_RATIO 0.65f
	float speed() const
	{
		float ret = _speed;
		if (the bk(x, y, "Player::speed").GetBrr() == BRR_FLUID)
			ret *= SPEED_FLUID_RATIO;
		return ret;
	}
	void ClampHP(void)
	{
		ClampA(hp,0,max_hp);
	}
	void ClampFP(void)
	{
		ClampA(fp,0,max_fp);
	}
	void ClampMP(void)
	{
		ClampA(mp,0,max_mp);
	}
	void Clamps(void)
	{
		ClampHP();
		ClampFP();
		ClampMP();
		LimitCoord();
	}
	the Item& item(UINT index)
	{
		if (index >= PLAYER_ITEM_CNT)
		{
			ErrorLogTip("�����Ʒ�±����Υ����" + ToString(index) + " >= " + ToString(PLAYER_ITEM_CNT), "Player::item");
			exit(0);
		}
		return _items[index];
	}
	the Item& handitem()	//���ϵ���Ʒ
	{
		return _items[curItem];
	}
	the Item& preitem()
	{
		if (g_precur < 0 || g_precur >= PLAYER_ITEM_CNT)
			g_precur = -1;	//���ı��� �������
		return _items[the g_precur];
	}
	the Item& armor(UINT index)
	{
		if (index >= PLAYER_ARMOR_CNT)
		{
			ErrorLogTip("���װ���±����Υ����" + ToString(index) + " >= " + ToString(PLAYER_ARMOR_CNT), "Player::item");
			exit(0);
		}
		return _armor[index];
	}

	the Item& helmet()
	{
		return armor(0);
	}
	the Item& chestplate()
	{
		return armor(1);
	}
	the Item& leggings()
	{
		return armor(2);
	}
	the Item& boots()
	{
		return armor(3);
	}

	the Item& equip(UINT index)
	{
		if (index >= PLAYER_EQUIP_CNT)
		{
			ErrorLogTip("�����Ʒ�±����Υ����" + ToString(index) + " >= " + ToString(PLAYER_EQUIP_CNT), "Player::item");
			exit(0);
		}
		return _equip[index];
	}
	//bool IsBagAvailable(ITEM_ID id, int cnt, int* pos=nullptr);
	bool GetItem(ITEM_ID id,int cnt=1,string tag="~")
	{	//ֱ�Ӹ�����Ʒ 
		if(id == 0 || cnt < 1)
			return false;
		int maxheap = GetItemMaxHeap(id);
		for(int i=0;i<PLAYER_ITEM_CNT;++i)
		{
			if(_items[i].id == 0)
			{
				_items[i].id = id;
				_items[i].cnt = cnt;
				_items[i].dur = GetItemDur(id);
				_items[i].tag = tag;
				if(_items[i].cnt > maxheap)
				{
					_items[i].cnt = maxheap;
					cnt -= maxheap;
				}else
					break;
			}else if(_items[i].id == id)
			{
				if(_items[i].cnt >= maxheap)
					continue;
				_items[i].cnt += cnt;
				if(_items[i].cnt > maxheap)
				{
					_items[i].cnt = maxheap;
					cnt -= maxheap;
				}else
					break;
			}
		}
		if(cnt > 0)
		{
			DebugLog("������Ʒʧ�ܣ�ʣ������="+ToString(cnt));
			return false;
		}
		return true;
	}
	void ClearInventory()
	{
		for (int i = 0; i < PLAYER_ITEM_CNT; i++)
			_items[i].Clear();
		for (int i = 0; i < PLAYER_ARMOR_CNT; i++)
			_armor[i].Clear();
		for (int i = 0; i < PLAYER_EQUIP_CNT; i++)
			_equip[i].Clear();
	}

	PIMAGE GetFigure()
	{
		return the GetFigure(fig_id);
	}
	the FigureAnimation GetAnimation()
	{
		return the figures.at(fig_id);
	}
	void Init()
	{
		x = y = 0.0f;
		bagOpen = false;
		curItem = 0;
		hp = max_hp = PLAYER_DEF_MAX_HP;
		fp = max_fp = PLAYER_DEF_MAX_FP;
		mp = max_mp = PLAYER_DEF_MAX_MP;
		_speed = PLAYER_DEF_SPEED;
		own ClearInventory();
		m = PLAYER_DEF_MASS;
		name = PLAYER_DEF_NAME;
	}
	void Birth(void)
	{
		RandomSpawn();	//randSpawn
		_items[1].id = 2;
		_items[1].cnt = 13;
		_items[21].id = 3;
		_items[21].cnt = 24;
		_items[11].id = 10;
		_items[11].cnt = 1;
	}
	void LimitCoord()
	{
		if (x < 0.05)	x = 0.05;
		if (x > the g_world_w-0.05)	x = the g_world_w - 0.05;
		if (y < 0.05)  y = 0.05;
		if (y > the g_world_h-0.05)	y = the g_world_h - 0.05;
	}
	void RandomSpawn()
	{	//�������
		x = the RandomRange(20, the g_world_w - 20, true, true);
		y = the RandomRange(20, the g_world_h - 20, true, true);
	}
#define VIEW_EXTRA_BLOCKS 2.0f
	bool InView(float _x, float _y)
	{	//�Ƿ�����Ұ��
		if (_x >= this->x - uix / 2.0f / g_block_px - VIEW_EXTRA_BLOCKS
			&& _x <= this->x + uix / 2.0f / g_block_px + VIEW_EXTRA_BLOCKS
			&& _y >= this->y - uiy / 2.0f / g_block_px - VIEW_EXTRA_BLOCKS
			&& _y <= this->y + uiy / 2.0f / g_block_px + VIEW_EXTRA_BLOCKS)
			return true;
		return false;
	}
	DWORD GetStepTime()	//ms
	{
		return DWORD(1800.0f / this->Physics::v.len());
	}
	void CountStepTime()
	{
		if (v() == 0.0f)
		{
			lastStep = clock();
		}
	}
	void Draw()	//�滭�ڳ���
	{
		PIMAGE img = newimage();
		DWORD i=0;
		DWORD stepTime = GetStepTime();
		if(this->Physics::v.len() < 0.5f)
			img = GetFigure();
		else if (the IsHorizDir(this->Physics::v.Dir4()))
		{
			if (ToMilliSecond(clock() - lastStep) % stepTime > stepTime / 2)
				i = 0;
			else
				i = 1;
			img = GetAnimation().walk[i];
		}
		else if (the IsVertDir(this->Physics::v.Dir4()))
		{
			if (ToMilliSecond(clock() - lastStep) % stepTime > stepTime / 2)
				i = 2;
			else
				i = 3;
			img = GetAnimation().walk[i];
		}
		putimage_withalpha(NULL, img, uix / 2 - the g_block_px / 2, uiy / 2 - the g_block_px / 2 + g_offset);
	}
};

vector<the Player> pls{};

float CJZAPI GetPlayerX(int i)
{
	return the pls.at(i).x;
}
float CJZAPI GetPlayerY(int i)
{
	return the pls.at(i).y;
}
float CJZAPI GetPlayerHp(int i)
{
	return the pls.at(i).hp;
}
bool CJZAPI GetItem(int i,ITEM_ID id,int cnt,string tag)
{
	return the pls.at(i).GetItem(id,cnt,tag);
}

bool the Physics::SideTouch(the Vector side)	//�뷽����ײ��� 
{
	side.ClampMagnitudeA(0.501f);
	float _x = x + side.x;
	float _y = y + side.y;

	if (OuttaWorld(_x, _y))
		return true;		//��ֹ����

	if (bk(_x,_y ,"Physics::SideTouch").IsBarrier())
	{
		return true;
		//return false;	//��ǽģʽ
	}
	else
		return false;
}

void GetMouseBlockPos(int& x, int& y)
{
	//Capture first
	int mx = g_mx;
	int my = g_my;
	//������ľ���
	int dcx = mx - (uix / 2/* - the g_block_px / 2*/);	
	int dcy = my - (uiy / 2);
	//ƫ�ư�񣬺ô���
	/*if (dcx > (the g_block_px / 2))
		dcx += (the g_block_px / 2);
	else if (dcx < -(the g_block_px / 2))
		dcx -= (the g_block_px / 2);
	if (dcy > (the g_block_px / 2))
		dcy += (the g_block_px / 2);
	else if (dcy < -(the g_block_px / 2))
		dcy -= (the g_block_px / 2);*/
	//Get them
	x = self.x + dcx / the g_block_px;
	y = self.y + dcy / the g_block_px;
}
