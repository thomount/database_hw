#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
using namespace std;
struct data {
	int index, value;
	int time;
	data * next=0;
	data(int _index, int _value, int _time): index(_index), value(_value), time(_time){}
	data() {}
};

struct WriteEvent {
	int index, val;
};