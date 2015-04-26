#include <gmock/gmock.h>
#include "object2.h"

using namespace codebase;


class Mock {
public:
    MOCK_CONST_METHOD0(constructed, void());
    MOCK_CONST_METHOD0(destructed, void());
};

class TestObject : public Object2 {
public:
    typedef boost::intrusive_ptr<TestObject> Ptr;
    static Ptr New(const Mock &mock) {
        return Ptr(new TestObject(mock));
    }

private:
    TestObject(const Mock &mock) 
        : mock_(mock) {
        mock_.constructed();
    }

    ~TestObject() {
        mock_.destructed();
    }

private:
    const Mock &mock_;
public:
    Ptr ptr_;
};

void intrusive_ptr_add_ref(TestObject *px) {
    px->add_ref();
}
void intrusive_ptr_release(TestObject *px) {
    px->release();
}


class ObjectTest : public ::testing::Test {
};


TEST_F(ObjectTest, test_construction_and_destruction) {

    Mock mock;
    EXPECT_CALL(mock, constructed()).Times(1);
    EXPECT_CALL(mock, destructed()).Times(1);

    TestObject::Ptr object = TestObject::New(mock);
}

TEST_F(ObjectTest, test_circular) {

    Mock mock;
    EXPECT_CALL(mock, constructed()).Times(3);
    EXPECT_CALL(mock, destructed()).Times(3);

    TestObject::Ptr obj0 = TestObject::New(mock);
    TestObject::Ptr obj1 = TestObject::New(mock);
    TestObject::Ptr obj2 = TestObject::New(mock);

    obj0->ptr_ = obj1;
    obj1->ptr_ = obj2;
    obj2->ptr_ = obj0;
}
