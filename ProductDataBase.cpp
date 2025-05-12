#include "ProductDataBase.h"

void ProductDataBase::UploadProducts() { 
    std::ifstream file(filepath_);

    if (!file.is_open()) { //Может быть сделать потом общий обработчик ошибок
        std::cerr << "Ошибка: не удалось открыть файл товаров\n";
        std::exit(1);
    }
    std::string line;
    std::getline(file, line);

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string name, barcode, price_str;

        if (!std::getline(ss, name, ',')) continue;
        if (!std::getline(ss, barcode, ',')) continue;
        if (!std::getline(ss, price_str, ',')) continue;

        try {
            double price = std::stod(price_str);
            database_.push_back(Product{name, barcode, price});
        } catch (const std::invalid_argument&) {
            std::cerr << "Ошибка: некорректная цена в строке: " << line << "\n";
        }
    }

    file.close();
}

std::optional<Product> ProductDataBase::FindByName(const std::string& name) const { //поиск товара по названию
    for (auto product: database_) {
        if (product.product_name == name) {
            return product;
        }
    }
    return std::nullopt;
}

std::optional<Product> ProductDataBase::FindByBarcode(const std::string& barcode) const { //поиск товара по штрихкоду
    for (auto product: database_) {
        if (product.barcode == barcode) {
            return product;
        }
    }
    return std::nullopt;
}