#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>


void progress(size_t total, size_t current) {
	float percentage = ((float) current / (float) total) * 100.0;
	printf("\rConverting...\t(%.2f %%)", percentage);
	fflush(stdout);
}

int main(int argc, char const *argv[])
{
	if (argc < 2) {
		printf("Usage: %s path/to/file [FIND REPLACE]\n", argv[0]);
		printf("\nFIND and REPLACE can assume one of these options:\n");
		printf("  -C\tFor comma ',' charater (default for FIND)\n");
		printf("  -T\tFor tabular '\\t' charater\n");
		printf("  -P\tFor pipe '|' charater (default for REPLACE\n");
		return 1;
	}

	char find,
		 extension[4],
		 *replace = malloc(sizeof(char));

	int pathLenght = strlen(argv[1]);
	char * path = malloc(sizeof(char) * pathLenght);
	char * newPath = malloc(sizeof(char) * pathLenght);

	FILE * newFile;
	FILE * file = fopen(argv[1], "r");

	char *value = malloc(sizeof(char));
	char *aux = malloc(sizeof(char));
	size_t position = 0;
	size_t fileSize = 0;
	off_t offset;

	clock_t start, stop;
	double seconds = 0;


	find = ',';
	*replace = '|';
	strcpy(extension, "psv");

	if (argc >= 3) {
		if (strcmp(argv[2], "-T") == 0) {
			find = '\t';
		} else if (strcmp(argv[2], "-C") == 0) {
			find = ',';
		} else if (strcmp(argv[2], "-P") == 0) {
			find = '|';
		}
	}

	if (argc >= 4) {
		if (strcmp(argv[3], "-T") == 0) {
			*replace = '\t';
			strcpy(extension, "tsv");
		} else if (strcmp(argv[3], "-C") == 0) {
			*replace = ',';
			strcpy(extension, "csv");
		} else if (strcmp(argv[3], "-P") == 0) {
			*replace = '|';
			strcpy(extension, "psv");
		}
	}

	if (find == *replace) {
		printf("The 'find' and 'replace' characters are the same: '%c' '%c'!\n", find, *replace);
		return 1;
	}

	strncpy(path, argv[1], pathLenght - 4);
	snprintf(newPath, pathLenght + 1, "%s.%s", path, extension);
	strcpy(path, argv[1]);


	if (file != NULL) {
		printf("File '%s'\n", argv[1]);
		fseek(file, 0, SEEK_END);
		fileSize = ftell(file);
		rewind(file);

		newFile = fopen(newPath, "w+");

		start = clock();
		while (feof(file) == 0) {
			position += fread(value, sizeof(char), 1, file);

			if (*value == find){
				fwrite(replace, sizeof(char), 1, newFile);
			}else if (*value == '\\') {
				position += fread(aux, sizeof(char), 1, file);
				if (*aux != 'N') {
					fwrite(value, sizeof(char), 1, newFile);
					fwrite(aux, sizeof(char), 1, newFile);
				}
			} else {
				fwrite(value, sizeof(char), 1, newFile);
			}
			progress(fileSize, position);
		}
		
		// truncate the last character 
		fseek(newFile, -1, SEEK_END);
		offset = ftello(newFile);
		ftruncate(fileno(newFile), offset);

		stop = clock();
		seconds = (double)(stop - start) / CLOCKS_PER_SEC;
		printf("\nDone after %.3f seconds\n", seconds);
		printf("New file: %s\n", newPath);
		fclose(file);
		fclose(newFile);
		free(path);
		free(newPath);
		free(value);
		free(aux);
	}

	return 0;
}