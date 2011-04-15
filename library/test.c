#include "list.c"

int main() {
	list *l;
	initList(l);
	int i,j;
	i = 10;
	while (i-->0) {
		listPushBack(l, (void*)&i);
	}

/*
	i=10;
	while(i-->0){
		listPopBack(l, (void*)&j);
		printf("%d  ",j);
	}
	*/
}
