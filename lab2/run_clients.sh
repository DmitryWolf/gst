#!/bin/bash

./client 1000 10000 5
python3 average.py

./client 5000 10000 5
python3 average.py

./client 10000 10000 5
python3 average.py

./client 20000 10000 5
python3 average.py

./client 20000 15000 5
python3 average.py

rm client server