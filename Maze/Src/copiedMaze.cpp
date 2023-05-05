#include <iostream>
#include <queue>
#include <string.h>
using namespace std;

#define MAX_SIZE (30)
#define MAX_TIME (1000)

constexpr int dir = 6; // 동서남북 상하 
int dx[dir]{ 1, -1, 0, 0, 0, 0 };
int dy[dir]{ 0, 0, 1, -1, 0, 0 };
int dz[dir]{ 0, 0, 0, 0, 1, -1 };

struct Data
{
    int l, r, c, t;
};

queue<Data> que;

int L, R, C;
char map[MAX_SIZE + 10][MAX_SIZE + 10][MAX_SIZE + 10];
int visited[MAX_SIZE + 10][MAX_SIZE + 10][MAX_SIZE + 10];

void Init()
{
    memset(map, 0, sizeof(map));
    memset(visited, 0, sizeof(visited));
}

bool InputData() {
    cin >> L >> R >> C;
    if ((L == 0) && (R == 0) && (C == 0)) return false;
    for (int l = 0; l < L; l++) {
        for (int r = 0; r < R; r++) {
            cin >> map[l][r];
        }
    }
    return true;
}

Data FindChar(char ch)
{
    Data dt{ 0,0,0,0 };
    for (int i = 0; i < L; i++)
    {
        for (int j = 0; j < R; j++)
        {
            for (int k = 0; k < C; k++)
            {
                if (map[i][j][k] == ch)
                {
                    dt.l = i, dt.r = j, dt.c = k;
                }
            }
        }
    }
    return dt;
}

bool isValid(Data dt)
{
    if (dt.l < 0 || dt.r < 0 || dt.c < 0) return false;
    if (dt.l >= L || dt.r >= R || dt.c >= C) return false;
    if (map[dt.l][dt.r][dt.c] == '#') return false;
    return true;
}

bool isSame(Data dt1, Data dt2)
{
    if (dt1.l == dt2.l && dt1.r == dt2.r && dt1.c == dt2.c) return true;
    return false;
}

int BFS(Data e_pnt)
{
    int min_time = MAX_TIME;
    while (!que.empty())
    {
        Data dt = que.front(); que.pop();
        for (int i = 0; i < dir; i++)
        {
            Data temp_dt = dt;
            temp_dt.l = dt.l + dz[i];
            temp_dt.r = dt.r + dy[i];
            temp_dt.c = dt.c + dx[i];
            temp_dt.t = dt.t + 1;
            //cout << temp_dt.l << ", " << temp_dt.r << ", " << temp_dt.c << ", " << temp_dt.t << endl; 
            if (isSame(temp_dt, e_pnt))
            {
                if (min_time > temp_dt.t) min_time = temp_dt.t;
            }
            if (!isValid(temp_dt)) continue;
            int& check = visited[temp_dt.l][temp_dt.r][temp_dt.c];
            if (!check || check > temp_dt.t)
            {
                check = temp_dt.t;
                que.push(temp_dt);
            }

        }
    }
    return min_time;
}

int Solve()
{
    Data s_pnt = FindChar('S');
    Data e_pnt = FindChar('E');

    que.push(s_pnt);
    int ans = BFS(e_pnt);
    if (ans == MAX_TIME) ans = -1;
    return ans;
}

int main() {
    int ans = -1;
    while (InputData()) {

        //여기서부터 작성
        ans = Solve();

        if (ans == -1) cout << "Trapped!" << endl;
        else cout << "Escaped in " << ans << " minute(s)." << endl;

        Init();
    }
    return 0;
}