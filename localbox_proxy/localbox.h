#ifndef LOCALBOX_PROXY_LOCALBOX_H_
#define LOCALBOX_PROXY_LOCALBOX_H_

#include "localbox_desc.h"
#include <boost/shared_ptr.hpp>
#include <string>


class HttpWebsocket;

class LocalBox {
public:
    LocalBox() {}
    ~LocalBox() {}

    explicit LocalBox(const LocalBox& other);
    LocalBox(const std::string& json, HttpWebsocket &ws);

    LocalBox& operator= (const LocalBox& other);
    bool operator== (const LocalBox& other) const;
    bool operator!= (const LocalBox& other) const;

    bool IsNull() const;


private:
    class Impl;
    boost::shared_ptr<Impl> impl_;
};

#endif  // LOCALBOX_PROXY_LOCALBOX_H_