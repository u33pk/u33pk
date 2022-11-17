# ART脱壳

该项目基于 `android-10.0.0_r17` 分支二次开发， 当前设备为 `pixel`

## 使用方法：
把 art 目录直接覆盖到 AOSP 项目中， 编译即可。

需要在 `/data/local/tmp/u33pk.conf` 写入配置文件

```conf
u33pk_pkg : <脱壳包名>
u33pk_func : <函数名>
```
### 需要注意
1. 如果要脱函数， 必须在要脱壳的app内， 即 `u33pk_func` 单独使用不生效
2. 冒号必须是英文半角， 前后各有一个空格。
3. 脱壳后的文件在 `/data/data/<pack_name>/u33pk/`, .dex文件是整体脱壳的文件 .item文件是函数脱壳后的文件

# future
1. 函数回填
2. frida集成
3. native/smali trace
4. ART 流量 FUZZ
5. 反反调
6. 或想起来的其他

## 希望不鸽