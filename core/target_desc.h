#ifndef YGG_CORE_TARGET_DESC_H_
#define YGG_CORE_TARGET_DESC_H_

#include "codebase/object.h"
#include <string>


namespace core {


class TargetDesc : public codebase::Object<TargetDesc> {
public:
    static TargetDesc::Ptr New(const std::string & json);
    ~TargetDesc() {}

    const std::string json;
    const std::string endpoint;


private:
    TargetDesc(const std::string & json
              , const std::string & endpoint);
};


}  // namespace core


#endif  // YGG_CORE_TARGET_DESC_H_
