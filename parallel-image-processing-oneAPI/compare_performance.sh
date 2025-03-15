#!/bin/bash

# Performance comparison script for serial vs parallel image processing
# Author: Intel Student Ambassador Program

# Colors for output
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Create output directory if it doesn't exist
mkdir -p outputs
mkdir -p outputs/images
mkdir -p outputs/reports

# Sample image
IMAGE_PATH="images/test.jpg"
REPORT_FILE="outputs/reports/performance_comparison_$(date '+%Y%m%d_%H%M%S').txt"

# Check if image exists, download if not
if [ ! -f "$IMAGE_PATH" ]; then
    echo -e "${YELLOW}Test image not found. Downloading sample image...${NC}"
    mkdir -p images
    wget -O $IMAGE_PATH https://source.unsplash.com/random/800x600/?nature
    
    if [ ! -f "$IMAGE_PATH" ]; then
        echo "Failed to download test image. Please provide a test image manually."
        exit 1
    fi
fi

# Check if executables exist
if [ ! -f "./serial_image_processor" ] || [ ! -f "./parallel_image_processor" ]; then
    echo -e "${YELLOW}Executables not found. Building project...${NC}"
    make clean
    make all
    
    if [ ! -f "./serial_image_processor" ] || [ ! -f "./parallel_image_processor" ]; then
        echo "Failed to build executables. Please check your build environment."
        exit 1
    fi
fi

# Initialize report file
echo "=========================================================" > $REPORT_FILE
echo "   Performance Comparison: Serial vs Parallel Processing  " >> $REPORT_FILE
echo "   Generated on: $(date)                                  " >> $REPORT_FILE
echo "   Image: $IMAGE_PATH                                     " >> $REPORT_FILE
echo "=========================================================" >> $REPORT_FILE
echo "" >> $REPORT_FILE

# Function to run tests for a specific filter
run_comparison() {
    local filter=$1
    local serial_output="outputs/images/serial_${filter}.jpg"
    local parallel_output="outputs/images/parallel_${filter}.jpg"
    
    echo -e "${BLUE}Running $filter filter comparison...${NC}"
    echo "Filter: $filter" >> $REPORT_FILE
    echo "-----------------------------------------------------------" >> $REPORT_FILE
    
    # Run serial version and extract timing
    echo -e "${YELLOW}Running serial version...${NC}"
    serial_output_text=$(./serial_image_processor $IMAGE_PATH $serial_output $filter 2>&1)
    serial_time=$(echo "$serial_output_text" | grep "$filter.*OpenCV" | grep -oP '\d+\.\d+(?= ms)')
    echo "$serial_output_text" >> $REPORT_FILE
    
    # Run parallel version and extract timing
    echo -e "${YELLOW}Running parallel version...${NC}"
    parallel_output_text=$(./parallel_image_processor $IMAGE_PATH $parallel_output $filter 2>&1)
    parallel_time=$(echo "$parallel_output_text" | grep "$filter.*DPC++" | grep -oP '\d+\.\d+(?= ms)')
    echo "$parallel_output_text" >> $REPORT_FILE
    
    # Calculate speedup
    if [ ! -z "$serial_time" ] && [ ! -z "$parallel_time" ] && [ "$parallel_time" != "0" ]; then
        speedup=$(echo "scale=2; $serial_time / $parallel_time" | bc)
        echo -e "${GREEN}Speedup: ${speedup}x${NC}"
        echo "Speedup: ${speedup}x" >> $REPORT_FILE
    else
        echo -e "${YELLOW}Could not calculate speedup${NC}"
        echo "Could not calculate speedup (missing timing data)" >> $REPORT_FILE
    fi
    
    echo "" >> $REPORT_FILE
    
    # Verify output files exist
    if [ -f "$serial_output" ] && [ -f "$parallel_output" ]; then
        echo -e "${GREEN}Output images saved to: ${serial_output} and ${parallel_output}${NC}"
    else
        echo -e "${YELLOW}Warning: One or more output files were not created${NC}"
    fi
}

# Run all filters
for filter in "grayscale" "edge" "blur"; do
    run_comparison $filter
done

# Generate summary
echo "=========================================================" >> $REPORT_FILE
echo "                        SUMMARY                          " >> $REPORT_FILE
echo "=========================================================" >> $REPORT_FILE

# Get device info
echo "Device Information:" >> $REPORT_FILE
if [ -f "./parallel_image_processor" ]; then
    device_info=$(./parallel_image_processor $IMAGE_PATH outputs/dummy.jpg grayscale 2>&1 | grep "Selected device")
    echo "$device_info" >> $REPORT_FILE
fi

# Check for Intel VTune and add recommendations
echo "" >> $REPORT_FILE
echo "Recommendations:" >> $REPORT_FILE
echo "- For more detailed performance analysis, run with Intel VTune Profiler:" >> $REPORT_FILE
echo "  vtune -collect hotspots -result-dir vtune_results ./parallel_image_processor $IMAGE_PATH outputs/vtune_test.jpg grayscale" >> $REPORT_FILE
echo "" >> $REPORT_FILE
echo "- For optimization suggestions, run with Intel Advisor:" >> $REPORT_FILE
echo "  advisor -collect survey -project-dir advisor_results -- ./parallel_image_processor $IMAGE_PATH outputs/advisor_test.jpg grayscale" >> $REPORT_FILE

echo -e "${GREEN}Comparison complete!${NC}"
echo -e "${GREEN}Report saved to: $REPORT_FILE${NC}"
echo -e "${YELLOW}To view the report: cat $REPORT_FILE${NC}"