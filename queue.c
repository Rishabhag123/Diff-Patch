#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "queue.h"

void initq(queue *q){
	q -> front = 0;
	q -> rear = 0;
	q -> c = 0;
}

int isemptyq(queue *q){
	return(q -> c == 0);
}


int isfullq(queue *q){
	return(q -> c == QMAX);
}

void enqueue(queue *q, char ch){
	if(!isfullq(q)){
		q -> c++;
		q -> arr[q -> rear] = ch;
		q -> rear = (q -> rear + 1) % QMAX;
		//printf("%c has been added\n", ch);
	}
	else {
		printf("No space \n");
		return;
	}
}

char dequeue(queue *q){
	char temp;
	temp = q -> arr[q -> front];
	q -> front = (q -> front +1) % QMAX;
	q -> c--;
	return temp;
}


