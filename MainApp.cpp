#include "MainApp.h"

bool MainApp::InitShiftFromScript() {
    std::string line;
    if (!std::getline(std::cin, line)) return false;

    std::stringstream first_line(line);
    std::string command;
    first_line >> command;

    if (command != "open_shift") {
        std::cerr << "Ошибка: смена не открыта\n";
        return false;
    }

    std::string cashier_name;
    int initial_cash;
    if (!(first_line >> cashier_name >> initial_cash)) {
        std::cerr << "Ошибка: некорректные параметры open_shift\n";
        return false;
    }

    shift_.OpenShift(cashier_name, initial_cash);
    std::cout << "Смена открыта\n";
    return true;
}

void MainApp::OpenReceipt(Receipt& receipt, bool& is_receipt_open) {
    if (is_receipt_open) {
        std::cerr << "Ошибка: чек уже открыт\n";
        return;
    }
    receipt = Receipt();
    is_receipt_open = true;
    std::cout << "Чек открыт\n";
}

void MainApp::AddByBarcode(std::stringstream& ss, Receipt& receipt, bool is_receipt_open) {
    if (!is_receipt_open) {
        std::cerr << "Ошибка: чек не открыт\n";
        return;
    }
    std::string barcode;
    int quantity;
    if (!(ss >> barcode >> quantity)) {
        std::cerr << "Ошибка: неверный формат команды add_by_barcode\n";
        return;
    }
    auto product_opt = db_.FindByBarcode(barcode);
    if (!product_opt.has_value()) {
        std::cerr << "Ошибка: товар со штрихкодом \"" << barcode << "\" не найден\n";
        return;
    }
    receipt.AddItem(*product_opt, quantity);

    std::cout << "Товар добавлен: " << product_opt.value().product_name << " | "
                    << "Цена: " << product_opt.value().price << "\n";
}

void MainApp::AddByName(std::stringstream& ss, Receipt& receipt, bool is_receipt_open) {
    if (!is_receipt_open) {
        std::cerr << "Ошибка: чек не открыт\n";
        return;
    }
    std::string name;
    int quantity;
    if (!(ss >> name >> quantity)) {
        std::cerr << "Ошибка: неверный формат команды add_by_name\n";
        return;
    }
    auto product_opt = db_.FindByName(name);
    if (!product_opt.has_value()) {
        std::cerr << "Ошибка: товар с именем \"" << name << "\" не найден\n";
        return;
    }
    receipt.AddItem(*product_opt, quantity);

    std::cout << "Товар добавлен: " << product_opt.value().product_name << " | "
                    << "Цена: " << product_opt.value().price << "\n";
}

void MainApp::Pay(std::stringstream& ss, Receipt& receipt, bool& is_receipt_open) {
    std::string method;
    if (!(ss >> method)) {
        std::cerr << "Ошибка: метод оплаты не указан\n";
        return;
    }

    if (method == "cash") {
        double amount;
        if (!(ss >> amount)) {
            std::cerr << "Ошибка: сумма оплаты не указана\n";
            return;
        }
        receipt.SetPaymentMethod(PaymentMethod::Cash, amount);
    } else if (method == "card") {
        receipt.SetPaymentMethod(PaymentMethod::Card);
    } else {
        std::cerr << "Ошибка: неизвестный метод оплаты\n";
        return;
    }

    receipt.CloseReceipt();
    if (receipt.IsClose()) {
        shift_.AddReceipt(receipt);
        is_receipt_open = false;
    } else {
        std::cout << "Повторите оплату\n";
    }
}

bool MainApp::CloseShift(bool is_receipt_open) {
    if (is_receipt_open) {
        std::cerr << "Внимание: есть открытый чек, который не был оплачен\n";
        return false;
    } else {
        std::cout << "Итог по смене:\n";
        shift_.CloseShift();
        std::cout << "Смена закрыта\n";
        return true;
    }
}

void MainApp::RunScript() {
    if (!InitShiftFromScript()) return;

    Receipt receipt;
    bool is_receipt_open = false;

    std::string line;
    while (std::getline(std::cin, line)) {
        std::stringstream ss(line);
        std::string cmd;
        ss >> cmd;

        if (cmd == "open_receipt") {
            OpenReceipt(receipt, is_receipt_open);
        } else if (cmd == "add_by_barcode") {
            AddByBarcode(ss, receipt, is_receipt_open);
        } else if (cmd == "add_by_name") {
            AddByName(ss, receipt, is_receipt_open);
        } else if (cmd == "pay") {
            Pay(ss, receipt, is_receipt_open);
        } else if (cmd == "cancel_receipt") {
            std::cout << "Чек отменён\n";
            is_receipt_open = false;
        } else if (cmd == "close_shift") {
            if (CloseShift(is_receipt_open)) break;
        } else if (cmd == "exit") {
            if (!is_receipt_open) break;
            std::cerr << "Внимание: есть открытый чек, который не был оплачен\n";
        } else if (cmd == "open_shift") {
            std::cerr << "Ошибка: смена уже открыта\n";
        } else {
            std::cerr << "Ошибка: неизвестная команда '" << cmd << "'\n";
        }
    }
}

void MainApp::RunInteractive() {
    std::string cashier_name;
    int initial_cash = 0;

    std::cout << "=== Открытие смены ===\n";
    std::cout << "Введите имя кассира: ";
    std::getline(std::cin, cashier_name);
    std::cout << "Введите начальную сумму в кассе: ";
    std::string cash_str;
    std::getline(std::cin, cash_str);
    try {
        initial_cash = std::stoi(cash_str);
    } catch (...) {
        std::cerr << "Ошибка: сумма должна быть числом\n";
        return;
    }
    shift_.OpenShift(cashier_name, initial_cash);
    std::cout << "Смена открыта\n";

    bool running = true;
    bool is_receipt_open = false;
    Receipt receipt;

    while (running) {
        if (!is_receipt_open) {
            std::cout << "\nВыберите действие:\n";
            std::cout << "1. Открыть чек\n";
            std::cout << "2. Закрыть смену\n";
            std::cout << "3. Выход\n";
            std::cout << "> ";
            std::string choice;
            std::getline(std::cin, choice);

            if (choice == "1") {
                receipt = Receipt();
                is_receipt_open = true;
                std::cout << "Чек открыт\n";
            } else if (choice == "2") {
                std::cout << "Итог по смене:\n";
                shift_.CloseShift();
                std::cout << "Смена закрыта\n";
                running = false;
            } else if (choice == "3") {
                running = false;
            } else {
                std::cerr << "Ошибка: неизвестный выбор\n";
            }
        } else {
            std::cout << "\nВыберите действие с чеком:\n";
            std::cout << "1. Добавить товар по штрихкоду\n";
            std::cout << "2. Добавить товар по имени\n";
            std::cout << "3. Оплатить\n";
            std::cout << "4. Отменить чек\n";
            std::cout << "> ";
            std::string choice;
            std::getline(std::cin, choice);

            if (choice == "1") {
                std::cout << "Введите штрихкод и количество: ";
                std::string input;
                std::getline(std::cin, input);
                std::stringstream ss(input);
                std::string barcode;
                int quantity;
                if (!(ss >> barcode >> quantity)) {
                    std::cerr << "Ошибка: введите строку вида 123456 2\n";
                    continue;
                }
                auto product_opt = db_.FindByBarcode(barcode);
                if (!product_opt) {
                    std::cerr << "Ошибка: товар не найден\n";
                    continue;
                }
                receipt.AddItem(*product_opt, quantity);
                std::cout << "Товар добавлен: " << product_opt.value().product_name << " | "
                    << "Цена: " << product_opt.value().price << "\n";
            } else if (choice == "2") {
                std::cout << "Введите имя товара и количество: ";
                std::string input;
                std::getline(std::cin, input);
                std::stringstream ss(input);
                std::string name;
                int quantity;
                if (!(ss >> name >> quantity)) {
                    std::cerr << "Ошибка: введите строку вида молоко 2\n";
                    continue;
                }
                auto product_opt = db_.FindByName(name);
                if (!product_opt) {
                    std::cerr << "Ошибка: товар не найден\n";
                    continue;
                }
                receipt.AddItem(*product_opt, quantity);
                std::cout << "Товар добавлен: " << product_opt.value().product_name << " | "
                    << "Цена: " << product_opt.value().price << "\n";
            } else if (choice == "3") {
                std::cout << "Выберите метод оплаты:\n1. Наличные\n2. Карта\n> ";
                std::string method_choice;
                std::getline(std::cin, method_choice);

                if (method_choice == "1") {
                    std::cout << "Введите сумму: ";
                    std::string amount_str;
                    std::getline(std::cin, amount_str);
                    double amount = std::stod(amount_str);
                    receipt.SetPaymentMethod(PaymentMethod::Cash, amount);
                } else if (method_choice == "2") {
                    receipt.SetPaymentMethod(PaymentMethod::Card);
                } else {
                    std::cerr << "Ошибка: неизвестный метод оплаты\n";
                    continue;
                }

                receipt.CloseReceipt();
                if (receipt.IsClose()) {
                    shift_.AddReceipt(receipt);
                    std::cout << "Чек оплачен и закрыт\n";
                    is_receipt_open = false;
                } else {
                    std::cerr << "Повторите оплату\n";
                }

            } else if (choice == "4") {
                std::cout << "Чек отменён\n";
                is_receipt_open = false;
            } else {
                std::cerr << "Ошибка: неизвестный выбор\n";
            }
        }
    }

    std::cout << "Программа завершена\n";
}
