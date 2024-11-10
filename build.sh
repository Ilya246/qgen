mkdir -p bin
g++ qgen.cpp -o bin/qgen -O3 -march=native -mtune=native -Wall -Wextra -pedantic
