#include "target_desc.h"
#include <json/json.h>


namespace core {


TargetDesc::Ptr TargetDesc::New(const std::string & json) {

    Json::Value root;
    Json::Reader reader;
    if (!reader.parse(json, root, false)) {
        return nullptr;
    }

    if (!root.isObject()) {
        return nullptr;
    }

    Json::Value value;

    value = root["endpoint"];
    if (!value.isString()) {
        return nullptr;
    }
    const std::string endpoint(value.asString());
    

    return new TargetDesc(json, endpoint);
}

TargetDesc::TargetDesc(const std::string & json
                      , const std::string & endpoint)
    : Object(this)
    , json(json)
    , endpoint(endpoint) {
    // nothing
}

}  // namespace core
