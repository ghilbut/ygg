#ifndef YGG_DUMMY_BOX_OBJECT_REF_H_
#define YGG_DUMMY_BOX_OBJECT_REF_H_


namespace codebase {

class Object;

class ObjectRef {
public:
    virtual ObjectRef& operator= (ObjectRef& other);


protected:
    ObjectRef();
    explicit ObjectRef(Object* object);
    explicit ObjectRef(ObjectRef& other);
    virtual ~ObjectRef();

    template<typename T>
    T* Get() const {
        return static_cast<T*>(object_);
    }

private:
    Object* object_;
};

}  // namespace codebase

#endif  // YGG_DUMMY_BOX_OBJECT_REF_H_