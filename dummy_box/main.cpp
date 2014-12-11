#include "box.h"
#include "impl/box_desc.h"
#include <json/json.h>
#include <vector>
#include <cstdio>


#include <boost/thread.hpp>

int main(int argc, char** argv) {

    if (argc < 2) {
        printf("Usage: %s [setting_file_path]\n", argv[0]);
        return -1;
    }

    const char* filepath = argv[1];
    FILE* fp = fopen(filepath, "r");
    if (fp ==NULL) {
        printf("can not open file path - \"%s\".\n", filepath);
        return -1;
    }

    std::string json;
    size_t read_bytes;
    static const int kBufferSize = 1024;
    char buffer[kBufferSize];
    do {
        read_bytes = fread(buffer, sizeof(char), 1024, fp);
        json.insert(json.end(), buffer, buffer + read_bytes);
    } while (kBufferSize == read_bytes);
    fclose(fp);

    Json::Value root;
    Json::Reader reader;
    if (!reader.parse(json, root, false)) {
        printf("invalid json format - \"%s\".\n", filepath);
        return -1;
    }

    try
    {
        IOService io_service;

        std::vector<Box> box_list;
        const Json::Value items = root.get("items", Json::Value::null);
        for (int i = 0, len = items.size(); i < len; ++i) {

            Json::FastWriter writer;
            std::string json = writer.write(items[i]);

            BoxDesc desc(json);
            if (!desc.IsEmpty()) {
                box_list.push_back(Box(io_service, desc));
            }
        }

        boost::thread t(boost::bind(&boost::asio::io_service::run, &io_service));

        char line[1024 + 1];
        while (std::cin.getline(line, 1024 + 1)) {
            box_list[0].Write(line);
        }

        auto itr = box_list.begin();
        auto end = box_list.end();
        for (; itr != end; ++itr) {
            itr->Close();
        }
        t.join();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}