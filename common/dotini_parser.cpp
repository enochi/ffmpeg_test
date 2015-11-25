/**
 * @author XiaKQ
 * @version 0.1
 * @brief ini operation
 */

#include "system_port.h"
#include "dotini_parser.h"
#include <ace/OS_NS_stdio.h>

typedef struct dotini_kvpair
{
    char key[64];
    char value[64];
    struct dotini_kvpair *next;
} dotini_kvpair_t;

typedef struct dotini_section
{
    char name[64];
    dotini_kvpair_t *kvpair;
    struct dotini_section *next;
} dotini_section_t;

dotini_section_t *g_section_head = NULL;

int  dotini_open(const char *file)
{
    int  lineno = 0;
    char *p = NULL;
    char *token = NULL;
    dotini_section_t *current_section = NULL;
    char buf[128];
    FILE *fp = NULL;

    if (file == NULL)
        return -1;

    fp = fopen(file, "r");
    if (fp == NULL)
    {
        fprintf(stderr, "dotini_open()::fopen(\"%s\", ...) error: %s \n", file, (const char *)strerror(errno));
        return -1;
    }

    memset(buf, 0, sizeof(buf));
    while (fgets(buf, sizeof(buf), fp) != NULL)
    {
        lineno++;
        p = buf;

        while (*p == ' ' || *p == '\t')
            p++;

        if (*p == '#' || *p == ';' || *p == '\r' || *p == '\n')
            continue;

        if (*p == '[')
        {
            token = strtok(p, " \t\n\r");
            if(token)
            {
                current_section = (dotini_section_t *)malloc(sizeof(dotini_section_t));
                if (current_section == NULL)
                {
                    fprintf(stderr, "dotini_open()::malloc(%d) error: %s\n", sizeof(dotini_section_t), (char *)strerror(errno));
                    dotini_close();
                    fclose(fp);
                    return -1;
                }

                if (token[strlen(token) - 1] != ']')
                {
                    fprintf(stderr, "dotini_open() error: 缺少']', 文件%s, 第%d行\n", file, lineno);
                    dotini_close();
                    fclose(fp);

                    if(current_section)
                    {
                        free(current_section);
                        current_section = 0;
                    }
                    return -1;
                }

                memset(current_section, 0, sizeof(dotini_section_t));
                token[strlen(token) - 1] = '\0';
                token++;
                strncpy(current_section->name, token, sizeof(current_section->name));

                current_section->next = g_section_head;
                g_section_head = current_section;
            }

            token = strtok(NULL, " \t\n\r");
            if (token != NULL)
                ACE_OS::fprintf(stderr, "dotini_open() warning: ']'后有多余的字符, 文件%s, 第%d行\n", file, lineno);
        }
        else
        {
            dotini_kvpair_t *current_kvpair = (dotini_kvpair_t *)malloc(sizeof(dotini_kvpair_t));
            if (current_kvpair == NULL)
            {
                fprintf(stderr, "dotini_open()::malloc(%d) error: %s\n", sizeof(dotini_kvpair_t), (char *)strerror(errno));
                dotini_close();
                fclose(fp);

                if(current_section)
                {
                    free(current_section);
                    current_section = 0;
                }
                return -1;
            }
            memset(current_kvpair, 0, sizeof(dotini_kvpair_t));

            if (g_section_head == NULL)
            {
                ACE_OS::fprintf(stderr, "dotini_open() error: 不合语法的开始, 缺少'[', 文件%s, 第%d行\n", file, lineno);
                dotini_close();
                fclose(fp);

                if(current_kvpair)
                {
                    free(current_kvpair);
                    current_kvpair = 0;
                }
                if(current_section)
                {
                    free(current_section);
                    current_section = 0;
                }
                return -1;
            }

            token = strchr(p, '=');
            if (token == NULL)
            {
                ACE_OS::fprintf(stderr, "dotini_open() error: 此行缺少'=', 文件%s, 第%d行\n", file, lineno);
                dotini_close();
                fclose(fp);

                if(current_kvpair)
                {
                    free(current_kvpair);
                    current_kvpair = 0;
                }
                if(current_section)
                {
                    free(current_section);
                    current_section = 0;
                }
                return -1;
            }

            if (*p == '=')
            {
                ACE_OS::fprintf(stderr, "dotini_open() error: '='前缺少key字符串, 文件%s, 第%d行\n", file, lineno);
                dotini_close();
                fclose(fp);

                if(current_kvpair)
                {
                    free(current_kvpair);
                    current_kvpair = 0;
                }
                if(current_section)
                {
                    free(current_section);
                    current_section = 0;
                }
                return -1;
            }

            token = strtok(p, " \t\r\n=");

            if(token)
                strncpy(current_kvpair->key, token, sizeof(current_kvpair->key));

            current_kvpair->next = g_section_head->kvpair;
            g_section_head->kvpair = current_kvpair;

            //FIXME: value字符串中可能含有' '和'='
            token = strtok(NULL, " \t\r\n=");
            if (token == NULL)
                ACE_OS::fprintf(stderr, "dotini_open() warning: '='后缺少value字符串, 文件%s, 第%d行\n", file, lineno);
            else
                strncpy(current_kvpair->value, token, sizeof(current_kvpair->key));
        }
    }

    fclose(fp);
    return 0;
}

void dotini_close()
{
    dotini_section_t *sc  = NULL;
    dotini_section_t *nsc = NULL;
    dotini_kvpair_t  *kv  = NULL;
    dotini_kvpair_t  *nkv = NULL;

    sc = g_section_head;
    while (sc != NULL)
    {
        kv = sc->kvpair;
        while (kv != NULL)
        {
            nkv = kv->next;
            free(kv);
            kv = nkv;
        }

        nsc = sc->next;
        free(sc);
        sc = nsc;
    }

    g_section_head = NULL;
    return;
}

char *dotini_get_string(const char *section, const char *key)
{
    dotini_section_t *sc = NULL;
    dotini_kvpair_t  *kv = NULL;

    if (section == NULL || key == NULL)
        return NULL;

    for (sc = g_section_head; sc != NULL; sc = sc->next)
    {
        if (strcmp(sc->name, section) == 0)
        {
            for (kv = sc->kvpair; kv != NULL; kv = kv->next)
            {
                if (strcmp(kv->key, key) == 0)
                    return kv->value;
            }
        }
    }

    return NULL;
}

int dotini_get_integer(const char *section, const char *key)
{
    char *s = dotini_get_string(section, key);

    if (s == NULL)
        return 0;

    return atoi(s);
}

double dotini_get_double(const char *section, const char *key)
{
    char *s = dotini_get_string(section, key);

    if (s == NULL)
        return 0.0;

    return atof(s);
}

//zq+ 设置配置文件参数
int dotini_set_string(const char *section, const char *key, const char *key_value)
{
    dotini_section_t *sc = NULL;
    dotini_kvpair_t  *kv = NULL;

    if (section == NULL || key == NULL || NULL == key_value)
        return -1;

    for (sc = g_section_head; sc != NULL; sc = sc->next)
    {
        if (strcmp(sc->name, section) == 0)
        {
            for (kv = sc->kvpair; kv != NULL; kv = kv->next)
            {
                if (strcmp(kv->key, key) == 0)
                {
                    strncpy(kv->value,key_value,63);
                    return 0;
                }
            }
        }
    }

    return -1;
}

int dotini_save(const char *file)
{
    if (file == NULL)
        return -1;

    FILE *fp = fopen(file, "w");
    if (fp == NULL)
    {
        fprintf(stderr, "dotini_open()::fopen(\"%s\", ...) error: %s \n", file, (const char *)strerror(errno));
        return -1;
    }

    dotini_section_t *sc = NULL;
    dotini_kvpair_t  *kv = NULL;

    //逆序
    /*
    dotini_section_t *pPre = NULL;
    dotini_section_t *pNode = g_section_head->next;
    while(pNode)
    {
        dotini_section_t *pTemp = pNode->next;
        pNode->next = pPre;
        pPre = pNode;
        pNode = pTemp;
    }
    g_section_head->next = pPre;
    */

    char wbuf[256] = {0};
    for (sc = g_section_head; sc != NULL; sc = sc->next)
    {
        sprintf(wbuf,"[%s]\n",sc->name);
        fwrite(wbuf,1,strlen(wbuf)*sizeof(char),fp);

        for (kv = sc->kvpair; kv != NULL; kv = kv->next)
        {
            sprintf(wbuf,"%s=%s\n",kv->key,kv->value);
            fwrite(wbuf,1,strlen(wbuf)*sizeof(char),fp);
        }
    }

    fclose(fp);
    return 0;
}
//zq-
