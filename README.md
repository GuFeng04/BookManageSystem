# 图书管理系统

使用C语言开发，为C语言课程设计。

在命令行界面实现图书管理系统。

![img](file:///C:\Users\古峰\AppData\Local\Temp\ksohtml13528\wps1.jpg)

## 实现功能：

1、登记图书，功能是将图书插入到图书馆的图书列表中。

2、删除图书，将图书从表中删除

3、查询图书，功能是在借阅前可以通过查询查看图书的基本信息。

4、借阅图书，实现了基本的借阅功能，并将借阅者的信息保存在借阅者列表中。

5、归还图书，实现了基本的归还功能，并将借阅者的信息从借阅者表中删除。

6、批量入库，实现了基本的修改图书数量的功能，使其更具有实用性。

7、图书排序，将图书进行排序，可以按照编号，或者按照价格。

8、罚金计算，逾期者将处以罚金，实现了简单的罚金功能。

9、保存与写入，将表保存在文件中，使其更具有实用性。

## 主要模块介绍与思路：

1、图书结构体book，其中保存了图书的各种信息。链表的表头指针储存在Node结构体中，通过这两个结构体实现基本表。

2、list链表，该链表中存储了图书的信息，是本项目中核心的储存结构。采用了链表的数据结构，并定义了一系列的方法来实现，比如创建表头、创建结点、插入结点。在插入结点时，使用了头插法插入。最后还实现了打印、删除、遍历等基本方法。

3、bList链表，该链表中存储了借阅者的信息，其原理与实现基本同list链表。通过这两个bList链表，实现了存储借阅者的信息，以实现借阅与还书功能。同时，该链表还会记录借阅的时间和归还的时间，来实现罚金计算的功能。

4、seconds变量，利用time.h包中的time方法，来获得现在到1970年的秒数。通过记录借阅时的秒数和归还时的秒数，来计算是否逾期。该解决方案极大的缩减了代码量，省下实现获取时间的方法。

5、菜单与按键跳转，利用switch语句实现不同的选项进入不同的功能，增加程序的使用便利。

6、文件的读写，利用FIFE对象将表写进文件中，实现保存的功能。同理，也实现了读取的功能。

项目结构：

![img](file:///C:\Users\古峰\AppData\Local\Temp\ksohtml1408\wps1.png)

功能一：退出。由exit函数实现，结束当前程序。

功能二：登记图书。

![img](file:///C:\Users\古峰\AppData\Local\Temp\ksohtml1408\wps2.png)

图2 登记图书流程图

如图所示，在登记图书功能中，输入图书的信息后，利用searchList方法返回是否存在这本书，如果存在，则不插入新的数据，将图书的数量和剩余数量+1。反之，通过insertNode方法将信息插入表中。

功能三：查询图书。

![img](file:///C:\Users\古峰\AppData\Local\Temp\ksohtml1408\wps3.png)

图3 查找图书流程图

如图所示，提供了两种查询的方法，一种利用图书号查询，一种利用图书名查询。通过输入的choose变量来控制选择。这两种查询集成在同一个方法中，在searchList方法中传入三个参数，list,name,number。通过传入NULL参数来实现控制。具体实现为：

while (name != NULL && posNode != NULL && strcmp(posNode->data.name, name) ||

 bookNumber != NULL&& posNode != NULL && posNode->data.bookNumber != bookNumber)

当strcmp相等时，返回0，结束循环。否则指针指向下一个结点，接着遍历。

当查询成功后，会打印图书的信息。

功能4：借阅图书。

![img](file:///C:\Users\古峰\AppData\Local\Temp\ksohtml1408\wps4.png)

图4 借阅图书流程图

 

如图所示，借阅功能将保存借阅者的姓名与借阅的书名，同时通过time方法返回的当前距1970年的秒数，来记录借阅时间。

在借阅时，通过searchList方法查询是否存在这本书，如果存在，判断是否还有剩余，如果还有剩余，那么借阅成功。

借阅成功后，将信息写入借阅者表bList中，将其保存在文件中。

功能5：归还图书。

![img](file:///C:\Users\古峰\AppData\Local\Temp\ksohtml1408\wps5.png)

图5 还书流程图

如图所示，输入所还书籍的名字就可以还书，通过searchList方法查询是否有这本换的书，如果有则还书成功。在成功还书后，利用BLdeleteNode方法删除借阅者表中的数据，实现还书目的。

功能6，删除，功能7排序，实现了简单的删除和排序，排序利用的冒泡排序。

功能8，批量入库，即修改，考虑到无法一次登记多个图书，在登记一本图书后，可以通过该功能修改图书的数量，实现登记多本图书。同时，输入一个负值，就可以实现图书的一定数量上的删除。而功能五的删除是直接删除全部的信息。

功能9，罚金计算。先通过BLsearchList方法找到这个人，然后通过Overdue方法计算罚金。在我们借阅时，会保存一个seconds变量，通过判断这个变量和现在时间的差距，就可以得出一个借阅了多长的时间。以此计算罚金。