#include "App.hpp"
#include "BufferPool.hpp"
#include <chrono>

int main() {


    App app;
    app.init();
    app.run();
    app.terminate();

    return EXIT_SUCCESS;
}