# DreamSky

[TOC]

## 1. 工程说明

在学习、工作、研究的过程中，学到的、看到的一些比较常用的处理，将其封装打包成一个库，为之后的学习、工作、研究提供直接可用的工程文件。

工程主要以`cpp`为主进行开发，将提供`vs sln`、`xmake`、`cmake`的构建脚本。

## 2. 构建工具

### 2.1 vs sln

严格来说，并不能称之为`vs sln`，但是这种构建方式也只能通过`Visual Studio`进行，并且给出了解决方案文件，就暂且这么称呼吧。

需要`vs 2017`及以上的版本才可进行处理，如果打开后显示不对应版本，请自行手动调节一下，工程中都是在`vs 2022`下进行开发了，没精力去适配那么多。

> **注意：**
>
> 工程中只提供了`x64 debug`的构建方式的配置，如果有其他平台和模式的需求，需要自行参考然后增加即可。

### 2.2 xmake

工程提供了`xmake.lua`脚本，支持[`xmake`](https://xmake.io/#/zh-cn/)的构建方式。

使用`xmake`构建的步骤如下：

`Windows`:

Windows下暂时没有进行`mingw`的构建方式的测试，只进行了`windows`的构建方式的测试，后续可能有精力进行完善。

```shell
xmake f -p windows -k shared  # 可以更改 kind，这个看需求
xmake -v
```

`Linux`:

```shell
xmake f -k shared  # 可以更改 kind，这个看需求
xmake -v
```

`xmake package`:

如果需要将编译后的文件进行打包，可以使用：

```shell
xmake package -o "dest dir"
```

### 2.3 cmake

暂时还没有精力进行完善，后续看计划。

## 3. 库说明

### 3.1 DreamCommon

```shell
DreamCommon
├── include
│   └── DreamSky
│       ├── dream_define.h
│       ├── dream_error.h
│       ├── dream_mem.h
│       ├── dream_print.h
│       ├── dream_string.h
│       ├── dream_time.h
│       └── string
│           └── dstring.h
├── README.md
├── src
│   ├── dream_error.cpp
│   ├── dream_mem.cpp
│   ├── dream_print.cpp
│   ├── dream_string.cpp
│   ├── dream_time.cpp
│   └── string
│       └── dstring.cpp
├── vcproj
│   ├── DreamCommon.vcxproj
│   ├── DreamCommon.vcxproj.filters
│   └── postbuild.bat
└── xmake.lua
```

一些常用的基础`API`，这里还写了一个并不完善的`dstring`，这个需要看后续需求一步步完善，力求实现一个高性能且易用的`string`。

> 吐槽一下：
>
> 如果不是Windows下vs的`STL`容器的导出问题，我也不想费这么大力气去重新搞一套string，虽然C++好用，但是还是顶不住这个`ABI`的妖精啊！！！

### 3.2 DreamConfig

```shell
DreamConfig/
├── include
│   └── DreamSky
│       ├── dream_ini.h
│       └── dream_json.h
├── README.md
├── src
│   ├── dream_ini.cpp
│   └── dream_json.cpp
├── vcproj
│   ├── DreamConfig.vcxproj
│   ├── DreamConfig.vcxproj.filters
│   └── postbuild.bat
└── xmake.lua
```

这个是想实现一个简易的`ini`配置文件的处理，以及在`rapidjson`的基础上进行`json`处理的封装，后续还需要加上`xml`等其他的封装。

### 3.3 DreamLog

```shell
DreamLog/
├── CMakeLists.txt
├── include
│   └── DreamSky
│       └── log_core.h
├── README.md
├── src
│   ├── log_core.cpp
│   ├── log_macro.h
│   ├── log_printf.cpp
│   └── log_printf.h
├── vcproj
│   ├── DreamLog.vcxproj
│   ├── DreamLog.vcxproj.filters
│   └── postbuild.bat
└── xmake.lua
```

这个主要是参考王健伟老师CPP课程代码进行设计的，小巧且加了时间输出，但是性能有限，后续会考虑参考[xmake作者的开源库](https://github.com/idealvin/coost)进行优化处理。

### 3.4 DreamProcess

```shell
DreamProcess/
├── doc
│   └── 函数说明.md
├── include
│   └── DreamSky
│       ├── dream_process.h
│       └── pipe
│           └── pipe_operation.h
├── src
│   ├── dream_process.cpp
│   └── pipe
│       └── pipe_operation.cpp
├── vcproj
│   ├── DreamProcess.vcxproj
│   ├── DreamProcess.vcxproj.filters
│   └── postbuild.bat
└── xmake.lua
```

主要是对一些进程处理封装，目前偏向于`Windows`下的`WIN32 API`的封装。

### 3.5 DreamSocket

```shell
DreamSocket/
├── include
│   └── DreamSky
│       ├── dream_socket.h
│       ├── win
│       │   ├── win_epoll.h
│       │   └── win_socket.h
│       └── x
│           └── x_socket.h
├── src
│   ├── dream_socket.cpp
│   └── win
│       ├── win_epoll.c
│       └── win_socket.cpp
├── vcproj
│   ├── DreamSocket.vcxproj
│   ├── DreamSocket.vcxproj.filters
│   └── postbuild.bat
└── xmake.lua
```

对于一些`socket`处理的封装，并且集成了[`wepoll`](https://github.com/piscisaureus/wepoll)的处理，从而可以实现`Windows`下的`epoll`处理，目前封装较浅，没有复杂的处理，后续考虑直接封装成开箱即用的综合`socket`库。

### 3.6 DreamThread

```shell
DreamThread/
├── doc
│   └── 函数说明.md
├── include
│   └── DreamSky
│       ├── dream_thread.h
│       ├── pthread
│       │   ├── p_mutex.h
│       │   ├── p_semaphore.h
│       │   └── p_thread.h
│       ├── std_thread
│       │   └── std_thread.h
│       └── win_thread
│           ├── win_mutex.h
│           ├── win_semaphore.h
│           └── win_thread.h
├── src
│   ├── dream_thread.cpp
│   ├── pthread
│   │   ├── p_mutex.cpp
│   │   ├── p_semaphore.cpp
│   │   └── p_thread.cpp
│   ├── std_thread
│   │   └── std_thread.cpp
│   └── win_thread
│       ├── win_mutex.cpp
│       ├── win_semaphore.cpp
│       └── win_thread.cpp
├── vcproj
│   ├── DreamThread.vcxproj
│   ├── DreamThread.vcxproj.filters
│   └── postbuild.bat
└── xmake.lua
```

对一些thread的处理的封装，可以根据需要选择`pthread`、`std thread`、`WIN32 thread`的处理，目前`std thread`部分暂时没有封装处理，后续根据工程实现补足。
