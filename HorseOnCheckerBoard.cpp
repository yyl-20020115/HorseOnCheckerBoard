#define STACK_INIT_SIZE 100
#define STACKINCREMENT 10
#define OVERFLOW -2
#define OK 1

#include<malloc.h>
#include<stdio.h>
#include<stdlib.h>
int Board[8][8] = { 0 };
int HTry1[8] = { 2,-1,1,-2,2,1,-1,-2 };
int HTry2[8] = { 1,2,2,1,-1,-2,-2,-1 };

typedef struct {
	int i;
	int j;
}PosType;

typedef struct {
	int ord;
	PosType seat;
	int di;
}SElemType;

typedef struct {
	SElemType* base;
	SElemType* top;
	int stacksize;
}SqStack;

int InitStack(SqStack* s1) {
	(*s1).base = (SElemType*)malloc(STACK_INIT_SIZE * sizeof(SElemType));
	if (!(*s1).base) exit(OVERFLOW);
	(*s1).top = (*s1).base;
	(*s1).stacksize = STACK_INIT_SIZE;
	return(OK);
}
SElemType Pop(SqStack* s, SElemType e) {
	e = *--(*s).top;
	return e;
}
int Push(SqStack* s1, SElemType e) {
	if ((*s1).top - (*s1).base >= (*s1).stacksize) {
		(*s1).base = (SElemType*)realloc((*s1).base,
			((*s1).stacksize + STACKINCREMENT) * sizeof
			(SElemType));
		if (!(*s1).base) exit(OVERFLOW);
		(*s1).top = (*s1).base + (*s1).stacksize;
		(*s1).stacksize += STACKINCREMENT;
	}
	*(*s1).top++ = e;
	return OK;
}

int StackEmpty(SqStack* s) {
	if ((*s).base == (*s).top)
		return(1);
	else
		return(0);
}

int curstep = 0;

int Pass(PosType s) {
	if ((Board[s.i][s.j] == 0) && (s.i <= 7) && (s.i >= 0) && (s.j <= 7) && (s.j >= 0))
		return(1);
	else
		return(0);
}

PosType NextPos(PosType s, int i) {
	s.i = s.i + HTry1[i - 1];
	s.j = s.j + HTry2[i - 1];
	return(s);
}

void HorseSteps(int Board[8][8], PosType start) {
	int k, j;
	SqStack S;
	SElemType e;
	PosType curpos = start;
	InitStack(&S);
	do {
		if (Pass(curpos)) {
			curstep++;
			Board[curpos.i][curpos.j] = curstep;
			e.seat = curpos;
			e.ord = curstep;
			e.di = 1;
			Push(&S, e);
			if (curstep == 64)
				break;
			else
				curpos = NextPos(curpos, 1);
		}//if
		else {
			if (!StackEmpty(&S)) {
				Pop(&S, e);
				if (e.di == 8) Board[e.seat.i][e.seat.j] = 0;
				while (e.di == 8 && !StackEmpty(&S)) {
					e = Pop(&S, e);
					if (e.di == 8) Board[e.seat.i][e.seat.j] = 0;
					curstep = e.ord;
				}//while
				if (e.di < 8) {
					e.di++;
					Push(&S, e);
					curpos = NextPos(e.seat, e.di);
				}//if
			}//if
		}//else
	} while (!StackEmpty(&S));
	if (StackEmpty(&S)) {
		printf("马儿从这个初始位置不能踏遍棋盘\n");
		printf("请按任意键推出本程序\n");
		getchar();
		exit(OVERFLOW);
	}
	for (j = 0; j < 8; j++) {
		printf("\n");
		for (k = 0; k < 8; k++)
			printf("%3d", Board[j][k]);
	}// for
}//函数结束


int main(int argc, char* argv[]) {
	int k, j;
	PosType t;
	//char a, b;
	printf("请输入马儿的初始位置\n");
	fflush(stdin);
	//scanf_s("%c%d,%d%c", &a, (unsigned int)sizeof(a), &k, &j, &b, (unsigned int)sizeof(b));
	scanf_s("%d,%d", &k, &j);
	t.i = k;
	t.j = j;
	printf("马儿走的路线为\n");
	HorseSteps(Board, t);
	return 0;
}
