#include <iostream>  // 입출력 하려고.. cin, cout
#include <vector>  // vector 쓰려고.. 크기가 유동적인 배열을 만들 때 사용
#include <cmath>  // 수학 함수인 log2, ceil 쓰려고

#define endl "\n" // endl : 줄바꿈하면서 출력버퍼 비움 -> 이걸 쓰면서 속도 더 빨리할 수 있음 
using namespace std; // 매번 std:: 안쓰려고 씀

int N, M; // N : 숫자의 개수, M : 질문의 개수
vector<int> Arr; // 입력받은 숫자 저장
vector<int> Segment_Tree; // 세그먼트 트리 저장
vector<pair<int, int>> Cmd; // pair<int,int> : 정수 두 개를 하나로 묶는 자료형

int Min(int A, int B) { // 두 수 중 더 작은 값을 반환
    if (A < B) return A;
    return B;
} // min(A, B)만 써도 무관

void Input() { // 입력 받는 함수
    cin >> N >> M;

    // reserve : 벡터의 용량을 미리 확보하는 것
    Arr.reserve(N);
    Cmd.reserve(M);
    
    // N 개의 숫자를 입력받음
    for (int i = 0; i < N; i++) {
        int a; // 숫자 하나를 잠깐 저장
        cin >> a; // 입력받기
        Arr.push_back(a); // 배열에 추가
    }
    
    // 질문 M개 입력받기
    for (int i = 0; i < M; i++) {
        int a, b;
        cin >> a >> b;
        Cmd.push_back(make_pair(a, b)); // 질의를 (a, b)형태로 묶어서 cmd에 저장
    }
}

// Node : 현재 트리에서 몇 번 노드인지, Start, End: 현재 노드가 원본 배열의 어느 구간을 담당하는지
// 현재 트리 노드가 Start ~ End 구간의 최소값을 저장하게 만들어라
int Make_SegmentTree(int Node, int Start, int End) {
    if (Start == End) { // 리프노드 (구간 길이가 1이라는 뜻)
        Segment_Tree[Node] = Arr[Start]; // 현재 노드에 그 값 저장
        return Segment_Tree[Node]; // 그 값을 반환
    }
    // 리프노드면 배열의 한 칸만 담당하고 있는 것이기 때문에 그 구간의 최소값은 그 값 자체임

    int Mid = (Start + End) / 2;
    int Left_Result = Make_SegmentTree(Node * 2, Start, Mid);
    int Right_Result = Make_SegmentTree(Node * 2 + 1, Mid + 1, End);
    // 왼쪽 구간 Start ~ Mid의 최소값을 구하고
    // 오른쪽 구간 Mid+1 ~ End의 최소값을 구하는 것

    Segment_Tree[Node] = Min(Left_Result, Right_Result);
    return Segment_Tree[Node];
    // 현재 노드가 담당하는 전체 구간의 최소값은 왼쪽 구간 최소값과 오른쪽 구간 최소값 중 더 작은 값
    // 두 값을 비교해서 현재 노드에 저장하고 그 값을 반환
    // 아래 구간부터 차례대로 올라오면서 각 노드에 "그 구간의 최소값"을 채워 넣는 함수
}

// 구간 최소값 질의를 처리하는 함수
// 현재 노드가 담당하는 구간과 우리가 원하는 구간이 얼마나 겹치는지를 보면서 최솟값을 찾아간다
int Query(int Node, int Start, int End, int Left, int Right) {
    if (Right < Start || Left > End) return (int)2e9; // 구간이 전혀 겹치지 않음 (문제의 최댓값:10억-> 20억을 반환함으로써 Min()에 반해가 안되게 아주 큰 값을 반환해버림)
    if (Left <= Start && End <= Right) return Segment_Tree[Node]; // 현재 구간이 질의 구간 안에 완전히 포함됨
    // 더 쪼갤 필요 없이 segment_Tree[Node]에 이미 저장된 그 구간의 최솟값 반환 -> 세그먼트 트리의 핵심 장점 !

    // 이제 일부만 걸쳐 있는 경우
    // 왼쪽 자식, 오른쪽 자식으로 내려가면서 찾기 -> 왼쪽 구간의 최솟값, 오른쪽 구간의 최솟값을 각각 저장
    int Mid = (Start + End) / 2;
    int Left_Result = Query(Node * 2, Start, Mid, Left, Right);
    int Right_Result = Query(Node * 2 + 1, Mid + 1, End, Left, Right);

    // 왼쪽 결과와 오른쪽 결과 중 더 작은 값을 반환 (현재 질의 구간 전체에서 최소값이 됨)
    return Min(Left_Result, Right_Result);
}

void Solution() { // 실제 풀이를 진행하는 함수
    int Tree_Height = (int)ceil(log2(N)); // 세그먼트 트리의 높이를 구함
    // 원소가 N개 있는 이진트리의 높이는 대략 log2(N)
    // N이 2의 거듭제곱이 아닐 경우를 대비해 ceil로 올림해서 높이 구함
    int Tree_Size = (1 << (Tree_Height + 1)); // 세그먼트 트리 배열 크기를 계산
    // 1 << k 는 2^k 를 뜻함
    // 즉, 이건 2^(Tree_Height + 1) 크기 만큼 배열을 잡겠다는 뜻 (트리 배열은 보통 넉넉하게 잡음)
    Segment_Tree.resize(Tree_Size); // 세그먼트 트리 벡터 크기를 확보 -> 이제 인덱스로 접근 가능(segment_Tree[n])

    Make_SegmentTree(1, 0, N - 1); // 세그먼트 트리를 실제로 생성
    // 1 : 루트 노드부터 시작 / 0, N-1 : 원본 배열 전체 구간 -> 배열 전체에 대한 최소값 구조를 한 번에 만듦

    // 입력받아 둔 질문들을 하나씩 처리하는 반복문
    for (int i = 0; i < (int)Cmd.size(); i++) { 
        int Index = Cmd[i].first - 1;
        int Index2 = Cmd[i].second - 1;
        // 문제에서 구간 번호는 1번부터 시작하지만 배열 인덱스는 0번부터 시작함 -> 각각 -1 해주기
        // ex. 질문이 (1,3)이면 배열 인덱스로는 (0,2)가 돼.

        // 각 질문에 대해 구간 최소값을 구해서 출력
        // 루트 노드 1번부터 시작해서 전체 배열 0 ~ N-1 범위 안에서 Index ~ Index2 구간의 최소값을 구하라는 뜻
        cout << Query(1, 0, N - 1, Index, Index2) << endl;
    }
}

int main() {
    Input(); // 입력 먼저 받고 
    Solution(); // 세그먼트 트리를 만들고 질의를 처리
}

// 전체 실행 : 입력 -> 트리 생성 -> 질의 처리 -> 출력
