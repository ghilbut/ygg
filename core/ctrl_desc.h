#ifndef YGG_CORE_CTRL_DESC_H_
#define YGG_CORE_CTRL_DESC_H_

#include "codebase/object.h"
#include <string>


namespace Json {
class Value;
}  // namespace Json


namespace core {


class CtrlDesc : public codebase::Object<CtrlDesc> {
public:
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


#endif  // YGG_CORE_CTRL_DESC_H_
