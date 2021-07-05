# UI
A `C++20` and [GTK 3](https://www.gtk.org/) based implementation for the jutta coffee maker UI to control JURA coffee makers.
The used protocol can be found [here](https://github.com/Jutta-Proto/protocol-cpp).

## Example
### UI example
![UI example image](https://user-images.githubusercontent.com/11741404/115422997-b925eb80-a1fd-11eb-8208-96093122b28e.png)


### Working Prototype with an JURA E6
https://user-images.githubusercontent.com/11741404/115424324-e757fb00-a1fe-11eb-8d50-90d94fd486d3.mp4

Instructions for building the prototype can be found here: https://github.com/Jutta-Proto/hardware-pi

## Requirements
The following requirements are required to build this project.
* A C++20 compatible compiler like [gcc](https://gcc.gnu.org/) or [clang](https://clang.llvm.org/)
* The build system is written using [CMake](https://cmake.org/)
* For managing dependencies in CMake, we are using [conan](https://conan.io/)
* Since the user interface is build based on [GTK3](https://www.gtk.org/), an up to date GTK3 installation is also required.

### Fedora
To install those dependencies on Fedora, run the following commands:
```bash
sudo dnf install -y gcc clang cmake python3 python3-pip gtk3-devel
pip3 install --user conan
```

### Raspberry Pi
To install those dependencies on a Raspberry Pi, running the [Raspberry Pi OS](https://www.raspberrypi.org/software/), run the following commands:
```bash
sudo apt install -y cmake python3 python3-pip libgtk-3-dev
pip3 install --user conan
```
For all the other requirements, head over here: https://github.com/Jutta-Proto/hardware-pi#raspberry-pi-os

## Building
Run the following commands to build this project:
```bash
# Clone the repository:
git clone https://github.com/Jutta-Proto/gtk-ui.git
# Switch into the newly cloned repository:
cd gtk-ui
# Build the project:
mkdir build
cd build
cmake ..
cmake --build .
```
