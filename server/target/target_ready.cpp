#include "target_ready.h"
#include <cassert>


namespace ygg {
namespace server {
namespace target {


TargetReady::TargetReady(Delegate * delegate) 
    : delegate_(delegate) {
    assert(delegate != nullptr);
}

void TargetReady::SetConnection(Connection::Ptr conn) {
    assert(conn_set_.find(conn) == conn_set_.end());
    conn_set_.insert(conn);
    conn->BindDelegate(this);
}

bool TargetReady::HasConnection(Connection::Ptr conn) {
    return (conn_set_.find(conn) != conn_set_.end());
}

void TargetReady::OnText(Connection * conn, const Text & text) {
    assert(delegate_ != nullptr);
    assert(conn_set_.find(conn) != conn_set_.end());

    const TargetDesc::Ptr desc(TargetDesc::New(text));
    if (desc == nullptr) {
        conn->Close();
        return;
    }

    Connection::Ptr p(conn);
    TargetProxy::Ptr proxy(TargetProxy::New(p, desc));
    assert(proxy != nullptr);

    conn_set_.erase(conn);
    delegate_->OnProxy(proxy);
}

void TargetReady::OnBinary(Connection * conn, const Bytes & bytes) {
    assert(delegate_ != nullptr);
    assert(conn_set_.find(conn) != conn_set_.end());

}

void TargetReady::OnClosed(Connection * conn) {
    assert(conn_set_.find(conn) != conn_set_.end());
    conn_set_.erase(conn);
}


}  // namespace target
}  // namespace server
}  // namespace ygg
