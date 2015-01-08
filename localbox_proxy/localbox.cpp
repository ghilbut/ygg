#include "localbox.h"
#include "localbox_desc.h"
#include <json/json.h>


LocalBox* LocalBox::New(HttpWebsocket* ws, const std::string& json) {

	LocalBoxDesc desc(json);
	if (desc.IsEmpty()) {
		// TODO(ghilbut): send error on websockt
		return 0;
	}

	return (new LocalBox(desc, ws));
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