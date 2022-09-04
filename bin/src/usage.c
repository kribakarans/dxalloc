
#include <stdio.h>
#include <dxalloc.h>

#define print_error(fmt, args...) (fprintf(stdout, "%21s:%-4d %25s: " fmt "\n", __FILE__, __LINE__, __func__, ##args))

void fun2()
{
	void *ptr = NULL;
	FILE  *fp = NULL;

	ptr = dxmalloc (1);
	print_error("dxmalloc ptr: %p", ptr);
	//dxfree(ptr);

	ptr = dxcalloc (1, 1);
	print_error("dxcalloc ptr: %p", ptr);
	//dxfree(ptr);

	ptr = dxrealloc (0, 1);
	print_error("dxrealloc ptr: %p", ptr);
	dxfree(ptr);

	fp = dxfopen ("/tmp/hello", "a+");
	print_error("dxfopen ptr: %p", fp);
	dxfclose(fp);

	ptr = dxstrdup ("Helloworld!!!");
	print_error("dxstrdup ptr: %p", ptr);
	dxfree(ptr);

	int retval = 0;
	retval = dxasprintf((char **)&ptr, "ERROR: %s:%d %s: __testing_dxasprintf__", __FILE__, __LINE__, __func__);
	print_error("dxasprintf ptr: %p", ptr);
	dxfree(ptr);
}

void fun1()
{
	fun2();
}

void fun()
{
	fun1();
	fun2();
}

int main ()
{
	init_dxtrace();

	fun();
	fun2();

	return 0;
}

/* EOF */
