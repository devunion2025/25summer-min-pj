#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// 주어진 수익률로 최종 자본금을 계산하는 함수
double calculate_final_capital(int total_months, double initial_capital, double monthly_contribution, double monthly_return_rate, int include_first_month) {
    double current_capital = initial_capital;
    int months_with_contribution = include_first_month ? total_months : total_months - 1;
    for (int month = 1; month <= total_months; month++) {
        current_capital += current_capital * monthly_return_rate;
        if (month <= months_with_contribution) {
            current_capital += monthly_contribution;
        }
    }
    return current_capital;
}

// 목표 수익을 달성하기 위한 월별 수익률을 찾는 함수 (이분 탐색)
double find_monthly_rate(int total_months, double initial_capital, double monthly_contribution, double target_profit, int include_first_month) {
    double low_rate = 0.0;
    double high_rate = 1.0;
    double monthly_return_rate = 0.0;
    const int ITERATIONS = 100;

    double total_investment = initial_capital + monthly_contribution * (include_first_month ? total_months : total_months - 1);
    double target_final_capital = total_investment + target_profit;

    for (int i = 0; i < ITERATIONS; i++) {
        monthly_return_rate = (low_rate + high_rate) / 2.0;
        double current_final_capital = calculate_final_capital(total_months, initial_capital, monthly_contribution, monthly_return_rate, include_first_month);

        if (current_final_capital < target_final_capital) {
            low_rate = monthly_return_rate;
        }
        else {
            high_rate = monthly_return_rate;
        }
    }
    return high_rate;
}

int main(int argc, char* argv[]) {
    if (argc != 6) {
        fprintf(stderr, "Usage: %s <total_months> <initial_capital> <monthly_contribution> <target_profit> <include_first_month>\n", argv[0]);
        return 1;
    }

    int total_months = atoi(argv[1]);
    double initial_capital = atof(argv[2]);
    double monthly_contribution = atof(argv[3]);
    double target_profit = atof(argv[4]);
    int include_first_month = atoi(argv[5]);

    double monthly_return_rate = find_monthly_rate(total_months, initial_capital, monthly_contribution, target_profit, include_first_month);
    double total_investment = initial_capital + monthly_contribution * (include_first_month ? total_months : total_months - 1);
    double final_capital = calculate_final_capital(total_months, initial_capital, monthly_contribution, monthly_return_rate, include_first_month);
    double total_profit = final_capital - total_investment;

    // JSON 형식으로 결과 출력
    printf("{\"monthly_rate\":%.10f,\"total_investment\":%.2f,\"final_capital\":%.2f,\"total_profit\":%.2f,\"monthly_data\":[",
        monthly_return_rate,
        total_investment,
        final_capital,
        total_profit
    );

    double current_capital = initial_capital;
    double total_profit_sum = 0.0;
    for (int month = 1; month <= total_months; month++) {
        double profit_this_month = current_capital * monthly_return_rate;
        total_profit_sum += profit_this_month;

        printf("{\"month\":%d,\"capital\":%.2f,\"profit\":%.2f}%s",
            month,
            current_capital,
            profit_this_month,
            month == total_months ? "" : ",");

        current_capital += profit_this_month;
        if (include_first_month || (!include_first_month && month < total_months)) {
            current_capital += monthly_contribution;
        }
    }
    printf("], \"total_profit_sum\": %.2f}\n", total_profit_sum);
    return 0;
}