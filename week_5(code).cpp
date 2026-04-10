#include <iostream>
#define endl "\n"
#define MAX 1500010 // 배열의 최대 크기 미리 정함

using namespace std;

int N, Answer; // N : 총 날짜 수, Answer : 최종 정답, 즉 최대 수익을 저장할 변수
int DP[MAX]; // 핵심 배열 - i일에 도달했을 때 얻을 수 있는 최대 수익 후보(벌 수 있는 최대 돈을 저장하는 배열)
int Arr[MAX][2]; // 각 날짜의 상담 정보를 저장하는 배열
// Arr[i][0] : i일에 시작한 상담의 소요 기간
// Arr[i][1] : i일에 시작한 상담의 수익

int Bigger(int A, int B)
{
    if (A > B) return A;
    return B;
}

void Input()
{
    cin >> N; // 총 날짜 수

    for (int i = 1; i <= N; i++)
    {
        cin >> Arr[i][0] >> Arr[i][1]; // 각 날짜의 상담 정보를 입력받음
    }
}

void Solution()
{
    int Current_Max = 0;

    for (int i = 1; i <= N + 1; i++) // N+1일에 퇴사 -> 상담이 정확히 N+1일에 끝나는건 가능
    {
        // 현재까지 최대 수익 갱신
        // i일에 도착했을 때 가능한 수익 후보 DP[i]와 지금까지 알고 있던 최대 수익 Current_Max 중 더 큰 값을 현재 최대 수익으로 유지하겠다
        Current_Max = Bigger(Current_Max, DP[i]);

        if (i + Arr[i][0] > N + 1) // 퇴사일을 넘는 상담은 불가능
            continue;

        // 이 줄이 DP 갱신의 핵심
        // DP[ i + Arr[i][0] : 상담이 끝난 시점에 도달했을 때 얻을 수 있는 최대 수익 후보
        // i일 상담을 선택해서 i+T일 시점에 도달할 수 있다면, 그떄의 최대 수익을 갱신해라
        DP[i + Arr[i][0]] =
            Bigger(Current_Max + Arr[i][1], DP[i + Arr[i][0]]); // 기존 값과 새로 계산한 값을 비교해서 거 큰 값을 저장해라
    }

    Answer = Current_Max; // 퇴사일까지 가능한 최대 수익 저장
}

int main(void)
{
    Input(); // 입력
    Solution(); // DP 계산
    cout << Answer << endl; // 출력
}
