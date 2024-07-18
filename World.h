//VisionaryWorld 专属头文件
//任何人不得盗用。
//Written by Rico
#pragma once

#define BIOME_NULL 0x00
#define BIOME_OVERWORLD 0x01
#define BIOME_GRASS 0x02
#define BIOME_CLIFF 0x03
#define BIOME_OCEAN 0x04
#define BIOME_RIVER 0x05
#define BIOME_DESERT 0x06
#define BIOME_ICELAND 0x07
#define BIOME_JUNGLE 0x08
#define BIOME_MOOR 0x09
#define BIOME_LAKE 0x0A

bool IsWaterBiome(BIOME_ID biome)
{
	return biome == BIOME_LAKE || biome == BIOME_RIVER || biome == BIOME_OCEAN;
}

#define BRR_AIR 0x00		//空
#define BRR_THROUGH 0x01	//可穿过
#define BRR_BLOCK 0x02		//隔档的方块
#define BRR_FLUID 0x03		//流体

#define BLOCK_DEF_PX 50

class BlockData {
public:
	BLOCK_ID id;
	size_t id2_cnt;

	string name;
	string eng_id;	//英文ID

	UINT brr;	//障碍方式
	int hardness;	//硬度

	vector<PIMAGE> images;

	int dur;	//点数

	BlockData()
	{
		hardness = 99;
		id = 0, dur = 99, id2_cnt=99;
		brr = BRR_AIR;
		name = "方块";
		eng_id = "block";
	}
	void GetImages()
	{
		if (id <= 0 || id2_cnt <= 0)	//剪枝
			return;

		string path;
		path = ASSETS_PATH;
		path += "Images\\";
		path += "Block_";
		string tmp;
		for (int i = 0; i < id2_cnt; i++)
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
			DebugLog(tmp,false);
			_getimage_withzoom(img, tmp.c_str(), the g_block_px, the g_block_px);

			images.push_back(img);
		}
	}
};
#define BLOCKDATA_PATH ASSETS_PATH+"Data\\BlockData.csv"
vector<the BlockData> blockData{};

void ReadBlockData()
{
	if (!the blockData.empty())
		the blockData.clear();	//防止异常

	DebugLog("【读取】读取方块数据文件于:" + BLOCKDATA_PATH);
	fstream fin(BLOCKDATA_PATH, ios::in);
	if (fin.fail())
	{
		ErrorLogTip("寻觅不得方块数据文件", "ReadBlockData");
		return;
	}
	vector<string> lines = ReadFileLines(BLOCKDATA_PATH);
	int i = 1;
	while (i < lines.size())
	{
		if (fin.eof())	break;
		string line;
		stringstream ss;

		line = lines.at(i);
		if (line.empty())	break;
		line = ResplitLine(line, ',', ' ');	//以空格再次分隔

		ss << line;
		DebugLog(line, false);

		the BlockData bdt;
		ss >> bdt.id;
		ss >> bdt.name;
		ss >> bdt.eng_id;
		ss >> bdt.id2_cnt;

		//nmd

		int tmp;
		ss >> tmp;
		//DebugLog(ToString(tmp));
		bdt.brr = tmp;
		ss >> tmp;
		//DebugLog(ToString(tmp));
		bdt.dur = tmp;
		ss >> tmp;
		//DebugLog(ToString(tmp));
		bdt.hardness = tmp;
		//DebugLog("id2cnt=" + ToString(bdt.id2_cnt) + "brr=" + ToString(bdt.brr) + "dur=" + ToString(bdt.dur) + "hard=" + ToString(bdt.hardness));
		bdt.GetImages();

		the blockData.push_back(bdt);	//推入
		i++;
	}
	fin.close();
}

#define GETBLOCK_CHECK  if(id >= vis::blockData.size())	\
						{									\
							ErrorTip(NULL, "方块数据访问违例", "VisionaryWorld ERROR");		\
						}__noop()
int GetBlockDur(BLOCK_ID id)
{
	GETBLOCK_CHECK;
	return the blockData.at(int(id)).dur;
}
size_t GetBlockId2Cnt(BLOCK_ID id)
{
	GETBLOCK_CHECK;
	return the blockData.at(int(id)).id2_cnt;
}
string GetBlockName(BLOCK_ID id)
{
	GETBLOCK_CHECK;
	return the blockData.at(int(id)).name;
}
string GetBlockEnglish(BLOCK_ID id)
{
	GETBLOCK_CHECK;
	return the blockData.at(int(id)).eng_id;
}
BYTE GetBlockBrr(BLOCK_ID id)
{
	GETBLOCK_CHECK;
	return the blockData.at(int(id)).brr;
}
PIMAGE GetBlockImage(BLOCK_ID id, int index = 0)
{
	EXCEPTION_L
	GETBLOCK_CHECK;
	if (index >= the blockData.at(int(id)).images.size())
	{
		ErrorLogTip("方块图片数量不足：" 
			+ ToString(index) + "<=" + ToString(the blockData.at(int(id)).images.size())
			+ "; id = " + ToString(id),
			"GetBlockImage");
	}
	return the blockData.at(int(id)).images.at(index);
	EXCEPTION_R
}
bool IsWoodenBlock(BLOCK_ID id)	//是否是木制方块 
{	//<!> 高级算法 
	string s = the GetBlockName(id);
	if(s.find("木") != string::npos || s.find("树") != string::npos)
		return true;
	return false;
}
bool IsStoneBlock(BLOCK_ID id)	//是否是木制方块 
{	//<!> 高级算法 
	string s = the GetBlockName(id);
	if(s.find("石") != string::npos || s.find("岩") != string::npos)
		return true;
	return false;
}
bool IsTinyBlock(BLOCK_ID id)
{
	GETBLOCK_CHECK;
	if(id == 11)
		return true;
	return false;
}

struct Block {	//方块数据

	BLOCK_ID id;
	BLOCK_ID id2;
	BIOME_ID biome;
	int dur;	//耐久/损害值

	Block(BLOCK_ID _id, BLOCK_ID _id2=0,int _dur=-1)
	{
		id = _id, id2 = _id2, dur = _dur;
		biome = BIOME_OVERWORLD;
		if (dur < 0)	//自动
			dur = the GetBlockDur(id);
	}
	Block()
	{
		biome = BIOME_OVERWORLD;
		id = 0, id2 = 0, dur = 0;
	}
	void Normalize(void)	//去除不正常化
	{
		if (id == 0)	//air
		{
			id2 = 0, dur = 0;
		}
		else if (dur <= 0)	//broken
		{
			this->Drop();
			this->Clear();
		}
	}
	void RandomId2()
	{
		int id2_cnt = the GetBlockId2Cnt(id);
		if (id2_cnt <= 0)
			return;
		else if (id2_cnt == 1)
			id2 = 0;
		else {
			id2 = the RandomRange(0, id2_cnt, true, false);
		}
	}
	void RandomOtherId2()
	{
		int id2_cnt = the GetBlockId2Cnt(id);
		if (id2_cnt <= 0 || id2_cnt == 1)
			return;
		else {
			BLOCK_ID pre = id2;
			do {
				pre = the RandomRange(0, id2_cnt, true, false);
			} while (pre == id2);
			id2 = pre;
		}
	}
	void FitDur()
	{
		dur = the GetBlockDur(id);
	}
	void Clear()
	{
		id = 0, id2 = 0, dur = 0;
		biome = BIOME_OVERWORLD;
	}
	void Drop()	//掉落 
	{
		if(id == 5)
		{
			GetItem(0,3,RandomRange(1,4));
		}else if(id == 4)
		{
			GetItem(0,3,RandomRange(2,4));
		}else if(id == 1 || id == 2 || id == 3 || id == 13)
			GetItem(0,BlockToItem(id),1);
		else if(id == 11 || id == 12)
			return;	//grass not dropping
		else{	//Auto drop Block's Item, probably fail
			DebugLog("[警告] Block::Drop: 未定义的方块掉落！bid="+ToString(id));
			GetItem(0,BlockToItem(id),1);
		}
	}
	BYTE GetBrr() const
	{
		return the GetBlockBrr(id);
	}
	bool IsBarrier() noexcept
	{
		return GetBrr() == BRR_BLOCK;
	}
	void Draw(float x,float y,PIMAGE dst=(PIMAGE)0) const
	{		//↓y  →x
		EXCEPTION_L
		if (id == 0)
			return;
		
		//Get Image
		PIMAGE img = newimage();
		img = the GetBlockImage(id, id2);
		if (img == nullptr)	return;

		
		//Coord Calculation
		int _x, _y;
		_x = uix / 2 - the g_block_px * (the GetPlayerX(0) - x);
		_y = uiy / 2 - the g_block_px * (the GetPlayerY(0) - y) + g_offset;
		
		/*if (x == GetPlayerX(0) && y == GetPlayerY(0))
		{
			setfont(20, 0, "新宋体");
			setcolor(AQUA);
			xyprintf(_x + 2, _y + 2, "这");
		}*/
		//DebugLog("x=" + ToString(x) + "y=" + ToString(y) + " id=" + ToString(id)
		//+"_x="+ToString(_x)+"_y="+ToString(_y)+ "px=" + ToString(the GetPlayerX(0)) + "py=" + ToString(the GetPlayerY(0)));
		//Put Image
		putimage_withalpha(dst, img, _x, _y);

		/*setfont(15, 0, "新宋体");
		setcolor(BLACK);
		xyprintf(_x, _y + 2, "%d", (int)y);*/
		EXCEPTION_R
	}
};

vector<the Block> blocks{};

bool OuttaWorld(float x, float y)	//是否出界
{
	EXCEPTION_L

	if (x < 0 || y < 0 || x >= g_world_w || y >= g_world_h)
		return true;
	else if (y * g_world_w + x >= the blocks.size())
		return true;
	return false;

	EXCEPTION_R
}

the Block& bk(int x, int y,string tag="notag")	//方块引用
{
	EXCEPTION_L
	         /*(the g_world_h - 1 - y)*/
	int index = y * the g_world_w + x;
	if (the blocks.empty() || the blocks.size() <= index)
	{
		ErrorLogTip("方块访问违例：x=" + ToString(x) + ",y=" + ToString(y) 
			+ "\nworld_w = " + ToString(the g_world_w) + " world_h = " + ToString(the g_world_h),
			"bk："+tag);
		exit(0);
	}
	return the blocks.at(index);

	EXCEPTION_R
}
the Block& SetBlock(float x, float y, BLOCK_ID id,BIOME_ID biome = BIOME_NULL)
{
	the bk(x, y, "SetBlock").id = id;
	the bk(x, y, "SetBlock").RandomId2();
	the bk(x, y, "SetBlock").FitDur();
	if (biome != BIOME_NULL)	//BIOME_NULL:保留原来的群系
		the bk(x, y, "SetBlock").biome = biome;
	//ignore biome
	return the bk(x, y, "SetBlock");
}

void DensitySpreadBlocks(float x1, float y1, float x2, float y2, BLOCK_ID bid, float dens = 0.1,BIOME_ID biome = BIOME_NULL,bool skipWater = true)
{	//密度分布特定方块
	if (the OuttaWorld(x1, y1) || the OuttaWorld(x2, y2))
	{
		DebugLog("DensitySpreadBlocks: 指定坐标无效。");
		return;
	}
	if (x1 > x2)
		swap(x1, x2);
	if (y1 > y2)
		swap(y1, y2);

	for (int dy = 0; dy < y2 - y1; dy++)
	{
		for (int dx = 0; dx < x2 - x1; dx++)
		{
			int r = RandomRange(0, 100, true, false);
			if (r < 100 * dens)
			{
				if (skipWater && the IsWaterBiome(the bk(x1 + dx, y1 + dy, "DensitySpreadBlocks").biome))
					continue;
				the SetBlock(x1 + dx, y1 + dy, bid, biome);
			}
		}
	}
}
void WorldSpreadBlocks( int times_min, int times_max, 
						int w_min, int w_max, int h_min, int h_max,
						BLOCK_ID bid, float dens_min, float dens_max,
						BIOME_ID biome=BIOME_NULL,bool skipWater=true)
{	//世界范围随机密度分布特定方块
	for (int i = 0; i < RandomRange(times_min, times_max); ++i)
	{
		int x;
		int y;
		do {
			x = RandomRange(1, g_world_w - 50);
			y = RandomRange(1, g_world_h - 50);
		} while (skipWater && the IsWaterBiome(the bk(x, y, "WorldSpreadBlocks").biome));
		int w = RandomRange(w_min, w_max);
		int h = RandomRange(h_min, h_max);

		//Avoid Invalid Coords
		while (x + w >= g_world_w)
			w--;
		while (y + h >= g_world_h)
			h--;
		the DensitySpreadBlocks(x, y, x + w, y + h, bid, RandomRangeDouble(dens_min, dens_max),biome,skipWater);	//spread
	}
}
class BiomeBlock {	//群落方块（带权重） 
	public:
		BLOCK_ID id;
		int weight;
		
		BiomeBlock()
		{
			id=0 , weight=1;
		}
		void Reset(BLOCK_ID _id,int _w)
		{
			id=_id , weight=_w;
		}
};

#define BIOMEDATA_PATH ASSETS_PATH+"Data\\BiomeData.csv"
class BiomeData {	//群落数据 
	public:
		BIOME_ID id;
		string name;
		string eng_id;
		vector<the BiomeBlock> blocks;
		//.........
		BiomeData()
		{
			id=BIOME_NULL;
			name="群落";
			eng_id="Biome";
		}
		void ClearBlocks()
		{
			blocks.clear();
		}
		void AddBiomeBlock(the BiomeBlock bb)
		{
			blocks.push_back(bb);
		}
};
vector<the BiomeData> biomedata{};

void ReadBiomeData()
{
	if (!the biomedata.empty())
		the biomedata.clear();	//防止异常

	DebugLog("【读取】读取群落数据文件于:" + BIOMEDATA_PATH);
	fstream fin(BIOMEDATA_PATH, ios::in);
	if (fin.fail())
	{
		ErrorLogTip("找不到群落数据文件", "ReadBiomeData");
		return;
	}
	vector<string> lines = ReadFileLines(BIOMEDATA_PATH);
	int i = 1;
	while (i < lines.size())
	{
		if (fin.eof())	break;
		string line;
		stringstream ss;

		line = lines.at(i);
		if (line.empty())	break;
		line = ResplitLine(line, ',', ' ');	//以空格再次分隔
		ss << line;
//		DebugLog(line, false);
		the BiomeData bdt;
		ss >> bdt.id;
		ss >> bdt.name;
		ss >> bdt.eng_id;
		//string strbls;
		//ss >> strbls;
		//DebugLog("strbls="+strbls,false);
		//stringstream ss2;
		//ss2 << strbls;
		
		while(!ss.fail())
		{
			the BiomeBlock bb;
			string tmp;
			ss >> tmp; 
			//DebugLog(tmp,false);
			bb.id = (BLOCK_ID)ToInt(tmp);
			if(ss.fail() || bb.id < 0)
				break;
			ss >> tmp;
			bb.weight = ToInt(tmp);
			bdt.AddBiomeBlock(bb);
		}
		//......
		the biomedata.push_back(bdt);	//推入
		i++;
	}
	fin.close();
}

BLOCK_ID RandomBiomeBlock(vector<the BiomeBlock> bbs)
{	//群落方块中随机选择一个方块 
	vector<BLOCK_ID> tmp{};
	if(bbs.empty())
	{
		DebugLog("RandomBiomeBlock 错误：指定方块集合为空！");
		exit(0);
		return 0;
	}
	for(int i=0;i<bbs.size();++i)
		for(int j=0;j<bbs.at(i).weight;++j)
			tmp.push_back(bbs.at(i).id);
	auto len = tmp.size();
	return tmp.at(RandomRange(0,len-1,true,true));
}
DIR4 RandomDir4()
{
	return DIR4(RandomRange(0,3));
}

const int _nxtAround[4][2] = {{1,0},{0,1},{-1,0},{0,-1}};

void bfsRenderWorld(int x,int y,int cnt,vector<the BiomeBlock> bbs,BIOME_ID biome=BIOME_NULL)
{	//递归渲染地图 
	if(cnt <= 0 || the bk(x,y,"bfsRenderWorld").biome == biome || the OuttaWorld(x,y))
		return;
	//...
//	DebugLog(ToString(cnt),false);
	BLOCK_ID bid = the RandomBiomeBlock(bbs);
	the SetBlock(x,y,bid,biome);
	/*int r = RandomRange(0,100,true,false);
	if(r < dirchgratio)
		dir = RandomDir4();*/
	for(int i=0;i<4;i++)
	{
		if (!the IsWaterBiome(biome) && RandomRange(0, 10, true, false) < 3)	//防止地形太规则
			continue;
		x += the _nxtAround[i][0];
		y += the _nxtAround[i][1];
		if(x < 0 || x >= g_world_w || y < 0 || y >= g_world_h)
			continue;		//越界剪枝
		the bfsRenderWorld(x,y,cnt-4,bbs,biome);	//递归之 
	}
}
void NormalRenderWorld(int cnt_min, int cnt_max, BIOME_ID biome)
{	//普通渲染
	//DebugLog(ToString(the biomedata.at(biome).blocks.size()));
	the bfsRenderWorld(RandomRange(5, g_world_w - 5), RandomRange(5, g_world_h - 5), RandomRange(cnt_min, cnt_max), the biomedata.at(biome).blocks, biome);
}
int g_pathCurliness = 60;	//弯曲程度(%)
void pathCreater(int x, int y, int cnt, BLOCK_ID bid, DIR dir, BIOME_ID biome=BIOME_NULL)
{
	if (the OuttaWorld(x, y) || cnt <= 0)
		return;
	the SetBlock(x, y, bid, biome);
	//变动方向
	int r = RandomRange(0, 100, true, false);
	//DebugLog(ToString(r));
	if (the IsHorizDir(dir)) {
		if (r < the g_pathCurliness)
		{
			//DebugLog("h");
			dir = RandomRange(0, 1, true, true) == 0 ? DIR_UP : DIR_DOWN;
		}
	}
	else if (the IsVertDir(dir)) {
		if (r < the g_pathCurliness)
		{
			//DebugLog("v");
			dir = RandomRange(0, 1, true, true) == 0 ? DIR_RIGHT : DIR_LEFT;
		}
	}
	else
		return;
	//递归
	if (dir == DIR_RIGHT)
		the pathCreater(x + 1, y, --cnt, bid, dir, biome);
	else if (dir == DIR_DOWN)
		the pathCreater(x, y + 1, --cnt, bid, dir, biome);
	else if (dir == DIR_LEFT)
		the pathCreater(x - 1, y, --cnt, bid, dir, biome);
	else if (dir == DIR_UP)
		the pathCreater(x, y - 1, --cnt, bid, dir, biome);
	else
		return;
}

void InitWorldSize()
{
	the g_world_w = the RandomRange(WORLD_W_MIN, WORLD_W_MAX, true, true);
	the g_world_h = the RandomRange(WORLD_H_MIN, WORLD_H_MAX, true, true);
	DebugLog("已随机设置世界尺寸，w=" + ToString(the g_world_w)+",h="+ToString(the g_world_h));
}

void EmptyWorld()
{	//注意：调用前确保世界尺寸已知 g_world_w/h
	DebugLog("EmptyWorld: 生成空世界...");
	the blocks.clear();
	for(int y=0;y<g_world_h;++y)
		for (int x = 0; x < g_world_w; ++x)
		{
			the Block b;
			b.id = b.id2 = b.dur = 0;
			the blocks.push_back(b);
		}
}

void InitWorld()
{
	EXCEPTION_L
	
	DebugLog("InitWorld: 正在生成世界...");

	the InitWorldSize();
#ifndef NO_UIEFFECT
	if (!stageEffects.empty())	stageEffects.clear();
#endif
	if (!the blocks.empty())	the blocks.clear();
	if (!the entities.empty())	the entities.clear();

	DebugLog("InitWorld: 构建世界框架...");
	for(int y = 0 ;y < the g_world_h; y++)
		for (int x = 0; x < the g_world_w; x++)
		{
			the Block b;
			BLOCK_ID id;
			id = the RandomBiomeBlock(the biomedata.at(BIOME_OVERWORLD).blocks);
			/*int r = the RandomRange(0, 100, true, false);
			if (r > 92)
				id = 1;	//dirt
			else if (r > 83)
				id = 2;	//stone
			else if (r > 76)
				id = 6;	//grassblock
			else
				id = 0;*/
			b.id = id;
			b.RandomId2();
			b.FitDur();

			the blocks.push_back(b);
		}
	//DebugLog("blocks cnt = "+ToString(blocks.size()));

	DebugLog("InitWorld: 随机生成岩壁..."); 
	//the WorldSpreadBlocks(2, 3, 25, 67, 30, 67, 2, 0.09, 0.8, BIOME_CAVE);	//stone
	for (int j = 0; j < 6; j++)
		the NormalRenderWorld(280, 750, BIOME_CLIFF);
	DebugLog("InitWorld: 随机生成草原...");
	//the WorldSpreadBlocks(2, 5, 60, 101, 50, 188, 6, 0.3, 0.9, BIOME_GRASS);	//grassblock
	for(int j=0; j<3; j++)
		the NormalRenderWorld(70, 200, BIOME_GRASS);
	
	DebugLog("InitWorld: 随机生成湖泊...");
	//the WorldSpreadBlocks(8, 18, 5, 64, 5, 65, 7, 1.0, 1.0, BIOME_POOL);	//water
	for (int j = 0; j < 14; j++)
		the NormalRenderWorld(5, 80, BIOME_LAKE);
	DebugLog("InitWorld: 随机生成海洋...");
	for (int j = 0; j < 4; j++)
		the NormalRenderWorld(90, 550, BIOME_OCEAN);
	DebugLog("InitWorld: 随机生成沙漠...");
	for (int j = 0; j < 8; j++)
		the NormalRenderWorld(160, 430, BIOME_DESERT);
	DebugLog("InitWorld: 随机生成冰原...");
	for (int j = 0; j < 7; j++)
		the NormalRenderWorld(110, 320, BIOME_ICELAND);
	DebugLog("InitWorld: 随机生成丛林...");
	for (int j = 0; j < 4; j++)
		the NormalRenderWorld(410, 640, BIOME_JUNGLE);
	DebugLog("InitWorld: 随机生成荒原...");
	for (int j = 0; j < 5; j++)
		the NormalRenderWorld(110, 250, BIOME_MOOR);

	DebugLog("InitWorld: 随机生成树木...");
	/*for (int j = 0; j < the RandomRange(400, 800); j++)	//as Entities
	{
		float rx, ry;
		do {
			rx = (float)the RandomRangeDouble(2.0, the g_world_w - 2.0, false, false, 2);
			ry = (float)the RandomRangeDouble(2.0, the g_world_h - 2.0, false, false, 2);
		} while (the bk(rx, ry,"InitWorld").id != 0);

		the Entity et;
		int r = the RandomRange(0, 10, true, false);
		if (r > 6)
			et.id = 1;	//tree
		else
			et.id = 2;	//also tree
		et.x = rx, et.y = ry;
		et.m = 100.0f;

		the entities.push_back(et);
	}*/
	the WorldSpreadBlocks(14, 28, 16, 256, 15, 256, 4, 0.0001, 0.15);	//pine
	the WorldSpreadBlocks(2, 13, 30, 199, 20, 199, 5, 0.006, 0.09);	//poplar
	DebugLog("InitWorld: 随机生成草排...");
	the g_pathCurliness = 7;
	for (int j = 0; j < RandomRange(50, 70); ++j)
		the pathCreater(RandomRange(2, g_world_w - 2), RandomRange(2, g_world_h - 2), RandomRange(4, 30),
			11, the RandomDir4(), BIOME_NULL);
	DebugLog("InitWorld: 随机生成花排...");
	the g_pathCurliness = 7;
	for (int j = 0; j < RandomRange(30, 40); ++j)
		the pathCreater(RandomRange(2, g_world_w - 2), RandomRange(2, g_world_h - 2), RandomRange(7, 60),
			13, the RandomDir4(), BIOME_NULL);
	DebugLog("InitWorld: 随机生成小径...");
	the g_pathCurliness = 2;
	for (int j = 0; j < RandomRange(90, 330); ++j)
		the pathCreater(RandomRange(2, g_world_w - 2), RandomRange(2, g_world_h - 2), RandomRange(40, 120),
			0, the RandomDir4(), BIOME_NULL);
	the g_pathCurliness = 9;
	for (int j = 0; j < RandomRange(30, 70); ++j)
		the pathCreater(RandomRange(2, g_world_w - 2), RandomRange(2, g_world_h - 2), RandomRange(40, 120),
			0, the RandomDir4(), BIOME_NULL);
	the g_pathCurliness = 33;
	for (int j = 0; j < RandomRange(60, 150); ++j)
		the pathCreater(RandomRange(2, g_world_w - 2), RandomRange(2, g_world_h - 2), RandomRange(40, 120),
			0, the RandomDir4(), BIOME_NULL);
	the g_pathCurliness = 70;
	for (int j = 0; j < RandomRange(10, 20); ++j)
		the pathCreater(RandomRange(2, g_world_w - 2), RandomRange(2, g_world_h - 2), RandomRange(40, 120),
			0, the RandomDir4(), BIOME_NULL);

	DebugLog("InitWorld: 随机生成河流...");
	the g_pathCurliness = 4;
	for(int j=0;j<RandomRange(30,50);++j)
		the pathCreater(RandomRange(2, g_world_w - 2), RandomRange(2, g_world_h - 2), RandomRange(20, 99),
			7, the RandomDir4(), BIOME_RIVER);
	the g_pathCurliness = 18;
	for (int j = 0; j < RandomRange(90, 220); ++j)
		the pathCreater(RandomRange(2, g_world_w - 2), RandomRange(2, g_world_h - 2), RandomRange(20, 99),
			7, the RandomDir4(), BIOME_RIVER);
	the g_pathCurliness = 40;
	for (int j = 0; j < RandomRange(70, 136); ++j)
		the pathCreater(RandomRange(2, g_world_w - 2), RandomRange(2, g_world_h - 2), RandomRange(20, 99),
			7, the RandomDir4(), BIOME_RIVER);
	the g_pathCurliness = 60;
	for (int j = 0; j < RandomRange(10, 22); ++j)
		the pathCreater(RandomRange(2, g_world_w - 2), RandomRange(2, g_world_h - 2), RandomRange(20, 99),
			7, the RandomDir4(), BIOME_RIVER);

	DebugLog("InitWorld: 世界生成完毕");
	
	EXCEPTION_R
}
