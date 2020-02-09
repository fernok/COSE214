#include <stdlib.h> // atoi, rand, qsort, malloc
#include <stdio.h>
#include <assert.h> // assert
#include <time.h> //time

#define RANGE 100.0

typedef struct
{
	float x;
	float y;
} point;

typedef struct
{
	point from;
	point to;
} line_segment;

////////////////////////////////////////////////////////////////////////////////
// function declaration

float distance( float a, float b, float c, point p)
{
    float result = abs((a * p.x + b * p.y + c));
	return result;
}

void separate_points( point *p, int n, point from, point to, point *s1, point *s2, int *n1, int *n2)
{
	if(n == 0) return;
	float result;
	for(int i = 0; i < n; i++)
	{
		result = (to.x - from.x) * p[i].y - (to.y - from.y) * p[i].x - from.y * (to.x - from.x) + from.x * (to.y - from.y);
		if(result < -0.001) s1[(*n1)++] = p[i]; 
		else if(result > 0.001) s2[(*n2)++] = p[i];
	}
}

// convex hull
void convex_hull_main( point *p, int n, point p1, point pn, line_segment *l, int *num_l)
{
	if(n == 0)
	{
		l[(*num_l)].from = p1;
		l[(*num_l)].to = pn;
		(*num_l)++;
		return;
	}
	int max = 0, index = 0;
	float dist = 0;
	int n1 = 0, n2 = 0, n3 = 0;
	float a = - (pn.y - p1.y);
	float b = (pn.x - p1.x);
	float c = -p1.y * (pn.x - p1.x) + p1.x * (pn.y - p1.y);

	for(int i = 0; i < n; i++)
	{
		dist = distance(a, b, c, p[i]);
		if(max < dist)
		{
			index = i;
			max = dist;
		}
	}

	point *s1 = (point *)malloc(sizeof (point) * n);
	point *s2 = (point *)malloc(sizeof (point) * n);
	point *s3 = (point *)malloc(sizeof (point) * 3 * n);
	
	separate_points( p, n, p1, p[index], s1, s3, &n1, &n3);
	separate_points( p, n, p[index], pn, s2, s3, &n2, &n3);
	convex_hull_main( s1, n1, p1, p[index], l, num_l);
	convex_hull_main( s2, n2, p[index], pn, l, num_l);

	free(s1);
	free(s2);
	free(s3);

}

////////////////////////////////////////////////////////////////////////////////
void print_header(char *filename)
{
	printf( "#! /usr/bin/env Rscript\n");
	printf( "png(\"%s\", width=700, height=700)\n", filename);
	
	printf( "plot(1:%.3f, 1:%.3f, type=\"n\")\n", RANGE, RANGE);
}

////////////////////////////////////////////////////////////////////////////////
void print_footer(void)
{
	printf( "dev.off()\n");
}

////////////////////////////////////////////////////////////////////////////////
int cmp_x( const void *p1, const void *p2)
{
	point *p = (point *)p1;
	point *q = (point *)p2;
	
	float diff = p->x - q->x;
	
    return ((diff >= 0.0) ? ((diff > 0.0) ? +1 : 0) : -1);
}

////////////////////////////////////////////////////////////////////////////////
void print_points( point *p, int n)
{
	int i;
	printf( "\n#points\n");
	
	for (i = 0; i < n; i++)
		printf( "points(%.3f,%.3f)\n", p[i].x, p[i].y);
}

////////////////////////////////////////////////////////////////////////////////
void print_line_segments( line_segment *l, int num_line)
{
	int i;

	printf( "\n#line segments\n");
	
	for (i = 0; i < num_line; i++)
		printf( "segments(%.3f,%.3f,%.3f,%.3f)\n", l[i].from.x, l[i].from.y, l[i].to.x, l[i].to.y);
}

////////////////////////////////////////////////////////////////////////////////
// [input] p : set of points
// [input] n : number of points
// [output] l : set of line segments that forms the convex hull
// return value : number of line segments
int convex_hull( point *p, int n, int min_index, int max_index, line_segment *l)
{
	int num_l = 0;
	int n1 = 0, n2 = 0;
	point *s1 = (point*)malloc(sizeof(point) * n);
	assert( s1 != NULL);

	point *s2 = (point*)malloc(sizeof(point) * n);
	assert( s2 != NULL);
	
	separate_points( p, n, p[min_index], p[max_index], s1, s2, &n1, &n2);
	
	convex_hull_main( s1, n1, p[min_index], p[max_index], l, &num_l);
	convex_hull_main( s2, n2, p[max_index], p[min_index], l, &num_l);

	free( s1);
	free( s2);
	
	return num_l;
}

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	float x, y;
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

	point *p;
	p = (point *)malloc(sizeof (point) * n);
	assert( p != NULL);
	
	line_segment *l;
	l = (line_segment *)malloc(sizeof (line_segment) * n);
	assert( l != NULL);

	fprintf( stderr, "Create %d points\n", n);

	float min_x = RANGE;
	float max_x = 0;
	int min_index = 0;
	int max_index = 0;
	
	// making n points
	srand( time(NULL));
	for (int i = 0; i < n; i++)
	{
		x = (float)rand() / (float)(RAND_MAX) * RANGE; // 0 ~ RANGE random float number
		y = (float)rand() / (float)(RAND_MAX) * RANGE; // 0 ~ RANGE random float number
	
		p[i].x = x;
		p[i].y = y;
		
		if (x < min_x) { min_x = x; min_index = i; }
		if (x > max_x) { max_x = x; max_index = i; }
 	}

	print_header( "convex.png");
	
	print_points( p, n);

	// convex hull algorithm
	int num_line = convex_hull( p, n, min_index, max_index, l);

	print_line_segments( l, num_line);
	
	print_footer();
	
	free( p);
	free( l);
	
	return 0;
}