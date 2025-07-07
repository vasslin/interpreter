#include <gtest/gtest.h>
#include <lib/interpreter/interpreter.h>

#include "lib/lexer/lexer.h"
#include "sstream"

std::vector<std::string> splitStrings(const std::string& s) {
    std::stringstream ss;
    ss << s;
    std::vector<std::string> ans;
    std::string tmp;
    while (ss >> tmp) {
        ans.push_back(std::move(tmp));
    }
    return ans;
}
