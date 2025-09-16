#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include <map>

// �ü���� ���� ȭ���� ����� �Լ�
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// ����/���� ������ ������ ����ü
struct Transaction {
    std::string date;
    std::string category;
    double amount;
    std::string memo;
};

// ���Ͽ� ������ �����ϴ� �Լ�
void saveTransaction(const Transaction& transaction) {
    std::ofstream outFile("finance_data.txt", std::ios::app);
    if (!outFile.is_open()) {
        std::cerr << "������ �� �� �����ϴ�!" << std::endl;
        return;
    }
    outFile << transaction.date << ","
        << transaction.category << ","
        << transaction.amount << ","
        << transaction.memo << std::endl;
    outFile.close();
    std::cout << "������ ���������� ����Ǿ����ϴ�." << std::endl;
}

// ���Ͽ��� ��� ������ �ҷ����� �Լ�
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
                    std::cerr << "�߸��� �ݾ� ����: " << part << std::endl;
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

// ��� ������ ����ϴ� �Լ�
void displayTransactions(const std::vector<Transaction>& transactions) {
    if (transactions.empty()) {
        std::cout << "����� ������ �����ϴ�." << std::endl;
        return;
    }

    std::cout << "\n--- ��� �ŷ� ���� ---" << std::endl;
    std::cout << std::left << std::setw(12) << "��¥"
        << std::setw(15) << "�׸�"
        << std::setw(10) << "�ݾ�"
        << "�޸�" << std::endl;
    std::cout << "------------------------------------------" << std::endl;

    for (const auto& t : transactions) {
        std::cout << std::left << std::setw(12) << t.date
            << std::setw(15) << t.category
            << std::setw(10) << t.amount
            << t.memo << std::endl;
    }
    std::cout << "------------------------------------------\n" << std::endl;
}

// �� ����/���� �� �׸� �м� �Լ�
void analyzeTransactions(const std::vector<Transaction>& transactions) {
    if (transactions.empty()) {
        std::cout << "�м��� ������ �����ϴ�." << std::endl;
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

    std::cout << "\n--- ���� �м� ��� ---" << std::endl;
    std::cout << "�� ����: " << totalIncome << "��" << std::endl;
    std::cout << "�� ����: " << totalExpense << "��" << std::endl;
    std::cout << "���� �ݾ�: " << totalIncome + totalExpense << "��" << std::endl;
    std::cout << "----------------------" << std::endl;
    std::cout << "�׸� ����:" << std::endl;
    for (const auto& pair : categoryExpenses) {
        std::cout << "- " << pair.first << ": " << pair.second << "��" << std::endl;
    }
    std::cout << "----------------------\n" << std::endl;
}

// ���� �Լ�
int main() {
    int choice;

    while (true) {
        clearScreen(); // �޴��� �����ֱ� ���� ȭ���� ����ϴ�.
        std::cout << "----------------------------------" << std::endl;
        std::cout << "    ���� ���� ���� ���α׷�" << std::endl;
        std::cout << "----------------------------------" << std::endl;
        std::cout << "1. ���� ���" << std::endl;
        std::cout << "2. ��� ���� ��ȸ" << std::endl;
        std::cout << "3. ���� �м�" << std::endl;
        std::cout << "4. ����" << std::endl;
        std::cout << "�޴��� �����ϼ���: ";
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cout << "�߸��� �Է��Դϴ�. ���ڸ� �Է����ּ���." << std::endl;
            std::cin.clear();
            std::cin.ignore(256, '\n');
            continue;
        }

        std::cin.ignore();

        clearScreen(); // �޴� ���� �� ȭ���� ����ϴ�.

        if (choice == 1) {
            Transaction newTransaction;
            std::cout << "��¥ (YYYY-MM-DD): ";
            std::getline(std::cin, newTransaction.date);
            std::cout << "�׸� (�ĺ�, �����, ���� ��): ";
            std::getline(std::cin, newTransaction.category);
            std::cout << "�ݾ� (������ ���, ������ ����): ";
            std::cin >> newTransaction.amount;
            std::cin.ignore();
            std::cout << "�޸�: ";
            std::getline(std::cin, newTransaction.memo);

            saveTransaction(newTransaction);
            std::cout << "����Ϸ��� Enter Ű�� ��������..." << std::endl;
            std::cin.ignore();
        }
        else if (choice == 2) {
            std::vector<Transaction> allTransactions = loadTransactions();
            displayTransactions(allTransactions);
            std::cout << "����Ϸ��� Enter Ű�� ��������..." << std::endl;
            std::cin.ignore();
        }
        else if (choice == 3) {
            std::vector<Transaction> allTransactions = loadTransactions();
            analyzeTransactions(allTransactions);
            std::cout << "����Ϸ��� Enter Ű�� ��������..." << std::endl;
            std::cin.ignore();
        }
        else if (choice == 4) {
            std::cout << "���α׷��� �����մϴ�." << std::endl;
            break;
        }
        else {
            std::cout << "�߸��� �޴� �����Դϴ�. �ٽ� �õ����ּ���." << std::endl;
            std::cout << "����Ϸ��� Enter Ű�� ��������..." << std::endl;
            std::cin.ignore();
        }
    }
    return 0;
}