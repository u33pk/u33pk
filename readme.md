# Android 集成多功能

该项目基于 `android-10.0.0_r17` 分支二次开发， 当前设备为 `pixel`

## 使用方法：
把 art 目录直接覆盖到 AOSP 项目中， 编译即可,建议编译 `user` 版本

* 编译好了之后, 刷入手机, 需要进行相对应的功能的时候, 在 `/data/local/tmp/u33pk.conf` 写入配置文件

```conf
u33pk_pkg : <脱壳包名>
u33pk_func : <函数名>
fake_invoke : true/false
trace_smali : <函数名>
trace_jni : <函数名>
fd_pkg : <加载frida的包名>
```
## 需要注意
1. 如果要脱函数， 必须在要脱壳的app内， 即 `u33pk_func` 单独使用不生效
2. 冒号必须是英文半角， 前后各有一个空格。
3. 脱壳后的文件在 `/sdcard/Android/data/<pack_name>/u33pk/`, .dex文件是整体脱壳的文件 .item文件是函数脱壳后的文件
4. smali trace 后的文件在 `/sdcard/Android/data/<pack_name>/u33pk/smali.txt`
5. JNI调用跟踪的输出文件在 `/sdcard/Android/data/<pack_name>/u33pk/jni.txt`
6. 如果 `/sdcard/Android/data/<pack_name>` 路径没有正常创建, 请自己创建,并赋予正确的全新啊

## 修改系统证书
1. 覆盖到AOSP之后执行 `bash makeKey.sh` 不要用密码创建key,否则编译会失败
2. 把 `~/.android-certs` 下生成的文件覆盖到 `build/target/product/security` 中

## frida支持
1. 可以自己定制frida-gadget,也可以使用官方的frida-gadget, 改成"libxxx.so"(自定义的so的名字)放到 out/target/product/sailfish/system/lib(64)/libxxx.so目录下,尽量32/64两个目录都放进去
2. 修改`frameworks/base/core/java/orz/u33pk/U33pk.java`中的`libfd_name`字段的名字即可,这个地方去掉前面的"lib"和后面的".so", 中间剩下的就是name
3. 修改配置文件,添加包名, 该配置可单独生效
4. 如果frida-gadget不是定制的,读取的gadget配置文件是默认的libxxx.config.so,也需要自行在编译的时候一起放入libxxx.so的同目录下

# 未来支持
1. ART 流量 FUZZ
2. 反反调
3. 或想起来的其他

# 更新内容

|日期|摘要|
----|----|
|22.11.18| 基础脱壳功能 |
|23.02.25| 隐藏root, 增加adb root|
|23.03.06| JNI调用跟踪, 监控JNI动态注册, smali trace |
|23.06.30| 抽取的函数回填, 修改抽取函数脱壳格式 |
|24.01.01| 修改脱壳路径, 优化脱壳内容 |
|24.03.01| 修改系统证书,使用release-key |
|24.04.20| 优化函数脱壳效果 |
|24.11.11| 增加主动调用 |
|25.01.14| 优化主动调用 消除WIFI连接受阻 |
|25.01.18| 添加frida支持 |