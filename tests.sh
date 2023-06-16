make -C tests/server run
gcovr --exclude tests
gcovr -b --exclude tests
