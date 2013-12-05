#include <stdio.h>
#include <time.h>

void main()
{
	clock_t begin,end;
	double time_spent;

	// start the clock
	begin = clock();

	// do something
	printf("Hello world\n");

	// end the clock
	end = clock();

	// calculate time spent
	time_spent = (double) (begin - end) / CLOCKS_PER_SEC ;

	printf("%f \n", time_spent);
}