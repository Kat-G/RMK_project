#pragma once

#include <vector>
#include <string>
#include <optional>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>

struct Product {
    std::string product_name;
    std::string barcode;
    double price;
};

class ProductDataBase {
public:
    ProductDataBase(const std::string& filepath): filepath_(filepath) { }
    void UploadProducts();

    std::optional<Product> FindByName(const std::string& name) const;
    std::optional<Product> FindByBarcode(const std::string& barcode) const;

private:
    std::vector<Product> database_;
    std::string filepath_;
};