//VisionaryWorld ר��ͷ�ļ�
//�κ��˲��õ��á�
//Written by Rico
#pragma once

template <typename _T>
string CJZAPI ToString(_T value)	//תΪstd::string
{
	stringstream ss;
	ss << value;
	return ss.str();
}
template <typename _T>
int CJZAPI ToInt(_T value)	//תΪint
{
	stringstream ss;
	int ret;
	ss << value;
	ss >> ret;
	return ret;
}


#define LINE_CHAR_MAX 65535
vector<string> CJZAPI ReadFileLines(string filePath, int lineMaxCharCnt = LINE_CHAR_MAX)
{	//��ȡ�ļ�ÿһ�� 
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
{	//���ݷֿ�������ȡ�� 
	//CSV ���ŷָ�ֵ�ļ� , �������в�ͬ 
	vector<string> data{};
	int p1 = 0, p2 = 0;

	for (int i = 0; i < line.size(); i++)
	{
		if (line.at(i) == sep
			|| /*line.at(i) == '\n' NO USE*/
			i == line.size() - 1)	//��©�����һ������ 
		{
			p2 = i;
			auto one = line.substr(p1, p2 - p1
				+ ((i == line.size() - 1
					&& line.at(i) != sep)	//��ֹ��������� 
					? 1 : 0));	//��©�����һ���ַ� 
			data.push_back(one);	//<!>���棺�п��ܻ�ſյ�str 
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
{	//���·ָ� ���ϲ���һ�� 
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

