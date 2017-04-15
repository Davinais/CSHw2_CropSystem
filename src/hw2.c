#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include "termctrl.h"
#include "stringutil.h"
#include "linkedlist.h"
#include "simplecropcsv.h"
#define TERM_LEN 80
#define HLINE_LEN 40

short INDENT = (TERM_LEN-HLINE_LEN) >> 1;

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
    printf("%s", strcenter(printbuf, "[7] 　讀取外部清單", 18, TERM_LEN));
    printf("%s", strcenter(printbuf, "[8] 　儲存作物清單", 18, TERM_LEN));
}

char askOption(char* question, char* valid)
{
    bool err = false;
    char ch = '\0';
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

void showList(List* list)
{
    while(true)
    {
        printList(list);
        printHline();
        printf("%*s%s\n", INDENT, "", "[A] 　改變排列依據");
        printf("%*s%s\n", INDENT, "", "[B] 　回到主選單");
        char opt = askOption("請選擇功能[A-B]：", "AaBb");
        switch(opt)
        {
            case 'A':
            case 'a':
                printHline();
                printf("%*s%s\n", INDENT, "", "屬性代號：");
                printf("%*s%s\n", INDENT+2, "", "[N] 　代碼(ID)");
                printf("%*s%s\n", INDENT+2, "", "[T] 　類型");
                printf("%*s%s\n", INDENT+2, "", "[P] 　售價");
                printf("%*s%s\n", INDENT+2, "", "[D] 　收成天數");
                printf("%*s%s\n", INDENT+2, "", "[Y] 　產量");
                printf("%*s%s\n", INDENT, "", "排序方式：");
                printf("%*s%s\n", INDENT+2, "", "[I] 　由小到大");
                printf("%*s%s\n", INDENT+2, "", "[D] 　由大到小");
                bool err = false;
                char modec = '\0', orderc = '\0';
                do
                {
                    printf("%*s%s", INDENT, "", "請輸入格式([屬性]-[排序])：");
                    scanf("%1[^\n]-%c", &modec, &orderc); //使用 %1[^\n] 而非 %c 來避免讀到 \n 又只讀1個字元
                    if(orderc == EOF || orderc == '\n') //若讀取到EOF或換行字元，不進flush以免再度停頓在getchar()
                        err = true;
                    else if(flush_in() > 0) //flush函式回傳值為不包括\n或EOF字元的被清除字元數，若大於0即代表為不合法輸入
                        err = true;
                    else if(modec == '\0' || orderc == '\0') //都維持'\0'沒變的情形是特例，特別拉出來寫
                        err = true;
                    else if(!strchr("NnTtPpDdYy", modec))
                        err = true;
                    else if(!strchr("IiDd", orderc))
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
                SortMode mode;
                SortOrder order;
                printf("%*s%s", INDENT, "", "您選擇了依 ");
                switch(modec)
                {
                    case 'N':
                    case 'n':
                        mode = ID_M;
                        printcolor("代碼(ID)", YELLOW);
                        break;
                    case 'T':
                    case 't':
                        mode = TYPE_M;
                        printcolor("類型", YELLOW);
                        break;
                    case 'P':
                    case 'p':
                        mode = PRICE_M;
                        printcolor("售價", YELLOW);
                        break;
                    case 'D':
                    case 'd':
                        mode = DAY_M;
                        printcolor("收成天數", YELLOW);
                        break;
                    case 'Y':
                    case 'y':
                        mode = YIELD_M;
                        printcolor("產量", YELLOW);
                        break;
                }
                printf(" 以 ");
                switch(orderc)
                {
                    case 'I':
                    case 'i':
                        order = INCREASING;
                        printcolor("由小到大", GREEN);
                        break;
                    case 'D':
                    case 'd':
                        order = DECREASING;
                        printcolor("由大到小", GREEN);
                        break;
                }
                printf(" 方式排列\n");
                reassembleList(list, mode, order);
                break;
            case 'B':
            case 'b':
            default:
                return;
        }
    }
}

void addCrop(List* list)
{
    while(true)
    {
        printHline();
        printf("%*s%s\n", INDENT, "", "[A] 　新增作物");
        printf("%*s%s\n", INDENT, "", "[B] 　回到主選單");
        char opt = askOption("請選擇功能[A-B]：", "AaBb");
        switch(opt)
        {
            case 'A':
            case 'a':
                {
                    bool err = false;
                    printHline();
                    printf("%*s%s\n", INDENT, "", "類型代號：");
                    printf("%*s%s\n", INDENT+2, "", "[F] 　水果");
                    printf("%*s%s\n", INDENT+2, "", "[B] 　花卉");
                    printf("%*s%s\n", INDENT+2, "", "[V] 　蔬菜");
                    printf("%*s%s\n", INDENT, "", "代碼需為正整數，不可重複");
                    printf("%*s%s\n", INDENT, "", "售價需為正整數，單位：元/kg");
                    printf("%*s%s\n", INDENT, "", "收成天數需為正整數，單位：天");
                    printf("%*s%s\n\n", INDENT, "", "產量需為正整數，單位：kg");
                    char type = '\0';
                    int id = 0, price = 0, day = 0, yield = 0;
                    do
                    {
                        printf("%*s%s\n", INDENT, "", "格式：[代碼]-[類型]-[售價]-[收成天數]-[產量]");
                        printf("%*s%s", INDENT, "", "請依格式輸入新作物：");
                        scanf("%d-%1[FfBbVv]-%d-%d-%d", &id, &type, &price, &day, &yield); //以%1[FfBbVv]限制只能讀取這些字元，並且長度最大為1的字串
                        if(flush_in() > 0)
                        {
                            err = true;
                            char errbuf[TERM_LEN*2];
                            sprintf(errbuf, "%*s%s", INDENT, "", "輸入格式錯誤，請重新輸入！\n");
                            printcolor(errbuf, RED);
                        }
                        else if(id < 1 || price < 1 || day < 1 || yield < 1)
                        {
                            err = true;
                            char errbuf[TERM_LEN*2];
                            sprintf(errbuf, "%*s%s", INDENT, "", "代碼、售價、收成天數及產量皆需為正整數！\n");
                            printcolor(errbuf, RED);
                        }
                        //嚴格來說這個if大概是不會走到，因為會在前面的flush_in()就會>0，導致進入該區塊
                        else if(type == '\0')
                        {
                            err = true;
                            char errbuf[TERM_LEN*2];
                            sprintf(errbuf, "%*s%s", INDENT, "", "類型需為[F]水果 [B]花卉 [V]蔬菜 其中一種！\n");
                            printcolor(errbuf, RED);
                        }
                        else if(searchID(list, id))
                        {
                            err = true;
                            char errbuf[TERM_LEN*2];
                            sprintf(errbuf, "%*s%s", INDENT, "", "ID代碼不可重複！\n");
                            printcolor(errbuf, RED);
                        }
                        else
                            err = false;
                    }while(err);
                    Node *crop = (Node*)malloc(sizeof(Node));
                    if(crop == NULL)
                    {
                        printcolor("記憶體不足，離開程式。", RED);
                        exit(1);
                    }
                    else
                    {
                        crop->id = id;
                        crop->type = type;
                        crop->price = price;
                        crop->day = day;
                        crop->yield = yield;
                        crop->next = NULL;
                    }
                    insertNode(list, crop);
                    break;
                }
            case 'B':
            case 'b':
            default:
                return;
        }
    }
}

void deleteCrop(List* list)
{
    while(true)
    {
        printHline();
        printf("%*s%s\n", INDENT, "", "[A] 　刪除作物");
        printf("%*s%s\n", INDENT, "", "[B] 　回到主選單");
        char opt = askOption("請選擇功能[A-B]：", "AaBb");
        switch(opt)
        {
            case 'A':
            case 'a':
                printHline();
                bool err = false;
                int id = 0;
                do
                {
                    printf("%*s%s", INDENT, "", "請輸入欲刪除的代號：");
                    scanf("%d", &id);
                    if(flush_in() > 0) //flush函式回傳值為不包括\n或EOF字元的被清除字元數，若大於0即代表為不合法輸入
                        err = true;
                    else if(id < 1)
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
                if(!searchID(list, id))
                {
                    char buf[TERM_LEN*2];
                    sprintf(buf, "%*s%s", INDENT, "", "此代號不存在於清單中！\n");
                    printcolor(buf, YELLOW);
                }
                else
                {
                    deleteByID(list, id);
                    char buf[TERM_LEN*2];
                    sprintf(buf, "%*s%s", INDENT, "", "刪除成功！\n");
                    printcolor(buf, GREEN);
                }
                break;
            case 'B':
            case 'b':
            default:
                return;
        }
    }
}

void recommendCrop(List* list)
{
     while(true)
    {
        printHline();
        printf("%*s%s\n", INDENT, "", "[A] 　輸入需求");
        printf("%*s%s\n", INDENT, "", "[B] 　回到主選單");
        char opt = askOption("請選擇功能[A-B]：", "AaBb");
        switch(opt)
        {
            case 'A':
            case 'a':
                printHline();
                bool err = false;
                int day = 0;
                char type = '\0';
                printf("%*s%s\n", INDENT, "", "類型代號：");
                printf("%*s%s\n", INDENT+2, "", "[F] 　水果");
                printf("%*s%s\n", INDENT+2, "", "[B] 　花卉");
                printf("%*s%s\n", INDENT+2, "", "[V] 　蔬菜");
                do
                {
                    printf("%*s%s", INDENT, "", "請依[類型]-[天數]格輸入：");
                    scanf("%1[FfBbVv]-%d", &type, &day); //使用 %1[^\n] 而非 %c 來避免讀到 \n 又只讀1個字元
                    if(flush_in() > 0) //flush函式回傳值為不包括\n或EOF字元的被清除字元數，若大於0即代表為不合法輸入
                        err = true;
                    else if(day < 1)
                        err = true;
                    else if(type == '\0')
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
                if(list->head == NULL)
                {
                    char printbuf[TERM_LEN*2];
                    sprintf(printbuf, "%*s%s", INDENT, "", "列表為空，無法推薦作物！\n");
                    printcolor(printbuf, RED);
                }
                else
                {
                    Node *current = list->head;
                    Node *recommend = NULL;
                    //建立一個recommendList，如此一來若推薦作物有多個，便可以一起印出
                    List recommendList = {0, ID_M, INCREASING, NULL};
                    int recommendTotal = 0, currentTotal = 0;
                    while(current != NULL)
                    {
                        if(current->type == toupper(type))
                        {
                            currentTotal = current->yield*(day/current->day)*current->price;
                            if(currentTotal > 0)
                            {
                                if(recommend == NULL)
                                {
                                    recommend = current;
                                    recommendTotal = currentTotal;
                                    Node *temp = (Node*)malloc(sizeof(Node));
                                    if(temp == NULL)
                                    {
                                        printcolor("記憶體不足，離開程式。\n", RED);
                                        exit(1);
                                    }
                                    *temp = *recommend;
                                    temp->next = NULL;
                                    appendNode(&recommendList, temp);
                                }
                                else if(currentTotal > recommendTotal)
                                {
                                    recommend = current;
                                    recommendTotal = currentTotal;
                                    cleanList(&recommendList); //若是比之前的大，就清空之前的推薦清單重新開始
                                    Node *temp = (Node*)malloc(sizeof(Node));
                                    if(temp == NULL)
                                    {
                                        printcolor("記憶體不足，離開程式。\n", RED);
                                        exit(1);
                                    }
                                    *temp = *recommend;
                                    temp->next = NULL;
                                    appendNode(&recommendList, temp);
                                }
                                else if(currentTotal == recommendTotal)
                                {
                                    Node *temp = (Node*)malloc(sizeof(Node));
                                    if(temp == NULL)
                                    {
                                        printcolor("記憶體不足，離開程式。\n", RED);
                                        exit(1);
                                    }
                                    *temp = *current;
                                    temp->next = NULL;
                                    appendNode(&recommendList, temp);
                                }
                            }
                        }
                        current = current->next;
                    }
                    if(recommend == NULL)
                    {
                        char printbuf[TERM_LEN*2];
                        sprintf(printbuf, "%*s%s", INDENT, "", "列表中不存在符合條件的作物！\n");
                        printcolor(printbuf, GREEN);
                    }
                    else
                    {
                        //印出recommendList，之後清空清單以還回malloc出來的空間
                        char printbuf[TERM_LEN*2];
                        sprintf(printbuf, "%*s%s", INDENT, "", "以下是推薦種植的作物：\n");
                        printcolor(printbuf, GREEN);
                        printList(&recommendList);
                        cleanList(&recommendList);
                    }
                }
                break;
            case 'B':
            case 'b':
            default:
                return;
        }
    }
}

int main()
{
    List croplist = {0, ID_M, INCREASING, NULL};
    char printbuf[TERM_LEN*3];
    printf("%s", strcenter(printbuf, "歡迎使用作物管理系統", 20, TERM_LEN));
    while(true)
    {
        printHline();
        printMenu();
        printHline();
        char func = askOption("請選擇功能[1-8]：", "12345678");
        switch(func)
        {
            case '1':
                showList(&croplist);
                break;
            case '2':
                addCrop(&croplist);
                break;
            case '3':
                deleteCrop(&croplist);
                break;
            case '4':
                cleanList(&croplist);
                sprintf(printbuf, "%*s%s", INDENT, "", "成功清空清單！\n");
                printcolor(printbuf, GREEN);
                break;
            case '5':
                recommendCrop(&croplist);
                break;
            case '7':
                printHline();
                printf("%*s請輸入檔案名稱：", INDENT, "");
                scanf("%s", printbuf);
                flush_in();
                readCSV(printbuf, &croplist);
                break;
            case '8':
                printHline();
                printf("%*s請輸入檔案名稱：", INDENT, "");
                scanf("%s", printbuf);
                flush_in();
                writeCSV(printbuf, &croplist);
                break;
            case '6':
            default:
                sprintf(printbuf, "%*s%s", INDENT, "", "感謝使用本系統！\n");
                printcolor(printbuf, GREEN);
                return 0;
        }
    }
}
