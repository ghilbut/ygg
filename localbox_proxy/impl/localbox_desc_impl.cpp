#include "localbox_desc.h"
#include "impl/localbox_desc_impl.h"
#include <json/json.h>
#include <cassert>


LocalBoxDesc::LocalBoxDesc(const std::string& json)
	: impl_(Impl::New(json)) {
	// nothing
}

LocalBoxDesc::LocalBoxDesc(const LocalBoxDesc& other)
    : impl_(other.impl_) {
	
}

LocalBoxDesc& LocalBoxDesc::operator= (const LocalBoxDesc& other) {
    impl_ = other.impl_;
	return *this;
}

bool LocalBoxDesc::operator== (const LocalBoxDesc& other) const {
	return (impl_ == other.impl_);
}

bool LocalBoxDesc::operator!= (const LocalBoxDesc& other) const {
	return (impl_ != other.impl_);
}

bool LocalBoxDesc::IsNull() const {
	return (impl_ == nullptr);
}

const char* LocalBoxDesc::id() const {
	assert(impl_ != nullptr);
	return impl_->id();
}


LocalBoxDesc::Impl::Ptr LocalBoxDesc::Impl::New(const std::string& json) {

    Json::Value root;
    Json::Reader reader;
    if (!reader.parse(json, root, false) || !root.isObject()) {
        // TODO(ghilbut): send error on websockt
        return nullptr;
    }

    Ptr self(new Impl(), &Impl::Delete);

    Json::Value v = root.get("id", Json::nullValue);
    if (!v.isString()) {
        // TODO(ghilbut): send error on websockt
        return nullptr;
    }
    self->id_ = v.asString();

    return self;
}

void LocalBoxDesc::Impl::Delete(Impl* impl) {
    delete impl;
}

const char* LocalBoxDesc::Impl::id() const {
	return id_.c_str();
}