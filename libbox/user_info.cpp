#include "user_info.h"
#include <json/json.h>


namespace box {


UserInfo * UserInfo::New(const std::string & json) {

    Json::Value root;
    Json::Reader reader;
    if (!reader.parse(json, root, false)) {
        return nullptr;
    }

    if (!root.isObject()) {
        return nullptr;
    }

    Json::Value value;

    value = root["user-id"];
    if (!value.isString()) {
        return nullptr;
    }
    const std::string id(value.asString());
    

    return new UserInfo(json, id);
}

const char * UserInfo::id() const {
    return id_.c_str();
}

UserInfo::UserInfo(const std::string & json
                 , const std::string & id)
    : json_(json)
    , id_(id) {
    // nothing
}

}  // namespace box
