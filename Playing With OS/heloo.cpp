//TODO: Print from 4 differnt threads sequencially

#include <stdio.h>
#include <pthread.h>

void *hello(void *arg){
	int *p = (int*) arg;
	int i = *p;
	printf("heloo %d\n",i);
	return 0;
}

int main()
{
int i;
int a[4];
pthread_t tid[4];
for(i=0;i<4;i++){
	a[i]=i;
	if(pthread_create(&tid[i],NULL,hello,&a[i])==0){
	//==	printf("created %d\n",i);
	}
}
for(i=0;i<4;i++){
	pthread_join(tid[i],NULL);
}
return 0;
}

