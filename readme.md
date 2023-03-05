# Android 集成多功能

该项目基于 `android-10.0.0_r17` 分支二次开发， 当前设备为 `pixel`

## 使用方法：
把 art 目录直接覆盖到 AOSP 项目中， 编译即可。

需要在 `/data/local/tmp/u33pk.conf` 写入配置文件

```conf
u33pk_pkg : <脱壳包名>
u33pk_func : <函数名>
trace_smali : <函数名>
trace_jni : <函数名>
```
### 需要注意
1. 如果要脱函数， 必须在要脱壳的app内， 即 `u33pk_func` 单独使用不生效
2. 冒号必须是英文半角， 前后各有一个空格。
3. 脱壳后的文件在 `/data/data/<pack_name>/u33pk/`, .dex文件是整体脱壳的文件 .item文件是函数脱壳后的文件
4. smali trace 后的文件在 `/data/data/<pack_name>/u33pk/smali.txt`
5. JNI调用跟踪的输出文件在 `/data/data/<pack_name>/u33pk/jni.txt`

# future
1. 函数回填
2. frida集成
3. ART 流量 FUZZ
4. 反反调
5. 或想起来的其他

# 更新内容

|日期|摘要|
----|----|
|2022.11.18| 基础脱壳功能 |
|2023.02.25| 隐藏root, 增加adb root|
|2323.03.06| JNI调用跟踪, 监控JNI动态注册, smali trace |