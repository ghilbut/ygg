#include "local_connection.h"
#include <cassert>


namespace ygg {
namespace net {


Connection::Ptr LocalConnection::New() {
    return new LocalConnection();
}

size_t LocalConnection::SendText(const Text & text) const {
    Connection::Ptr conn(conn_.Lock());
    if (conn != nullptr) {
        conn->FireOnTextEvent(text);
    }
    return text.length();
}

size_t LocalConnection::SendBinary(const Bytes & bytes) const {
    Connection::Ptr conn(conn_.Lock());
    if (conn != nullptr) {
        conn->FireOnBinaryEvent(bytes);
    }
    return bytes.size();
}

void LocalConnection::Close() {
    FireOnClosedEvent();
    Connection::Ptr conn(conn_.Lock());
    if (conn != nullptr) {
        conn->FireOnClosedEvent();
    }
}

Connection::Ptr LocalConnection::other() const {
    return conn_.Lock();
}

LocalConnection::LocalConnection() 
    : Connection()
    , other_(new LocalConnection(this))
    , conn_(other_) {

    other_->AddRef();
}

LocalConnection::LocalConnection(Connection * conn) 
    : Connection(), other_(nullptr), conn_(conn) {

    assert(conn != nullptr);
}
 
LocalConnection::~LocalConnection() {
    if (other_ != nullptr) {
        other_->Release();
    }
}


}  // namespace net
}  // namespace ygg 
