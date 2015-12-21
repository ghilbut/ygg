#include "ctrl_ready.h"
#include <cassert>


namespace ygg {
namespace server {
namespace target {


CtrlReady::CtrlReady(CtrlReady::Delegate * delegate) 
    : delegate_(delegate) {
    assert(delegate != nullptr);
}

void CtrlReady::SetConnection(Connection::Ptr conn) {
    assert(conn_set_.find(conn) == conn_set_.end());
    conn_set_.insert(conn);
    conn->BindDelegate(this);
}

bool CtrlReady::HasConnection(Connection::Ptr conn) {
    return (conn_set_.find(conn) != conn_set_.end());
}

void CtrlReady::OnText(Connection * conn, const Text & text) {
    assert(delegate_ != nullptr);
    assert(conn_set_.find(conn) != conn_set_.end());

    const CtrlDesc::Ptr desc(CtrlDesc::New(text));
    if (desc == nullptr) {
        conn->Close();
        return;
    }

    Connection::Ptr p(conn);
    CtrlProxy::Ptr proxy(CtrlProxy::New(p, desc));
    assert(proxy != nullptr);

    conn_set_.erase(conn);
    delegate_->OnProxy(proxy);
}

void CtrlReady::OnBinary(Connection * conn, const Bytes & bytes) {
    assert(delegate_ != nullptr);
    assert(conn_set_.find(conn) != conn_set_.end());

}

void CtrlReady::OnClosed(Connection * conn) {
    assert(conn_set_.find(conn) != conn_set_.end());
    conn_set_.erase(conn);
}


}  // namespace target
}  // namespace server
}  // namespace ygg
