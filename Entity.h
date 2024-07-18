//VisionaryWorld 专属头文件
//任何人不得盗用。
//Written by Rico
#pragma once

typedef UINT ENTITY_ID;

//Entity Types
#define ETT_NONE '_'
#define ETT_BIGPLANT 'P'
#define ETT_SMALLPLANT 'p'
#define ETT_OBJECT 'O'
#define ETT_ANIMAL 'a'
#define ETT_MONSTER 'm'
#define ETT_PROJ '>'

class EntityData {
public:
	ENTITY_ID id;
	string name;

	UINT img_cnt;
	vector<PIMAGE> imgs;
	BYTE type;

	EntityData()
	{
		id = 0, name = "默认实体";
		img_cnt = 1, type = ETT_NONE;
	}
	void GetImages()
	{
		if (type == ETT_NONE || id == 0 || img_cnt < 1)
			return;
		if (!imgs.empty())	imgs.clear();

		string path;
		path = ASSETS_PATH;
		path += "Images\\";
		path += "Entity_";
		string tmp;
		for (int i = 0; i < img_cnt; i++)
		{
			tmp = path;
			tmp += ToString(int(id));
			if (i != 0)
			{
				tmp += "_";
				tmp += ToString(i);	//e.g.  Block_1_1.png
			}
			tmp += ".png";
			PIMAGE img = newimage();
			//_getimage_withzoom(img, tmp.c_str(), the g_block_px, the g_block_px);
			_getimage(img, tmp.c_str());

			imgs.push_back(img);
		}
	}
};
#define ENTITYDATA_PATH ASSETS_PATH+"Data\\EntityData.csv"
vector<the EntityData> entityData{};

void ReadEntityData()
{
	if (!the entityData.empty())
		the entityData.clear();	//防止异常

	DebugLog("【读取】读取实体数据文件于:" + ENTITYDATA_PATH);
	fstream fin(ENTITYDATA_PATH, ios::in);
	if (fin.fail())
	{
		ErrorLogTip("寻觅不得实体数据文件", "ReadEntityData");
		return;
	}
	string line;
	stringstream ss;
	vector<string> lines = ReadFileLines(ENTITYDATA_PATH);
	int i = 1;
	while (i < lines.size())
	{
		if (fin.eof())	break;
		ss.str("");

		line = lines.at(i);
		line = ResplitLine(line, ',', ' ');	//以空格再次分隔

		ss << line;
		DebugLog(line, false);

		the EntityData edt;
		ss >> edt.id;
		ss >> edt.name;
		ss >> edt.img_cnt;
		ss >> edt.type;

		edt.GetImages();

		the entityData.push_back(edt);	//推入
		i++;
	}
	fin.close();
}

#define GETENTITY_CHECK  if(id >= vis::entityData.size())	\
						{									\
							ErrorTip(NULL, "实体数据访问违例", "VisionaryWorld ERROR");		\
						}__noop()
PIMAGE GetEntityImage(ENTITY_ID id,UINT index=0)
{
	GETENTITY_CHECK;
	return the entityData.at(int(id)).imgs.at(index);
}

class Entity : public the Physics {		//实体
public:
	//float m,v,x,y;

	ENTITY_ID id;
	float hp;		//血量
	float max_hp;	//血量上限

	Entity()
	{
		x = 1, y = 1, id = 0;
	}
	void SetPos(float _x, float _y)
	{
		x = _x, y = _y;
	}
	virtual void Draw(int index=0, PIMAGE dst = (PIMAGE)0)
	{	//↓y  →x

		if (id == 0)
			return;
		//Get Image
		PIMAGE img = newimage();
		img = the GetEntityImage(id,index);
		if (img == nullptr)	return;

		//Coord Calculation
		int _x, _y;
		_x = uix / 2 - the g_block_px * (the GetPlayerX(0) - x);
		_y = uiy / 2 - the g_block_px * (the GetPlayerY(0) - y);

		putimage_withalpha(dst, img, _x, _y);
	}
};
/*
class Creature : public the Entity, the Physics {
public:

	//ENTITY_ID Entity::id;
	//float   Physics::x,y;

	Creature()
	{
		Entity::id = 0;
		Physics::x = Physics::y = 1.0f;
		Physics::m = 1.0f;
	}
};*/

vector<the Entity> entities{};
