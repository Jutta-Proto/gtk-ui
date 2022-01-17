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
sudo dnf install -y gcc clang cmake python3 python3-pip gtk3-devel gtkmm30-devel bluez-libs-devel
pip3 install --user conan
```

### Raspberry Pi
To install those dependencies on a Raspberry Pi, running the [Raspberry Pi OS](https://www.raspberrypi.org/software/), run the following commands:
```bash
sudo apt install -y cmake python3 python3-pip libgtk-3-dev libgtkmm-3.0-dev libbluetooth-dev
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

## License and Copyright Notice 
This piece of software uses the following other libraries and dependencies:

### Catch2 (2.13.4)
Catch2 is mainly a unit testing framework for C++, but it also provides basic micro-benchmarking features, and simple BDD macros.

<details>
  <summary>License</summary>

```
Boost Software License - Version 1.0 - August 17th, 2003

Permission is hereby granted, free of charge, to any person or organization
obtaining a copy of the software and accompanying documentation covered by
this license (the "Software") to use, reproduce, display, distribute,
execute, and transmit the Software, and to prepare derivative works of the
Software, and to permit third-parties to whom the Software is furnished to
do so, all subject to the following:

The copyright notices in the Software and this entire statement, including
the above license grant, this restriction and the following disclaimer,
must be included in all copies of the Software, in whole or in part, and
all derivative works of the Software, unless such copies or derivative
works are solely in the form of machine-executable object code generated by
a source language processor.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
```

</details>

### Nlohmann JSON (3.9.1)
JSON for Modern C++.

<details>
  <summary>License</summary>

```
MIT License 

Copyright (c) 2013-2021 Niels Lohmann

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

</details>

### spdlog (1.8.5)
Very fast, header-only/compiled, C++ logging library.

<details>
  <summary>License</summary>

```
The MIT License (MIT)

Copyright (c) 2016 Gabi Melman.                                       

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

-- NOTE: Third party dependency used by this software --
This software depends on the fmt lib (MIT License),
and users must comply to its license: https://github.com/fmtlib/fmt/blob/master/LICENSE.rst
```

</details>

### Crypto++ (8.5.0)
Free C++ Class Library of Cryptographic Schemes.

<details>
  <summary>License</summary>

```
Compilation Copyright (c) 1995-2019 by Wei Dai.  All rights reserved.
This copyright applies only to this software distribution package
as a compilation, and does not imply a copyright on any particular
file in the package.

All individual files in this compilation are placed in the public domain by
Wei Dai and other contributors.

I would like to thank the following authors for placing their works into
the public domain:

Joan Daemen - 3way.cpp
Leonard Janke - cast.cpp, seal.cpp
Steve Reid - cast.cpp
Phil Karn - des.cpp
Andrew M. Kuchling - md2.cpp, md4.cpp
Colin Plumb - md5.cpp
Seal Woods - rc6.cpp
Chris Morgan - rijndael.cpp
Paulo Baretto - rijndael.cpp, skipjack.cpp, square.cpp
Richard De Moliner - safer.cpp
Matthew Skala - twofish.cpp
Kevin Springle - camellia.cpp, shacal2.cpp, ttmac.cpp, whrlpool.cpp, ripemd.cpp
Ronny Van Keer - sha3.cpp
Aumasson, Neves, Wilcox-O'Hearn and Winnerlein - blake2.cpp, blake2b_simd.cpp, blake2s_simd.cpp
Aaram Yun - aria.cpp, aria_simd.cpp
Han Lulu, Markku-Juhani O. Saarinen - sm4.cpp sm4_simd.cpp
Daniel J. Bernstein, Jack Lloyd - chacha.cpp, chacha_simd.cpp, chacha_avx.cpp
Andrew Moon - ed25519, x25519, donna_32.cpp, donna_64.cpp, donna_sse.cpp

The Crypto++ Library uses portions of Andy Polyakov's CRYPTOGAMS for Poly1305
scalar multiplication, aes_armv4.S, sha1_armv4.S and sha256_armv4.S. CRYPTOGAMS
is dual licensed with a permissive BSD-style license. The CRYPTOGAMS license is
reproduced below.

The Crypto++ Library uses portions of Jack Lloyd's Botan for ChaCha SSE2 and
AVX. Botan placed the code in public domain for Crypto++ to use.

The Crypto++ Library (as a compilation) is currently licensed under the Boost
Software License 1.0 (http://www.boost.org/users/license.html).

Boost Software License - Version 1.0 - August 17th, 2003

Permission is hereby granted, free of charge, to any person or organization
obtaining a copy of the software and accompanying documentation covered by
this license (the "Software") to use, reproduce, display, distribute,
execute, and transmit the Software, and to prepare derivative works of the
Software, and to permit third-parties to whom the Software is furnished to
do so, all subject to the following:

The copyright notices in the Software and this entire statement, including
the above license grant, this restriction and the following disclaimer,
must be included in all copies of the Software, in whole or in part, and
all derivative works of the Software, unless such copies or derivative
works are solely in the form of machine-executable object code generated by
a source language processor.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.

CRYPTOGAMS License

Copyright (c) 2006-2017, CRYPTOGAMS by <appro@openssl.org>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

* Redistributions of source code must retain copyright notices,
  this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above
  copyright notice, this list of conditions and the following
  disclaimer in the documentation and/or other materials
  provided with the distribution.
* Neither the name of the CRYPTOGAMS nor the names of its copyright
  holder and contributors may be used to endorse or promote products
  derived from this software without specific prior written permission.
  ```

</details>

