CXX ?= g++
CXXFLAGS = -std=c++11 -Wall -Wextra
SRCS = $(wildcard src/*.cpp) $(wildcard src/optional/*.cpp)

stardew: $(SRCS) $(wildcard src/*.h) $(wildcard src/optional/*.h)
	$(CXX) $(CXXFLAGS) $(SRCS) -o stardew

run: stardew
	./stardew

clean:
	rm -f stardew stardew.exe
