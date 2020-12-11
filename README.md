# stooq-analysis
Stock Analysis programs for stooq's database, using their CSV file format.

What it does: it performs a linear regression on the files inside the US stock data, 
returning the slope (gradient) of the stock.

It might not be a fair comparison, since it compares stock from since they were first listed,
and not by recent dates. This skews it towards IPOs being better stocks to buy.

# To run

cd src
./compile.sh

virtualenv -p python3 venv
source venv/bin/active

pip install matplotlib
pip install scipy

unzip the data files from stooq...

modify analyse.py's path variable to point to the data directory of interest...

python3 analyse.py | ./sort


