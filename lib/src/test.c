
#include <stdio.h>
#include <logit/logit.h>
#include <alloc/dxalloc.h>

void fun2()
{
	void *ptr = NULL;
	FILE  *fp = NULL;

	ptr = dxmalloc (1);
	dxlog("dxmalloc ptr: %p", ptr);
	dxfree(ptr);

	ptr = dxcalloc (1, 1);
	dxlog("dxcalloc ptr: %p", ptr);
	dxfree(ptr);

	ptr = dxrealloc (0, 1);
	dxlog("dxrealloc ptr: %p", ptr);
	dxfree(ptr);

	fp = dxfopen ("/tmp/hello", "a+");
	dxlog("dxfopen ptr: %p", fp);
	dxfclose(fp);

	ptr = dxstrdup ("Helloworld!!!");
	dxlog("dxstrdup ptr: %p", ptr);
	dxfree(ptr);

	int retval = 0;
	retval = dxasprintf((char **)&ptr, "DXTRACE: %s:%d %s: __testing_dxasprintf__", __FILE__, __LINE__, __func__);
	dxlog("dxasprintf ptr: %p", ptr);
	dxfree(ptr);

	fp = dxfopen (NULL, "a+");
	dxlog("dxfopen ptr: %p", fp);
	dxfclose(fp);

	return;
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
