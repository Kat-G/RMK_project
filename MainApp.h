#pragma once

#include <Shift.h>
#include <ProductDataBase.h>

class MainApp {
public:
    MainApp(const std::string& database_path): db_(database_path) {
        db_.UploadProducts();
    }

    bool InitShiftFromScript();
    void OpenReceipt(Receipt& receipt, bool& is_receipt_open);
    void AddByBarcode(std::stringstream& ss, Receipt& receipt, bool is_receipt_open);
    void AddByName(std::stringstream& ss, Receipt& receipt, bool is_receipt_open);
    void Pay(std::stringstream& ss, Receipt& receipt, bool& is_receipt_open);
    bool CloseShift(bool is_receipt_open);

    void RunScript();
    void RunInteractive();

private:
    ProductDataBase db_;
    Shift shift_;
};