#include <gmock/gmock.h>
#include "box_info.h"


namespace box {


class BoxReadyTest : public ::testing::Test {
};


TEST_F(BoxReadyTest, test_box_info_return_null_with_invalid_json_format) {

    const std::string json = "012345678";
    BoxInfo * info = BoxInfo::New(json);

    ASSERT_TRUE(info == nullptr);
}

TEST_F(BoxReadyTest, test_box_info_return_object_with_valid_json_data) {

    const std::string json = "{ \"box-id\": \"box00\" }";
    BoxInfo * info = BoxInfo::New(json);

    ASSERT_TRUE(info != nullptr);
    ASSERT_STREQ("box00", info->id());
}



}  // namespace box
