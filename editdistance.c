#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define INSERT_OP      0x01
#define DELETE_OP      0x02
#define SUBSTITUTE_OP  0x04
#define MATCH_OP       0x08
#define TRANSPOSE_OP   0x10

#define INSERT_COST	1
#define DELETE_COST	1
#define SUBSTITUTE_COST	1
#define TRANSPOSE_COST	1

int count = 1;

////////////////////////////////////////////////////////////////////////////////
// 정렬된 문자쌍들을 출력 
void print_alignment( char align_str[][8], int level)
{
	int i;
	printf("\n[%d] ==============================\n", count);
	for (i = level; i >= 0; i--)
	{
		printf( "%s\n", align_str[i]);
	}
	count++;
	printf("\n");
}

////////////////////////////////////////////////////////////////////////////////
// 세 정수 중에서 가장 작은 값을 리턴한다.
int __GetMin3( int a, int b, int c)
{
	int min = a;
	if (b < min)
		min = b;
	if(c < min)
		min = c;
	return min;
}

////////////////////////////////////////////////////////////////////////////////
// 네 정수 중에서 가장 작은 값을 리턴한다.
int __GetMin4( int a, int b, int c, int d)
{
	int min = __GetMin3( a, b, c);
	return (min > d) ? d : min;
}

////////////////////////////////////////////////////////////////////////////////

// 재귀적으로 연산자 행렬을 순회하며, 두 문자열이 최소편집거리를 갖는 모든 가능한 정렬(alignment) 결과를 출력한다.
// op_matrix : 이전 상태에 대한 연산자 정보(backtrace)가 저장된 행렬 (1차원 배열임에 주의!)
// col_size : op_matrix의 열의 크기
// str1 : 문자열 1
// str2 : 문자열 2
// n : 문자열 1의 길이
// m : 문자열 2의 길이
// level : 재귀호출의 레벨 (0, 1, 2, ...)
// align_str : 정렬된 문자쌍들의 정보가 저장된 문자열 배열 예) "a - a", "a - b", "* - b", "ab - ba"
void backtrace_main( int *op_matrix, int col_size, char *str1, char *str2, int n, int m, int level, char align_str[][8])
{
	int value = op_matrix[col_size * m + n];
	if(n == 1 && m == 1) {
		if(value % 2) { align_str[level][0] = '*'; align_str[level][1] = ' '; align_str[level][2] = '-'; align_str[level][3] = ' '; align_str[level][4] = str2[n - 1]; align_str[level][5] = ' '; align_str[level][6] = ' '; align_str[level][7] = 0;
			}
		if((value >> 1) % 2) { align_str[level][0] = str1[m - 1]; align_str[level][1] = ' '; align_str[level][2] = '-'; align_str[level][3] = ' '; align_str[level][4] = '*'; align_str[level][5] = ' '; align_str[level][6] = ' '; align_str[level][7] = 0;
			}
		if((value >> 2) % 2) { align_str[level][0] = str1[m - 1]; align_str[level][1] = ' '; align_str[level][2] = '-'; align_str[level][3] = ' '; align_str[level][4] = str2[n - 1]; align_str[level][5] = ' '; align_str[level][6] = ' '; align_str[level][7] = 0;
			}
		if((value >> 3) % 2) { align_str[level][0] = str1[m - 1]; align_str[level][1] = ' '; align_str[level][2] = '-'; align_str[level][3] = ' '; align_str[level][4] = str2[n - 1]; align_str[level][5] = ' '; align_str[level][6] = ' '; align_str[level][7] = 0;
			}
		if((value >> 4) % 2) { align_str[level][0] = str1[m - 2]; align_str[level][1] = str1[m - 1]; align_str[level][2] = ' '; align_str[level][3] = '-'; align_str[level][4] = ' '; align_str[level][5] = str2[n - 2]; align_str[level][6] = str2[n - 1]; align_str[level][7] = 0;
			}
		print_alignment(align_str, level);
	}
	else
	{
		if(value % 2) { align_str[level][0] = '*'; align_str[level][1] = ' '; align_str[level][2] = '-'; align_str[level][3] = ' '; align_str[level][4] = str2[n - 1]; align_str[level][5] = ' '; align_str[level][6] = ' '; align_str[level][7] = 0;
			backtrace_main(op_matrix, col_size, str1, str2, n - 1, m, level + 1, align_str); }
		if((value >> 1) % 2) { align_str[level][0] = str1[m - 1]; align_str[level][1] = ' '; align_str[level][2] = '-'; align_str[level][3] = ' '; align_str[level][4] = '*'; align_str[level][5] = ' '; align_str[level][6] = ' '; align_str[level][7] = 0;
			backtrace_main(op_matrix, col_size, str1, str2, n, m - 1, level + 1, align_str); }
		if((value >> 2) % 2) { align_str[level][0] = str1[m - 1]; align_str[level][1] = ' '; align_str[level][2] = '-'; align_str[level][3] = ' '; align_str[level][4] = str2[n - 1]; align_str[level][5] = ' '; align_str[level][6] = ' '; align_str[level][7] = 0;
			backtrace_main(op_matrix, col_size, str1, str2, n - 1, m - 1, level + 1, align_str); }
		if((value >> 3) % 2) { align_str[level][0] = str1[m - 1]; align_str[level][1] = ' '; align_str[level][2] = '-'; align_str[level][3] = ' '; align_str[level][4] = str2[n - 1]; align_str[level][5] = ' '; align_str[level][6] = ' '; align_str[level][7] = 0;
			backtrace_main(op_matrix, col_size, str1, str2, n - 1, m - 1, level + 1, align_str); }
		if((value >> 4) % 2) { align_str[level][0] = str1[m - 2]; align_str[level][1] = str1[m - 1]; align_str[level][2] = ' '; align_str[level][3] = '-'; align_str[level][4] = ' '; align_str[level][5] = str2[n - 2]; align_str[level][6] = str2[n - 1]; align_str[level][7] = 0;
			backtrace_main(op_matrix, col_size, str1, str2, n - 2, m - 2, level + 1, align_str); }
	}
}

////////////////////////////////////////////////////////////////////////////////
// backtrace_main을 호출하는 wrapper 함수
void backtrace( int *op_matrix, int col_size, char *str1, char *str2, int n, int m)
{
	char align_str[n+m][8]; // n+m strings
	
	count = 1;
	
	backtrace_main( op_matrix, col_size, str1, str2, n, m, 0, align_str);
}

// 강의 자료의 형식대로 op_matrix를 출력 (좌하단(1,1) -> 우상단(n, m))
// 각 연산자를  괄호 안의 기호로 표시한다. 삽입(I), 삭제(D), 교체(S), 일치(M), 전위(T)
void print_matrix( int *op_matrix, int col_size, int n, int m)
{
	int index = 0;
	int value = 0;
	char str[8] = "";
	
	for(int j = m; j >= 1; j--)
	{
		for(int i = 1; i < n + 1; i++)
		{
			value = op_matrix[col_size * j + i];
			if(value % 2) str[index++] = 'I';
			if((value >> 1 ) % 2) str[index++] = 'D';
			if((value >> 2 ) % 2) str[index++] = 'S';
			if((value >> 3 ) % 2) str[index++] = 'M';
			if((value >> 4 ) % 2) str[index++] = 'T';
			printf("%-8s", str);
			index = 0;
			str[0] = str[1] = str[2] = str[3] = str[4] = str[5] = str[6] = str[7] = 0;
		}
		printf("\n");
	}
}

// 두 문자열 str1과 str2의 최소편집거리를 계산한다.
// return value : 최소편집거리
// 이 함수 내부에서 print_matrix 함수와 backtrace 함수를 호출함
int min_editdistance( char *str1, char *str2)
{
	int distance = 0;
	int n = strlen(str2);
	int m = strlen(str1);
	int min;
	int mch_or_sub;
	int op_matrix[(n + 1) * (m + 1)];
	
	int index = 0;
	int value = 0;
	int col_size = n + 1;
	
	int a, b, c, d;
	
	for(int i = 0; i < n + 1; i++)
	{
		for(int j = 0; j < m + 1; j++)
		{
			if(j == 0) op_matrix[i] = i;
			else if(i == 0) op_matrix[col_size * j + i] = j;
			else if(i == 1 && j == 1)
			{
				if(str1[j - 1] == str2[i - 1]) mch_or_sub = 0;
				else mch_or_sub = SUBSTITUTE_COST;
				op_matrix[col_size * i + j] = __GetMin3(op_matrix[col_size * (i - 1) + j] + INSERT_COST, op_matrix[col_size * i + (j - 1)] + DELETE_COST, mch_or_sub);
			}
			else if(i < 2 || j < 2)	// don't check transpose
			{
				if(str1[j - 1] == str2[i - 1]) mch_or_sub = op_matrix[col_size * (j - 1) + (i - 1)];	// match
				else mch_or_sub = op_matrix[col_size * (j - 1) + (i - 1)] + SUBSTITUTE_COST;		// substitute
				op_matrix[col_size * j + i] = __GetMin3(op_matrix[col_size * (j - 1) + i] + INSERT_COST, op_matrix[col_size * j + (i - 1)] + DELETE_COST, mch_or_sub);
			}
			else
			{
				if(str1[j - 1] == str2[i - 1]) mch_or_sub = op_matrix[col_size * (j - 1) + (i - 1)];	// match
				else mch_or_sub = op_matrix[col_size * (j - 1) + (i - 1)] + SUBSTITUTE_COST;		// substitute
				if(str1[j - 1] == str2[i - 2] && str1[j - 2] == str2[i - 1]) op_matrix[col_size * j + i] = __GetMin4(op_matrix[col_size * (j - 1) + i] + INSERT_COST, op_matrix[col_size * j + (i - 1)] + DELETE_COST, mch_or_sub, op_matrix[col_size * (j - 2) + (i - 2)] + TRANSPOSE_COST);
				else op_matrix[col_size * j + i] = __GetMin3(op_matrix[col_size * (j - 1) + i] + INSERT_COST, op_matrix[col_size * j + (i - 1)] + DELETE_COST, mch_or_sub);
			}
		}
	}
	
	distance = op_matrix[m * col_size + n];
	
	
	for(int j = m; j >= 0; j--)
	{
		for(int i = n; i >= 0; i--)
		{
			index = col_size * j + i;
			value = op_matrix[index];
			op_matrix[index] = 0;
			if(i == 0 || j == 0) continue;
			else
			{
				if(value == op_matrix[index - 1] + INSERT_COST) op_matrix[index] += INSERT_OP;
				if(value == op_matrix[index - col_size] + DELETE_COST) op_matrix[index] += DELETE_OP;
				if(value == op_matrix[index - col_size - 1] + SUBSTITUTE_COST) op_matrix[index] += SUBSTITUTE_OP;
				if(value == op_matrix[index - col_size - 1] && str1[j - 1] == str2[i - 1]) op_matrix[index] += MATCH_OP;
				// check transpose
				if(i >= 2 && j >= 2) { if(value == op_matrix[index - 2 * col_size - 2] + TRANSPOSE_COST && str1[j - 1] == str2[i - 2] && str1[j - 2] == str2[i - 1]) op_matrix[index] += TRANSPOSE_OP; }
			}
		}
	}
	
	print_matrix(op_matrix, n + 1, n, m);
	
	backtrace(op_matrix, n + 1, str1, str2, n, m);
	
	return distance;
}


////////////////////////////////////////////////////////////////////////////////
int main()
{
	char str1[30];
	char str2[30];
	
	int distance;
	
	printf( "INSERT_COST = %d\n", INSERT_COST);
	printf( "DELETE_COST = %d\n", DELETE_COST);
	printf( "SUBSTITUTE_COST = %d\n", SUBSTITUTE_COST);
	printf( "TRANSPOSE_COST = %d\n", TRANSPOSE_COST);
	
	while( fscanf( stdin, "%s\t%s", str1, str2) != EOF)
	{
		printf( "\n==============================\n");
		printf( "%s vs. %s\n", str1, str2);
		printf( "==============================\n");
		
		distance = min_editdistance( str1, str2);
		
		printf( "\nMinEdit(%s, %s) = %d\n", str1, str2, distance);
	}
	return 0;
}
