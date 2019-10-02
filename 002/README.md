# 002 
log system which is thread-safe

## feature
* 线程安全
* 日志文件最大大小可配，若超过会截短一半
* 可打印至文件或终端

## description
### 日志格式
时间 日志级别 文件:行数(函数名): 日志内容  
例如：  
03-14 22:55:52 [FATAL] main.cpp:33(thread2): ++++++++++++++++++++0

### 初始化
```cpp
int log_init(log_level_t log_level=DEFAULT_LOG_LEVEL, const char *log_file=NULL, uint16_t max_mb=10);
```
1. 可不初始化，此情况与`log_init()`等效。`log_file=NULL`则将日志打印至终端
2. `log_level`指定打印等级，有FATAL、ERROR、WARNING、INFO、DEBUG五级，详细程度依次递增
3. `max_mb`指定最大文件大小，单位：MB

### 日志打印
```cpp
log(level, s, ...);
```
1. 这是一个宏，`level`指定日志级别，`s`指定字串格式，与`printf`的`format`类似，但不可以是指针变量，只能是一串常字串，比如: "abc%s%c"  
2. A value of `level` that is negative or is larger than DEBUG will result in undefined behavior
