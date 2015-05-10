#ifndef YGG_LIBBOX_BOX_INFO_H_
#define YGG_LIBBOX_BOX_INFO_H_


#include <string>


namespace box {


class BoxInfo {
public:
    static BoxInfo * New(const std::string & json);
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
