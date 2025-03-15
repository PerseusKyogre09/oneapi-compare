#include <CL/sycl.hpp>
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <chrono>

//namespace sycl = cl::sycl;

// Function to convert image to grayscale using DPC++
void convertToGrayscaleDPCPP(const cv::Mat& input, cv::Mat& output) {
    // Get dimensions
    int width = input.cols;
    int height = input.rows;
    int channels = input.channels();
    
    // Ensure output has the right dimensions
    output = cv::Mat(height, width, CV_8UC1);
    
    // Create buffers for input and output
    std::vector<unsigned char> input_data(input.data, input.data + width * height * channels);
    std::vector<unsigned char> output_data(width * height);
    
    // Start timer
    auto start = std::chrono::high_resolution_clock::now();
    
    try {
        // Create a SYCL queue
        sycl::queue q{sycl::default_selector_v};
        std::cout << "Selected device: " << q.get_device().get_info<sycl::info::device::name>() << std::endl;
        
        // Create buffers
        sycl::buffer<unsigned char, 1> input_buffer(input_data.data(), sycl::range<1>(width * height * channels));
        sycl::buffer<unsigned char, 1> output_buffer(output_data.data(), sycl::range<1>(width * height));
        
        // Submit the kernel
        q.submit([&](sycl::handler& h) {
            auto in = input_buffer.get_access<sycl::access::mode::read>(h);
            auto out = output_buffer.get_access<sycl::access::mode::write>(h);
            
            h.parallel_for(sycl::range<1>(width * height), [=](sycl::id<1> idx) {
                int i = idx[0];
                int row = i / width;
                int col = i % width;
                
                // Calculate position in input array (RGB)
                int pos = (row * width + col) * channels;
                
                // Convert to grayscale using the formula: 0.299*R + 0.587*G + 0.114*B
                out[i] = static_cast<unsigned char>(
                    0.299f * in[pos] +
                    0.587f * in[pos + 1] +
                    0.114f * in[pos + 2]
                );
            });
        }).wait();
        
        // End timer
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = end - start;
        std::cout << "Grayscale conversion (DPC++): " << duration.count() << " ms" << std::endl;
        
        // Copy data back to output Mat
        std::memcpy(output.data, output_data.data(), width * height);
    }
    catch (sycl::exception& e) {
        std::cerr << "SYCL exception caught: " << e.what() << std::endl;
    }
}

// Function to apply edge detection using DPC++
void applyEdgeDetectionDPCPP(const cv::Mat& input, cv::Mat& output) {
    // Convert to grayscale first if needed
    cv::Mat gray;
    if (input.channels() == 3) {
        cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = input.clone();
    }
    
    // Get dimensions
    int width = gray.cols;
    int height = gray.rows;
    
    // Ensure output has the right dimensions
    output = cv::Mat(height, width, CV_8UC1);
    
    // Create buffers for input and output
    std::vector<unsigned char> input_data(gray.data, gray.data + width * height);
    std::vector<unsigned char> output_data(width * height);
    
    // Start timer
    auto start = std::chrono::high_resolution_clock::now();
    
    try {
        // Create a SYCL queue
        sycl::queue q{sycl::default_selector_v};
        
        // Create buffers
        sycl::buffer<unsigned char, 1> input_buffer(input_data.data(), sycl::range<1>(width * height));
        sycl::buffer<unsigned char, 1> output_buffer(output_data.data(), sycl::range<1>(width * height));
        
        // Define Sobel kernels
        int sobel_x[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
        int sobel_y[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
        
        // Submit the kernel
        q.submit([&](sycl::handler& h) {
            auto in = input_buffer.get_access<sycl::access::mode::read>(h);
            auto out = output_buffer.get_access<sycl::access::mode::write>(h);
            
            h.parallel_for(sycl::range<2>(height - 2, width - 2), [=](sycl::id<2> idx) {
                int row = idx[0] + 1;  // Add 1 to skip the border
                int col = idx[1] + 1;  // Add 1 to skip the border
                
                int gx = 0;
                int gy = 0;
                
                // Apply Sobel operators
                for (int i = -1; i <= 1; i++) {
                    for (int j = -1; j <= 1; j++) {
                        int pixel_index = (row + i) * width + (col + j);
                        gx += in[pixel_index] * sobel_x[i+1][j+1];
                        gy += in[pixel_index] * sobel_y[i+1][j+1];
                    }
                }
                
                // Calculate gradient magnitude
                int magnitude = std::min(255, static_cast<int>(std::sqrt(gx * gx + gy * gy)));
                
                // Set output pixel
                out[row * width + col] = static_cast<unsigned char>(magnitude);
            });
        }).wait();
        
        // End timer
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = end - start;
        std::cout << "Edge detection (DPC++): " << duration.count() << " ms" << std::endl;
        
        // Copy data back to output Mat
        std::memcpy(output.data, output_data.data(), width * height);
    }
    catch (sycl::exception& e) {
        std::cerr << "SYCL exception caught: " << e.what() << std::endl;
    }
}

// Function to apply blur using DPC++
void applyBlurDPCPP(const cv::Mat& input, cv::Mat& output) {
    // Get dimensions
    int width = input.cols;
    int height = input.rows;
    int channels = input.channels();
    
    // Ensure output has the right dimensions
    output = cv::Mat(height, width, input.type());
    
    // Create buffers for input and output
    std::vector<unsigned char> input_data(input.data, input.data + width * height * channels);
    std::vector<unsigned char> output_data(width * height * channels);
    
    // Start timer
    auto start = std::chrono::high_resolution_clock::now();
    
    try {
        // Create a SYCL queue
        sycl::queue q{sycl::default_selector_v};
        
        // Create buffers
        sycl::buffer<unsigned char, 1> input_buffer(input_data.data(), sycl::range<1>(width * height * channels));
        sycl::buffer<unsigned char, 1> output_buffer(output_data.data(), sycl::range<1>(width * height * channels));
        
        // Define the kernel size
        const int kernel_size = 5;
        const int kernel_radius = kernel_size / 2;
        
        // Submit the kernel
        q.submit([&](sycl::handler& h) {
            auto in = input_buffer.get_access<sycl::access::mode::read>(h);
            auto out = output_buffer.get_access<sycl::access::mode::write>(h);
            
            h.parallel_for(sycl::range<2>(height - 2*kernel_radius, width - 2*kernel_radius), [=](sycl::id<2> idx) {
                int row = idx[0] + kernel_radius;
                int col = idx[1] + kernel_radius;
                
                for (int c = 0; c < channels; c++) {
                    int sum = 0;
                    int count = 0;
                    
                    // Apply box filter
                    for (int i = -kernel_radius; i <= kernel_radius; i++) {
                        for (int j = -kernel_radius; j <= kernel_radius; j++) {
                            int pixel_index = ((row + i) * width + (col + j)) * channels + c;
                            sum += in[pixel_index];
                            count++;
                        }
                    }
                    
                    // Write average to output
                    out[(row * width + col) * channels + c] = static_cast<unsigned char>(sum / count);
                }
            });
        }).wait();
        
        // End timer
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = end - start;
        std::cout << "Blur (DPC++): " << duration.count() << " ms" << std::endl;
        
        // Copy data back to output Mat
        std::memcpy(output.data, output_data.data(), width * height * channels);
    }
    catch (sycl::exception& e) {
        std::cerr << "SYCL exception caught: " << e.what() << std::endl;
    }
}

int main(int argc, char* argv[]) {
    // Check command line arguments
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <input_image> <output_image> <filter_type>" << std::endl;
        std::cerr << "Filter types: grayscale, edge, blur" << std::endl;
        return 1;
    }
    
    std::string input_path = argv[1];
    std::string output_path = argv[2];
    std::string filter_type = argv[3];
    
    // Read input image
    cv::Mat input_image = cv::imread(input_path);
    if (input_image.empty()) {
        std::cerr << "Error: Could not read input image." << std::endl;
        return 1;
    }
    
    cv::Mat output_image;
    
    // Apply the selected filter
    if (filter_type == "grayscale") {
        convertToGrayscaleDPCPP(input_image, output_image);
    } else if (filter_type == "edge") {
        applyEdgeDetectionDPCPP(input_image, output_image);
    } else if (filter_type == "blur") {
        applyBlurDPCPP(input_image, output_image);
    } else {
        std::cerr << "Error: Unknown filter type. Choose from grayscale, edge, blur." << std::endl;
        return 1;
    }
    
    // Save output image
    cv::imwrite(output_path, output_image);
    std::cout << "Output image saved to " << output_path << std::endl;
    
    return 0;
}
