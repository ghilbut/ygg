#include <gmock/gmock.h>

#include "object.h"
#include "test/test_mock.h"
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>


namespace codebase {


class TestObject : public Object<TestObject> {
public:
    static Ptr New(const test::LifeCycleMock * mock = nullptr) {
        return new TestObject(mock);
    }

private:
    TestObject(const test::LifeCycleMock * mock) 
        : Object()
        , mock_(mock) {

        if (mock_ != nullptr) {
            mock_->constructed();
        }
    }

    ~TestObject() {
        if (mock_ != nullptr) {
            mock_->destructed();
        }
    }

private:
    const test::LifeCycleMock * mock_;
public:
    Ptr ptr_;
};


TEST(ObjectTest, test_construction_and_destruction) {

    test::LifeCycleMock mock;
    EXPECT_CALL(mock, constructed()).Times(1);
    EXPECT_CALL(mock, destructed()).Times(1);

    TestObject::Ptr object = TestObject::New(&mock);
}

/*
TEST(ObjectTest, DISABLED_test_circular) {

    Mock mock;
    EXPECT_CALL(mock, constructed()).Times(3);
    EXPECT_CALL(mock, destructed()).Times(3);

    TestObject::Ptr obj0(TestObject::New(mock));
    TestObject::Ptr obj1(TestObject::New(mock));
    TestObject::Ptr obj2(TestObject::New(mock));

    obj0->ptr_ = obj1;
    obj0->ptr_.MakeWeak();
    obj1->ptr_ = obj2;
    obj1->ptr_.MakeWeak();
    obj2->ptr_ = obj0;
    obj2->ptr_.MakeWeak();

}
*/

TEST(ObjectTest, test_object_ptr_validation_as_container_hash_key) {

    test::LifeCycleMock mock;
    EXPECT_CALL(mock, constructed()).Times(1);
    EXPECT_CALL(mock, destructed()).Times(1);

    TestObject::Ptr obj0(TestObject::New(&mock));
    TestObject::Ptr obj1(obj0);
    TestObject::Ptr obj2(obj1);

    TestObject * obj = obj0.get();

    {
        std::set<TestObject::Ptr> set;
        set.insert(obj0);
        EXPECT_NE(set.end(), set.find(obj0));
        EXPECT_NE(set.end(), set.find(obj1));
        EXPECT_NE(set.end(), set.find(obj2));
        EXPECT_NE(set.end(), set.find(obj));
    }

    {
        std::unordered_set<TestObject::Ptr, TestObject::Hash> uset;
        uset.insert(obj0);
        EXPECT_NE(uset.end(), uset.find(obj0));
        EXPECT_NE(uset.end(), uset.find(obj1));
        EXPECT_NE(uset.end(), uset.find(obj2));
        EXPECT_NE(uset.end(), uset.find(obj));
    }

    {
        std::map<TestObject::Ptr, int> map;
        map[obj0] = 0;
        EXPECT_EQ(0, map[obj0]);
        EXPECT_EQ(0, map[obj1]);
        EXPECT_EQ(0, map[obj2]);
        EXPECT_EQ(0, map[obj]);
        map[obj1] = 1;
        EXPECT_EQ(1, map[obj0]);
        EXPECT_EQ(1, map[obj1]);
        EXPECT_EQ(1, map[obj2]);
        EXPECT_EQ(1, map[obj]);
        map[obj2] = 2;
        EXPECT_EQ(2, map[obj0]);
        EXPECT_EQ(2, map[obj1]);
        EXPECT_EQ(2, map[obj2]);
        EXPECT_EQ(2, map[obj]);
        map[obj] = 3;
        EXPECT_EQ(3, map[obj0]);
        EXPECT_EQ(3, map[obj1]);
        EXPECT_EQ(3, map[obj2]);
        EXPECT_EQ(3, map[obj]);
    }

    {
        std::unordered_map<TestObject::Ptr, int, TestObject::Hash> umap;
        umap[obj0] = 0;
        EXPECT_EQ(0, umap[obj0]);
        EXPECT_EQ(0, umap[obj1]);
        EXPECT_EQ(0, umap[obj2]);
        EXPECT_EQ(0, umap[obj]);
        umap[obj1] = 1;
        EXPECT_EQ(1, umap[obj0]);
        EXPECT_EQ(1, umap[obj1]);
        EXPECT_EQ(1, umap[obj2]);
        EXPECT_EQ(1, umap[obj]);
        umap[obj2] = 2;
        EXPECT_EQ(2, umap[obj0]);
        EXPECT_EQ(2, umap[obj1]);
        EXPECT_EQ(2, umap[obj2]);
        EXPECT_EQ(2, umap[obj]);
        umap[obj] = 3;
        EXPECT_EQ(3, umap[obj0]);
        EXPECT_EQ(3, umap[obj1]);
        EXPECT_EQ(3, umap[obj2]);
        EXPECT_EQ(3, umap[obj]);
    }
}


}  // namespace codebase
