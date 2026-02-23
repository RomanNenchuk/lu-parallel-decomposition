CXX = g++
CXXFLAGS = -O3 -fopenmp -Iinclude

SRC = src/main.cpp \
      src/matrix.cpp \
      src/lu_sequential.cpp \
      src/lu_parallel_for.cpp \
      src/lu_parallel_task.cpp \
      src/io.cpp

TARGET = lu

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)