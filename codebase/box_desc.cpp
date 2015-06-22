#include "box_desc.h"
#include <json/json.h>


namespace codebase {


BoxDesc::Ptr BoxDesc::New(const std::string & json) {

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
    

    return new BoxDesc(json, id);
}

const char * BoxDesc::id() const {
    return id_.c_str();
}

BoxDesc::BoxDesc(const std::string & json
                 , const std::string & id)
    : Object(this)
    , json_(json)
    , id_(id) {
    // nothing
}

}  // namespace codebase
