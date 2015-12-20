#include "vars.h"
#include <json/json.h>
#include <cstdlib>


namespace ygg {
namespace test {


static const std::string kAlphaDigit(
    "abcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "0123456789");


std::string GetCtrlJson(const std::string & user,
                        const std::string & endpoint) {

    Json::Value root(Json::objectValue);
    root["user"] = user;
    root["endpoint"] = endpoint;

    Json::FastWriter w;
    return w.write(root);
}

std::string GetTargetJson(const std::string & endpoint) {

    Json::Value root(Json::objectValue);
    root["endpoint"] = endpoint;

    Json::FastWriter w;
    return w.write(root);
}


std::string GetRandomString() {

    srand((unsigned int) time(nullptr));

    std::string text;
    const int size = 100 + (rand() % 100);
    for (int i = 0; i < size; ++i) {
        const int r = rand() % kAlphaDigit.length();
        const char c = kAlphaDigit[r-1];
        text.push_back(c);
    }

    return text;
}


std::vector<uint8_t> GetRandomBytes() {

    srand((unsigned int) time(nullptr));

    std::vector<uint8_t> bytes;
    const int size = 100 + (rand() % 100);
    for (int i = 0; i < size; ++i) {
        const int r = rand() % kAlphaDigit.length();
        const char c = kAlphaDigit[r-1];
        bytes.push_back(c);
    }

    return bytes;
}


}  // namespace test
}  // namespace ygg
