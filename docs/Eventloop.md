# EventLoop

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