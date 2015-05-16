#include "user_info.h"
#include <json/json.h>


namespace box {


UserInfo::Ptr UserInfo::New(const std::string & json) {

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
    
    value = root["box-id"];
    if (!value.isString()) {
        return nullptr;
    }
    const std::string box_id(value.asString());


    return new UserInfo(json, id, box_id);
}

const char * UserInfo::id() const {
    return id_.c_str();
}

const char * UserInfo::box_id() const {
    return box_id_.c_str();
}

UserInfo::UserInfo(const std::string & json
                 , const std::string & id
                 , const std::string & box_id)
    : Object()
    , json_(json)
    , id_(id)
    , box_id_(box_id) {
    // nothing
}


}  // namespace box
