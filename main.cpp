#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include <map>

// 운영체제에 따라 화면을 지우는 함수
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// 지출/수입 내역을 저장할 구조체
struct Transaction {
    std::string date;
    std::string category;
    double amount;
    std::string memo;
};

// 파일에 내역을 저장하는 함수
void saveTransaction(const Transaction& transaction) {
    std::ofstream outFile("finance_data.txt", std::ios::app);
    if (!outFile.is_open()) {
        std::cerr << "파일을 열 수 없습니다!" << std::endl;
        return;
    }
    outFile << transaction.date << ","
        << transaction.category << ","
        << transaction.amount << ","
        << transaction.memo << std::endl;
    outFile.close();
    std::cout << "내역이 성공적으로 저장되었습니다." << std::endl;
}

// 파일에서 모든 내역을 불러오는 함수
std::vector<Transaction> loadTransactions() {
    std::vector<Transaction> transactions;
    std::ifstream inFile("finance_data.txt");
    if (!inFile.is_open()) {
        return transactions;
    }

    std::string line;
    while (std::getline(inFile, line)) {
        Transaction t;
        std::stringstream ss(line);
        std::string part;
        int partCount = 0;

        while (std::getline(ss, part, ',')) {
            switch (partCount) {
            case 0: t.date = part; break;
            case 1: t.category = part; break;
            case 2:
                try {
                    t.amount = std::stod(part);
                }
                catch (const std::invalid_argument& e) {
                    std::cerr << "잘못된 금액 형식: " << part << std::endl;
                    continue;
                }
                break;
            case 3: t.memo = part; break;
            }
            partCount++;
        }
        transactions.push_back(t);
    }
    inFile.close();
    return transactions;
}

// 모든 내역을 출력하는 함수
void displayTransactions(const std::vector<Transaction>& transactions) {
    if (transactions.empty()) {
        std::cout << "저장된 내역이 없습니다." << std::endl;
        return;
    }

    std::cout << "\n--- 모든 거래 내역 ---" << std::endl;
    std::cout << std::left << std::setw(12) << "날짜"
        << std::setw(15) << "항목"
        << std::setw(10) << "금액"
        << "메모" << std::endl;
    std::cout << "------------------------------------------" << std::endl;

    for (const auto& t : transactions) {
        std::cout << std::left << std::setw(12) << t.date
            << std::setw(15) << t.category
            << std::setw(10) << t.amount
            << t.memo << std::endl;
    }
    std::cout << "------------------------------------------\n" << std::endl;
}

// 총 수입/지출 및 항목별 분석 함수
void analyzeTransactions(const std::vector<Transaction>& transactions) {
    if (transactions.empty()) {
        std::cout << "분석할 내역이 없습니다." << std::endl;
        return;
    }

    double totalIncome = 0.0;
    double totalExpense = 0.0;
    std::map<std::string, double> categoryExpenses;

    for (const auto& t : transactions) {
        if (t.amount > 0) {
            totalIncome += t.amount;
        }
        else {
            totalExpense += t.amount;
            categoryExpenses[t.category] += t.amount;
        }
    }

    std::cout << "\n--- 재정 분석 결과 ---" << std::endl;
    std::cout << "총 수입: " << totalIncome << "원" << std::endl;
    std::cout << "총 지출: " << totalExpense << "원" << std::endl;
    std::cout << "남은 금액: " << totalIncome + totalExpense << "원" << std::endl;
    std::cout << "----------------------" << std::endl;
    std::cout << "항목별 지출:" << std::endl;
    for (const auto& pair : categoryExpenses) {
        std::cout << "- " << pair.first << ": " << pair.second << "원" << std::endl;
    }
    std::cout << "----------------------\n" << std::endl;
}

// 메인 함수
int main() {
    int choice;

    while (true) {
        clearScreen(); // 메뉴를 보여주기 전에 화면을 지웁니다.
        std::cout << "----------------------------------" << std::endl;
        std::cout << "    개인 재정 관리 프로그램" << std::endl;
        std::cout << "----------------------------------" << std::endl;
        std::cout << "1. 내역 기록" << std::endl;
        std::cout << "2. 모든 내역 조회" << std::endl;
        std::cout << "3. 재정 분석" << std::endl;
        std::cout << "4. 종료" << std::endl;
        std::cout << "메뉴를 선택하세요: ";
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cout << "잘못된 입력입니다. 숫자를 입력해주세요." << std::endl;
            std::cin.clear();
            std::cin.ignore(256, '\n');
            continue;
        }

        std::cin.ignore();

        clearScreen(); // 메뉴 선택 후 화면을 지웁니다.

        if (choice == 1) {
            Transaction newTransaction;
            std::cout << "날짜 (YYYY-MM-DD): ";
            std::getline(std::cin, newTransaction.date);
            std::cout << "항목 (식비, 교통비, 수입 등): ";
            std::getline(std::cin, newTransaction.category);
            std::cout << "금액 (수입은 양수, 지출은 음수): ";
            std::cin >> newTransaction.amount;
            std::cin.ignore();
            std::cout << "메모: ";
            std::getline(std::cin, newTransaction.memo);

            saveTransaction(newTransaction);
            std::cout << "계속하려면 Enter 키를 누르세요..." << std::endl;
            std::cin.ignore();
        }
        else if (choice == 2) {
            std::vector<Transaction> allTransactions = loadTransactions();
            displayTransactions(allTransactions);
            std::cout << "계속하려면 Enter 키를 누르세요..." << std::endl;
            std::cin.ignore();
        }
        else if (choice == 3) {
            std::vector<Transaction> allTransactions = loadTransactions();
            analyzeTransactions(allTransactions);
            std::cout << "계속하려면 Enter 키를 누르세요..." << std::endl;
            std::cin.ignore();
        }
        else if (choice == 4) {
            std::cout << "프로그램을 종료합니다." << std::endl;
            break;
        }
        else {
            std::cout << "잘못된 메뉴 선택입니다. 다시 시도해주세요." << std::endl;
            std::cout << "계속하려면 Enter 키를 누르세요..." << std::endl;
            std::cin.ignore();
        }
    }
    return 0;
}