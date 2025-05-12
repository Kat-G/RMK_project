#include "MainApp.h"

int main(int argc, char* argv[]) {
    MainApp app("products.csv");

    if (argc > 1 && std::string(argv[1]) == "--interactive") {
        app.RunInteractive();
    } else {
        app.RunScript();
    }
}