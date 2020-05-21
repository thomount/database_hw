#include <cstdio>
#include <cstring>
#include <ctime>
#include "proc.h"
using namespace std;
int main(int argc, char * argv[]) {
	int start_time = clock();
	int att_N;
	FILE *log = initData(att_N);
	//printf("%d\n", att_N);
	int n = 1;
	if (argc > 1)
		n = argv[1][0]-'0';
	proc * p = new proc[n];
	thread *t = new thread[n];
	for (int i = 0; i < n; i++) {
		p[i].init(att_N);
		t[i] = thread(&proc::run, &p[i], i+1);
	}
	for (int i = 0; i < n; i++)
		t[i].join();

	fclose(log);
	//printf("waiting time = %.3lf\n", get_average());
	printf("using time = %.3lf\n", 1.0*(clock()-start_time)/CLOCKS_PER_SEC);
	return 0;
}