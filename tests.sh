make -C tests/server run > /dev/null

# Server coverage
line_cov=$(  gcovr --exclude tests     | tail -n 2 | head -n 1 | sed -n 's/.* \([0-9]\+%$\)/\1/p')
branch_cov=$(gcovr --exclude tests -b  | tail -n 2 | head -n 1 | sed -n 's/.* \([0-9]\+%$\)/\1/p')
echo "Server - Line coverage:   $line_cov"

# AI coverage
cp tests/ai/test_main.py .
coverage run -m unittest discover > /dev/null 2> /dev/null
ai_line_cov=$(coverage report -m | tail -n 1 | sed -n 's/.* \([0-9]\+%$\)/\1/p')
echo "Server - Branch coverage: $branch_cov"
echo "  AI   - Coverage:        $line_cov"
rm test_main.py
