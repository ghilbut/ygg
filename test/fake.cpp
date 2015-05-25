#include "fake.h"


namespace test {


FakeConnection::FakeConnection(LifeCycleMock * mock) 
    : Connection(), mock_(mock) {

    if (mock_ != nullptr) {
        mock_->constructed();
    }
}

FakeConnection::~FakeConnection() {
    if (mock_ != nullptr) {
        mock_->destructed();
    }
}

bool FakeConnection::Open() {
    if (session_ != nullptr) {
        //session_->FireOnOpenEvent(this);
    }
    return true;
}

size_t FakeConnection::SendText(const std::string & text) const {
    if (session_ != nullptr) {
        session_->FireOnTextEvent(text);
    }
    return text.length();
}

size_t FakeConnection::SendBinary(const std::vector<uint8_t> & bytes) const {
    if (session_ != nullptr) {
        session_->FireOnBinaryEvent(bytes);
    }
    return bytes.size();
}

void FakeConnection::Close() {
    if (session_ != nullptr) {
        session_->FireOnClosedEvent();
    }
}


FakeSession::FakeSession(LifeCycleMock * mock) 
    : Session(), mock_(mock) {

    if (mock_ != nullptr) {
        mock_->constructed();
    }
}

FakeSession::~FakeSession() {
    if (mock_ != nullptr) {
        mock_->destructed();
    }
}

size_t FakeSession::SendText(const std::string & text) const {
    return text.length();
}

size_t FakeSession::SendBinary(const std::vector<uint8_t> & bytes) const {
    return bytes.size();
}

void FakeSession::Close() {
    // nothing
}


}  // namespace test
