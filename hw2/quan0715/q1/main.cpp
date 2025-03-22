#include "line.hpp"
#include <stdexcept>
#include <iostream>

int main() {
    // 創建一個包含 3 個點的線
    Line line(3);
    
    // 設置點的坐標
    line.x(0) = 0; line.y(0) = 1;
    line.x(1) = 1; line.y(1) = 3;
    line.x(2) = 2; line.y(2) = 5;
    
    // 輸出第一條線的點
    std::cout << "line: number of points = " << line.size() << std::endl;
    for (size_t i = 0; i < line.size(); ++i) {
        std::cout << "point " << i << ": x = " << line.x(i) 
                  << " y = " << line.y(i) << std::endl;
    }
    
    // 創建第二條線（通過拷貝構造函數）
    Line line2 = line;
    line2.x(0) = 9;  // 修改第一個點的 x 坐標
    
    // 輸出第二條線的點
    std::cout << "line2: number of points = " << line2.size() << std::endl;
    for (size_t i = 0; i < line2.size(); ++i) {
        std::cout << "point " << i << ": x = " << line2.x(i) 
                  << " y = " << line2.y(i) << std::endl;
    }
    
    return 0;
}