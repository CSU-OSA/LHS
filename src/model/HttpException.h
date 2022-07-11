//
// Created by OctAutumn on 2022/7/5.
//

#ifndef LHS_HTTPEXCEPTION_H
#define LHS_HTTPEXCEPTION_H

#include <exception>

class HttpException : public std::exception
{

    const char *what() const throw();

public:
    HttpException(const char* msg);

    const char *msg;
};


#endif //LHS_HTTPEXCEPTION_H
