#ifndef YGG_DUMMY_BOX_IMPL_BOX_DESC_H_
#define YGG_DUMMY_BOX_IMPL_BOX_DESC_H_

#include <json/json.h>
#include <string>

class BoxDesc
{
public:
    BoxDesc();
    explicit BoxDesc(const std::string& json);
    explicit BoxDesc(const BoxDesc& other);
    ~BoxDesc();

    BoxDesc& operator= (const BoxDesc& other);

    bool Parse(const std::string& json);
    const char* Stringify() const;
    bool IsEmpty() const;

    const char* id() const;
    const char* host() const;
    int port() const;

private:
    bool empty_;
    std::string json_;
    std::string id_;
    std::string host_;
    int port_;
};

#endif  // YGG_DUMMY_BOX_IMPL_BOX_DESC_H_