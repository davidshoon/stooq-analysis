# requires python 3.5+

import matplotlib.pyplot as plt
from scipy import stats
import glob

def whereIs(line, substr):
	x = line.split(",")
	count = 0
	for i in x:
		if i == substr:
			break
		count += 1

	return count

def whereIsTicker(line):
	return whereIs(line, '<TICKER>')

def whereIsDate(line):
	return whereIs(line, '<DATE>')

def whereIsClose(line):
	return whereIs(line, '<CLOSE>')

my_path = "data/daily/us/nasdaq stocks"

files = glob.glob(my_path + '/**/*.txt', recursive=True)

slope_list = []

for file in files:
	filename = file.split('/')[-1]
	filename = filename.split('.')[0]

	x = []
	y = []

	ticker_symbol = ''

	with open(file, 'r') as in_file:
		line = in_file.readline()
		ticker_index = whereIsTicker(line)
		date_index = whereIsDate(line)
		close_index = whereIsClose(line)


		for line in in_file:
			tokens = line.split(',')
			ticker_symbol = tokens[ticker_index]
			x.append(int(tokens[date_index]))
			y.append(float(tokens[close_index]))


	if len(x) != 0 or len(x) != 0:
		slope, intercept, r, p, std_err = stats.linregress(x, y)
#		print(ticker_symbol, slope, intercept, r, p, std_err)

		slope_list.append({ 'ticker': ticker_symbol, 'slope': slope })

# graphics/plotting doesn't work on my machine...

#		def myfunc(x):
#			return slope * x + intercept

#		mymodel = list(map(myfunc, x))

#		plt.scatter(x, y)
#		plt.plot(x, mymodel)
#		plt.show()

#		break



# sorting doesn't work...

#def slope_sort(e):
#	return float(e['slope'])

#slope_list.sort(key=slope_sort)


for i in slope_list:
	print(i['ticker'], i['slope'])
