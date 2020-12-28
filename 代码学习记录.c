在IPMI_Main.c文件中的main函数中创建相关线程


数据获取

其中创建了MsgHndlr线程，主要是处理message
其中有个函数GetMsg函数获取相关message（包括IPMI指令）



PDKAccess.c 中InitPDKHooks 函数中注册PDKHandle相关函数
g_PDKHandle[g_PDKInitialize[i].PDKHookNum]= dlsym(dl_pdkhandle,(char *)g_PDKInitialize[i].PDKHookName);




关于指令注册（指令的解析都在ProcessIPMIReq中）

PDKCmdsAccess.c 中InitPDKCmdHooks 函数中注册PDKCmdsHandle相关函数
g_PDKCmdsHandle[g_PDKCmdsInitialize[i].PDKHookNum]= dlsym(pdkCmdAccessHandles.PDKCmdHooksHandle,(char *)g_PDKCmdsInitialize[i].PDKHookName);
有OEM相关的cmd

在InitBMCSharedMem（SharedMem.c）函数中初始化了pBMCInfo->MsgHndlrTbl （注册的数据来源于m_MsgHndlrTbl数组msghndlr.c）
结构体（此结构体中有AMI（ipmi协议中）相关的cmd）

                                  


数据流向example：
KCS->管道1->MsgHndlr(处理数据)->管道2->KCS
管道1的名称所有interface向同一管道发送数据。
管道2的名称会通过在管道1中读到的数据（res）得到，然后解析处理完数据后通过管道2返回给对应的interface

其中8个interface
《ifcSupport.c》中
DynamicPriority_T m_dynamicloadpriority[MAX_IPMI_IFC_COUNT] = 
{
    {KCS_IFC_TASK, 0},
    {BT_IFC_TASK, 0},
    {SSIF_IFC_TASK, 0},
    {SERIAL_IFC_TASK, 0},
    {USB_IFC_TASK, 0},
    {IPMB_IFC_TASK, 0},
    {LAN_IFC_TASK, 0},
    {UDS_IFC_TASK, 0}
};
 if(0 != InitIPMIConfig(BMCInstCount))  //其中有8个interface接口的函注册到g_BMCInfo[]中
 StartIfcTasks(BMCInst);    //在这里面创建8个interface的Task
关于向通道MSG_HNDLR_Q中写入数据的入口有
	首先通过InitIPMIConfig->GetLibraryMetaData函数把8个interface函数添加进入了g_BMCInfo[]数组中同时使能相关函数
	









其中关于指令调用的
1、CMD_SEND_MSG指令->SEND_MSG->SendMessage(AppDevice.c中)->if (0 != PostMsg (&m_MsgPkt,MSG_HNDLR_Q,tmpBMCInst))
一个指令

2、SetACPIState（）->PostMsg (&MsgPkt,MSG_HNDLR_Q, BMCInst);

3、XXX->API_ExecuteCmd(MsgPkt_T *pMsgPkt, int BMCInst)[执行一些指令]
->PostMsgHandlerPkt(pMsgPkt,BMCInst)
->PostMsgNonBlock(pMsgPkt, MsgHndlrkey, BMCInst)[non block运行发送一些数据到Q]

4、IPMBifcTask->ProcessIPMBReq->PostMsg (pReq, MsgHndlrkey, BMCInst)  
来源于IPMB接口上的cmd发送到到Q

5、KCSIfcTask->在KCSIfc.c 中的processKCSReq->PostMsg (pReq, MsgHndlrkey, BMCInst)数据的写入Q 
来源于KCS上的一些cmd

6、PEFTask->EventFilter->PerformActionAlert->




Log->PostMsg (&MsgPkt, MSG_HNDLR_Q,BMCInst);


7、LANIfcTask->ProcessLANReq (&Req, &Params, BMCInst)->ProcessRMCPReq

   ->ProcIPMIReq->PostMsg (&pBMCInfo->LANConfig.MsgReq, MSG_HNDLR_Q,BMCInst)
   ->Proc20Payload->ProcIPMIReq->PostMsg (&pBMCInfo->LANConfig.MsgReq, MSG_HNDLR_Q,BMCInst)
其中两个写入Q的函数（这个是关于ipmitool指令加一个参数Q）
	

8、ACTIVATE_SESSION->ActivateSession函数[AppDevice.c]->PasswordViolation (BMCInst)
->PostMsg (&m_MsgPkt, MSG_HNDLR_Q,BMCInst);
一个指令调

9、UDSIfcTask->ProcessUDSReq(&Req,BMCInst)->PostMsg (pReq,MsgHndlrkey, BMCInst)
来源于UDS接口上的cmd发送到到Q

10、USBIfcTask->ProcessCmd(BMCInst)
->ProcessConfig(CFG_WRITE,CMD_REG, (char *)&(ReqPkt->Data), ReqPkt->DataLen, BMCInst)
->ProcessDataInState(Mode, Reg, Buffer, DataLen, BMCInst)
->StartCmdProcessing(BMCInst);
->ProcessConfigCmd->PostMsg (pReq, MsgHndlrkey,BMCInst)


关于MDS
R12   使用12.0.1
R11.8 使用4.0.3
R11.5 使用4.0.1





关于添加cmd指令：
在（PDKCmds.c）中的数组中添加相关数据，同时需要到（cmdselect.c）中使能相关cmd













学习记录
/opt/mds/mds-12.0.1/SPX/spx/utils 下有相关的SPX包的工具可以制作SXP解压SPX。


打PATCH 注意事项：具体使用百度，指令（diff -u test.c test_new.c > 01_test.patch）
在使用第一个脚本的时候会打PATCH。注意同一个文件打多个PATCH。
注意打PATCH的路径



把project中的文件打包成SPX包的python脚本
./PackSPX.py -c /home/morton/gitrepo/MeiTuan/B_RR12/packages/spx/libipmipdk-6.16.0.0.0-ARM-AST2500-WolfPass-AMI-src.spx /home/morton/gitrepo/AS60G1_1/B_RR12/AS60G1_pdk_packages/pdk/libipmipdk-6.16.0.0.0-ARM-AST2500-WolfPass-AMI-src
在（PMConfig.c）initSDRSELNVRConfig函数中运行g_PDKHandle[PDKINITNVRSDR]函数从SDR.dat（由Wolfpass.pmc图形化界面生成）中读取数据初始化SDR



在PDKHooks.c-》PDK_Postinit-》PDK_RegisterSensorHooks(BMCInst)中注册了一下两类函数
pPDK_MonitorHook_T			pPreMonitor;		/* Pre monitor hook   */
pPDK_MonitorHook_T			pPostMonitor;		/* Post Monitor hook */










软件烧写：（4种）
web
烧录器
soc
（yafu）
curl




关于MDS4.0.1 打开PMC文件后，SDR不全的问题。需要ddf文件（向静）




SensorMonitorTask
管理阈值和非阈值sensor 
对于在图形化界面中有连接iic的线的sensor和没有连接线的sensor有不同的处理方式
ast2500evb.c和WolfPass.c（R11和R12）下面有MDS中不连线的sensor的读写函数



修改ipmi user 在usrinfo.c文件下 在UserInfo_LoadFile函数中





//关于PID的table 修改添加
在config下修改，使用kk脚本，生成文件拷贝到下面目录中（同时修改两个脚本拷贝（dev-install）和（RULES））
libipmipar_ast2500evb-6.15.0.0.0-ARM-AST2500-WolfPass-src/data
spx文件下有另个脚本需要修改
//关于PID fan悾速
在PDKhook.c->PID_Fan_Speed_Control();函数中
//关于PWM 的使能文档
pdk：platform-6.1.0.0.0-ARM-AST2500-WolfPass-src/data
pdk：Bootloader_ex-6.3.0.0.0-ARM-AST2500-WolfPass-src/data



关于H5 和java 的权限  mds勾选
《ExtendedPrivilege》


远程拷入
scp -r <user>@10.99.100.102:/home/morton/gitrepo ./

//把tftpd-hpa考出
sudo scp tftpd-hpa fight@10.99.100.20:/tmp   


清屏  ctrl+l

ps -ef |grep  minicom    查看并筛选进程
kill 编号  杀死进程

CPU占用率
top



添加ADD sel
AddSELEntry
IEC_AddSELEntry




OS下 SOL配置文件：
1.add below content to /etc/default/grub:
	GRUB_CMDLINE_LINUX_DEFAULT="console=tty0 console=ttyS0,115200n8"
	GRUB_TERMINAL=serial
	GRUB_SERIAL_COMMAND="serial --speed=115200 --unit=0 --word=8 --parity=no --stop=1"
2.run "grub2-mkconfig -o /boot/grub2/grub.cfg" to rebuild the grub.cfg file. 

OS下 SOL配置文件：centos
编辑文件/etc/grub2-efi.cfg,在相应kernel字段的最后加入文字：console=tty0 console=ttyS0,115200n8
	


  

git log --stat  //查看每次commmit修改

git log --oneline //所有log


//提交代码有问题以后，无需abandon：
//参考：https://www.jianshu.com/p/a8a2ac58f37d
git commit --amend  随后git push

//vim窗口显示垂直切分窗口
:vsplit +文件路径
例如：
:vsplit  /etc/httpd/conf.d/vhost.conf


//share nic
//AMI OEM SPEC
中 0x32 0x71 0x01
设置ethX EN或者DIS的 就是share link
IPTmFun.c->OEM_Bus_Network_StatusTask()//监测eth0和eth1状态。
//关于AS60G1的  die nic 和share nic 
默认开启的boanding 在MDS中选择 然后开机以后系统会读取EEprom中由BIOS写入的数据 
一个独立的线程通过此来判断nic 模式（dis en auto）这三种模式，然后重新设置nic模式
READ_EEPROM (EEPROM_24C256, FRU_I2C_BUS_NUM, SLV_ADDR_FRU,&NetworkSwitchFlag, SysConfig_addr+0x18, 1);//读取EEPROM
OEM_MonitorNicTask（）线程一直在根据NetworkSwitchFlag值来 Dis/En share nic

//一键KVM
https://IP/oem/kvm 

//查看二进制文件
hexdump dhcp6c_duid      

//查看网卡型号
linux中怎么查看网卡型号: 
lspci | grep Ethernet


kvm java的使用
使用javaws 运行下载下来的文件jviewer.jnlp  
同时可能需要运行：use/lib/jvm/jdk-----/bin/jcontrol 添加对应的IP





//使用linux远程登录另一台linux
ssh -l root 192.168.0.1



//git 直接merge
git cherry-pick 17d170611905077d4c97dfd9f91850496ad93342 


//关于sol
打开sol，会把创建（fp=fopen(SOLInfoFile,"a+");）文件/var/solinfo/solinfo.log 并把数据传输到这里
sol注意串口的映射关系
//SOL开启指令
# ipmitool -I lanplus -H 172.17.0.177 -U toutiao -P toutiao\!@# sol activate


//修改chassis part number 
ipmitool -H 10.99.100.111 -U ADMIN -P ADMIN fru edit 0 field c 0 (06)  


//Seaking 代码地址 SVN
https://10.99.100.208/BMC/SPX_RR11-8_AST2500/branch/Seaking_Meituan


//Seadra
0x77 0x6f 0x6c 0x66 0x70 0x61 0x73 0x73   0x00
ipmitool -U ADMIN -P ADMIN -H 192.168.0.21 raw 6 0x52 3 0xa6 0 0x4 00   0x49 0x43 0x45 0x42 0x45 0x52 0x47 0x33 0x5f 0x4a 0x42 0x4f 0x44 0x31  0x00




//seckingde 项目
关于PMC文件，另个pmc文件都会用到。（一个是12HDD一个是24HDD的）
生成的SDR.dat 要改成12SDR.dat和24SDR.dat
同时使用脚本改变24HDD的ast2500evb_24hdd.c和ast2500evb_24hdd.sdr文件
libipmipar_ast2500evb_ami-3.2.0.2.0-ARM-AST2500-AST2500EVB-AMI-src/data 



//CPU VR就是CPU的电源芯片


//redfish 账号密码：
Administrator
superuser




//SVN 使用指令：
svn co https://10.99.100.208/BMC/SPX_RR11-8_AST2500/trunk
svn add ./lighttpd-3.1.0.2.0-src.spx
svn commit -m '1.fixed http->https  will copy .spx to patch'
svn update//把库里最新的代码更新到本地
//学习记录tag
svn cp -m "tag for Seaking_Meituan_V29.06.00" /home/morton/SVN_project/Seaking_Meituan/trunk  https://10.99.100.208/BMC/SPX_RR11-8_AST2500/tag/Seaking_Meituan_V29.06.00 



//git的使用指令：
git clone 。。。。。（网站上的）
git checkout -b dev 创建新的本地分支
修改文件
git add 
git commit -m （提交本地分支代码）
git checkout master 
git merge -squash 《dev》  加squash 不提交-m 的log，随后需要再次commit提交log
git commit -m （提交本地代码）
git push origin master 提交到远端库
//提交tar 
git push origin --tag


//修改Project-ID
在pdkcmds.c->IPT_GetDevID();中直接修改




//内部调用ipmicom
API_ExecuteCmd (&MsgPkt, BMCInst);







//关于BMC中保存ipmi指令的log文件
/var/log/audit.log 


//BMC中查看GPIO状态 
gpiotool --get-gpios-data 36

gpiotool --get-dir 35
gpiotool --get-dir 36
gpiotool --get-dir 37

gpiotool --get-gpios-data 218
gpiotool --set-dir-output 36  
gpiotool --set-dir-output 218  
gpiotool --set-gpios-high 36

gpiotool --set-gpios-high 218
gpiotool --set-gpios-low 218
//查看linux设备
dmesg

//修改uboot
拷贝 bootloader下的 uboot文件到 uboot.orig进行备份
打开mds修改对应文件，改完后右键Create BootLoader patch到指定的spx包
需要WIPE编译代码才能编过



ipv6 配置文件

AMI ipv6 分配不到地址的建议（没有发送请求）

相关进程

版本号 



TB600G4 雷神2  THOR2
TESLA   雷神1  THOR1



//AMD 处理器
7502 罗马平台的
Naples  bios110 
Rome CPU使用bios 010 
Rome是Naples的下一代产品
Naples(那不勒斯) 罗马 米兰 


//Seaking-字节下载sol log
具体下载文件地址在kvm.c文件下有  922行
curl -X POST https://10.99.100.17/api/session -d 'password=XXX&username=XXX' -c ./cookie -k

curl -X GET https://10.99.100.17/api/ipt/download/soldump -H "X-CSRFTOKEN:XXXXXX" -k -O  //  -O/--remote-name 把输出写到该文件中，保留远程文件的文件名  
curl -X GET https://10.99.100.17/api/ipt/download/diaginfo -H "X-CSRFTOKEN:XXXXXX" -k -O  //  -O/--remote-name 把输出写到该文件中，保留远程文件的文件名  需要提前通过指令搜集
curl -X GET -o diag https://10.99.100.17/api/ipt/diaginfo -k //需要提前通过指令收集log

curl -X DELETE https://10.99.100.17/api/session -H "X-CSRFTOKEN:XXXXXXXX" -b ./cookie -k
 
 
//通过weget方式下载  需要提前通过指令搜集
wget https://192.168.0.5/api/ipt/download/diaginfo --no-check-certificate  
 
//截图指令截取一张0x01图片路径：/var/bsod 
0x30 0xa2 0x01 
curl -X GET -o XXXXX.jecp  https://10.99.100.22/api/settings/download_image -H "X-CSRFTOKEN:3YwAsbVU" -k
wget https://192.168.0.5/api/settings/download_image --no-check-certificate    

//百度curl升级FW
curl -X POST -d "username=ADMIN&password=ADMIN" "https://192.168.0.20/baidu/session" -c ./cookie -k


curl https://192.168.0.20/baidu/maintenance/BMC/firmware -F "fwimage=@./rom.ima" -H "X-CSRFTOKEN:wVM7b2p0" -b ./cookie -k


//关闭BMC下ipmimain
/etc/init.d/ipmistack stop 


//webui***********************************************************************************
model.get("timeStamp")
MODEL_ADD_INTEGER("timestamp", timestamp);








//linux挂载文件
mount -o nolock -t nfs 192.168.0.25:/var/lib/tftpboot/BMC1/ImageTree/usr/ /usr



RR11-8 AMI svn仓库路径如下：
https://VSVNInventec:Inventec%40Perfect8mkB37@svnsm.ami.com.cn/svn/SPX/projects-x/trendy/customers/AMI_AST2500_RR11-8-Plus/packages
https://VSVNInventec:Inventec%40Perfect8mkB37@svnsm.ami.com.cn/svn/SPX/projects-x/trendy/customers/AMI_AST2500_RR11-8-Plus/packages
//账户密码   可以通过web登录查看
VSVNInventec
Inventec@Perfect8mkB37



//SF600指令下载
 /c/Program\ Files\ \(x86\)/DediProg/SF100/dpcmd.exe --type MX25L25735F -z  Desktop/FLASH/rom.ima

//seaking sel等级函数
OEM_SNMP_GetSeverity

//查看网络
netstat -nat

//张建
fzf 
->vim **     
->cd **
ranger

//关于cccat资料
https://github.com/jingweno/ccat 



//ME depending 不工作
ipmitool -P admin -U admin -H 10.99.104.40 -b 6 -t 0x2c raw 0x2e 0xdf 0x57 1 0 1

关闭代理指令：
unset http_proxy
unset https_proxy

//打印程序行号文件
printf("%s(%d)-%s MT------TEST!\n",__FILE__,__LINE__,__FUNCTION__); 
printf("%s,read temp421 fail,line=%d\n",__FUNCTION__,__LINE__);


http-》https （家玲）

pdk_packages/spx_restservice-3.25.0.9.0-src/data/lighttpd.conf
-server.modules              = ("mod_fastcgi", "mod_setenv")
+server.modules              = ("mod_fastcgi", "mod_setenv", "mod_redirect") 
最后加（尾行）
+$HTTP["scheme"] == "http" {
+    $HTTP["url"] !~ "^/api|^/redfish|^/wsman|^/fonts|^/Java" {
+        $HTTP["host"] =~ ".*" {
+            url.redirect = (".*" => "HYPERLINK "https://%250$0/"https://%0$0")
+        }
+    }
+}  





Horsea-a 开机函数
iec_power_on();



项目如何开启NCSI功能
1: PRJ file勾选:
 device configuration -> NCSI Configuration Support 
device configuration -> SideBand_LAN(NCSI Device Support) 
Runtime Libraries Configuration -> NC-SI Libaray Support
AST2500EVB Configuration -> Number of Network Interfaces = 2
2:PMC文件修改：
IPMI.conf ：MAX_LAN_CHANNEL=2  (支持的interface数量)  


关于NCSI的调试：
在BMC中/proc/sys/ractrends/ncsi 下有驱动对应的接口
指令：可以通过向UserCommand文件写指令，在UserLastResponse中得到response



BIOS版本读取：
BIOS_FW_VERSION_OFFSET      =   0x1300,
BIOS_VERSION_LEN            =   32,
#define MB_FRU_BUS_NUM              3
#define MB_FRU_SLAVE_ADDR           0xA6



JavaScript弹出窗口 打印log
alert("Log collection not completed or started");
console 窗口log
console.log("Sample log");



Fearow项目curl升级
curl -X POST -d "username=ADMIN&password=ADMIN" "http://192.168.0.49/api/session" -c ./cookie  

curl -F "fwimage=@./tool/061HLSB10230B1.ima" -H "X-CSRFTOKEN:KUbtXqQv" "http://10.32.111.81/baidu/maintenance/BMC/firmware" -b ./cookie


ME指令对应的文件
pnm.c中


BMC和PCH间的USB   /conf/vmedia.conf中的 cdrom_mode置1


MD5 加密指令
~ » openssl passwd -1 '123456'                                                                                                                         morton@morton
$1$USjjA/T4$cSX61mR7czK8idCodRiWe/


prochot#表示CPU有点热啊。要降频。加快风扇转速
THERMTRIP#表示热的受不了啦。必须断电了


BMC中常用路径：
主：
/conf
/var
/etc/init.d --->一些可执行的工具
辅：
/bin
/usr/local/bin
/etc

PMbus：
Packet Error Checking (PEC)


MOC卡测试：
i2c-test -b 13 -s 0x71 （sddr：0xe2）-m 1 -w -d 0x04  （切刀slot 3上） 
i2c-test -b 13 -s 0x10（addr：0x20） -m 1 -rc 1 -d  0x00  （读取温度）

i2c-test -b 13 -s 0x71 -m 1 -w -d 0x04  
i2c-test -b 13 -s 0x10 -m 1 -rc 1 -d  0x00  




auditlog：（horse-a code）
在iec_auditlog.c中添加数组信息。
在OEM_RecordAuditLog 截取msghand中的指令



linux 对寄存器的读写
读：0x1e784000
devmem 0x1e784000 
写：向0x1e784000 写入0x00000000
devmem 0x1e784000 32 0x00000000

devmem 0x1e6e2080 32
0x00000002

devmem 0x1e6e208c 32

devmem 0x1e6e2070 32


hi Eva
Please confirm whether "ME" obtains power consumption from BMC or from PSU. BMC confirms that "ME" does not obtain power from BMC through command
