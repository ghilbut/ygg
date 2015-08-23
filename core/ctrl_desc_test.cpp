#include <gmock/gmock.h>
#include "test_variables.h"
#include "ctrl_desc.h"


using namespace core;


TEST(CtrlDescTest, return_null_when_string_is_empty) {

    const auto desc = CtrlDesc::New("");
    ASSERT_EQ(nullptr, desc);
}

TEST(CtrlDescTest, return_null_when_string_is_invalid_json) {

    const auto b = kCtrlJson.begin();
    const auto e = b + (kCtrlJson.length() / 2);
    const std::string json(b, e);

    const auto desc = CtrlDesc::New(json);
    ASSERT_EQ(nullptr, desc);
}

TEST(CtrlDescTest, return_null_when_user_value_is_not_exists) {

    Json::Reader r;
    Json::Value root;
    ASSERT_TRUE(r.parse(kCtrlJson, root));

    Json::FastWriter w;
    root.removeMember("user");
    const std::string json(w.write(root));

    const auto desc = CtrlDesc::New(json);
    ASSERT_EQ(nullptr, desc);
}

TEST(CtrlDescTest, return_null_when_endpoint_value_is_not_exists) {

    Json::Reader r;
    Json::Value root;
    ASSERT_TRUE(r.parse(kCtrlJson, root));

    Json::FastWriter w;
    root.removeMember("endpoint");
    const std::string json(w.write(root));

    const auto desc = CtrlDesc::New(json);
    ASSERT_EQ(nullptr, desc);
}

TEST(CtrlDescTest, return_object_when_json_is_valid) {

    const auto desc = CtrlDesc::New(kCtrlJson);
    ASSERT_EQ(kCtrlJson, desc->json);
    ASSERT_EQ("A", desc->user);
    ASSERT_EQ("B", desc->endpoint);
}
