#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {

	if (argc != 5)
	{
		fprintf(stderr, "Usage: %s <input_file> <output_file> <order:0|1> <memory_limit, Mb>\n", argv[0]);
		return 1;
	}

	auto input_filename = argv[1];
	auto output_filename = argv[2];
	auto order = atoi(argv[3]);
	auto memory_limit = atoi(argv[4]);

	printf("Input:\t\t%s\n", input_filename);
	printf("Output:\t\t%s\n", output_filename);
	printf("Order:\t\t%d\n", order);
	printf("Memory limit:\t%d\n", memory_limit);

	auto input_file_ptr = fopen(input_filename, "r");
	if (input_file_ptr == NULL)
	{
		fprintf(stderr, "Cannot open the file: %s", input_filename);
		return 1;
	}

	fclose(input_file_ptr);

	getchar();

	return 0;
}
