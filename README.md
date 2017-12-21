# MemoryTracer
一个简易的、跨平台的c++内存泄漏检测模块

# 它是如何工作的
MemoryTracer 使用重载全局 new 和 delete 这种侵入方式来追踪内存的创建和释放，并利用 python 脚本去记录这些操作来达到检测内存泄漏的目的。
因此，它并不适用于所有项目。

# 为什么不侵入 malloc、free
因为很多人和我们的项目不需要侵入 malloc、free，我们只关心 new 和 delete

# 为什么做这个
做这个的目的起初于我们的项目使用了mingw。而据我所知，到目前为止没有支持 mingw 并能够轻易获取的免费或开源工具或库（云风的 backtrace 放在了google，不是那么容易获取）。 有知道的，希望能分享给我：332010372@qq.com。

# 关于 example  
example 使用的开发环境为 qt mingw，因此，使用其它环境的情况下，在编译的时候需要链接相应的 python 静态库，运行时也需要加载相应的 python 动态库
