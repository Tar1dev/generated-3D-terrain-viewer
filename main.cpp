#include <iostream>
#include <core/Application.hpp>

int main(void) {
    
    Application app;    
    if (!app.initialize()) {
        std::cerr << "Failed to init the App" << std::endl;
        return 1;
    }

    app.run();
    app.shutdown();

    return 0;
}

