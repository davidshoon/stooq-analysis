/*
	Example/test parser for CSV files from stooq database.

*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdint.h>

#include <assert.h>

#include <vector>

struct Line
{
	int32_t date;
	double open;
	double high;
	double low;
	double close;
	long long volume;
	long long openint;
};

std::vector <Line> g_lines;

int test_first_line(char *buf)
{
	char *t;
	int ok = 0;

	t = strtok(buf, ",");
	if (t && strcasecmp(t, "Date") != 0) assert(0);

	t = strtok(NULL, ",");
	if (t && strcasecmp(t, "Open") != 0) assert(0);

	t = strtok(NULL, ",");
	if (t && strcasecmp(t, "High") != 0) assert(0);

	t = strtok(NULL, ",");
	if (t && strcasecmp(t, "Low") != 0) assert(0);

	t = strtok(NULL, ",");
	if (t && strcasecmp(t, "Close") != 0) assert(0);

	t = strtok(NULL, ",");
	if (t && strcasecmp(t, "Volume") != 0) assert(0);

	t = strtok(NULL, "\r\n");
	if (t && strcasecmp(t, "OpenInt") != 0) assert(0);

	ok = 1;

	return ok;
}

int main()
{
	char buf[1024];

	fgets(buf, sizeof(buf), stdin);

	if (test_first_line(buf)) {
		printf("OK!\n");
	}

	while (fgets(buf, sizeof(buf), stdin)) {
		Line line;
		sscanf(buf, "%d,%lf,%lf,%lf,%lf,%lld,%lld", &line.date, &line.open, &line.high, &line.low, &line.close, &line.volume, &line.openint);

		printf("%d,%lf,%lf,%lf,%lf,%lld,%lld\n", line.date, line.open, line.high, line.low, line.close, line.volume, line.openint);

		g_lines.push_back(line);
	}
}
