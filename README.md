# UI
A `C++20` and [GTK 3](https://www.gtk.org/) based implementation for the jutta coffee maker UI to control JURA coffee makers.
The used protocol can be found [here](https://github.com/Jutta-Proto/protocol-cpp).

## Example
### UI example
![UI example image](https://user-images.githubusercontent.com/11741404/115422997-b925eb80-a1fd-11eb-8208-96093122b28e.png)


### Working Prototype with an JURA E6
https://user-images.githubusercontent.com/11741404/115424324-e757fb00-a1fe-11eb-8d50-90d94fd486d3.mp4

## Building
```bash
$ pip3 install conan --user
$ git clone https://github.com/Jutta-Proto/gtk-ui.git
$ cd gtk-ui
$ mkdir build
$ cd build
$ cmake ..
$ cmake --build .
```
