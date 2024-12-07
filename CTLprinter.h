#pragma once
#include <cmath>			//��ѧ��
#include <vector>			//������
#include <string>			//�ַ���
#include <iostream>			//���������
#include <algorithm>		//�㷨
#include <chrono>			//ʱ�����
#include <numeric>			//��ֵ����
#include <sstream>    // ostringstream
#include <iomanip>
using namespace std;
class ProgressBar {
private:
	int progressBarLines; // ������ռ�õ�����
	ostringstream output; // �Զ�����Ϣ��
	size_t count;			//��Ϣ������

	std::string redColor;
	std::string greenColor;
	std::string yellowColor;
	std::string resetColor;

	void outputParaTableByLine(vector<string> names, vector<string> name_units, vector<double> values)
	{
		// �������������
		size_t num_columns = names.size();
		size_t wide = 12;
		size_t setprecision_num = 4;
		// �����ͷ�ͷָ���
		for (size_t i = 0; i < num_columns; ++i)
		{
			output << "+" << std::string(wide + 2, '-'); // ��ȼ��������ո�һ��������߽磬һ�������ұ߽�
		}
		output << "+\n";
		// �����ͷ
		for (size_t i = 0; i < num_columns; ++i)
		{
			output << "| " << std::setw(wide) << names[i] << " ";
		}
		output << "|\n";

		// �����λ�ͷָ���
		for (size_t i = 0; i < num_columns; ++i)
		{
			output << "| " << std::setw(wide) << name_units[i] << " ";
		}
		output << "|\n";

		// �����ֵ�ͷָ���
		for (size_t i = 0; i < num_columns; ++i)
		{
			output << "| " << std::setw(wide) << std::setprecision(setprecision_num) << values[i] << " ";
		}
		output << "|\n";

		// ����ײ��ָ���
		for (size_t i = 0; i < num_columns; ++i)
		{
			output << resetColor << "+" << std::string(wide + 2, '-'); // ��ȼ��������ո�һ��������߽磬һ�������ұ߽�
		}
		output << "+\n";
	}
	void hideCursor() {
		std::cout << "\x1b[?25l"; // ���ع��
	}
	void showCursor() {
		std::cout << "\x1b[?25h"; // ��ʾ���
	}
	double nextPowerOfTen(double num) {
		if (num <= 0) {
			return 0.0; // ���벻�Ϸ�������0
		}

		int exponent = static_cast<int>(std::ceil(std::log10(num))); // ��������ָ�����֣�����λ����
		return std::pow(10, exponent); // ���ش��ڸ�������С�� 10 �� n ����
	}
public:
	ProgressBar() : progressBarLines(1) {
		// ���������ռ�õ����������������ռ��һ�У�
		// ��ʼ����ɫ���Ʊ���
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
		// �������������
		double progress = (time_final != 0) ? (static_cast<double>(time) / time_final * 50) : 0;

		// ��ӡ������
		output << "[";

		// ���ݽ���ѡ����ɫ
		if (progress <10) {
			output << redColor; // ����С��20%ʱʹ�ú�ɫ
		}
		else if (progress < 30) {
			output << yellowColor; // ����С��80%ʱʹ�û�ɫ
		}
		else {
			output << greenColor; // ���ȴ��ڵ���80%ʱʹ����ɫ
		}

		for (int j = 0; j < static_cast<int>(progress); ++j) {
			output << "=";
		}

		// ��ӡ��תб��
		static char slash[] = { '/', '-', '\\', '|' };
		static int idx = 0;
		output << ">" << slash[idx];

		// ����ո��Ա��̶ֹ����
		for (int j = static_cast<int>(progress) + 1; j < 50; ++j) {
			output << " ";
		}

		// �ָ�Ĭ����ɫ
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
		idx = (idx + 1) % 4; // ����б��λ��
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
		// �ҵ�ÿ�����������ֵ������ȷ����״ͼ�ı���
		vector<double> max_values;
		for (size_t i = 0; i < names.size(); ++i) {
			max_values.push_back(nextPowerOfTen(values[i]));
		}

		// �ҵ�������ƺ͵�λ�ĳ��ȣ���ȷ������Ķ���λ��
		size_t max_name_length = 0;
		size_t max_unit_length = 0;
		for (const auto& name : names) {
			max_name_length = std::max(max_name_length, name.length());
		}
		for (const auto& unit : name_units) {
			max_unit_length = std::max(max_unit_length, unit.length());
		}

		// ���ÿ��������ֱ�߹��
		for (size_t i = 0; i < names.size(); ++i) {
			// ���㵱ǰ����ֵ��ֱ���ϵĹ��λ��
			int cursor_position = static_cast<int>((values[i] / max_values[i]) * 50);

			// ����������͵�λ�����룩
			output << std::setw(max_name_length) << std::left << names[i] << " (" << std::setw(max_unit_length) << std::left << name_units[i] << "): [";

			// ���ֱ���ϵĹ��
			for (int j = 0; j < 50; ++j) {
				if (j == cursor_position) {
					output << "��";
				}
				else {
					output << "��";
				}
			}

			// �������ֵ
			output << "] " << std::setprecision(2) << std::setw(7) << std::right << values[i] << "/" << std::left << max_values[i] << "\n";
		}
	}
	void outputParaTable(vector<string> names, vector<string> name_units, vector<double> values, size_t num_columns)
	{
		// �������ݵ�����
		size_t num_rows = names.size() / num_columns;
		if (names.size() % num_columns != 0) {
			num_rows++; // ������ݲ�����������������Ҫ�����һ��
		}

		// �ָ����ݲ�ѭ������outputPara_table��ӡ���
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
		// ͳ����Ϣ��������
		std::string str = output.str();
		count = std::count(str.begin(), str.end(), '\n');

		// �����Ϣ���� std::cout
		std::cout << output.str();
		cout.unsetf(ios::fixed);
		output.str(""); // �����Ϣ������

		// ʹ�� ANSI ת���������֮ǰ������
		std::cout << "\x1b[" << count << "A"; // ������ƶ�����һ�������λ��
		//std::cout << "\x1b[K"; // ����ӹ��λ�õ���β����

		
	};

};