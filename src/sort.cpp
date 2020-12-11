#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <string>
#include <vector>
#include <algorithm>
#include <utility>

bool compare_func(std::pair <std::string, double> i, std::pair <std::string, double> j) { return i.second < j.second; }

int main()
{
	char buf[1024];
	char ticker[1024];
	double slope;

	std::vector<std::pair<std::string, double>> slope_list;

	while (fgets(buf, sizeof(buf), stdin)) {
		sscanf(buf, "%s %lf", ticker, &slope);

		slope_list.push_back(std::pair<std::string, double> (ticker, slope));
	}

	std::sort(slope_list.begin(), slope_list.end(), compare_func);

	for (auto &x: slope_list) {
		printf("%s %f\n", x.first.c_str(), x.second);
	}

}
