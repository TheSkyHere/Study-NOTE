获取semsor PID setpoint 等参数 ：ipmitool -H 10.99.100.11 -U admin -P kuaishou raw 0x30 0xf5 0x01 

//获取BMC时间
ipmitool sel time get

//修改IPMItool用户名、密码、权限、使能
ipmitool -H 10.99.100.11 -U admin -P 11111111 user set name（password） 3 admin2 

ipmitool -H 10.99.100.21 -U ADMIN -P ADMIN channel setaccess 1 3 callin=on ipmi=on link=off privilege=4

ipmitool user （enable）disable  3

//修改BMC-linux root用户密码（AMI）
ipmitool -H 10.99.100.11 -U toutiao -P toutiao\!@# raw 0x32 0x91 0x02 0x31 0x31 0x31 0x31 0x31 0x31 0x31 0x31


//horsea a ipmi常用指令：
风扇最大速度:ipmitool raw  0x30 0xf6 (0x0X) 0x64 0x64 0x03   (其中0x0X是对应的风扇号，seaking一共6个风扇1,2,3,4,5,6)
ipmitool raw 0x30 0xf6 1 30 30 0x03   指令应该这样发，先读出来数据，然后设置自己想要的duity，需要还原的时候把老数据发回去。
具体这个  没有相关的spec的。


//获取PSU info
ipmitool -H 192.168.0.5 -U toutiao -P toutiao\!@# raw 0x30 0xb1 0x0a 0x01  

//修改mac地址 （应该也是R11）
11320  ipmitool -I lan -H 192.168.0.17 -U ADMIN -P ADMIN raw 0x0c 0x02 0x08 0xc2 0x00 0x00
11321  ipmitool -I lan -H 192.168.0.17 -U ADMIN -P ADMIN raw 0x0c 0x01 0x08 0xc2 0x01
11322  ipmitool -I lan -H 192.168.0.17 -U ADMIN -P ADMIN raw 0x0c 0x01 0x08 0x05     0x7c 0xD3 0x0A 0xE4 0x02 0x0A

在uboot下修改地址：
AST2500EVB>setenv ethaddr 7c:d3:0a:e4:02:0E
AST2500EVB>saveenv



R11代码
MAC address查看：(通用)
Ipmitool lan print 1（查看BMC MAC地址及IP）
Ipmitool lan print 8（查看Sharelink MAC地址及IP）

ipmitool raw 0x0c 1 1 0xc2 0/1 (0表示BMC dedicated NIC，1 表示share NIC）这条命令为让MAC地址为可修改状态
ipmitool raw 0x0c 1 1 5 0xXX 0xXX 0xXX 0xXX 0xXX 0xXX (刷dedicated MAC 地址，0xXX 表示MAC地址）
ipmitool raw 0x0c 1 8 5 0xXX 0xXX 0xXX 0xXX 0xXX 0xXX (刷share NIC MAC 地址，0xXX 表示MAC地址） 


38:68:dd:33:7c:d0

R12代码：
修改 MAC1 用 :
ipmitool -I lanplus -H 127.0.0.1 -U admin -P admin raw 0x0c 0x01 0x01 0xc2 使能修改mac
ipmitool -I lanplus -H 127.0.0.1 -U admin -P admin raw 0x0c 0x01 0x01 0x05 0x7c 0xD3 0x0A 0xE4 0x02 0x08 //mac 地址第一字节必须是偶数
修改 MAC2用：
ipmitool -I lanplus -H BMCIP -U ADMIN -P ADMIN raw 0x0c 0x01 0x08 0xc2 
ipmitool -I lanplus -H BMCIP -U ADMIN -P ADMIN raw 0x0c 0x01 0x08 0x05 0x7c 0xD3 0x0A 0xE4 0x02 0x08 //mac 地址第一字节必须是偶数


//获取PSU info active
3. Set PSU1 enter to active mode with command  "ipmitool raw 0x6 0x52 0x5  0xB2 0x0 0xD0 0x01 0x81"
4. Set PSU0 enter to standby mode with command "ipmitool raw 0x6 0x52 0x0B 0xB0 0x0 0xD0 0x02 0x5E"
5. Check PSU0 status with command "ipmitool raw 0x6 0x52 0x5 0xb0 1 0xd0" and PSU0 LED.


//16进制转换字符
https://www.bejson.com/convert/ox2str/ 

//设置静态IP V4
设置ipmi的静态ip
lan set 1 ipsrc dhcp
ipmitool lan set 1 ipsrc static // 设置ipsrc的类型，可以使dhcp， 也可以是static.     set 1 表示 网络连接的类型，为channel1, 
ipmitool lan set 1 ipaddr 172.20.1.1                 //设置静态IP
ipmitool lan set 1 netmask 255.255.0.0           //必须先设置静态IP，然后再设置掩码。


//设置IPV6网关《gateway》
ipmitool -H 10.99.100.17 -U toutiao -P toutiao\!@# raw 0x0c 0x01 0x01 64 0x01   《使能》
ipmitool -H 10.99.100.17 -U toutiao -P toutiao\!@# raw 0x0c 0x01 0x01 0x44 0x20 0x01 0x0d 0xb8 0x00 0x00 0x00 0x23 0x00 0x08 0x08 0x00 0x20 0x0c 0x41 0x7a


//seaking 项目的  disable/enable IPV4 IPV6
ipmitool -H 10.99.100.14 -U toutiao -P toutiao\!@# raw 0x0c 0x01 0x01 0xc3 0x01   		//enable IPV6  、、可以在bond模式下设置
ipmitool -H 10.99.100.14 -U toutiao -P toutiao\!@# raw 0x0c 0x01 0x01 0xc3 0x00    		//disable IPV6 、、可以在bond模式下设置                     
ipmitool -H 10.99.100.14 -U toutiao -P toutiao\!@# raw 0x0c 0x02 0x01 0xc3 0x00  0x00   //查看IPV6 状态
//seaking 项目的  disable/enable IPV4 IPV6
ipmitool -H 10.99.100.13 -U toutiao -P toutiao\!@# raw 0x32 0x71 0x00 0x01 0x03  		//设置eth1 ipv4和ipv6的状态3  、、不可以在bond模式下设置 
ipmitool -H 10.99.100.14 -U toutiao -P toutiao\!@# raw 0x32 0x72 0x00 0x00 0x00         //查看eth0 ipv4和ipv6的状态
//seaking 解除bond 3E 05 01
ipmitool -H 10.99.100.117 -U toutiao -P toutiao\!@# raw 0x32 0x71 0x01 0x00 0x00 0x01 0x64 0x00 0x03 0x01    
//seaking 开启bond 3E 05 00
ipmitool -H 10.99.100.117 -U toutiao -P toutiao\!@# raw 0x32 0x71 0x01 0x01 0x00 0x01 0x64 0x00 0x03 0x01 



//seaking--一键收集log
raw 0x3e 0x02	开始收集
raw 0x3e 0x03	完成确认
raw 0x3e 0x04	删除收集文件
//通过weget方式下载  需要提前通过指令搜集（一键log）
wget https://192.168.0.5/api/ipt/download/diaginfo --no-check-certificate  
//通过weget方式下载  （SOL log）
wget https://192.168.0.5/api/ipt/download/soldump --no-check-certificate  



horsea 一键收集log
收集:
ipmitool -U admin -P admin -H 192.168.0.8 raw 0x2e 0x13 0xa9 0x19 0 0 0 0 0  0x05 0x05  0x61 0x64 0x6d 0x69 0x6e  0x61 0x64 0x6d 0x69 0x6e
检测是否完成：
ipmitool -U admin -P admin -H 192.168.0.8 raw 0x2e 0x14 0xa9 0x19 0x0
delete：
ipmitool -U admin -P admin -H 192.168.0.8 raw 0x2e 0x15 0xa9 0x19 0x0
下载：
wget http://192.168.0.8/dump_collect/dump_collect.tar.gz 


horsea-百度：获取sol log
curl -X POST -d "username=ADMIN&password=ADMIN" "http://192.168.0.10/baidu/session" -c ./cookie
curl -X GET http://192.168.0.10/baidu/maintenance/sol/dump -H "X-CSRFTOKEN:7XGNludK" -b ./cookie -o sol.txt  

horsea-设置风扇转速：
ipmitool raw 0x30 0xf6 0x01（0x01到0x06效果一样） 30 30 0x03 

horsea-a 获取风扇当前最高转速的信息（OEM）：
ipmitool -H 192.168.0.8 -U admin -P admin raw 0x3e 0x10 0x00 0x4c 0xA5 0x10



power capping：
~ » ipmitool -U ADMIN -P ADMIN -H 192.168.0.15 -b 6 -t 0x2c nm power                                                                                                         morton@morton
Missing parameters: nm power range min <minimum> max <maximum>.




horse-读取outlet_temp:
i2c-test -b 1 -s 0x48 -m 1 -rc 2 -d 0x04

//机器上电后OS状态： 
//设置状态为always -off  
~ » ipmitool -H 192.168.0.5 -U toutiao -P toutiao\!@# raw 0 6 0      
//查看状态
ipmitool -H 192.168.0.5 -U toutiao -P toutiao\!@# chassis status   


//ME指令
ipmitool -H 192.168.0.36 -U toutiao -P toutiao\!@# -t 0x2c -b 0x6 raw 0x2e 0xc8 0x57 0x01 0x00 0x1 0x00 0x00  
其中 -t target 目标（发送给ME）



i2c指令：

//HW给我们的iic地址是从G1到G9  对应的地址是G1=iic0
//查看iic 地址指令（在BMC中）地址是7位的！！！
i2c-test --scan -b 3   （3是IIC3）
GPU卡测试
i2c-test -b 4 -s 0x72 -m 0 -rc 1 -d 0x02    //switch
i2c-test -b 4 -s 0x4f -m 1 -rc 1 -d 0x5c

i2c-test -b 4 --scan
ipmitool -H 192.168.0.59 -U ADMIN -P ADMIN raw 0x6 0x52 9 0xe4 0 0x00 0x00 0x01// 切switch e4 到1   0x00 0x00 是offset


//对摸个iic-5读数据（9e cmd）（b2地址）（0x12 读的数据长度）
ipmitool -H 192.168.0.5 -U toutiao -P toutiao\!@# i2c bus=5 0xb2 0x12 0x9e 
等价：i2c-test -b 5 -s 0x59 -m 1 -rc 18 -d 0x9e


//对摸个iic d（13）读数据（97 cmd）（e2地址）（0x2 读的数据长度）
ipmitool -H 10.99.100.48 -U ADMIN -P ADMIN raw 0x6 0x52 0xd 0xe2 0x2 0x97     

//查看IIC
ipmitool -H 10.99.100.34 -U ADMIN -P ADMIN raw 0x38 0xba 1 


raw 0x30 0xba 0/1  切i2c 的bus number

write
ipmitool –H xxx.xxx.xxx.xxx –U ADMIN –P ADMIN raw 0x6 0x52 9(i2c) 0xa0(addr) 0 (read) 0x00(offset high) 0x24(offset low) 0x3(data) 0x4(data)
read:
ipmitool –H xxx.xxx.xxx.xxx –U ADMIN –P ADMIN raw 0x6 0x52 9(i2c) 0xa0(addr) 2(read count) 0x00(offset high) 0x24(offset low) 
Response:0x3 0x4 

write
i2c-test -b 4 -s 0x50 -m 1  -w -d 0x00(offset high) 0x24(offset low) 0x04(date) 0x05(date)
read:
i2c-test -b 4 -s 0x50 -m 1  -rc 2 -d  0x00(offset high) 0x24(offset low)

i2c-test -b 4 -s 0x59 -m 1  -w -d 0x03



读取PSU 0x19
i2c-test -b 5  -s 0x58  -m 1  -rc 1 -d  0x19
i2c-test -b 5  -s 0x58  -m 1  -rc 6 -d  0x99
ipmitool –H xxx.xxx.xxx.xxx –U ADMIN –P ADMIN raw 0x6 0x52 11(i2c) 0xb2(addr) 3(read count) 0x00(offset high) 0x8d(offset low) 



//horsea-a 测试风扇
ipmitool raw 0x30 0xf6 1 30 30 0x3 （Byte 3 表示第几组风扇，Byte 4 表示限定风扇的最低转速百分比，Byte 5 表示风扇的最高转速百分 


horsea-a测试使用
i2c-test -b 13 -s 0x71 （sddr：0xe2）-m 1  -w -d 4  （对应的 1 2 4 8 16 32  对应的是channel 0 1 2 3 4 5）
i2c-test -b 13 --scan  （扫描）
i2c-test -b 13 -s 0x10（addr：0x20） -m 1  -rc 1 -d  0x00  （读取温度）

//horse-a MOS solt
//J100
i2c-test -b 13 -s 0x71 -m 1 -w -d 4
i2c-test -b 13 --scan
//J103
i2c-test -b 4 -s 0x71 -m 1 -w -d 4
i2c-test -b 4 --scan
//J92
i2c-test -b 13 -s 0x70 -m 1 -w -d 8
i2c-test -b 13 --scan



MOC的串口读取
	switch:
	i2c_bus=13
	addr = 0xe2
	channe = 2
i2c-test -b 13 -s 0x71 -m 1  -w -d 0x02
i2c-test -b 13 --scan  //可以看看是否能扫描到

	moc_temp
	i2c_bus=13
	addr = 0x20 offset=0x00
i2c-test -b 13 -s 0x71 -m 1  -rc 2 -d  0x00	



SENSOR_OEM_GPU2_Temp

	INT8U AMD_GPU_SLV_ADDR= 0x5c;  ///FOR GPU1 TEMP and GPU2 TEMP

	INT8U NVIDIA_GPU_SLV_ADDR1 = 0x9c;  ///FOR GPU cpu0 TEMP

	INT8U NVIDIA_GPU_SLV_ADDR2 = 0x9e;  ///FOR GPU cpu1 TEMP

	INT8U SLV_ADDR_PCIE_MUX_EA = 0xea;

	INT8U SLV_ADDR_PCIE_MUX = 0xe4;






用户名admin，密码：ngxdcstT&byteD@2012


/conf/BMC1 # 
/conf/BMC1 # 
/conf/BMC1 # grep[12037 : 12038 INFO]RAID Device ( 2) is not present on the bus 5
 -ir "c[12037 : 12038 INFO]RAID Device ( 2) is not present on the bus 5
hanne[12037 : 12039 INFO]COMP_MNGR : Get the Avago SAS IT Controller Count 
[12037 : 12039 INFO]SAS IT (HBA) Device ( a) is not present on the bus 5
pAMIGetSASCountInfo is Fail
ln[12037 : 12039 INFO]RAID Device ( 2) is not present on the bus 5
pGetRAIDCountInfo is fail
u[12037 : 12038 INFO]RAID Device ( 2) is not present on the bus 5
mber"[12037 : 12038 INFO]RAID Device ( 2) is not present on the bus 5
 *
  *
LanChcfg1.ini:ChannelNumber=8
SerialChcfg.ini:ChannelNumber=2
Smlinkipmbchcfg.ini:ChannelNumber=6
Smmchcfg.ini:ChannelNumber=5
SysifcChCfg.ini:ChannelNumber=15
sudo grep -ir "Channelnumber" *


Fearow项目笔记：

ipmitool -H 192.168.0.59 -U ADMIN -P ADMIN raw 0x30 0xab 0 5 2 0 0 0x300(channel)   //读ME    Bus: 0, Device: 5, Function: 2 (Errors)
ipmitool -H 192.168.0.59 -U ADMIN -P ADMIN raw 0x30 0xac 0 0x00 0x00 0x30(channel)    //MSR寄存器


==================Reciver IPMB Data begin============
Netfun=2e
CMD=40
20 bd 23 2c 0 40 0（完成码） 57 1 0 40（是peci的completion code） 7f 0 0 0 0 0 0 0 7d（CRC）
==================Reciver IPMB Data Over=============


修改mask
ipmitool -I lanplus -H 127.0.0.1 -U ADMIN -P ADMIN raw 0xc 0x1 0x8 0xc2 0
ipmitool -I lanplus -H 127.0.0.1 -U ADMIN -P ADMIN raw 0xc 0x1 0x8 0x5 0x00 0x01 0xEC 0x99 0x07 0x17 

查看ncsi
~ # ipmitool -H 127.0.0.1 -U ADMIN -P ADMIN raw 0x32 0x72 0 1 0 
是能ncsi
~ # ipmitool -H 127.0.0.1 -U ADMIN -P ADMIN raw 0x32 0x71 0 1 3



//horsea-a安全模式：
带外执行，这里面忽略了的命令格式
1、获取安全模式的命令：ipmitool raw 0x3E 0x5B， 如果返回 0x80，说明是关闭；0x81 开启
2、设置安全模式命令：
开启：ipmitool raw 0x3E 0x5A 0x81
关闭：ipmitool raw 0x3E 0x5A 0x80


添加IPMI用户
~ » ipmitool -H 192.168.0.36 -U toutiao -P toutiao\!@# user set name 3 ADMIN123                                                                                                                   morton@morton
~ » ipmitool -H 192.168.0.36 -U toutiao -P toutiao\!@# user set password 3 ADMIN123  
~ » ipmitool -H 192.168.0.36 -U toutiao -P toutiao\!@# channel setaccess 1 3 callin=on ipmi=on link=on privilege=4
~ » ipmitool -H 192.168.0.36 -U toutiao -P toutiao\!@# user enable/disable 3 
   

查看ME status：
ipmitool -b 6 -t 0x2c raw 6 1 50 01 04 14 02 21 57 01 00 0a 0b 04 33 90 01 



jviewer 的使用指令
java -jar JViewer.jar -apptype StandAlone -hostname 192.168.0.21 -webport 443 -u admin -p admin -launch JViewer -lang EN


网络抓包工具
tcpdump -v    具体使用百度

//查看当前目录下文件大小
sudo du -sh *  

fdisk -l

lsblk



//APML相关指令power limit：
步骤如下：
1.首先加压CPU，通过SDR发现CPU功率达到了120W左右（测试机器只安装了一个cpu）
2.通过ipmitool -U admin -P admin -H 192.168.0.25 raw 0x36 0x15 0x00  0x01可以直接访问cpu获取功耗发现功率正常120W左右
3.通过ipmitool -U admin -P admin -H 192.168.0.25 raw 0x36 0x16 0x00  0x01  0x00 0xd0 0x00 0x00设置limit为53W左右，
4.然后通过ipmitool -U admin -P admin -H 192.168.0.25 raw 0x36 0x15 0x00  0x01和SDR发现功率降到53W左右了，
5.还原设置后cpu功耗恢复到120W左右。




FLASHtool工具使用：
./FlashTool64Bit -H 192.168.0.87 -U jdroot -P JCss%6\!8 -p MACHAMP -t cpld -i 1 -r 0 -f MACHAMP_MLB_CPLD_U27_V0.6_for_fearow_ncsi_test.jed




cent OS 开机自启动：
一、
	（图形化）需要提前登陆进OS
	/etc/profile 文件中加入：gnome-terminal（打开控制台） -x bash -c 'cd /home/inventec/BMC_test;./run.sh;exec bash'
二、
	无需提前登陆
	vim /etc/rc.local中添加如下两条：
	cd /home/inventec/BMC_test/
	./run.sh    
三、
	echo 'xterm -e '$PWD'/test2/BMC-test2.sh' >> /etc/profile.d/auto.sh 



//OS中加载系统中的ipmi
service ipmi start


centOS自动登录（无图形化）：
vim /etc/systemd/system/getty.target.wants/getty@tty1.service 
ExecStart=-/sbin/agetty --noclear %I $TERM  --autologin root


centOS自动登录（图形化）：
	vim /etc/gdm/custom.conf
修改：
	AutomaticLoginEnable=1
	AutomaticLogin=root



GUA切换到控制台ctrl+alt+F7



添加SEL log
添加CPU在位Deasserted
ipmitool -U admin -P admin -H 192.168.0.8 raw 0x0a 0x44    0x24 0x00 0x02 0xf6 0x06 0x9d 0x4d 0x20 0x00 0x04     0x07 0x70 0xef  0x07 0x00 0x00

ipmitool -U admin -P admin -H 192.168.0.8 raw 0x0a 0x44    0x24 0x00 0x02 0xf6 0x06 0x9d 0x4d 0x20 0x00 0x04     0x07 0x70 0x6f  0x01 0x00 0x00
18 | 01/03/2011 | 00:29:14 | Processor CPU0_Status | Thermal Trip | Asserted
~ » ipmitool -U admin -P admin -H 192.168.0.8 sel get 0x18                                                                                                                                        morton@morton
SEL Record ID          : 0018
 Record Type           : 02
 Timestamp             : 01/03/2011 00:29:14
 Generator ID          : 0020
 EvM Revision          : 04
 Sensor Type           : Processor
 Sensor Number         : 70
 Event Type            : Sensor-specific Discrete
 Event Direction       : Assertion Event
 Event Data (RAW)      : 010000
 Event Interpretation  : Missing
 Description           : Thermal Trip

Sensor ID              : CPU0_Status (0x70)
 Entity ID             : 3.5
 Sensor Type (Discrete): Processor
 States Asserted       : Processor
                         [Presence detected]

horsea-a
add sel
添加Configuration Error | Asserted
ipmitool -U admin -P admin -H 192.168.0.8 raw 0x0a 0x44    0x24 0x00 0x02 0xf6 0x06 0x9d 0x4d 0x20 0x00 0x04     0x07 0x70 0x6f  0x05 0x00 0x00 
添加Configuration Error | Deasserted
ipmitool -U admin -P admin -H 192.168.0.8 raw 0x0a 0x44    0x24 0x00 0x02 0xf6 0x06 0x9d 0x4d 0x20 0x00 0x04     0x07 0x70 0xef  0x05 0x00 0x00 
添加DIMM Correctable ECC
ipmitool -U admin -P admin -H 192.168.0.8 raw 0x0a 0x44    0x24 0x00 0x02 0xf6 0x06 0x9d 0x4d 0x20 0x00 0x04     0x0c 0x90 0x6f  0x05 0x00 0x00
ipmitool -U admin -P admin -H 192.168.0.8 raw 0x0a 0x44    0x24 0x00 0x02 0xf6 0x06 0x9d 0x4d 0x20 0x00 0x04     0x0c 0x90 0xef  0x05 0x00 0x00
添加FAN在位Deasserted
ipmitool_a -U admin -P admin -H 192.168.0.25 raw 0x0a 0x44    0x24 0x00 0x02 0xf6 0x06 0x9d 0x4d 0x20 0x00 0x04     0x04 0x48 0x88  0x00 0x00 0x00 


//yafu升级//2是烧写BIOS  1是烧写BMC  加-cp（保留配置升级）
workspace/Build/.workspace/tmp/yafuflash-5.7.0.1.0-ANY/data/obj/Linux_x86_64
yafuflash.exe -nw -ip 10.99.100.49 -u admin -p admin  -d 2 所要烧写的.BIN位置  
Yafuflash -nw -ip 192.168.0.5 -u toutiao -p toutiao!@# -d 1 /home/morton/SVN_project/Seaking_Common_release/trunk/workspace/Build/output/rom.ima 



//socflash 升级BMC指令
./socflash_x64 if=./BMC1.03/athenaG2BMC103.ima  option=g rstflash=mxic


grep的使用：
grep  "192.168.1" Horsea-a-MOC-N01-minicom.cap -c （-c只显示数量）



先执行获取名利
Ipmitool raw 0x0c 0x2 8 210
返回：
data1，data2

再设置，把data1代入，照如下设置
Ipmitool raw 0x0c 0x1 8 210    0xxx（interface data1） 0x01（0h – Change Auto to Manual    1h -  Change Manual to Auto）



关于sol功能 切串口的函数
PDK_SwitchEMPMux（）


查看CPU使用率：
top -H -p pid命令查看进程内各个线程占用的CPU百分比
ps H -eo pid,tid,%cpu,%mem,comm --sort=%cpu



celeastica:
使用HPM升级BMC
ipmitool hpm check
ipmitool hpm upgrade rom.hpm
ipmitool hpm upgrade -z 0x7fff rom.hpm

切BMC的串口
shift +456 ，然后2 是BMC   0、1、3是其他





打开和关闭PDU  athenaG1 
apc>olOff 1
E000: Success

apc>olOn 1 
E000: Success