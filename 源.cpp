#include <cstdio>
#include <vector>
#include <map>
#include <queue>
#include<string>
#include<string.h>
#include<algorithm>
#include<iostream>
#include<fstream>
#pragma warning(disable:4996)
using namespace std;

struct info {
	int length;
	string road;
};

const int maxn = 31; //���ֺ��±�ƥ��
int k, m, n, s;
map<int, info>step;
map<int, char> v; //
bool input[maxn][maxn];//��¼�����������һ����ʾ�ڼ�������������� // chushihua
vector<bool> original(maxn); //����������ʲô������
int beginning; //chushihua 

int inline readNum() // ���ٶ���
{
	char ch = getchar();
	while (ch < '0' || ch > '9') ch = getchar();
	int v = 0;
	while (ch >= '0' && ch <= '9') {
		v = v * 10 + ch - '0';
		ch = getchar();
	}
	return v;
};

void inline getdigit(int s) {
	for (int i = n; i >= 1; i--) {
		original[i] = s % 2;
		s /= 2;
	}
}

int inline getnum() {
	int tmp = 0;
	for (int i = 1; i <= n; i++) {
		tmp = tmp * 2 + original[i];
	}
	return tmp;
}

int inline change(int u, int d) {
	getdigit(u);
	for (int i = 1; i <= n; i++) {
		if (input[d][i]) original[i] = !original[i];
	}
	return getnum();
}

void inline bfs(queue<int>& Q) {
	//�������յ�״̬
	v[beginning] = 1;
	v[s] = 2;
	//�������յ㲽��
	step[beginning].length = 0;
	step[s].length = 1;
	//���
	Q.push(beginning);
	Q.push(s);
	while (Q.size())
	{
		int cur, next;
		cur = Q.front(); //���е�һ��������
		Q.pop();

		for (int i = 1; i <= m; i++) //m�����ж�
		{
			next = change(cur, i);

			if (v[cur] == v[next])
				continue;

			if (3 == v[cur] + v[next]) //����
			{
				int len = step[next].length + step[cur].length;
				printf("%d ", len);
				string s;
				if (v[cur] == 1) s = step[cur].road + (char)i + step[next].road;
				else s = step[next].road + (char)i + step[cur].road;
				sort(s.begin(), s.end());
				char c;
				/*for (int i = 0; i < len; i++) {
					c = s[i];
					printf("%d ", c);
				}
				printf("-1\n");*/
				return;
			}

			v[next] = v[cur];
			step[next].length = step[cur].length + 1;
			if (v[cur] == 1) {
				step[next].road = step[cur].road;
				step[next].road += (char)i;
			}
			else {
				step[next].road = step[cur].road;
				step[next].road = (char)i + step[cur].road;
			}

			Q.push(next);
		}
	}
}

void mybfs(ifstream&ifile) {
	memset(input, 0, sizeof(input));
	step.clear();
	v.clear();
	queue<int> Q;
	beginning = 0;
	for (int i = 1; i <= n; i++) {
		original[i] = 0;
	}

	if (s) {
		for (int i = 0; i < n; i++)
			s *= 2;
		s--;
	}

	for (int i = 1; i <= m; i++) {
		int tmp;
		ifile >> tmp;
		while (tmp != -1) {
			input[i][tmp] = 1;
			ifile >> tmp;
		}
	}

	for (int i = 1; i <= n; i++) { int ddd; ifile >> ddd; original[i]=ddd; }
	for (int i = 1; i <= n; i++) {
		beginning = beginning * 2 + original[i];
	}
	bfs(Q);
}

bool matrix[31][31];

void myGuass(bool* flag, bool(*ans)[31]) {
	short sum = 0; short checked_line = 1;
	for (int i = 1; i <= m; i++) {
		int p = checked_line;
		while (!matrix[p][i] && p <= n)p++;
		if (p == n + 1) { continue; }
		else { //��p���ж���
			//p�к�checked_line����
			for (int k = 0; k <= m; k++) swap(matrix[checked_line][k], matrix[p][k]);
			flag[i] = 1; sum++;
			//����checked_line����Ԫ��
			for (int k = checked_line + 1; k <= n; ++k) {
				if (matrix[k][i]) {
					int s = (-matrix[k][i]) / matrix[checked_line][i];//������
					for (int j = i + 1; j <= m; ++j) matrix[k][j] = (matrix[k][j] + s * matrix[checked_line][j]) % 2;//��Ԫ����
					matrix[k][i] = 0; //��Ԫ
					matrix[k][0] = (matrix[k][0] + s * matrix[checked_line][0]) % 2; //���������Ԫ����
				}
			}
			checked_line++;
		}
	}

	int symbol = sum;
	for (int i = m; i >= 1; i--) {
		if (flag[i]) {
			ans[0][i] = matrix[symbol][0], symbol--;
			for (int j = 1; j <= symbol; j++) {
				matrix[j][0] = int(matrix[j][0] - matrix[j][i] * ans[0][i]) % 2;
			}
		}
	}

	//suantongjie
	int cnt = 1;
	for (int ii = 1; ii <= m; ii++) {
		//���һ��ͨ��
		if (!flag[ii]) {
			int tmp[31] = { 0 };
			ans[cnt][ii] = 1;
			for (int j = 1; j <= sum; j++) {//jshihang 
				tmp[j] = matrix[j][ii];
			}
			int symbol = sum;
			for (int i = m; i >= 1; i--) {
				if (flag[i]) {
					ans[cnt][i] = tmp[symbol], symbol--;
					for (int j = 1; j <= symbol; j++) {
						tmp[j] = int(tmp[j] - matrix[j][i] * ans[cnt][i]) % 2;
					}
				}
			}
			cnt++;
		}
	}

	//suandaan
	int times = 1;
	for (int i = 0; i < m - sum; i++)
		times *= 2;
	int c[31];
	bool road[31] = { 0 }; short length = 1000;
	bool ans_road[31] = { 0 };

	for (int ii = 0; ii < times; ii++) {
		int daiti = ii;
		for (int j = 1; j <= m - sum; j++) {
			c[j] = daiti % 2;
			daiti /= 2;
		}

		for (int k = 1; k <= m; k++)
			road[k] = ans[0][k];
		for (int j = 1; j <= m - sum; j++) {
			for (int k = 1; k <= m; k++) {
				road[k] = int(road[k] + c[j] * ans[j][k]) % 2;
			}
		}

		int tmp_count = 0;
		for (int i = 1; i <= m; i++)
			if (road[i]) tmp_count++;
		if (tmp_count < length) {
			length = tmp_count;
			for (int k = 1; k <= m; k++)
				ans_road[k] = road[k];
		}
	}

	printf("%d ", length);
	/*for (int i = 1; i <= m; i++) {
		if (ans_road[i]) printf("%d ", i);
	}
	printf("-1\n");*/
}

void work(ifstream&ifile) {
	bool flag[31] = { 0 };
	bool ans[31][31] = { 0 };//��0�з��ؽ�
	int tmp;
	memset(matrix, 0, sizeof(matrix));

	for (int maa = 1; maa <= m; maa++) {
		ifile >> tmp;
		while (tmp != -1) {
			matrix[tmp][maa] = 1;
			ifile >> tmp;
		}
	}
	/**/
	for (int naa = 1; naa <= n; naa++) {
		matrix[naa][0] = s;
		ifile >> tmp;
		matrix[naa][0] = (int)(s - tmp) % 2;
	}

	myGuass(flag, ans);
}

int main() {
	ifstream ifile("test.txt");
	if (!ifile) exit(1);
	ifile >> k;
	for (int ii = 0; ii < k; ii++) {
		ifile >> n >> m >> s;
		if (m - n > 5 || n - m > 5)
			mybfs(ifile);
		else
			work(ifile);
	}
	ifile.close();
	return 0;
}