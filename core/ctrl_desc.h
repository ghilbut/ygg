#ifndef YGG_CORE_CTRL_DESC_H_
#define YGG_CORE_CTRL_DESC_H_

#include "codebase/object.h"
#include <boost/intrusive_ptr.hpp>
#include <string>


namespace Json {
class Value;
}  // namespace Json


namespace ygg {
namespace core {


class CtrlDesc : public Object {
public:
    typedef boost::intrusive_ptr<CtrlDesc> Ptr;

    static CtrlDesc::Ptr New(const std::string & json);
    static CtrlDesc::Ptr New(const std::string & json, Json::Value & root);
    ~CtrlDesc() {}

    const std::string json;
    const std::string user;
    const std::string endpoint;

private:
    CtrlDesc(const std::string & json
            , const std::string & user
            , const std::string & endpoint);
};


}  // namespace core
}  // namespace ygg


#endif  // YGG_CORE_CTRL_DESC_H_
