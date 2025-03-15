#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <chrono>

// Function to convert image to grayscale using OpenCV
void convertToGrayscaleOpenCV(const cv::Mat& input, cv::Mat& output) {
    // Start timer
    auto start = std::chrono::high_resolution_clock::now();
    
    // Convert to grayscale using OpenCV
    cv::cvtColor(input, output, cv::COLOR_BGR2GRAY);
    
    // End timer
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    std::cout << "Grayscale conversion (OpenCV): " << duration.count() << " ms" << std::endl;
}

// Function to apply edge detection using OpenCV
void applyEdgeDetectionOpenCV(const cv::Mat& input, cv::Mat& output) {
    // Start timer
    auto start = std::chrono::high_resolution_clock::now();
    
    // Convert to grayscale if needed
    cv::Mat gray;
    if (input.channels() == 3) {
        cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = input.clone();
    }
    
    // Apply Sobel operator
    cv::Mat grad_x, grad_y;
    cv::Mat abs_grad_x, abs_grad_y;
    
    // Gradient X
    cv::Sobel(gray, grad_x, CV_16S, 1, 0, 3);
    cv::convertScaleAbs(grad_x, abs_grad_x);
    
    // Gradient Y
    cv::Sobel(gray, grad_y, CV_16S, 0, 1, 3);
    cv::convertScaleAbs(grad_y, abs_grad_y);
    
    // Total Gradient
    cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, output);
    
    // End timer
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    std::cout << "Edge detection (OpenCV): " << duration.count() << " ms" << std::endl;
}

// Function to apply blur using OpenCV
void applyBlurOpenCV(const cv::Mat& input, cv::Mat& output) {
    // Start timer
    auto start = std::chrono::high_resolution_clock::now();
    
    // Apply box filter
    cv::blur(input, output, cv::Size(5, 5));
    
    // End timer
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    std::cout << "Blur (OpenCV): " << duration.count() << " ms" << std::endl;
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
        convertToGrayscaleOpenCV(input_image, output_image);
    } else if (filter_type == "edge") {
        applyEdgeDetectionOpenCV(input_image, output_image);
    } else if (filter_type == "blur") {
        applyBlurOpenCV(input_image, output_image);
    } else {
        std::cerr << "Error: Unknown filter type. Choose from grayscale, edge, blur." << std::endl;
        return 1;
    }
    
    // Save output image
    cv::imwrite(output_path, output_image);
    std::cout << "Output image saved to " << output_path << std::endl;
    
    return 0;
}
