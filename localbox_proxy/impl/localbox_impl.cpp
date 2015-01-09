#include "localbox.h"
#include "localbox_impl.h"
#include "localbox_desc.h"
#include <json/json.h>


LocalBox::LocalBox(const LocalBox& other) 
    : impl_(other.impl_) {
    // nothing
}

LocalBox::LocalBox(const std::string& json, HttpWebsocket &ws)
    : impl_(Impl::New(json, ws)) {
    // nothing
}

LocalBox& LocalBox::operator= (const LocalBox& other) {
    impl_ = other.impl_;
    return *this;
}

bool LocalBox::operator== (const LocalBox& other) const {
    return (impl_.get() == other.impl_.get());
}

bool LocalBox::operator!= (const LocalBox& other) const {
    return (impl_.get() != other.impl_.get());
}

bool LocalBox::IsNull() const {
    return (impl_.get() == nullptr);
}



LocalBox::Impl::Impl(const LocalBoxDesc& desc, HttpWebsocket &ws)
    : desc_(desc)
    , ws_(ws) {
    // nothing
}

LocalBox::Impl::Ptr LocalBox::Impl::New(const std::string& json, HttpWebsocket &ws) {

	LocalBoxDesc desc(json);
	if (desc.IsNull()) {
		// TODO(ghilbut): send error on websockt
		return nullptr;
	}

    return Ptr(new Impl(desc, ws), &Impl::Delete);
}

void LocalBox::Impl::Delete(Impl* impl) {
    delete impl;
}