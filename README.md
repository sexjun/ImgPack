## libpng 
https://blog.csdn.net/weixin_38176039/article/details/82020865

## conan using 
```c
# 安装
pip install conan

# 版本
conan --version

# 升级
pip install conan --upgrade  
 
# 搜索包
conan search poco --remote=conan-center
conan search libpng -r=conan-center
  
# 查看  
conan inspect poco/1.9.4
  
# 配置文件名  
conanfile.txt
  
# 设置默认编译版本 
conan profile update settings.compiler.libcxx=libstdc++11 default

# 安装库
conan install .. 


# cmake使用例子
add_definitions("-std=c++11")
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()
 
 
# linux构建命令
$ cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
$ cmake --build .

# (win)
$ cmake .. -G "Visual Studio 16"
$ cmake --build . --config Release


```