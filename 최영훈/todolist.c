#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LEN 100         //문자열 최대 길이

//구조체 연결리스트 정의
typedef struct Todo {
    char task[MAX_LEN];      //내용
    char date[11];        // 날짜 YYYY-MM-DD 형식의 /0까지 11자
    struct Todo *next;    //다음 노드 포인터
} Todo;

Todo *head = NULL;   // 연결 리스트 시작 지점

// 날짜 비교 함수(문자열 비교 불가능 하기에 정 수 변환)
int compareDate(const char *date1, const char *date2) {
    int d1 = atoi(date1) * 10000 + atoi(date1 + 5) * 100 + atoi(date1 + 8);
    int d2 = atoi(date2) * 10000 + atoi(date2 + 5) * 100 + atoi(date2 + 8);
    return d1 - d2;
}

//할 일 추가 함수(뒷 추가)
void addTodo(const char *task, const char *date) {
    Todo *newTodo = (Todo *)malloc(sizeof(Todo));
    if (!newTodo) {
        printf("메모리 할당 실패\n");
        return;
    }
    
	//내용 복사
    strcpy(newTodo->task, task);
    strcpy(newTodo->date, date);
    newTodo->next = NULL;
	
	// 리스트가 비었을 예외 고려(헤드에 추가)
    if (head == NULL) {
        head = newTodo;
    } else { //포인터를 계속 타고 들어가 나오지 않을 때 추가(= 맨뒤에 추가)
        Todo *temp = head;
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = newTodo;
    }

    printf("할 일이 추가되었습니다.\n");
}

//현재 날짜기준 D-Day체크
void printRemainingTime(const char *targetDate) {
    // "2025-08-15" 같은 문자열에서 연도, 월, 일 분리
	int year, month, day;
	sscanf(targetDate, "%d-%d-%d", &year, &month, &day);

	// 구조체에 할당
	struct tm deadline = {0};
	deadline.tm_year = year - 1900; // tm 구조체는 1900년 기준
	deadline.tm_mon = month - 1;    // 0 ~ 11 범위
	deadline.tm_mday = day;
	deadline.tm_hour = 0;
	deadline.tm_min = 0;
	deadline.tm_sec = 0;
    deadline.tm_hour = 0; deadline.tm_min = 0; deadline.tm_sec = 0;  // 자정 기준

    time_t now = time(NULL);  //현재 시간 가져오기
    time_t targetTime = mktime(&deadline); //마감일 시간 단위로 변환

    double seconds = difftime(targetTime, now); //초 단위로 변환 이후 차이 계산
    int days = (int)(seconds / (60 * 60 * 24));  //일 수 계산
    int hours = (int)(seconds / (60 * 60)) % 24; // 남은 것 시간으로 계산

    if (seconds < 0) {
        printf(" (마감 지남)");
    } else {
        printf(" (D-%d일 %d시간)", days, hours);
    }
}

// 할 일 출력 (날짜 순 정렬까지)
void printTodosByDate() {
    if (head == NULL) {
        printf("할 일이 없습니다.\n");
        return;
    }

	//원본 리스트 정렬준비(원본 손상 가지 않게 복사)
    Todo *sorted = NULL;
    Todo *curr = head;

    while (curr != NULL) {
        Todo *newNode = (Todo *)malloc(sizeof(Todo));  //노드 복사
        *newNode = *curr;
        newNode->next = NULL;
		
		//날짜 기준 정렬 삽입
        if (sorted == NULL || compareDate(newNode->date, sorted->date) < 0) {   //sorted가 비어있음 or 맨 앞보다 앞설 경우
            newNode->next = sorted;
            sorted = newNode;
        } else {    //newNode가 중간이나 뒤에 들어가야 할때
            Todo *temp = sorted;
            while (temp->next != NULL && compareDate(newNode->date, temp->next->date) >= 0)
                temp = temp->next;
            newNode->next = temp->next;   //데이터 삽입 부분(위에 있는 조건은 위치 찾는 코드)
            temp->next = newNode;
        }

        curr = curr->next;
    }
	
	//정렬 리스트 출력
    printf("\n[날짜순 할 일 목록 + 남은 시간]\n");
    int index = 1;
    Todo *temp = sorted;
    while (temp != NULL) {
        printf("%d. [%s] - %s", index++, temp->date, temp->task);
        printRemainingTime(temp->date);  // 남은 시간 표시
        printf("\n");
        temp = temp->next;
    }

    // 정렬된 리스트 해제(해제 이유:정렬이 완료되었으니 메모리낭비 방지를 위해 해제)
    while (sorted != NULL) {
        Todo *next = sorted->next;
        free(sorted);
        sorted = next;
    }
}

//delete함수
void deleteTodo(int index) {
    if (head == NULL) {
        printf("삭제할 할 일이 없습니다.\n");
        return;
    }

    Todo *temp = head;
    Todo *prev = NULL;
	
	//첫 번쨰 노드 삭제 처리
    if (index == 1) {
        head = temp->next;
        free(temp);
        printf("1번 항목이 삭제되었습니다.\n");
        return;
    }
	
	//삭제 노드 탐색
    for (int i = 1; temp != NULL && i < index; i++) {
        prev = temp;
        temp = temp->next;
    }
	
	//없을 경우 에러코드 출력
    if (temp == NULL) {
        printf("해당 번호의 할 일이 없습니다.\n");
    } else {                    //노드 삭제: 이전 노드가 다음 노드를 건너뛰도록 연결 재설정
        prev->next = temp->next;
        free(temp);
        printf("%d번 항목이 삭제되었습니다.\n", index);
    }
}
	
	// 삭제 전 원본 리스트 출력 함수
	void printOriginalList() {
	    if (head == NULL) {
	        printf("할 일이 없습니다.\n");
	        return;
    	}
    	printf("\n[현재 할 일 목록]\n");
    	int index = 1;
    	Todo *temp = head;
	    while (temp != NULL) {
	        printf("%d. [%s] - %s\n", index++, temp->date, temp->task);
    	    temp = temp->next;
    	}
	}

// 메모리 낭비 방지를 위한 프로그램 종료 시 메모리 해제
void freeTodos() {
    Todo *temp = head;
    while (temp != NULL) {
        Todo *next = temp->next;
        free(temp);
        temp = next;
    }
    head = NULL;
}

int main() {
    int choice;
    char task[MAX_LEN];
    char date[11];
    int delIndex;

    while (1) {
        printf("\n==== TODO 리스트 ====\n");
        printf("1. 할 일 추가\n");
        printf("2. 할 일 출력 (날짜순 + 남은 시간)\n");
        printf("3. 할 일 삭제\n");
        printf("4. 종료\n");
        printf("선택: ");
        scanf("%d", &choice);
        getchar(); // 개행 문자 제거

        switch (choice) {
            case 1:
                printf("할 일을 입력하세요: ");
                fgets(task, MAX_LEN, stdin);
                task[strcspn(task, "\n")] = '\0';   // fgets 개행문자 제거

                printf("날짜 입력 (YYYY-MM-DD): ");
                fgets(date, sizeof(date), stdin);
                date[strcspn(date, "\n")] = '\0';  // fgets 개행문자 제거

                addTodo(task, date);
                break;

            case 2:
                printTodosByDate();
                break;

            case 3:
			    printOriginalList(); 
			    printf("삭제할 항목 번호 입력: ");
			    scanf("%d", &delIndex);
				getchar();
			    deleteTodo(delIndex);
			    break;

            case 4:
                freeTodos();   //메모리 해제
                printf("프로그램을 종료합니다.\n");
                return 0;

            default:
                printf("잘못된 입력입니다.\n");
        }
    }

    return 0;
}
