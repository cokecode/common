README for IP-range Utilities
=====================================

@2013/06/05 22:51:40

这一组小程序用来实现解析IP段、在IP段基础上产生随机IP的功能，

* merge-ip-range.c
  合并IP段。比如下面两个IP段，
    - (192.168.1.0, 192.168.1.255)
    - (192.168.2.0, 192.168.2.255)<br>
  可以合并成，
    - (192.168.1.0, 192.168.2.255)<br>
  合并之后IP段的数量将会少很多。合并之前应该先排序，这是显然的。

* analyze-ip-range.c  
  解析各种IP段，并产生 x.x.x.x/netmask 这种形式。

* generate-all-ip-from-range.c  
  从IP段（人眼可读，而非netmask形式）产生出这些IP段内的所有IP。

* generate-random-ip.c  
  从IP段产生随机IP，可指定要产生的IP数量。

* print-ip.c
  以十进制、十六进制、二进制打印出一个IP地址。
  
  









