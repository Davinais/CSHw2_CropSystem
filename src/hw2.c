#include <stdio.h>
#include <stdbool.h>
#include "termctrl.h"
#include "stringutil.h"
#include "linkedlist.h"
#define TERM_LEN 80
#define HLINE_LEN 40

typedef struct Node Crop;

void printHline()
{
    char printbuf[TERM_LEN*3];
    printf("%s", strcenter(printbuf, "════════════════════════════════════════", HLINE_LEN, TERM_LEN));
}
void printMenu()
{
    char printbuf[TERM_LEN*3];
    printf("%s", strcenter(printbuf, "[1] 　檢視作物清單", 18, TERM_LEN));
    printf("%s", strcenter(printbuf, "[2] 　新增作物　　", 18, TERM_LEN));
    printf("%s", strcenter(printbuf, "[3] 　刪除作物　　", 18, TERM_LEN));
    printf("%s", strcenter(printbuf, "[4] 　清空作物清單", 18, TERM_LEN));
    printf("%s", strcenter(printbuf, "[5] 　作物推薦　　", 18, TERM_LEN));
    printf("%s", strcenter(printbuf, "[6] 　結束管理系統", 18, TERM_LEN));
}

char askOption(char* question, char* valid)
{
    bool err = false;
    char ch = '\0';
    short INDENT = (TERM_LEN-HLINE_LEN) >> 1;
    do
    {
        printf("%*s%s", INDENT, "", question);
        ch = getchar();
        if(ch == EOF || ch == '\n') //若為EOF或換行字元，不進flush以免再度停頓在getchar()
            err = true;
        else if(flush_in() > 0) //flush函式回傳值為不包括\n或EOF字元的被清除字元數，若大於0即代表為不合法輸入
            err = true;
        else if(!strchr(valid, ch)) //檢查是否在合法選項字元之內
            err = true;
        else
            err = false;
        if(err)
        {
            char errbuf[TERM_LEN*2];
            sprintf(errbuf, "%*s%s", INDENT, "", "錯誤的輸入，請重新輸入！\n");
            printcolor(errbuf, RED);
        }
    }while(err);
    return ch;
}

int main()
{
    List croplist = {0, ID_M, INCREASING, NULL};
    char printbuf[TERM_LEN*3];
    short INDENT = (TERM_LEN-HLINE_LEN) >> 1;
    printf("%s", strcenter(printbuf, "歡迎使用作物管理系統", 20, TERM_LEN));
    printHline();
    printMenu();
    printHline();
    char func = askOption("請選擇功能[1-6]：", "123456");
    switch(func)
    {
        case '1':
            printList(&croplist);
            break;
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        default:
            break;
    }
}
