#include "localbox_desc.h"
#include "impl/localbox_desc_impl.h"
#include <json/json.h>
#include <cassert>


LocalBoxDesc::LocalBoxDesc() 
	: impl_(0) {
	// nothing
}

LocalBoxDesc::LocalBoxDesc(const std::string& json)
	: impl_(Impl::New(json)) {
	// nothing
}

LocalBoxDesc::LocalBoxDesc(const LocalBoxDesc& other) {
	other.impl_->AddRef();
}

LocalBoxDesc::~LocalBoxDesc() {
	impl_->Release();
}

LocalBoxDesc& LocalBoxDesc::operator= (const LocalBoxDesc& other) {
	if (impl_ != other.impl_) {
		other.impl_->AddRef();
		impl_->Release();
		impl_ = other.impl_;
	}
	return *this;
}

bool LocalBoxDesc::operator== (const LocalBoxDesc& other) {
	return (impl_ == other.impl_);
}

bool LocalBoxDesc::operator!= (const LocalBoxDesc& other) {
	return (impl_ != other.impl_);
}

bool LocalBoxDesc::IsEmpty() const {
	return (impl_ == 0);
}

const char* LocalBoxDesc::id() const {
	assert(impl_ != 0);
	return impl_->id();
}


LocalBoxDesc::Impl::Impl(const std::string& json)
	: invalid_(true)
	, ref_count_(1) {

	Json::Value root;
	Json::Reader reader;
	if (!reader.parse(json, root, false) || !root.isObject()) {
		// TODO(ghilbut): send error on websockt
		return;
	}

	LocalBoxDesc* desc = new LocalBoxDesc();

	Json::Value v = root.get("id", Json::nullValue);
	if (!v.isString()) {
		// TODO(ghilbut): send error on websockt
		return;
	}
	id_ = v.asString();

	invalid_ = false;
}

LocalBoxDesc::Impl* LocalBoxDesc::Impl::New(const std::string& json) {

	Impl* self = new Impl(json);
	if (self->invalid_) {
		delete self;
	}
	return self;
}

int LocalBoxDesc::Impl::AddRef() const {
	return ++ref_count_;
}

void LocalBoxDesc::Impl::Release() const {
	--ref_count_;
	if (ref_count_ < 1) {
		delete this;
	}
}

const char* LocalBoxDesc::Impl::id() const {
	return id_.c_str();
}