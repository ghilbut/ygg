#ifndef YGG_LIBBOX_BOX_READY_DELEGATE_H_
#define YGG_LIBBOX_BOX_READY_DELEGATE_H_


namespace box {


class BoxProxy;


class BoxReadyDelegate {
public:
    virtual void OnBoxReady(BoxProxy * proxy) = 0;

protected:
    virtual ~BoxReadyDelegate() {}
};


}  // namespace box


#endif  // YGG_LIBBOX_BOX_READY_DELEGATE_H_
