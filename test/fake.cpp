#include "fake.h"
#include <cassert>


namespace test {


FakeConnection::FakeConnection(LifeCycleMock * mock) 
    : Connection()
    , fake_(new FakeSession(this))
    , session_(fake_)
    , mock_(mock) {

    fake_->AddRef();

    if (mock_ != nullptr) {
        mock_->constructed();
    }
}

FakeConnection::FakeConnection(Session * session, LifeCycleMock * mock) 
    : Connection(), fake_(nullptr), session_(session), mock_(mock) {

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

bool FakeConnection::Open() {
    FireOnOpenedEvent();
    return true;
}

size_t FakeConnection::SendText(const std::string & text) const {
    Session::Ptr session(session_.Lock());
    if (session != nullptr) {
        session->FireOnTextEvent(text);
    }
    return text.length();
}

size_t FakeConnection::SendBinary(const std::vector<uint8_t> & bytes) const {
    Session::Ptr session(session_.Lock());
    if (session != nullptr) {
        session->FireOnBinaryEvent(bytes);
    }
    return bytes.size();
}

void FakeConnection::Close() {
    FireOnClosedEvent();
    Session::Ptr session(session_.Lock());
    if (session != nullptr) {
        session->FireOnClosedEvent();
    }
}

Session::Ptr FakeConnection::session() const {
    return session_.Lock();
}


FakeSession::FakeSession(LifeCycleMock * mock) 
    : Session()
    , fake_(new FakeConnection(this))
    , conn_(fake_)
    , mock_(mock) {

    fake_->AddRef();

    if (mock_ != nullptr) {
        mock_->constructed();
    }
}

FakeSession::FakeSession(Connection * conn, LifeCycleMock * mock) 
    : Session(), fake_(nullptr), conn_(conn), mock_(mock) {

    if (mock_ != nullptr) {
        mock_->constructed();
    }
}

FakeSession::~FakeSession() {
    if (mock_ != nullptr) {
        mock_->destructed();
    }
    if (fake_ != nullptr) {
        fake_->Release();
    }
}

size_t FakeSession::SendText(const std::string & text) const {
    printf("FakeSession::SendText\n");
    Connection::Ptr conn(conn_.Lock());
    if (conn != nullptr) {
        printf("FakeSession:: conn->FireTextEvent\n");
        conn->FireOnTextEvent(text);
    }
    return text.length();
}

size_t FakeSession::SendBinary(const std::vector<uint8_t> & bytes) const {
    Connection::Ptr conn(conn_.Lock());
    if (conn != nullptr) {
        conn->FireOnBinaryEvent(bytes);
    }
    return bytes.size();
}

void FakeSession::Close() {
    FireOnClosedEvent();
    Connection::Ptr conn(conn_.Lock());
    if (conn != nullptr) {
        conn->FireOnClosedEvent();
    }
}

Connection::Ptr FakeSession::conn() const {
    return conn_.Lock();
}


}  // namespace test
