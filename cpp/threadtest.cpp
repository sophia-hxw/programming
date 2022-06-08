#include <iostream>
#include <atomic>
#include <thread>
#include <vector>

using namespace std;

atomic<int> Counter(0);
int order[400];

void work(int id)
{
	int no;
	for (int i = 0; i < 100; i++) {
		no = Counter++;
		order[no] = id;
	}
}

int main(int argc, char* argv[])
{
	vector<thread> threads;
	//创建多线程访问计数器
	for (int i = 0; i != 4; ++i)
		//线程工作函数与线程标记参数
		threads.push_back(thread(work, i));
	for (auto & th : threads)
		th.join();
	//最终的计数值
	cout << "final :" << Counter << endl;
	//观察各线程的工作时序
	for (int i = 0; i < 400; i++)
		cout << "[" << i << "]=" << order[i] << " ";
	system("Pause");
	return 0;
}