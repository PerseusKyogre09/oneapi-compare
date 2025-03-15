# Makefile for Parallel Image Processing with oneAPI DPC++

# Compiler settings
CXX = dpcpp
CXXFLAGS = -std=c++17 -O3 -g -Wall

# OpenCV settings
OPENCV_LIBS = `pkg-config --libs opencv4`
OPENCV_CFLAGS = `pkg-config --cflags opencv4`

# Targets
all: parallel_image_processor serial_image_processor

# Parallel version using oneAPI DPC++
parallel_image_processor: main.cpp
	$(CXX) $(CXXFLAGS) $(OPENCV_CFLAGS) $< -o $@ $(OPENCV_LIBS)

# Serial version using OpenCV
serial_image_processor: serial_opencv.cpp
	g++ -std=c++17 -O3 -g -Wall $(OPENCV_CFLAGS) $< -o $@ $(OPENCV_LIBS)

# Create images directory if it doesn't exist
images:
	mkdir -p images

# Clean up
clean:
	rm -f parallel_image_processor serial_image_processor

# Run the performance test
test: all images
	./serial_image_processor images/test.jpg images/output_serial.jpg grayscale
	./parallel_image_processor images/test.jpg images/output_parallel.jpg grayscale
	./serial_image_processor images/test.jpg images/output_serial_edge.jpg edge
	./parallel_image_processor images/test.jpg images/output_parallel_edge.jpg edge
	./serial_image_processor images/test.jpg images/output_serial_blur.jpg blur
	./parallel_image_processor images/test.jpg images/output_parallel_blur.jpg blur

.PHONY: all clean test images
