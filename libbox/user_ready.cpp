#include "user_ready.h"


namespace box {


void UserReady::SetUserSession(Session * session) {
    readys_.insert(session);
    session->BindDelegate(this);
}

bool UserReady::HasUserSession(Session * session) const {
    return readys_.find(session) != readys_.end();
}

void UserReady::OnText(Session * session, const std::string & text) {

}

void UserReady::OnBinary(Session * session, const uint8_t bytes[], size_t size) {

}

void UserReady::OnClosed(Session * session) {
    readys_.erase(session);
}


}  // namespace box
