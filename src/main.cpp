#include <iostream>
#include <string>
#include <spk_compat.h>
#include <spk_setproctitle.h>

static void usage() 
{
    std::cerr << "usage: sparkle -i "
        << ""
        << std::endl;
}

int main(int argc, const char **argv) 
{
    spk_setproctitle(argv, "sparkle");
    spk_pthread_setname(pthread_self(), "sparkle");
    int err = 0;

    // 解析命令行参数
    // 加载并解析配置文件
    // 初始化日志记录器
    // 初始化功能模块
    // 初始化事件驱动模型
    // 初始化定时器
    // 初始化网络IO
    // 初始化消息协议模块
    // 初始化KV持久化存储
    // 初始化事务日志
    // 初始化数据节点服务（向虚拟节点管理服务中注册事件处理回调函数，并向定时器注册定时事件）
    // 初始化超级节点服务（ ... ）
    // 初始化访问节点服务（ ... ）
    // 从节点文件尝试恢复节点（数据节点和/或超级节点）
    // 启动消息循环

    return 0;
}