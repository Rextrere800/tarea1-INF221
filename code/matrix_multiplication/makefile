CXX = g++
CXXFLAGS = -O2 -std=c++17
TARGET = a.exe
SRC = matrix_multiplication.cpp algorithms/naive.cpp algorithms/strassen.cpp
PYTHON = python

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

generate:
	cd scripts && $(PYTHON) matrix_generator.py

plots:
	cd scripts && $(PYTHON) plot_generator.py

run: all
	$(TARGET)

report: run plots

clean:
	del $(TARGET)

clean-data:
	del /Q data\matrix_output\*.txt
	del /Q data\measurements\*.txt
	del /Q data\plots\*.png data\plots\*.csv