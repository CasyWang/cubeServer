cubeServer
==========

A distribute open source hardware testing server. 

### Features

* 1. Automatically download *.bin *.yaml from FTP server.
* 2. Asys communication Mode in both serial and socket.
* 3. mDNS service advertiser/discovery.
* 4. Auto parse 3-rd party tool's output log.
* 5. Supports multiple client connection.
* 6. Supports most of the open source hardware main board, such as arduino,seeeduino.

#### Architecture
![Create new testing solution](https://raw.githubusercontent.com/CasyWang/cubeServer/dev/doc/cubeTest4.jpg)

### Dependency 

* 1. Boost
* 2. Curl

### How to Make

At present, the cubeServer is developed by CodeBlocks and MinGW.

### About cube Project

This project consists of two main parts.
* (1). cubeServer : the controller of testing 
* (2). cubeTester : the view of testing. 

### cubeTester UI:

![Automatically detect main board](https://raw.githubusercontent.com/CasyWang/cubeServer/f5298411e6425828cae643d334fe9b902aa5889d/doc/cubeTest1.jpg)

![Testing](https://raw.githubusercontent.com/CasyWang/cubeServer/f5298411e6425828cae643d334fe9b902aa5889d/doc/cubeTest2.jpg)

![Create new testing solution](https://raw.githubusercontent.com/CasyWang/cubeServer/f5298411e6425828cae643d334fe9b902aa5889d/doc/cubeTest3.jpg)

### Author

* Oliver <lxtech2013@gmail.com>

Any kinds of contribution from the community is warmly welcomed.