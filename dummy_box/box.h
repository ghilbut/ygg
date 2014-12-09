#ifndef YGG_DUMMY_BOX_BOX_H_
#define YGG_DUMMY_BOX_BOX_H_

class Box {
public:
    Box();
    ~Box();


private:
    class Impl;
    Impl* impl_;
};

#endif  // YGG_DUMMY_BOX_BOX_H_