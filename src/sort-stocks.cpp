/*

	Sorts the output from the average growth program.

	by David Shoon
*/



#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <string>
#include <vector>
#include <algorithm>

#include <unistd.h>

struct Line
{
	std::string filepath;
	double average_growth;
};

std::vector <Line> lines;

bool compare_lines(Line a, Line b)
{
	return (a.average_growth) < (b.average_growth);
}

char *strip_newline(char *s)
{
	char *p = strpbrk(s, "\r\n");
	if (p) *p = '\0';
	return s;
}

int main(int argc, char **argv)
{
	char buf[4*1024];

	if (argc < 2) {
		printf("Usage: %s <hash file name>\n", argv[0]);
		exit(1);
	}

	FILE *fp = fopen(argv[1], "rb");
	if (!fp) { perror("fopen"); exit(1); }

	while (fgets(buf, sizeof(buf), fp)) {
		strip_newline(buf);

		char *tok = strtok(buf, ",");
		Line line;
		line.filepath = tok;

		tok = strtok(NULL, "");
		sscanf(tok, "%lf", &line.average_growth);

		lines.push_back(line);
	}

	std::sort(lines.begin(), lines.end(), compare_lines);

	for (int i = 0; i < lines.size(); i++) {
		printf("%s,%lf\n", lines[i].filepath.c_str(), lines[i].average_growth);
	}
}
