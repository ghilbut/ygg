#include "user_desc.h"
#include <json/json.h>


namespace codebase {


UserDesc::Ptr UserDesc::New(const std::string & json) {
    Json::Value root;
    return New(json, root);
}

UserDesc::Ptr UserDesc::New(const std::string & json, Json::Value & root) {

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

    return new UserDesc(json, id);
}

const char * UserDesc::id() const {
    return id_.c_str();
}

UserDesc::UserDesc(const std::string & json
                 , const std::string & id)
    : Object(this)
    , json_(json)
    , id_(id) {
    // nothing
}


}  // namespace codebase
