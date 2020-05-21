#include <mutex>
#include <map>
#include <ctime>
#include "func.h"
mutex proclock;
mutex rwlock;
mutex tmlock;

FILE * flog;
int st_sec = 0;
int last = 0;
int get_time(){								//获取当前时间
	return last++;
}

struct database {
	vector<data *> head;
	data d[200000];
	int tot;
	database() {head.clear(); tot = 0;}
	data * new_data(int index, int val, int time) {
		d[tot].index = index;
		d[tot].value = val;
		d[tot].time = time;
		return &d[tot++];
	}
} db;
struct counter {
	int rt = 0, rc = 0;
	int wt = 0;
} ct;
int read(int index, int time){				//读取index数据time时的版本
	//return 0;
	rwlock.lock();
	data * now = db.head[index];
	rwlock.unlock();
	//ct.rt ++;
	//ct.rc ++;
	while (now && now->time > time) {
		now = now->next;
		//ct.rc ++;
	}
	if (now) return now->value; 
}
//void get_lock(int index){					//获取index数据的写锁
//}
//void release_lock(int index){				//释放index数据的写锁
//}
void write(int index, int val){	//在time时刻向index写入data数据
	//return;
	data * ndata = db.new_data(index, val, get_time());
	//fprintf(flog, "%d %d %d\n", index, val, ndata->time);
	rwlock.lock();
	ndata->next = db.head[index];
	db.head[index] = ndata;
	rwlock.unlock();
}
void init(int index, int data){				//初始化index数据为data
	write(index, data);
}
/*
oper parse(string s){						//根据s获得指令

}
*/
map<unsigned int, int> mp;
static int tot = 0;
int get_name(char * s) {
	
	unsigned int hash = 0;
	for (int i = 0; i < strlen(s); i++) hash = (hash<<5)|s[i]-'A';
	if (!mp[hash]) mp[hash] = ++tot;
	if (mp[hash]-1 == 4) {
		printf("error : %s\n", s);
	}
	return mp[hash]-1;
	//return s[strlen(s)-1]-'A';
}

int start() {
	//ct.wt -= clock();
	proclock.lock();
	int ret = get_time();
	proclock.unlock();
	//ct.wt += clock();
	return ret;
}

int commit(vector<WriteEvent> & wv, int id) {
	//return 0;
	//ct.wt -= clock();
	proclock.lock();
	int ret = get_time();
	for (WriteEvent w: wv) {
		write(w.index, w.val);
	}
	proclock.unlock();
	//ct.wt += clock();
	return ret;
}

FILE * initData(int & n) {
	FILE * f = fopen("data_prepare.txt", "r");
	flog = fopen("log.txt", "w");
	char op[20], name[20];
	int val;
	n = 0;
	while (fscanf(f, "%s%s%d", op, name, &val) >= 0) {
		n++;
		while(db.head.size() <= n)
			db.head.push_back(nullptr);
		//printf("%s\n", name);
		init(get_name(name), val);
	}
	st_sec = get_time();
	//printf("%d\n", st_sec);
	fclose(f);

}

double get_average() {
	return 1.0*ct.wt/CLOCKS_PER_SEC;
}