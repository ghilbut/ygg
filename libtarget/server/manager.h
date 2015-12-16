#ifndef YGG_LIBTARGET_SERVER_MANAGER_H_
#define YGG_LIBTARGET_SERVER_MANAGER_H_


namespace ygg {
namespace target {
namespace server {


class Manager
    : public CtrlReady::Delegate
    , public TargetReady::Deleagate {

public:
    Manager();
    ~Manager();

    void SetCtrlConnection(Connection::Ptr & conn);
    void SetTargetConnection(Connection::Ptr & conn);

    virtual void OnProxy(CtrlProxy::Ptr & proxy);
    virtual void OnProxy(TargetProxy::Ptr & proxy);


private:
    CtrlReady ctrl_ready_;
    TargetReady target_ready_;
};


}  // namespace server
}  // namespace target
}  // namespace ygg


#endif  // YGG_LIBTARGET_SERVER_MANAGER_H_
