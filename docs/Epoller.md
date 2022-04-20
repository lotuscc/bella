# EPoller

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