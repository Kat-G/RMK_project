#include "Shift.h"

void Shift::OpenShift(const std::string& cashier_name, double initial_cash) {
    cashier_name_ = cashier_name; 
    initial_cash_ = initial_cash;
}

void Shift::AddReceipt(const Receipt& receipt) {
    receipt_info_.push_back(receipt);
    initial_cash_ += receipt.GetTotal();
}

void Shift::CloseShift() {
    PrintSummary();
    receipt_info_.clear(); 
}

void Shift::PrintSummary() const {
    std::cout << "Кассир: " << cashier_name_ << "\n";
    std::cout << "Всего чеков: " << receipt_info_.size() << "\n";
    std::cout << "Сумма за наличную оплату : " << CalcSumBy(PaymentMethod::Cash) << " рублей\n";
    std::cout << "Сумма за оплату картой: " << CalcSumBy(PaymentMethod::Card) << " рублей\n";
    std::cout << "Сумма в кассе на окончание смену: " << initial_cash_ << " рублей\n";
}

double Shift::CalcSumBy(PaymentMethod method) const {
    double sum = 0.0;
    for (auto receipt: receipt_info_) {
        if (receipt.GetPaymentMethod() == method) {
            sum += receipt.GetTotal();
        }
    }
    return sum;
}