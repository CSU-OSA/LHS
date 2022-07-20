//
// Created by OctAutumn on 2022/7/4.
//

#include "HttpRequest.h"

HttpRequest::HttpRequest(const char *httpMsg_Str)
{
    short parseStatus = 0;

    int p = 0;  //总 数据指针
    int p_in = 0;
    char lineBuffer[1024];
    while (httpMsg_Str[p] != '\0')
    {
        if (httpMsg_Str[p] == '\r')
        {
            if (p + 1 <= BUFSIZ && httpMsg_Str[p + 1] == '\n')
            {
                lineBuffer[p_in] = '\0';
                p += 2;

                // 读完一行请求报文，解析
                if (parseStatus == 0)
                {// 第一行请求
                    if (parseHttpHead_requestLine(lineBuffer) == -1)
                        throw HttpException("Exception when parsing first line.");
                    p_in = 0;
                    parseStatus = 1;
                } else if (parseStatus == 1)
                {
                    int ret = parseHttpHead_requestParams(lineBuffer);
                    if (ret == -1)
                    {
                        throw HttpException("Exception when parsing request params.");
                    } else if (ret == 1)
                        break;
                    p_in = 0;
                }
                continue;
            } else
            {
                throw HttpException("Exception when reading a line.");
            }
        }
        lineBuffer[p_in] = httpMsg_Str[p];
        p_in++;
        p++;
    }
    while (httpMsg_Str[p] != '\0')
    {
        context += httpMsg_Str[p];
        p++;
    }
}

/**
 * 解析HTTP请求头（请求行部分）
 * @param a_line 一行HTTP报文
 * @return 若解析成功则返回0，否则返回-1
 */
int HttpRequest::parseHttpHead_requestLine(const char *a_line)
{
    int p = 0;          //总指针
    char part_flag = 0; //请求参数标识

    //读method
    while (a_line[p] != '\0')
    {
        if (a_line[p] == ' ')   //发现空格，进入Url_Path读取
        {
            part_flag++;
            p++;
            break;
        }
        method += a_line[p];
        p++;
    }
    //读url_path
    while (a_line[p] != '\0')
    {
        if (a_line[p] == '?')   //发现'?'进入Url参数读取
        {
            part_flag++;
            p++;
            break;
        }
        if (a_line[p] == ' ')   //发现空格，进入HttpVersion读取
        {
            part_flag++;
            break;
        }
        URL_path += a_line[p];
        p++;
    }
    //读url_params
    std::string K, V;
    while (a_line[p] != '\0')   //逐字处理
    {
        if (a_line[p] == ' ')   //发现空格，进入HttpVersion读取
        {
            p++;
            break;
        }
        K.clear();
        V.clear();
        while (a_line[p] != '\0' && a_line[p] != '=')
        {
            K += a_line[p];
            p++;
        }
        p++;
        while (a_line[p] != '\0' && a_line[p] != '&' && a_line[p] != ' ')
        {
            V += a_line[p];
            p++;
        }
        if (K.empty() || V.empty()) return -1;
        URL_parameters.insert({K, V});
    }
    //读version
    while (a_line[p] != '\0')
    {
        if (a_line[p] == ' ')   //发现空格，报错
        {
            part_flag++;
            break;
        }
        version += a_line[p];
        p++;
    }

    if (part_flag != 2) return -1;

    return 0;
}

/**
 * 解析HTTP请求头（请求参数部分）
 * @param a_line 一行HTTP报文
 * @return 若解析成功且解析到参数则返回0，若解析成功但未解析到参数则返回1，解析失败则返回-1
 */
int HttpRequest::parseHttpHead_requestParams(const char *a_line)
{
    int p = 0;          //总指针
    std::string K, V;   //请求参数str组

    //逐字处理（K部分）
    while (a_line[p] != '\0')
    {
        //遇到第一个冒号，跳过两个字符，进入Value部分
        if (a_line[p] == ':')
        {
            K += '\0';
            p += 2;
            break;
        }
        K += a_line[p];
        p++;
    }
    //逐字处理（V部分）
    while (a_line[p] != '\0')
    {
        V += a_line[p];
        p++;
    }

    if (K.empty() && V.empty()) return 1;   //未读取到参数KV对，Param段已结束
    else if (K.empty() || V.empty()) return -1;

    parameters.insert({K, V});

    return 0;
}
