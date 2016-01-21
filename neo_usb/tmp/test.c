#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <usbd_core.h>
void _init() {

}

int main()
{
USBD_Init(NULL,
	            0,
	            NULL,
	            NULL,
	            NULL);
	
	return 1;
}