#ifndef YGG_LIBBOX_USER_READY_DELEGATE_H_
#define YGG_LIBBOX_USER_READY_DELEGATE_H_


namespace box {


class UserProxy;


class UserReadyDelegate {
public:
    virtual void OnUserReady(UserProxy * proxy) = 0;

protected:
    virtual ~UserReadyDelegate() {}
};


}  // namespace box


#endif  // YGG_LIBBOX_USER_READY_DELEGATE_H_
