#include "localbox_manager.h"
#include "localbox.h"
#include "http/http_websocket.h"


LocalBoxManager::LocalBoxManager()
    : server_(*this) {

}

LocalBoxManager::~LocalBoxManager() {
    Stop();
}

void LocalBoxManager::Start() {
    server_.Start(8080);
}

void LocalBoxManager::Stop() {
    server_.Stop();
}


void LocalBoxManager::OnRequest(void) {

}

void LocalBoxManager::OnConnect(HttpWebsocket *ws) {
	// TODO(ghilbut): check timeout from connection to running state
	readyTable_[ws] = time(0);
}

void LocalBoxManager::OnTextMessage(HttpWebsocket *ws, const std::string& text) {
    ws->WriteText("ECHO: " + text);

	const size_t erased = readyTable_.erase(ws);
	if (erased == 1) {
		regLocalBox(ws, text);
		return;
	}

	auto itr = localboxTable_.find(ws);
	if (itr == localboxTable_.end()) {
		return;
	}
}

void LocalBoxManager::OnClose(HttpWebsocket *ws) {

	auto ready = readyTable_.find(ws);
	if (ready != readyTable_.end()) {
		readyTable_.erase(ready);
		return;
	}

	unregLocalBox(ws);
}



bool LocalBoxManager::regLocalBox(HttpWebsocket* ws, const std::string& text) {



	LocalBox* box = LocalBox::New(ws, text);
	if (box == 0) {
		// TODO(ghilbut): notify cause of construction failed on websocket
		// ws->Close();
		delete ws;
		return false;
	}
	
	localboxTable_[ws] = box;
	return true;
}

void LocalBoxManager::unregLocalBox(HttpWebsocket* ws) {

	auto box = localboxTable_.find(ws);
	if (box == localboxTable_.end()) {
		return;
	}

	LocalBox::Delete(box->second);
	localboxTable_.erase(box);
}