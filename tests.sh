make -C tests/server run
gcovr --exclude tests
gcovr -b --exclude tests
cp tests/ai/test_main.py .
coverage run -m unittest discover
coverage report -m
rm test_main.py
