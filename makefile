build: src/*
	clang++ -Wall -std=c++2a src/*.cpp -ledit -o a.out
test: src/*
	clang++ -Dtestmode=true -Wall -lgtest -std=c++2a src/*.cpp -ledit -o test
	./test
