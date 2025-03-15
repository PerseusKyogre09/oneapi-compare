# Parallel Image Processing using Intel oneAPI DPC++

A small yet impactful project for the Intel Student Ambassador Program

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

## Future Improvements

- Add more complex filters
- Implement batch processing
- Optimize kernel parameters
- Support for more image formats
