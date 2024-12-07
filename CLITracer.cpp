#pragma once
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>  // ��Ӵ�ͷ�ļ�
#include <cstdlib>  // ����rand����
#include <ctime>    // ����time����
#include "CTLprinter.h"
using namespace std;

int main()
{
        // ��ʼ��һ������������Ԫ�ص�����
        size_t num_elements = 1000;
        std::vector<double> data(num_elements, 1.0);  // ����������Ԫ��ȫΪ 1.0

        // ��������������
        ProgressBar progressBar;

        // ��¼��ʼʱ��
        auto start_time = std::chrono::steady_clock::now();
        // ��ʼ��ʱ��������ģ��������ʱ�䣬����չʾ��
        double time_final = 10.0;  // ���������������Ӧ�û��� 10 ����
        double current_time = 0.0; // ��ǰʱ��


        double p = 0.1;
        double T = 20;

        //�������������б�
        vector<string> names = { "p","T" };
        //������λ�����б�
        vector<string> name_units = { "MPa","K" };
        //���������б�
        vector<double> values;


        // ģ�������̲�ʵʱ���½���
        for (size_t i = 0; i < num_elements; ++i) {
            //���������
            values.clear();
            
            // ����ÿ�μ����ʱ��Ϊ 10 ����
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            
            // ʹ�õ�ǰʱ����Ϊ������������������
            std::srand(std::time(0));
            double random_number = std::rand() % 100;
            p = 00.1 + 00.1 * (random_number / 100);
            T = 20.0 + 20.0 * (random_number / 100);
            //��������
            values.push_back(p);
            values.push_back(T);

            // ����ʱ��
            current_time += 0.01;

            // ���½�����
            progressBar.outputProgressBar(current_time, time_final, start_time);
           
            //һ��7�д�ӡ������
            progressBar.outputParaTable(names, name_units, values,7);

            //��״ͼ��ӡ������
            progressBar.outputBar(names, name_units, values);

            //���������
            progressBar.print();
        }
        
      
        return 0;
    
};
