#include "localbox.h"
#include "localbox_desc.h"
#include <json/json.h>


LocalBox::Ptr LocalBox::New(HttpWebsocket* ws, const std::string& json) {

	LocalBoxDesc desc(json);
	if (desc.IsEmpty()) {
		// TODO(ghilbut): send error on websockt
		return 0;
	}

	return Ptr(new LocalBox(desc, ws), &LocalBox::Delete);
}

void LocalBox::Delete(LocalBox* box) {
	delete box;
}


LocalBox::LocalBox(const LocalBoxDesc& desc, HttpWebsocket* ws)
	: desc_(desc)
	, ws_(ws) {
	// nothing
}

LocalBox::~LocalBox() {
	// nothing
}