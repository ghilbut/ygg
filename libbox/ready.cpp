#include "ready.h"


namespace box {


class Proxy;


void Ready::SetSession(Session * session) {
    readys_.insert(session);
    session->BindDelegate(this);
}

bool Ready::HasSession(Session * session) const {
    return readys_.find(session) != readys_.end();
}

void Ready::OnText(Session * session, const std::string & text) {

}

void Ready::OnBinary(Session * session, const uint8_t bytes[], size_t size) {

}

void Ready::OnClosed(Session * session) {
    readys_.erase(session);
}


}  // namespace box
