//
// Created by OctAutumn on 2022/7/5.
//

#include "HttpException.h"

const char *HttpException::what() const throw()
{
    return msg;
}

HttpException::HttpException(const char *msg)
{
    this->msg = msg;
}
