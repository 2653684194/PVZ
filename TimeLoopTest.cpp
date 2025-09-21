#include <iostream>
#include <chrono>
#include <thread> // 用于模拟耗时操作

int main() {
    

    while (true) {
        // 记录第一次循环的时间点
        auto prev_time = std::chrono::high_resolution_clock::now();
        // 模拟循环内的耗时操作（例如游戏逻辑、数据处理）
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 100ms 延迟

        // 记录当前时间点
        auto current_time = std::chrono::high_resolution_clock::now();

        // 计算时间差（毫秒）
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
            current_time - prev_time
        ).count();

        // 输出时间差
        std::cout << "Time between loops: " << duration << "ms" << std::endl;

        // 模拟循环内的耗时操作（例如游戏逻辑、数据处理）
        std::this_thread::sleep_for(std::chrono::milliseconds(100-duration)); // 100ms 延迟
    }

    return 0;
}
