# oneapi-compare Instructions

## Introduction
Welcome to the oneapi-compare project! This is the set of steps I followed to install and set up everything.

## Prerequisites
Before you begin, ensure you have met the following requirements:
- [ ] You have installed [mingw](https://sourceforge.net/projects/mingw/) and setup.


## Installation
To setup, follow these steps:
1. Download the Intel oneAPI Base Toolkit Installer:
    ```bash
    wget https://registrationcenter-download.intel.com/akdlm/IRC_NAS/20f4e6a1-6b0b-4752-b8c1-e5eacba10e01/l_BaseKit_p_2024.0.0.49564_offline.sh
    ```
2. Make the installer executable:
    ```bash
    chmod +x l_BaseKit_p_2024.0.0.49564_offline.sh
    ```
3. Run the installer:
    ```bash
    sudo ./l_BaseKit_p_2024.0.0.49564_offline.sh
    ```
4. Install the OpenCV
    ```bash
    sudo apt update
    sudo apt install libopencv-dev
    ```

## Usage
To use the tool, follow these steps:
1. Setup the environment for the project:
     ```bash
     source /opt/intel/oneapi/setvars.sh
     ```
2. Run the following commands
    ```bash
    ./serial_image_processor images/test.jpg images/output_serial.jpg grayscale
    ./parallel_image_processor images/test.jpg images/output_parallel.jpg grayscale

    ```
3. Alternate Method:
    I have created a `compare_performance.sh` for taking the input for `test.jpg` and giving output in `outputs` directory. Run it using:
    ```bash
    ./compare_performance.sh
    ```


## Contributing
If you would like to contribute to this project, please follow these steps:
1. Fork the repository.
2. Create a new branch (`git checkout -b feature-branch`).
3. Make your changes and commit them (`git commit -m 'Add some feature'`).
4. Push to the branch (`git push origin feature-branch`).
5. Create a new Pull Request.

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Contact
If you have any questions, please open an issue or contact [me.](mailto:kyogre.perseus09@gmail.com).
