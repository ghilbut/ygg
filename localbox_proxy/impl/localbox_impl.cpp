#include "localbox.h"
#include "localbox_impl.h"
#include <json/json.h>


LocalBox::LocalBox(const std::string &json, codebase::HttpWebsocket &ws)
	: Object(Impl::New(json, ws)) {
	// nothing
}


LocalBox::Impl::Impl(const LocalBoxDesc &desc, codebase::HttpWebsocket &ws)
    : desc_(desc)
    , ws_(ws) {
    // nothing
}

LocalBox::Impl* LocalBox::Impl::New(const std::string &json, codebase::HttpWebsocket &ws) {

	LocalBoxDesc desc(json);
	if (desc == nullptr) {
		// TODO(ghilbut): send error on websockt
		return nullptr;
	}

    return (new Impl(desc, ws));
}