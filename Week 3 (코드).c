# 백준 16235
#include <iostream>
#include <algorithm>
#include <deque>

#define endl "\n"
#define MAX 11  // 문제 조건이 N<=10 이고 인덱스를 1부터 쓰고 있기 때문에 배열 크기는 최소 11이 필요, 0번 인덱스는 안씀

using namespace std;

int N, M, K;
int Energy[MAX][MAX]; // 양분 배열
int Insert_Energy[MAX][MAX]; // 겨울마다 추가할 양분 배열

deque<int> MAP[MAX][MAX];  // 나무 나이 목록 
// (2,3)칸에 나이 1,3,7짜리 나무가 있으면 -> MAP[2][3] = [ 1, 3, 7 ]
// deque<int> 를 쓴 이유는 앞 뒤에서 나무를 넣고 빼고가 용이하기 때문
// 어린 나이 나무부터 양분을 먹어야 하기 때문에 나무들을 나이순으로 다뤄야 함. -> 각 칸의 나무들을 오름차순으로 deque에 저장
// 봄엔 앞에서부터 꺼내고 가을에 번식하면 나이 1짜리 나무가 생기기 때문에 push_front(1)로 가장 어린 나무를 맨 앞에 넣음


int dx[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
int dy[] = { -1,  0,  1, -1, 1, -1, 0, 1 };
// 번식시에 8방향 배열
// 현재 칸 (i,j)에서 (i + dx[a], j + dy[a])를 하면 주변 8칸으로 이동할 수 있음

void SpringAndSummer()
{
    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= N; j++)
        {
            if (MAP[i][j].empty()) continue; // 모든 칸을 돌면서 그 칸에 나무가 없으면 그냥 넘어가기
            // continue -> 가장 가까운 반복문 (여기서는 j for문)을 건너뛰고 다음으로 넘어간다 (여기서는 j++ 하고 다음 j턴으로 가겠다)

            int Die_Tree_Energy = 0; // 이번 봄에 죽은 나무들이 여름에 양분으로 바뀌어서 더해질 값을 모아두는 변수
            deque<int> Temp;  // 새로 빈 deque 선언
            // 이번 봄이 끝난 뒤 살아남은 나무들을 저장할 임시 deque (기존 deque를 수정하면서 처리하면 헷갈리기 쉽기 때문에 임시 저장 사용)
            // 기존 MAP[i][j]에서 하나씩 꺼내고 살아남으면 Temp에 넣고 마지막에 MAP[i][j] = Temp로 갈아끼우는 구조

            while (!MAP[i][j].empty()) // 나무가 있다면
            {
                int Age = MAP[i][j].front(); 
                MAP[i][j].pop_front(); //나무를 앞에서부터 하나씩 꺼냄(가장 어린 나무)

                if (Energy[i][j] >= Age) 
                {
                    Energy[i][j] -= Age;  // 양분 빼기
                    Temp.push_back(Age + 1); // 나이 하나 증가
                } 
                // 양분 >= 나이 이면 자기 나이만큼 양분을 먹고 살아남음
                // 왜 pushback? 어린 나무부터 순서대로 처리 중이기 때문
                else
                {
                    Die_Tree_Energy += (Age / 2);  // 죽은 나무는 여름에 나이/2 만큼 양분이 되니까 일단 누적

                    // 현재 나무보다 뒤에 있는 나무들은 더 나이가 많거나 같으므로 전부 죽음
                    while (!MAP[i][j].empty())
                    {
                        Die_Tree_Energy += (MAP[i][j].front() / 2);
                        MAP[i][j].pop_front();
                    }
                    break;
                    // 중요
                    // 현재 이 나무가 양분을 못 먹었다는 뜻은 지금 남은 양분이 지금 이 나무 나이보다 작다는 뜻
                    // 뒤에 있는 나무들은 나이가 지금 나무보다 같거나 많음 (오름차순)
                    // 따라서 뒤쪽 나무들은 전부 죽는다고 바로 처리
                }
            }

            MAP[i][j] = Temp;
            Energy[i][j] += Die_Tree_Energy;
            // 살아남은 나무 목록으로 칸의 나무를 교체하고 죽은 나무에서 나온 양분을 현재 칸에 더함
        }
    }
}

void Fall()
{
    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= N; j++)
        {
            if (MAP[i][j].empty()) continue;  // 모든 칸을 돌면서 나무가 있는 칸만 처리

            for (int Age : MAP[i][j])
            {
                if (Age % 5 == 0) // 5의 배수인 것만 번식
                {
                    for (int a = 0; a < 8; a++)
                    {
                        int nx = i + dx[a];
                        int ny = j + dy[a];

                        if (nx >= 1 && ny >= 1 && nx <= N && ny <= N) // 주변 8칸을 돌면서 범쉬 안이면
                        {
                            MAP[nx][ny].push_front(1); // 그 칸에 나이 1 나무를 추가 (가장 나이가 어리니까 front에)
                        }
                    }
                }
            }
        }
    }
}

void Winter()
{
    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= N; j++)
        {
            Energy[i][j] += Insert_Energy[i][j];  // 모든 칸에 겨울 양분 추가
        }
    }
}

void Solution()
{
    // 초기 입력에 대해서는 한 번 정렬 필요 (처음 입력도니 나무들은 순서가 보장되지 않기 때문)
    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= N; j++)
        {
            sort(MAP[i][j].begin(), MAP[i][j].end());  // 오름차순 정렬 (봄에 어린 나무부터 처리해야 하기 떄문에 중요)
        }
    }

    for (int i = 0; i < K; i++)
    {
        // 사계절 함수 순서대로 호출
        SpringAndSummer();
        Fall();
        Winter();
    }

    int Answer = 0;

    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= N; j++)
        {
            Answer += MAP[i][j].size();
        }
    }

    cout << Answer << endl;
    // 모든 칸을 돌면서 남아 있는 나무 수를 다 더해 출력
}

int main(void)
{
    // 입력받기
    cin >> N >> M >> K;

    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= N; j++)
        {
            cin >> Insert_Energy[i][j]; // 겨울에 추가할 양분 배열 입력받음
            Energy[i][j] = 5;  // 처음 양분을 모두 5로 셋팅
        }
    }

    // 초기 나무 정보 입력
    for (int i = 0; i < M; i++)
    {
        int a, b, c;
        cin >> a >> b >> c;
        MAP[a][b].push_back(c); // (a, b) 위치에 나이 c인 나무를 넣음
        // MAP[a][b]가 가장 중요, (i,j)칸에 있는 나무들의 나이 목록
    }

    // 솔루션 호출
    Solution();
}
