#include<stdio.h>
#define QMAX 50

typedef struct queue{
	char arr[QMAX];
	int front, rear;
	int c;
}queue;
void initq(queue *q);
int isemptyq(queue *q);
int isfullq(queue *q);
void enqueue(queue *q, char a);
char dequeue(queue *q);

