#include "fake.h"
#include <cassert>


namespace ygg {
namespace test {


Connection::Ptr FakeConnection::New(LifeCycleMock * mock) {
    return new FakeConnection(mock);
}

Connection::Ptr FakeConnection::New(Connection * conn, LifeCycleMock * mock) {
    assert(conn != nullptr);
    return new FakeConnection(conn, mock);
}

size_t FakeConnection::SendText(const std::string & text) const {
    Connection::Ptr conn(conn_.Lock());
    if (conn != nullptr) {
        conn->FireOnTextEvent(text);
    }
    return text.length();
}

size_t FakeConnection::SendBinary(const std::vector<uint8_t> & bytes) const {
    Connection::Ptr conn(conn_.Lock());
    if (conn != nullptr) {
        conn->FireOnBinaryEvent(bytes);
    }
    return bytes.size();
}

void FakeConnection::Close() {
    FireOnClosedEvent();
    Connection::Ptr conn(conn_.Lock());
    if (conn != nullptr) {
        conn->FireOnClosedEvent();
    }
}

Connection::Ptr FakeConnection::conn() const {
    return conn_.Lock();
}

FakeConnection::FakeConnection(LifeCycleMock * mock) 
    : Connection()
    , fake_(new FakeConnection(this))
    , conn_(fake_)
    , mock_(mock) {

    fake_->AddRef();

    if (mock_ != nullptr) {
        mock_->constructed();
    }
}

FakeConnection::FakeConnection(Connection * conn, LifeCycleMock * mock) 
    : Connection(), fake_(nullptr), conn_(conn), mock_(mock) {

    assert(conn != nullptr);

    if (mock_ != nullptr) {
        mock_->constructed();
    }
}
 
FakeConnection::~FakeConnection() {
    if (mock_ != nullptr) {
        mock_->destructed();
    }
    if (fake_ != nullptr) {
        fake_->Release();
    }
}


}  // namespace test
}  // namespace ygg
