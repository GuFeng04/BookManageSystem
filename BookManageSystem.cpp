#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include <time.h>


typedef struct book {
	int	bookNumber;					//图书编号
	char name[20];					//书名
	char author[20];					//作者
	char publishingHouse[20];			//出版社
	char yearOfPublication[20];			//出版年份
	float price;						//价格
	int    num;						//数量			
	int    remainNum;					//剩余数量
}bookInfo;

typedef struct Node
{
	bookInfo data;
	struct Node* next;
}LinkList;

//借阅者
typedef struct  borrower {
	char name[20];
	char bookName[20];
	int borrowingdate;					//借阅日期
	float overdueFine;					//逾期罚款
} borrowerInfo;

typedef struct borrowerNode {
	borrowerInfo data;
	struct borrowerNode* next;
}borrowerList;

//创建全局图书链表
LinkList* list = NULL;
borrowerList* bList = NULL;

//获得时间，从1970年到现在的秒数
int seconds = time((time_t*)NULL);

//创建表头
LinkList* createHead()
{
	LinkList* headNode = (LinkList*)malloc(sizeof(LinkList));
	headNode->next = NULL;
	return headNode;
}

//创建结点,通过图书编号，创建结点
LinkList* createNode(bookInfo data) {
	LinkList* newNode = (LinkList*)malloc(sizeof(LinkList));
	newNode->data = data;
	newNode->next = NULL;
	return newNode;
}

//头插法插入数据
void  insertNode(LinkList* headNode, bookInfo data) {
	LinkList* newNode = createNode(data);
	newNode->next = headNode->next;				//将Newnode指向headNode后一个结点
	headNode->next = newNode;					//再将headNode指向NewNode
}

//打印链表,该打印仅图书链表，借阅者链表不打印
void printList(LinkList* headNode) {
	LinkList* pMove = headNode->next;
	printf("图书信息：\n----------------------\n");
	while (pMove)
	{
		printf("图书编号：\t%d\n", pMove->data.bookNumber);
		printf("书名：\t\t%s\n", pMove->data.name);
		printf("作者：\t\t%s\n", pMove->data.author);
		printf("出版社：\t%s\n", pMove->data.publishingHouse);
		printf("出版年份：\t%s\n", pMove->data.yearOfPublication);
		printf("价格：\t\t%.1f\n", pMove->data.price);
		printf("数量：\t\t%d\n", pMove->data.num);
		printf("剩余数量：\t%d\n", pMove->data.remainNum);
		pMove = pMove->next;
	}
}

//删除链表，通过图书编号，删除图书信息
void deleteNode(LinkList* headNode, char* name, int bookNumber) {
	LinkList* posLeftNode = headNode;
	LinkList* posNode = headNode->next;
	//当posNode不为空且posNode不等于要被删除的结点时，往继续遍历
	//当name不为空时，按name删除，当bookNumber不为空时，按编号删除
	while (name != NULL && posNode != NULL && strcmp(posNode->data.name, name) || bookNumber != NULL
		&& posNode != NULL && posNode->data.bookNumber != bookNumber)
	{
		posLeftNode = posNode;
		posNode = posLeftNode->next;
	}
	//删除
	if (posNode == NULL) {
		printf("没有找到指定目标，删除失败！");
	}
	else {
		posLeftNode->next = posNode->next;
		free(posNode);		//释放空间
		posNode = NULL;		//置空
		printf("删除成功！\n");
	}
}
//写文件
void saveToFile(const char* filename, LinkList* headNode) {
	FILE* fp = fopen(filename, "w");
	LinkList* pMove = headNode->next;
	while (pMove != NULL) {		//当指针不指向空，即没有到链表尾的时候，写入
		fprintf(fp, "%d\t%s\t%s\t%s\t%s\t%.1f\t%d\t%d\n", pMove->data.bookNumber, pMove->data.name,
			pMove->data.author, pMove->data.publishingHouse, pMove->data.yearOfPublication, pMove->data.price,
			pMove->data.num,pMove->data.remainNum);
		pMove = pMove->next;
	}
	fclose(fp);
}

//读文件
void readFile(const char* filename, LinkList* headNode) {
	FILE* fp = fopen(filename, "r");
	if (fp == NULL) {
		//当第一次读取文件时，文件不存在，需要创建，w+可读可写
		fp = fopen(filename, "w+");
	}
	bookInfo tempData;			//定义临时指针，将数据写入链表
	while (fscanf(fp, "%d\t%s\t%s\t%s\t%s\t%f\t%d\t%d\n",
		&tempData.bookNumber, tempData.name, tempData.author, tempData.publishingHouse,
		tempData.yearOfPublication, &tempData.price, &tempData.num,&tempData.remainNum) != EOF) {
		insertNode(list, tempData);
	}
	fclose(fp);
}
//冒泡排序,输入1按编号排序，输入2按价格排序
void bubbleSortList(LinkList* headNode, int choose) {
	bookInfo  tempData;
	for (LinkList* p = headNode->next;p != NULL;p = p->next) {
		for (LinkList* q = headNode->next;q->next != NULL;q = q->next) {
			if (choose == 1) {
				if (q->data.bookNumber > q->next->data.bookNumber) {//升序
					tempData = q->data;
					q->data = q->next->data;
					q->next->data = tempData;
				}
			}
			else if (choose == 2) {
				if (q->data.price > q->next->data.price) {
					tempData = q->data;
					q->data = q->next->data;
					q->next->data = tempData;
				}
			}
			else
				printf("ERROR INPUT!");
		}
	}
	printList(headNode);			//打印排序完的链表
}

//查找，类似删除，通过NULL实现按编号查找或按书名查找
LinkList* searchList(LinkList* headNode, char* name, int bookNumber) {
	LinkList* posNode = headNode->next;
	while (name != NULL && posNode != NULL && strcmp(posNode->data.name, name) || bookNumber != NULL
		&& posNode != NULL && posNode->data.bookNumber != bookNumber)//strcmp相等时返回0，退出循环，否则接着遍历
	{
		posNode = posNode->next;
	}
	return posNode;				//返回空，没有找到，非空，找到
}

//以下内容为借阅者的链表
//创建借阅者表头
borrowerList* BLcreateHead() {
	borrowerList* headNode = (borrowerList*)malloc(sizeof(borrowerList));
	headNode->next = NULL;
	return headNode;
}
//创建结点,
borrowerList* BLcreateNode(borrowerInfo data) {
	borrowerList* newNode = (borrowerList*)malloc(sizeof(borrowerList));
	newNode->data = data;
	newNode->next = NULL;
	return newNode;
}

//头插法插入数据
void  BLinsertNode(borrowerList* headNode, borrowerInfo data) {
	borrowerList* newNode = BLcreateNode(data);
	newNode->next = headNode->next;				//将Newnode指向headNode后一个结点
	headNode->next = newNode;					//再将headNode指向NewNode
}

//查找,按名字查找
borrowerList* BLsearchList(borrowerList* headNode, char* name) {
	borrowerList* posNode = headNode;
	while (posNode != NULL && strcmp(posNode->data.name, name) )
	{
		posNode = posNode->next;
	}
	return posNode;				//返回空，没有找到，非空，找到
}

//传位置指针和当前的时间，判断时候逾期,超过50天逾期
 int Overdue(borrowerList* headNode, char* name,int seconds) {
	 borrowerList* posNode = NULL;
	 int fine=0;
	 posNode = BLsearchList(bList, name);
	//测试,立马超时
	int overtime = seconds - (posNode->data.borrowingdate);//计算已经超期的时间
	//int overtime = seconds - (posNode->data.borrowingdate) - 50 * 24 * 60 * 60;//计算已经超期的时间
	if (overtime > 0 ) {
		//已逾期，计算罚金。每天1块
		//fine += overtime / 60 / 60 / 24 * 1;
		fine += overtime;					//测试立马逾期
	}
	return fine;
}
 //写文件
 void BLsaveToFile(const char* filename, borrowerList* headNode) {
	 FILE* fp = fopen(filename, "w");
	 borrowerList* pMove = headNode->next;
	 while (pMove != NULL) {		//当指针不指向空，即没有到链表尾的时候，写入
		 fprintf(fp, "%s\t%s\t%d\t%.1f\n", pMove->data.name,pMove->data.bookName,pMove->data.borrowingdate,pMove->data.overdueFine);
		 pMove = pMove->next;
	 }
	 fclose(fp);
 }

 //读文件
 void BLreadFile(const char* filename, borrowerList* headNode) {
	 FILE* fp = fopen(filename, "r");
	 if (fp == NULL) {
		 //当第一次读取文件时，文件不存在，需要创建，w+可读可写
		 fp = fopen(filename, "w+");
	 }
	borrowerInfo tempData;			//定义临时指针，将数据写入链表
	 while (fscanf(fp, "%s\t%s\t%d\t%f\n",
		 tempData.name,tempData.bookName,&tempData.borrowingdate,&tempData.overdueFine) != EOF) {
		 BLinsertNode(bList, tempData);
	 }
	 fclose(fp);
 }
 //删除借阅者链表,通过图书名
 void BLdeleteNode(borrowerList* headNode, char* bookName) {
	 borrowerList* posLeftNode = headNode;
	 borrowerList* posNode = headNode->next;
	 //当posNode不为空且posNode不等于要被删除的结点时，往继续遍历
	 while ( posNode != NULL && strcmp(posNode->data.bookName, bookName) )
	 {
		 posLeftNode = posNode;
		 posNode = posLeftNode->next;
	 }
	 //删除
	 if (posNode == NULL) {
		 printf("没有找到指定目标，删除失败！");
	 }
	 else {
		 posLeftNode->next = posNode->next;
		 free(posNode);		//释放空间
		 posNode = NULL;		//置空
		 printf("归还成功！已从借阅者表中删除！\n");
	 }
 }
//菜单界面
void Menu() {
	printf("-----------------------------------\n");
	printf("\t图书管理系统\n");
	printf("\t0.退出\n");
	printf("\t1.登记图书\n");
	printf("\t2.查询图书\n");
	printf("\t3.借阅图书\n");
	printf("\t4.归还图书\n");
	printf("\t5.删除图书\n");
	printf("\t6.批量入库\n");
	printf("\t7.图书排序\n");
	printf("\t8.罚金计算\n");
	printf("\t9.保存\n");
	printf("-----------------------------------\n");
	printf("请输入选项：");
}


//菜单按键跳转
void keyjump() {
	int userkey = 0;				//临时变量，用于保存用户的选项
	bookInfo tempBook;			//临时变量，用于保存输入的书籍信息
	borrowerInfo tempborrower;		//同上，借阅者
	scanf("%d", &userkey);			
	int choose = 0;				//临时变量，用来保存排序时所输入的选项
	LinkList* tempNode = NULL;		//临时指针，用于保存函数返回的指针
	borrowerList* tempBLNode = NULL;
	int fine;
	switch (userkey)
	{
	case 0:
		printf("【  退出  】\n");
		printf("退出成功！");
		exit(0);					//关闭程序
		system("puase");
		break;
	case 1:
		printf("【 登记图书 】\n");
		printf("输入图书信息(空格隔开)：\n图书编号,书名,作者,出版社,出版年份,价格\n");
		scanf("%d %s %s %s %s %2f",
			&tempBook.bookNumber, tempBook.name, tempBook.author, tempBook.publishingHouse,
			tempBook.yearOfPublication, &tempBook.price);
		//查询库中是否有同名书籍，如果有，数量+1
		tempNode = searchList(list, tempBook.name, NULL);
		if (tempNode != NULL) {
			tempNode->data.num += 1;
			tempNode->data.remainNum += 1;
		}
		else {//如果不是同名书籍，那将这条数据插入链表,数量和剩余数量置1
			tempBook.num = 1;
			tempBook.remainNum = 1;
			insertNode(list, tempBook);
		}
		saveToFile("bookinfo.txt", list);
		break;
	case 2:
		printf("【 查询图书 】\n");
		printf("请输入查找的书籍，输入1按编号查找，输入2按书名查找：");
		scanf("%d",&choose);
		if (choose == 1) {
			printf("请输入查找书籍的编号：");
			scanf("%d", &tempBook.bookNumber);
			tempNode = searchList(list, NULL, tempBook.bookNumber);
		}
		else if (choose == 2) {
			printf("请输入查找书籍的名称：");
			scanf("%s", tempBook.name);
			tempNode = searchList(list, tempBook.name, NULL);
		}
		else
			printf("ERROR INPUT");
		if (tempNode == NULL) {
			printf("查询失败！查询目标不在文档中！");
		}
		else {
			printf("查询成功！\n");//由于没有在printList方法中增加额外的参数控制单个打印，所以手动打印
			printf("图书编号：\t%d\n", tempNode->data.bookNumber);
			printf("书名：\t\t%s\n", tempNode->data.name);
			printf("作者：\t\t%s\n", tempNode->data.author);
			printf("出版社：\t%s\n", tempNode->data.publishingHouse);
			printf("出版年份：\t%s\n", tempNode->data.yearOfPublication);
			printf("价格：\t\t%.1f\n", tempNode->data.price);
		}
		break;
	case 3:
		 //借阅功能，先查询是否存在，然后判断是否有库存。
		printf("【 借阅图书 】\n");
		printf("请输入你的姓名：");
		scanf("%s", tempborrower.name);
		printf("请输入借阅的书名：");
		scanf("%s", tempBook.name);
		tempNode = searchList(list, tempBook.name, NULL);
		if (tempNode != NULL) {
			if (tempNode->data.remainNum > 0) {
				tempNode->data.remainNum--;
				strcpy(tempborrower.bookName ,tempNode->data.name);
				tempborrower.borrowingdate = seconds;
				tempborrower.overdueFine = 0;
				printf("借阅成功，当前书籍还剩%d本。\n", tempNode->data.remainNum);
			}
			else {
				printf("借阅失败！当前书籍已被借光,当前剩余%d本\n", tempNode->data.remainNum);
			}
		}
		else {
			printf("库中没有相关图书！");
		}
		BLinsertNode(bList, tempborrower);
		BLsaveToFile("borrowerinfo.txt", bList);
		break;
	case 4:
		printf("【 归还图书 】\n");
		printf("请输入归还的书名：");
		scanf("%s", tempBook.name);
		tempNode = searchList(list, tempBook.name, NULL);
		if (tempNode != NULL) {
			printf("成功查询到该书籍，当前剩余%d本\n", tempNode->data.remainNum);
			printf("归还中....\n");
			tempNode->data.remainNum++;
			printf("归还成功，当前书籍剩余%d本。\n", tempNode->data.remainNum);
			BLdeleteNode(bList,tempBook.name);			//归还成功后，从借阅者链表中删除信息
		}
		else {
			printf("库中没有此类图书！");
		}
		BLsaveToFile("borrowerinfo.txt",bList);
		break;
	case 5:
		printf("【 删除图书 】\n");
		printf("输入1按编号删除，输入2按书名删除：\n");
		scanf("%d", &choose);
		if (choose == 1) {
			printf("请输入删除书籍的编号：");
			scanf("%d",&tempBook.bookNumber);
			deleteNode(list,NULL,tempBook.bookNumber);
			saveToFile("bookinfo.txt", list);
		}
		else if (choose == 2) {
			printf("请输入删除书籍的名称：");
			scanf("%s", tempBook.name);
			deleteNode(list, tempBook.name,NULL);
			saveToFile("bookinfo.txt", list);
		}
		else
			printf("ERROR INPUT");
		break;
	case 6:
		printf("【批量入库】\n");
		printf("请输入要入库的图书名：\n");
		scanf("%s", tempBook.name);
		tempNode = searchList(list, tempBook.name, NULL);
		if (tempNode == NULL) {
			printf("ERROR!\n");
		}
		else {
			printf("%s目前的数量是：%d\n请输入该图书新增的数量\n输入整数增加，输入负数表示批量删除\n", tempNode->data.name, tempNode->data.num);
			scanf("%d", &tempBook.num);//这里使用choose,省下定义一个新的临时变量
			tempNode->data.num = tempNode->data.num + tempBook.num;//计算数量，如果是负的直接减了
			printf("修改完毕！当前的数量为%d\n", tempNode->data.num);
		}
		saveToFile("bookinfo.txt", list);
		break;
	case 7:
		printf("【 图书排序 】\n");
		//冒泡排序，1通过编号,2通过价格
		printf("输入1按编号排序，输入2按价格排序：\n");
		scanf("%d",&choose);
		bubbleSortList(list, choose);
		break;
	case 8:
		printf("【 罚金计算 】\n");
		printf("请输入要查询的借阅者：\n");
		scanf("%s", tempborrower.name);
		tempBLNode = BLsearchList(bList, tempborrower.name);//先查询有没有这个人
		if (tempBLNode != NULL) {
			printf("%s当前的罚金计算为：\n", tempBLNode->data.name);//用tempBLNode方法来调用姓名，说明查找方法没有问题。
			fine = Overdue(bList, tempBLNode->data.name, seconds);
			if (fine > 0) {
				printf("当前罚金：%d\n", fine);
				tempBLNode->data.overdueFine = fine;
			}
			else {
				printf("没有逾期书籍，信誉良好！\n");
			}
		}
		else {
			printf("查询失败！\n");
		}
		break;
	case 9:
		printf("【 保存 】\n");
		printf("保存成功！\n");
		saveToFile("bookinfo.txt", list);
		BLsaveToFile("borrowerinfo.txt", bList);
		break;
	default:
		printf("【 ERROR！ 】\n");
		break;
	}
}

int main() {
	list = createHead();
	bList = BLcreateHead();
	readFile("bookinfo.txt", list);
	BLreadFile("borrowerinfo.txt", bList);
	while (1) {
		Menu();
		keyjump();
		system("pause");				//暂停
		system("cls");					//清屏
	}
	system("pause");
	return 0;
}