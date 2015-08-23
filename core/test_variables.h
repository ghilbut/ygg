#ifndef _YGG_CORE_TEST_VARIABLES_H_
#define _YGG_CORE_TEST_VARIABLES_H_

#include <json/json.h>


namespace core {


static std::string InitCtrlJson(); 
static std::string InitTargetJson(); 

static const std::string kCtrlJson(InitCtrlJson());
static const std::string kTargetJson(InitTargetJson());



std::string InitCtrlJson() {

    Json::Value root(Json::objectValue);
    root["user"] = "A";
    root["endpoint"] = "B";

    Json::FastWriter w;
    return w.write(root);
}

std::string InitTargetJson() {

    Json::Value root(Json::objectValue);
    root["endpoint"] = "B";

    Json::FastWriter w;
    return w.write(root);
}


}  // namespace core
 

#endif  //  _YGG_CORE_TEST_VARIABLES_H_
