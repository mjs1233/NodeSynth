#include "App.hpp"
#include "BufferPool.hpp"
#include <chrono>

class ScopedTimer {
private:
    std::string_view timer_name;
    std::chrono::time_point<std::chrono::steady_clock> start_time;

public:
    ScopedTimer(std::string_view name)
        : timer_name(name), start_time(std::chrono::steady_clock::now()) {
    }

    ~ScopedTimer() {
        auto end_time = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

        std::cout << "[Timer: " << timer_name << "] "
            << duration.count() << " us\n";
    }
};

void run() {
    //BufferPool<int> buf_pool(512, 1024);
    ScopedTimer timer("Buffer Pool");


    for (size_t idx = 0; idx < 1024; idx++) {

        std::vector<int> buf;
        buf.resize(512);
        for (size_t idx = 0; idx < 512; idx++)
            buf[idx] = 0;

        buf.clear();
    }


}

int main() {

    
    run();
    return 0;

    App app;
    app.init();
    app.run();
    app.terminate();

    return EXIT_SUCCESS;
}