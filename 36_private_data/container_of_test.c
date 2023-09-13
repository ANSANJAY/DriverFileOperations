#include <stdio.h>

#define offsetof(TYPE, MEMBER)	((size_t) &((TYPE *)0)->MEMBER)

#define container_of(ptr, type, member)	({	\
						const typeof ( ((type *)0)->member) *__mptr = (ptr); \
						(type *)( (char *)__mptr - offsetof(type, member));})

struct test
{
	int member1;
	float member2;
};

int main()
{
	struct test t1;
	float *ptr = &t1.member2;
	printf("Address of t1:\t\t%p\n", &t1);
	printf("Address of t1 container_method:\t\t%p\n", 
			container_of(ptr, struct test, member2));
	return 0;
}
