build: *
	clang++ -Wall -std=c++2a src/main.cpp -ledit -o a.out
test: *
	clang++ -Wall -std=c++2a src/test.cpp -ledit -o test
	./test
