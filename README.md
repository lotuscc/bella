# ell网络库

主要参考muduo库：*[https://github.com/chenshuo/muduo](https://github.com/chenshuo/muduo)*

主要参考资料：《Linux高性能服务器编程》《Linux多线程服务器端编程》

一个简单Reactor架构的网络库。

本项目采用的是*epoll*来实现的多路复用，关于epoll等网络的基本知识来源于《Linux高性能服务器编程》。

Reactor基本架构主要由 `Channel`,`EventLoop`,`EPoller` 三个关键部件组成。

Channel部件主要负责一个文件描述符上的事件处理。 EPoller部件主要负责采用epoll系统调用来实现同时监听多个文件描述符上的事件，并且获得对应的事件。 EventLoop部件主要负责循环处理EPoller返回的事件。

三个主要部件的关系图如下：EventLoop包含一个EPoller以及ChannelList。

事件处理的主要流程为：在EventLoop的loop中，先通过epoll_wait等待监听事件发生，然后循环处理对应的事件