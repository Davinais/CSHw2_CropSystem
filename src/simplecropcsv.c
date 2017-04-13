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
    char *dataname[] = {"ID", "TYPE", "PRICE", "DAY", "YIELD"};
    FILE *csvin;

    csvin = fopen(filename, "r");
    if(csvin == NULL)
    {
        char printbuf[240];
        sprintf(printbuf, "%*s%s", INDENT, "", "讀取失敗，請確認檔案是否存在。");
        printcolor(printbuf, RED);
        return;
    }
    char buffer[120];
    char* data;
    bool check = true;
    if(fgets(buffer, 120, csvin) != NULL)
    {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n')
            buffer[--len] = '\0';
        int i = 0;
        char *s = strdup(buffer);
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
        cleanList(list);
        char *crop[5];
        while(fgets(buffer, 120, csvin) != NULL)
        {
            size_t len = strlen(buffer);
            if (len > 0 && buffer[len-1] == '\n')
                buffer[--len] = '\0';
            int i = 0;
            char *s = strdup(buffer);
            for(data = strsep(&s, ","); data != NULL; data=strsep(&s, ","))
            {
                crop[i] = data;
                i++;
            }
            int id = atoi(crop[0]);
            char typeCh[1];
            strncpy(typeCh, crop[1], 1);
            int price = atoi(crop[2]), day = atoi(crop[3]), yield = atoi(crop[4]);
            if(searchID(list, id))
            {
                char errbuf[160];
                sprintf(errbuf, "%*sID代碼重複：[%d]，忽略此ID！\n", INDENT, "", id);
                printcolor(errbuf, RED);
            }
            else if(!strchr("BbFfVv", typeCh[0]) || strlen(crop[1]) != 1)
            {
                char errbuf[160];
                sprintf(errbuf, "%*s類型錯誤：[%d]，忽略此ID！\n", INDENT, "", id);
                printcolor(errbuf, RED);
            }
            else if(id < 1 || price < 1 || day < 1 || yield < 1)
            {
                char errbuf[160];
                sprintf(errbuf, "%*s代碼、售價、收成天數及產量皆需為正整數：[%d]，忽略此ID！\n", INDENT, "", id);
                printcolor(errbuf, RED);
            }
            else
            {
                Node *cropNode = (Node*)malloc(sizeof(Node));
                if(cropNode == NULL)
                {
                    printcolor("記憶體不足，離開程式。", RED);
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
