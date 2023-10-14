本篇文章仅供交流与学习使用，请勿作出任何违反国家法律的行为。

CentOS起手式

```shell
sudo yum update
sudo yum install epel-release
```

# 1.ShadowSocks

## 下载一键部署脚本

使用github上的一键部署脚本(下载需要的即可)

[teddysun/shadowsocks_install: Auto Install Shadowsocks Server for CentOS/Debian/Ubuntu (github.com)](https://github.com/teddysun/shadowsocks_install)

## 部署运行

比如我下载了`shadowsocksR.sh`

为其赋予权限:
`chmod 777 shadowsocksR.sh`

运行脚本:
`./shadowsocksR.sh`

之后会让你设置一些配置，**端口号**和**密码**要配置好，后面的不知道的话默认（按回车）即可。

成功之后就可以在其他客户端上连接了。

## 修改配置

如果需要修改配置，再运行一次部署脚本即可
`./shadowsocksR.sh`

## 卸载

如果需要卸载，在运行的脚本后面加`uninstall`作为参数
`./shadowsocksR.sh uninstall`



## SS/SSR节点转换成Clash配置

因为平时习惯用clash，所以再用一个shadowsocks有点多了，可以通过以下转换网站将ssr节点转成clash配置文件：

[在线订阅转换网站分享](https://clashios.com/unblocked-subconvert-websites/)

或者直接参考我的配置文件：

[ssr-clash-sample.yml (github.com)](https://github.com/qissqi/var-bin/blob/master/梯子/ssr-clash-sample.yml)



## 参考：

[云服务器搭建SSR教程｜LiuChenZe的Blog](https://liuchenze.github.io/2018/09/19/云服务器搭建SSR教程/)

如果需要配合bbr加速：[sucong426/ssr-bbr-vpn: 小白VPN搭建图文教程/开启加速bbr/搭建教程/ (github.com)](https://github.com/sucong426/ssr-bbr-vpn)



# 2.使用OpenVPN

因为吃瘪了，所以不推荐

## 服务端

### 准备

需要使用的有：OpenVpn、easy-rsa（管理生成证书）、iptables（类似防火墙，设置流量转发）

```shell
sudo yum install openvpn
sudo yum install easy-rsa	
sudo yum install iptables-services
```


### 证书生成

下载的easy-rsa 默认位于 /usr/share/easy-rsa/3.0.8/ 中，如果版本更新可自行查找相关位置

进入文件夹中

```
cd /usr/share/easy-rsa/3.0.8
```

初始化证书

```
./easyrsa init-pki
```

生成ca证书（无密钥）

```
./easyrsa build-ca nopass
```

创建server端证书和私钥文件

```
./easyrsa gen-req server nopass
```

给server端证书签名，记得后面要输入yes，如果ca证书有密钥要输入密码

```
./easyrsa sign server server
```

创建密钥交换算法dh文件

```
./easyrsa gen-dh
```

创建tls认证密钥

```
openvpn --genkey --secret ta.key
```

为了方便，顺便也把客户端需要的证书也一起生成了

```
./easyrsa gen-req client nopass
```

为证书签名（记得输yes）

```
./easyrsa sign client client
```



把需要的文件复制到方便的位置

```
cp ./pki/ca.crt /root/
 
cp ./pki/dh.pem /root/
 
cp ./pki/issued/server.crt /root/
 
cp ./pki/private/server.key /root/
 
cp ta.key /root/

cp ./pki/issued/client1.crt /root/
 
cp ./pki/private/client1.key /root/

```



### 配置server文件

进入openvpn配置文件夹

```
cd /etc/openvpn/
```

编辑配置文件

```
vi server.conf
```

```
# 监听本机ip地址 (这里填本机地址 你服务器的ip)
local xxx.xxx.xxx.xxx
 
# 监控本机端口号
port 1194 #你运行的端口
 
# 指定采用的传输协议，可以选择tcp或udp
proto tcp
 
dev tun # 指定创建的通信隧道类型，可选tun或tap,window服务器必须是tap(tun支持手机上使用)
# 指定CA证书的文件路径
ca /root/ca.crt
 
# 指定服务器端的证书文件路径
cert /root/server.crt
 
# 指定服务器端的私钥文件路径
key /root/server.key 
 
# 指定迪菲赫尔曼参数的文件路径
dh /root/dh.pem
 
# 指定虚拟局域网占用的IP地址段和子网掩码，不能和服务器eth0同网段
server 10.8.0.0 255.255.255.0
 
# 服务器自动给客户端分配IP后，客户端下次连接时，仍然采用上次的IP地址(第一次 分配的IP保存在ipp.txt中，下一次分配其中保存的IP)。
ifconfig-pool-persist ipp.txt
 
# 自动推送客户端上的网关及DHCP,此项开启了流量转发,有这项才能使用服务器代理上网
push "redirect-gateway def1 bypass-dhcp"
 
# OpenVPN的DHCP功能为客户端提供指定的 DNS、WINS等
push "dhcp-option DNS 8.8.8.8"
 
# 备用DNS
push "dhcp-option DNS 8.8.4.4"
 
# 允许客户端与客户端相连接，默认情况下客户端只能与服务器相连接
client-to-client
 
# 允许同一个客户端证书多次登录,看需配置
duplicate-cn
 
# 心跳包，10秒一次，120秒无响应掉线
keepalive 10 120
 
# 开启TLS-auth，使用ta.key防御攻击。服务器端的第二个参数值为0，客户端的为1。
tls-auth /root/ta.key 0
 
# 加密认证算法,2.4之前是AES-256-CBC
cipher AES-256-GCM
 
# 使用lzo压缩的通讯,服务端和客户端都必须配置
comp-lzo
 
# 最大连接用户
max-clients 100
 
# 定义运行的用户和组,openvpn用户是安装的时候系统自动创建的，好像新版本的openvpn不需要这两行
user openvpn
group openvpn
 
# 重启时仍保留一些状态
persist-key
persist-tun
 
# 输出短日志,每分钟刷新一次,以显示当前的客户端
status /var/log/openvpn-status.log
 
# 日志保存路径
log /etc/openvpn/log/openvpn.log
log-append /etc/openvpn/log/openvpn.log
 
# 指定日志文件的记录详细级别，可选0-9，等级越高日志内容越详细 3为普通运行等级 5-6是调试等级（如果你发现无法连接等问题可开启）
verb 3
 
#相同信息的数量，如果连续出现 20 条相同的信息，将不记录到日志中
mute 20
 
# 下面这项只能udp连接开启
 #explicit-exit-notify 1
 
# n秒后重新协商data chan. key (默认值=3600) 这里我设置为0意思是不协商 以防经常断连
reneg-sec 0

```



### 配置代理转发

允许转发：

```
echo net.ipv4.ip_forward=1 >> /etc/sysctl.conf
```

使配置生效

```
sysctl -p
```

关闭firewalld防火墙

```
systemctl stop firewalld
systemctl disable firewalld
```

启动iptables

```
systemctl enable iptables
systemctl start iptables
```

配置iptables转发流量

```
iptables -t nat -A POSTROUTING -s 10.8.0.0/24 -o eth0 -j MASQUERADE
```

允许tcp/udp 通过防火墙（你用的什么协议就只写哪一条 写多了可能导致转发失败，1194是你开放的openvpn端口）

```
iptables -I INPUT -p tcp --dport 1194 -j ACCEPT #tcp

iptables -I INPUT -p udp --dport 1194 -j ACCEPT #udp
```

保存规则并重启

```
service iptables save
systemctl restart iptables
```



### 配置启动服务文件

看看是否有该配置文件，如果有了就可以不管了（应该吧）

```
vim /lib/systemd/system/openvpn@.service
```

如果没有这个文件，则复制以下内容

```
[Unit]
 
Description=OpenVPN Robust And Highly Flexible Tunneling Application On %I
 
After=network.target
[Service]
 
Type=notify
 
PrivateTmp=true
 
ExecStart=/usr/sbin/openvpn --cd /etc/openvpn/ --config %i.conf
[Install]
 
WantedBy=multi-user.target
```



### 启动openvpn服务

配置好后就可以尝试启动openvpn了

```
systemctl enable openvpn@server
systemctl start openvpn@server
```

查看是否启动成功

```
systemctl status openvpn@server
```



如果启动失败了，多半是server.conf配置文件出了问题，请仔细检查各项有没有配置对。

如果是云服务器，记得开放安全组的端口



## 客户端

从网上下载openvpn GUI

[Community Downloads - Open Source VPN | OpenVPN](https://openvpn.net/community-downloads/)

客户端需要4个文件：

`client.crt`  `client.key`  `ta.key`  `ca.crt`

将四个文件从服务端上获取下来，保存在openVpn的配置文件夹里

windows配置文件夹默认应该是位于`C:\Users\xxx\OpenVPN\config`，建议在里面新建一个文件夹再保存进去

然后新建一个.ovpn的文件（openVpn客户端的配置文件，与其他4个文件同目录）

```
# 客户端
client
# 隧道类型,与服务器一致
dev tun
# tcp还是udp,与服务器一致
proto tcp
# 服务器ip和端口
remote xxx.xxx.xxx.xxx pppp
# 自动重连
resolv-retry infinite
# 不绑定本地特定的端口
nobind
# 服务器重启后保持一些状态
persist-key
persist-tun
# 客户端证书目录
ca ca.crt
cert client1.crt
key client1.key

# 不明,推测是远程证书验证之类的
remote-cert-tls server
# tls握手秘钥,与服务器保持一致,服务器0,客户端1
tls-auth ta.key 1
# 加密方式与服务器一致
cipher AES-256-GCM
# 开启数据压缩与服务器一致
comp-lzo
verb 3
# 相同信息的数量，如果连续出现 20 条相同的信息，将不记录到日志中
mute 20
# 使客户端中所有流量经过VPN,所有网络连接都使用vpn
redirect-gateway def1 bypass-dns
# 不保存密码
auth-nocache

```

配置完成即可尝试连接。



## 参考：

[Centos搭建Openvpn完全攻略 – 岩石的秘密小窝 (littlerook.co)](https://littlerook.co/2022/08/21/centos_openvpn/)

[CentOS 搭建 OpenVPN 服务，一次性成功！收藏了-腾讯云开发者社区-腾讯云 (tencent.com)](https://cloud.tencent.com/developer/article/2315269)

[Centos7 配置iptables NAT端口转发 - 香菜哥哥 - 博客园 (cnblogs.com)](https://www.cnblogs.com/yizhipanghu/p/17055728.html)

<br>
其他参考：

[中囯自制OpenVPN 翻墙出国回国评测-2023 – 墻知乎 (wallzhihu.com)](https://wallzhihu.com/openvpn/)

[最新翻件科网指南—揭开防城神秘面纱 ](https://nicechinavpn.com/)
