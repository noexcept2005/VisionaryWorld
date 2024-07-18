//VisionaryWorld 专属头文件
//任何人不得盗用。
//Written by Rico
#pragma once

template <typename _T>
string CJZAPI ToString(_T value)	//转为std::string
{
	stringstream ss;
	ss << value;
	return ss.str();
}
template <typename _T>
int CJZAPI ToInt(_T value)	//转为int
{
	stringstream ss;
	int ret;
	ss << value;
	ss >> ret;
	return ret;
}


#define LINE_CHAR_MAX 65535
vector<string> CJZAPI ReadFileLines(string filePath, int lineMaxCharCnt = LINE_CHAR_MAX)
{	//读取文件每一行 
	vector<string> ret{};
	fstream fin(filePath, ios::in);
	if (fin.fail())
		return ret;
	while (1)
	{
		CHAR s[LINE_CHAR_MAX];

		if (fin.eof())
			break;

		fin.getline(s, lineMaxCharCnt);
		//        cout<<s<<endl;
		ret.push_back(ToString(s));
	}
	fin.close();
	return ret;
}

vector<string> CJZAPI CSVCutLine(string line, char sep = ',')
{	//数据分开，单独取出 
	//CSV 逗号分隔值文件 , 处理略有不同 
	vector<string> data{};
	int p1 = 0, p2 = 0;

	for (int i = 0; i < line.size(); i++)
	{
		if (line.at(i) == sep
			|| /*line.at(i) == '\n' NO USE*/
			i == line.size() - 1)	//别漏了最后一个数据 
		{
			p2 = i;
			auto one = line.substr(p1, p2 - p1
				+ ((i == line.size() - 1
					&& line.at(i) != sep)	//防止最后多个逗号 
					? 1 : 0));	//别漏了最后一个字符 
			data.push_back(one);	//<!>警告：有可能会放空的str 
//			MSGPRINT(one);
			p1 = i + 1;
		}
	}
	return data;
}

vector<string> CJZAPI CutLine(string line, char sep = ' ')
{
	vector<string> ret{};
	if (line.empty()) return ret;
	int p1 = 0, p2 = 0;
	for (int i = 0; i < line.size(); i++)
	{
		if (line.at(i) == sep
			|| i == line.size() - 1)
		{
			p2 = i;
			string s = line.substr(p1, p2 - p1 + (i == line.size() - 1 ? 1 : 0));
			ret.push_back(s);
			p1 = i + 1;
		}
	}
	return ret;
}

string CJZAPI ResplitLine(string line, char sep = ',', char newSep = ' ')
{	//重新分隔 并合并成一句 
	vector<string> data = CSVCutLine(line, sep);
	string ret;
	for (int i = 0; i < data.size(); i++)
	{
		ret += data.at(i);
		if (i != data.size() - 1)
			ret += newSep;
	}
	return ret;
}
string CJZAPI CSVResplitLine(string line, char sep = ',', char newSep = ' ')
{
	return ResplitLine(line, sep, newSep);
}

