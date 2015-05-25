#include <gmock/gmock.h>

#include "object.h"
#include "test/mock.h"
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>


using namespace test;


namespace codebase {


class TestObject : public Object<TestObject> {
public:
    static Ptr New(const LifeCycleMock * mock = nullptr) {
        return new TestObject(mock);
    }

    void set_other(Ptr & other) {
        other_ = other;
    }

private:
    TestObject(const LifeCycleMock * mock) 
        : Object(this)
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
    const LifeCycleMock * mock_;
    Weak other_;
};


TEST(ObjectTest, test_construction_and_destruction) {

    LifeCycleMock mock;
    EXPECT_CALL(mock, constructed()).Times(1);
    EXPECT_CALL(mock, destructed()).Times(1);

    TestObject::Ptr obj(TestObject::New(&mock));
}

TEST(ObjectTest, test_construction_and_destruction_with_operator_equal) {

    LifeCycleMock mock;
    EXPECT_CALL(mock, constructed()).Times(1);
    EXPECT_CALL(mock, destructed()).Times(1);

    TestObject::Ptr obj;
    obj = TestObject::New(&mock);
}

TEST(ObjectTest, test_replace_and_compare_operator) {

    TestObject::Ptr obj(TestObject::New());
    TestObject::Ptr obj0 = obj;
    TestObject::Ptr obj1 = obj.get();

    ASSERT_EQ(obj, obj0);
    ASSERT_EQ(obj, obj1);
    ASSERT_EQ(obj0, obj1);
    ASSERT_TRUE(obj == obj0);
    ASSERT_TRUE(obj == obj1);
    ASSERT_TRUE(obj0 == obj1);
}

TEST(ObjectTest, test_object_ptr_validation_as_container_hash_key) {

    LifeCycleMock mock;
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

TEST(ObjectTest, test_lock_and_expired_of_object_weak) {

    LifeCycleMock mock;
    EXPECT_CALL(mock, constructed());
    EXPECT_CALL(mock, destructed());

    TestObject::Weak weak;

    {
        TestObject::Ptr obj(TestObject::New(&mock));
        weak = obj;

        ASSERT_EQ(obj, weak.Lock());
        ASSERT_NE(nullptr, weak.Lock());
        ASSERT_FALSE(weak.Expired());
    }

    ASSERT_EQ(nullptr, weak.Lock());
    ASSERT_TRUE(weak.Expired());
}

TEST(ObjectTest, test_circular_with_object_weak) {

    LifeCycleMock mock;
    EXPECT_CALL(mock, constructed()).Times(2);
    EXPECT_CALL(mock, destructed()).Times(2);

    TestObject::Ptr obj0(TestObject::New(&mock));
    TestObject::Ptr obj1(TestObject::New(&mock));

    obj0->set_other(obj1);
    obj1->set_other(obj0);
}


// TODO(ghilbut): how can I test CountHelper template life-cycle?


}  // namespace codebase
