#include "box_ready.h"


namespace box {


void BoxReady::SetBoxConnection(WebSocket * ws) {
    readys_.insert(ws);
    ws->BindDelegate(this);
}

bool BoxReady::HasBoxConnection(WebSocket * ws) const {
    return readys_.find(ws) != readys_.end();
}

void BoxReady::OnTextMessage(WebSocket * ws, const std::string & text) {

}

void BoxReady::OnClosed(WebSocket * ws) {
    readys_.erase(ws);
}


}  // namespace box
