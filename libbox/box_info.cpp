#include "box_info.h"
#include <json/json.h>


namespace box {


BoxInfo::Ptr BoxInfo::New(const std::string & json) {

    Json::Value root;
    Json::Reader reader;
    if (!reader.parse(json, root, false)) {
        return nullptr;
    }

    if (!root.isObject()) {
        return nullptr;
    }

    Json::Value value;

    value = root["box-id"];
    if (!value.isString()) {
        return nullptr;
    }
    const std::string id(value.asString());
    

    return new BoxInfo(json, id);
}

const char * BoxInfo::id() const {
    return id_.c_str();
}

BoxInfo::BoxInfo(const std::string & json
                 , const std::string & id)
    : Object(this)
    , json_(json)
    , id_(id) {
    // nothing
}

}  // namespace box
