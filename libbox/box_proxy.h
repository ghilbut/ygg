#ifndef YGG_LIBBOX_BOX_PROXY_H_
#define YGG_LIBBOX_BOX_PROXY_H_

#include "net/session_delegate.h"


using namespace net;


namespace box {


class BoxInfo;


class BoxProxy : public net::Session::Delegate {
public:
    static BoxProxy * New(const std::string & json, Session * session);
    ~BoxProxy();

    const BoxInfo * info() const;

    // net::Session::Delegate
    virtual void OnText(Session * session, const std::string & text);
    virtual void OnBinary(Session * session, const uint8_t bytes[], size_t size);
    virtual void OnClosed(Session * session);

private:
    BoxProxy(const BoxInfo * info, Session * session);

private:
    const BoxInfo * info_;
    net::Session * session_;
};


}  // namespace box


#endif  // YGG_LIBBOX_BOX_PROXY_H_
