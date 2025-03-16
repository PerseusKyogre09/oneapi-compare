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
| Grayscale   | TBD         | TBD               | TBD     |
| Edge        | TBD         | TBD               | TBD     |
| Blur        | TBD         | TBD               | TBD     |

## Problems Encountered

During the development and testing of this project, I encountered several challenges:

1. **Performance Discrepancy**: The performance results show that the parallel implementation is actually slower than the serial implementation. This is contrary to what we would expect from a parallel implementation.

2. **Hardware Limitations**: The main issue is that I'm currently testing on a system without a dedicated GPU. The oneAPI DPC++ implementation is specifically optimized for GPU acceleration, but it's running on CPU fallback mode.

3. **CPU Fallback Inefficiency**: When running on CPU, the DPC++ runtime introduces overhead that makes it less efficient than the highly optimized OpenCV library, which is already well-optimized for CPU execution.

4. **Misleading Benchmarks**: The current performance metrics are not representative of the true potential of the parallel implementation, as they don't showcase the GPU acceleration capabilities.

To get accurate performance comparisons, this code should be tested on a system with Intel GPU support. The current results reflect the limitations of running GPU-optimized code on CPU hardware rather than actual algorithm efficiency.

## Future Improvements

- Add more complex filters
- Implement batch processing
- Optimize kernel parameters
- Support for more image formats
