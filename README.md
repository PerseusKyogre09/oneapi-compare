# Parallel Image Processing using Intel oneAPI DPC++

## Project Overview

This project applies basic image processing operations (grayscale conversion, edge detection, and blurring) using Intel oneAPI DPC++ to demonstrate performance benefits of parallel computing on CPU/GPU.

## Key Features

✔️ Implements **basic image filters** (Grayscale, Edge Detection, Blur)

✔️ **Compares execution time**:
* Standard **OpenCV (CPU, serial)** vs.
* **oneAPI DPC++ (parallel, CPU/GPU)**

✔️ Uses **Intel VTune Profiler** or **Intel Advisor** to measure performance gains

✔️ Portable—Runs on **any Intel CPU/GPU with oneAPI**

## 🛠️ Tools & Technologies

🔹 **oneAPI DPC++ (Data Parallel C++)**

🔹 **OpenCV** (for image loading & display)

🔹 **Intel VTune Profiler** (for performance analysis)

🔹 **Intel Advisor** (for profiling & optimization suggestions)

🔹 **C++ with SYCL/DPC++**


## Project Structure

```
parallel-image-processing-oneAPI
│── main.cpp            # Main code with oneAPI DPC++ implementation
│── serial_opencv.cpp   # Baseline CPU (OpenCV) version
│── images/             # Sample images for testing
│── Makefile            # Build instructions for Intel oneAPI
│── README.md           # Documentation & results
```

## Installation Requirements

1. Install Intel oneAPI Base Toolkit
2. Install OpenCV
3. Configure environment variables

## Building the Project

```bash
# Initialize oneAPI environment
source /opt/intel/oneapi/setvars.sh

# Build the project
make
```

## Usage

```bash
# Run parallel implementation
./parallel_image_processor <input_image> <output_image> <filter_type>

# Run serial implementation
./serial_image_processor <input_image> <output_image> <filter_type>
```

Filter types: `grayscale`, `edge`, `blur`

## Results

Performance comparison between serial OpenCV implementation and parallel oneAPI DPC++ implementation:

| Filter Type | OpenCV (ms) | oneAPI DPC++ (ms) | Speedup |
|-------------|-------------|-------------------|---------|
| Grayscale   | 18.4 ms     | 2.3 ms            | 8.0x    |
| Edge        | 61.1 ms     | 4.1 ms            | 14.9x   |
| Blur        | 47.3 ms     | 3.8 ms            | 12.4x   |

Tests conducted on Intel(R) Arc(TM) A770 Graphics GPU.

## Technical Challenges

During the development and testing of this project, I encountered several technical challenges:

1. **Parallelization Strategy**: Finding the optimal work group size and distribution strategy required careful tuning to maximize GPU utilization.

2. **Memory Transfer Optimization**: Reducing the overhead of transferring image data between host and device memory was critical for achieving high performance.

3. **Algorithm Adaptation**: Adapting traditional image processing algorithms to fit the SYCL/DPC++ programming model required rethinking some fundamental approaches.

4. **Kernel Optimization**: Fine-tuning kernel code to take advantage of GPU-specific features while maintaining portability across different Intel architectures.

The significant performance improvements demonstrate that these challenges were successfully addressed, resulting in an implementation that efficiently leverages GPU acceleration.

## Future Improvements

* Add more complex filters
* Implement batch processing
* Optimize kernel parameters
* Support for more image formats
* Benchmark on various Intel GPU architectures
