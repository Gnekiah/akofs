# sparkle

## 依赖

- C11 / C++11
- cmake-3.10.x
- zlib-1.x.x（libz）
- openssl-1.x.x（libcrypto / libssl）
- pthread-2.x.x（libpthread）
- gtest-1.x.x（可选）

## 单元测试

#### Linux 平台

- 在 `test/` 目录下添加 `.hpp` 源码，补充单元测试用例
- 在 `test/main.cpp` 中添加测试单元 `.hpp` 头文件
- `cd test/` ，执行 `cmake3 ./` 生成 Makefile 文件
- 执行 `make` 编译测试用例
- 执行 `./bin/sparkle-test` 运行测试用例

#### MSVC 

- 在 `test/` 目录下添加 `.hpp` 源码，补充单元测试用例
- 在 `test/main.cpp` 中添加测试单元 `.hpp` 头文件
- 执行编译调试