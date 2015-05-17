#ifndef YGG_LIBBOX_USER_INFO_H_
#define YGG_LIBBOX_USER_INFO_H_

#include "codebase/object.h"
#include <string>


namespace Json {
class Value;
}  // namespace Json


namespace box {


class UserInfo : public codebase::Object<UserInfo> {
public:
    static UserInfo::Ptr New(const std::string & json);
    static UserInfo::Ptr New(const std::string & json, Json::Value & root);
    ~UserInfo() {}

    const char * id() const;

private:
    UserInfo(const std::string & json
            , const std::string & id);

private:
    const std::string json_;
    const std::string id_;
};


}  // namespace box


#endif  // YGG_LIBBOX_USER_INFO_H_
