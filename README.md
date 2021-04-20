# UI
A `C++20` and [GTK 3](https://www.gtk.org/) based implementation for the jutta coffee maker UI to control JURA coffee makers.
The used protocol can be found [here](https://github.com/Jutta-Proto/protocol-cpp).

## Example
### UI example
![UI example image](https://user-images.githubusercontent.com/11741404/115422997-b925eb80-a1fd-11eb-8208-96093122b28e.png)


### Working Prototype with an JURA E6
Click on the image to watch a short demo Video.  
[<img src="https://user-images.githubusercontent.com/11741404/115422271-0a81ab00-a1fd-11eb-9858-a0602a9e9851.png" width="400">](https://home.in.tum.de/~sauterf/html-data/coffee_maker_demo_pi_new.MOV)

## Building
```bash
$ git clone https://github.com/Jutta-Proto/gtk-ui.git
$ cd gtk-ui
$ mkdir build
$ cd build
$ cmake ..
$ cmake --build .
```
