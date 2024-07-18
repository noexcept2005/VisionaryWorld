//VisionaryWorld ר��ͷ�ļ�
//�κ��˲��õ��á�
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
class ItemData {	//��Ʒ���� �Ա���Ԫ
public:
	ITEM_ID id;
	string name;
	string eng_id;	//Ӣ��ID

	UINT max_heap;	//���ѵ�
	BYTE type;		//��Ʒ����

	union {		//���Ի�����

		struct {	//����
			BLOCK_ID block;
		}_block;	

		struct {	//��ս����
			DWORD damage;
			UINT singVarn;
			DWORD cd;
			short dur;
			float momentum;	//����
		}_blade;

		struct {	//��
			DWORD damage;
			int power;		//����
			int diglvl;		//�ھ�ȵ�
			UINT singVarn;
			DWORD cd;
			short dur;
			float momentum;	//����
		}_pickaxe;

		struct {	//��
			DWORD damage;
			int power;		//����
			UINT singVarn;
			DWORD cd;
			short dur;
			float momentum;	//����
		}_axe;

		struct {	//Զ������
			DWORD damage;
			UINT singVarn;
			DWORD cd;	//������ȴ
			short dur;
			ENTITY_ID proj;	//��ҩID
			DWORD proj_move_cd;	//��ҩ������ȴ
			float momentum;	//����
		}_far;

	}u;

	PIMAGE image;	//ͼ��

	ItemData()
	{
		id = 0;
		name = "δ������Ʒ";
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
		the itemdata.clear();	//��ֹ�쳣

	DebugLog("����ȡ����ȡ��Ʒ�����ļ���:" + ITEMDATA_PATH);
	fstream fin(ITEMDATA_PATH, ios::in);
	if (fin.fail())
	{
		ErrorLogTip("Ѱ�ٲ�����Ʒ�����ļ�", "ReadItemData");
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
		line = ResplitLine(line, ',', ' ');	//�Կո��ٴηָ�

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
			//DebugLog("<!> ������Ч����Ʒ���ͣ�");
		}

		idt.GetImage();	//ͼƬ֮��ȡ

		the itemdata.push_back(idt);	//����
		
		DebugLog("id="+ToString(idt.id)+" name="+the GetItemName(idt.id));
		
		i++;
	}
	fin.close();
}

#define GETITEM_CHECK  if(id >= vis::itemdata.size())	\
						{									\
							ErrorTip(NULL, "��Ʒ���ݷ���Υ��", "VisionaryWorld ERROR");		\
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
		DebugLog("<����> ��ͼ��ȡ���˺���Ʒ������Ʒ���˺���");
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
		DebugLog("<����> ��ͼ��ȡ���˺���Ʒ������Ʒ���˺����򲨶���");
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
		DebugLog("<����> ��ͼ��ȡ���;���Ʒ���;öȣ�");
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
		DebugLog("<����> ��ͼ��ȡ���˺���Ʒ������Ʒ�ĳ�����");
		return 0;
	}
}
DWORD GetItemBlock(ITEM_ID id)
{
	GETITEM_CHECK;
	return the itemdata.at(int(id)).u._block.block;
}
ITEM_ID BlockToItem(BLOCK_ID id)	//����IDת��ƷID 
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
		DebugLog("<����> ��ͼ��ȡ���˺���Ʒ������Ʒ����ȴʱ�䣡");
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
		DebugLog("<����> ��ͼ��ȡ��������Ʒ���͵���Ʒ���ھ�/��������");
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
		DebugLog("<����> ��ͼ��ȡ�Ǹ�����Ʒ���ھ�ȼ���");
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
	//<!>ITT_PUT �д���ȶ
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
{	//<!>���ڿ��ܻ�����ͷ֮��.
	BYTE type = the GetItemType(id);
	return (type == ITT_PICKAXE);
}
bool IsAxeItem(ITEM_ID id)
{	//<!>���ڿ��ܻ��о���֮��.
	BYTE type = the GetItemType(id);
	return (type == ITT_AXE);
}
bool IsWoodenItem(ITEM_ID id)	//�Ƿ���ľ����Ʒ 
{	//<!> �߼��㷨 
	string s = the GetItemName(id);
	if(s.find("ľ") != string::npos || s.find("��") != string::npos)
		return true;
	return false;
}

class Item {	//��Ʒ

public:
	ITEM_ID id;
	INT cnt;	//����
	short dur;	//�;�
	string tag;	//���ݱ�ǩ

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
	string GetRealDescription()	//��ȡ��Ʒ��ǩ��������
	{
		Normalize();
		BYTE type = the GetItemType(id);
		stringstream ss;
		ss << GetName();
			//ss << " (#" << id << ")" << endl;	//ID
		if (cnt > 1)
			ss << " (" << cnt << ")" << endl;		//����
		else
			ss << endl;
		if (the IsDamagableItemType(type))
		{
			ss << "�����˺���" << the GetItemDamage(id) << endl;
			ss << "�;�ֵ��" << dur << " / " << the GetItemDur(id) << endl;
			ss << "��������" << the GetItemMomenum(id) << endl;
			ss << "������ȴ��" << the GetItemCD(id) << "ms" << endl;
		}
		if (type == ITT_PICKAXE)
		{
			ss << "�ھ�ȼ���" << the GetItemDigLevel(id) << endl;
			ss << "�ھ�����" << the GetItemPower(id) << endl;
		}else if (type == ITT_AXE)
		{
			ss << "��������" << the GetItemPower(id) << endl;
		}
		else if (type == ITT_FAR)
		{
			//ENTITY_ID proj = the GetItemProj(id);
			//if(id != ARROW && id != BULLET)	//<!>����ȫ
			ss << "ʹ�õ�ҩ��" << the GetItemName(the GetItemProj(id)) << endl;
		}
		return ss.str();
	}
	void Normalize()	//������
	{
		if (id == 0)		//�ռ��ǿ�
		{
			cnt = 0, tag = "~";
		}else if (cnt == 0)	//���˼��ǿ���
		{
			id = 0, tag = "~";
		}
		else if (cnt > GetMaxHeap())
			cnt = GetMaxHeap();

		if (dur <= 0)	//����
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
		return (id == sec.id && dur == sec.dur && tag == sec.tag);	//cnt���ܵ� 
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
