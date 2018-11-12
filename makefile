CXX = sudo g++
CXXFLAGS = -std=c++17
blight: main.cpp
	$(CXX) $(CXXFLAGS) -o /usr/bin/blight  main.cpp
