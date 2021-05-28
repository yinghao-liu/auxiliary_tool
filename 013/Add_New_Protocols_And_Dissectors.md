# Wireshark添加自定义协议解析























Wireshark支持添加对自定义协议的解析，可以通过内置和自注册插件两种方式，两种方式各有优缺点，详见[Packet Dissection](https://www.wireshark.org/docs/wsdg_html_chunked/ChapterDissection.html)

除此之外，Wireshark还内置了Lua解释器，也可以通过Lua来配置解析自定义协议，对Lua的解析是通过加载在 [*global configuration directory*](https://www.wireshark.org/docs/wsug_html_chunked/ChAppFilesConfigurationSection.html)下的`init.lua`文件开始的。

如果Lua是开启的，Wireshark会继续从用户的[*personal configuration directory*](https://www.wireshark.org/docs/wsug_html_chunked/ChAppFilesConfigurationSection.html)加载一个`init.lua`的文件，和所有在 [*plugins directory*](https://www.wireshark.org/docs/wsug_html_chunked/ChPluginFolders.html)下的以`.lua`结尾的文件。





















参照RTP协议的格式定义，选取部分测试使用：

```
 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|V=2|P|X|  CC   |M|     PT      |       sequence number         |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
```





































## reference

- [Functions For New Protocols And Dissectors](https://www.wireshark.org/docs/wsdg_html_chunked/lua_module_Proto.html)
- [rfc1889 RTP: A Transport Protocol for Real-Time Applications](https://www.rfc-editor.org/rfc/rfc1889.html)
- [wireshark lua wiki](https://wiki.wireshark.org/Lua)
- [wireshark lua examples wiki](https://wiki.wireshark.org/Lua/Examples)
- [Lua编写wireshark插件基础](https://blog.csdn.net/chen_jianjian/article/details/81502840)
- [tvb mean](https://stackoverflow.com/questions/55510128/what-does-tvboffsetstring-mean)



