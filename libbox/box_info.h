#ifndef YGG_LIBBOX_BOX_INFO_H_
#define YGG_LIBBOX_BOX_INFO_H_

#include "codebase/object.h"
#include <string>


using namespace codebase;


namespace box {


class BoxInfo : public Object<BoxInfo> {
public:
    static BoxInfo::Ptr New(const std::string & json);
    ~BoxInfo() {}

    const char * id() const;

private:
    BoxInfo(const std::string & json
            , const std::string & id);

private:
    const std::string json_;
    const std::string id_;
};


}  // namespace box


#endif  // YGG_LIBBOX_BOX_INFO_H_
