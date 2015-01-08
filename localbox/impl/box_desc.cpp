#include "box_desc.h"


BoxDesc::BoxDesc()
    : empty_(true)
    , port_(-1) {
}

BoxDesc::BoxDesc(const std::string& json) {
    Parse(json);
}

BoxDesc::BoxDesc(const BoxDesc& other)
    : empty_(other.empty_)
    , json_(other.json_)
    , id_(other.id_)
    , host_(other.host_)
    , port_(other.port_) {
    // nothing
}

BoxDesc::~BoxDesc() {
    // nothing
}

BoxDesc& BoxDesc::operator= (const BoxDesc& other) {
    empty_ = other.empty_;
    if (!empty_) {
        json_ = other.json_;
        id_   = other.id_;
        host_ = other.host_;
        port_ = other.port_;
    }
    return *this;
}

bool BoxDesc::Parse(const std::string& json) {

    empty_ = true;

    Json::Value root;
    Json::Reader reader;
    if (!reader.parse(json, root, false)) {
        return false;
    }

    if (!root.isObject()) {
        return false;
    }

    Json::Value tmp;
    tmp = root.get("id", Json::Value::null);
    if (!tmp.isString()) {
        return false;
    }
    id_ = tmp.asString();

    Json::Value cloud = root.get("server", Json::Value::null);
    if (!cloud.isObject()) {
        return false;
    }

    tmp = cloud.get("host", Json::Value::null);
    if (!tmp.isString()) {
        return false;
    }
    host_ = tmp.asString();

    tmp = cloud.get("port", Json::Value::null);
    if (!tmp.isInt()) {
        return false;
    }
    port_ = tmp.asInt();

    json_ = json;
    empty_ = false;
    return true;
}

const char* BoxDesc::Stringify() const {
    return (empty_ ? NULL : json_.c_str());
}

bool BoxDesc::IsEmpty() const {
    return empty_;
}

const char* BoxDesc::id() const {
    return (empty_ ? NULL : id_.c_str());
}

const char* BoxDesc::host() const {
    return (empty_ ? NULL : host_.c_str());
}

int BoxDesc::port() const {
    return (empty_ ? -1 : port_);
}