# Event Vision Library (EVL)
[![CircleCI](https://circleci.com/gh/EventVisionLibrary/evl/tree/develop.svg?style=svg&circle-token=88a46ce106424172fa3e461953f29795b6222771)](https://circleci.com/gh/EventVisionLibrary/evl/tree/develop)

Open Source Computer Vision Library for Event data.

## Supported devices

* DAVIS
* DVS128

And more...

## Installation

### Supported OS

* Mac
* Linux

We are now preparing for Windows OS support. Stay tuned, or contribute!

### Requirements

* C++11
* CMake >= 3.1
* `OpenCV`: `brew install opencv` or `apt-get install libopencv-dev` [Link](https://opencv.org/)
* `Boost >= 1.65`: `brew install boost` or `apt-get install libboost-all-dev` [Link](https://www.boost.org/)
* `Eigen3`: `brew install eigen` or `apt-get install libeigen3-dev` [Link](http://eigen.tuxfamily.org/index.php?title=Main_Page)
* `libcaer >= 2`: `brew install libcaer` or `bash install_dependencies.sh` [Link](https://github.com/inilabs/libcaer)

### Build

```
$ git clone https://github.com/EventVisionLibrary/evl.git
$ cd evl
$ cd examples
$ mkdir build
$ cd build
$ cmake ..
$ make
```

## Resources

* Example: <https://github.com/EventVisionLibrary/evl/blob/master/src/test>
* Issue tracking: <https://github.com/EventVisionLibrary/evl/issues>

## Contributing

Please read the [contribution guidelines] before starting work on a pull request.

### Summary of the guidelines:

* One pull request per issue;
* Choose the right base branch;
* Include tests and documentation;
* Clean up "oops" commits before submitting;
* Follow the [coding style guide]().


## License

BSD 3-clause License. See [detail](https://github.com/EventVisionLibrary/evl/blob/master/LICENSE).

2018, Event Vision Library.
