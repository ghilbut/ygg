#ifndef YGG_LIBBOX_RULE_BYPASS_H_
#define YGG_LIBBOX_RULE_BYPASS_H_

#include "box_proxy.h"
#include "user_proxy.h"
#include "codebase/object.h"
#include <unordered_set>


using namespace codebase;


namespace box {
namespace rule {


class Bypass
    : public Object<Bypass>
    , public BoxProxy::Delegate 
    , public UserProxy::Delegate {
public:
    class Delegate {
    public:
        virtual void OnClosed(Bypass * rule) = 0;
    };

    static Bypass::Ptr New(BoxProxy::Ptr & box, Delegate * delegate);

    void SetUser(UserProxy::Ptr & user);
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
    Bypass(BoxProxy::Ptr & box, Delegate * delegate);
    ~Bypass();

private:
    typedef std::unordered_set<UserProxy::Ptr, UserProxy::Hash> UserList;

    BoxProxy::Ptr box_;
    UserList user_list_;
    Delegate * delegate_;
};


}  // namespace rule
}  // namespace box


#endif  // YGG_LIBBOX_RULE_BYPASS_H_
