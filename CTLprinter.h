#pragma once
#include <cmath>			//数学库
#include <vector>			//向量库
#include <string>			//字符串
#include <iostream>			//输入输出流
#include <algorithm>		//算法
#include <chrono>			//时间控制
#include <numeric>			//数值方法
#include <sstream>    // ostringstream
#include <iomanip>
using namespace std;
class ProgressBar {
private:
	int progressBarLines; // 进度条占用的行数
	ostringstream output; // 自定义信息流
	size_t count;			//信息流行数

	std::string redColor;
	std::string greenColor;
	std::string yellowColor;
	std::string resetColor;

	void outputParaTableByLine(vector<string> names, vector<string> name_units, vector<double> values)
	{
		// 计算输出的列数
		size_t num_columns = names.size();
		size_t wide = 12;
		size_t setprecision_num = 4;
		// 输出表头和分隔线
		for (size_t i = 0; i < num_columns; ++i)
		{
			output << "+" << std::string(wide + 2, '-'); // 宽度加上两个空格，一个用于左边界，一个用于右边界
		}
		output << "+\n";
		// 输出表头
		for (size_t i = 0; i < num_columns; ++i)
		{
			output << "| " << std::setw(wide) << names[i] << " ";
		}
		output << "|\n";

		// 输出单位和分隔线
		for (size_t i = 0; i < num_columns; ++i)
		{
			output << "| " << std::setw(wide) << name_units[i] << " ";
		}
		output << "|\n";

		// 输出数值和分隔线
		for (size_t i = 0; i < num_columns; ++i)
		{
			output << "| " << std::setw(wide) << std::setprecision(setprecision_num) << values[i] << " ";
		}
		output << "|\n";

		// 输出底部分隔线
		for (size_t i = 0; i < num_columns; ++i)
		{
			output << resetColor << "+" << std::string(wide + 2, '-'); // 宽度加上两个空格，一个用于左边界，一个用于右边界
		}
		output << "+\n";
	}
	void hideCursor() {
		std::cout << "\x1b[?25l"; // 隐藏光标
	}
	void showCursor() {
		std::cout << "\x1b[?25h"; // 显示光标
	}
	double nextPowerOfTen(double num) {
		if (num <= 0) {
			return 0.0; // 输入不合法，返回0
		}

		int exponent = static_cast<int>(std::ceil(std::log10(num))); // 计算数的指数部分（整数位数）
		return std::pow(10, exponent); // 返回大于该数的最小的 10 的 n 次幂
	}
public:
	ProgressBar() : progressBarLines(1) {
		// 计算进度条占用的行数（假设进度条占用一行）
		// 初始化颜色控制变量
		redColor = "\033[31m";
		greenColor = "\033[32m";
		yellowColor = "\033[33m";
		resetColor = "\033[0m";
	}
	~ProgressBar(){
		for (size_t i = 0; i < count; i++)
		{
			cout << endl;
		}

	}
	void outputProgressBar(double time, double time_final, std::chrono::steady_clock::time_point start_time) {
		// 计算进度条长度
		double progress = (time_final != 0) ? (static_cast<double>(time) / time_final * 50) : 0;

		// 打印进度条
		output << "[";

		// 根据进度选择颜色
		if (progress <10) {
			output << redColor; // 进度小于20%时使用红色
		}
		else if (progress < 30) {
			output << yellowColor; // 进度小于80%时使用黄色
		}
		else {
			output << greenColor; // 进度大于等于80%时使用绿色
		}

		for (int j = 0; j < static_cast<int>(progress); ++j) {
			output << "=";
		}

		// 打印旋转斜杠
		static char slash[] = { '/', '-', '\\', '|' };
		static int idx = 0;
		output << ">" << slash[idx];

		// 补充空格以保持固定宽度
		for (int j = static_cast<int>(progress) + 1; j < 50; ++j) {
			output << " ";
		}

		// 恢复默认颜色
		output << resetColor;

		auto current_time = std::chrono::steady_clock::now();
		std::chrono::duration<double> elapsed_seconds = current_time - start_time;
		double precent_all = time / time_final;
		output << "]" << std::fixed << std::setprecision(2) << precent_all * 100 << "%(" << std::setprecision(3) << time << "/" << time_final << ") ";
		output << "\n";
		output << "Time: " << std::setprecision(3) << elapsed_seconds.count() << "s";
		double remaining_time = elapsed_seconds.count() * (1 / precent_all - 1);
		if (remaining_time <= 0)
		{
			remaining_time = 0;
		}
		output << " Remaining: " << std::setprecision(3) << remaining_time << "s"<<"      ";
		output << "\n";
		idx = (idx + 1) % 4; // 更新斜杠位置
	}
	void outputPara(vector<string>names, vector<string>name_units, vector<double> values)
	{
		for (auto name : names)
		{
			output << name << "\t";
		}
		output << "\n";
		for (auto name_unit : name_units)
		{
			output << name_unit << "\t";
		}
		output << "\n";
		for (auto valve : values)
		{
			output << std::setprecision(2) << valve << "\t";
		}
		output << "\n";
	}
	void outputBar(vector<string> names, vector<string> name_units, vector<double> values)
	{
		// 找到每个参数的最大值，用于确定条状图的比例
		vector<double> max_values;
		for (size_t i = 0; i < names.size(); ++i) {
			max_values.push_back(nextPowerOfTen(values[i]));
		}

		// 找到最长的名称和单位的长度，以确定输出的对齐位置
		size_t max_name_length = 0;
		size_t max_unit_length = 0;
		for (const auto& name : names) {
			max_name_length = std::max(max_name_length, name.length());
		}
		for (const auto& unit : name_units) {
			max_unit_length = std::max(max_unit_length, unit.length());
		}

		// 输出每个参数的直线光标
		for (size_t i = 0; i < names.size(); ++i) {
			// 计算当前参数值在直线上的光标位置
			int cursor_position = static_cast<int>((values[i] / max_values[i]) * 50);

			// 输出参数名和单位（对齐）
			output << std::setw(max_name_length) << std::left << names[i] << " (" << std::setw(max_unit_length) << std::left << name_units[i] << "): [";

			// 输出直线上的光标
			for (int j = 0; j < 50; ++j) {
				if (j == cursor_position) {
					output << "┿";
				}
				else {
					output << "━";
				}
			}

			// 输出参数值
			output << "] " << std::setprecision(2) << std::setw(7) << std::right << values[i] << "/" << std::left << max_values[i] << "\n";
		}
	}
	void outputParaTable(vector<string> names, vector<string> name_units, vector<double> values, size_t num_columns)
	{
		// 计算数据的行数
		size_t num_rows = names.size() / num_columns;
		if (names.size() % num_columns != 0) {
			num_rows++; // 如果数据不能整除列数，则需要多添加一行
		}

		// 分割数据并循环调用outputPara_table打印表格
		size_t start_index = 0;
		for (size_t i = 0; i < num_rows; ++i) {
			size_t end_index = std::min((i + 1) * num_columns, names.size());
			vector<string> sub_names(names.begin() + start_index, names.begin() + end_index);
			vector<string> sub_units(name_units.begin() + start_index, name_units.begin() + end_index);
			vector<double> sub_values(values.begin() + start_index, values.begin() + end_index);
			outputParaTableByLine(sub_names, sub_units, sub_values);
			start_index = end_index;
		}
	}
	void print()
	{
		// 统计信息流的行数
		std::string str = output.str();
		count = std::count(str.begin(), str.end(), '\n');

		// 输出信息流到 std::cout
		std::cout << output.str();
		cout.unsetf(ios::fixed);
		output.str(""); // 清空信息流内容

		// 使用 ANSI 转义序列清除之前的内容
		std::cout << "\x1b[" << count << "A"; // 将光标移动到上一个输出的位置
		//std::cout << "\x1b[K"; // 清除从光标位置到行尾内容

		
	};

};