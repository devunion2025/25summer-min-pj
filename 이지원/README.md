#베팅 가위바위보 게임

```c
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int valid_number(char* str) {    // 숫자로만 구성되어 있는지 검사 하는 함수 선언
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '\n') continue;  // 줄바꿈 문자 무시하는 코드
        if (str[i] < '0' || str[i] > '9') return 0;
    }
    return 1;
}

int main() {
    double coins = 100.0; //초기 코인 설정 코드
    int player, computer; // 플레이어와 컴퓨터 변수 선언
    char input[100]; // 입력 변수 받을 배열 선언
    char choice[10];  // y/n 응답 받을 배열 선언

    srand(time(NULL));  // 랜덤 시드 초기화

    printf("=== 가위(1), 바위(2), 보(3) 게임 ===\n");
    printf("초기 코인: %.1f개\n", coins);

    while (coins > 0.000001) {  //코인이 0보다 크다면 게임을 지속하는 반복문
        printf("\n가위(1), 바위(2), 보(3) 중 선택하세요 (0 입력 시 종료): ");
        fgets(input, sizeof(input), stdin); // 입력을 받는 코드

        if (!valid_number(input)) {
            printf("잘못된 입력입니다. 숫자만 입력하세요.\n");
            continue;
        }

        player = atoi(input);
        if (player == 0) { // 0을 입력 받으면 게임을 종료하는 코드
            printf("0을 입력하셨습니다. 게임을 종료합니다.\n");
            break;
        }

        if (player < 1 || player > 3) { // 1~3을 입력 받는 코드
            printf("잘못된 숫자입니다. 1~3 사이로 입력하세요.\n");
            continue;
        }

        computer = rand() % 3 + 1; // 컴퓨터가 내는 것 랜덤 설정
        printf("컴퓨터의 선택: %s\n", computer == 1 ? "가위" : (computer == 2 ? "바위" : "보"));

        // 승패 판정 코드
        if (player == computer) { // 비김 판정 확인 코드
            printf("비겼습니다!\n");
        }
        else if ((player == 1 && computer == 3)  // 이김 판정 확인 코드
            (player == 2 && computer == 1) ||
            (player == 3 && computer == 2)) {
            printf("이겼습니다! 코인 2배!\n");
            coins *= 2;
        }
        else {
            printf("졌습니다... 코인 0.5배.\n"); // 진 판정 확인 코드
            coins *= 0.5;
        }

        printf("현재 코인: %.1f개\n", coins); //현재 코인 출력

        if (coins < 0.000001) { // 소지한 코인이 0보다 적으면 게임 종료
            printf("코인이 모두 소진되었습니다. 게임 종료!\n");
            break;
        }

        // 계속 여부 입력 받는 코드
        while (1) {
            printf("계속 하시겠습니까? (y/n): ");
            fgets(choice, sizeof(choice), stdin);

            if (choice[0] == 'y' || choice[0] == 'Y') {
                break;  // 탈출후 처음 while 문에서 다시 시작
            }
            else if (choice[0] == 'n' || choice[0] == 'N') { // n을 입력했을 시 게임 종료
                printf("게임을 종료합니다. 최종 코인: %.1f개\n", coins);
                return 0;
            }
            else {
                printf("잘못된 입력입니다. y 또는 n만 입력하세요.\n");
            }
        }
    }

    return 0;
}




## C언어

## 이지원
