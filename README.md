# qClash

A rule based proxy For Linux/Win/Mac base on [Clash](https://github.com/Dreamacro/clash).

## Features

- HTTP/HTTPS and SOCKS protocol
- Surge like configuration
- GeoIP rule support
- Support Vmess/Shadowsocks/Trojan/Socks5
- Support for Netfilter TCP redirect

## Install

You can download from [release](https://github.com/josephpei/qClash/releases) page

You also need `clash` in your path.

Basic usage just like `ClashX` on Mac.

## Build

You need place `Country.mmdb` in the `config` directory. I don't inlucde this file in the source tree.

```
# Install Qt libraries use your package manager.
# Or use the official qt installer, then set the environment
export QT_ROOT=$HOME/Misc/tools/Qt5.14.2/5.14.2/gcc_64
```

# Thanks

* [Clash](https://github.com/Dreamacro/clash)
* [V2ray-Desktop](https://github.com/Dr-Incognito/V2Ray-Desktop)
* [ClashX](https://github.com/yichengchen/clashX)
* [mmdb_china_ip_list](https://github.com/alecthw/mmdb_china_ip_list)