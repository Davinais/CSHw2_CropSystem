#define _GNU_SOURCE 1
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include "simplecropcsv.h"
#include "linkedlist.h"
#include "termctrl.h"
#define INDENT 20

void readCSV(char* filename, List* list)
{
    char *dataname[] = {"ID", "TYPE", "PRICE", "DAY", "YIELD"}; //欄位名稱
    FILE *csvin;

    csvin = fopen(filename, "r");
    //確認檔案是否有開啟
    if(csvin == NULL)
    {
        char printbuf[240];
        sprintf(printbuf, "%*s%s", INDENT, "", "讀取失敗，請確認檔案是否存在。\n");
        printcolor(printbuf, RED);
        return;
    }
    char buffer[120];
    char* data;
    bool check = true;
    //檢查第一行是否符合欄位名稱順序
    if(fgets(buffer, 120, csvin) != NULL)
    {
        //去除行尾的\n
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n')
            buffer[--len] = '\0';
        int i = 0;
        char *s = buffer;
        for(data = strsep(&s, ","); data != NULL; data=strsep(&s, ","))
        {
            if(i >= 5)
            {
                check = false;
                break;
            }
            if(strcmp(data, dataname[i]) != 0)
            {
                check = false;
                break;
            }
            i++;
        }
    }
    if(check)
    {
        int line = 1;
        cleanList(list);
        char *crop[5];
        //每次讀取一行資料
        while(fgets(buffer, 120, csvin) != NULL)
        {
            line++;
            //去除行尾的\n
            size_t len = strlen(buffer);
            if (len > 0 && buffer[len-1] == '\n')
                buffer[--len] = '\0';
            int i = 0;
            char *s = buffer;
            //以逗號分隔字串
            for(data = strsep(&s, ","); data != NULL; data=strsep(&s, ","))
            {
                crop[i] = data;
                i++;
            }
            //將各個資料轉成相應的類型並檢查是否有效
            int id = atoi(crop[0]);
            char typeCh[1];
            strncpy(typeCh, crop[1], 1); //char*字串常數 -> char字元
            int price = atoi(crop[2]), day = atoi(crop[3]), yield = atoi(crop[4]);
            if(searchID(list, id))
            {
                char errbuf[160];
                sprintf(errbuf, "%*s[第%d行]ID代碼重複，忽略此行！\n", INDENT, "", line);
                printcolor(errbuf, RED);
            }
            else if(!strchr("BbFfVv", typeCh[0]) || strlen(crop[1]) != 1)
            {
                char errbuf[160];
                sprintf(errbuf, "%*s[第%d行]類型錯誤，忽略此行！\n", INDENT, "", line);
                printcolor(errbuf, RED);
            }
            else if(id < 1 || price < 1 || day < 1 || yield < 1)
            {
                char errbuf[160];
                sprintf(errbuf, "%*s[第%d行]代碼、售價、收成天數及產量皆需為正整數，忽略此行！\n", INDENT, "", line);
                printcolor(errbuf, RED);
            }
            else
            {
                Node *cropNode = (Node*)malloc(sizeof(Node));
                if(cropNode == NULL)
                {
                    printcolor("記憶體不足，離開程式。\n", RED);
                    exit(1);
                }
                else
                {
                    cropNode->id = id;
                    cropNode->type = typeCh[0];
                    cropNode->price = price;
                    cropNode->day = day;
                    cropNode->yield = yield;
                    cropNode->next = NULL;
                    insertNode(list, cropNode);
                }
            }
        }
        char buf[160];
        sprintf(buf, "%*s檔案讀取完成！\n", INDENT, "");
        printcolor(buf, GREEN);
    }
    else
    {
        char errbuf[160];
        sprintf(errbuf, "%*s讀取失敗：檔案格式錯誤！\n", INDENT, "");
        printcolor(errbuf, RED);
    }
    fclose(csvin);
}

void writeCSV(char* filename, List* list)
{
    char *dataname[] = {"ID", "TYPE", "PRICE", "DAY", "YIELD"};//欄位名稱
    FILE *csvout;

    csvout = fopen(filename, "w");
    //檢查能否成功建立或存取檔案
    if(csvout == NULL)
    {
        char printbuf[240];
        sprintf(printbuf, "%*s%s", INDENT, "", "存檔失敗，無法存取該目錄或者無法存取檔案。\n");
        printcolor(printbuf, RED);
        return;
    }
    //輸出欄位名稱
    fprintf(csvout, "%s,%s,%s,%s,%s\n", dataname[0], dataname[1], dataname[2], dataname[3], dataname[4]);
    Node* current = list->head;
    //輸出每行資料
    while(current != NULL)
    {
        fprintf(csvout, "%d,%c,%d,%d,%d\n", current->id, toupper(current->type), current->price, current->day, current->yield);
        current = current->next;
    }
    char buf[160];
    sprintf(buf, "%*s檔案儲存成功！\n", INDENT, "");
    printcolor(buf, GREEN);
    fclose(csvout);
}
