## 鸟叔的私房菜

* 1.查日期
      **date**
* 2.[Tab] 接在一串指令的第一个字后面，则为[命令补全]
   [Tab] 接在一串指令的第一个字后面，则为[命令补全]
* 3.将数据同步写入硬盘的指令 ：**sync**
* 4.惯用的关机指令 : **shutdown**
* 5.重新启动，关机:  **reboot** 、**halt**、**poweroff**
* 6.权限
  + r (read)       4
  + w(write)      2
  + x(execute)  1
  + owner  =  rwx = 4 + 2 + 1 = 7
  + group   = r-x = 4 + 1 = 5
  + others  = --x = 1
* 7. 创建文件夹 mkdir
* 8. 删除文件夹 rm -rf 文件夹名 （-f 强制删除）
* 9.删除文件 rm -f 文件名
* 10.删除一个空的文件夹 rmdir
* 11.显示当前文件夹 pwd
* 12.列出全部文件 ls -a
* 13.按时间排序列出文件 ls -t
* 14.查找文件: 
  + whereis (只搜索特定目录)
  +  locate 
  + find (全盘搜索)  find ./ -name "*.text *"

