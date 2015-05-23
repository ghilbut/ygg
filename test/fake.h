#ifndef YGG_TEST_TEST_FAKE_H_
#define YGG_TEST_TEST_FAKE_H_

#include "mock.h"
#include "codebase/session.h"


using namespace codebase;


namespace test {


class FakeSession : public Session {
public:
    static Session::Ptr New(LifeCycleMock * mock = nullptr) {
        return new FakeSession(mock);
    }

    virtual size_t SendText(const std::string & text) const {
        return text.length();
    }

    virtual size_t SendBinary(const std::vector<uint8_t> & bytes) const {
        return bytes.size();
    }

    virtual void Close() {
        // nothing
    }

private:
    FakeSession(LifeCycleMock * mock) 
        : mock_(mock) {

        if (mock_ != nullptr) {
            mock_->constructed();
        }
    }

    ~FakeSession() {
        if (mock_ != nullptr) {
            mock_->destructed();
        }
    }

    LifeCycleMock * mock_;
};


}  // namespace test


#endif  // YGG_TEST_TEST_FAKE_H_
