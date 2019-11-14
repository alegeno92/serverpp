//
// Created by alessandro on 04/11/18.
//

#include <exception>

#ifndef AGENT_INVALIDPATH_H
#define AGENT_INVALIDPATH_H


class InvalidPath: public std::exception {
    const char* what() const noexcept override;
};


#endif //AGENT_INVALIDPATH_H
