#ifndef YGG_LIBBOX_BOX_PROXY_H_
#define YGG_LIBBOX_BOX_PROXY_H_

#include "codebase/session_delegate.h"


using namespace codebase;


namespace box {


class BoxInfo;


class BoxProxy : 
    public Object<BoxProxy>
    , public Session::Delegate {

public:
    static BoxProxy::Ptr New(Session::Ptr & session, const std::string & json);
    ~BoxProxy();

    const BoxInfo * info() const;

    // net::Session::Delegate
    virtual void OnText(Session * session, const std::string & text);
    virtual void OnBinary(Session * session, const uint8_t bytes[], size_t size);
    virtual void OnClosed(Session * session);

private:
    BoxProxy(Session::Ptr & session, const BoxInfo * info);

private:
    Session::Ptr & session_;
    const BoxInfo * info_;
};


}  // namespace box


#endif  // YGG_LIBBOX_BOX_PROXY_H_
