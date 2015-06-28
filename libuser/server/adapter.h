#ifndef YGG_LIBUSER_SERVER_ADAPTER_H_
#define YGG_LIBUSER_SERVER_ADAPTER_H_

#include "libbox/box_proxy.h"
#include "codebase/user_proxy.h"


using namespace box;


namespace user {
namespace server {


class Adapter
    : public Object<Adapter> 
    , public BoxProxy::Delegate 
    , public UserProxy::Delegate {

public:
    class Delegate {
    public:
        virtual void OnClosed(Adapter * adapter) = 0;
    };

    static Adapter::Ptr New(UserProxy::Ptr & user, Delegate * delegate);

    void SetBox(BoxProxy::Ptr & box);
    void Close();

    // BoxProxy::Delegate
    virtual void OnText(BoxProxy * box, const std::string & text);
    virtual void OnBinary(BoxProxy * box, const std::vector<uint8_t> & bytes);
    virtual void OnClosed(BoxProxy * box);

    // UserProxy::Delegate
    virtual void OnText(UserProxy * user, const std::string & text);
    virtual void OnBinary(UserProxy * user, const std::vector<uint8_t> & bytes);
    virtual void OnClosed(UserProxy * user);

private:
    Adapter(UserProxy::Ptr & user, Delegate * delegate);
    ~Adapter();

private:
    BoxProxy::Ptr box_;
    UserProxy::Ptr user_;
    Delegate * delegate_;
};


}  // namespace server
}  // namespace user


#endif  // YGG_LIBUSER_SERVER_ADAPTER_H_
