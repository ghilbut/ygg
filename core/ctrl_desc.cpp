#include "ctrl_desc.h"
#include <json/json.h>


namespace ygg {
namespace core {


CtrlDesc::Ptr CtrlDesc::New(const std::string & json) {
    Json::Value root;
    return New(json, root);
}

CtrlDesc::Ptr CtrlDesc::New(const std::string & json, Json::Value & root) {

    Json::Reader reader;
    if (!reader.parse(json, root, false)) {
        return nullptr;
    }

    if (!root.isObject()) {
        return nullptr;
    }

    Json::Value value;

    value = root["user"];
    if (!value.isString()) {
        return nullptr;
    }
    const std::string user(value.asString());

    value = root["endpoint"];
    if (!value.isString()) {
        return nullptr;
    }
    const std::string endpoint(value.asString());

    return new CtrlDesc(json, user, endpoint);
}

CtrlDesc::CtrlDesc(const std::string & json
                  , const std::string & user
                  , const std::string & endpoint)
    : Object()
    , json(json)
    , user(user)
    , endpoint(endpoint) {
    // nothing
}


}  // namespace core
}  // namespace ygg
