#include <unistd.h>
int main()
{
	write(STDOUT_FILENO, "Hello world!",  12);
	return 0;
}
