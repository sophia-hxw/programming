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
	//�������̷߳��ʼ�����
	for (int i = 0; i != 4; ++i)
		//�̹߳����������̱߳�ǲ���
		threads.push_back(thread(work, i));
	for (auto & th : threads)
		th.join();
	//���յļ���ֵ
	cout << "final :" << Counter << endl;
	//�۲���̵߳Ĺ���ʱ��
	for (int i = 0; i < 400; i++)
		cout << "[" << i << "]=" << order[i] << " ";
	system("Pause");
	return 0;
}