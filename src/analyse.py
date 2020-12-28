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
#my_path = "data/daily/jp/tse stocks"
# do linear regression only after this date... (i.e. last 30 days of trade)
after_date = 20201201

# only include stocks that have been around since this date (i.e. 10 year old companies)
been_around_since = 20100101

files = glob.glob(my_path + '/**/*.txt', recursive=True)

slope_list = []

for file in files:
	filename = file.split('/')[-1]
	filename = filename.split('.')[0]

	x = []
	y = []

	ticker_symbol = ''
	been_around_since_ok = False

	with open(file, 'r') as in_file:
		line = in_file.readline()

		ticker_index = whereIsTicker(line)
		date_index = whereIsDate(line)
		close_index = whereIsClose(line)

		for line in in_file:
			tokens = line.split(',')
			ticker_symbol = tokens[ticker_index]

			if int(tokens[date_index]) < been_around_since:
				been_around_since_ok = True

			if int(tokens[date_index]) >= after_date:
				x.append(int(tokens[date_index]))
				y.append(float(tokens[close_index]))


	if (len(x) != 0) and (been_around_since_ok == True):
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
