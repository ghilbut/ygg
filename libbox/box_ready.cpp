#include "box_ready.h"


namespace box {


void BoxReady::SetBoxSession(Session * session) {
    readys_.insert(session);
    session->BindDelegate(this);
}

bool BoxReady::HasBoxSession(Session * session) const {
    return readys_.find(session) != readys_.end();
}

void BoxReady::OnText(Session * session, const std::string & text) {

}

void BoxReady::OnBinary(Session * session, const uint8_t bytes[], size_t size) {

}

void BoxReady::OnClosed(Session * session) {
    readys_.erase(session);
}


}  // namespace box
