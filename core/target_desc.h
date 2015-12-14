#ifndef YGG_CORE_TARGET_DESC_H_
#define YGG_CORE_TARGET_DESC_H_

#include "codebase/object.h"
#include <string>


namespace ygg {
namespace core {


class TargetDesc : public Object {
public:
    typedef boost::intrusive_ptr<TargetDesc> Ptr;

    static TargetDesc::Ptr New(const std::string & json);
    ~TargetDesc() {}

    const std::string json;
    const std::string endpoint;


private:
    TargetDesc(const std::string & json
              , const std::string & endpoint);
};


}  // namespace core
}  // namespace ygg


#endif  // YGG_CORE_TARGET_DESC_H_
