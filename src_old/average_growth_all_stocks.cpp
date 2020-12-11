/*
	Calculate the average daily growth for every stock/ticker symbol, in each .csv file.

	Requires the CSV database from https://stooq.com/db/h/

	By David Shoon
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <math.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#include <stdint.h>
#include <assert.h>
#include <vector>

const int MAX_PATHNAME_LEN = 2048;
const int MAX_FILENAME_LEN = 2048;

const int MAX_HASH_DIGEST = 1024;

struct FileInfo
{
	char fullpathname[MAX_PATHNAME_LEN]; 
	char pathname[MAX_PATHNAME_LEN]; // pathname only -- does not include filename
	char filename[MAX_FILENAME_LEN];

	time_t last_modification;
	off_t file_size;
};

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

struct bad_first_line_exception
{};

std::vector <Line> g_lines;
static FILE *output_fp{nullptr};

void analyse(FileInfo &fileinfo);

int test_first_line(char *buf)
{
	char *t;
	int ok = 0;

	t = strtok(buf, ",");
	if (t && strcasecmp(t, "Date") != 0) throw bad_first_line_exception();

	t = strtok(NULL, ",");
	if (t && strcasecmp(t, "Open") != 0) throw bad_first_line_exception();

	t = strtok(NULL, ",");
	if (t && strcasecmp(t, "High") != 0) throw bad_first_line_exception();

	t = strtok(NULL, ",");
	if (t && strcasecmp(t, "Low") != 0) throw bad_first_line_exception();

	t = strtok(NULL, ",");
	if (t && strcasecmp(t, "Close") != 0) throw bad_first_line_exception();

	t = strtok(NULL, ",");
	if (t && strcasecmp(t, "Volume") != 0) throw bad_first_line_exception();

	t = strtok(NULL, "\r\n");
	if (t && strcasecmp(t, "OpenInt") != 0) throw bad_first_line_exception();

	ok = 1;

	return ok;
}

void read_file(FILE *fp)
{
	char buf[1024];

	fgets(buf, sizeof(buf), fp);

	if (test_first_line(buf)) {
//		printf("OK!\n");
	}

	g_lines.clear();

	while (fgets(buf, sizeof(buf), fp)) {
		Line line;
		sscanf(buf, "%d,%lf,%lf,%lf,%lf,%lld,%lld", &line.date, &line.open, &line.high, &line.low, &line.close, &line.volume, &line.openint);

//		printf("%d,%lf,%lf,%lf,%lf,%lld,%lld\n", line.date, line.open, line.high, line.low, line.close, line.volume, line.openint);

		g_lines.push_back(line);
	}
}



// ---------------------------------- begin code ------------------------------------

void recursive_analyse_files(char *root)
{
	DIR *rootdir;
	char pathname[MAX_PATHNAME_LEN];
	char saved_pathname[MAX_PATHNAME_LEN];

	getcwd(saved_pathname, MAX_PATHNAME_LEN);

	chdir(root);

	getcwd(pathname, MAX_PATHNAME_LEN);

	rootdir = opendir(".");

	struct dirent *dent;

	while ((dent = readdir(rootdir)) != NULL)
	{
		if (strcmp(dent->d_name, ".") == 0)
			continue;

		if (strcmp(dent->d_name, "..") == 0)
			continue;

		struct stat statbuf;

		if (lstat(dent->d_name, &statbuf) == -1)
		{
			perror("lstat");
			printf("dent->d_name: %s\n", dent->d_name);
			continue;
		}

		if (S_ISREG(statbuf.st_mode))
		{
			FileInfo finfo;

			strcpy(finfo.fullpathname, pathname);
			strcat(finfo.fullpathname, "/");
			strcat(finfo.fullpathname, dent->d_name);

			strcpy(finfo.pathname, pathname);
			strcpy(finfo.filename, dent->d_name);

			finfo.last_modification = statbuf.st_mtime;
			finfo.file_size = statbuf.st_size;

			analyse(finfo);
		}

		else if (S_ISDIR(statbuf.st_mode))
		{
			recursive_analyse_files(dent->d_name);
		}
	}

	closedir(rootdir);

	chdir(saved_pathname);
}

void analyse(FileInfo &fileinfo)
{
	FILE *fp;

	fp = fopen(fileinfo.fullpathname, "rb");
	if (!fp)
	{
		perror("fopen");
		printf("%s\n", fileinfo.fullpathname);
		return;
	}

	try {
		read_file(fp);

		double total_daily_growth = 0.0;
		long number_of_days = g_lines.size();
		double average_daily_growth = 0.0;

		for (int i = 0; i < g_lines.size(); i++) {
			total_daily_growth += (g_lines[i].close - g_lines[i].open) / g_lines[i].open;
		}

		average_daily_growth = total_daily_growth / number_of_days;

		fprintf(output_fp, "%s,%lf\n", fileinfo.fullpathname, average_daily_growth);

	}

	catch (bad_first_line_exception ex) {
		printf("Error reading file: %s\n", fileinfo.fullpathname);
	}

	fclose(fp);
}

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		printf("Usage: %s <output file> <root directory to scan>\n", argv[0]);
		return 0;
	}

	output_fp = fopen(argv[1], "wb");
	if (!output_fp) { perror("fopen"); exit(1); }

	recursive_analyse_files(argv[2]);
}
