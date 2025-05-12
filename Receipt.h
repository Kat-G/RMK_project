#pragma once

#include <ProductDataBase.h>
#include <iostream>

struct ReceiptItem {
    Product product;
    int quantity;
};

enum class PaymentMethod {
    Cash,
    Card
};

class Receipt {
public:
    Receipt() = default;

    void AddItem(const Product& product, int quantity);

    void SetPaymentMethod(PaymentMethod method, double paid_amount = 0.0);

    void CloseReceipt();

    double GetTotal() const;

    bool IsClose() const;

    PaymentMethod GetPaymentMethod() const;

private:
    std::vector<ReceiptItem> product_list_;
    PaymentMethod payment_method_;
    double paid_amount_ = 0.0;
    bool is_payment_set_ = false;
    bool is_closed_ = false;

    double CalculateTotal() const;

    bool Payment() const;

    void PrintReceipt() const;
};
