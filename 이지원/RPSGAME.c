#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int valid_number(char* str) {    // ���ڷθ� �����Ǿ� �ִ��� �˻� �ϴ� �Լ� ����
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '\n') continue;  // �ٹٲ� ���� �����ϴ� �ڵ�
        if (str[i] < '0' || str[i] > '9') return 0;
    }
    return 1;
}

int main() {
    double coins = 100.0; //�ʱ� ���� ���� �ڵ�
    int player, computer; // �÷��̾�� ��ǻ�� ���� ����
    char input[100]; // �Է� ���� ���� �迭 ����
    char choice[10];  // y/n ���� ���� �迭 ����

    srand(time(NULL));  // ���� �õ� �ʱ�ȭ

    printf("=== ����(1), ����(2), ��(3) ���� ===\n");
    printf("�ʱ� ����: %.1f��\n", coins);

    while (coins > 0.000001) {  //������ 0���� ũ�ٸ� ������ �����ϴ� �ݺ���
        printf("\n����(1), ����(2), ��(3) �� �����ϼ��� (0 �Է� �� ����): ");
        fgets(input, sizeof(input), stdin); // �Է��� �޴� �ڵ�

        if (!valid_number(input)) {
            printf("�߸��� �Է��Դϴ�. ���ڸ� �Է��ϼ���.\n");
            continue;
        }

        player = atoi(input);
        if (player == 0) { // 0�� �Է� ������ ������ �����ϴ� �ڵ�
            printf("0�� �Է��ϼ̽��ϴ�. ������ �����մϴ�.\n");
            break;
        }

        if (player < 1 || player > 3) { // 1~3�� �Է� �޴� �ڵ�
            printf("�߸��� �����Դϴ�. 1~3 ���̷� �Է��ϼ���.\n");
            continue;
        }

        computer = rand() % 3 + 1; // ��ǻ�Ͱ� ���� �� ���� ����
        printf("��ǻ���� ����: %s\n", computer == 1 ? "����" : (computer == 2 ? "����" : "��"));

        // ���� ���� �ڵ�
        if (player == computer) { // ��� ���� Ȯ�� �ڵ�
            printf("�����ϴ�!\n");
        }
        else if ((player == 1 && computer == 3)  // �̱� ���� Ȯ�� �ڵ�
            (player == 2 && computer == 1) ||
            (player == 3 && computer == 2)) {
            printf("�̰���ϴ�! ���� 2��!\n");
            coins *= 2;
        }
        else {
            printf("�����ϴ�... ���� 0.5��.\n"); // �� ���� Ȯ�� �ڵ�
            coins *= 0.5;
        }

        printf("���� ����: %.1f��\n", coins); //���� ���� ���

        if (coins < 0.000001) { // ������ ������ 0���� ������ ���� ����
            printf("������ ��� �����Ǿ����ϴ�. ���� ����!\n");
            break;
        }

        // ��� ���� �Է� �޴� �ڵ�
        while (1) {
            printf("��� �Ͻðڽ��ϱ�? (y/n): ");
            fgets(choice, sizeof(choice), stdin);

            if (choice[0] == 'y' || choice[0] == 'Y') {
                break;  // Ż���� ó�� while ������ �ٽ� ����
            }
            else if (choice[0] == 'n' || choice[0] == 'N') { // n�� �Է����� �� ���� ����
                printf("������ �����մϴ�. ���� ����: %.1f��\n", coins);
                return 0;
            }
            else {
                printf("�߸��� �Է��Դϴ�. y �Ǵ� n�� �Է��ϼ���.\n");
            }
        }
    }

    return 0;
}
