//VisionaryWorld 专属头文件
//任何人不得盗用。
//Written by Rico
#pragma once

string GetItemName(ITEM_ID id);

#define ITT_NONE '0'
#define ITT_MATERIAL 'm'
#define ITT_BLOCK 'B'
#define ITT_BLADE '|'
#define ITT_AXE '%'
#define ITT_PICKAXE '<'
#define ITT_FAR 'f'
#define ITT_PUT '$'
#define ITT_ARMOR 'a'

#define DEF_MAX_HEAP 99
#define DEF_REACH_DISTANCE 3.0f
#define DEF_REACH_MIN_DISTANCE 1.0f
#define ITEMBLOCK_CD 200
class ItemData {	//物品数据 以便享元
public:
	ITEM_ID id;
	string name;
	string eng_id;	//英文ID

	UINT max_heap;	//最大堆叠
	BYTE type;		//物品类型

	union {		//个性化属性

		struct {	//方块
			BLOCK_ID block;
		}_block;	

		struct {	//近战刀具
			DWORD damage;
			UINT singVarn;
			DWORD cd;
			short dur;
			float momentum;	//冲量
		}_blade;

		struct {	//镐
			DWORD damage;
			int power;		//镐力
			int diglvl;		//挖掘等第
			UINT singVarn;
			DWORD cd;
			short dur;
			float momentum;	//冲量
		}_pickaxe;

		struct {	//斧
			DWORD damage;
			int power;		//斧力
			UINT singVarn;
			DWORD cd;
			short dur;
			float momentum;	//冲量
		}_axe;

		struct {	//远程武器
			DWORD damage;
			UINT singVarn;
			DWORD cd;	//攻击冷却
			short dur;
			ENTITY_ID proj;	//弹药ID
			DWORD proj_move_cd;	//弹药飞行冷却
			float momentum;	//冲量
		}_far;

	}u;

	PIMAGE image;	//图像

	ItemData()
	{
		id = 0;
		name = "未定义物品";
		eng_id = "item";
		max_heap = DEF_MAX_HEAP;
		type = ITT_NONE;
		image = newimage();
	}
	#define BLOCK_SLOT_RATIO 0.3f	//15 / 50
	void GetImage()
	{
		if (type == ITT_NONE || id == 0)
			return;
		bool from_block = false;
		string path = ASSETS_PATH;
		path += "Images\\";
		path += "Item_";
		path += ToString(int(id));
		path += ".png";
		if(type == ITT_BLOCK && !ExistFile(path))
		{
			from_block = true;
			path = ASSETS_PATH;
			path += "Images\\";
			path += "Block_";
			path += ToString(int(u._block.block));
			path += ".png";
		}
		if(!from_block)
			_getimage(image, path.c_str());
		else
			_getimage_withzoom(image, path.c_str(), the g_slot_px * BLOCK_SLOT_RATIO, the g_slot_px * BLOCK_SLOT_RATIO);
		//_getimage_withzoom(image, path.c_str(), the g_slot_px, the g_slot_px);
	}
};
#define ITEMDATA_PATH ASSETS_PATH+"Data\\ItemData.csv"
vector<the ItemData> itemdata{};

void ReadItemData()
{
	if (!the itemdata.empty())
		the itemdata.clear();	//防止异常

	DebugLog("【读取】读取物品数据文件于:" + ITEMDATA_PATH);
	fstream fin(ITEMDATA_PATH, ios::in);
	if (fin.fail())
	{
		ErrorLogTip("寻觅不得物品数据文件", "ReadItemData");
		return;
	}
	string line;
	vector<string> lines = ReadFileLines(ITEMDATA_PATH);
	int i = 1;
	while (i < lines.size())
	{
		if (fin.eof())	break;
		stringstream ss;

		line = lines.at(i);
		line = ResplitLine(line, ',', ' ');	//以空格再次分隔

		ss << line;
		DebugLog(line, false);

		the ItemData idt;
		/*UINT utmp;
		ss >> utmp;
		idt.id = utmp;
		DebugLog("utmp="+ToString(utmp)+" id="+ToString(idt.id));
		string stmp;
		ss >> stmp;
		idt.name = stmp;
		DebugLog("stmp="+ToString(stmp)+" name="+ToString(idt.name));*/
		ss >> idt.id;
		ss >> idt.name;
		ss >> idt.eng_id;
		ss >> idt.max_heap;
		ss >> idt.type;

		if (idt.type == ITT_BLOCK)
		{
			ss >> idt.u._block.block;
		}
		else if (idt.type == ITT_BLADE)
		{
			ss >> idt.u._blade.damage;
			ss >> idt.u._blade.singVarn;
			ss >> idt.u._blade.dur;
			ss >> idt.u._blade.cd;
			ss >> idt.u._blade.momentum;
		}
		else if (idt.type == ITT_AXE)
		{
			ss >> idt.u._axe.damage;
			ss >> idt.u._axe.singVarn;
			ss >> idt.u._axe.dur;
			ss >> idt.u._axe.cd;
			ss >> idt.u._axe.momentum;
			ss >> idt.u._axe.power;
		}
		else if (idt.type == ITT_PICKAXE)
		{
			ss >> idt.u._pickaxe.damage;
			ss >> idt.u._pickaxe.singVarn;
			ss >> idt.u._pickaxe.dur;
			ss >> idt.u._pickaxe.cd;
			ss >> idt.u._pickaxe.momentum;
			ss >> idt.u._pickaxe.power;
			ss >> idt.u._pickaxe.diglvl;
		}
		else if (idt.type == ITT_FAR)
		{
			ss >> idt.u._far.damage;
			ss >> idt.u._far.singVarn;
			ss >> idt.u._far.dur;
			ss >> idt.u._far.cd;
			ss >> idt.u._far.momentum;
			ss >> idt.u._far.proj;
			ss >> idt.u._far.proj_move_cd;
		}
		else {
			//DebugLog("<!> 错误：无效的物品类型！");
		}

		idt.GetImage();	//图片之获取

		the itemdata.push_back(idt);	//推入
		
		DebugLog("id="+ToString(idt.id)+" name="+the GetItemName(idt.id));
		
		i++;
	}
	fin.close();
}

#define GETITEM_CHECK  if(id >= vis::itemdata.size())	\
						{									\
							ErrorTip(NULL, "物品数据访问违例", "VisionaryWorld ERROR");		\
						}__noop()
PIMAGE GetItemImage(ITEM_ID id)
{
	GETITEM_CHECK;
	return the itemdata.at(int(id)).image;
}
UINT GetItemMaxHeap(ITEM_ID id)
{
	GETITEM_CHECK;
	return the itemdata.at(int(id)).max_heap;
}
string GetItemName(ITEM_ID id)
{
	GETITEM_CHECK;
	return the itemdata.at(int(id)).name;
}
string GetItemEnglish(ITEM_ID id)
{
	GETITEM_CHECK;
	return the itemdata.at(int(id)).eng_id;
}
BYTE GetItemType(ITEM_ID id)
{
	GETITEM_CHECK;
	return the itemdata.at(int(id)).type;
}
DWORD GetItemDamage(ITEM_ID id)
{
	GETITEM_CHECK;
	BYTE type = the GetItemType(id);
	if (type == ITT_BLADE)
		return the itemdata.at(int(id)).u._blade.damage;
	else if (type == ITT_AXE)
		return the itemdata.at(int(id)).u._axe.damage;
	else if (type == ITT_PICKAXE)
		return the itemdata.at(int(id)).u._pickaxe.damage;
	else if (type == ITT_FAR)
		return the itemdata.at(int(id)).u._far.damage;
	else {
		DebugLog("<错误> 试图获取非伤害物品类型物品的伤害！");
		return 0;
	}
}
DWORD GetItemSingVarn(ITEM_ID id)
{
	GETITEM_CHECK;
	BYTE type = the GetItemType(id);
	if (type == ITT_BLADE)
		return the itemdata.at(int(id)).u._blade.singVarn;
	else if (type == ITT_AXE)
		return the itemdata.at(int(id)).u._axe.singVarn;
	else if (type == ITT_PICKAXE)
		return the itemdata.at(int(id)).u._pickaxe.singVarn;
	else if (type == ITT_FAR)
		return the itemdata.at(int(id)).u._far.singVarn;
	else {
		DebugLog("<错误> 试图获取非伤害物品类型物品的伤害单向波动！");
		return 99999;
	}
}
short GetItemDur(ITEM_ID id)
{
	GETITEM_CHECK;
	BYTE type = the GetItemType(id);
	if (type == ITT_BLADE)
		return the itemdata.at(int(id)).u._blade.dur;
	else if (type == ITT_AXE)
		return the itemdata.at(int(id)).u._axe.dur;
	else if (type == ITT_PICKAXE)
		return the itemdata.at(int(id)).u._pickaxe.dur;
	else if (type == ITT_FAR)
		return the itemdata.at(int(id)).u._far.dur;
	else {
		DebugLog("<错误> 试图获取无耐久物品的耐久度！");
		return 1;
	}
}
float GetItemMomenum(ITEM_ID id)
{
	GETITEM_CHECK;
	BYTE type = the GetItemType(id);
	if (type == ITT_BLADE)
		return the itemdata.at(int(id)).u._blade.momentum;
	else if (type == ITT_AXE)
		return the itemdata.at(int(id)).u._axe.momentum;
	else if (type == ITT_PICKAXE)
		return the itemdata.at(int(id)).u._pickaxe.momentum;
	else if (type == ITT_FAR)
		return the itemdata.at(int(id)).u._far.momentum;
	else {
		DebugLog("<错误> 试图获取非伤害物品类型物品的冲量！");
		return 0;
	}
}
DWORD GetItemBlock(ITEM_ID id)
{
	GETITEM_CHECK;
	return the itemdata.at(int(id)).u._block.block;
}
ITEM_ID BlockToItem(BLOCK_ID id)	//方块ID转物品ID 
{
	for(int i=0;i<the itemdata.size();++i)
		if(the itemdata.at(i).type == ITT_BLOCK && the itemdata.at(i).u._block.block == id)
			return the itemdata.at(i).id;
	return ITEM_ID(0);
}
DWORD GetItemCD(ITEM_ID id)
{
	GETITEM_CHECK;
	BYTE type = the GetItemType(id);
	if (type == ITT_BLADE)
		return the itemdata.at(int(id)).u._blade.cd;
	else if (type == ITT_AXE)
		return the itemdata.at(int(id)).u._axe.cd;
	else if (type == ITT_PICKAXE)
		return the itemdata.at(int(id)).u._pickaxe.cd;
	else if (type == ITT_FAR)
		return the itemdata.at(int(id)).u._far.cd;
	else if(type == ITT_BLOCK)
		return ITEMBLOCK_CD;
	else {
		DebugLog("<错误> 试图获取非伤害物品类型物品的冷却时间！");
		return 99999;
	}
}
int GetItemPower(ITEM_ID id)
{
	GETITEM_CHECK;
	BYTE type = the GetItemType(id);
	if(type == ITT_AXE)
		return the itemdata.at(int(id)).u._axe.power;
	else if(type == ITT_PICKAXE)
		return the itemdata.at(int(id)).u._pickaxe.power;
	else {
		DebugLog("<错误> 试图获取无力度物品类型的物品的挖掘/劈砍力！");
		return 0;
	}
}
int GetItemDigLevel(ITEM_ID id)
{
	GETITEM_CHECK;
	BYTE type = the GetItemType(id);
	if (type == ITT_PICKAXE)
		return the itemdata.at(int(id)).u._pickaxe.diglvl;
	else {
		DebugLog("<错误> 试图获取非镐子物品的挖掘等级！");
		return 0;
	}
}
the ENTITY_ID GetItemProj(ITEM_ID id)
{
	GETITEM_CHECK;
	return the itemdata.at(int(id)).u._far.proj;
}
DWORD GetItemProjMoveCD(ITEM_ID id)
{
	GETITEM_CHECK;
	return the itemdata.at(int(id)).u._far.proj_move_cd;
}
bool IsDamagableItemType(BYTE type)
{
	return (type == ITT_AXE || type == ITT_BLADE || type == ITT_FAR || type == ITT_PICKAXE);
	//<!>ITT_PUT 有待商榷
}
bool IsWeaponItem(ITEM_ID id)
{
	BYTE type = the GetItemType(id);
	return the IsDamagableItemType(type);
}
bool IsBlockItem(ITEM_ID id)
{
	BYTE type = the GetItemType(id);
	return (type == ITT_BLOCK);
}
bool IsPickaxeItem(ITEM_ID id)
{	//<!>后期可能还有钻头之类.
	BYTE type = the GetItemType(id);
	return (type == ITT_PICKAXE);
}
bool IsAxeItem(ITEM_ID id)
{	//<!>后期可能还有锯子之类.
	BYTE type = the GetItemType(id);
	return (type == ITT_AXE);
}
bool IsWoodenItem(ITEM_ID id)	//是否是木制物品 
{	//<!> 高级算法 
	string s = the GetItemName(id);
	if(s.find("木") != string::npos || s.find("树") != string::npos)
		return true;
	return false;
}

class Item {	//物品

public:
	ITEM_ID id;
	INT cnt;	//数量
	short dur;	//耐久
	string tag;	//数据标签

	Item()
	{
		id = 0, cnt = 0;
		dur = 0;
		tag = "~";
	}
	Item(ITEM_ID _id, INT _cnt) : id{ _id }, cnt{ _cnt } 
	{ tag = "~";  }

	void Clear()
	{
		id = 0;
		cnt = 0;
		tag = "~";
		dur = 0;
	}
	UINT GetMaxHeap() const
	{
		return the GetItemMaxHeap(id);
	}
	string GetName() const 
	{
		return the GetItemName(id);
	}
	short GetDur() const
	{
		return the GetItemDur(id);
	}
	bool IsBroken()
	{
		if (dur <= 0)
		{
			dur = 0;
			return true;
		}
		return false;
	}
	string GetRealDescription()	//获取物品标签所有内容
	{
		Normalize();
		BYTE type = the GetItemType(id);
		stringstream ss;
		ss << GetName();
			//ss << " (#" << id << ")" << endl;	//ID
		if (cnt > 1)
			ss << " (" << cnt << ")" << endl;		//数量
		else
			ss << endl;
		if (the IsDamagableItemType(type))
		{
			ss << "攻击伤害：" << the GetItemDamage(id) << endl;
			ss << "耐久值：" << dur << " / " << the GetItemDur(id) << endl;
			ss << "击退力：" << the GetItemMomenum(id) << endl;
			ss << "攻击冷却：" << the GetItemCD(id) << "ms" << endl;
		}
		if (type == ITT_PICKAXE)
		{
			ss << "挖掘等级：" << the GetItemDigLevel(id) << endl;
			ss << "挖掘力：" << the GetItemPower(id) << endl;
		}else if (type == ITT_AXE)
		{
			ss << "劈砍力：" << the GetItemPower(id) << endl;
		}
		else if (type == ITT_FAR)
		{
			//ENTITY_ID proj = the GetItemProj(id);
			//if(id != ARROW && id != BULLET)	//<!>待补全
			ss << "使用弹药：" << the GetItemName(the GetItemProj(id)) << endl;
		}
		return ss.str();
	}
	void Normalize()	//正常化
	{
		if (id == 0)		//空即是空
		{
			cnt = 0, tag = "~";
		}else if (cnt == 0)	//空了即是空了
		{
			id = 0, tag = "~";
		}
		else if (cnt > GetMaxHeap())
			cnt = GetMaxHeap();

		if (dur <= 0)	//报废
		{
			id = 0, cnt = 0, tag = "~";
		}
	}
	bool IsFull()
	{
		Normalize();
		return (cnt >= GetMaxHeap());
	}
	bool IsEmpty()
	{
		Normalize();
		return (id == 0);
	}
	void Add(UINT addCnt = 1)
	{
		cnt += addCnt;
		Normalize();
	}
	void Minus(UINT minusCnt = 1)
	{
		cnt -= minusCnt;
		Normalize();
	}

	bool operator == (Item& sec) const
	{
		return (id == sec.id && dur == sec.dur && tag == sec.tag);	//cnt不管的 
	}
	Item& operator = (Item& src)
	{
		id = src.id;
		cnt = src.cnt;
		dur = src.dur;
		tag = src.tag;
		return (*this);	//necessary returning
	}
};

//Useful macros
#define SAME_ITEM(a,b) (a==b)
#define SAME_ITEM_IGNORE_DUR(a,b) (a.id==b.id&&a.tag==b.tag)
#define SAME_ITEM_T(a,b) (a.id==b.id)
#define SWAP_ITEM(a,b) do{Item _tmp=a;a=b;b=_tmp;}while(0)
