#include <stdlib.h> // atoi, rand
#include <stdio.h>
#include <assert.h> // assert
#include <time.h> //time

#define RANGE 100
#define COMPARE(x, y) (((x) > (y)) ? 1 : ((x) == (y)) ? 0 : -1)

typedef struct
{
	int x;
	int y;
} point;

typedef struct
{
	point from;
	point to;
} line_segment;

////////////////////////////////////////////////////////////////////////////////
void print_header(char *filename)
{
	printf( "#! /usr/bin/env Rscript\n");
	printf( "png(\"%s\", width=700, height=700)\n", filename);
	
	printf( "plot(1:%d, 1:%d, type=\"n\")\n", RANGE, RANGE);
}
////////////////////////////////////////////////////////////////////////////////
void print_footer(void)
{
	printf( "dev.off()\n");
}

////////////////////////////////////////////////////////////////////////////////
void print_points( point *p, int n)
{
	printf("\n#points\n");
	for(int i = 0; i < n; i++)
	{
		printf( "points(%d,%d)\n", p[i].x, p[i].y);
	}
}

void append( point *p, int i, int j, line_segment *l, int n)
{
	l[n].from.x = p[i].x;
	l[n].from.y = p[i].y;
	l[n].to.x = p[j].x;
	l[n].to.y = p[j].y;
}

// [input] p : set of points
// [input] n : number of points
// [output] l : set of line segments that form the convex hull
// return value : number of line segments
int convex_hull( point *p, int n, line_segment *l)
{
	int num_line = 0;
	float x, y;
	float result;
	int count_plus = 0;
	int count_minus = 0;
	short int *check = malloc(sizeof(short int) * n);
	for(int check_index = 0; check_index < n; check_index++)
	{
		check[check_index] = 0;
	}
	
	for(int i = 0; i < n - 1; i++)				// first point index
	{
		for(int j = i + 1; j < n; j++)			// second point index
		{
			if(p[i].x != p[j].x)
			{
				for(int k = 0; k < n; k++)		// varifying point index
				{
					if((k == i) || (k == j)) k++;
					x = p[k].x;
					y = p[k].y;
				
					result = (p[j].y - p[i].y) * (x - p[i].x) / (p[j].x - p[i].x) + p[i].y;
					switch(COMPARE(y, result))
					{
						case 1: 
							check[k] = 1; break;
						case 0:
							check[k] = 0; break;
						case -1:
							check[k] = -1; break;
					}
				}
			}
			else		// p[i].x == p[j].x
			{
				for(int k = 0; k < n; k++)
				{
					if((k == i) || (k == j)) k++;
					x = p[k].x;
					switch(COMPARE(x, p[i].x))
					{
						case 1: 
							check[k] = 1; break;
						case 0:
							check[k] = 0; break;
						case -1:
							check[k] = -1; break;
					}
				}
			}
			for(int a = 0; a < n; a++)
			{
				if(check[a] == 1) count_plus++;
				else if(check[a] == -1) count_minus++;
				check[a] = 0;					// reset check array
			}
			switch(COMPARE(count_plus, count_minus))
			{
				case 1:
					if(count_minus == 0)
					{
						append(p, i, j, l, num_line);
						num_line++;
					}
					break;
				case 0:
					if(count_plus == 0)			// all points are in same line
					{
						append(p, i, j, l, num_line);
						num_line++;
					}
					break;
				case -1:
					if(count_plus == 0)
					{
						append(p, i, j, l, num_line);
						num_line++;
					}
					break;
			}
			count_plus = 0;
			count_minus = 0;
		}
	}
	return num_line;
}

void print_line_segments( line_segment *l, int num_line)
{
	printf("\n#line segments\n");
	for(int i = 0; i < num_line; i++)
	{
		printf("segments(%d,%d,%d,%d)\n", l[i].from.x, l[i].from.y, l[i].to.x, l[i].to.y);
	}
}

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	int x, y;
	int n; // number of points
	
	if (argc != 2)
	{
		printf( "%s number_of_points\n", argv[0]);
		return 0;
	}

	n = atoi( argv[1]);
	if (n <= 0)
	{
		printf( "The number of points should be a positive integer!\n");
		return 0;
	}

	point p[n];
	line_segment l[n * 2];

	fprintf( stderr, "Create %d points\n", n);

	// making n points
	srand( time(NULL));
	for (int i = 0; i < n; i++)
	{
		x = rand() % RANGE + 1; // 1 ~ RANGE random number
		y = rand() % RANGE + 1;
		
		p[i].x = x;
		p[i].y = y;
 	}

	print_header( "convex.png");
	
	print_points( p, n);
	
	int num_line = convex_hull( p, n, l);

	print_line_segments( l, num_line);
	
	print_footer();
	
	return 0;
}
