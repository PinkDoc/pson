### pson
一个轻量级json解析器。

#### 关于
我实现了两个版本的value， 
第一个版本(src)是用模板元求类型(Object, Array, String...)最大值，
然后使用一段内存(char buf[union_size])来表示，
这个版本的优势是类型转换的时候速度更快。

第二个版本则是使用
```
union holder {
  Number number,
  ...,
  String* string,
  Array* array_,
  ...
}
```
这种`new`标准库的做法,所占空间更小，但是很明显，这种做法速度不快，
而且每次切换一个类型，就要进行一个`delete -> new`的操作，这种做法坏处很多，更容易产生内存碎片，速度慢(相对于做法1)。
#### 功能
* json文本生成器
* json文本解析器
#### 使用

```
std::string data = "{\n"
                   "    \n"
                   "    \"version\": \"0.2.0\",\n"
                   "    \"configurations\": [\n"
                   "        {\n"
                   "            \"name\": \"g++ -o\",\n"
                   "            \"type\": \"main\uffffg\",\n"
                   "            \"request\": \"launch\",\n"
                   "            \"program\": \"${fileDirname}/${fileBasenameNoExtension}\",\n"
                   "            \"args\": [],\n"
                   "            \"stopAtEntry\": false,\n"
                   "            \"cwd\": \"${fileDirname}\",\n"
                   "            \"environment\": [],\n"
                   "            \"externalConsole\": false,\n"
                   "            \"MIMode\": \"gdb\",\n"
                   "            \"setupCommands\": [\n"
                   "                {\n"
                   "                    \"description\": \"zxczxc\",\n"
                   "                    \"text\": \"-enable-pretty-printing\",\n"
                   "                    \"ignoreFailures\": true\n"
                   "                },\n"
                   "                {\n"
                   "                    \"description\": \"zxczxczxcIntel\",\n"
                   "                    \"text\": \"-gdb-set disassembly-flavor intel\",\n"
                   "                    \"ignoreFailures\": true\n"
                   "                }\n"
                   "            ],\n"
                   "            \"preLaunchTask\": \"C/C++: g+zxczxc\",\n"
                   "            \"miDebuggerPath\": \"/usr/bin/gdb\"\n"
                   "        }\n"
                   "    ]\n"
                   "}";


```


```
    pson::Value     val;
    pson::Parser    parser(data);
    
    parser.Parse(val);
    
    auto& obj = val.AsObject();
    assert(obj.GetAsString("version") == "0.2.0");
    assert(val.IsObject());
```
