#include <stdio.h>

int main() {
	FILE *fp = NULL;
	if ((fp = fopen("timeanalysis.txt", "wb")) == NULL) {
		printf("failed to open file.");
		return -1;
	}

	fprintf(fp,"Test to write file.");
	fclose(fp);

	return 0;
}
