#ifndef YGG_LIBBOX_USER_INFO_H_
#define YGG_LIBBOX_USER_INFO_H_


#include <string>


namespace box {


class UserInfo {
public:
    static UserInfo * New(const std::string & json);
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
