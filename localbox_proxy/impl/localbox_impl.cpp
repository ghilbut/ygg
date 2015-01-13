#include "localbox.h"
#include "localbox_impl.h"
#include <boost/network.hpp>
#include <json/json.h>


LocalBox::LocalBox(const std::string &json, codebase::HttpWebsocket &ws)
	: Object(Impl::New(json, ws)) {
	// nothing
}


LocalBox::Impl::Impl(const LocalBoxDesc &desc, codebase::HttpWebsocket &ws)
    : desc_(desc)
    , ws_(ws) {
    // nothing

	/*
	Json::Value root(Json::objectValue);
	root["box-id"] = desc_.id();
	root["address"] = "0.0.0.0";
	Json::FastWriter writer;
	const std::string json = writer.write(root);

	printf("%s\n", json.c_str());

	try {
	boost::network::uri::uri uri("http://www.daum.net/api/reg/localbox/proxy/address");
	boost::network::http::client::request request(uri);
	//request << boost::network::header("Connection", "close");
	//request << boost::network::header("Content-Type", "application/json");
	//request << boost::network::header("Accepts", "application/json");
	//request << boost::network::body(json);
	boost::network::http::client client;
	//boost::network::http::client::response response = client.post(request);
	boost::network::http::client::response response = client.post(request, json, std::string("application/json"));
	
		//std::string body = boost::network::http::body(response);
		//printf("%s\n", body.c_str());
		printf("%s\n", response.body().c_str());
	}
	catch (std::exception& e) {
		printf("%s\n", e.what());
	}
	*/
}

LocalBox::Impl* LocalBox::Impl::New(const std::string &json, codebase::HttpWebsocket &ws) {

	LocalBoxDesc desc(json);
	if (desc == nullptr) {
		// TODO(ghilbut): send error on websockt
		return nullptr;
	}

    return (new Impl(desc, ws));
}