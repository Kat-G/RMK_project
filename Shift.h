#pragma once

#include <Receipt.h>

class Shift {
public:
    Shift() { }
    void OpenShift(const std::string& cashier_name, double initial_cash);
    void AddReceipt(const Receipt& receipt);
    void CloseShift();
    void PrintSummary() const;

private:
    std::string cashier_name_;
    double initial_cash_;
    std::vector<Receipt> receipt_info_;

    double CalcSumBy(PaymentMethod method) const;
};
