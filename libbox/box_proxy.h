#ifndef YGG_LIBBOX_BOX_PROXY_H_
#define YGG_LIBBOX_BOX_PROXY_H_

#include "box_info.h"
#include "user_proxy.h"
#include "codebase/session_delegate.h"
#include <unordered_set>


using namespace codebase;


namespace box {


class BoxInfo;


class BoxProxy
    : public Object<BoxProxy>
    , public Session::Delegate {

public:
    class Delegate {
    public:
        virtual void OnText(BoxProxy * box, const std::string & text) = 0;
        virtual void OnBinary(BoxProxy * box, const std::vector<uint8_t> & bytes) = 0;
        virtual void OnClosed(BoxProxy * box) = 0;
    };

public:
    static BoxProxy::Ptr New(Session::Ptr & session, const std::string & json);
    ~BoxProxy();

    void BindDelegate(Delegate * delegate);
    void UnbindDelegate();

    const BoxInfo & info() const;

    void SetUser(UserProxy::Ptr & user);

    size_t SendText(const std::string & text) const;
    size_t SendBinary(const std::vector<uint8_t> & bytes) const;
    void Close();

    // net::Session::Delegate
    virtual void OnText(Session * session, const std::string & text);
    virtual void OnBinary(Session * session, const std::vector<uint8_t> & bytes);
    virtual void OnClosed(Session * session);

private:
    BoxProxy(Session::Ptr & session, const BoxInfo::Ptr & info);

private:
    Delegate * delegate_;
    Session::Ptr session_;
    const BoxInfo::Ptr info_;
};


}  // namespace box


#endif  // YGG_LIBBOX_BOX_PROXY_H_
