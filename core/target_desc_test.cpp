#include <gmock/gmock.h>
#include "test_variables.h"
#include "target_desc.h"


using namespace ygg::core;


TEST(TargetDescTest, return_null_when_string_is_empty) {

    const auto desc = TargetDesc::New("");
    ASSERT_EQ(nullptr, desc);
}

TEST(TargetDescTest, return_null_when_string_is_invalid_json) {

    const auto b = kTargetJson.begin();
    const auto e = b + (kTargetJson.length() / 2);
    const std::string json(b, e);

    const auto desc = TargetDesc::New(json);
    ASSERT_EQ(nullptr, desc);
}

TEST(TargetDescTest, return_null_when_endpoint_value_is_not_exists) {

    Json::Reader r;
    Json::Value root;
    ASSERT_TRUE(r.parse(kTargetJson, root));

    Json::FastWriter w;
    root.removeMember("endpoint");
    const std::string json(w.write(root));

    const auto desc = TargetDesc::New(json);
    ASSERT_EQ(nullptr, desc);
}

TEST(TargetDescTest, return_object_when_json_is_valid) {

    const auto desc = TargetDesc::New(kTargetJson);
    ASSERT_EQ(kTargetJson, desc->json);
    ASSERT_EQ("B", desc->endpoint);
}
