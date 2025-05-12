#include "Receipt.h"

void Receipt::AddItem(const Product& product, int quantity) {
    product_list_.push_back({product, quantity});
}

void Receipt::SetPaymentMethod(PaymentMethod method, double paid_amount) {
    payment_method_ = method;
    paid_amount_ = paid_amount;
    is_payment_set_ = true;
}

void Receipt::CloseReceipt() {
    if (!is_payment_set_) {
        std::cerr << "Ошибка: способ оплаты не задан\n";
        return;
    }

    if (!Payment()) {
        std::cerr << "Ошибка: оплата не прошла\n";
        return;
    }

    is_closed_ = true;
    PrintReceipt();
}

double Receipt::GetTotal() const {
    return CalculateTotal();
}

bool Receipt::IsClose() const {
    return is_closed_;
}

PaymentMethod Receipt::GetPaymentMethod() const {
    return payment_method_;
}

double Receipt::CalculateTotal() const {
        double total = 0.0;
        for (const auto& item : product_list_) {
            total += item.quantity * item.product.price;
        }
        return total;
    }

bool Receipt::Payment() const {
    std::cout << "Оплата... ";
    double total = CalculateTotal();

    if (payment_method_ == PaymentMethod::Cash) {
        if (paid_amount_ < total) {
            std::cerr << "Ошибка: недостаточно наличных\n";
            return false;
        }
        std::cout << "Сдача: " << paid_amount_ - total << "\n";
    }
    std::cout << "Оплачено\n";
    return true;
}

void Receipt::PrintReceipt() const {
    std::cout << "-- Чек --\n";
    for (const auto& item : product_list_) {
        std::cout << item.product.product_name << " | "
                    << "Цена: " << item.product.price << " | "
                    << "Кол-во: " << item.quantity << "\n";
    }

    std::cout << "Итог: " << CalculateTotal() << "\n";
    if (payment_method_ == PaymentMethod::Cash) {
        std::cout << "Сдача: " << paid_amount_ - CalculateTotal() << "\n";
    }
}