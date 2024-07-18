//VisionaryWorld 专属头文件
//任何人不得盗用。
//Written by Rico
#pragma once

#define PLAYER_DEF_NAME "Player"//默认玩家名称
#define PLAYER_DEF_MAX_HP 100	//默认最大血量
#define PLAYER_DEF_MAX_FP 20	//默认最大饱食 
#define PLAYER_DEF_MAX_MP 0	//默认最大魔法 
#define PLAYER_DEF_SPEED 4.0f	//默认标准速度
//#define PLAYER_DEF_SPEED 40.0f	//默认标准速度
#define PLAYER_DEF_MASS 50		//kg

//fig_id
#define PERSON_MAN 0

#define FAT_NONE 0
#define FAT_PERSON 1
#define FAT_ENTITY 2

class FigureAnimation {		//形象动画
public:
	UINT type;	//形象属于什么类型

	ENTITY_ID id;
	PIMAGE figure;
	PIMAGE walk[4];	//0,1左右   2,3前后

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

vector<the FigureAnimation> figures{};	//人物形象

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
			DebugLog("获取人物图像及其动图：" + tmp);
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
				ErrorLogTip("竟然没有一个人物图像。", "ReadFigures");
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
		ErrorLogTip("还么有人物图像。", "GetFigure");
		return nullptr;
	}
	return the figures.at(int(id)).figure;
}

#define PLAYER_ITEM_CNT 40	//物品数量
#define PLAYER_ARMOR_CNT 4	//装备数量
#define PLAYER_EQUIP_CNT 5	//饰品数量

class Player : public the Physics{
public:
	//Basic Info
	string name;
	UINT fig_id;	//形象ID

			/*//POSITION
			float x;
			float y;

			float v;	//瞬时速度 m/s*/
	float _speed;	//标准速度

protected:
	//Inventory
	the Item _items[PLAYER_ITEM_CNT];	//背包
	the Item _armor[PLAYER_ARMOR_CNT];	//装甲
	the Item _equip[PLAYER_EQUIP_CNT];	//饰品

public:

	the Item tmpItem;	//临时物品
	short lastItemSrc;	//上一个拿取的物品的下标
	clock_t lastTmpItemChanged;

	short curItem;
	bool bagOpen;
	clock_t lastBag;	//上一次开/关背包
	
	//Values
	int hp;		//血量
	int max_hp;	//血量上限
	int mp;		//魔法 
	int max_mp;	//魔法上限
	int fp;		//饱食 
	int max_fp;	//饱食上限 

	clock_t lastAttack;
	clock_t lastStep;	//离上一跬的时间(ms)

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
			ErrorLogTip("玩家物品下标访问违例！" + ToString(index) + " >= " + ToString(PLAYER_ITEM_CNT), "Player::item");
			exit(0);
		}
		return _items[index];
	}
	the Item& handitem()	//手上的物品
	{
		return _items[curItem];
	}
	the Item& preitem()
	{
		if (g_precur < 0 || g_precur >= PLAYER_ITEM_CNT)
			g_precur = -1;	//最后的保险 以免崩溃
		return _items[the g_precur];
	}
	the Item& armor(UINT index)
	{
		if (index >= PLAYER_ARMOR_CNT)
		{
			ErrorLogTip("玩家装备下标访问违例！" + ToString(index) + " >= " + ToString(PLAYER_ARMOR_CNT), "Player::item");
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
			ErrorLogTip("玩家饰品下标访问违例！" + ToString(index) + " >= " + ToString(PLAYER_EQUIP_CNT), "Player::item");
			exit(0);
		}
		return _equip[index];
	}
	//bool IsBagAvailable(ITEM_ID id, int cnt, int* pos=nullptr);
	bool GetItem(ITEM_ID id,int cnt=1,string tag="~")
	{	//直接给予物品 
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
			DebugLog("给予物品失败，剩余数量="+ToString(cnt));
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
	{	//随机出生
		x = the RandomRange(20, the g_world_w - 20, true, true);
		y = the RandomRange(20, the g_world_h - 20, true, true);
	}
#define VIEW_EXTRA_BLOCKS 2.0f
	bool InView(float _x, float _y)
	{	//是否在视野中
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
	void Draw()	//绘画于场景
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

bool the Physics::SideTouch(the Vector side)	//与方块碰撞检测 
{
	side.ClampMagnitudeA(0.501f);
	float _x = x + side.x;
	float _y = y + side.y;

	if (OuttaWorld(_x, _y))
		return true;		//防止崩溃

	if (bk(_x,_y ,"Physics::SideTouch").IsBarrier())
	{
		return true;
		//return false;	//穿墙模式
	}
	else
		return false;
}

void GetMouseBlockPos(int& x, int& y)
{
	//Capture first
	int mx = g_mx;
	int my = g_my;
	//与中央的距离
	int dcx = mx - (uix / 2/* - the g_block_px / 2*/);	
	int dcy = my - (uiy / 2);
	//偏移半格，好处理
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
