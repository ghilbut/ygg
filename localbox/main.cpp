#include "box.h"
#include "impl/box_desc.h"
#include <boost/filesystem.hpp>
#include <boost/thread.hpp>
#include <json/json.h>


using namespace boost::filesystem;


const Json::Value ReadSettings(const char* filepath);


int main(int argc, char **argv) {

	if (argc < 2) {
		printf("Usage: %s [root_dir]\n", argv[0]);
		return -1;
	}

	const path root(canonical(argv[1]));
	const path user_file(root / "settings.json");
	const path default_file(root / "settings.json.default");

	path target_file;
	if (exists(user_file)) {
		target_file = user_file;
	}
	else if (exists(default_file)) {
		target_file = default_file;
	}
	else {
		printf("Could not found %s file.\n", default_file.string().c_str());
		return -1;
	}

	const Json::Value settings = ReadSettings(target_file.string().c_str());
	if (settings.isNull()) {
		return -1;
	}

	Json::Value admin = settings.get("admin", Json::nullValue);
	if (admin.isNull()) {
		return -1;
	}

	Json::Value user_list = settings.get("user_list", Json::nullValue);
	if (user_list.isNull()) {
		return -1;
	}

	try {
		IOService io_service;

		Json::FastWriter writer;
		std::string json = writer.write(settings);

		BoxDesc desc(json);
		if (desc.IsEmpty()) {
			printf("box construction is failed.");
			return -1;
		}

		Box box(io_service, desc);

		boost::thread t(boost::bind(&boost::asio::io_service::run, &io_service));

		char line[1024 + 1];
		while (std::cin.getline(line, 1024 + 1)) {
			box.Write(line);
		}

		box.Close();
		t.join();
	}
	catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << "\n";
	}

    return 0;
}



const Json::Value ReadSettings(const char* filepath) {

	FILE *fp = fopen(filepath, "r");
	if (!fp) {
		// TODO(ghilbut): [2015-01-03] use and pass system error class
		printf("Could not open %s file.\n", filepath);
		return Json::nullValue;
	}

	std::string json;

	size_t size;
	char buffer[1024];
	do {
		size = sizeof(buffer);
		size = fread(buffer, sizeof(char), size, fp);
		json.append(buffer, buffer + size);
	} while (size > 0);

	fclose(fp);


	Json::Value root;
	Json::Reader reader;
	if (!reader.parse(json, root, false)) {
		// TODO(ghilbut): [2015-01-03] use and pass system error class
		printf("%s is invalid json format file.\n", filepath);
		return Json::nullValue;
	}
	if (!root.isObject()) {
		printf("%s is invalid setting format file.\n", filepath);
		return Json::nullValue;
	}

	return root;
}