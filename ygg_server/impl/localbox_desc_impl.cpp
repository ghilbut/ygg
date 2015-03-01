#include "localbox_desc.h"
#include "impl/localbox_desc_impl.h"

#include "codebase/scoped_ptr.h"
#include <json/json.h>
#include <cassert>


LocalBoxDesc::LocalBoxDesc(const std::string& json)
	: Object(Impl::New(json)) {
	// nothing
}

const char* LocalBoxDesc::id() const {
	assert(impl_ != nullptr);
	return static_cast<Impl*>(impl_)->id();
}



LocalBoxDesc::Impl* LocalBoxDesc::Impl::New(const std::string& json) {

    Json::Value root;
    Json::Reader reader;
    if (!reader.parse(json, root, false) || !root.isObject()) {
        // TODO(ghilbut): send error on websockt
        return nullptr;
    }


	codebase::ScopedPtr<Impl> self(new Impl(), Impl::Delete);

    Json::Value v = root.get("id", Json::nullValue);
    if (!v.isString()) {
        // TODO(ghilbut): send error on websockt
        return nullptr;
    }
    self->id_ = v.asString();

    return self.Detach();
}

void LocalBoxDesc::Impl::Delete(Impl* impl) {
	delete impl;
}

const char* LocalBoxDesc::Impl::id() const {
	return id_.c_str();
}