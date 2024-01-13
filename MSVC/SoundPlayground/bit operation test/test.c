#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<math.h>


int main(void) {

	//int64_t i = 32767;

	int a = ceil((double)23 / 8);

	printf("%d", a);

	int16_t ar[] = { 1,2,3,4,5,6,7,8,9,1,2,3,4,5,6,7,8,9,1,2,3,4,5,6,7,8,9 };
	FILE* wav = fopen("out", "wb");
	fwrite(ar, sizeof (int16_t), 27, wav);
	fclose(wav);
	return 0;
}