# 1 概述
异步多线程编程中，我们可以将后台进一步抽象为更大粒度的一些组件。这些组件对进程内可分为消息队列，线程(池)，对外可分为连接池、会话管理器、tcp服务器/客户端、udp服务器/客户端
## 2.1 线程(池)
* ThreadMutex/ThreadMutexGuard,互斥锁，对pthread_mutex_t进一步封装
* ThreadRWMutex，读写互斥锁，粒度比互斥锁小一些
* ThreadCond, 条件变量，对pthread_cond_t的进一步封装
* Thread，线程对象，也可以用来创建线程池
* ThreadMeet, 用来同步多个线程。常用语线程同步编程 。

## 2.2 消息队列
异步编程中，我们经常需要队列来存放消息，cppbrick库封装了四种容器：TVector,TQueue, TMapInt,TMapString。这四种容器都是线程安全的，可以用来做队列，作为线程间传递消息的管道。
TVector,TQueue具有限速和限容功能。
这四种容器特点如下：
* TVector, 本质上是std::vector<T>
* TQueue, 本质上是std::queue<T>
* TMapInt,本质上是map<int, T>
* TMapString,本质上是map<string, T>

## 2.3 网络组件
* UdpClient类，用来创建一个udp 客户端。
* TcpReactor, 用来创建基于tcp的reactor，用户必须要基于EventHandler实现自己的连接处理函数。主要是handle_accept/handle_input/handle_close接口。
* UdpReactor, 用来创建一个基于udp的reactor，用户必须要基于EventHandler实现自己的连接处理函数。主要是handle_input/handle_close接口。
* EventHandler, Reactor处理事件的基类。 
* SimpleTcpClient, 用来创建简单的同步tcp client或者tcp server,便于快速写测试小例子。
* ConnMgtLB, 一个连接池，用户需要自己实现自己的tcp连接客户端。同目录下实现了一个基于epoll的异步客户端。
* TcpClientEpoll, 一个基于epoll的tcp 客户端，有同步和异步两种模式。

## 2.4 其它一些基本的组件封装
* StringUtils类：主要封装了标准库没有trim函数和split函数和to_string函数
* TimeStamp类，用来获取当前时间，最小精度us。类对+、-、=、<、>运算符进行类重载。
* StdTypes.h, 对基本类型unsigned char/short/int/long and long long的重新命名，使得代码更加美观
* min/max method
* XCrypto,用来进行des/aes/base64等对称加密和非对称加密；XDigest则用来生成MD5/SHA1/SHA256/HMAC_SHA1等各种摘要
* Base64则是一个单独实现的类，用来提供base64的加解密算法
* TSingleton, 单例模板类，可以方便快速的定义单例
* XMLParser类，用来解析xml文件，基于tinyxml库。不支持紧凑的写法(<key field1="a", field2="b"/>),要使用节凑的写法，注意写成数组模式。注意，数字也必须用双引号括起来。
* SelectTimer, 基于select实现的单线程定时器
* CBLog, 日志接口，实现了DEBUG/WARN/ERROR三种级别的日志

TODO:
* mysql wrapper
* redis wrapper
* mongo wrapper
* session manager
* uid_svr
* cache_proxy_svr
* access_svr


