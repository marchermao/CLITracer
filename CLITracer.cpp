#pragma once
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>  // 添加此头文件
#include <cstdlib>  // 包含rand函数
#include <ctime>    // 包含time函数
#include "CTLprinter.h"
using namespace std;

int main()
{
        // 初始化一个包含大数量元素的数组
        size_t num_elements = 1000;
        std::vector<double> data(num_elements, 1.0);  // 假设数组中元素全为 1.0

        // 创建进度条对象
        ProgressBar progressBar;

        // 记录开始时间
        auto start_time = std::chrono::steady_clock::now();
        // 初始化时间总量（模拟计算的总时间，方便展示）
        double time_final = 10.0;  // 假设整个计算过程应该花费 10 秒钟
        double current_time = 0.0; // 当前时间


        double p = 0.1;
        double T = 20;

        //创建变量名称列表
        vector<string> names = { "p","T" };
        //创建单位名称列表
        vector<string> name_units = { "MPa","K" };
        //创建变量列表
        vector<double> values;


        // 模拟计算过程并实时更新进度
        for (size_t i = 0; i < num_elements; ++i) {
            //清理计算结果
            values.clear();
            
            // 假设每次计算的时间为 10 毫秒
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            
            // 使用当前时间作为随机数种子生成随机数
            std::srand(std::time(0));
            double random_number = std::rand() % 100;
            p = 00.1 + 00.1 * (random_number / 100);
            T = 20.0 + 20.0 * (random_number / 100);
            //计算结果入
            values.push_back(p);
            values.push_back(T);

            // 更新时间
            current_time += 0.01;

            // 更新进度条
            progressBar.outputProgressBar(current_time, time_final, start_time);
           
            //一行7列打印计算结果
            progressBar.outputParaTable(names, name_units, values,7);

            //条状图打印计算结果
            progressBar.outputBar(names, name_units, values);

            //输出进度条
            progressBar.print();
        }
        
      
        return 0;
    
};
