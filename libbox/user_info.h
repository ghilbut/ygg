#ifndef YGG_LIBBOX_USER_INFO_H_
#define YGG_LIBBOX_USER_INFO_H_

#include "codebase/object.h"
#include <string>


namespace box {


class UserInfo : public codebase::Object<UserInfo> {
public:
    static UserInfo::Ptr New(const std::string & json);
    ~UserInfo() {}

    const char * id() const;
    const char * box_id() const;

private:
    UserInfo(const std::string & json
            , const std::string & id
            , const std::string & box_id);

private:
    const std::string json_;
    const std::string id_;
    const std::string box_id_;
};


}  // namespace box


#endif  // YGG_LIBBOX_USER_INFO_H_
