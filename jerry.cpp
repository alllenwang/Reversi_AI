#include "othello16.h"
#include "gamti.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
static int cost[16][16];
class othello_ai{
	othello16 o;//ʵ����othello16��
	othello16 temp;
public:
	void init(int color, string s);
	void move(int color, int x, int y);
	int f(othello16 a, int line, int position, int var);
	int result(othello16 a);
	int depth_1(othello16 a,int num);
	pair<int, int> get();
};
void othello_ai::init(int color, string s){
	o.init(color, s);//��sdk��ʼ������
	cost[0][0] = 9;
	cost[0][15] = 9;
	cost[15][0] = 9;
	cost[15][15] = 9;
	int i;
	int j;
	for(i = 2 ;i < 14 ; i ++)
	{
		cost[0][i] = 4;
		cost[i][0] = 4;
		cost[15][i] = 4;
		cost[i][15] = 4;
	}
	for(i = 0; i < 16; i ++)
	{
		cost[1][i] = -2;
		cost[14][i] = -2;
		cost[i][1] = -2;
		cost[i][14] = -2;
	}
	cost[1][1] = -20;
	cost[1][0] = -20;
	cost[0][1] = -20;

	cost[0][14] = -20;
	cost[1][14] = -20;
	cost[1][15] = -20;

	cost[14][0] = -20;
	cost[14][1] = -20;
	cost[15][1] = -20;

	cost[14][15] = -20;
	cost[14][14] = -20;
	cost[15][14] = -20;
	for(i = 2 ; i < 14; i ++)
		for(j = 2 ; j < 14 ; j ++)
			cost[i][j] = 1;

}
void othello_ai::move(int color, int x, int y){
	o.play(color, x, y);//����sdk�������,�ı����

}
int othello_ai::result(othello16 a)
{
	int n1, n2, n3, n4;
	int num = 0;//���Ӹ�����
	int canput;//�����Ӳ�(�ж���)
	int bothputnum;
	int pos = 0;//���ֲ�
	int evaluation = 0;
	int potential_canput = 0;
	vector<pair<int, int> > t1 = o.allmove(o.mycolor);
	vector<pair<int, int> > t2 = o.allmove(3 - o.mycolor);

	canput = t1.size() - t2.size();
	bothputnum = t1.size() + t2.size();
	int total;
	string borad = a.tostring();
	if(a.count(3 - a.mycolor) == 0)
	{
		return 99999;
	}
	if(a.count(a.mycolor) == 0)
	{
		return -99999;
	}
	num = a.count(a.mycolor) - a.count(3 - a.mycolor);//���Ӹ�����
	total = a.count(a.mycolor) + a.count(3 - a.mycolor);
	if(bothputnum == 0)
	{
		if(num > 0)
			return 99999;
		else if(num < 0)
			return -99999;
		else
			return	0;
	}
	int i,j,z;
	for(i = 0 ; i < 16 ; i ++)
	{
		for(j = 0 ; j < 16 ; j ++)
		{
			char t =  borad.at(i * 16 + j);
			if(t == '0' + a.mycolor)
			{
				pos = pos + cost[i][j];
			}
			else if(t == '0' + 3 - a.mycolor)
			{
				pos = pos - cost[i][j];
			}
		}
	}
	int x[9];
	for(i = 0 ; i < 16 ; i ++)
	{
		for(j = 0 ; j < 16 ; j ++)
		{
			for(z = 0 ; z < 9 ; z ++)
			{
				x[z] = i * 16 + j + (z/3 - 1) * 16 + (z%3 - 1);
				if(x[z] >= 0 && x[z] <= 255)
				{
					char t = borad.at(i * 16 + j);
					char t1 = borad.at(x[z]);
					if(t1 == '0')
					{
						if(t == '0' + a.mycolor)
						{
							potential_canput = potential_canput - 1;
						}
						else if(t == '0' + 3 - a.mycolor)
						{
							potential_canput = potential_canput + 1;
						}
					}
				}
			}
		}
	}
	if(total < 210)
	{
		n1 = 1;
		n2 = 4;
		n3 = 3;
		n4 = 4;

	}
	else
	{
		n1 = 4;
		n2 = 2;
		n3 = 1;
		n4 = 2;
	}
	evaluation =n1 * num + n2 * canput + n3 * pos + n4 * potential_canput;
	return evaluation;
}
int othello_ai::f(othello16 a, int line, int position, int var)
{
	if(line == 4)
	{
		int x;
		x = result(a);
		return x;
	}
	else
	{
		int number = line % 2;
		int color;
		if(number == 1)
			color = 3 - a.mycolor;
		else
			color = a.mycolor;
		vector<pair<int, int> > t = a.allmove(color);
		
		int len = t.size();
		int i = 0;
		int max = var;
		int pos = 0;
		if(position == 1)
			pos = 1;
		if(line == 2)
			pos = 0;
		if(len == 0)
		{
			int t = result(a);
			return t;
		}
		for( ; i < len ; i ++)
		{
			temp = a;
			temp.play(color,t[i].first,t[i].second);
			int t = f(temp,line + 1, pos ,max);
	//		std::cerr<<t<<" "<<"    var: "<<var<<"   "<<"line: "<<line + 1<<"\n";
			if(i == 0)
			{
				max = t;
			}
			pos = 1;
			if(number == 1)
			{
				if(max > t)
				{
					max = t;
				}
				if(t <= var && position > 0)
				{
	//				std::cerr<<"------"<<"\n";
					return t;
				}
			}
			else
			{
				if(t >= var && position > 0)
				{
	//				std::cerr<<"------"<<"\n";
					return t;
				}
				if(max < t)
				{
					max = t;
				}
			}
		}
	//	std::cerr<<"\n";
		return max;
	}
}
int othello_ai::depth_1(othello16 a,int num)
{
	int n1, n2, n3, n4;
	int num_compare = 0;//���Ӹ�����
	int canput;//�����Ӳ�(�ж���)
	int change_num = a.count(a.mycolor) - num;

	int bothputnum;
	int pos = 0;//���ֲ�
	int evaluation = 0;
	int potential_canput = 0;
	vector<pair<int, int> > t1 = o.allmove(o.mycolor);
	vector<pair<int, int> > t2 = o.allmove(3 - o.mycolor);
	if(a.count(3 - a.mycolor) == 0)
	{
		return 99999;
	}
	if(a.count(a.mycolor) == 0)
	{
		return -99999;
	}
	canput = -t2.size();
	bothputnum = t1.size() + t2.size();
	int total;
	string borad = a.tostring();

	num_compare = a.count(a.mycolor) - a.count(3 - a.mycolor);//���Ӹ�����
	total = a.count(a.mycolor) + a.count(3 - a.mycolor);
	if(bothputnum == 0)
	{
		if(num_compare > 0)
			return 99999;
		else if(num_compare < 0)
			return -99999;
		else
			return	0;
	}
	int i,j,z;
	for(i = 0 ; i < 16 ; i ++)
	{
		for(j = 0 ; j < 16 ; j ++)
		{
			char t =  borad.at(i * 16 + j);
			if(t == '0' + a.mycolor)
			{
				pos = pos + cost[i][j];
			}
			else if(t == '0' + 3 - a.mycolor)
			{
				pos = pos - cost[i][j];
			}
		}
	}
	int x[9];
	for(i = 0 ; i < 16 ; i ++)
	{
		for(j = 0 ; j < 16 ; j ++)
		{
			for(z = 0 ; z < 9 ; z ++)
			{
				x[z] = i * 16 + j + (z/3 - 1) * 16 + (z%3 - 1);
				if(x[z] >= 0 && x[z] <= 255)
				{
					char t = borad.at(i * 16 + j);
					char t1 = borad.at(x[z]);
					if(t1 == '0')
					{
						if(t == '0' + a.mycolor)
						{
							potential_canput = potential_canput - 1;
						}
						else if(t == '0' + 3 - a.mycolor)
						{
							potential_canput = potential_canput + 1;
						}
					}
				}
			}
		}
	}
	if(total < 210)
	{
		n1 = 1;
		n2 = 4;
		n3 = 3;
		n4 = 4;

	}
	else
	{
		n1 = 4;
		n2 = 2;
		n3 = 1;
		n4 = 2;
	}
	evaluation =n1 * num_compare + n2 * canput + n3 * pos + n4 * potential_canput + change_num * 5;
	return evaluation;
}

pair<int, int> othello_ai::get(){
	vector<pair<int, int> > ans = o.allmove(o.mycolor);
	//������п����ӵ�λ��,�����к��е�˳��
	if(o.tostring().at(0) == '0' + o.mycolor)
	{
		cost[0][1] = 5;
		cost[1][0] = 5;
	}
	if(o.tostring().at(15) == '0' + o.mycolor)
	{
		cost[0][14] = 5;
		cost[1][15] = 5;
	}
	if(o.tostring().at(16*15) == '0' + o.mycolor)
	{
		cost[14][0] = 5;
		cost[15][1] = 5;
	}
	if(o.tostring().at(16*16 - 1) == '0' + o.mycolor)
	{
		cost[14][15] = 5;
		cost[15][14] = 5;
	}
	int len = ans.size();
	int i;
	int max = 0;
	int pos = 0;
	int position = 0;

	for(i = 0 ; i < len ; i ++)
	{
		int n = cost[ans[i].first][ans[i].second];
		if(max < n)
		{
			max = n;
			pos = i;
		}
	}
	if(max > 0)
	{
		std::cerr<<"----------------------------------------------------"<<"\n";
		std::cerr<<"time : "<<get_time()<<"   choose:"<<pos<<"\n";
		return ans[pos];
	}
	if(len == 1)
	{
		std::cerr<<"====================================================="<<"\n";
		std::cerr<<"time : "<<get_time()<<"   choose: 0"<<"\n";
		return ans[0];
	}
	max = 0;
	pos = 0;
	int count = o.count(o.mycolor);
	for(i = 0 ; i < len ; i ++)
	{
		temp = o;
		temp.play(temp.mycolor,ans[i].first,ans[i].second);
		int t = f(temp, 1, position, max);
	//	std::cerr<<t<<"  "<<"line: 1"<<"\n";
		if(i == 0)
			max = t;
		position = 1;
		if(max < t)
		{
			max = t;
			pos = i;
		}
	}
	std::cerr<<">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"<<"\n";
	std::cerr<<"time : "<<get_time()<<"   choose:"<<pos<<"\n";
	
	return ans[pos];//��������λ��Ϊ��һ�����µĵ�
}