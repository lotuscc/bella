# Channel

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