#include <json/json.h>
#include <cstdio>

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


    for (int i = 0, len = root.size(); i < len; ++i) {
        const Json::Value& item = root[i];
        const Json::Value& cloud = item["cloud"];

        printf("----------------------------------------------------------------\n");
        printf("id %s\n", item["id"]);
        printf("cloud::host %s\n", cloud["host"].asCString());
        printf("cloud::port %d\n", cloud["port"].asInt());
    }
    printf("----------------------------------------------------------------\n");

    return 0;
}