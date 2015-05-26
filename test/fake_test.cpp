#include <gmock/gmock.h>

#include "fake.h"
#include "mock.h"


namespace test {


TEST(FakeTest, test_life_cycle) {

    LifeCycleMock mock;
    EXPECT_CALL(mock, constructed()).Times(6);
    EXPECT_CALL(mock, destructed()).Times(6);

    {
        Connection::Ptr conn(new FakeConnection(&mock));
        Session::Ptr session(new FakeSession(&mock));
    }

    {
        Connection::Ptr conn(new FakeConnection(&mock));
        Session::Ptr session(new FakeSession(conn.get(), &mock));
    }

    {
        Session::Ptr session(new FakeSession(&mock));
        Connection::Ptr conn(new FakeConnection(session.get(), &mock));
    }
}

TEST(FakeTest, test_pipe_of_fake_connection_and_fake_session) {

    {
        FakeConnection * fake = new FakeConnection();
        Connection::Ptr conn(fake);
        Session::Ptr session(fake->session());
    }

    {
        FakeSession * fake = new FakeSession();
        Session::Ptr session(fake);
        Connection::Ptr conn(fake->conn());
    }

}


}  // namespace test
