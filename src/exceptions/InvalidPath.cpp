//
// Created by alessandro on 04/11/18.
//

#include "exceptions/InvalidPath.h"

const char* InvalidPath::what() const noexcept{
    return "Invalid JSON path";
}