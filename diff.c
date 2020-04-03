#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>
#include "stack.h"
#include "queue.h"

int i, j, c[200][200];
char b[200][200];
struct stat filestat1;
struct stat filestat2; 
stack s1, s2;
queue q1, q2;
int cflag;					

void diff(char *x[], char *y[], int i, int j){		//recursive backtracking
	int l1, l, count = 0;
	if(i > 0 && j > 0 && b[i][j] == 'd'){
		push(&s1, j);
		push(&s2, i);
		diff(x, y, i - 1, j - 1);
	}
	else if(j > 0 && (i == 0 || b[i][j] == 'l')){
		diff(x, y, i, j - 1);
	}
	else if(i > 0 && (j == 0 || b[i][j] == 'u')){
		diff(x, y, i - 1, j);
	}		
}

void addatstart(char *x[], char *y[], int a, int b){	
	int i;
	if(b == 2){
		if(cflag != 0)
			enqueue(&q2, '+');
		else{
			printf("0a1\n");
			printf("> %s", y[0]);
		}
	}
	else{
		if(cflag != 0){
			for(i = 0; i < (b - 1); i++){
				enqueue(&q2, '+');
			}
		}
		else{
			printf("0a1,%d\n", b - 1);
			for(i = 0; i < (b - 1); i++){
				printf("> %s", y[i]);
			}
		}
	}
}

void delatstart(char *x[], char *y[], int a, int b){
	int i;
	if(a == 2){
		if(cflag != 0)
			enqueue(&q1, '-');
		else{	
			printf("1d0\n");
			printf("< %s", x[0]);
		}
	}
	else{
		if(cflag != 0){
			for(i = 0; i < (a - 1); i++){
				enqueue(&q1, '-');
			}
		}
		else{
			printf("1,%dd0\n", a - 1);
			for(i = 0; i < (a - 1); i++){
				printf("< %s", x[i]);
			}
		}
	}
}

void changeatstart(char *x[], char *y[], int a, int b){
	int i;
	if(a == 2 && b == 2){
		if(cflag != 0){
			enqueue(&q1, '!');
			enqueue(&q2, '!');
		}
		else{
			printf("1c1\n");
			printf("< %s", x[0]);
			printf("---\n");
			printf("> %s", y[0]);
		}
	}
	else if(a == 2 && b != 2){
		if(cflag != 0){
			enqueue(&q1, '!');
			for(i = 0; i < (b - 1); i++)
				enqueue(&q2, '!');
		}
		else{
			printf("1c1,%d\n", b - 1);
			printf("< %s", x[0]);
			printf("---\n");
			for(i = 0; i < (b - 1); i++)
				printf("> %s", y[i]);
		}
	}
	else if(a != 2 && b == 2){
		if(cflag != 0){
			enqueue(&q2, '!');
			for(i = 0; i < (a - 1); i++)
				enqueue(&q1, '!');
		}
		else{
			printf("1,%dc1\n", a - 1);
			for(i = 0; i < (a - 1); i++)
				printf("< %s", x[i]);
			printf("---\n");
			printf("> %s", y[0]);
		}
	}
	else if(a != 2 && b != 2){
		if(cflag != 0){
			for(i = 0; i < (a - 1); i++)
				enqueue(&q1, '!');
			for(i = 0; i < (a - 1); i++)
				enqueue(&q2, '!');
		}
		else{
			printf("1,%dc1,%d\n", a - 1, b - 1);
			for(i = 0; i < (a - 1); i++)
				printf("< %s", x[i]);
			printf("---\n");
			for(i = 0; i < (b - 1); i++)
				printf("> %s", y[i]);
		}
	}
}

void addatmid(char *x[], char *y[],int tempa, int tempb, int a, int b){
	if(b - tempb == 2){
		if(cflag != 0){
			enqueue(&q2, '+');
		}
		else{
			printf("%da%d\n", a - 1, b - 1);
			printf("> %s", y[b - 2]);
		}
	}
	else{
		if(cflag != 0){
			for(i = tempb; i < b - 1; i++)
				enqueue(&q2, '+');
		}
		else{
			printf("%da%d,%d\n", a - 1, tempb + 1, b - 1);
			for(i = tempb; i < b - 1; i++)
				printf("> %s", y[i]);
		}
	}
}

void delatmid(char *x[], char *y[], int tempa, int tempb, int a, int b){
	if(a - tempa == 2){
		if(cflag != 0){
			enqueue(&q1, '-');
		}
		else{
			printf("%dd%d\n", a - 1, b - 1);
			printf("< %s", x[a - 2]);
		}
	}
	else{
		if(cflag != 0){
			for(i = tempa; i < a - 1; i++)	
				enqueue(&q1, '-');
		}
		else{
			printf("%d,%dd%d\n", tempa + 1, a - 1, b - 1);
			for(i = tempa; i < a - 1; i++)
				printf("< %s", x[i]);
		}
	}
}

void changeatmid(char *x[], char *y[], int tempa, int tempb, int a, int b){
	if(a - tempa == 2 && b - tempb == 2){
		if(cflag != 0){
			enqueue(&q1, '!');
			enqueue(&q2, '!');
		}
		else{
			printf("%dc%d\n", a - 1, b - 1);
			printf("< %s", x[a - 2]);
			printf("---\n");
			printf("> %s", y[b - 2]);
		}
	}
	else if(a - tempa != 2 && b - tempb == 2){
		if(cflag != 0){
			for(i = tempa; i < a - 1; i++)
				enqueue(&q1, '!');
			enqueue(&q2, '!');
		}
		else{
			printf("%d,%dc%d\n", tempa + 1, a - 1, b - 1);
			for(i = tempa; i < a - 1; i++)
				printf("< %s", x[i]);
			printf("---\n");
			printf("> %s", y[b - 2]);
		}
	}
	else if(a - tempa == 2 && b - tempb != 2){
		if(cflag != 0){
			for(i = tempb; i < b - 1; i++)
				enqueue(&q2, '!');
			enqueue(&q1, '!');
		}
		else{
			printf("%dc%d,%d\n", a - 1, tempb + 1, b - 1);
			printf("< %s", x[i]);
			printf("---\n");
			for(i = tempb; i < b - 1; i++)
				printf("> %s", y[i]);
		}
	}
	else{
		if(cflag != 0){
			for(i = tempa; i < a - 1; i++)
				enqueue(&q1, '!');
			for(i = tempb; i < b - 1; i++)
				enqueue(&q2, '!');	
		}
		else{
			printf("%d,%dc%d,%d\n", tempa + 1, a - 1, tempb + 1, b - 1);
			for(i = tempa; i < a - 1; i++)
				printf("< %s", x[i]);
			printf("---\n");
			for(i = tempb; i < b - 1; i++)
				printf("> %s", y[i]);
		}
	}
}

void addatend(char *x[], char *y[], int n, int a, int b){
	int i;
	if(b == n - 1){
		if(cflag != 0){
			enqueue(&q2, '+');
		}
		else{
			printf("%da%d\n", a, n);
			printf("> %s", y[n - 1]);
		}
	}
	else{
		if(cflag != 0){
			for(i = b; i < n; i++)
				enqueue(&q2, '+');
		}
		else{
			printf("%da%d,%d\n", a, b + 1, n);
			for(i = b; i < n; i++){
				printf("> %s", y[i]);
			}
		}	
	}
}

void delatend(char *x[], char *y[], int m, int a, int b){
	int i;
	if(a == m - 1){
		if(cflag != 0){
			enqueue(&q1, '-');
		}
		else{
			printf("%dd%d\n", m, b);
			printf("< %s", x[m - 1]);
		}
	}
	else{
		if(cflag != 0){
			for(i = a; i < m; i++)
				enqueue(&q1, '-');
		}
		else{
			printf("%d,%dd%d\n", a + 1, m, b);
			for(i = a; i < m; i++){
				printf("< %s", x[i]);
			}	
		}
	}
}

void changeatend(char *x[], char *y[], int m, int n, int a, int b){
	int i;
	if(a == m - 1 && b == n - 1){
		if(cflag != 0){
			enqueue(&q1, '!');
			enqueue(&q2, '!');
		}
		else{
			printf("%dc%d\n", m, n);
			printf("< %s", x[m - 1]);
			printf("---\n");
			printf("> %s", y[n - 1]);
		}	
	}
	else if(a == m - 1 && b != n - 1){
		if(cflag != 0){
			enqueue(&q1, '!');
			for(i = b; i < n; i++)
				enqueue(&q2, '!');
		}
		else{
			printf("%dc%d,%d\n", m, b + 1, n);
			printf("< %s", x[m - 1]);
			printf("---\n");
			for(i = b; i < n; i++)
				printf("> %s", y[i]);
		}
	}
	else if(a != m - 1 && b == n - 1){
		if(cflag != 0){
			enqueue(&q2, '!');
			for(i = a; i < m; i++)
				enqueue(&q1, '!');
		}
		else{
			printf("%d,%dc%d\n", a + 1, m, n);
			for(i = a; i < m; i++)
				printf("< %s", x[i]);
			printf("---\n");
			printf("> %s", y[n - 1]);
		}
	}
	else if(a != m - 1 && b != n - 1){
		if(cflag != 0){
			for(i = a; i < m; i++)
				enqueue(&q1, '!');
			for(i = b; i < n; i++)
				enqueue(&q2, '!');
				
		}
		else{
			printf("%d,%dc%d,%d\n", a + 1, m, b + 1, n);
			for(i = a; i < m; i++)
				printf("< %s", x[i]);
			printf("---\n");
			for(i = b; i < n; i++)
				printf("> %s", y[i]);
		}
	}
}

void change(char *x[], char *y[], int m, int n){
	if(cflag != 0){
		for(i = 0; i < m; i++)
			enqueue(&q1, '!');
		for(i = 0; i < n; i++)
			enqueue(&q2, '!');
	}
	else{
		if(m != 1 && n != 1)
			printf("1,%dc1,%d\n", m, n);
		else if(m == 1 && n != 1)
			printf("1c1,%d\n", n);
		else if(m != 1 && n == 1)
			printf("1,%dc1\n", m);
		else if(m == 1 && n == 1)
			printf("1c1\n");
		for(i = 0; i < m; i++)
			printf("< %s", x[i]);
		printf("---\n");
		for(i = 0; i < n; i++)
			printf("> %s", y[i]);
	}
}

void acd(char *x[],char *y[],int m,int n){			//tests conditions for add, change, delete
	int a, b, flag = 0, tempa, tempb;
	if(isempty(&s1)){
		if(m != 0 && n != 0){
			change(x, y, m, n);
		}
		else if(m != 0 && n == 0){
			delatstart(x, y, m + 1, n + 1);
		}
		else if(m == 0 && n != 0){
			addatstart(x, y, m + 1, n + 1);
		}
	}
	else{
		while(!isempty(&s1)){
			a = pop(&s1);
			b = pop(&s2);
				if(a == 1 && b == 1 && flag == 0){
					flag++;
				}
				else if(a == 1 && b != 1 && flag == 0){
					flag++;
					addatstart(x, y, a, b);
				}
				else if(a != 1 && b == 1 && flag == 0){
					flag++;
					delatstart(x, y, a, b);
				}
				else if(a != 1 && b != 1 && flag == 0){
					flag++;
					changeatstart(x, y, a, b);
				}
				else if(flag != 0){
					if(a != tempa + 1 && b == tempb + 1){
						delatmid(x, y, tempa, tempb, a, b);
					}
					else if(a == tempa + 1 && b != tempb + 1){
						addatmid(x, y, tempa, tempb, a, b);
					}
					else if(a != tempa + 1 && b != tempb + 1){
						changeatmid(x, y, tempa, tempb, a, b);
					}
				}
			tempa = a;
			tempb = b;
		}	
		if(a != m && b == n){
			delatend(x, y, m, a, b);
		}
		else if(a == m && b != n){
			addatend(x, y, n, a, b);
		}
		else if(a != m && b != n){
			changeatend(x, y, m, n, a, b);
		}
	}
}

void optu(char *x[], char *y[], int i, int j){		//option -u
	if(i > 0 && j > 0 && b[i][j] == 'd'){
		optu(x, y, i - 1, j - 1);
		printf(" %s", x[j - 1]);
	}
	else if(j > 0 && (i == 0 || b[i][j] == 'l')){
		optu(x, y, i, j - 1);
		printf("-%s", x[j - 1]);              
	}
	else if(i > 0 && (j == 0 || b[i][j] == 'u')){
		optu(x, y, i - 1, j);
		printf("+%s", y[i - 1]);		
	}	
}

void optc1(char *x[], char *y[], int i, int j){		//option c for file 1
	if(i > 0 && j > 0 && b[i][j] == 'd'){
		optc1(x, y, i - 1, j - 1);
		printf("  %s", x[j - 1]);
	}
	else if(j > 0 && (i == 0 || b[i][j] == 'l')){
		optc1(x, y, i, j - 1);
		char ch = dequeue(&q1);
		printf("%c %s", ch, x[j - 1]);               
	}
	else if(i > 0 && (j == 0 || b[i][j] == 'u')){
		optc1(x, y, i - 1, j);		
	}	
}

void optc2(char *x[], char *y[], int i, int j){		//option c for file 2
	if(i > 0 && j > 0 && b[i][j] == 'd'){
		optc2(x, y, i - 1, j - 1);
		printf("  %s", x[j - 1]);
	}
	else if(j > 0 && (i == 0 || b[i][j] == 'l')){
		optc2(x, y, i, j - 1);               
	}
	else if(i > 0 && (j == 0 || b[i][j] == 'u')){
		optc2(x, y, i - 1, j);
		char ch = dequeue(&q2);
		printf("%c %s", ch, y[i - 1]);		
	}	
}

void lcs(char *x[], char *y[], int m, int n){		//implements longest common subsequence
	for(i = 0; i <= n; i++)
		c[i][0] = 0;                               
	for(j = 0; j <= m; j++)
		c[0][j] = 0;
	for(i = 1; i <= n; i++){
		for(j = 1;j <= m; j++){
			if(strcmp(y[i - 1], x[j - 1]) == 0){
				c[i][j] = c[i - 1][j - 1] + 1;
				b[i][j] = 'd';
			}
			else if(c[i - 1][j] >= c[i][j - 1]){
				c[i][j] = c[i - 1][j];
				b[i][j] = 'u';
			}
			else{
				c[i][j] = c[i][j - 1];
				b[i][j] = 'l';
			}
		}  
	}                      
}

void lcsopti(char *x[], char *y[], int m, int n){		//option -i
	for(i = 0; i <= n; i++)
		c[i][0] = 0;                               
	for(j = 0; j <= m; j++)
		c[0][j] = 0;
	for(i = 1; i <= n; i++){
		for(j = 1;j <= m; j++){
			if(strcasecmp(y[i - 1], x[j - 1]) == 0){
				c[i][j] = c[i - 1][j - 1] + 1;
				b[i][j] = 'd';
			}
			else if(c[i - 1][j] >= c[i][j - 1]){
				c[i][j] = c[i - 1][j];
				b[i][j] = 'u';
			}
			else{
				c[i][j] = c[i][j - 1];
				b[i][j] = 'l';
			}
		}  
	}                    
}
 
int main(int argc, char *argv[]){
	char *x[100], *y[100], *x2[100], *y2[100], *x3[100], *y3[100];
	int m = 0, n = 0, l1, l2;        
    	char str1[200], str2[200], arr1[200], arr2[200], arr3[200], arr4[200];       
    	int size1 = sizeof(str1);
    	int size2 = sizeof(str2);
    	init(&s1);
    	init(&s2);
    	initq(&q1);
    	initq(&q2);
	FILE *fp1, *fp2;
	if(argc == 2){
		if((strcmp(argv[1],"--help")) == 0){								
			printf("Usage: diff [OPTION]... FILES\nCompare FILES line by line.\n\n");
			printf("-c   output NUM (default 3) lines of copied context\n");
			printf("-u   output NUM (default 3) lines of unified context\n");
			printf("-i   ignore case differences in file contents\n");
			printf("-b   ignore changes in the amount of white space\n");
			printf("-w   ignore all white space\n");
			printf("-q   report only when files differ\n");
  			printf("-s   report when two files are the same\n");
			exit(0);
		}
		else{
			printf("diff: missing operand after '%s'\n", argv[1]);
			printf("diff: Try 'diff --help' for more information.\n");
			exit(0);
		}
	}
	else if(argc == 1){
		printf("diff: missing operand after '%s'\n", argv[0]);
		printf("diff: Try 'diff --help' for more information.\n");
		exit(0);
	}
	else if(argc == 3){
		fp1 = fopen(argv[1], "r+");
		fp2 = fopen(argv[2], "r+");
		if(fp1 == NULL){
			printf("diff: %s: No such file or directory\n", argv[1]);
			exit(0);
		}
		if(fp2 == NULL){
			printf("diff: %s: No such file or directory\n", argv[2]);
			exit(0);
		}
	}
	else if(argc == 4){
		if(strcmp(argv[1],"-u") == 0 || strcmp(argv[1],"-c") == 0 || strcmp(argv[1],"-i") == 0 || strcmp(argv[1],"-b") == 0 || 			strcmp(argv[1],"-w") == 0 || strcmp(argv[1],"-q") == 0 || strcmp(argv[1],"-s") == 0){
			fp1 = fopen(argv[2], "r+");
			fp2 = fopen(argv[3], "r+");
			stat(argv[2], &filestat1);
			stat(argv[3], &filestat2);
			if(fp1 == NULL){
				printf("diff: %s: No such file or directory\n", argv[2]);
				exit(0);
			}
			if(fp2 == NULL){
				printf("diff: %s: No such file or directory\n", argv[3]);
				exit(0);
			}	
		}
		else{
			printf("diff: extra operand '%s'\n", argv[3]);
			printf("diff: Try 'diff --help' for more information.\n");
			exit(0);
		}
		
	}
	else if(argc > 4){
		printf("diff: extra operand '%s'\n", argv[3]);
		printf("diff: Try 'diff --help' for more information.\n");
		exit(0);
	}
	
    	while(fgets(str1, size1, fp1) != NULL){        
	       	x[m] = (char *)malloc(size1);  
        	strcpy(x[m], str1);                         
        	m++;
    	}
    	while(fgets(str2, size2, fp2) != NULL){        
        	y[n] = (char *)malloc(size2);
        	strcpy(y[n], str2);                          
        	n++;
    	}
    	fseek(fp1, 0, SEEK_SET);
    	fseek(fp2, 0, SEEK_SET);
    	
    	// for option b
	int a = 0;
	while(!feof(fp1)){
		char arr1[200];
		int i = 0, count = 0;
		for(char ch = fgetc(fp1); ch != EOF && ch != '\n'; ch = fgetc(fp1)){
			if(ch != ' ' && ch != '\t'){
				arr1[i++] = ch;
				count++;
			}
			else
				continue;
		}
		arr1[i] = '\0';
		x2[a] = (char *)malloc(count);
		strcpy(x2[a], arr1);
		a++;
	}
		
	int b = 0;
	while(!feof(fp2)){
		char arr2[200];
		int i = 0, count = 0;
		for(char ch = fgetc(fp2); ch != EOF && ch != '\n'; ch = fgetc(fp2)){
			if(ch != ' ' && ch != '\t'){
				arr2[i++] = ch;
				count++;
			}
			else
				continue;
		}
		arr2[i] = '\0';
		y2[b] = (char *)malloc(count);
		strcpy(y2[b], arr2);
		b++;
	}
	
	fseek(fp1, 0, SEEK_SET);
    	fseek(fp2, 0, SEEK_SET);
    	
	//for option w
	int c = 0;
	while(!feof(fp1)){
		char arr1[200];
		int i = 0, count = 0;
		for(char ch = fgetc(fp1); ch != EOF && ch != '\n'; ch = fgetc(fp1)){
			if(!isspace(ch)){
				arr1[i++] = ch;
				count++;
			}
			else
				continue;
		}
		arr1[i] = '\0';
		x3[c] = (char *)malloc(count);
		strcpy(x3[c], arr1);
		c++;
	}
		
	int d = 0;
	while(!feof(fp2)){
		char arr2[200];
		int i = 0, count = 0;
		for(char ch = fgetc(fp2); ch != EOF && ch != '\n'; ch = fgetc(fp2)){
			if(!isspace(ch)){
				arr2[i++] = ch;
				count++;
			}
			else
				continue;
		}
		arr2[i] = '\0';
		y3[d] = (char *)malloc(count);
		strcpy(y3[d], arr2);
		d++;
	}

     	char t1[100], t2[100];
    	strftime(t1, 100, "%Y-%m-%d %H:%M:%S %z", localtime(&filestat1.st_mtime));		//for timestamp
    	strftime(t2, 100, "%Y-%m-%d %H:%M:%S %z", localtime(&filestat2.st_mtime));
	if(argc == 3){						//diff without options
		lcs(x, y, m, n);
		diff(x, y, n, m);
		acd(x, y, m, n);
	}
	else{
		if(strcmp(argv[1],"-u") == 0){			//It prints the output in +,- format with timestamp
			if(m == 0 && n == 0){
				exit(0);
			}
			else{
				printf("--- %s	%s\n", argv[2], t1);
				printf("+++ %s	%s\n", argv[3], t2);
				if(m > 1 && n > 1){
					printf("@@ -1,%d +1,%d @@\n", m, n);
				}
				else if(m == 1 && n > 1){
					printf("@@ -1 +1,%d @@\n", n);
				}
				else if(m > 1 && n == 1){
					printf("@@ -1,%d +1 @@\n", m);
				}
				else if(m == 1 && n == 1){
					printf("@@ -1 +1 @@\n");
				}
				else if(m == 0 && n > 1){
					printf("@@ -0,0 +1,%d @@\n", n);
				}
				else if(m == 0 && n == 1){
					printf("@@ -0,0 +1 @@\n");
				}
				else if(m == 1 && n == 0){
					printf("@@ -1 +0,0 @@\n");
				}
				else if(m > 1 && n == 0){
					printf("@@ -1,%d +0,0 @@\n", m);
				}
				lcs(x, y, m, n);
				optu(x, y, n, m);
			}
		}
		else if(strcmp(argv[1],"-c") == 0){		//prints output for each file separately in +,- format with timestamp
			if(m == 0 && n == 0){
				exit(0);
			}
			else{
				cflag++;
				printf("*** %s	%s\n", argv[2], t1);
				printf("--- %s	%s\n", argv[3], t2);
				lcs(x, y, m, n);
				diff(x, y, n, m);
				acd(x, y, m, n);
				printf("***************\n");
				if(m > 1){
					printf("*** 1,%d ****\n", m);
				}
				else if(m == 1){
					printf("*** 1 ****\n");
				}
				else{
					printf("*** 0 ****\n");
				}
				optc1(x, y, n, m);
				if(n > 1){
					printf("--- 1,%d ----\n", n);
				}
				else if(n == 1){
					printf("--- 1 ----\n");
				}
				else{
					printf("--- 0 ----\n");
				}
				optc2(x, y, n, m);
			}
		}	
		else if(strcmp(argv[1],"-i") == 0){		//It gives diff output by ignoring cases of letters
			lcsopti(x, y, m, n);
			diff(x, y, n, m);
			acd(x, y, m, n);
		}	
		else if(strcmp(argv[1],"-b") == 0){		// ignores blanks and tabs
			lcs(x2, y2, a, b);
			diff(x, y, n, m);
			acd(x, y, m, n);
		}
		else if(strcmp(argv[1],"-w") == 0){		//ignores all whitespaces(space, h and v tabs, form feed, form feed)
			lcs(x3, y3, a, b);
			diff(x, y, n, m);
			acd(x, y, m, n);
		}
		else if(strcmp(argv[1],"-q") == 0){		//reports when files differ otherwise prints output
			lcs(x, y, m, n);
			diff(x, y, n, m);
			if(s1.i != m){
				printf("Files %s and %s differ\n", argv[2], argv[3]);
			}
			else{
				acd(x, y, m, n);
			}		
		}
		else if(strcmp(argv[1],"-s") == 0){		//reports when files identical otherwise prints output
			lcs(x, y, m, n);
			diff(x, y, n, m);
			if(s1.i == m){
				printf("Files %s and %s are identical\n", argv[2], argv[3]);
			}
			else{
				acd(x, y, m, n);
			}
		}	
	}
	fclose(fp1);
	fclose(fp2);
	return 0;
}
