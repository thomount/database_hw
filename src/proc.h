#include <mutex>
#include <thread>
#include <cstdio>
#include <cstring>

//#include <windows.h>

#include "func.h"
using namespace std;


mutex mylock;
class proc {
	FILE * f, * fo;
	int start_time = 0;
	int tid, id;
	vector<WriteEvent> wv;
	int * temp = 0;
	int * changed = 0;
	int n;
	int size;
public:
	void init(int _n) {
		n = _n+2;
		int x;
		size = sizeof(x)*n;
		temp = new int[n];
		changed = new int[n];
		memset(temp, 0, size);
		memset(changed, 0, size);
	}
	void run(int _id) {
		wv.clear();
		//printf("start process %d\n", _id);
		id = _id;
		char cid = '0'+_id;
		string iname = string("thread_")+cid+".txt";
		string oname = string("output_thread_")+cid+".csv";
		f = fopen(iname.c_str(), "r");
		fo = fopen(oname.c_str(), "w");
		fprintf(fo, "transaction_id,type,time,value\n");
		char op[10];
		char name1[10], name2[10];
		while (fscanf(f, "%s", op) >= 0) {
			//printf("%s\n", op);
			if (op[0] == 'B') {		//BEGIN
				fscanf(f, "%d", &tid);
				//continue;
				start_time = start();
				//fprintf(fo, "Begin\n");
				fprintf(fo, "%d,BEGIN,%d,\n", tid, start_time);
				memset(temp, 0, size);
				memset(changed, 0, size);
			}
			if (op[0] == 'C') {		//COMMIT
				int eid;
				fscanf(f, "%d", &eid);
				//continue;
				wv.clear();
				for (int i = 0; i < n; i++) if (changed[i] == 2) wv.push_back({i, temp[i]});
				int ctime = commit(wv, id);
				fprintf(fo, "%d,END,%d,\n", tid, ctime);
			}
			if (op[0] == 'R') {		//READ
				fscanf(f, "%s", name1);
				int index = get_name(name1);
				//continue;
				if (!changed[index]) {
					temp[index] = read(index, start_time);
					changed[index] = 1;
				}
				fprintf(fo, "%d,%s,%d,%d\n", tid, name1, get_time(), temp[index]);
			}
			if (op[0] == 'S') {		//SET
				char sign[2];
				int x;
				fscanf(f, "%s%s%s%d", name1, name2, sign, &x);
				name1[strlen(name1)-1] = 0;
				int dst = get_name(name1);
				int src = get_name(name2);
				//continue;
				if (!changed[src]) {
					temp[src] = read(src, start_time);
					changed[src] = 1;
				}
				int val = temp[src] + ((sign[0] == '+')?x:-x);
				temp[dst] = val;
				changed[dst] = 2;
			}

		}
		fclose(f);
		fclose(fo);

	}
};
