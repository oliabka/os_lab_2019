#include "swap.h"

void Swap(char *left, char *right)
{
	char temp = *(right + 0);
    *(right + 0) = *(left + 0);
    *(left + 0) = temp;
}
