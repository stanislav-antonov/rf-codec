## Synopsis

This library implements a bundle of various data coding approaches well suitable for wireless data communication over RF.

It is designated to be deployed on embedded devices with relatively modest computing capabilities, in connection with that it leverages fast algorithms and requires a quite small memory footprint.

By the moment the library exposes the following functions:
* Data encoding/decoding with Hamming codes
* CCITT data whitening

## How to run tests

Navigate to the project's root directory and:

```
mkdir build
cd build
cmake -DTARGET_GROUP=test ..
make
make test
```
