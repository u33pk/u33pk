# Android 集成多功能

该项目基于 `android-10.0.0_r17` 分支二次开发， 当前设备为 `pixel`

## 使用方法：
把 art 目录直接覆盖到 AOSP 项目中， 编译即可,建议编译 `user` 版本

* 编译好了之后, 刷入手机, 需要进行相对应的功能的时候, 在 `/data/local/tmp/u33pk.conf` 写入配置文件

```conf
u33pk_pkg : <脱壳包名>
u33pk_func : <函数名>
trace_smali : <函数名>
trace_jni : <函数名>
```
## 需要注意
1. 如果要脱函数， 必须在要脱壳的app内， 即 `u33pk_func` 单独使用不生效
2. 冒号必须是英文半角， 前后各有一个空格。
3. 脱壳后的文件在 `/sdcard/Android/data/<pack_name>/u33pk/`, .dex文件是整体脱壳的文件 .item文件是函数脱壳后的文件
4. smali trace 后的文件在 `/sdcard/Android/data/<pack_name>/u33pk/smali.txt`
5. JNI调用跟踪的输出文件在 `/sdcard/Android/data/<pack_name>/u33pk/jni.txt`
6. 如果 `/sdcard/Android/data/<pack_name>` 路径没有正常创建, 请自己创建,并赋予正确的全新啊

## 修改系统证书
1. 覆盖到AOSP之后执行 `bash makeKey.sh`
2. 把 `~/.android-certs` 下生成的文件覆盖到 `build/target/product/security` 中


# 未来支持
1. frida集成
2. ART 流量 FUZZ
3. 反反调
4. 或想起来的其他

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