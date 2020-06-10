# AkoFS


## 依赖

- C11 & C++11
- cmake-3.10.x
- zlib-1.x.x（libz）
- openssl-1.x.x（libcrypto / libssl）
- pthread-2.x.x（libpthread）
- jerasure-2.0
- gtest-1.x.x（可选）

## 单元测试

#### Linux

- 在 `test/` 目录下添加 `.hpp` 源码，补充单元测试用例
- 在 `test/main.cpp` 中添加测试单元 `.hpp` 头文件
- `cd test/` ，执行 `cmake3 ./` 生成 Makefile 文件
- 执行 `make` 编译测试用例
- 执行 `./akofs-test` 运行测试用例

#### MSVC 

- 在 `test/` 目录下添加 `.hpp` 源码，补充单元测试用例
- 在 `test/main.cpp` 中添加测试单元 `.hpp` 头文件
- 执行编译调试

## 软件构建

#### Linux

- `cd src/` ，执行 `cmake3 ./` 生成 Makefile 文件
- `make` 编译原型系统 akofs 和控制台交互程序 akocli
- 执行 `./akofs` 运行原型系统，执行 `nohup ./akofs &` 将软件以守护进程的方式启动
- 执行 `./akocli --help` 查看控制台交互程序的使用帮助

#### MSVC

- 将启动项目设置为 `akofs`
- 执行编译生成 `akofs` 程序
- 将启动项目设置为 `akocli`
- 执行编译生成 `akocli` 程序
