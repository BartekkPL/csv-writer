#Welcome to **Csv Writer**, your new C++ library for write to .csv file!
[![Build Status](https://travis-ci.org/BartekkPL/csv-writer.svg?branch=master)](https://travis-ci.org/BartekkPL/csv-writer)
=======================

  - [Features](#features)
  - [Platforms](#platforms)
  - [Usage](#usage)
    - [Build](#build)
  - [Projects using this library](#projexts-using-this-bibrary)
  - [License](#license)

## Important notes ##

You should know that this library is actually for my own usage and there may be not enough functionality for you.

It is library not single header file, because I plan to develop more functionality to it in future.

## Features ##

  * Logs from working provided by [**easyloging++**](https://github.com/muflihun/easyloggingpp)
  * Easy in using
  * Tested via [**Google Test**](https://github.com/google/googletest) framework

## Platforms ##

This writer has been used only on Linux, but
it probably can works on any other platform as well.

## Usage ##

The only thing what you have to do is build and link
this library!

### Build ###
```
git clone https://github.com/BartekkPL/csv-writer.git
cd csv-writer/
mkdir build
cd build/
cmake ..
make
```

And after these steps you have builded library ready to use it in new projects!

You found it at **csv-writer/build/lib/libcsvWriter.so**.

## Projects using this library ##

I use it in my engineering thesis.

## License ##

CSV-WRITER is released under [Apache License](https://opensource.org/licenses/Apache-2.0).