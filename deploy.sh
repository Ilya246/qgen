mkdir -p bin
g++ qgen.cpp -o bin/qgen -O3
x86_64-w64-mingw32-g++-posix qgen.cpp -o bin/qgen.exe -O3 --static

cd bin

strip qgen
strip qgen.exe

zip qgen_win64.zip qgen.exe
tar -czvf qgen_linux.tar.gz qgen
