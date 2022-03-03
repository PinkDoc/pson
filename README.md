# pson
一个轻量级json解析器。
## Usage-使用

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
