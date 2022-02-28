inputFile = test1.csv
outputFile = EGraphSage

all: clean compile
	
compile: src/EGraphSage.cpp
	g++ -std=c++11 src/*.cpp -o $(outputFile)

run: $(outputFile)
	./$(outputFile) -i $(inputFile)

clean:
	rm -rf objs/*
	rm -rf $(outputFile)