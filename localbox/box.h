#ifndef YGG_DUMMY_BOX_BOX_H_
#define YGG_DUMMY_BOX_BOX_H_

#include "codebase/boost_fwd.h"
#include "codebase/object_ref.h"


class BoxDesc;

class Box : public codebase::ObjectRef {
public:
    Box(IOService& io_service, BoxDesc& desc);
    ~Box();

    void Write(const std::string& text);
    void Close();
};

#endif  // YGG_DUMMY_BOX_BOX_H_