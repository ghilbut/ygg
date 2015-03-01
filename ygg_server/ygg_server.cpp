#include "ygg_server.h"


YggServer::YggServer() {

}

YggServer::~YggServer() {

}

void YggServer::Start() {
	io_service_.Start();
}

void YggServer::Stop() {
	io_service_.Stop();
}