#ifndef YGG_CODEBASE_BOX_PROXY_H_
#define YGG_CODEBASE_BOX_PROXY_H_

#include "box_desc.h"
#include "connection.h"


namespace codebase {


class BoxProxy
    : public Object<BoxProxy>
    , public Connection::Delegate {

public:
    class Delegate {
    public:
        virtual void OnText(BoxProxy * box, const std::string & text) = 0;
        virtual void OnBinary(BoxProxy * box, const std::vector<uint8_t> & bytes) = 0;
        virtual void OnClosed(BoxProxy * box) = 0;
    };

public:
    static BoxProxy::Ptr New(Connection::Ptr & conn, const std::string & json);
    ~BoxProxy();

    void BindDelegate(Delegate * delegate);
    void UnbindDelegate();

    const BoxDesc & info() const;

    size_t SendText(const std::string & text) const;
    size_t SendBinary(const std::vector<uint8_t> & bytes) const;
    void Close();

    // codebase::Connection::Delegate
    virtual void OnText(Connection * conn, const std::string & text);
    virtual void OnBinary(Connection * conn, const std::vector<uint8_t> & bytes);
    virtual void OnClosed(Connection * conn);

private:
    BoxProxy(Connection::Ptr & conn, const BoxDesc::Ptr & info);

private:
    Delegate * delegate_;
    Connection::Ptr conn_;
    const BoxDesc::Ptr info_;
};


}  // namespace codebase


#endif  // YGG_CODEBASE_BOX_PROXY_H_
