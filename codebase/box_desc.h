#ifndef YGG_CODEBASE_BOX_DESC_H_
#define YGG_CODEBASE_BOX_DESC_H_

#include "object.h"
#include <string>


namespace codebase {


class BoxDesc : public Object<BoxDesc> {
public:
    static BoxDesc::Ptr New(const std::string & json);
    ~BoxDesc() {}

    const char * id() const;

private:
    BoxDesc(const std::string & json
            , const std::string & id);

private:
    const std::string json_;
    const std::string id_;
};


}  // namespace codebase


#endif  // YGG_CODEBASE_BOX_DESC_H_
