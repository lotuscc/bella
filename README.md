# ell网络库

主要参考muduo库：*[https://github.com/chenshuo/muduo](https://github.com/chenshuo/muduo)*

主要参考资料：《Linux高性能服务器编程》《Linux多线程服务器端编程》 《C++并发编程实战》

一个简单Reactor架构的网络库。

本项目采用的是*epoll*来实现的多路复用，关于epoll等网络的基本知识来源于《Linux高性能服务器编程》。

Reactor基本架构主要由 `Channel`,`EventLoop`,`EPoller` 三个关键部件组成。

Channel部件主要负责一个文件描述符上的事件处理。 EPoller部件主要负责采用epoll系统调用来实现同时监听多个文件描述符上的事件，并且获得对应的事件。 EventLoop部件主要负责循环处理EPoller返回的事件。

三个主要部件的关系图如下：EventLoop包含一个EPoller以及ChannelList。

事件处理的主要流程为：在EventLoop的loop中，先通过epoll_wait等待监听事件发生，然后循环处理对应的事件

*[简单文档](https://lotuscc.gitbook.io/ell-wang-luo-ku/)*


# Channel 简要介绍

Linux系统上有一个非常重要的概念为：一切皆文件。

对于网络编程范围来说，`sockfd`就是一个文件描述符，每个文件描述符都是系统资源。

对于文件描述符类资源来说，一共有四类事件，可读事件，可写事件，关闭事件，错误事件。
当一个文件描述符可写时，产生可写事件。
当一个文件描述符可读时，产生可读事件。
当一个文件描述符关闭时，产生关闭事件。
当一个文件描述符上发生错误时，产生错误事件。

Channel不拥有fd的所有权，只负责观察fd上的一系列事件。

Channel相当与挂在fd上的一条观察通道，从某一个角度观察fd上的事件，当对应的事件发生的时候就采取对应的处理。
一个fd可以拥有多个Channel。
例如，一个Channel负责观察fd上的写事件，一个Channel 负责观察fd上的读事件，等等

`Channel`类中主要的数据成员有以下几个：

```cpp
const int fd_;
int events_;
int revents_;

EventCallBack closeCallBack_;
EventCallBack errorCallBack_;
EventCallBack writeCallBack_;
EventCallBack readCallBack_;
```

`fd` 的类型为`const`，意味着创建Channel的时候必须传入一个fd，一个不负责任何fd的Channel是没有意义的！

`events` 代表Channel所负责所关心的事件标志集合，我们想要关心fd上的哪些事件，就可以在events中设置对应的标志位。
因此，下面的函数成员中有对应的方法可以修改events的值。
例如，我们只关心fd上的可读事件时，就只需要设置可读事件的标志，当发生其他事件时就会被忽略。
并且此events与`epoll`中保持一致，所有epoll可以直接使用，而不需要再进行一次转换。

`revents` 代表fd上已发生的事件标志集合，revents的值不是由我们修改的，而是由内核修改的。
因此函数成员中没有方法给用户使用，只要一个方法给epoll使用。
当我们采用epoll_wait等待事件发生时，如果有关心的事件发生，内核就会修改revents的值并返回给我们，
我们根据revents的值就能知道fd上发生了哪些事件，然后采取对应处理。

`closeCallBack`，`errorCallBack`，`writeCallBack`，`readCallBack`：
四个事件处理函数，由于我们采用的是基于函数回调的架构，因此这四个事件处理函数也被抽象成了数据成员，
类型为`std::function`函数类型，并且因为发生哪些事件而采用哪些事件处理方法是由我们决定，因此下面的函数成员中有对应的方法可以修改其值。

说完，Channel类中主要的数据成员，接下来分析Channel类中主要的函数成员。
有了上面的数据成员的基础，下面的函数成员就比较好分析，因为函数成员基本上都是围绕着数据成员而设置，都是数据成员的一些增删改查方法。
Channel类中主要的函数成员有以下几个：

```cpp
void handleEvent();

int fd() const;
int events() const;

// used by pollers
void set_revents(int revt);

void enableReading();
void disableReading();

void enableWriting();
void disableWriting();

void set_closeCallBack(EventCallBack callback);
void set_errorCallBack(EventCallBack callback);
void set_readCallBack(EventCallBack callback);
void set_writeCallBack(EventCallBack callback);
```

其中大部分函数成员通过名字都能知晓含义，其中需要说明的主要是`handleEvent`函数，
其中主要源码如下，可以看到，主要过程就是通过`revents`来判断发生了哪些事件，并且采取对应的事件处理。

```cpp
// 根据 revents_ 处理对应事件
void Channel::handleEvent() {
    if ((revents_ & POLLHUP) && !(revents_ & POLLIN)) {
        if (closeCallBack_) {
            closeCallBack_();
        }
    }
    if (revents_ & (POLLERR | POLLNVAL)) {
        if (errorCallBack_) {
            errorCallBack_();
        }
    }
    if (revents_ & (POLLIN | POLLPRI | POLLRDHUP)) {
        if (readCallBack_) {
            readCallBack_();
        }
    }
    if (revents_ & POLLOUT) {
        if (writeCallBack_) {
            writeCallBack_();
        }
    }
}
```


# EPoller 简要介绍

`EPoller`部件主要负责采用`epoll`系统调用来监听各类事件。
因此，EPoller类主要封装了与epoll系统调用相关的一些操作以及需要的数据。

`EPoller`类的数据成员主要如下：

```cpp
using EventList = std::vector<struct epoll_event>;
using ChannelMap = std::map<int, Channel *>;

int epollfd_;
EventList events_;
ChannelMap listenChannels_;
```

`epollfd`为该epoll的fd，其值在EPoller的构造函数中由`epoll_create`产生。

`events`的类型为`std::vector<struct epoll_event>`，是epoll_event的容器类，当使用`epoll_wait`系统调用时，需要把它传入内核，
内核会根据发生的事件将其填充，我们再进行查看就可以知道发生了哪些事件。

`listenChannels`的类型为`std::map<int, Channel *>`，根据fd保存了相应Channel，这里根据fd来保存会存在一个问题
即同一个fd只能设置一个Channel,在muduo中不是根据fd来保存的，这里只是为了简单考虑，而采用后面会修改这个问题。

EPoller类的函数成员主要如下：

```cpp
void poll(int timeoutMs, ChannelList *activeChannels);
void fillActiveChannels(int numEvents, ChannelList *activeChannels);

void append_listenChannel(Channel *listenChannel);
void remove_listenChannel(Channel *listenChannel);
```

其中暴露给外部使用的有`poll`，`append_listenChannel`，`remove_listenChannel`。

`poll`为整个`EPoller`类的核心函数，其中采用了`epoll_wait`系统调用来监听各类事件，并且采用`fillActiveChannels`函数
将事件复制出去供别人使用。

其主要代码如下：

```cpp
// 阻塞，获取事件
int numEvents = ::epoll_wait(epollfd_, &*events_.begin(),
                             static_cast<int>(events_.size()), timeoutMs);
//填充激活事件到activeChannels列表中
fillActiveChannels(numEvents, activeChannels);
```

`append_listenChannel`，`remove_listenChannel`函数主要负责维护EPoller监听的各类事件，
如果想要通过EPoller监听一个事件就通过append_listenChannel加入到监听列表中。


# EventLoop 简要介绍

`EventLoop`部件主要负责事件的循环处理。
EventLoop类主要数据成员主要有：

```cpp
ChannelList activeChannels_;
std::unique_ptr<EPoller> poller_;
```

`activeChannels_` 的类型为ChannelList，主要负责获取`epoll`中内核返回的已发生事件列表。
`poller_`，每个EventLoop都有一个,负责通过epoll监听事件。

EventLoop类主要函数成员主要有：

```cpp
void loop();
void append_channel(Channel *channel);
```

其中`append_channel`主要负责向poller中添加需要监听的事件。
`loop`函数为主要的处理函数，主要过程就是循环监听事件然后处理事件。
其主要代码如下：

```cpp
void EventLoop::loop() {
    while (true) {
        activeChannels_.clear();

        // 阻塞
        poller_->poll(-1, &activeChannels_);

        for (ChannelList::iterator it = activeChannels_.begin();
             it != activeChannels_.end(); ++it) {
            (*it)->handleEvent();
        }
    }
}
```