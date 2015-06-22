#ifndef YGG_CODEBASE_USER_DESC_H_
#define YGG_CODEBASE_USER_DESC_H_

#include "codebase/object.h"
#include <string>


namespace Json {
class Value;
}  // namespace Json


namespace codebase {


class UserDesc : public codebase::Object<UserDesc> {
public:
    static UserDesc::Ptr New(const std::string & json);
    static UserDesc::Ptr New(const std::string & json, Json::Value & root);
    ~UserDesc() {}

    const char * id() const;

private:
    UserDesc(const std::string & json
            , const std::string & id);

private:
    const std::string json_;
    const std::string id_;
};


}  // namespace codebase


#endif  // YGG_CODEBASE_USER_DESC_H_
